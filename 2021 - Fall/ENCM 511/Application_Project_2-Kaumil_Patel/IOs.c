/* 
 * File:   IOs.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 6, 2021, 1:36 PM
 */

#include <xc.h>

#include "IOs.h"
#include "TimeDelay.h"
#include "config.h"
#include "ADC.h"
#include "CTMU.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"

int RA2flag = 0; //Flag signifying button connected to RA2 pin has been pressed.
int RA4flag = 0; //Flag signifying button connected to RA4 pin has been pressed.
int RB4flag = 0; //Flag signifying button connected to RB4 pin has been pressed.

int counter = 0;
int frequencyFlag = 0;

int refSwitch = 0;
    
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    if(IFS1bits.CNIF == 1) {
        
        if(PORTBbits.RB7 == 1 && T2CONbits.TON == 1){                           // if timer is on and measuring frequency
            counter++;                                                          // increment counter
            IFS1bits.CNIF = 0; // clear IF flag                                 // clear interrupt
            return;
        }else if(PORTBbits.RB7 == 1){                                           // if timer is off and frequency is incoming then measure it
            frequencyFlag = 1;
        }
        
        if(PORTAbits.RA2 == 0){ // if RA2 id pushed down
            RA2flag = 1; // user defined global variable used as flag set to 1
        }else{
            RA2flag = 0;
        }
        
        if (PORTAbits.RA4 == 0){ // if RA4 id pushed down
            RA4flag = 1; // user defined global variable used as flag set to 1
        }else{
            RA4flag = 0;
        }
        
        if (PORTBbits.RB4 == 0){ // if RB4 id pushed down
            RB4flag = 1; // user defined global variable used as flag set to 1
        }else{
            RB4flag = 0;
        }
    }
    IFS1bits.CNIF = 0; // clear IF flag
    Nop();
}

// RB12 used for capacitance measurement

void IOinit(){
    //These three inputs are each connected to a different switch
    TRISAbits.TRISA2 = 1; //Set A2 as input
    TRISAbits.TRISA4 = 1; //Set A4 as input
    TRISBbits.TRISB4 = 1; //Set B4 as input
    
    // used for frequency measurements
    TRISBbits.TRISB7 = 1; //Set B7 as input
    
    CNPU2bits.CN30PUE = 1; //Enable pull up for A2
    CNPU1bits.CN0PUE = 1;  //Enable pull up for A4
    CNPU1bits.CN1PUE = 1;  //Enable pull up for B4
    
    //Initialize CN Interrupt
    CNEN1bits.CN0IE = 1;     //Enable CN0 interrupt
    CNEN1bits.CN1IE = 1;     //enable CN1 interrupt
    CNEN2bits.CN30IE = 1;    //enable CN30 interrupt
    
    CNEN2bits.CN23IE = 1;    //enable CN30 1interrupt
    
    IPC4bits.CNIP = 5;       //set CN interrupt priority
    IFS1bits.CNIF = 0;       //clears the CN interrupt flag
    IEC1bits.CNIE = 1;       //enable the CN interrupt (general)
    
    TRISBbits.TRISB8 = 0; //Set B8 as output, turns on LED when 1
}

void IOcheck(){
    // Vdd = 3.43v
    while(1){
        if(RA2flag == 1){                                                       // if RA2 flag is enabled
            LATBbits.LATB8 = 1;
            Disp2String("VOLTMETER Voltage =");
            ADCswitchVoltage();                                                 // Enable ADC for voltage
            Disp2Dec(1*do_ADC()*3.43/1023);                                     // Calculate and display voltage
            Disp2String("V    \r");
        }else if(RA4flag == 1){                                                 // if RA4 flag is enabled
            LATBbits.LATB8 = 1;
            Disp2String("OHMMETER Resistance =");
            ADCswitchResistor();                                                // Enable ADC for resistor
            float voltage = do_ADC()*3.43/1023;                                 // Calculate  voltage
            Disp2Dec(1000*voltage/(3.43-voltage));                              // Calculate and display resistance
            Disp2String("Ohm\r");
        }else if(RB4flag == 1){                                                 // if RB4 flag is enabled
            LATBbits.LATB8 = 1;
            Disp2String("Capacitance = ");
            int capacitance = ctmu();                                           // Use CTMU
            Disp2Dec(capacitance);
            Disp2String("uF        \r");
        } else {
            if(frequencyFlag == 1){                                             // if frequency is registered
                LATBbits.LATB8 = 1;
                frequencyFlag = 0;
                if(T2CONbits.TON == 0){                                         // if timer is off enable it
                    Delay_ms(1000, 0);
                    counter = 0;
                    while(T2CONbits.TON == 1){                                  // measure frequency
                        Idle();
                    }
                    Disp2String("Frequency =");
                    Disp2Dec(counter);
                    Disp2String("Hz           \r");
                }
                if(refSwitch == 0){                                             // ALTERNATE REF CLOCK  
                    REFOCONbits.ROEN = 0;
                    REFOCONbits.RODIV = 0b1001;                                 // 512 - ref clock = 63 Hz
                    REFOCONbits.ROEN = 1;
                    refSwitch = 1;
                }else{
                    REFOCONbits.ROEN = 0;
                    REFOCONbits.RODIV = 0b1000;                                 // 256 - ref clock = 125 Hz
                    REFOCONbits.ROEN = 1;
                    refSwitch = 0;
                }
            }else{                                                              // if nothing pressed and no incoming frequency then IDLE
                LATBbits.LATB8 = 0;
                Disp2String("Idle Mode                      \r");
                Idle();
            }
        }
    }
}
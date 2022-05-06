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

#include "string.h"
#include "UART2.h"

int RA2flag = 0; //Flag signifying button connected to RA2 pin has been pressed.
int RA4flag = 0; //Flag signifying button connected to RA4 pin has been pressed.
int RB4flag = 0; //Flag signifying button connected to RB4 pin has been pressed.

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    if(IFS1bits.CNIF == 1) {
        if(PORTAbits.RA2 == 0){ // if RA2 id pushed down
            RA2flag = 1; // user defined global variable used as flag set to 1
        }else{
            RA2flag = 0; // user defined global variable used as flag set to 0
        }
        if (PORTAbits.RA4 == 0){ // if RA4 id pushed down
            RA4flag = 1; // user defined global variable used as flag set to 1
        }else{
            RA4flag = 0; // user defined global variable used as flag set to 0
        }
        if (PORTBbits.RB4 == 0){ // if RB4 id pushed down
            RB4flag = 1; // user defined global variable used as flag set to 1
        }else{
            RB4flag = 0; // user defined global variable used as flag set to 0
        }
    }
    IFS1bits.CNIF = 0; // clear IF flag
    Nop();
}

void IOinit(){
    //These three inputs are each connected to a different switch
    TRISAbits.TRISA2 = 1; //Set A2 as input
    TRISAbits.TRISA4 = 1; //Set A4 as input
    TRISBbits.TRISB4 = 1; //Set B4 as input
    
    CNPU2bits.CN30PUE = 1; //Enable pull up forA2
    CNPU1bits.CN0PUE = 1; //Enable pull up for A4
    CNPU1bits.CN1PUE = 1; //Enable pull up for B4
    
    //Initialize CN Interrupt
    CNEN1bits.CN0IE = 1;    //Enable CN0 interrupt
    CNEN1bits.CN1IE = 1;    //enable CN1 interrupt
    CNEN2bits.CN30IE = 1;    //enable CN30 interrupt
    IPC4bits.CNIP = 5;      //set CN interrupt priority
    IFS1bits.CNIF = 0;      //clears the CN interrupt flag
    IEC1bits.CNIE = 1;      //enable the CN interrupt (general)
    
    TRISBbits.TRISB8 = 0; //Set B8 as output, turns on LED when 1
}

void IOcheck(){
    while(1){
        if(RA2flag == 1 && RA4flag == 1 && RB4flag == 1){                       // If All switches are pressed.
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("All PBs pressed        \r");                           // Display "All PBs pressed\r"
            
        }else if(RA2flag == 1 && RA4flag == 1){                                 // If RA2 and RA4 switches are pressed.
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB1 and PB2 are pressed\r");                           // Display "PB1 and PB2 are pressed\r"
            
        }else if(RA2flag == 1 && RB4flag == 1){                                 // If RA2 and RB4 switches are pressed.
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB1 and PB3 are pressed\r");                           // Display "PB1 and PB3 are pressed\r"
            
        }else if(RA4flag == 1 && RB4flag == 1){                                 // If RA4 and RB4 switches are pressed.
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB2 and PB3 are pressed\r");                           // Display "PB2 and PB3 are pressed\r"
            
        }else if(RA2flag == 1){                                                 // if RA2 flag is enabled
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB1 is pressed    ");                                  // Display "PB1 is pressed\r"
            Delay_ms(20);                                                       // Wait for 0.02 second
            LATBbits.LATB8 = 0;                                                 // Turn LED off
            Disp2String("     \r");                                             // Display "PB1 is pressed\r"
            Delay_ms(340);                                                      // Wait for 0.340 second
            RA2flag = 0;                                                        // set RA2 Flag 0
            
        }else if(RA4flag == 1){                                                 // if RA4 flag is enabled
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB2 is pressed         \r");                           // Display "PB2 is pressed\r"
            Delay_ms(1360);                                                     // Wait for 1.36 seconds
            LATBbits.LATB8 = 0;                                                 // Turn LED off
            Delay_ms(2000);                                                     // Wait for 2 seconds    
            RA4flag = 0;                                                        // set RA4 Flag 0
            
        }else if(RB4flag == 1){                                                 // if RB4 flag is enabled
            LATBbits.LATB8 = 1;                                                 // Turn LED on
            Disp2String("PB3 is pressed         \r");                           // Display "PB3 is pressed\r"
            Delay_ms(2360);                                                     // Wait for 2.36 seconds
            LATBbits.LATB8 = 0;                                                 // Turn LED off
            Delay_ms(3000);                                                     // Wait for 3 seconds    
            RB4flag = 0;                                                        // set RB4 Flag 0
            
        } else {
            LATBbits.LATB8 = 0;                                                 // Turn LED off
            Disp2String("Nothing pressed        \r");                           // Display "Nothing pressed\r"
        }
    }
}
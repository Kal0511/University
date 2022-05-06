/*
 * File:   main.c
 * Author: Ryan Sommerville && Kaumil Patel
 *
 * Created on November 21, 2021, 11:41 AM
 */


#include "xc.h"
#include "ADC.h"
#include "config.h"
#include "UART2.h"
#include "IOs.h"

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);
/*
 * An interrupt service routine for the input and output pins.
 * Promises:
 *  Triggers if there is a change in the input from pins A2, A4, or B4.
 *  Sets flags accordingly:
 *      If RA2 is 0, sets RA2flag to 1.
 *      If RA4 is 0, sets RA4flag to 1.
 *      If RB4 is 0, sets RB4flag to 1.
 */

int buttonPressed = 0;

int main(void) {
    hardwareConfig();
    //NewClk(32);
    InitUART2();
    ADCinit();
    IOinit();
    
    Disp2String("\n\n\rProgram Starting. Press button to measure input.");
    while(1)
    {
        Idle();
        if(buttonPressed)
        {
            Disp2String("\r\n\nButton has been pressed. Beginning to measure input.");
            IEC1bits.CNIE = 0;      //disable the CN interrupt (general)
            IEC0bits.AD1IE = 1; //Enable ADC interrupts
            ADC_measure();            
            IEC0bits.AD1IE = 0; //Disable ADC interrupts
            IEC1bits.CNIE = 1; //Enable CN interrupts
            buttonPressed = 0;
        }
        Disp2String("\r\nInput has been measured and displayed. Program going into idle mode.\n");
    }
    return 0;
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    buttonPressed = 1;
    IFS1bits.CNIF = 0; // clear IF flag
    Nop();
}
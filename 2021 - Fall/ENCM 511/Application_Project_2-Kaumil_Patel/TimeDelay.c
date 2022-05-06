/* 
 * File:   TimeDelay.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 6, 2021, 1:36 PM
 */

#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "config.h"


void Delay_ms(uint16_t time_ms, int idleFlag){
    // T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    // T2CONbits.TGATE = 0;
    
    //T2CON prescaler select, set prescaler to 1:256
    T2CONbits.TCKPS0 = 0;
    T2CONbits.TCKPS1 = 0;
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 3;  //Set interrupt priority to level 3 (7 is highest)
    IEC0bits.T2IE = 1;  //enable timer interrupt
    IFS0bits.T2IF = 0;  // Clear timer 2 flag
   
    
    PR2 = time_ms << 4;    //PR2 stores the target to trigger T2 interrupt
    TMR2 = 0;           //zero TMR2 register to start
    T2CONbits.TON = 1;  //start timer 2
    
    if(idleFlag == 1){
        Idle();
    }
    
    return;
}

/*
 *  Timer 2 Interrupt Service Routine
 *  Inputs: nothing
 *  Return: nothing
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){    
    T2CONbits.TON = 0;
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    TMR2 = 0;
    return;
}
/* 
 * File:   IOs.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 6, 2021, 1:36 PM
 */

#include <xc.h>

#include "IOs.h"
#include "config.h"

#include "string.h"
#include "UART2.h"





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
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


void IOinit(){
    
    //These three inputs are each connected to a different switch
    TRISAbits.TRISA2 = 1; //Set A2 as input
    TRISAbits.TRISA4 = 1; //Set A4 as input
    TRISBbits.TRISB4 = 1; //Set B4 as input
    
    CNPU2bits.CN30PUE = 1; //Enable pull up forA2
    CNPU1bits.CN0PUE = 1; //Enable pull up for A4
    CNPU1bits.CN1PUE = 1; //Enable pull up for B4
    
    TRISBbits.TRISB8 = 0; //Set B8 as output, turns on LED when 1
}

void IOcheck(){
    while(1){
        if(PORTAbits.RA2 == 0){
            if(PORTAbits.RA4 == 0 || PORTBbits.RB4 == 0){ //If either of other switches are pressed.
                LATBbits.LATB8 = 1; //Turn LED on
            }else{
                LATBbits.LATB8 = 1; //Turn LED on
                Delay_ms(1000); // Wait for 1 second
                LATBbits.LATB8 = 0; // Turn LED off
                Delay_ms(1000); //Wait for one second
            }
        }else if(PORTAbits.RA4 ==0){
            if(PORTAbits.RA2 == 0 || PORTBbits.RB4 == 0){ //If either of other switches are pressed.
                LATBbits.LATB8 = 1; //Turn LED on
            }else{
                LATBbits.LATB8 = 1; //Turn LED on
                Delay_ms(2000); //Wait for 2 seconds
                LATBbits.LATB8 = 0; //Turn LED off
                Delay_ms(2000);   //Wait for 2 seconds         
            }
        }else if(PORTBbits.RB4 ==0){
            if(PORTAbits.RA2 == 0 || PORTAbits.RA4 == 0){ //If either of other switches are pressed.
                LATBbits.LATB8 = 1; //Turn LED on
            }else{
                LATBbits.LATB8 = 1; //Turn LED on
                Delay_ms(3000);//Wait for 3 seconds
                LATBbits.LATB8 = 0; //Turn LED off
                Delay_ms(3000);     //Wait for 3 seconds       
            }
        } else {
            LATBbits.LATB8 = 0; //Turn LED off
        }
    }
}
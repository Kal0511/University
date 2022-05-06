/* 
 * File:   main.c
 * Author: Kaumil Patel
 *
 * Created on September 26, 2021, 10:32 AM
 */

#include <xc.h>
#include <p24F16KA101.h>


// RA6     RA2     RA1     RA0     Number of LEDs on
// 0       0       0       0       0 - None 
// 0       0       0       1       3 
// 0       0       1       0       0 - None  
// 0       0       1       1       0 - None  
// 0       1       0       0       8 
// 0       1       0       1       8 
// 0       1       1       0       0 - None  
// 0       1       1       1       9 
// 1       0       0       0       6 
// 1       0       0       1       0 - None  
// 1       0       1       0       0 - None 
// 1       0       1       1       0 - None 
// 1       1       0       0       0 - None 
// 1       1       0       1       0 - None 
// 1       1       1       0       0 - None
// 1       1       1       1       0 - None 


void main(void) {
    // setting 4 IO pins A0, A1, A2, and A6 to be used as input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA6 = 1;
    
    // setting 9 IO pins B0, B1, B2, B4, B7, B8, B9, B12, and B13 to be used as output
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;

    // entering main loop which runs the program forever
    while(1){
        
        // for input - 0001      output - 3 LEDs
        if(PORTAbits.RA6 == 0 && PORTAbits.RA2 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA0 == 1){
            
            // set 3 LEDs on
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            
            // set other 6 LEDs off
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
            LATBbits.LATB13 = 0;
            
        // for input - 0100 or 0101       output - 8 LEDs
        }else if (  (PORTAbits.RA6 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA1 == 0 && PORTAbits.RA0 == 0) ||
                    (PORTAbits.RA6 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA1 == 0 && PORTAbits.RA0 == 1) ){
            
            // set 8 LEDs on
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            LATBbits.LATB9 = 1;
            LATBbits.LATB12 = 1;
            
            // set other 1 LED off
            LATBbits.LATB13 = 0;
            
        // for input - 0111      output - 9 LEDs
        }else if (PORTAbits.RA6 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA1 == 1 && PORTAbits.RA0 == 1){
            
            // set all 9 LEDs on
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            LATBbits.LATB9 = 1;
            LATBbits.LATB12 = 1;
            LATBbits.LATB13 = 1;
            
        // for input - 1000      output - 6 LEDs
        }else if (PORTAbits.RA6 == 1 && PORTAbits.RA2 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA0 == 0){
            // set 6 LEDs on
            LATBbits.LATB0 = 1;
            LATBbits.LATB1 = 1;
            LATBbits.LATB2 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB7 = 1;
            LATBbits.LATB8 = 1;
            
            // set other 3 LED off
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
            LATBbits.LATB13 = 0;
            
            // for input - 0000 or 1001 to 1111     output - 0 LEDs
        }else{
            
            // set all 9 LEDs off
            LATBbits.LATB0 = 0;
            LATBbits.LATB1 = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB7 = 0;
            LATBbits.LATB8 = 0;
            LATBbits.LATB9 = 0;
            LATBbits.LATB12 = 0;
            LATBbits.LATB13 = 0;
        }
    }
}


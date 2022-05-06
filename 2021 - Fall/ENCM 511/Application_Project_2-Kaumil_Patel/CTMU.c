/*
 * File:   CTMU.c
 * Author: Kaumil P
 *
 * Created on December 6, 2021, 4:47 PM
 */


#include "xc.h"

void setup(void){ 
    CTMUCON = 0x1090; //make sure CTMU is disabled
    CTMUCONbits.TGEN = 0;
    CTMUICON = 0x0100; 
    CTMUICONbits.IRNG0 = 1; // 55uA, Nominal - No Adjustment
    CTMUICONbits.IRNG1 = 1; // 55uA, Nominal - No Adjustment

    AD1CON1bits.ADON = 0;
    AD1CHSbits.CH0SA=0b1100;    
    TRISBbits.TRISB12 = 1;  // RB 12 used
    AD1PCFGbits.PCFG12 = 1;     

    AD1CSSL=0x0000; //
    AD1CON1 = 0x8000; // Turn On A/D Converter, continue in Idle mode,
    AD1CON2 = 0x0000; // VR+ = AVDD, V- = AVSS, Don't scan,
    AD1CON3 = 0x0000; // A/D uses system clock, conversion clock = 1xTcy
}



int ctmu(){
    int voltage; //storage for reading
    setup();
    CTMUCONbits.CTMUEN = 1; //Enable the CTMU
    AD1CON1bits.SAMP = 1; //Manual sampling start
    CTMUCONbits.IDISSEN = 1; //drain charge on the circuit

    IEC1bits.CNIE = 0; 
    Delay_ms(1000,1); //wait 1 sec

    CTMUCONbits.IDISSEN = 0; //end drain of circuit
    CTMUCONbits.EDG1STAT = 1; //Begin charging the circuit
    //using CTMU current source
    Delay_ms(500,1);  //wait 0.5 sec
    IEC1bits.CNIE = 1; 

    CTMUCONbits.EDG1STAT = 0; //Stop charging circuit
    IFS0bits.AD1IF = 0; //make sure A/D Int not set
    AD1CON1bits.SAMP = 0; //and begin A/D conv.
    while(!IFS0bits.AD1IF); //Wait for A/D convert complete
    AD1CON1bits.DONE = 0;
    voltage = ADC1BUF0; //Get the value from the A/D
    float ans = 54*(0.5+0.08)/((voltage*3.37)/1023); // 0.8 is circuit offset
    AD1CON1bits.ADON = 0;
    CTMUCONbits.CTMUEN = 0;
    return (int)ans;
}
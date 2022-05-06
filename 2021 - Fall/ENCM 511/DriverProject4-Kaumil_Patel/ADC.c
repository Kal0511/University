/*
 * File:   ADC.c
 * Author: Ryan Sommerville && Kaumil Patel
 *
 * Created on November 21, 2021, 11:36 AM
 */


#include "xc.h"
#include "ADC.h"
#include "UART2.h"
#include <stdio.h>

uint16_t ADC_buffer [8];
int samplingComplete = 0;

uint16_t do_ADC(void){
    uint16_t sum = 0;
    int counter = 0; //Counts number of times that have been looped.
    LATBbits.LATB8 = 1; //Turn on LED at RB8
    
    while(counter < 1000)
    {
        IFS0bits.AD1IF = 0; //Reset Interrupt flag
        samplingComplete = 0; //Reset sampling global flag
        AD1CON1bits.ASAM = 1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
        while(!samplingComplete){} //Ends when interrupt sets global flag to true
        AD1CON1bits.ASAM = 1; //Stop sampling
        if(AD1CON2bits.BUFS == 1)
        {
            ADC_buffer[0] = ADC1BUF0;
            ADC_buffer[1] = ADC1BUF1;
            ADC_buffer[2] = ADC1BUF2;
            ADC_buffer[3] = ADC1BUF3;
            ADC_buffer[4] = ADC1BUF4;
            ADC_buffer[5] = ADC1BUF5;
            ADC_buffer[6] = ADC1BUF6;
            ADC_buffer[7] = ADC1BUF7;
        }
        else
        {
            ADC_buffer[0] = ADC1BUF8;
            ADC_buffer[1] = ADC1BUF9;
            ADC_buffer[2] = ADC1BUFA;
            ADC_buffer[3] = ADC1BUFB;
            ADC_buffer[4] = ADC1BUFC;
            ADC_buffer[5] = ADC1BUFD;
            ADC_buffer[6] = ADC1BUFE;
            ADC_buffer[7] = ADC1BUFF;
        }
        int j = 0;
        while(j < 8)
        {
            sum += ADC_buffer[j];
            counter++;
            j++;
        }
    }    
    LATBbits.LATB8 = 0; //Turn LED on RB8 off.
    uint16_t average = sum/1000; //Average of 1000 measurements
    return average; //returns 10 bitADC output stored in ADC1BIF0 to calling function}
}

void ADCinit(void)
{
    AD1CON1bits.ADON = 0; // Disable ADC, turn on before sampling
    AD1CON1bits.ADSIDL = 0; //Enable ADC in idle mode.
    AD1CON1bits.FORM = 0b00; //Form of ADC input: 00(int), 01(signed int), 10(fractional), 11(signed fractional)
    AD1CON1bits.SSRC = 0b111; //Automatically start conversion when done sampling
    AD1CON1bits.ASAM = 0; //Wait until SAMP bit set to sample
    AD1CON1bits.SAMP = 0; //Set to 1 to start sampling.
    
    //Sampling time and Clk select.
    
    AD1CON2bits.VCFG = 0b000; //Use vdd and vss for vref
    AD1CON2bits.CSCNA = 0; // Don't scan inputs, use channel selected by CHOSA
    AD1CON2bits.SMPI = 0b0111; //Interrupts occur every 8 sample/convert seq.
    AD1CON2bits.BUFM = 1; //Configure buffer as 2 8 word buffers
    AD1CON2bits.ALTS = 0; //Use Mux A input Mux settings
    AD1CON3bits.ADRC = 0; //Use system clock
    AD1CON3bits.SAMC = 0b00001; //Sample time
    AD1CON3bits.ADCS = 0b00001011; //Conversion clock select bits.
    //TAD = (2/8MHz)*64 = 0.016 ms
    //SAMC = 31*0.016 ms = 0.496 ms
    //1 conversion in 12 TADs: conv = 0.192 ms
    //Full conversion = ~0.6 ms
    //250 Hz = 4 ms Period
    
    //Mux settings
    AD1CHSbits.CH0NB = 0; //CH0 MUXB -ve input is VR-
    AD1CHSbits.CH0SB = 0b0101; //Positive input is pin 8 for MUXB
    AD1CHSbits.CH0NA = 0; //CH0 MUXA -ve input is VR-
    AD1CHSbits.CH0SA = 0b0101; //Positive input is pin 8 for MUXA
    
    //IO port selection
    TRISAbits.TRISA3 = 1; //Set Pin 8 as input
    AD1PCFG = 0xFFFF; //Make all pins digital
    AD1PCFGbits.PCFG5 = 0;//Set Pin 8 as analog input for ADC
    AD1CSSL = 0; //Input Scan Disabled
    
    //INT bit setup
    IPC3bits.AD1IP = 5;
    
    AD1CON1bits.ADON = 1; //Turn on ADC
    
    return;
}

void ADC_measure(void)
{
    int i = 0;
    while(i < 10) //Loop through ten times.
    {
        uint16_t average = do_ADC();//Find average value of 1000 measurements made in a second.
        displayBar(average); //Display value in a bar graph.
        i++;  
    }
}

void displayBar(uint16_t value)
{
    Disp2String("\n\r");//Go to beginning of next line.
    XmitUART2('-', value*2); //Print bar
    Disp2Hex(value); //Print value in hexadecimal
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0; //Reset interrupt flag
    AD1CON1bits.ASAM = 0; //Stop sampling
    samplingComplete = 1; //Set sampling flag to true
    return;
}
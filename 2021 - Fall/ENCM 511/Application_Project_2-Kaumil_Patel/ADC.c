/*
 * File:   ADC.c
 * Author: spdmo
 *
 * Created on December 7, 2021, 12:06 PM
 */


#include "xc.h"

uint16_t do_ADC(void){
    IEC0bits.AD1IE = 1; //Enable ADC interrupts 
    LATBbits.LATB8 = 1; //Turn on LED at RB8
    IFS0bits.AD1IF = 0; //Reset Interrupt flag77
    AD1CON1bits.ASAM = 1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE==0){}
    AD1CON1bits.ASAM = 0; //Stop sampling
    LATBbits.LATB8 = 0; //Turn LED on RB8 off.
    IEC0bits.AD1IE = 0; //Enable ADC interrupts 
    AD1CON1bits.ADON = 0;
    return ADC1BUF0; //returns 10 bitADC output stored in ADC1BIF0 to calling function}
}

void ADCinit(void){
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

void ADCswitchResistor(void){
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
    
    //Mux settings
    AD1CHSbits.CH0NB = 0; //CH0 MUXB -ve input is VR-
    AD1CHSbits.CH0SB = 0b1011; //Positive input is pin 8 for MUXB
    AD1CHSbits.CH0NA = 0; //CH0 MUXA -ve input is VR-
    AD1CHSbits.CH0SA = 0b1011; //Positive input is pin 8 for MUXA
    
    //IO port selection
    TRISBbits.TRISB13 = 1; //Set Pin 8 as input
    AD1PCFG = 0xFFFF; //Make all pins digital
    AD1PCFGbits.PCFG11 = 0;//Set Pin 8 as analog input for ADC
    AD1CSSL = 0; //Input Scan Disabled
    
    //INT bit setup
    IPC3bits.AD1IP = 5;
    
    AD1CON1bits.ADON = 1; //Turn on ADC
    return;
}

void ADCswitchVoltage(void){
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

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0; //Reset interrupt flag
    AD1CON1bits.ASAM = 0; //Stop sampling
    return;
}

/*
 * File:   config.c
 * Author: Ryan Sommerville
 *
 * Created on November 21, 2021, 11:42 AM
 */


#include "xc.h"
#include "config.h"

// clkval = 8 8MHZ
// clkval = 500 500 kHz
// clkval = 32 32 kHz
void NewClk (unsigned int clkval) 
{
    char COSCNOSC;
    
    if (clkval == 8)    //8 MHZ
    {
        COSCNOSC = 0x00; 
    }
    else if (clkval == 500)    //500 kHZ
    {
        COSCNOSC = 0x66; 
    }
    else if (clkval == 32)    //32 kHZ
    {
        COSCNOSC = 0x55; 
    }
    else                      //default 32 kHZ
    {
        COSCNOSC = 0x55;
    }
    SRbits.IPL = 7;           //disable interrupts 
    CLKDIVbits.RCDIV = 0;     //turn clock division to 0
    __builtin_write_OSCCONH(COSCNOSC);  //write COSCNOSC to the high side of OSCON
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1; 
    while (OSCCONbits.OSWEN = 1)
    {}
    SRbits.IPL = 0;           //enable interrupts
}


void hardwareConfig(void)
{
    // PIC24F16KA101 Configuration Bit Settings

    // 'C' source line config statements

    // FBS
    #pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
    #pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

    // FGS
    #pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
    #pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

    // FOSCSEL
    #pragma config FNOSC = LPFRC            // Oscillator Select (500 kHz Low-Power FRC oscillator with divide-by-N (LPFRCDIV))
    #pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

    // FOSC
    #pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
    #pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
    #pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
    #pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
    #pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

    // FWDT
    #pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
    #pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
    #pragma config WINDIS = ON              // Windowed Watchdog Timer Disable bit (Windowed WDT enabled)
    #pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

    // FPOR
    #pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
    #pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
    #pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
    #pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
    #pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

    // FICD
    #pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

    // FDS
    #pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
    #pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
    #pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
    #pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
    #pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer Enable bit (DSWDT disabled)
}


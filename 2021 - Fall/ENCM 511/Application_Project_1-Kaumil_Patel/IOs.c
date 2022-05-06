/* 
 * File:   IOs.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 6, 2021, 1:36 PM
 */

//            LATBbits.LATB8 = 1;                                                 // Turn LED on
//            Disp2String("PB2 is pressed         \r");                           // Display "PB2 is pressed\r"
//            Delay_ms(1360);                                                     // Wait for 1.36 seconds
//            LATBbits.LATB8 = 0;                                                 // Turn LED off
//            Delay_ms(2000);                                                     // Wait for 2 seconds    
//            RA4flag = 0;                                                        // set RA4 Flag 0

#include <xc.h>

#include "IOs.h"
#include "TimeDelay.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART2.h"

int RA2flag = 0; //Flag signifying button connected to RA2 pin has been pressed.
int RA4flag = 0; //Flag signifying button connected to RA4 pin has been pressed.
int RB4flag = 0; //Flag signifying button connected to RB4 pin has been pressed.

int timerCompleteFlag = 0;
int stopwatchRunning = 0;
int stopwatchMode = 1;
int timerRunning = 0;
int reactionGameFlag = 0;
int gameResetFlag = 0;
    
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    if(IFS1bits.CNIF == 1) {
        if(RB4flag == 1 && PORTBbits.RB4 == 1 && timerCompleteFlag == 1){
            if(T2CONbits.TON == 1){
                timerCompleteFlag = 0;
                IFS0bits.T2IF = 1;
            } 
            RB4flag = 0;
        } else if(((RA4flag == 1 && PORTAbits.RA4 == 1) || (RA2flag == 1 && PORTAbits.RA2 == 1)) && timerCompleteFlag == 1){
            if(T2CONbits.TON == 1){
                timerCompleteFlag = 0;
                IFS0bits.T2IF = 1;
            }
            RA4flag = 0;
            RA2flag = 0;
        }
        
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
    CNPU1bits.CN0PUE = 1;  //Enable pull up for A4
    CNPU1bits.CN1PUE = 1;  //Enable pull up for B4
    
    //Initialize CN Interrupt
    CNEN1bits.CN0IE = 1;     //Enable CN0 interrupt
    CNEN1bits.CN1IE = 1;     //enable CN1 interrupt
    CNEN2bits.CN30IE = 1;    //enable CN30 interrupt
    IPC4bits.CNIP = 5;       //set CN interrupt priority
    IFS1bits.CNIF = 0;       //clears the CN interrupt flag
    IEC1bits.CNIE = 1;       //enable the CN interrupt (general)
    
    TRISBbits.TRISB8 = 0; //Set B8 as output, turns on LED when 1
}

void IOcheck(){
    int min = 0;
    int sec = 0;
    
    char time[] = "00m : 0000s\r";                                              
    
    sprintf(time, "%02dm : %02ds\r", min, sec);  
    Disp2String(time);
    
    while(1){
        if(RA2flag == 1 && RA4flag == 1){                                       // if RA2 and RA4 flag is enabled then Switch clock mode
            timerCompleteFlag = 1;
            Delay_ms(3000);
            if(timerCompleteFlag == 1){                                         // if more than 3 seconds 
                min = 0;
                sec = 0;
                stopwatchRunning = 0;
                Disp2String("Clock Mode: ");
                if(stopwatchMode == 1){                                         // if in stopwatch mode then switch to timer mode
                    stopwatchMode = 0;
                    Disp2String("Timer\r");
                    Delay_ms(500);
                    sprintf(time, "%02dm : %04ds", min, sec);  
                }else{                                                          // if in timer mode then switch to stopwatch mode
                    stopwatchMode = 1;
                    Disp2String("Stopwatch\r");
                    Delay_ms(500);
                    sprintf(time, "%02dm : %02ds", min, sec);  
                }
                Disp2String(time);
                Disp2String("            \r");
                timerCompleteFlag = 0;                                          // reset timerCompleteFlag
            }
        }else if(RA2flag == 1){                                                 // if RA2 flag is enabled
            if(reactionGameFlag == 1){                                          // PB1 is pressed to stop the timer in game mode
                if(timerRunning == 1){
                    timerRunning = 0;
                    LATBbits.LATB8 = 0; 
                    sprintf(time, "%02dm : %04ds\r", min, sec);  
                    Disp2String(time);
                }
                gameResetFlag = 1;
            }else if(stopwatchMode == 0){                                       // if in timer mode
                if(timerRunning == 1){                                          // if timer is running turn it off timer and led
                    timerRunning = 0;
                    LATBbits.LATB8 = 0; 
                }else{                                                          // if timer is off turn it on and turn on the led
                    timerRunning = 1;
                    LATBbits.LATB8 = 1; 
                }
            }else{                                                              // if in stopwatch mode then increment min and display it
                min++;
                if(min == 60){
                    min = 0;
                }
                sprintf(time, "%02dm : %02ds\r", min, sec);  
                Disp2String(time);
                Delay_ms(20);
            }
        }else if(RA4flag == 1){                                                 // if RA4 flag is enabled
            if(reactionGameFlag == 1){                                          // if in game mode then exit game
                reactionGameFlag = 0;
                timerRunning = 0;
                sec = 0;
                min = 0;
                sprintf(time, "%02dm : %04ds\r", min, sec);  
                Disp2String(time);
            }else if(stopwatchMode == 0){                                       // if in timer mode then reset timer
                sec = 0;
                min = 0;
                sprintf(time, "%02dm : %04ds\r", min, sec);  
                Disp2String(time);
            }else{                                                              // if in stopwatch mode then increment sec
                sec++;
                if(sec == 60){
                    sec = 0;
                }
                sprintf(time, "%02dm : %02ds\r", min, sec);  
                Disp2String(time);
                Delay_ms(20);
            }
        }else if(RB4flag == 1){                                                 // if RB4 flag is enabled
            if(reactionGameFlag == 1){                                          // if in game mode then reset game
                sec = 0;
                min = 0;
                timerRunning = 0;
                sprintf(time, "%02dm : %04ds\r", min, sec);  
                Disp2String(time);
                gameResetFlag = 0;
            }else if(stopwatchMode == 0){                                       // if in timer mode and timer is not running then enter game mode
                if(timerRunning == 0){
                    reactionGameFlag = 1;
                }
            }else{                                                              // if in stopwatch mode then wait delay
                timerCompleteFlag = 1;
                Delay_ms(3000);
                if(timerCompleteFlag == 1){                                     // if timer is complete then then reset stopwatch
                    min = 0;
                    sec = 0;
                    stopwatchRunning = 0;
                    sprintf(time, "%02dm : %02ds\r", min, sec);
                    Disp2String(time);
                    timerCompleteFlag = 0;
                }else if(stopwatchRunning == 1){                                // if delay is not complete then turn stopwatch on or off
                    stopwatchRunning = 0;
                    LATBbits.LATB8 = 0; 
                }else{
                    stopwatchRunning = 1;
                    LATBbits.LATB8 = 1; 
                }   
            }
            RB4flag = 0;                                                        // set RB4 Flag 0
        } else {
            if(reactionGameFlag == 1 && timerRunning == 0 && gameResetFlag == 0){   // start reaction game
                Disp2String("Game Started\r");
                Disp2String("            \r");
                sec = 0;
                min = 0;
                sprintf(time, "%02dm : %04ds\r", min, sec);  
                Disp2String(time);
                LATBbits.LATB8 = 1; 
                Delay_ms(1000);
                LATBbits.LATB8 = 0; 
                Delay_ms(1000);
                LATBbits.LATB8 = 1; 
                Delay_ms(1000);
                LATBbits.LATB8 = 0; 
                Delay_ms(1000);
                LATBbits.LATB8 = 1; 
                Delay_ms(1000);
                LATBbits.LATB8 = 0; 
                int wait = rand() % 2500 + 501;
                Delay_ms(wait);
                LATBbits.LATB8 = 1; 
                timerRunning = 1;
            }else if(timerRunning == 1){                                        // if in timer mode then increment timer
                sec++;
                if(sec == 60){
                    min++;
                    sec = 0;
                }
                sprintf(time, "%02dm : %04ds\r", min, sec);  
                Disp2String(time);
                Delay_ms(275);
            }else if(stopwatchRunning == 1){                                    // if in stopwatch mode then increment stopwatch
                
                if(min == 0 && sec == 0){                            // if stopwatch over case
                    LATBbits.LATB8 = 1; 
                    sprintf(time, "%02dm : %02ds", min, sec);
                    Disp2String(time);
                    Disp2String(" --ALARM\r");
                    stopwatchRunning = 0;
                    Idle();
                    Disp2String(time);
                    Disp2String("                 \r");
                }else{                                                          // else blink and display time
                    if(LATBbits.LATB8 == 1){
                        LATBbits.LATB8 = 0;
                    }else{
                        LATBbits.LATB8 = 1;
                    }
                    sec--;
                    if(sec == -1){
                        min--;
                        sec = 59;
                    }
                    Delay_ms(364);
                    if(min == 0 && sec == 0){
                        continue;
                    }
                    sprintf(time, "%02dm : %02ds\r", min, sec);  
                    Disp2String(time);
                }
            }else{                                                              // nothing pressed or running enter idle state
                Idle();
            }
        }
    }
}
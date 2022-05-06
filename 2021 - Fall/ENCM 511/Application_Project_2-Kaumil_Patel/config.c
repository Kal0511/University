/*
 * File:   config.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 12, 2021, 2:18 PM
 */


#include "xc.h"
#include "IOs.h"
#include "config.h"

void setConfig(){
    AD1PCFG = 0xFFFF;
    configClk();
    configRefO();
}

void configClk() {
    CLKDIVbits.RCDIV = 0;     //turn clock division to 0
    IPC3bits.AD1IP = 7;
}

void configRefO() {
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = 0b1000;
    REFOCONbits.ROEN = 1;
}
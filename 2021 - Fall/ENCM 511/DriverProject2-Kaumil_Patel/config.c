/*
 * File:   config.c
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 12, 2021, 2:18 PM
 */


#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "config.h"

void setConfig(){
    AD1PCFG = 0xFFFF;
    configClk();
}

void configClk() {
    CLKDIVbits.RCDIV= 0;     //turn clock division to 0
}
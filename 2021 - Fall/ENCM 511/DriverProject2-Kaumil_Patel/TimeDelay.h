/* 
 * File:   TimeDelay.h
 * Author: Kaumil Patel && Ryan Sommerville
 *
 * Created on October 6, 2021, 1:16 PM
 */




#ifndef TimeDelay_H
#define	TimeDelay_H

#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "config.h"

void Delay_ms(uint16_t time_ms);
/**
 * Requires:
 *  time_ms is a non-negative number.
 * Promises:
    * Delays the program by the time specified in the parameter
    * time_ms.
 */

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
/**
 * An interrupt function that receives an interrupt from Timer2.
 * Promises:
 *  Resets interrupt flag for Timer2.
 */

#endif 
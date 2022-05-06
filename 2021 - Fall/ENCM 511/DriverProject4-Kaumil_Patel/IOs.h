/* 
 * File:   IOs.h
 * Author: Kaumil Patel & Ryan Sommerville
 *
 * Created on October 6, 2021, 1:16 PM
 */

#ifndef IOS_H
#define	IOS_H

#include "xc.h"
#include "config.h"

void IOinit();
/*
 * Promises: 
 *  Initializes pins A2, A4, and B4 as input
 *  with automatic pull up, and B8 as output.
 */

void IOcheck();
/*
 * Requires:
 *  The input and outputs pins initialized in
 *  IOinit() still exist.
 * Promises:
 *  Repeatedly looks at input variables to determine state of LED.
 *  If only input A2 is low, blinks LED
 *  for 1 second intervals. If only input A4 is low,
 *  blinks LED for 2 seconds intervals. If only input B4
 *  is low, blinks LED for 3 seconds intervals. If two
 *  or more inputs are set low, keeps LED on. If on inputs
 *  are pressed, turns LED off.
 */



#endif 
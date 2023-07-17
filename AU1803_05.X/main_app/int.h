/******************************************************************************
 *
 * FILE: int.h
 *
 * 
 * 
 * DESCRIPTION: This file declares the API functions and variables for int 
 *              (interrupt) service routine container component.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * Int is a component that contains all the isr functions of the application.
 *
 *****************************************************************************/
#ifndef INT_H
#define INT_H

#include "../mcc_generated_files/tmr1.h"
#include "../mcc_generated_files/tmr3.h"
#include "../daq/daq_adc.h"
#include "../can/can.h"
#include "scheduler.h"

void INT_initInterruptService(void);

#endif  // INT_H
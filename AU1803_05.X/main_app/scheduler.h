/******************************************************************************
 *
 * FILE: scheduler.h
 *
 * This file declares the API functions and variables for scheduler.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 *
 *****************************************************************************/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include "sys_time.h"
#include "../smbus/smbus.h"
#include "../can/can.h"
#include "../protection/checkSafety.h"
#include "../i2c/i2c_a1006.h"
#include "../mcc_generated_files/adc1.h"
#include "../mcc_generated_files/watchdog.h"

void Process1mTimer(void);
void ProcessNormalTimer(void);
void Scheduler_init(void);
void Scheduler_mainFunction_foreground(void);
void Scheduler_mainFunction_background(void);

#endif  // SCHEDULER_H
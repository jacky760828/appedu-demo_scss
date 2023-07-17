/******************************************************************************
 * FILE: sys_time.h
 *
 * This file defines the functions for system timer component.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * System time is a component that manages software timer function for
 * a given application. This is a very useful way of keeping time in the
 * applications. For example, in one application,
 * UINT32 pfcStateChangeTime;
 *
 * The functions are re-entrant and thread safe.
 *
 ******************************************************************************/
#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../protection/checkSafety.h"

typedef enum {
    on,
    msec,
    sec,
    minute
} TIME_FORMAT_t;

typedef struct {
    int time;
    uint32_t cut;
    TIME_FORMAT_t Time_format;
    union{
        uint8_t state;
        struct {
            uint8_t rev:6;
            uint8_t over:1;
            uint8_t start:1;
        };
    };
} CutTime_t;

typedef enum {
    ON_TIMER,
    RUN_TIMER,
    STOP_TIMER,
    OVER_TIMER,
    ERROR
} TIME_RESPOND_t;

extern CutTime_t IntoFET;
extern CutTime_t COV_time;
extern CutTime_t POV_time;
extern CutTime_t EOCV_time;
extern CutTime_t CUV_time;
extern CutTime_t PUV_time;
extern CutTime_t OIMBV_time;
extern CutTime_t OCI_time;
extern CutTime_t ODI_1_time;
extern CutTime_t ODI_2_time;
extern CutTime_t ODI_3_time;
extern CutTime_t ODI_4_time;
extern CutTime_t H_ODI_time;
extern CutTime_t H_SCI_time;
extern CutTime_t COT_time;
extern CutTime_t COT_time1;
extern CutTime_t DOT_time;
extern CutTime_t DOT_time1;
extern CutTime_t CUT_time;
extern CutTime_t CUT_time1;
extern CutTime_t DUT_time;
extern CutTime_t MOT_time;
extern CutTime_t UVPF_time;
extern CutTime_t OVPF_time;
extern CutTime_t OTPF_time;
extern CutTime_t CIMPF_time;
extern CutTime_t THOPF_time;
extern CutTime_t THSPF_time;
extern CutTime_t CFETPF_time;
extern CutTime_t DFETPF_time;
extern CutTime_t BQ_F_time;
extern CutTime_t poweron_while_time;
extern CutTime_t poweroff_time;
extern CutTime_t BMS_Electrical_Update_time;
extern CutTime_t EVENTLOG_LOG_time;;


/******************************************************************************
*
******************************************************************************/
void SysTime_init(void);


/******************************************************************************
*
******************************************************************************/
void SysTime_mainFunction_tick(void);


/******************************************************************************
*
******************************************************************************/
uint32_t SysTime_getMsec(void);

uint32_t SysTime_getSec(void);

uint32_t SysTime_getMinute(void);


/******************************************************************************
*
******************************************************************************/

bool SysTime_msecIntervalElapsed(uint32_t startTime, uint32_t interval);

bool SysTime_secIntervalElapsed(uint32_t startTime, uint32_t interval);

bool SysTime_minuteIntervalElapsed(uint32_t startTime, uint32_t interval);
//======================by hugo======================================
typedef unsigned int TIMER_HANDLE;
typedef void (*TIMER_CALLBACK)(void*);

//==================================================

//==================================================
//	Function Declare
//==================================================
//
void TimerFunction1ms(void);
void TimerFunction10ms(void);
//
unsigned long Timer_GetMiniSecondTime(void);
void Timer_set_wait_timers(TIME_FORMAT_t format, int time);
void Timer_stop_wait_timers(void);
void Timer_restart_wait_timers(void);
TIME_RESPOND_t Timer_get_wait_timers(void);
uint32_t Timer_get_wait_for_timers(void);

void Timer_Protection_set_wait_timers(CutTime_t *Protection_Timer, TIME_FORMAT_t FORMAT, int time);
void Timer_Protection_stop_wait_timers(CutTime_t *Protection_Timer);
TIME_RESPOND_t Timer_Protection_get_wait_timers(CutTime_t *Protection_Timer);
uint32_t Timer_Protection_get_wait_for_timers(CutTime_t *Protection_Timer);
void Timer_Protection_timers(CutTime_t *Protection_Timer);
bool Timer_Protection_timers_Fun(CutTime_t *Protection_Timer, TIME_FORMAT_t FORMAT, int time, uint16_t *Log);

#endif  // SYS_TIME_H

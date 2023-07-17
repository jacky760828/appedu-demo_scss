/******************************************************************************
 * FILE: sys_time.c
 *
 * This file defines the functions for system timer component.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * System time is a component that can be to manage software timer function for
 * a given application.
 *
 *
 ******************************************************************************/
#include "sys_time.h"
#include "../main_app/mcu_init.h"
#include "../can/can_dbc.h"
#include "../main_app/hw_abstraction.h"
#include "../battery_manage_system/battery_manage_system.h"
#include "../daq/daq_adc.h"
#include "../smbus/smbus.h"

CutTime_t BMS_Operating_times;
CutTime_t IntoFET;

CutTime_t COV_time;
CutTime_t POV_time;
CutTime_t EOCV_time;
CutTime_t CUV_time;
CutTime_t PUV_time;
CutTime_t OIMBV_time;
CutTime_t OCI_time;
CutTime_t ODI_1_time;
CutTime_t ODI_2_time;
CutTime_t ODI_3_time;
CutTime_t ODI_4_time;
CutTime_t H_ODI_time;
CutTime_t H_SCI_time;
CutTime_t COT_time;
CutTime_t COT_time1;
CutTime_t DOT_time;
CutTime_t DOT_time1;
CutTime_t CUT_time;
CutTime_t CUT_time1;
CutTime_t DUT_time;
CutTime_t MOT_time;
CutTime_t UVPF_time;
CutTime_t OVPF_time;
CutTime_t OTPF_time;
CutTime_t CIMPF_time;
CutTime_t THOPF_time;
CutTime_t THSPF_time;
CutTime_t CFETPF_time;
CutTime_t DFETPF_time;
CutTime_t BQ_F_time;
CutTime_t poweron_while_time;
CutTime_t poweroff_time;
CutTime_t BMS_Electrical_Update_time;
CutTime_t EVENTLOG_LOG_time;

void Process1mTimer(void)
{
    Timer_Protection_timers(&COV_time);
    Timer_Protection_timers(&POV_time);
    Timer_Protection_timers(&EOCV_time);
    Timer_Protection_timers(&CUV_time);
    Timer_Protection_timers(&PUV_time);
    Timer_Protection_timers(&OIMBV_time);
    Timer_Protection_timers(&OCI_time);
    Timer_Protection_timers(&ODI_1_time);
    Timer_Protection_timers(&ODI_2_time);
    Timer_Protection_timers(&ODI_3_time);
    Timer_Protection_timers(&ODI_4_time);
    Timer_Protection_timers(&H_ODI_time);
    Timer_Protection_timers(&H_SCI_time);
    Timer_Protection_timers(&COT_time);
    Timer_Protection_timers(&COT_time1);
    Timer_Protection_timers(&DOT_time);
    Timer_Protection_timers(&DOT_time1);
    Timer_Protection_timers(&CUT_time);
    Timer_Protection_timers(&CUT_time1);
    Timer_Protection_timers(&DUT_time);
    Timer_Protection_timers(&MOT_time);
    Timer_Protection_timers(&UVPF_time);
    Timer_Protection_timers(&OVPF_time);
    Timer_Protection_timers(&OTPF_time);
    Timer_Protection_timers(&CIMPF_time);
    Timer_Protection_timers(&THOPF_time);
    Timer_Protection_timers(&THSPF_time);
    Timer_Protection_timers(&CFETPF_time);
    Timer_Protection_timers(&DFETPF_time);
    Timer_Protection_timers(&BQ_F_time);

    if(DFET_Pre.start) {
        if(SysTime_msecIntervalElapsed(DFET_Pre.cut, DFET_Pre_time)) {
            HWA_76200_DFET_FREE();
            DFET_Pre.start = 0;
            DFET_ON.cut = SysTime_getSec();
            DFET_ON.start = 1;
        }
    }
    
    if(DFET_ON.start) {
        if(SysTime_secIntervalElapsed(DFET_ON.cut, DFET_ON_time)) {
            HWA_Pre_Discharge_DISABLE();
            DFET_ON.start = 0;
        }
    }
}

//==================================================
//	ProcessCommunication
//==================================================
void ProcessNormalTimer(void)
{
    static unsigned char guc500msCount;
    static unsigned int Discharge30sCnt;
    
    if(++guc500msCount >= 50) {           //500ms
        Event500msFlag = 1;
        guc500msCount = 0;
    }

    if(DischargeFlag == 1) {
        if(++Discharge30sCnt >= 3000) {
            Discharge30sFlag = 1;
            Discharge30sCnt = 0;
        }
    }
    
    if(IntoFET.start) {
        if(SysTime_msecIntervalElapsed(IntoFET.cut, IntoFET.time)) {
            printf("\e[0;31m IntoFET.start \033[0m \n\r");
            Mcu_set_CFET_DFET_TRICKLE(KEEP,ON,HOLDING);
            BMS_Set_Statu_DFET(1);
            IntoFET.start = 0;
        }
    }
    
    if(BMS_Electrical_Update_time.start) {
        if(SysTime_msecIntervalElapsed(BMS_Electrical_Update_time.cut, BMS_Electrical_Update_time.time)) {
            BMS_Electrical_Update_time.over = 1;
            BMS_Electrical_Update_time.start = 0;
        }
    }
    
    if(EVENTLOG_LOG_time.start) {
        if(SysTime_secIntervalElapsed(EVENTLOG_LOG_time.cut, EVENTLOG_LOG_time.time)) {
            EVENTLOG_LOG_time.over = 1;
            EVENTLOG_LOG_time.start = 0;
        }
    }
    
    if(NOWAKEUP_time.start) {
        if(SysTime_secIntervalElapsed(NOWAKEUP_time.cut, NOWAKEUP_time10S_cut)) {
            BMS_Set_Operating_Flag(NO_WAKEUP_10S, 1);
        }
        if(SysTime_secIntervalElapsed(NOWAKEUP_time.cut, NOWAKEUP_time10min_cut)) {
            BMS_Set_Operating_Flag(NO_WAKEUP_10min, 1);
        }
    }
    
    if(BMS_Operating_times.start) {
        switch(BMS_Operating_times.Time_format) {
            case msec:
                if(SysTime_msecIntervalElapsed(BMS_Operating_times.cut, BMS_Operating_times.time)) {
                    BMS_Operating_times.over = 1;
                    BMS_Operating_times.start = 0;
                }
                break;
            case sec:
                if(SysTime_secIntervalElapsed(BMS_Operating_times.cut, BMS_Operating_times.time)) {
                    BMS_Operating_times.over = 1;
                    BMS_Operating_times.start = 0;
                }
                break;
            case minute:
                if(SysTime_minuteIntervalElapsed(BMS_Operating_times.cut, BMS_Operating_times.time)) {
                    BMS_Operating_times.over = 1;
                    BMS_Operating_times.start = 0;
                }
                break;
            default:
                break;
        }
    }
}

void SysTime_init(void)
{
}

/******************************************************************************
*
******************************************************************************/

bool SysTime_msecIntervalElapsed(uint32_t startTime, uint32_t interval)
{
    return ((uint32_t)(Timer_GetMiniSecondTime() - startTime) >= interval)?TRUE:FALSE;
}


bool SysTime_secIntervalElapsed(uint32_t startTime, uint32_t interval)
{
    return ((uint32_t)(Timer_GetMiniSecondTime()/1000 - startTime) >= interval)?TRUE:FALSE;
}


bool SysTime_minuteIntervalElapsed(uint32_t startTime, uint32_t interval)
{
    //return ((uint32_t)(d_sysTime.minute - startTime) >= interval)?TRUE:FALSE;
    return ((uint32_t)(Timer_GetMiniSecondTime()/60000 - startTime) >= interval)?TRUE:FALSE;
}


/******************************************************************************
*
******************************************************************************/

uint32_t SysTime_getMsec(void)
{
    return Timer_GetMiniSecondTime();//(d_sysTime.msec);
}


uint32_t SysTime_getSec(void)
{
    return (Timer_GetMiniSecondTime()/1000);//(d_sysTime.sec);
}


uint32_t SysTime_getMinute(void)
{
    return Timer_GetMiniSecondTime()/60000;//(d_sysTime.minute);
}

//=====================================by hugo=====================================
volatile static unsigned long gulMiniSecondCount;

//==================================================
//	TimerFunction1ms
//==================================================
void TimerFunction1ms(void)
{
	if(++gulMiniSecondCount == 0xFFFFFFFF)	
        gulMiniSecondCount = 0;
    Process1mTimer();
}

//==================================================
//	TimerFunction10ms
//==================================================
void TimerFunction10ms(void)
{
	ProcessNormalTimer();
}

//==================================================
//	Timer_GetMiniSecondTime 
//==================================================
unsigned long Timer_GetMiniSecondTime(void)
{
	return gulMiniSecondCount;
}

void Timer_set_wait_timers(TIME_FORMAT_t format, int time)
{
    if(format != on) {
        BMS_Operating_times.time = time;
        BMS_Operating_times.over = 0;
        switch(format) {
            case msec:
                BMS_Operating_times.Time_format = msec;
                BMS_Operating_times.cut = SysTime_getMsec();
                BMS_Operating_times.start = 1;
                break;
            case sec:
                BMS_Operating_times.Time_format = sec;
                BMS_Operating_times.cut = SysTime_getSec();
                BMS_Operating_times.start = 1;
                break;
            case minute:
                BMS_Operating_times.Time_format = minute;
                BMS_Operating_times.cut = SysTime_getMinute();
                BMS_Operating_times.start = 1;
                break;
            default:
                BMS_Operating_times.start = 0;
                break;
        }
    } else {
        BMS_Operating_times.start = 0;
    }
}

void Timer_stop_wait_timers(void)
{
    BMS_Operating_times.Time_format = on;
    BMS_Operating_times.time = 0;
    BMS_Operating_times.over = 0;
    BMS_Operating_times.cut = 0;
    BMS_Operating_times.start = 0;
}

void Timer_restart_wait_timers(void){
    switch(BMS_Operating_times.Time_format)
    {
        case msec:
            BMS_Operating_times.cut = SysTime_getMsec();
            break;
        case sec:
            BMS_Operating_times.cut = SysTime_getSec();
            break;
        case minute:
            BMS_Operating_times.cut = SysTime_getMinute();
            break;
        default:
            break;
    }
}

TIME_RESPOND_t Timer_get_wait_timers(void)
{
    TIME_RESPOND_t ret;
    if(!BMS_Operating_times.start && !BMS_Operating_times.over && BMS_Operating_times.cut == 0 && BMS_Operating_times.Time_format == on) {
        ret = ON_TIMER;
    } else if(BMS_Operating_times.start && !BMS_Operating_times.over && BMS_Operating_times.Time_format != on) {
        ret = RUN_TIMER;
    } else if(!BMS_Operating_times.start && !BMS_Operating_times.over && BMS_Operating_times.Time_format != on) {
        ret = STOP_TIMER;
    } else if(!BMS_Operating_times.start && BMS_Operating_times.over && BMS_Operating_times.Time_format != on){
        ret = OVER_TIMER;
    } else {
        ret = ERROR;
    }
    return ret;
}

uint32_t Timer_get_wait_for_timers(void)
{
    uint32_t ret = -1;
    
    if(BMS_Operating_times.start) {
        switch(BMS_Operating_times.Time_format) {
            case msec:
                ret = SysTime_getMsec() - BMS_Operating_times.cut;
                break;
            case sec:
                ret = SysTime_getSec() - BMS_Operating_times.cut;
                break;
            case minute:
                ret = SysTime_getMinute() - BMS_Operating_times.cut;
                break;
            default:
//                printf("onformat \n\r");
                break;
        }
    } else {
//        printf("on start \n\r");
    }
    return ret;
}

void Timer_Protection_set_wait_timers(CutTime_t *Protection_Timer, TIME_FORMAT_t FORMAT, int time)
{
    if(FORMAT != on) {
        Protection_Timer->time = time;
        Protection_Timer->over = 0;
        Protection_Timer->Time_format = FORMAT;
        switch(FORMAT) {
            case msec:
                Protection_Timer->cut = SysTime_getMsec();
                Protection_Timer->start = 1;
                break;
            case sec:
                Protection_Timer->cut = SysTime_getSec();
                Protection_Timer->start = 1;
                break;
            case minute:
                Protection_Timer->cut = SysTime_getMinute();
                Protection_Timer->start = 1;
                break;
            default:
                Protection_Timer->start = 0;
                break;
        }
    } else {
        Protection_Timer->start = 0;
    }
}

void Timer_Protection_stop_wait_timers(CutTime_t *Protection_Timer)
{
    Protection_Timer->Time_format = on;
    Protection_Timer->time = 0;
    Protection_Timer->over = 0;
    Protection_Timer->cut = 0;
    Protection_Timer->start = 0;
}

TIME_RESPOND_t Timer_Protection_get_wait_timers(CutTime_t *Protection_Timer)
{
    TIME_RESPOND_t ret;
    if(!Protection_Timer->start && !Protection_Timer->over && Protection_Timer->cut == 0 && Protection_Timer->Time_format == on) {
        ret = ON_TIMER;
    } else if(Protection_Timer->start && !Protection_Timer->over && Protection_Timer->Time_format != on) {
        ret = RUN_TIMER;
    } else if(!Protection_Timer->start && !Protection_Timer->over && Protection_Timer->Time_format != on) {
        ret = STOP_TIMER;
    } else if(!Protection_Timer->start && Protection_Timer->over && Protection_Timer->Time_format != on){
        ret = OVER_TIMER;
    } else {
        ret = ERROR;
    }
    return ret;
}

uint32_t Timer_Protection_get_wait_for_timers(CutTime_t *Protection_Timer)
{
    uint32_t ret = -1;
    
    if(Protection_Timer->start) {
        switch(Protection_Timer->Time_format) {
            case msec:
                ret = SysTime_getMsec() - Protection_Timer->cut;
                break;
            case sec:
                ret = SysTime_getSec() - Protection_Timer->cut;
                break;
            case minute:
                ret = SysTime_getMinute() - Protection_Timer->cut;
                break;
            default:
//                printf("onformat \n\r");
                break;
        }
    } else {
//        printf("on start \n\r");
    }
    return ret;
}

void Timer_Protection_timers(CutTime_t *Protection_Timer)
{
    if(Protection_Timer->start) {
        switch(Protection_Timer->Time_format) {
            case msec:
                if(SysTime_msecIntervalElapsed(Protection_Timer->cut, Protection_Timer->time)) {
                    Protection_Timer->over = 1;
                    Protection_Timer->start = 0;
                }
                break;
            case sec:
                if(SysTime_secIntervalElapsed(Protection_Timer->cut, Protection_Timer->time)) {
                    Protection_Timer->over = 1;
                    Protection_Timer->start = 0;
                }
                break;
            case minute:
                if(SysTime_minuteIntervalElapsed(Protection_Timer->cut, Protection_Timer->time)) {
                    Protection_Timer->over = 1;
                    Protection_Timer->start = 0;
                }
                break;
            default:
                break;
        }
    }
}

bool Timer_Protection_timers_Fun(CutTime_t *Protection_Timer, TIME_FORMAT_t FORMAT, int time, uint16_t *Log)
{
    bool ret = 0;
    
    if(ON_TIMER == Timer_Protection_get_wait_timers(Protection_Timer)) {
        Timer_Protection_set_wait_timers(Protection_Timer, FORMAT, time);
        printf("\e[0;31m start wait for %dS \033[0m \r\n",time);
        *Log = -1;
    } else if(OVER_TIMER == Timer_Protection_get_wait_timers(Protection_Timer)) {
        printf("\e[0;31m over %dS \033[0m \r\n",time);
        ret = 1;
    } else {
        if(*Log == -1) {
            *Log = (int)Timer_Protection_get_wait_for_timers(Protection_Timer);
            printf("\e[0;31m wait %dmS \033[0m \r\n",(int)Timer_Protection_get_wait_for_timers(Protection_Timer));
        } else if(*Log != (int)Timer_Protection_get_wait_for_timers(Protection_Timer)) {
            *Log = (int)Timer_Protection_get_wait_for_timers(Protection_Timer);
            printf("\e[0;31m wait %dmS \033[0m \r\n",(int)Timer_Protection_get_wait_for_timers(Protection_Timer));
        } else {
        }
    }
    
    return ret;
}


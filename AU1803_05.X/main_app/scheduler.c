/******************************************************************************
 *
 * FILE: scheduler.c
 *
 * This file defines the functions for scheduler.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 *
 *****************************************************************************/
#include "scheduler.h"

uint8_t d_highTaskID;
uint8_t d_lowTaskID;
bool d_highTaskGo;
uint32_t d_1MinTime;

/*******************************************************************************
* Function Name  : Scheduler_init
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
void Scheduler_init(void)
{
    d_highTaskGo = TRUE;
    d_highTaskID = 0;
    d_lowTaskID = 0;
    Protection_init();
    I2C1_Initialize(I2C_100Kbps);
    initAuthentification();
}

void Delay_us(unsigned int delay)
{
    unsigned int i;
    
    for (i = 0; i < delay; i++){
        __asm__ volatile ("repeat #39");
        __asm__ volatile ("nop");
    }
}

/*******************************************************************************
* Function Name  : Scheduler_mainFunction_foreground
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
void Scheduler_mainFunction_foreground(void)
{
   // SysTime_mainFunction_tick();
   TimerFunction1ms();
   // OC_mainFunction();
}

/*******************************************************************************
* Function Name  : Scheduler_mainFunction_background
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
void Scheduler_mainFunction_background(void)
{
    if(d_highTaskGo) {
        switch(d_highTaskID) {
            case 0:
                a1006Authentication();
                SMBUS_mainFunction();
                i2c_Machine();
                d_highTaskID = 1;
                break;

            case 1:
                if(BQ78350_all_info_ready()) {
                    tx_mainFunction();
                    CanDbc_mainFunction();
                }
                d_highTaskID = 2;
                break;

            case 2:
                WATCHDOG_TimerClear();
                d_highTaskID = 3;
                break;

            case 3:
                d_highTaskGo = false;
                d_highTaskID = 0;
                break;

            default:
                d_highTaskID = 0;
                break;
        }
    } else {
        d_highTaskGo = true;
        switch(d_lowTaskID) {
            case 0:
                Read_Port();
                ADC1_SoftwareTriggerEnable();
                Delay_us(20);
                ADC1_SoftwareTriggerDisable();
                d_lowTaskID = 1;
                break;

            case 1:
                d_lowTaskID = 2;
                break;

            case 2:
                SMBUS_sortMinMaxCellVolt();
                SMBUS_Temperature_info();
                d_lowTaskID = 3;
                break;

            case 3:
                if(BQ78350_all_info_ready()) {
                    ProtectionMonitor();
                }
                d_lowTaskID = 4;
                break;

            case 4:
                d_lowTaskID = 5;
                break;

            case 5:
                d_lowTaskID = 6;
                break;

            case 6:
                d_lowTaskID = 7;
                break;

            case 7:
                d_lowTaskID = 8;
                break;

            case 8:
                BMS_mainFunction();
                d_lowTaskID = 9;
                break;

            case 9:
                GetUpgradeFlag();
                d_lowTaskID = 10;
                break;

            case 10:
                if (d_1MinTime != SysTime_getMinute())
                {
                    d_1MinTime = SysTime_getMinute();
                    IdleDischargeLostCheck();
                }
                d_lowTaskID = 0;
                break;

            default:
                d_lowTaskID = 0;
                break;
        }            
    }
}
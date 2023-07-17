/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name   
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33EP256MC504
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.35
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
//#include "Class_B/classb.h"  //Alan_20200712 Add Class B Library for UL60730 test, Add

#include "mcc_generated_files/system.h"  //Alan_20200712 Add Class B Library for UL60730 test, Add

#include "mcu_init.h"
#include "int.h"
#include "scheduler.h"
#include "sys_time/sys_time.h"
#include "smbus/smbus.h"
#include "daq/daq_adc.h"
#include "flash/MX25L.h"
#include "can/can.h"
#include "main_app/hw_abstraction.h"
#include "main_app/version_config.h"
#include "battery_manage_system/battery_manage_system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr3.h"
#include "mcc_generated_files/reset_types.h"
#include "mcc_generated_files/watchdog.h"
#include "i2c/i2cstate.h"

/*
                         Main application
 */
//==========================================================================================

#ifdef USE_APP_FET_RESTORE
RESETSAVE RESETWORD;
#endif
int main(void)
{
    uint32_t FlashID = 0;
    
    SYSTEM_Initialize();
  //  __delay_ms(500);
    // Initialize all port to default
    McuInit_port();
       
     // Re-define interrupt service routine
    INT_initInterruptService();
    
    // Initialize system time
    SysTime_init();
    // Initialize pwm for lower power system
    //PWM_init(); //20200116 Alan Chen remove PWM2 due to not use it (pin 36) and AN4 (pin 24)) (Hardware not implement)

    printf("Application %d.%d.%d -%s %s- \n\r", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_BUILD, __DATE__, __TIME__);
    // Initialize ADC module
    DaqAdc_init();
    
    // Initialize SMBUS
    SMBUS_init();
    
    // Initialize CAN bus related
    CAN_init();
    CanDbc_init();
    
#ifdef EVENT_LOG
    if(!Check_EventLog_Lable()) {
        Erase_Flash_SE(EVENT_LABEL1_ADR);
        Erase_Flash_SE(SOC_RECORDER_ADR_1);
        Erase_Flash_SE(SOC_RECORDER_ADR_2);
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR1);
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR2);
        Erase_Flash_SE(DYNAMIC_K_VALUE_ADR1);
        Erase_Flash_SE(DYNAMIC_K_VALUE_ADR2);
        Erase_Flash_SE(SOHFlashAddress);   // SOH.c=>SOHFlashAddress for 4kb
        Erase_Flash_SE(LDAT_HEAD_AREA1);            // Erase 4kb
        Erase_Flash(LDAT_MSG_START_ADR,1);          // Erase 64kb
        Erase_Flash(EVENT_MSG_START_ADR,1);         // Erase 64kb
        Write_EventLog_Lable();
        printf("Erase_Flash_SE \r\n");
    } else {
        Get_Event_Index();
        Get_Ldat_Index();
    }
    CanDbc_init();
 #endif      
    // Initialize scheduler
    Scheduler_init();

    // Initialize state machine
    BMS_init();
    
    // Read Flash IC Memory Address

    // Enable watchdog   
//    WATCHDOG_TimerSoftwareEnable();
//    WATCHDOG_TimerClear();
    // Use the following macros to:
    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //TMR3_Stop();
    //AD1CON1bits.ADON = 1;
    //HWA_76200_DFET_FREE();
    //RCONbits.SWDTEN = 0;
     
    TMR1_Start();
    TMR3_Start();
    
    setOperatingBmsState(BMSState_OFF);
    EVENTLOG_BATTERY_MANAGE();
    EVENTLOG_CFET_DFET_TRICKLE();
    Get_Dynamic_Data();
    SOH_Init();
    FlashID = sFLASH_ReadID();
    printf("sFLASH_ReadID:%ld \r\n", FlashID);
    poweron_while_time.time = 2500;
    poweron_while_time.Time_format = msec;
    poweron_while_time.over = 0;
    poweron_while_time.cut = SysTime_getMsec();
    poweron_while_time.start = 1;
    
    BMS_Electrical_Update_time.time = 100;
    BMS_Electrical_Update_time.Time_format = msec;
    BMS_Electrical_Update_time.over = 0;
    BMS_Electrical_Update_time.cut = SysTime_getMsec();
    BMS_Electrical_Update_time.start = 1;

    EVENTLOG_LOG_time.time = 1;
    EVENTLOG_LOG_time.Time_format = sec;
    EVENTLOG_LOG_time.over = 0;
    EVENTLOG_LOG_time.cut = SysTime_getSec();
    EVENTLOG_LOG_time.start = 1;
    printf("while \n\r");
    BMS_Electrical_Update();
    while (1)
    {
#ifdef EVENT_LOG
        TimeAreaProcess();
        updateLdatData();
        ProcessEventData();
        CanRx_EvnetFunction();
#endif
        if(BMS_Electrical_Update_time.over) {
            BMS_Electrical_Update();
            BMS_Electrical_Update_time.over = 0;
            BMS_Electrical_Update_time.cut = SysTime_getMsec();
            BMS_Electrical_Update_time.start = 1;
        }
        
        Scheduler_mainFunction_background();
        
        if(EVENTLOG_LOG_time.over) {
            RC_old();
            EVENTLOG_BATTERY_MANAGE();
            EVENTLOG_PROTECTION();
            EVENTLOG_SOH();
            EVENTLOG_LOG_time.over = 0;
            EVENTLOG_LOG_time.cut = SysTime_getSec();
            EVENTLOG_LOG_time.start = 1;
        }
    }
    return 1; 
}
/**
 End of File
*/


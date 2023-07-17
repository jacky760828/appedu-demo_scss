/******************************************************************************
 *
 * FILE: ulclassbif.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
 //Alan_20200712 Add Class B Library for UL60730 test, Add, Start
#include "classb.h"
#include "ulclassbif.h"
#include "../main_app/platform_types.h"
#include "../battery_manage_system/battery_manage_system.h"
#include "../battery_manage_system/battery_manage_system_private.h"

extern uint16_t Watchdog_Reset_Counter1 __attribute__ ((persistent)) ;  //Alan_20200712 Add Class B Library for UL60730 test, Add
extern uint16_t Watchdog_Reset_Counter2 __attribute__ ((persistent)) ;  //Alan_20200712 Add Class B Library for UL60730 test, Add

void ulClassBFunctions(ULClassBCmdUsage_t code) 
{
BmsFaultFlag_t *pSvdFaultFlag;
pSvdFaultFlag = (BmsFaultFlag_t *)&d_thisBmsState.svdFaultFlag.AllBits;

    CLASSBRESULT  result;
    switch (code)
    {
        case UL_REGISTER_B_CMD  : //0x11
            pSvdFaultFlag->AllBits.fault_ULP_MCU_REG_F = TRUE;
            //result=CLASSB_CPURegistersTest();
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        //case UL_INSTRUCTION_C_CMD  :
        //    break;
        case UL_PC_B_CMD  : //0x13
            pSvdFaultFlag->AllBits.fault_ULP_MCU_PC_F = TRUE;
            //result=CLASSB_CPUPCTest();
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        //case UL_ADDRESS_C_CMD  :
        //    break;
        //case UL_DATA_FETCH_C_CMD  :
        //    break;
        case UL_INTERRUPT_B_CMD  : //0x20
            pSvdFaultFlag->AllBits.fault_ULP_BQ_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_CLOCK_B_CMD  : //0x30
            pSvdFaultFlag->AllBits.fault_ULP_MCU_CLK_F = TRUE;
           // result=CLASSB_ClockTest();
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_NVRAM_B_CMD  : //0x41
            pSvdFaultFlag->AllBits.fault_ULP_MCU_FLASH_F = TRUE;
            pSvdFaultFlag->AllBits.fault_ULP_MCU_EE_F = TRUE;
            //result=CLASSB_CRCFlashTest();
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_RAM_B_CMD  : //0x42
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            //result = CLASSB_RAMMarchBTest(0x1000,0x1000);
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_ADDRESS_B_MEM_CMD  : //0x43
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_DATA_B_CMD  : //0x51
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_ADDRESS_B_1_CMD  : //0x52
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_EXTERNAL_COMM_B_CMD  : //0x60
            pSvdFaultFlag->AllBits.fault_ULP_BQ_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        //case UL_DATA_C_CMD  :
        //    break;
        case UL_ADDRESS_B_2_CMD  : //0x62
            pSvdFaultFlag->AllBits.fault_ULP_MCU_FLASH_F = TRUE;
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;
        case UL_WRONG_TIMING_B_CMD  : //0x63
            pSvdFaultFlag->AllBits.fault_ULP_BQ_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
        case UL_WRONG_ORDER_B_CMD  : //0x64
            pSvdFaultFlag->AllBits.fault_ULP_MCU_FLASH_F = TRUE;
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
        case UL_IN_OUT_PERIPHERY_B_CMD  : //0x70
            pSvdFaultFlag->AllBits.fault_ULP_MCU_IO_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
        case UL_IO_C_CMD  : //0x71
            pSvdFaultFlag->AllBits.fault_ULP_MCU_IO_F = TRUE;
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
            
        case UL_ADC_DAC_B_CMD  : //0x73
            pSvdFaultFlag->AllBits.fault_ULP_MCU_IO_F = TRUE; // we use its flag
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
            
        case UL_ANALOG_MULTI_B_CMD : // 0x74
            pSvdFaultFlag->AllBits.fault_ULP_MCU_IO_F = TRUE; // we use its flag
            Watchdog_Reset_Counter1++;
            Watchdog_Reset_Counter2++;
            break;  
            
        default: 
            break;
    }

} 
//Alan_20200712 Add Class B Library for UL60730 test, Add, End
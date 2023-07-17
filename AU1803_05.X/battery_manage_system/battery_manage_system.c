/******************************************************************************
 *
 * FILE: battery_manage_system.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "battery_manage_system.h"
#include "../main_app/hw_abstraction.h"
#include "../daq/daq_adc.h"
#include "SOC.h"
#include "SOH.h"
#include "../main_app/mcu_init.h"
#include <stdio.h>

#include "../protection/checkSafety.h"
#include "../smbus/smbus.h"
#include "../can/can.h"
#ifdef EVENT_LOG 
#include "../main_app/scheduler.h"
#include "../EventLogData/EventLog.h"
#endif

#define BUS_VOLTAGE_VTH_1V                          1.0f                // Unit: V : 1.0f= 1.0v
#define BUS_VOLTAGE_VTH_3V                          3.0f                // Unit: V 3.0f= 3v
#define WAITING_FET_DRIVER_IC_STABLE                100                // Unit: mSec //20200522 for fix init vol out bug
#define WAITING_STATE_STABLE                        100                 // Unit: mSec
#define swReset() __asm__ volatile ("reset")  // frank software reset 8/12

bool d_firstCanConnect;
const UINT8 skuName[] = SKU_NAME_DEFAULT_RBC_STRING;
float vBus = 0, vBat = 0;                           //3.2fV
int32_t iBatt = 0;                                  //mA
float cellVmax, cellVmin, cellVavg, cellVmaxmin;    //3.2fV

UINT32 d_timerFetDriverStable;
UINT32 d_timerLedFlash;

bool d_chrgStatus = false;
BOOL d_flagFetDriverStable;
BOOL d_flagStateStable;
BOOL d_flagRedLedSolidCtrl;
BOOL d_flagRedLedBlinkCtrl;
BOOL d_flagGreenLedSolidCtrl;
BOOL d_flagGreenLedBlinkCtrl;
BOOL d_flagRedLedSolidNow;
BOOL d_flagRedLedBlinkNow;
BOOL d_flagGreenLedSolidNow;
BOOL d_flagGreenLedBlinkNow;
BOOL d_flagLedHalfPeriod;

BatteryManageSystem_t d_thisBmsState; //frank

#ifdef EVENT_LOG

UINT8 CatchBootRsocFlag = 0;
uint16_t RemainTimeCnt = 0XFFFF;
UINT8 EventSOH = 0;

int16_t CellTempature_Min = 0;
int16_t CellTempature_Max = 0;
int16_t CellTempature_Avg = 0;
int16_t CellTempature = 0;
int16_t FetTempature = 0;
uint32_t CntEventLength = 0;
//=== Flash IC Memory EventLog Recorder Address ================================
//=====Ldata Log Gobal Various=========== 
UINT16 Ldat_DCIRString;;
UINT16 Ldat_DCIRStringMin;
//======Event Log Gobal Various==========

BATTERY_STATU_TypeDef Battery_Statu;
INT16 iOverTrickleChrgCurr;
BOOL samelog = 0;
int sametime = -1;
CutTime_t NOWAKEUP_time;
uint32_t CHARGE_TimeS = 0;
uint32_t DISCHARGE_TimeS = 0;
uint32_t IDLE_TimeS = 0;
uint8_t logsme_state = 0;
uint16_t Reamin_Time_Voltage[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t Reamin_Time_Current[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t Reamin_Time_AVG_index = 0;
#endif

OPERATING_STATU_TypeDef Operating_Flag;
uint16_t d_DSGFCC;
uint8_t Gague_SOC;
uint16_t Gague_FCC;
uint16_t Gague_RC;
uint8_t Calculation_SOC;
uint16_t Calculation_FCC;
uint16_t Calculation_RC;
uint8_t Out_SOC;
uint16_t Out_FCC;
uint16_t Out_RC;

void BMS_Set_firstCanCommect(bool onoff)
{
    d_firstCanConnect = onoff;
}

void BMS_Set_Operating_Flag(OPERATING_STATU_t Flag, bool state)
{
    switch(Flag) {
        case VBATT_BUS_more_54 :
            Operating_Flag.VBATT_BUS_more_54 = state;
            break;
        case VBATT_BUS_more_30:
            Operating_Flag.VBATT_BUS_more_30 = state;
            break;
        case VBATT_BUS_less_30:
            Operating_Flag.VBATT_BUS_less_30 = state;
            break;
        case VBATT_BUS_less_30_10S:
            Operating_Flag.VBATT_BUS_less_30_10S = state;
            break;
        case VBATT_BUS_more_VBATT:
            Operating_Flag.VBATT_BUS_more_VBATT = state;
            break;
        case VBATT_BUS_less_VBATT_VTH1:
            Operating_Flag.VBATT_BUS_less_VBATT_VTH1 = state;
            break;
        case VBATT_BUS_less_VBATT_VTH3:
            Operating_Flag.VBATT_BUS_less_VBATT_VTH3 = state;
            break;
        case VBATT_BUS_more_VBATT_VTH1:
            Operating_Flag.VBATT_BUS_more_VBATT_VTH1 = state;
            break;
        case IBATT_less_minus3000:
            Operating_Flag.IBATT_less_minus3000 = state;
            break;
        case IBATT_less_minus3000_1S:
            Operating_Flag.IBATT_less_minus3000_1S = state;
            break;
        case IBATT_more_plus500:
            Operating_Flag.IBATT_more_plus500 = state;
            break;
        case IBATT_less_percentage20current:
            Operating_Flag.IBATT_less_percentage20current = state;
            break;
        case WAKEUP:
            Operating_Flag.WAKEUP = state;
            break;
        case NO_WAKEUP_10S:
            Operating_Flag.NO_WAKEUP_10S = state;
            break;
        case NO_WAKEUP_10min:
            Operating_Flag.NO_WAKEUP_10min = state;
            break;
        case LOSS_COMM:
            Operating_Flag.LOSS_COMM = state;
            break;
        case RSOC_more_30:
            Operating_Flag.RSOC_more_30 = state;
            break;
        case RSOC_more_50:
            Operating_Flag.RSOC_more_50 = state;
            break;
        case RSOC_less_29:
            Operating_Flag.RSOC_less_29 = state;
            break;
        case RSOC_less_50:
            Operating_Flag.RSOC_less_50 = state;
            break;
        case RSOC_less_95:
            Operating_Flag.RSOC_less_95 = state;
            break;
        default:
            break;
    }
}

uint32_t BMS_Get_Operating_Flag(OPERATING_STATU_t Flag)
{
    uint32_t ret;
    
    switch(Flag) {
        case VBATT_BUS_more_54 :
            ret = Operating_Flag.VBATT_BUS_more_54;
            break;
        case VBATT_BUS_more_30:
            ret = Operating_Flag.VBATT_BUS_more_30;
            break;
        case VBATT_BUS_less_30:
            ret = Operating_Flag.VBATT_BUS_less_30;
            break;
        case VBATT_BUS_less_30_10S:
            ret = Operating_Flag.VBATT_BUS_less_30_10S;
            break;
        case VBATT_BUS_more_VBATT:
            ret = Operating_Flag.VBATT_BUS_more_VBATT;
            break;
        case VBATT_BUS_less_VBATT_VTH1:
            ret = Operating_Flag.VBATT_BUS_less_VBATT_VTH1;
            break;
        case VBATT_BUS_less_VBATT_VTH3:
            ret = Operating_Flag.VBATT_BUS_less_VBATT_VTH3;
            break;
        case VBATT_BUS_more_VBATT_VTH1:
            ret = Operating_Flag.VBATT_BUS_more_VBATT_VTH1;
            break;
        case IBATT_less_minus3000:
            ret = Operating_Flag.IBATT_less_minus3000;
            break;
        case IBATT_less_minus3000_1S:
            ret = Operating_Flag.IBATT_less_minus3000_1S;
            break;
        case IBATT_more_plus500:
            ret = Operating_Flag.IBATT_more_plus500;
            break;
        case IBATT_less_percentage20current:
            ret = Operating_Flag.IBATT_less_percentage20current;
            break;
        case WAKEUP:
            ret = Operating_Flag.WAKEUP;
            break;
        case NO_WAKEUP_10S:
            ret = Operating_Flag.NO_WAKEUP_10S;
            break;
        case NO_WAKEUP_10min:
            ret = Operating_Flag.NO_WAKEUP_10min;
            break;
        case LOSS_COMM:
            ret = Operating_Flag.LOSS_COMM;
            break;
        case RSOC_more_30:
            ret = Operating_Flag.RSOC_more_30;
            break;
        case RSOC_more_50:
            ret = Operating_Flag.RSOC_more_50;
            break;
        case RSOC_less_29:
            ret = Operating_Flag.RSOC_less_29;
            break;
        case RSOC_less_50:
            ret = Operating_Flag.RSOC_less_50;
            break;
        case RSOC_less_95:
            ret = Operating_Flag.RSOC_less_95;
            break;
        case ALL:
            ret = Operating_Flag.all;
            break;
        default:
            ret = 0xFFFFFFFF;
            break;
    }
    
    return ret;
}

void BMS_WAKEUP_Update(void)
{
    if(HWA_WAKEUP_PIN()) {
        Operating_Flag.WAKEUP = 1;
        Operating_Flag.NO_WAKEUP_10S = 0;
        Operating_Flag.NO_WAKEUP_10min = 0;
        NOWAKEUP_time.start = 0;
    } else {
        Operating_Flag.WAKEUP = 0;
        NOWAKEUP_time.cut = SysTime_getSec();
        NOWAKEUP_time.start = 1;
    }
}

void BMS_Electrical_Update(void)
{
    int16_t CalTemperature;
    int16_t FETTemperature;
    uint16_t ret;
    int16_t ret1;
    
    vBus = DagAdc_get_VBUS();
    vBat = (SMBUS_getVoltage()/100.0f);
    ret1 = SMBUS_getCurrent();
    if(0 > ret1) {
        ret1 = 0 - ret1;
        iBatt = 0 - ((uint32_t)ret1 * 10);
    } else {
        iBatt = ret1 *10;
    }
    cellVmax = (SMBUS_getMaxCellVolt()/1000.0f);
    cellVmin = (SMBUS_getMinCellVolt()/1000.0f);
    cellVavg = ((uint16_t)(vBat * 1000 / CELL_IN_SERIES) / 1000.0f);
    cellVmaxmin = cellVmax - cellVmin;
    
    CalTemperature = SMBUS_getCellTemperature();
    if(CalTemperature < 0) {
        CellTempature = (((~CalTemperature + 1)-2731)/10);
    } else {
        CellTempature = ((CalTemperature-2731)/10);
    }

    CalTemperature = SMBUS_getMaxTemperature();
    if(CalTemperature < 0) {
        CellTempature_Max = (((~CalTemperature + 1)-2731)/10);
    } else {
        CellTempature_Max = ((CalTemperature-2731)/10);
    }
    
    CalTemperature = SMBUS_getMinTemperature();
    if(CalTemperature < 0) {
        CellTempature_Min = (((~CalTemperature + 1)-2731)/10);
    } else {
        CellTempature_Min = ((CalTemperature-2731)/10);
    }
        
    FETTemperature = SMBUS_getFetTemperature();
    if(FETTemperature < 0) {
        FetTempature = (((~FETTemperature + 1)-2731)/10);
    } else {
        FetTempature = ((FETTemperature-2731)/10);
    }
    
    BMS_WAKEUP_Update();
    Gague_FCC = SMBUS_getGaugeFCC();
    Gague_RC = SMBUS_getGaugeRC();
    Gague_SOC = ((uint32_t)Gague_RC * 100) / Gague_FCC;
    SOH_STATE.DecaySOH = SMBUS_getStateOfHealth();
#if 0
    Calculation_SOC = ((uint32_t)Calculation_RC * 100) / Calculation_FCC;
    Calculation_FCC = Gague_FCC;
    Calculation_RC = Gague_RC;
    Out_FCC = Calculation_FCC;
    Out_RC = Calculation_RC;
    Out_SOC = ((uint32_t)Out_RC * 100) / Out_FCC;
    SOH_STATE.DSGFCC = Out_FCC;
#else
    Out_FCC = dsgFCC;
    Out_RC = uiCalRCapacity;
    Out_SOC = ((uint32_t)Out_RC * 100) / Out_FCC;
    SOH_STATE.DSGFCC = Out_FCC;
#endif
    ret = Time_Dis_ChargingTotal();
    if(ret <= 10) {
//        RemainTimeCnt = 0;
    } else {
//        RemainTimeCnt = (ret - 10);
    }
    EventSOH = (uint8_t)SMBUS_getStateOfHealth() * 100;

//    EVENTLOG_SOH();
}

BmsState_t BMS_Get_Statu(void)
{
    return Battery_Statu.all;
}

void BMS_Set_Statu_CFET(BOOL statu)
{
	Battery_Statu.CFET = statu;
    d_thisBmsState.previousState = d_thisBmsState.currentState;
    d_thisBmsState.currentState = Battery_Statu.all;
}

BOOL BMS_Get_Statu_CFET(void)
{
    return Battery_Statu.CFET;
}

void BMS_Set_Statu_DFET(BOOL statu)
{
	Battery_Statu.DFET = statu;
    d_thisBmsState.previousState = d_thisBmsState.currentState;
    d_thisBmsState.currentState = Battery_Statu.all;
}

BOOL BMS_Get_Statu_DFET(void)
{
    return Battery_Statu.DFET;
}

void BMS_Set_Statu_TRICKLE(BOOL statu)
{
	d_thisBmsState.previousState = d_thisBmsState.currentState;
    d_thisBmsState.currentState = Battery_Statu.all;
}

BOOL BMS_Get_Statu_TRICKLE(void)
{
    return Battery_Statu.TRICKLE;
}

void BMS_Set_Statu_FAULT(BOOL statu)
{
	Battery_Statu.FAULT = statu;
    d_thisBmsState.previousState = d_thisBmsState.currentState;
    d_thisBmsState.currentState = Battery_Statu.all;
}

BOOL BMS_Get_Statu_FAULT(void)
{
    return Battery_Statu.FAULT;
}

/*******************************************************************************
* Function Name  : BMS_init
* Description    : Initial all variables for Battery manage system
* Input          : None
* Return         : None
*******************************************************************************/
void BMS_init(void)
{
	d_thisBmsState.BmsSvdSoftCmd.all = 0x00; //20200615 Jason Lin add pre-discharge function
    d_thisBmsState.currentState = BMSState_OFF;    
    
    d_thisBmsState.svdChargeSts.all = 0x00;
    d_thisBmsState.svdChargeCmd.all = 0x00;
    d_thisBmsState.svdCommand.all = 0x00;
    d_thisBmsState.svdCurrent.all = 0x00;
    d_thisBmsState.svdTemperature.all = 0x00;
    d_thisBmsState.svdVoltage.all = 0x00;
    d_thisBmsState.svdVoltage2.all = 0x00;
    d_thisBmsState.svdManufacturCmd.all = 0x00;
    d_thisBmsState.stateTime = SysTime_getSec();
    d_thisBmsState.stateStableTime = SysTime_getMsec();
    d_flagStateStable = FALSE;
#ifdef EVENT_LOG
    NoDischargeCnt = SysTime_getMsec();
    CellTempature_Min = 2888;
    CellTempature_Max = 0;
    CellTempature_Avg = 0;
#endif

    d_timerFetDriverStable = 0;  //=pSM->stateTime
    d_timerLedFlash = 0;

//    ChrgLogical_init();
    d_flagFetDriverStable = FALSE;
    d_flagRedLedBlinkCtrl = FALSE;
    d_flagRedLedSolidCtrl = TRUE; // tsai 10/12  FALSE;
    d_flagGreenLedSolidCtrl = TRUE; // tsai 10/12  FALSE;
    d_flagRedLedSolidNow = TRUE;//FALSE; tsai 10/12
    d_flagGreenLedSolidNow = TRUE;//FALSE; tsai 10/12
    d_flagGreenLedBlinkCtrl = FALSE;
    d_flagRedLedBlinkNow = FALSE;
    
    d_flagGreenLedBlinkNow = FALSE;
    d_flagLedHalfPeriod = FALSE;
    
    BmsSvdVoltBits_t *pSvdVolt = &d_thisBmsState.svdVoltage.bit; //tsai 7/28
    pSvdVolt->vBusTooLow = FALSE;//TRUE; tsai 5/31
    DBCFLAGS.byte=0;//tsai 8/2
    ReadImageVersionInFlash();//tsai 11/22
    Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,DISCHRG_NORMAL);
}

/*******************************************************************************
* Function Name  : updateStateTransVariables
* Description    : Collects the system information to update transition variables
* Input          : None
* Return         : None
*******************************************************************************/
void updateStateTransVariables(void)
{
	BmsSvdChrgStatusBits_t *pSvdChrgSts = &d_thisBmsState.svdChargeSts.bit;
    
	if(CatchBootRsocFlag == 1) {
        if(Calculation_SOC >= 100) {
            pSvdChrgSts->fullyChrged = TRUE;
            pSvdChrgSts->reChrgReqstByRSOC = FALSE;
        } else if(Calculation_SOC <= 94) {
            pSvdChrgSts->fullyChrged = FALSE;
            pSvdChrgSts->reChrgReqstByRSOC = TRUE;
        }        
    }
    
    if(d_timerFetDriverStable == 0) {
        d_timerFetDriverStable = SysTime_getMsec();
    } else if(SysTime_msecIntervalElapsed(d_timerFetDriverStable, WAITING_FET_DRIVER_IC_STABLE)) {
        d_flagFetDriverStable = TRUE;
    }

    if(SysTime_msecIntervalElapsed(d_thisBmsState.stateStableTime, WAITING_STATE_STABLE)) {
        d_flagStateStable = TRUE;
    }
}

BOOL bmsIsPermanentFaultOccurr(void)
{
    BmsFaultFlag_t *pSvdFaultFlag;
    
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;

    if (d_flagFetDriverStable && d_flagStateStable) {
        if( pSvdFaultFlag->fault_PFP ) { //frank 7/13
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

BOOL bmsIsUL60730FaultOccurr(void)
{
    BmsFaultFlag_t *pSvdFaultFlag;
    
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;

    if (d_flagFetDriverStable && d_flagStateStable) {
        if( pSvdFaultFlag->AllBits.fault_ULP_BQ_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_CLK_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_EE_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_FLASH_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_IO_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_PC_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_RAM_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_REG_F ||
            pSvdFaultFlag->AllBits.fault_ULP_MCU_WDT_F) {
                return TRUE;
            } else {
                return FALSE;
            }
    } else {
        return FALSE;
    }
}

BOOL Loss_COMM(void)
{
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
     
    if ((pSvdCmd->systemWakeUpDetected && pSvdCmd->lostCommunication)) {
        return TRUE; // by hugo 20210114
    }
    return FALSE;
}


UINT8 bmsIsFaultOccurr(void)
{ 
    BmsFaultFlag_t *pSvdFaultFlag;
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;

    if (d_flagFetDriverStable && d_flagStateStable) {
        if( pSvdFaultFlag->AllBits.fault_VP_OIMBV || //by MCU, 20200526 Alan Chen Temporary Disable it due to not ready yet
            pSvdFaultFlag->AllBits.fault_TP_DOT ||
            pSvdFaultFlag->AllBits.fault_TP_DUT ||
            pSvdFaultFlag->AllBits.fault_TP_MOT ||
            pSvdFaultFlag->fault_PFP) {      //frank 7/13
            return TRUE;  //by hugo 20210114
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

bool bmsIs_C_FaultOccurr(void)
{
    BmsFaultFlag_t *pSvdFaultFlag;
    
    BmsSvdVoltBits_t *pSvdVolt = &d_thisBmsState.svdVoltage.bit;
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;

    if (d_flagFetDriverStable && d_flagStateStable) {
        if( (pSvdFaultFlag->AllBits.fault_VP_COV ) || //by AFE delete "&& pSvdCurr->iBattGtZeroCharCurr" for state diagram v2.0b
            (pSvdFaultFlag->AllBits.fault_VP_POV ) || //delete "&& pSvdCurr->iBattGtZeroCharCurr" for state diagram v2.0b, by MCU, 20200526 Alan Chen Temporary Disable it due to not ready yet
             pSvdFaultFlag->AllBits.fault_CP_OCI || //AFE
            (pSvdFaultFlag->AllBits.fault_TP_COT ) || //AFE delete "&& pSvdCurr->iBattGtZeroCharCurr" for state diagram v2.0b
            (pSvdFaultFlag->AllBits.fault_TP_CUT ) || //AFE delete "&& pSvdCurr->iBattGtZeroCharCurr" for state diagram v2.0b
             pSvdVolt->packOverVoltage) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

BOOL bmsIs_D_FaultOccurr(void)
{
    BmsFaultFlag_t *pSvdFaultFlag;
    BmsSvdVoltBits_t *pSvdVolt = &d_thisBmsState.svdVoltage.bit;
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;
    if (d_flagFetDriverStable && d_flagStateStable) {
        if( (pSvdFaultFlag->AllBits.fault_VP_CUV ) || //by AFE delete "&& pSvdCurr->iBattLessZeroDischrgCurr" for state diagram v2.0b
            (pSvdFaultFlag->AllBits.fault_VP_PUV ) || //by AFE delete "&& pSvdCurr->iBattLessZeroDischrgCurr" for state diagram v2.0b
             pSvdFaultFlag->AllBits.fault_CP_ODI_1 || //by AFE
             pSvdFaultFlag->AllBits.fault_CP_ODI_2 || //by AFE
             pSvdFaultFlag->AllBits.fault_CP_H_ODI || //by MCU, 20200526 Alan Chen Temporary Disable it due to not ready yet
             pSvdFaultFlag->AllBits.fault_CP_H_SCI || //by AFE
             pSvdVolt->packUnderVoltage) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

BOOL isFullyCharge(void)
{
    BmsFaultFlag_t *pSvdFaultFlag;
    
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag; 
    if (d_flagFetDriverStable && d_flagStateStable) {
        if( pSvdFaultFlag->AllBits.fault_VP_EOCV ) { //by AFE
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

/*******************************************************************************
* Function Name  : setOperatingBmsState
* Description    : This function sets the new BMS state and provides necessary
*                  signals to other components.
* Input          : None
* Return         : None
*******************************************************************************/
void setOperatingBmsState(BmsState_t desiredState)
{
	switch(desiredState) {
        case BMSState_STANDBY:
            Mcu_set_CFET_DFET_TRICKLE(OFF, OFF, DISCHRG);
            BMS_Set_Statu_DFET(0);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(0);
            break;
        case BMSState_DISCHARGE_STANDBY:
            Mcu_set_CFET_DFET_TRICKLE(OFF, ON, DISCHRG);
            BMS_Set_Statu_DFET(1);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(0);
            break;
        case BMSState_CHARGE_RUN:
            Mcu_set_CFET_DFET_TRICKLE(ON, OFF, DISCHRG);
            BMS_Set_Statu_DFET(0);
            BMS_Set_Statu_CFET(1);
            BMS_Set_Statu_TRICKLE(0);
            break;
        case BMSState_DISCHARGE_RUN:
            Mcu_set_CFET_DFET_TRICKLE(ON, ON, DISCHRG);
            BMS_Set_Statu_DFET(1);
            BMS_Set_Statu_CFET(1);
            BMS_Set_Statu_TRICKLE(0);
            break;
        case BMSState_CHARGE_IMBALANCE:
            Mcu_set_CFET_DFET_TRICKLE(OFF, OFF, ENCHRG);
            BMS_Set_Statu_DFET(0);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(1);
            break;
        case BMSState_BALANCING:
            Mcu_set_CFET_DFET_TRICKLE(OFF, ON, ENCHRG);
            BMS_Set_Statu_DFET(1);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(1);
            break;
        case BMSState_PERMANENT_FAULT:
            Mcu_set_CFET_DFET_TRICKLE(OFF, OFF, DISCHRG);
            BMS_Set_Statu_DFET(0);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(0);
            break;
        case BMSState_OFF:
            Mcu_set_CFET_DFET_TRICKLE(OFF, OFF, DISCHRG);
            BMS_Set_Statu_DFET(0);
            BMS_Set_Statu_CFET(0);
            BMS_Set_Statu_TRICKLE(0);
            break;
        default:
            break;
    }
    
    d_thisBmsState.previousState = d_thisBmsState.currentState;
    d_thisBmsState.currentState = desiredState;
    d_thisBmsState.stateTime = SysTime_getSec();
    d_thisBmsState.stateStableTime = SysTime_getMsec();
//	EVENTLOG_BATTERY_MANAGE();
    EVENTLOG_BATTERY_STATE_CHANG();
}

/*******************************************************************************
* Function Name  : bmsStandby
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
void bmsStandby(void)
{
    if(bmsIsPermanentFaultOccurr() == TRUE) {                                       //Permanent Failure Protection
        Timer_stop_wait_timers();
        printf("(0)Permanent Failure Protection \r\n");
        setOperatingBmsState(BMSState_PERMANENT_FAULT);
    } else if(bmsIsUL60730FaultOccurr() == TRUE) {                                  //UL60730 Protection
        Timer_stop_wait_timers();
        printf("(0)UL60730 Protection \r\n");
        setOperatingBmsState(BMSState_PERMANENT_FAULT);
    } else if(CAN_DBC_Get_UPS_CTRL()) {                                             //UPS CMD ctrl
        Timer_stop_wait_timers();
        //printf("UPS_CTRL_FLAG \r\n");
    } else if((!BMS_Get_Operating_Flag(WAKEUP)) && (vBus < 30)) {   //(NOT WakeUP AND VBATT_BUS < 30V) for 10s
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(sec, 10);
            printf("(0)start (NOT WakeUP && VBATT_BUS < 30V) wait for 10s \r\n");
            sametime = -1;
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(0)(NOT WakeUP && VBATT_BUS < 30V) for 10s \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            if(sametime == -1) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(0)(NOT WakeUP && VBATT_BUS < 30V) wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else if(sametime != (int)Timer_get_wait_for_timers()) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(0)(NOT WakeUP && VBATT_BUS < 30V) wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else {
            }
        }
    } else if((!BMS_Get_Operating_Flag(WAKEUP)) && (Calculation_SOC < 29) && (vBus > vBat)) {              //(NOT WakeUP AND RSOC < 29%) AND (VBATT > VBATT)
        Timer_stop_wait_timers();
        printf("(0)(NOT WakeUP && RSOC < 29%%) && (VBATT > VBATT) \r\n");
        setOperatingBmsState(BMSState_CHARGE_IMBALANCE);
    } else if((Loss_COMM()) && (Calculation_SOC < 50) && (vBus > vBat)) {                  //(Loss COMM and RSOC <50%) AND (VBATT > VBATT)
        Timer_stop_wait_timers();
        printf("(0)(Loss COMM && RSOC <50%%) && (VBATT > VBATT) \r\n");
        setOperatingBmsState(BMSState_CHARGE_IMBALANCE);
    } else {                                                                        //nothing
        Timer_stop_wait_timers();
    }
}

void bmsDischargeStandby(void)
{
    if(bmsIsPermanentFaultOccurr() == TRUE) {                       //Permanent Failure Protection
        Timer_stop_wait_timers();
        printf("(1)Permanent Failure Protection \r\n");
        setOperatingBmsState(BMSState_PERMANENT_FAULT);
    } else if(bmsIsUL60730FaultOccurr() == TRUE) {                  //UL60730 Protection
        Timer_stop_wait_timers();
        printf("(1)UL60730 Protection \r\n");
        setOperatingBmsState(BMSState_PERMANENT_FAULT);
    } else if(bmsIs_D_FaultOccurr()) {                              //D-fault
        Timer_stop_wait_timers();
        printf("(1)D-fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(bmsIsFaultOccurr()) {                                 //Fault
        Timer_stop_wait_timers();
        printf("(1)Fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(iBatt < -3000) {
        Timer_stop_wait_timers();
        printf("(1)iBatt < -3000 \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_RUN);
    } else if(CAN_DBC_Get_UPS_CTRL()) {                             //UPS CMD ctrl
        Timer_stop_wait_timers();
        //printf("UPS_CTRL_FLAG \r\n");
    } else if(!BMS_Get_Operating_Flag(WAKEUP)) {    //NOT WakeUP for 10S
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(sec, 10);
            printf("(1)start NOT WakeUP wait for 10s \r\n");
            sametime = -1;
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(1)NOT WakeUP for 10S \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            if(sametime == -1) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(1)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else if(sametime != (int)Timer_get_wait_for_timers()) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(1)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else {
            }
        }
    } else {                                                        //nothing
        Timer_stop_wait_timers();
    }
}

void bmsChargeRun(void)
{
    if(bmsIsFaultOccurr()) {                                        //Fault
        Timer_stop_wait_timers();
        printf("(2)Fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
        logsme_state = 0;
    } else if(bmsIs_C_FaultOccurr()) {                              //C-fault
        Timer_stop_wait_timers();
        printf("(2)C-fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
        logsme_state = 0;
    } else if(CAN_DBC_Get_UPS_CTRL()) {                             //UPS CMD ctrl
        Timer_stop_wait_timers();
        logsme_state = 0;
    } else if(!BMS_Get_Operating_Flag(WAKEUP)) {    //NOT WakeUP for 10S
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(sec, 10);
            printf("(2)start NOT WakeUP for 10S \r\n");
            sametime = -1;
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(2)NOT WakeUP for 10S \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            if(sametime == -1) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(2)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else if(sametime != (int)Timer_get_wait_for_timers()) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(2)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else {
            }
        }
        logsme_state = 0;
    } else if(vBus < 30) {                                          //VBATT_BUS<30V
        Timer_stop_wait_timers();
        printf("(2)VBATT_BUS<30V \r\n");
        setOperatingBmsState(BMSState_STANDBY);
        logsme_state = 0;
    } else if(vBus >= (vBat + BUS_VOLTAGE_VTH_1V)) {                  //VBATT_BUS >= (VBATT+Vth_1V)
        Timer_stop_wait_timers();
        printf("(2)VBATT_BUS >= (VBATT+Vth_1V) \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_RUN);
        logsme_state = 0;
    } else if(iBatt > 500) {                                        //IBATT >0.5A
        Timer_stop_wait_timers();
        printf("(2)IBATT >0.5A \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_RUN);
        logsme_state = 0;
    } else {
        if(isFullyCharge() && (vBus > 54)) {                        //FULLY CHARGED && VBATT_BUS > 54V
            if(logsme_state == 0) {
                printf("(2)FULLY CHARGED && VBATT_BUS > 54V \r\n");
                Mcu_set_CFET_DFET_TRICKLE(OFF, KEEP, HOLDING);
            }
            logsme_state++;
        } else if(Calculation_SOC < 95) {                                  //RSOC < 95%
            if(logsme_state == 0) {
                printf("(2)RSOC < 95%% \r\n");
                Mcu_set_CFET_DFET_TRICKLE(ON, KEEP, HOLDING);
            }
            logsme_state++;
        }
        Timer_stop_wait_timers();
    }
}

void bmsDischargeRun(void)
{
    if(bmsIs_C_FaultOccurr()) {                                     //C-fault
        Timer_stop_wait_timers();
        printf("(3)C-fault \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_STANDBY);
        logsme_state = 0;
    } else if(bmsIs_D_FaultOccurr()) {                              //D-fault
        Timer_stop_wait_timers();
        printf("(3)D-fault \r\n");
        setOperatingBmsState(BMSState_CHARGE_RUN);
        logsme_state = 0;
    } else if(bmsIsFaultOccurr()) {                                 //Fault
        Timer_stop_wait_timers();
        printf("(3)Fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
        logsme_state = 0;
    } else if(CAN_DBC_Get_UPS_CTRL()) {                             //UPS CMD ctrl
        Timer_stop_wait_timers();
        logsme_state = 0;
    } else if(!BMS_Get_Operating_Flag(WAKEUP)) {    //NOT WakeUP for 10S
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(sec, 10);
            printf("(3)start NOT WakeUP for 10S \r\n");
            sametime = -1;
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(3)NOT WakeUP for 10S \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            if(sametime == -1) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(3)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else if(sametime != (int)Timer_get_wait_for_timers()) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(3)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else {
            }
        }
        logsme_state = 0;
    } else {
        Timer_stop_wait_timers();
        if(isFullyCharge() && (vBus > 54)) {                        //FULLY CHARGED && VBATT_BUS > 54V
            if(logsme_state == 0) {
                printf("(3)FULLY CHARGED && VBATT_BUS > 54V \r\n");
                Mcu_set_CFET_DFET_TRICKLE(OFF, KEEP, HOLDING);
            }
            logsme_state++;
        } else if((Calculation_SOC < 95) || (iBatt < -3000)) {                //RSOC < 95% || IBATT < -3A
            if(logsme_state == 0) {
                printf("(3)RSOC < 95%% || IBATT < -3A \r\n");
                Mcu_set_CFET_DFET_TRICKLE(ON, KEEP, HOLDING);
            }
            logsme_state++;
        }
    }
}

void bmsChargeImbalance(void)
{
    if(bmsIs_C_FaultOccurr()) {                                                         //C-fault
        Timer_stop_wait_timers();
        printf("(4)C-fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(bmsIsFaultOccurr()) {                                                     //Fault
        Timer_stop_wait_timers();
        printf("(4)Fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(CAN_DBC_Get_UPS_CTRL()) {                                                 //UPS CMD ctrl
        Timer_stop_wait_timers();
    } else  if(vBus <= (vBat + BUS_VOLTAGE_VTH_1V)) {                                   //VBATT_BUS <= (VBATT+Vth_1V)
        Timer_stop_wait_timers();
        printf("(4)VBATT_BUS <= (VBATT+Vth_1V) \r\n");
        setOperatingBmsState(BMSState_CHARGE_RUN);
    } else if((vBus <= (vBat + BUS_VOLTAGE_VTH_3V)) && (iBatt < iOverTrickleChrgCurr)) {    //(VbATT_BUS <= (VBATT+Vth_3V)) && (IBATT < 20% of TRICKLE CURRENT SPEC)
        Timer_stop_wait_timers();
        printf("(4)(VbATT_BUS <= (VBATT+Vth_3V)) && (IBATT < 20%% of TRICKLE CURRENT SPEC) \r\n");
        setOperatingBmsState(BMSState_CHARGE_RUN);
    } else if(Calculation_SOC >= 30 && !BMS_Get_Operating_Flag(WAKEUP)) {                                    //RSOC >= 30% && NOT WAKEUP
        Timer_stop_wait_timers();
        printf("(4)RSOC >= 30%% && NOT WAKEUP \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(Calculation_SOC >= 50 && Loss_COMM()) {                                          //RSOC >= 50% && Loss COMM
        Timer_stop_wait_timers();
        printf("(4)RSOC >= 50%% && Loss COMM \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(vBus < 30) {                                                              //VBATT_BUS < 30V
        Timer_stop_wait_timers();
        printf("(4)VBATT_BUS < 30V \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else {                                                                            //nothing
        Timer_stop_wait_timers();
    }
}

void bmsBalancing(void)
{
    if(bmsIs_D_FaultOccurr()) {                                                         //D-fault
        Timer_stop_wait_timers();
        printf("(5)D-fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(bmsIsFaultOccurr()) {                                                     //Fault
        Timer_stop_wait_timers();
        printf("(5)Fault \r\n");
        setOperatingBmsState(BMSState_STANDBY);
    } else if(bmsIs_C_FaultOccurr()) {                                                  //C-fault
        Timer_stop_wait_timers();
        printf("(5)C-fault \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_STANDBY);
    } else if(CAN_DBC_Get_UPS_CTRL()) {                                                 //UPS CMD ctrl
        Timer_stop_wait_timers();
    } else if(!BMS_Get_Operating_Flag(WAKEUP)) {                                        //NOT WakeUP for 10S
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(sec, 10);
            printf("(5)start NOT WakeUP for 10S \r\n");
            sametime = -1;
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(5)NOT WakeUP for 10S \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            if(sametime == -1) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(5)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else if(sametime != (int)Timer_get_wait_for_timers()) {
                sametime = (int)Timer_get_wait_for_timers();
                printf("(5)NOT WakeUP wait %dS \r\n",(int)Timer_get_wait_for_timers());
            } else {
            }
        }
    } else if(vBus <= (vBat+BUS_VOLTAGE_VTH_1V)) {                                      //VBATT_BUS <= (VBATT+Vth_1V)
        Timer_stop_wait_timers();
        printf("(5)VBATT_BUS <= (VBATT+Vth_1V) \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_RUN);
    } else if((vBus <= (vBat+BUS_VOLTAGE_VTH_3V)) && (iBatt < iOverTrickleChrgCurr)) {  //VBATT_BUS <= (VBATT+Vth_3V) && (IBATT < 20% of TRICKLE CURRENT SPEC)
        Timer_stop_wait_timers();
        printf("(5)VBATT_BUS <= (VBATT+Vth_3V) && (IBATT < 20%% of TRICKLE CURRENT SPEC) \r\n");
        setOperatingBmsState(BMSState_DISCHARGE_RUN);
    } else {                                                                            //nothing
        Timer_stop_wait_timers();
    }
}

void bmsPermanentFault(void)
{
    if(!BMS_Get_Operating_Flag(WAKEUP)) {                           //NOT WakeUP for 10min
        if(ON_TIMER == Timer_get_wait_timers()) {
            Timer_set_wait_timers(minute, 10);
            printf("(8)start NOT WakeUP for 10min \r\n");
        } else if(OVER_TIMER == Timer_get_wait_timers()) {
            printf("(8)NOT WakeUP for 10min \r\n");
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
        } else {
            printf("(8)NOT WakeUP wait %dmin \r\n",(int)Timer_get_wait_for_timers());
        }
    } else {                                                        //nothing
        Timer_stop_wait_timers();
    }
}

void bmsOFF(void)
{
    if(!poweron_while_time.over) {
        if(!SysTime_msecIntervalElapsed(poweron_while_time.cut, poweron_while_time.time)) {
            return;
        } else {
            poweron_while_time.over = 1;
            SOH_Init_while();
        }
    }
    if(!SMBUS_getShutDownEnable() && !SMBUS_is_ShutDown_Run()) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (vBus > 30.0f)) {               //WakeUP && (VBATT_BUS > 30V)
            Timer_stop_wait_timers();
            printf("(255)WakeUP && (VBATT_BUS > 30V) \r\n");
            setOperatingBmsState(BMSState_STANDBY);
            samelog = 0;
        } else if(!BMS_Get_Operating_Flag(WAKEUP) && (vBus > vBat)) {   //NOT WakeUP && (VBATT_BUS > VBATT)
            Timer_stop_wait_timers();
            printf("(255)NOT WakeUP && (VBATT_BUS > VBATT) \r\n");
            setOperatingBmsState(BMSState_STANDBY);
            samelog = 0;
        } else if(BMS_Get_Operating_Flag(WAKEUP) && (vBus < 30.0f)) {      //WakeUP && (VBATT_BUS < 30)
            Timer_stop_wait_timers();
            printf("(255)WakeUP && (VBATT_BUS < 30V) \r\n");
            setOperatingBmsState(BMSState_DISCHARGE_STANDBY);
            samelog = 0;
        } else if(!BMS_Get_Operating_Flag(WAKEUP)) {
            Timer_stop_wait_timers();
            printf("(255)NOT WakeUP \r\n");
            SMBUS_setShutDownEnable(true);
            samelog = 0;
        } else {
            Timer_stop_wait_timers();
            printf("(255)NOT nothing \r\n");
            samelog = 0;
        }
    } else {                                                        //nothing
        Timer_stop_wait_timers();
        if(!samelog) {
            printf("(255)Shipping Mode \r\n");
            if(-1 != SOH_Write_Flash(SOH_STATE.flashaddress, &SOH_STATE)) {
                samelog = 1;
                poweroff_time.time = 1000;
                poweroff_time.Time_format = msec;
                poweroff_time.over = 0;
                poweroff_time.cut = SysTime_getMsec();
                poweroff_time.start = 1;
            } else {
                printf("(255)SOH Write Flash -error \r\n");
            }
        } else {
            if(!SysTime_msecIntervalElapsed(poweroff_time.cut, poweroff_time.time)) {
                printf("..");
            } else {
                SMBUS_setShutDownEnable(true);
                printf("shutDown -error \r\n");
                swReset();
            }
        }
    }
}

/*******************************************************************************
* Function Name  : thisBmsStateMachine
* Description    : Calls this bms state machine. It gets command from user
*                  and executes the local state machine. It gives commands to
*                  output_on and output_off based on different state transitions.
* Input          : None
* Return         : None
*******************************************************************************/
void thisBmsStateMachine(void)
{
	switch(d_thisBmsState.currentState) {
        case BMSState_STANDBY:
//            printf("BMSState_STANDBY \n\r");
            bmsStandby();
            break;
            
        case BMSState_DISCHARGE_STANDBY:
//            printf("BMSState_DISCHARGE_STANDBY \n\r");
            bmsDischargeStandby();
            break;
            
        case BMSState_CHARGE_RUN:
//            printf("BMSState_CHARGE_RUN \n\r");
            bmsChargeRun();
            break;
            
        case BMSState_DISCHARGE_RUN:
//            printf("BMSState_DISCHARGE_RUN \n\r");
            bmsDischargeRun();
            break;
            
        case BMSState_CHARGE_IMBALANCE:
//            printf("BMSState_CHARGE_IMBALANCE \n\r");
            bmsChargeImbalance();
            break;
                  
        case BMSState_BALANCING:
//            printf("BMSState_BALANCING \n\r");
            bmsBalancing();
            break;
        
        case BMSState_PERMANENT_FAULT:
//            printf("BMSState_PERMANENT_FAULT \n\r");
            bmsPermanentFault();
            break;
        
        case BMSState_OFF:
//            printf("BMSState_OFF \n\r");
            bmsOFF();
            break;
        default:
            setOperatingBmsState(BMSState_OFF);
            SMBUS_setShutDownEnable(true);
            printf("\e[0;31m thisBmsStateMachine ERR(%d) \033[0m \n\r", d_thisBmsState.currentState);
            break;
    }
}

void reflashLED(void) // tsai 10/8
{
    BmsSvdVoltBits_t *pSvdVolt = &d_thisBmsState.svdVoltage.bit;
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
    BmsSvdCurrentBits_t *pSvdCurr = &d_thisBmsState.svdCurrent.bit;
    static BOOL d_dsg3A = FALSE;
    BOOL UpdateLed = FALSE;
    static UINT16 uiFlashLedTime=500;
    static UINT8 ucDutyCycle=0, ucBlinkCount=1;
    
    if ( ( (bmsIsPermanentFaultOccurr() == TRUE) || ( pSvdCmd->lostCommunication && ((GetFETsState() & 0x03) != 0)) ) && d_flagFetDriverStable) {//bmsIsFaultOccurr()
        //fault condition, light red led on
        d_flagRedLedSolidCtrl = TRUE;
        d_flagGreenLedSolidCtrl = FALSE;
    } else if ( bmsIs_D_FaultOccurr() || bmsIs_C_FaultOccurr() ) {
        //1st protection, amber
        d_flagRedLedSolidCtrl = TRUE;
        d_flagGreenLedSolidCtrl = TRUE;
    } else if( !pSvdCmd->lostCommunication ) { //!Loss_COMM())//!pSvdCmd->lostCommunication && d_chrgStatus == FALSE)
        //if (SysTime_secIntervalElapsed(d_timerNoCAN, 10)){
        //  <  -3A  // amber
        if (pSvdCurr->iBattGtDischrgCurr == TRUE ) {// && pSvdCurr->iBattLessZeroDischrgCurr == FALSE){
            d_flagRedLedSolidCtrl = TRUE;
            d_flagGreenLedSolidCtrl = TRUE;
            d_dsg3A=TRUE;
        } else if ((BMS_Get_Operating_Flag(WAKEUP)) && d_firstCanConnect ) { //>=-3A  // tsai 10/18
            // green
            d_flagRedLedSolidCtrl = FALSE;
            if ( pSvdCurr->iBattLessZeroDischrgCurr == FALSE) {  
                if (d_dsg3A == TRUE) { //amber
                    d_flagRedLedSolidCtrl = TRUE;
                }
            } else { //if (pSvdCurr->iBattLessZeroDischrgCurr == TRUE)  // > -1A  green
                d_dsg3A =FALSE;
            }
            d_flagGreenLedSolidCtrl = TRUE;
        } else {  // others, amber
            if (d_chrgStatus && d_firstCanConnect==0) { //for default is Canconnect
                goto blink;
            }
            d_flagRedLedSolidCtrl = TRUE;
            d_flagGreenLedSolidCtrl = TRUE;
            d_dsg3A = FALSE;
        }
    } else { // no communication       
        blink:    
        //stand alone without system
        d_dsg3A = FALSE;    
        if(d_chrgStatus ) { //2
            if  (!BMS_Get_Operating_Flag(WAKEUP)) { //no ups
                if((Calculation_SOC < 10 ) && pSvdVolt->vBusGtVBat ) {  //
                    d_flagRedLedBlinkCtrl = TRUE;
                    d_flagGreenLedBlinkCtrl = TRUE;
                    ucBlinkCount=3;
                    if(ucDutyCycle > 2) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 20) && pSvdVolt->vBusGtVBat ) { //
                    if (ucDutyCycle >= 1) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagRedLedBlinkCtrl = FALSE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    }
                    ucBlinkCount=3;
                    if(ucDutyCycle > 2) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 30) && pSvdVolt->vBusGtVBat ) { // 
                    if (ucDutyCycle >= 2) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagGreenLedBlinkCtrl = TRUE;
                        d_flagRedLedBlinkCtrl = FALSE;
                    }
                    ucBlinkCount=3;
                    if(ucDutyCycle > 2) {
                        ucDutyCycle=0;
                    }
                }
            } else { //ucWakeUpPin== TRUE
                if((Calculation_SOC < 10 ) && pSvdVolt->vBusGtVBat ) {
                    d_flagRedLedBlinkCtrl = TRUE;
                    d_flagGreenLedBlinkCtrl = TRUE;
                    ucBlinkCount=5;
                    if(ucDutyCycle > 4) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 20) && pSvdVolt->vBusGtVBat ) {
                    if (ucDutyCycle >= 1) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagRedLedBlinkCtrl = FALSE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    }
                    ucBlinkCount=5;
                    if(ucDutyCycle > 4) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 30) && pSvdVolt->vBusGtVBat ) {
                    if (ucDutyCycle >= 2) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagGreenLedBlinkCtrl = TRUE;
                        d_flagRedLedBlinkCtrl = FALSE;
                    }
                    ucBlinkCount=5;
                    if(ucDutyCycle > 4) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 40) && pSvdVolt->vBusGtVBat ) {
                    if (ucDutyCycle >= 3) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagGreenLedBlinkCtrl = TRUE;
                        d_flagRedLedBlinkCtrl = FALSE;
                    }
                    ucBlinkCount=5;
                    if(ucDutyCycle > 4) {
                        ucDutyCycle=0;
                    }
                } else if((Calculation_SOC < 50) && pSvdVolt->vBusGtVBat ) {
                    if (ucDutyCycle >= 4) {
                        d_flagRedLedBlinkCtrl = TRUE;
                        d_flagGreenLedBlinkCtrl = TRUE;
                    } else {
                        d_flagGreenLedBlinkCtrl = TRUE;
                        d_flagRedLedBlinkCtrl = FALSE;
                    }
                    ucBlinkCount=5;
                    if(ucDutyCycle > 4) {
                        ucDutyCycle=0;
                    }
                }
            }
        } else { //trickle charge off
            d_flagRedLedBlinkCtrl = FALSE;
            if(Calculation_SOC <= 30) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=3;
                 if(ucDutyCycle > 2) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 40) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=4;
                if(ucDutyCycle > 3) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 50) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=5;
                if(ucDutyCycle > 4) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 60) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=6;
                if(ucDutyCycle > 5) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 70) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=7;
                if(ucDutyCycle > 6) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 80) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=8;
                if(ucDutyCycle > 7) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 90) {
                d_flagGreenLedBlinkCtrl = TRUE;
                ucBlinkCount=9;
                if(ucDutyCycle > 8) {
                    ucDutyCycle=0;
                }
            } else if(Calculation_SOC <= 100) {
                ucBlinkCount=10;
                if(ucDutyCycle > 9) {
                    ucDutyCycle=0;
                }
            }
        }
    }

    if(SysTime_msecIntervalElapsed(d_timerLedFlash, uiFlashLedTime)) {
        d_timerLedFlash = SysTime_getMsec();
        UpdateLed = TRUE;
        if(d_flagLedHalfPeriod) {
            d_flagLedHalfPeriod = FALSE;
            ucDutyCycle++;
            if (((ucDutyCycle % ucBlinkCount) == 1) || (ucBlinkCount == 1) ) {  
                uiFlashLedTime = 1500;
            } else {
                uiFlashLedTime = 500;
            }
        } else {
            if ((ucDutyCycle % ucBlinkCount) == 1 || (ucBlinkCount == 1)) {
                uiFlashLedTime = 500;
          /*} else if ((ucBlinkCount == 1)) {
                uiFlashLedTime = 1500;*/
            }
            d_flagLedHalfPeriod = TRUE;
        }
    }
    
    if(UpdateLed) {
        if(d_flagLedHalfPeriod) {
            if(d_flagGreenLedSolidNow || d_flagGreenLedBlinkNow) {
                HWA_LED2_ON();
            }
        
            if(d_flagRedLedSolidNow || d_flagRedLedBlinkNow) {
                HWA_LED1_ON();
            }
        } else {
            if(d_flagGreenLedSolidNow) {
                HWA_LED2_ON();
            } else {
                HWA_LED2_OFF();
            }
        
            if(d_flagRedLedSolidNow) {
                HWA_LED1_ON();
            } else {
                HWA_LED1_OFF();
            }
            
            d_flagRedLedSolidNow = d_flagRedLedSolidCtrl;
            d_flagRedLedBlinkNow = d_flagRedLedBlinkCtrl;
            d_flagGreenLedSolidNow = d_flagGreenLedSolidCtrl;
            d_flagGreenLedBlinkNow = d_flagGreenLedBlinkCtrl;
                
            d_flagRedLedSolidCtrl = FALSE;
            d_flagRedLedBlinkCtrl = FALSE;
            d_flagGreenLedSolidCtrl = FALSE;
            d_flagGreenLedBlinkCtrl = FALSE;
            
        }
    }
}

uint16_t Time_Dis_ChargingTotal(void) //250mS
{
    uint16_t ret = 0xFFFF;
    int i;
    uint32_t vlot = 0;
    uint32_t ma = 0;
    
    if(iBatt > 0) {
//        ret = (((dsgFCC - uiCalRCapacityWH) * 1000) / (vlot * ma) * 60 * 60;
        CHARGE_TimeS += 10;
        DISCHARGE_TimeS = 0;
        IDLE_TimeS = 0;
        Reamin_Time_AVG_index = 0;
    } else if(iBatt < 0) {
        Reamin_Time_Voltage[(Reamin_Time_AVG_index % 10)] = (uint16_t)(vBus * 10);  // 50.4V -> 504
        Reamin_Time_Current[(Reamin_Time_AVG_index % 10)] = ((uint16_t)(0 - iBatt) / 10);    // 520mA -> 52
        Reamin_Time_AVG_index++;
        if(Reamin_Time_AVG_index <= 10) {
            for(i = 0; i < Reamin_Time_AVG_index; i++) {
                vlot += Reamin_Time_Voltage[i];
                ma += Reamin_Time_Current[i];
            }
            vlot = vlot / Reamin_Time_AVG_index;
            ma = ma / Reamin_Time_AVG_index;
        } else {
            if(Reamin_Time_AVG_index > 20)
                Reamin_Time_AVG_index = 11;
            for(i = 0; i < 10; i++) {
                vlot += Reamin_Time_Voltage[i];
                ma += Reamin_Time_Current[i];
            }
            vlot = vlot / 10;
            ma = ma / 10;
        }
//        for(i = 0; i < 10; i++) {
//            printf("Reamin_Time_Voltage:0x%04X ", Reamin_Time_Voltage[i]);
//            printf("Reamin_Time_Current:0x%04X ", Reamin_Time_Current[i]);
//            printf("\r\n");
//        }
//        printf("Reamin_Time_AVG_index:%d\r\n", Reamin_Time_AVG_index);
//        printf("vlot:%ld ma:%ld \r\n", vlot, ma);
//        printf("ret:%ld \r\n", (uiCalRCapacityWH * 1000));
//        printf("ret:%ld \r\n", (vlot * ma));
        ret = ((((uint32_t)Out_RC * 1000) * 60 * 60) / (vlot * ma));
//        printf("ret:%d \r\n", ret);
        CHARGE_TimeS = 0;
        DISCHARGE_TimeS += 10;
        IDLE_TimeS = 0;
    } else {
        ret = ((((uint32_t)Out_RC * 1000) * 60 * 60) / (vlot * BMS_IDLE_IBATT));
        CHARGE_TimeS = 0;
        DISCHARGE_TimeS = 0;
        IDLE_TimeS += 10;
    }
    
    return ret;
}

/*******************************************************************************
* Function Name  : BMS_mainFunction
* Description    : Battery manage system main function
* Input          : None
* Return         : None
*******************************************************************************/
void BMS_mainFunction(void)
{
    updateStateTransVariables();
    thisBmsStateMachine();
    reflashLED();
}

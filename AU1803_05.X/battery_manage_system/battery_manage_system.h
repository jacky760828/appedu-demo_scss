/******************************************************************************
 *
 * FILE: battery_manage_system.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef BATTERY_MANAGE_SYSTEM_H
#define	BATTERY_MANAGE_SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../main_app/platform_types.h"
#include "battery_manage_system_private.h"
#include "../sys_time/sys_time.h"
#include "pack_config.h"
#include <stdbool.h>
#include "../main_app/version_config.h"
#include "SOH.h"
#include "SOC.h"

#define RBC_DSGFCC                                  330
#define XBP_DSGFCC                                  1050
#define RBC_TRICKLE_CHARGE_CURRENT_20_PERCENT       200     // 200mA
#define XBP_TRICKLE_CHARGE_CURRENT_20_PERCENT       400     // 400mA
#define RC_MAX                                      94      //%
#define RC_MIN                                      8       //%
    
#define Debug_CAN                       0
#define Debug_State                     1         // Debug State Diagram Mode 1:Enable  0:Disable    
#define LDAT_TOTAL_BYTES                72
#define LDAT_DISCHARGING_MAX_TIME       0X7FFFF1C0
#define LDAT_CHARGING_MAX_TIME          0X7FFFF1C0

#define VBUS_LOW_CHECK_TH               35.0f //V; original: 40V by hugo 20210208 
#define VBUS_LOW_CHECK_TH1              30.0f //V;

#define BMS_IDLE_IBATT                  4      //mA;

#define NOWAKEUP_time10S_cut            10      //10S
#define NOWAKEUP_time10min_cut          10*60   //10min

typedef union
{
    UINT8 all;
    struct
    {
        UINT8   DFET    :1;
        UINT8	CFET    :1;
        UINT8   TRICKLE :1;
        UINT8	FAULT   :1;
        UINT8   Reserved:4;
    };
} BATTERY_STATU_TypeDef;


typedef enum {
    VBATT_BUS_more_54,
    VBATT_BUS_more_30,
    VBATT_BUS_less_30,
    VBATT_BUS_less_30_10S,
    VBATT_BUS_more_VBATT,
    VBATT_BUS_less_VBATT_VTH1,
    VBATT_BUS_less_VBATT_VTH3,
    VBATT_BUS_more_VBATT_VTH1,
    IBATT_less_minus3000,
    IBATT_less_minus3000_1S,
    IBATT_more_plus500,
    IBATT_less_percentage20current,
    WAKEUP,
    NO_WAKEUP_10S,
    NO_WAKEUP_10min,
    LOSS_COMM,
    RSOC_more_30,
    RSOC_more_50,
    RSOC_less_29,
    RSOC_less_50,
    RSOC_less_95,
    ALL
} OPERATING_STATU_t;

typedef union
{
    UINT32 all;
    struct
    {
        UINT8   VBATT_BUS_more_54               :1;
        UINT8	VBATT_BUS_more_30               :1;
        UINT8   VBATT_BUS_less_30               :1;
        UINT8   VBATT_BUS_less_30_10S           :1;
        UINT8	VBATT_BUS_more_VBATT            :1;
        UINT8	VBATT_BUS_less_VBATT_VTH1       :1;
        UINT8	VBATT_BUS_less_VBATT_VTH3       :1;
        UINT8	VBATT_BUS_more_VBATT_VTH1       :1;
        
        UINT8	IBATT_less_minus3000            :1;
        UINT8	IBATT_less_minus3000_1S         :1;
        UINT8	IBATT_more_plus500              :1;
        UINT8   IBATT_less_percentage20current  :1;
        UINT8   WAKEUP                          :1;
        UINT8   NO_WAKEUP_10S                   :1;
        UINT8   NO_WAKEUP_10min                 :1;
        UINT8   LOSS_COMM                       :1;
        
        UINT8   RSOC_more_30                    :1;
        UINT8   RSOC_more_50                    :1;
        UINT8   RSOC_less_29                    :1;
        UINT8   RSOC_less_50                    :1;
        UINT8   RSOC_less_95                    :1;
        UINT16   Reserved                       :15;
    };
} OPERATING_STATU_TypeDef;

void BMS_Set_firstCanCommect(bool onoff);
void BMS_Set_Operating_Flag(OPERATING_STATU_t Flag, bool state);
BmsState_t BMS_Get_Statu(void);
void BMS_Set_Statu_CFET(BOOL statu);
BOOL BMS_Get_Statu_CFET(void);
void BMS_Set_Statu_DFET(BOOL statu);
BOOL BMS_Get_Statu_DFET(void);
void BMS_Set_Statu_TRICKLE(BOOL statu);
BOOL BMS_Get_Statu_TRICKLE(void);
void BMS_Set_Statu_FAULT(BOOL statu);
BOOL BMS_Get_Statu_FAULT(void);

void BMS_Electrical_Update(void);
extern void BMS_init(void);
extern void BMS_mainFunction(void);
#ifdef EVENT_LOG
#include "../EventLogData/EventLog.h" 

#endif

extern const UINT8 skuName[];
extern UINT16 cellTemp;// unit .1oK
extern int32_t iBatt; //unit 10mA
extern float vBus;
extern float vBat;
extern int16_t CellTempature_Max,CellTempature_Min,CellTempature_Avg;
extern int16_t CellTempature;
extern int16_t FetTempature;
extern BatteryManageSystem_t d_thisBmsState;
extern float cellVmax, cellVmin, cellVavg, cellVmaxmin;
extern CutTime_t NOWAKEUP_time;
extern UINT16 Ldat_DCIRString;
extern UINT16 Ldat_DCIRStringMin;
extern INT16 iOverTrickleChrgCurr;
extern uint16_t RemainTimeCnt;
extern UINT8 EventSOH;
extern UINT32 CntEventLength;
extern uint32_t CHARGE_TimeS;
extern uint32_t DISCHARGE_TimeS;
extern uint32_t IDLE_TimeS;
extern BOOL d_flagRedLedSolidCtrl;
extern BOOL d_flagRedLedBlinkCtrl;
extern BOOL d_flagGreenLedSolidCtrl;
extern BOOL d_flagGreenLedBlinkCtrl;
extern uint16_t d_DSGFCC;
extern uint8_t Gague_SOC;
extern uint16_t Gague_FCC;
extern uint16_t Gague_RC;
extern uint8_t Calculation_SOC;
extern uint16_t Calculation_FCC;
extern uint16_t Calculation_RC;
extern uint8_t Out_SOC;
extern uint16_t Out_FCC;
extern uint16_t Out_RC;
extern bool d_chrgStatus;

void setOperatingBmsState(BmsState_t desiredState);
uint32_t BMS_Get_Operating_Flag(OPERATING_STATU_t Flag);
uint16_t Time_Dis_ChargingTotal(void);
#ifdef	__cplusplus
}
#endif

#endif	/* BATTERY_MANAGE_SYSTEM_H */


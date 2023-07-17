/******************************************************************************
 *
 * FILE: battery_manage_system_private.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef BATTERY_MANAGE_SYSTEM_PRIVATE_H
#define	BATTERY_MANAGE_SYSTEM_PRIVATE_H


#ifdef	__cplusplus
extern "C" {
#endif

#include "../main_app/platform_types.h"
#ifdef EVENT_LOG 
#include "../EventLogData/EventLog.h"

#define CAN_LOSS        0X01
#define CAN_RECOVER     0x02
#define POWER_ON        0X04
#define POWER_OFF       0X08
#define STATE_CHANGE    0X10
#define FOLLOW_CMD      0X20
#define REGULAR1        0X40
#define REGULAR2        0X80   
#endif    
typedef enum {
    BMSState_STANDBY                = 0x00,
    BMSState_DISCHARGE_STANDBY      = 0x01,
    BMSState_CHARGE_RUN             = 0x02,
    BMSState_DISCHARGE_RUN          = 0x03,
    BMSState_CHARGE_IMBALANCE       = 0x04,
    BMSState_BALANCING              = 0x05,
    BMSState_PERMANENT_FAULT        = 0x08,
    BMSState_OFF                    = 0xFF
} BmsState_t;


#define  BMSState_STANDBY_VALUE_0                           0
#define  BMSState_DISCHARGE_ONLY_VALUE_1                    1
#define  BMSState_STANDBY_VALUE_4                           4
#define  BMSState_CHARGE_ONLY_VALUE_2                       2
#define  BMSState_NORMAL_VALUE_3                            3
#define  BMSState_DISCHARGE_ONLY_VALUE_5                    5
#define  BMSState_PERMANENT_FAULT_F0_VALUE_8                8
#define  BMSState_OFF_VALUE_0                               0

typedef enum {
    BmsCFET_OFF = 0,
    BmsCFET_FREE = 1
}BmsCFETCmd_t;

typedef enum {
    BmsDFET_OFF = 0,
    BmsDFET_FREE = 1
}BmsDFETCmd_t;

typedef struct{
    unsigned disChrgHiTemperature       :1;		// high temperature is detected in discharge mode
    unsigned disChrgLoTemperature       :1;		// low temperature is detected in discharge mode
    unsigned chrgHiTemperature          :1;		// high temperature is detected in charge mode
    unsigned chrgLoTemperature          :1;		// low temperature is detected in charge mode
    unsigned cellHiTemperature          :1;     // higher cell(s) temperature is detected
    
    unsigned reserved1                  :3;
}BmsSvdTempBits_t;

typedef union{
    UINT8 all;
    BmsSvdTempBits_t bit;
}BmsSvdTemp_t;

typedef struct{
    unsigned packOverVoltage             :1; // Pack Over voltage is detected, POV
    unsigned cellOverVoltage             :1; // Cell Over voltage is detected, COV
    unsigned packUnderVoltage            :1; // Pack Under voltage is detected, PUV
    unsigned cellUnderVoltage            :1; // Cell Under voltage is detected, CUV
        
    unsigned vBusLessVBat               :1;     // bus voltage equals or small than battery voltage
    unsigned vBusGtVBat                 :1;     // bus voltage great than battery voltage
    unsigned vBusTooLow                 :1;     // bus voltage too low is detected
    unsigned vBusTooLowLongTime         :1; // bus voltage too low is detected for a long time = 10 min
}BmsSvdVoltBits_t;

typedef union{
    UINT8 all;
    BmsSvdVoltBits_t bit;
}BmsSvdVolt_t;

typedef struct{
    unsigned vBusGtVBatSubVth           :1;		// bus voltage great than (VBAT-Vth)
    unsigned vBusGtVBatPlusVth          :1;		// bus voltage great than (VBAT+Vth) //20200423 Alan Chen modify for new State Diagram, Add 
    unsigned vBusLessVBatPlusVth        :1;     // bus voltage less than (VBAT+Vth)
    unsigned vBusLessVBatPlus3V         :1;     // bus voltage less than (VBAT+3V)
    
    unsigned vBusLess30V                :1;     // bus voltage less than 30V
    unsigned reserved1                  :3;                                         //20200423 Alan Chen modify for new State Diagram, Modify
}BmsSvdVolt2Bits_t;

typedef union{
    UINT8 all;
    BmsSvdVolt2Bits_t bit;
}BmsSvdVolt2_t;

typedef struct{
    unsigned disChrgOvrCurrent          :1;     // over current is detected in discharge mode
    unsigned chrgOvrCurrent             :1;     // over current is detected in charge mode
    unsigned iBattGtDischrgCurr         :1;     // battery discharge current great than specified current limit
    unsigned iBattLessMinChrgCurr       :1;     // battery charge current less than min. charge current
    
    unsigned iBattLessTrickle20ChrgCurr :1;     // battery charge current less than 20% of trickle charge current
    unsigned iBattGtZeroCharCurr        :1;     //20200318 Alan Chen modify for new State Diagram, Add
    unsigned iBattLessZeroDischrgCurr   :1;     //20200318 Alan Chen modify for new State Diagram, Add
    unsigned iBattLessEOCC              :1;     //20200526 Alan Chen add for End Of Charge Current (EOCC)  //20200526 Alan Chen fix EOCV flag, Add
}BmsSvdCurrentBits_t;

typedef union{
    UINT8 all;
    BmsSvdCurrentBits_t bit;
}BmsSvdCurrent_t;

typedef struct{
    unsigned cmdTrickleChrgDisable      :1;     // the command of trickle charge disable is received
    unsigned cmdTrickleChrgEnable       :1;     // the command of trickle charge enable is received
    unsigned cmdDisChrgEnable           :1;     // the command of discharge enable is received
    unsigned cmdDisChrgDisable          :1;     // the command of discharge disable is received
    
    unsigned cmdChrgEnable              :1;     // the command of charge enable is received
    unsigned cmdChrgDisable             :1;     // the command of charge disable is received
    unsigned lostCommunication          :1;     // communication is lost for specified time
    unsigned systemWakeUpDetected       :1;     // the signal of system wakeup is detected
}BmsSvdCmdBits_t;

typedef union{
    UINT8 all;
    BmsSvdCmdBits_t bit;
}BmsSvdCmd_t;

//20200414 Alan Chen modify for new state diagram, Add, Start
typedef struct{
    unsigned NotsystemWakeUpDetectedLongTime :1; // Now get the signal of system wakeup for a long time : 10 min
    unsigned systemNotWakeUpFor10SecDetected :1; //20200429 Alan Chen modify it for New State Diagram, Add
    unsigned reserved1                  :6;
}BmsSvdCmdExtend1Bits_t;

typedef union{
    UINT8 all;
    BmsSvdCmdExtend1Bits_t bit;
}BmsSvdCmdExtend1_t;
//20200414 Alan Chen modify for new state diagram, Add, End

typedef struct{
    unsigned fullyChrged                :1;     // the battery pack is charged completely
    unsigned reChrgReqstByRSOC          :1;     // re-charge is requested by specified RSOC
    unsigned trickleChrgEnable          :1;     // indicate that trickle charger is enabled
    
    unsigned reserved1                  :5;
}BmsSvdChrgStatusBits_t;

typedef union{
    UINT8 all;
    BmsSvdChrgStatusBits_t bit;
}BmsSvdChrgStatus_t;

typedef struct{
    unsigned cmdTrickleChargerON        :1;     // command the trickle charger on
    
    unsigned reserved1                  :7;
}BmsSvdChrgCmdBits_t;

typedef union{
    UINT8 all;
    BmsSvdChrgCmdBits_t bit;
}BmsSvdChrgCmd_t;

typedef struct{
    unsigned mfrMode                    :1;     // manufacture mode
    
    unsigned reserved1                  :7;
}BmsSvdMfrCmdBits_t;

typedef union{
    UINT8 all;
    BmsSvdMfrCmdBits_t bit;
}BmsSvdMfrCmd_t;

//20200615 Jason Lin add pre-discharge function
typedef struct{
    unsigned Pre_discharge                    :1;     // manufacture mode
    unsigned soft_start                         :1;
    unsigned Pre_discharge_fault            :1;
    unsigned DOC_fault                  :1;
    unsigned reserved1                  :4;
}BmsSvdSoftCmdBits_t;

typedef union{
    UINT8 all;
    BmsSvdSoftCmdBits_t bit;
}BmsSvdSoftCmd_t;

//20200318 Alan Chen modify for new State Diagram, Add, Start
// frank 7/9 modify to long
typedef union {
    struct
    {
        unsigned long fault_VP_COV     :1; // Item  0: 1.Voltage Protect
        unsigned long fault_VP_POV     :1; // Item  1: 2.Voltage Protect
        unsigned long fault_VP_EOCV    :1; // Item  2: 3.Voltage Protect
        unsigned long fault_VP_CUV     :1; // Item  3: 4.Voltage Protect
        unsigned long fault_VP_PUV     :1; // Item  4: 5.Voltage Protect
        unsigned long fault_VP_OIMBV   :1; // Item  5: 6.Voltage Protect
        unsigned long fault_CP_OCI     :1;   // Item  6: 7.Current Protect
        unsigned long fault_CP_ODI_1   :1;   // Item  7: 8.Current Protect        
        unsigned long fault_CP_ODI_2   :1;   // Item  8: 9.Current Protect
        unsigned long fault_CP_ODI_3   :1;   // Item  9: 10.Current Protect
        unsigned long fault_CP_ODI_4   :1;   // Item  10: 11.Current Protect
        unsigned long fault_CP_H_ODI   :1;   // Item  11: 12.Current Protect
        unsigned long fault_CP_H_SCI   :1;   // Item  12: 13.Current Protect
        unsigned long fault_TP_COT     :1;      // Item  13: 14.Temperature Protect
        unsigned long fault_TP_DOT     :1;      // Item  14: 15.Temperature Protect
        unsigned long fault_TP_CUT     :1;      // Item  15: 16.Temperature Protect        
        unsigned long fault_TP_DUT     :1;      // Item  16: 17.Temperature Protect
        unsigned long fault_TP_MOT     :1;      // Item  17: 18.Temperature Protect
        unsigned long fault_PFP_UVPF   :1;        // Item  18: 19.Permanent Failure Protect
        unsigned long fault_PFP_OVPF   :1;        // Item  19: 20.Permanent Failure Protect
        unsigned long fault_PFP_OTPF   :1;        // Item  20: 21.Permanent Failure Protect
        unsigned long fault_PFP_FUSEPF :1;        // Item  21: 22.Permanent Failure Protect
        unsigned long fault_PFP_CIMPF  :1;        // Item  22: 23.Permanent Failure Protect
        unsigned long fault_PFP_THOPF  :1;        // Item  23: 24.Permanent Failure Protect        
        unsigned long fault_PFP_THSPF  :1;        // Item  24: 25.Permanent Failure Protect
        unsigned long fault_PFP_CFETPF :1;        // Item  25: 26.Permanent Failure Protect
        unsigned long fault_PFP_DFETPF :1;        // Item  26: 27.Permanent Failure Protect
        unsigned long fault_ULP_BQ_F         :1;    // Item  27: 28.UL670730 Protect
        unsigned long fault_ULP_MCU_IO_F     :1;    // Item  28: 29.UL670730 Protect
        unsigned long fault_ULP_MCU_CLK_F    :1;    // Item  29: 30.UL670730 Protect
        unsigned long fault_ULP_MCU_PC_F     :1;    // Item  30: 31.UL670730 Protect
        unsigned long fault_ULP_MCU_REG_F    :1;    // Item  31: 32.UL670730 Protect 
        unsigned char fault_ULP_MCU_WDT_F    :1;    // Item  32: 33.UL670730 Protect
        unsigned char fault_ULP_MCU_RAM_F    :1;    // Item  33: 34.UL670730 Protect
        unsigned char fault_ULP_MCU_FLASH_F  :1;    // Item  34: 35.UL670730 Protect
        unsigned char fault_ULP_MCU_EE_F     :1;    // Item  35: 36.UL670730 Protect
        unsigned char reserved1              :4;
    } __attribute__ ((__packed__)) AllBits;
    struct {
        unsigned long fault_VP : 6;
        unsigned long fault_CP : 7;
        unsigned long fault_TP : 5;
        unsigned long fault_PFP : 9;
        unsigned long fault_ULP0 : 5;
        unsigned char fault_ULP1 : 4;
        unsigned char bmsState : 4;
    } __attribute__ ((__packed__)); 
    // This is to allow individual byte access within its id.
    struct
    {
        UINT8 array[5];
    } AllBytes;
}BmsFaultFlag_t;
//20200318 Alan Chen modify for new State Diagram, Add, End

typedef union {
    unsigned int Status;
    struct{
        unsigned int BQFbit:1;
        unsigned int IOFbit:1;
        unsigned int CLKFbit:1;
        unsigned int PCFbit:1;
        unsigned int REGFbit:1;
        unsigned int WDTFbit:1;
        unsigned int RAMFbit:1;
        unsigned int FLASHFbit:1;
        
        unsigned int EEPROMFbit:1;
        unsigned int CFETFbit:1;
        unsigned int DFETFbit:1;
    };
}MCU_FAULT;

typedef struct {
	BmsState_t          currentState;
	BmsState_t          previousState;	           
    
    UINT32              stateTime;
    UINT32              stateStableTime;

    BmsSvdTemp_t        svdTemperature;
    BmsSvdVolt_t        svdVoltage;
    BmsSvdVolt2_t       svdVoltage2;
    BmsSvdCurrent_t     svdCurrent;
    BmsSvdCmd_t         svdCommand;
    BmsSvdCmdExtend1_t  svdCommandExtend1;
    BmsSvdChrgStatus_t  svdChargeSts;    
    BmsSvdChrgCmd_t     svdChargeCmd;
    BmsSvdMfrCmd_t      svdManufacturCmd;
    BmsFaultFlag_t      svdFaultFlag; //20200318 Alan Chen modify for new State Diagram, Add
    BmsSvdSoftCmd_t     BmsSvdSoftCmd; //20200615
}BatteryManageSystem_t;

#define Watchdog_Reset_Counter_Max 65006


#ifdef	__cplusplus
}
#endif

#endif	/* BATTERY_MANAGE_SYSTEM_PRIVATE_H */


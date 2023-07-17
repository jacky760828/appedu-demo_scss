/******************************************************************************
 *
 * FILE: smbus_cmd.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/

#ifndef SMBUS_CMD_H
#define	SMBUS_CMD_H

#ifdef	__cplusplus
extern "C" {
#endif

//-----------------------------------------------
//	SMBus Protocols
//-----------------------------------------------
//      Read write  BLOCK/WORD/BYTE
//0000    0    0         00
#define BYTE        0x01
#define WORD        0x02
#define BLOCK       0x03
#define WRITE_BYTE  0x05
#define WRITE_WORD  0x06
#define WRITE_BLOCK 0x07
#define READ_BYTE   0x09
#define READ_WORD   0x0A
#define READ_BLOCK  0x0B
#define RW_BYTE     0x0D
#define RW_WORD     0x0E
#define RW_BLOCK    0x0F

//--------------------------------------------------
//	Command Codes
//--------------------------------------------------
/* 0x44 ManufacturerBlockAccess() 0x44CMD */
#define SBS_CMD                                 0xFF
#define MANUFACTURER_ACCESS_CMD                 0x00
#define MANUFACTURER_BLOCK_ACCESS_CMD           0x44

#define SBS_RemainingCapacityAlarm              0x01
#define SBS_RemainingTimeAlarm                  0x02
#define SBS_BatteryMode                         0x03
#define SBS_AtRate                              0x04
#define SBS_AtRateTimeToFull                    0x05
#define SBS_AtRateTimeToEmpty                   0x06
#define SBS_AtRateOK                            0x07
#define SBS_Temperature                         0x08
#define SBS_Voltage                             0x09
#define SBS_Current                             0x0A
#define SBS_AverageCurrent                      0x0B
#define SBS_MaxError                            0x0C
#define SBS_RelativeStateOfCharge               0x0D
#define SBS_AbsoluteStateOfCharge               0x0E
#define SBS_RemainingCapacity                   0x0F
#define SBS_FullChargeCapacity                  0x10
#define SBS_RunTimeToEmpty                      0x11
#define SBS_AverageTimeToEmpty                  0x12
#define SBS_AverageTimeToFull                   0x13
#define SBS_ChargingCurrent                     0x14
#define SBS_ChargingVoltage                     0x15
#define SBS_BatteryStatus                       0x16
#define SBS_CycleCount                          0x17
#define SBS_DesignCapacity                      0x18
#define SBS_DesignVoltage                       0x19
#define SBS_SpecificationInfo                   0x1A
#define SBS_ManufacturerDate                    0x1B
#define SBS_SerialNumber                        0x1C
#define SBS_ManufacturerName                    0x20
#define SBS_DeviceName                          0x21
#define SBS_DeviceChemistry                     0x22
#define SBS_ManufacturerData                    0x23
#define SBS_HostFETControl                      0x2B
#define SBS_GPIOStatus                          0x2C
#define SBS_GPIOControl                         0x2D
#define SBS_VAUXVoltage                         0x2E
#define SBS_Authenticate                        0x2F
#define SBS_Reserved                            0x30
#define SBS_CellVoltage15                       0x31
#define SBS_CellVoltage14                       0x32
#define SBS_CellVoltage13                       0x33
#define SBS_CellVoltage12                       0x34
#define SBS_CellVoltage11                       0x35
#define SBS_CellVoltage10                       0x36
#define SBS_CellVoltage9                        0x37
#define SBS_CellVoltage8                        0x38
#define SBS_CellVoltage7                        0x39
#define SBS_CellVoltage6                        0x3A
#define SBS_CellVoltage5                        0x3B
#define SBS_CellVoltage4                        0x3C
#define SBS_CellVoltage3                        0x3D
#define SBS_CellVoltage2                        0x3E
#define SBS_CellVoltage1                        0x3F
#define SBS_DynamicPower                        0x4C
#define SBS_ExtAveCellVoltage                   0x4D
#define SBS_PendingEDV                          0x4E
#define SBS_StateOfHealth                       0x4F
#define SBS_SafetyAlert                         0x50
#define SBS_SafetyStatus                        0x51
#define SBS_PFAlert                             0x52
#define SBS_PFStatus                            0x53
#define SBS_OperationStatus                     0x54
#define SBS_ChargingStatus                      0x55
#define SBS_GaugingStatus                       0x56
#define SBS_ManufacturingStatus                 0x57
#define SBS_AFEStatus                           0x58
#define SBS_AFEConfig                           0x59
#define SBS_AFEVCx                              0x5A
#define SBS_AFEData                             0x5B
#define SBS_Lifetime_Data_Block_1               0x60
#define SBS_Lifetime_Data_Block_2               0x61
#define SBS_Lifetime_Data_Block_3               0x62
#define SBS_Lifetime_Data_Block_4               0x63
#define SBS_Lifetime_Data_Block_5               0x64
#define SBS_Lifetime_Data_Block_6               0x65
#define SBS_Lifetime_Data_Block_7               0x66
#define SBS_ManufacturerInfo                    0x70
#define SBS_DAStatus1                           0x71
#define SBS_DAStatus2                           0x72
#define SBS_CUV                                 0x80
#define SBS_COV                                 0x81

/*0x00 ManufacturerAccess() */
#define DeviceType                          0x0001
#define FirmwareVersion                     0x0002
#define HardwareVersion                     0x0003
#define IFChecksum                          0x0004
#define StaticDFSignature                   0x0005
#define ChemID                              0x0006
#define StaticChemDFSignature               0x0008
#define AllDFSignature                      0x0009
#define ShutdownMode                        0x0010
#define SleepMode                           0x0011
#define CellBalanceToggle                   0x001B
#define AFEDelayDisable                     0x001C
#define SAFEToggle                          0x001D
#define PrechargeFET                        0x001E
#define ChargeFET                           0x001F
#define DischargeFET                        0x0020
#define Gauging                             0x0021
#define LifetimeDataCollection              0x0023
#define PermanentFailure                    0x0024
#define BlackBoxRecorder                    0x0025
#define SAFE                                0x0026
#define LEDDisplayEnable                    0x0027
#define LifetimeDataReset                   0x0028
#define PermanentFailureDataReset           0x0029
#define BlackBoxRecorderReset               0x002A
#define LED                                 0x002B
#define LEDDisplayPress                     0x002C
#define CalibrationMode                     0x002D
#define SealDevice                          0x0030
#define SecurityKeys                        0x0035
#define AuthenticationKey                   0x0037
#define DeviceReset                         0x0041
#define SafetyAlert                         0x0050
#define SafetyStatus                        0x0051
#define PFAlert                             0x0052
#define PFStatus                            0x0053
#define OperationStatus                     0x0054
#define ChargingStatus                      0x0055
#define GaugingStatus                       0x0056
#define ManufacturingStatus                 0x0057
#define AFEStatus                           0x0058
#define AFEConfig                           0x0059
#define AFEVCx                              0x005A
#define AFEData                             0x005B
#define LifetimeDataBlock1                  0x0060
#define LifetimeDataBlock2                  0x0061
#define LifetimeDataBlock3                  0x0062
#define LifetimeDataBlock4                  0x0063
#define LifetimeDataBlock5                  0x0064
#define LifetimeDataBlock6                  0x0065
#define LifetimeDataBlock7                  0x0066
#define ManufacturerInfo                    0x0070
#define DAStatus1                           0x0071
#define DAStatus2                           0x0072
#define CUVSnapshot                         0x0080
#define COVSnapshot                         0x0081
#define ROMMode                             0x0F00
#define ExitCalibrationOutput               0xF080
#define OutputCellVoltageforCalibration     0xF081
#define OutputCellVoltageCCandTemp          0xF082
//--------------------------------------------------
//	Command DATA
//--------------------------------------------------
#define UNSEAL_KEY1                         0x0414
#define UNSEAL_KEY2                         0x3672
#define UNFullSEAL_KEY1                     0x0414
#define UNFullSEAL_KEY2                     0x3672

extern uint8_t SMBUS_ManufacturerAccess[2];
extern uint8_t SMBUS_remainingCapacityAlarm[2];
extern uint8_t SMBUS_remainingTimeAlarm[2];
extern uint8_t SMBUS_batteryMode[2]; 
extern uint8_t SMBUS_atRate[2];
extern uint8_t SMBUS_atRateTimeToFull[2];
extern uint8_t SMBUS_atRateTimeToEmpty[2];
extern uint8_t SMBUS_atRateOK[2];
extern uint8_t SMBUS_temperature[2];
extern uint8_t SMBUS_voltage[2];
extern uint8_t SMBUS_current[2];
extern uint8_t SMBUS_averageCurrent[2];
extern uint8_t SMBUS_maxError[2];
extern uint8_t SMBUS_relativeStateOfChare[2];
extern uint8_t SMBUS_absoluteStateOfCharge[2];
extern uint8_t SMBUS_remainingCapacity[2];
extern uint8_t SMBUS_fullChargeCapacity[2];
extern uint8_t SMBUS_runTimeToEmpty[2];
extern uint8_t SMBUS_averageTimeToEmpty[2];
extern uint8_t SMBUS_averageTimeToFull[2];
extern uint8_t SMBUS_chargingCurrent[2];
extern uint8_t SMBUS_chargingVoltage[2];
extern uint8_t SMBUS_batteryStatus[2];
extern uint8_t SMBUS_cycleCount[2];
extern uint8_t SMBUS_designCapacity[2];
extern uint8_t SMBUS_designVoltage[2];
extern uint8_t SMBUS_specInfo[2];
extern uint8_t SMBUS_manufactureDate[2];
extern uint8_t SMBUS_serialNumber[2]; //e
extern uint8_t SMBUS_manufactureName[20];
extern uint8_t SMBUS_deviceName[8];
extern uint8_t SMBUS_deviceChemistry[4];
extern uint8_t SMBUS_manufactureData[32];
extern uint8_t SMBUS_hostFetControl[2];
extern uint8_t SMBUS_gpioStatus[2];
extern uint8_t SMBUS_gpioControl[2];
extern uint8_t SMBUS_vauxVoltage[2];
extern uint8_t SMBUS_authenticate[20];
extern uint8_t SMBUS_cellVoltage[15][2];
extern uint8_t SMBUS_dynamicPower[2];
extern uint8_t SMBUS_extAveCellVoltage[2];
extern uint8_t SMBUS_pendingEdv[2];
extern uint8_t SMBUS_stateOfHeath[2];

extern uint8_t SMBUS_safetyAlert[4];
extern uint8_t SMBUS_safetyStatus[4];
extern uint8_t SMBUS_pfAlert[4];
extern uint8_t SMBUS_pfStatus[4];
extern uint8_t SMBUS_operationStatus[4];
extern uint8_t SMBUS_chargingStatus[2];
extern uint8_t SMBUS_gaugingStatus[2];
extern uint8_t SMBUS_manufacturingStatus[2];
extern uint8_t SMBUS_afeStatus[2];
extern uint8_t SMBUS_afeConfig[12];
extern uint8_t SMBUS_afeVcx[32];
extern uint8_t SMBUS_afeData[14];
extern uint8_t SMBUS_lifeTimeDataBlock1[32];
extern uint8_t SMBUS_lifeTimeDataBlock2[32];
extern uint8_t SMBUS_lifeTimeDataBlock3[14];
extern uint8_t SMBUS_lifeTimeDataBlock4[20];
extern uint8_t SMBUS_lifeTimeDataBlock5[18];
extern uint8_t SMBUS_lifeTimeDataBlock6[32];
extern uint8_t SMBUS_lifeTimeDataBlock7[32];
extern uint8_t SMBUS_manufactureInfo[32];
extern uint8_t SMBUS_daStatus1[32];
extern uint8_t SMBUS_daStatus2[16];
extern uint8_t SMBUS_cuvSnapShot[32];
extern uint8_t SMBUS_covSnapShot[32];

typedef struct __attribute__ ((__packed__)) {		
	uint8_t commandCode;
	uint8_t protocol;
	uint8_t byteNumber;
	uint8_t *ptrCommandData;
} command;

typedef struct __attribute__ ((__packed__)) {		
	uint16_t commandCode;
	uint8_t protocol;
	uint8_t byteNumber;
	uint8_t *ptrCommandData;
} ManufacturerAccesscommand;

extern uint16_t BQ78350SBSCmd_MAXSIZE;
extern uint16_t BQ78350ManufacturerAccessCmd_MAXSIZE;

extern uint8_t smbusBQ78350_SBSCmdlist[];
extern const command smbusMatrixCmd[];
extern const ManufacturerAccesscommand BQ78350ManufacturerAccessCmd[];

#ifdef	__cplusplus
}
#endif

#endif	/* SMBUS_CMD_H */
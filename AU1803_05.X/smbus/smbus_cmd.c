/*
 * File:   smbcmdlist.c
 * Author: frank.tsai
 *
 * Created on May 20, 2020, 9:05 AM
 */

#include "smbus.h"

uint8_t SMBUS_ManufacturerAccess[2];
uint8_t SMBUS_remainingCapacityAlarm[2];
uint8_t SMBUS_remainingTimeAlarm[2];
uint8_t SMBUS_batteryMode[2]; 
uint8_t SMBUS_atRate[2];
uint8_t SMBUS_atRateTimeToFull[2];
uint8_t SMBUS_atRateTimeToEmpty[2];
uint8_t SMBUS_atRateOK[2];
uint8_t SMBUS_temperature[2];
uint8_t SMBUS_voltage[2];
uint8_t SMBUS_current[2];
uint8_t SMBUS_averageCurrent[2];
uint8_t SMBUS_maxError[2];
uint8_t SMBUS_relativeStateOfChare[2];
uint8_t SMBUS_absoluteStateOfCharge[2];
uint8_t SMBUS_remainingCapacity[2];
uint8_t SMBUS_fullChargeCapacity[2];
uint8_t SMBUS_runTimeToEmpty[2];
uint8_t SMBUS_averageTimeToEmpty[2];
uint8_t SMBUS_averageTimeToFull[2];
uint8_t SMBUS_chargingCurrent[2];
uint8_t SMBUS_chargingVoltage[2];
uint8_t SMBUS_batteryStatus[2];
uint8_t SMBUS_cycleCount[2];
uint8_t SMBUS_designCapacity[2];
uint8_t SMBUS_designVoltage[2];
uint8_t SMBUS_specInfo[2];
uint8_t SMBUS_manufactureDate[2];
uint8_t SMBUS_serialNumber[2]; //e
uint8_t SMBUS_manufactureName[20];
uint8_t SMBUS_deviceName[8];
uint8_t SMBUS_deviceChemistry[4];
uint8_t SMBUS_manufactureData[32];
uint8_t SMBUS_hostFetControl[2];
uint8_t SMBUS_gpioStatus[2];
uint8_t SMBUS_gpioControl[2];
uint8_t SMBUS_vauxVoltage[2];
uint8_t SMBUS_authenticate[20];
uint8_t SMBUS_cellVoltage[15][2];
uint8_t SMBUS_dynamicPower[2];
uint8_t SMBUS_extAveCellVoltage[2];
uint8_t SMBUS_pendingEdv[2];
uint8_t SMBUS_stateOfHeath[2];

uint8_t SMBUS_safetyAlert[4];
uint8_t SMBUS_safetyStatus[4];
uint8_t SMBUS_pfAlert[4];
uint8_t SMBUS_pfStatus[4];
uint8_t SMBUS_operationStatus[4];
uint8_t SMBUS_chargingStatus[2];
uint8_t SMBUS_gaugingStatus[2];
uint8_t SMBUS_manufacturingStatus[2];
uint8_t SMBUS_afeStatus[2];
uint8_t SMBUS_afeConfig[12];
uint8_t SMBUS_afeVcx[32];
uint8_t SMBUS_afeData[14];
uint8_t SMBUS_lifeTimeDataBlock1[32];
uint8_t SMBUS_lifeTimeDataBlock2[32];
uint8_t SMBUS_lifeTimeDataBlock3[14];
uint8_t SMBUS_lifeTimeDataBlock4[20];
uint8_t SMBUS_lifeTimeDataBlock5[18];
uint8_t SMBUS_lifeTimeDataBlock6[32];
uint8_t SMBUS_lifeTimeDataBlock7[32];
uint8_t SMBUS_manufactureInfo[32];
uint8_t SMBUS_daStatus1[32];
uint8_t SMBUS_daStatus2[16];
uint8_t SMBUS_cuvSnapShot[32];
uint8_t SMBUS_covSnapShot[32];

uint8_t smbusBQ78350_SBSCmdlist[] = {
    MANUFACTURER_ACCESS_CMD,
    SBS_RemainingCapacityAlarm,
    SBS_RemainingTimeAlarm,
    SBS_BatteryMode,
    SBS_AtRate,
    SBS_AtRateTimeToFull,
    SBS_AtRateTimeToEmpty,
    SBS_AtRateOK,
    SBS_Temperature,
    SBS_Voltage,
    SBS_Current,
    SBS_AverageCurrent,
    SBS_MaxError,
    SBS_RelativeStateOfCharge,
    SBS_AbsoluteStateOfCharge,
    SBS_RemainingCapacity,
    SBS_FullChargeCapacity,
    SBS_RunTimeToEmpty,
    SBS_AverageTimeToEmpty,
    SBS_AverageTimeToFull,
    SBS_ChargingCurrent,
    SBS_ChargingVoltage,
    SBS_BatteryStatus,
    SBS_CycleCount,
    SBS_DesignCapacity,
    SBS_DesignVoltage,
    SBS_SpecificationInfo,
    SBS_ManufacturerDate,
    SBS_SerialNumber,
    SBS_ManufacturerName,
    SBS_DeviceName,
    SBS_DeviceChemistry,
//    SBS_ManufacturerData,
    SBS_HostFETControl,
    SBS_GPIOStatus,
    SBS_GPIOControl,
    SBS_VAUXVoltage,
    SBS_Authenticate,
//    SBS_Reserved,
    SBS_CellVoltage15,
    SBS_CellVoltage14,
    SBS_CellVoltage13,
    SBS_CellVoltage12,
    SBS_CellVoltage11,
    SBS_CellVoltage10,
    SBS_CellVoltage9,
    SBS_CellVoltage8,
    SBS_CellVoltage7,
    SBS_CellVoltage6,
    SBS_CellVoltage5,
    SBS_CellVoltage4,
    SBS_CellVoltage3,
    SBS_CellVoltage2,
    SBS_CellVoltage1,
    SBS_DynamicPower,
    SBS_ExtAveCellVoltage,
    SBS_PendingEDV,
    SBS_StateOfHealth,
    SBS_SafetyAlert,
    SBS_SafetyStatus,
    SBS_PFAlert,
    SBS_PFStatus,
    SBS_OperationStatus,
    SBS_ChargingStatus,
    SBS_GaugingStatus,
    SBS_ManufacturingStatus,
    SBS_AFEStatus,
    SBS_AFEConfig,
//    SBS_AFEVCx,
    SBS_AFEData,
    SBS_Lifetime_Data_Block_1,
    SBS_Lifetime_Data_Block_2,
    SBS_Lifetime_Data_Block_3,
    SBS_Lifetime_Data_Block_4,
    SBS_Lifetime_Data_Block_5,
    SBS_Lifetime_Data_Block_6,
    SBS_Lifetime_Data_Block_7,
    SBS_ManufacturerInfo,
    SBS_DAStatus1,
    SBS_DAStatus2,
    SBS_CUV,
    SBS_COV
};

const command smbusMatrixCmd[] = {
//	---------------------Command Number---------------------Protocol-------Byte Number---------Command RAM Index---------------Command Name----------------Command Type-------
    {MANUFACTURER_ACCESS_CMD,   RW_WORD,	2,	SMBUS_ManufacturerAccess},
    {SBS_RemainingCapacityAlarm,RW_WORD,	2,	SMBUS_remainingCapacityAlarm},
    {SBS_RemainingTimeAlarm,    RW_WORD,	2,	SMBUS_remainingTimeAlarm},
    {SBS_BatteryMode,           RW_WORD,	2,	SMBUS_batteryMode},
    {SBS_AtRate,                RW_WORD,	2,	SMBUS_atRate},
    {SBS_AtRateTimeToFull,      READ_WORD,	2,	SMBUS_atRateTimeToFull},
    {SBS_AtRateTimeToEmpty,     READ_WORD,	2,	SMBUS_atRateTimeToEmpty},
    {SBS_AtRateOK,              READ_WORD,	2,	SMBUS_atRateOK},
    {SBS_Temperature,           READ_WORD,	2,	SMBUS_temperature},
    {SBS_Voltage,               READ_WORD,	2,  SMBUS_voltage},
    {SBS_Current,               READ_WORD,	2,	SMBUS_current},
    {SBS_AverageCurrent,        READ_WORD,	2,	SMBUS_averageCurrent},
    {SBS_MaxError,              READ_BYTE,	2,	SMBUS_maxError},
    {SBS_RelativeStateOfCharge,	READ_BYTE,	2,	SMBUS_relativeStateOfChare},
    {SBS_AbsoluteStateOfCharge,	READ_BYTE,	2,	SMBUS_absoluteStateOfCharge},
    {SBS_RemainingCapacity,     RW_WORD,	2,	SMBUS_remainingCapacity},
    {SBS_FullChargeCapacity,    READ_WORD,	2,	SMBUS_fullChargeCapacity},
    {SBS_RunTimeToEmpty,        READ_WORD,	2,	SMBUS_runTimeToEmpty},
    {SBS_AverageTimeToEmpty,    READ_WORD,	2,	SMBUS_averageTimeToEmpty},
    {SBS_AverageTimeToFull,     READ_WORD,	2,	SMBUS_averageTimeToFull},
    {SBS_ChargingCurrent,       READ_WORD,	2,	SMBUS_chargingCurrent},
    {SBS_ChargingVoltage,       READ_WORD,	2,	SMBUS_chargingVoltage},
    {SBS_BatteryStatus,         READ_WORD,	2,	SMBUS_batteryStatus},
    {SBS_CycleCount,            READ_WORD,	2,	SMBUS_cycleCount},
    {SBS_DesignCapacity,        READ_WORD,	2,	SMBUS_designCapacity},
    {SBS_DesignVoltage,         READ_WORD,	2,	SMBUS_designVoltage},
    {SBS_SpecificationInfo,     READ_WORD,	2,	SMBUS_specInfo},
    {SBS_ManufacturerDate,      READ_WORD,	2,	SMBUS_manufactureDate},
    {SBS_SerialNumber,          READ_WORD,	2,	SMBUS_serialNumber},
    {SBS_ManufacturerName,      READ_BLOCK,	11,	SMBUS_manufactureName},
    {SBS_DeviceName,            READ_BLOCK,	7,	SMBUS_deviceName},
    {SBS_DeviceChemistry,      	READ_BYTE,	5,	SMBUS_deviceChemistry},
//    {SBS_ManufacturerData,      READ_BLOCK,	4,	SMBUS_manufactureData},
    {SBS_HostFETControl,      	RW_WORD,	2,	SMBUS_hostFetControl},
    {SBS_GPIOStatus,           	READ_WORD,	2,	SMBUS_gpioStatus},
    {SBS_GPIOControl,          	RW_WORD,	2,	SMBUS_gpioControl},
    {SBS_VAUXVoltage,          	READ_WORD,	2,	SMBUS_vauxVoltage},
    {SBS_Authenticate,          RW_BLOCK,	20,	SMBUS_authenticate},
//    {SBS_Reserved,          	RW_BLOCK,	20,	SMBUS_authenticate},
    {SBS_CellVoltage15,       	READ_WORD,	2,	SMBUS_cellVoltage[15]},
    {SBS_CellVoltage14,       	READ_WORD,	2,	SMBUS_cellVoltage[14]},
    {SBS_CellVoltage13,       	READ_WORD,	2,	SMBUS_cellVoltage[13]},
    {SBS_CellVoltage12,       	READ_WORD,	2,	SMBUS_cellVoltage[12]},
    {SBS_CellVoltage11,       	READ_WORD,	2,	SMBUS_cellVoltage[11]},
    {SBS_CellVoltage10,         READ_WORD,	2,	SMBUS_cellVoltage[10]},
    {SBS_CellVoltage9,          READ_WORD,	2,	SMBUS_cellVoltage[9]},
    {SBS_CellVoltage8,        	READ_WORD,	2,	SMBUS_cellVoltage[8]},
    {SBS_CellVoltage7,          READ_WORD,	2,	SMBUS_cellVoltage[7]},
    {SBS_CellVoltage6,          READ_WORD,	2,	SMBUS_cellVoltage[6]},
    {SBS_CellVoltage5,          READ_WORD,	2,	SMBUS_cellVoltage[5]},
    {SBS_CellVoltage4,          READ_WORD,	2,	SMBUS_cellVoltage[4]},
    {SBS_CellVoltage3,          READ_WORD,	2,	SMBUS_cellVoltage[3]},
    {SBS_CellVoltage2,          READ_WORD,	2,	SMBUS_cellVoltage[2]},
    {SBS_CellVoltage1,          READ_WORD,	2,	SMBUS_cellVoltage[1]},
    {SBS_DynamicPower,          READ_WORD,	2,	SMBUS_dynamicPower},
    {SBS_ExtAveCellVoltage,     READ_WORD,	2,	SMBUS_extAveCellVoltage},
    {SBS_PendingEDV,            READ_WORD,	2,	SMBUS_pendingEdv},
    {SBS_StateOfHealth,       	READ_WORD,	2,	SMBUS_stateOfHeath},
    {SBS_SafetyAlert,           READ_BLOCK,	4,	SMBUS_safetyAlert},
    {SBS_SafetyStatus,          READ_BLOCK,	4,	SMBUS_safetyStatus},
    {SBS_PFAlert,               READ_BLOCK,	4,	SMBUS_pfAlert},
    {SBS_PFStatus,              READ_BLOCK,	4,	SMBUS_pfStatus},
    {SBS_OperationStatus,       READ_BLOCK,	5,	SMBUS_operationStatus},
    {SBS_ChargingStatus,       	READ_BLOCK,	3,	SMBUS_chargingStatus},
    {SBS_GaugingStatus,        	READ_BLOCK,	2,	SMBUS_gaugingStatus},
    {SBS_ManufacturingStatus,  	READ_BLOCK,	2,	SMBUS_manufacturingStatus},
    {SBS_AFEStatus,            	READ_BLOCK,	2,	SMBUS_afeStatus},
    {SBS_AFEConfig,             READ_BLOCK,	11,	SMBUS_afeConfig},
    {SBS_AFEVCx,                READ_BLOCK,	32,	SMBUS_afeVcx},
    {SBS_AFEData,               READ_BLOCK,	14,	SMBUS_afeData},
    {SBS_Lifetime_Data_Block_1,	READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock1},
    {SBS_Lifetime_Data_Block_2,	READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock2},
    {SBS_Lifetime_Data_Block_3,	READ_BLOCK,	14,	SMBUS_lifeTimeDataBlock3},
    {SBS_Lifetime_Data_Block_4,	READ_BLOCK,	20,	SMBUS_lifeTimeDataBlock4},
    {SBS_Lifetime_Data_Block_5,	READ_BLOCK,	18,	SMBUS_lifeTimeDataBlock5},
    {SBS_Lifetime_Data_Block_6,	READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock6},
    {SBS_Lifetime_Data_Block_7,	READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock7},
    {SBS_ManufacturerInfo,      READ_BLOCK,	32,	SMBUS_manufactureInfo},
    {SBS_DAStatus1,           	READ_BLOCK,	32,	SMBUS_daStatus1},
    {SBS_DAStatus2,           	READ_BLOCK,	16,	SMBUS_daStatus2},
    {SBS_CUV,                   READ_BLOCK,	32,	SMBUS_cuvSnapShot},
    {SBS_COV,                   READ_BLOCK,	32,	SMBUS_covSnapShot}
};

uint16_t smbusBQ78350_ManufacturerAccessCmdlist[] = {
    DeviceType,
    FirmwareVersion,
    HardwareVersion,
    IFChecksum,
    StaticDFSignature,
    ChemID,
    StaticChemDFSignature,
    AllDFSignature,
    ShutdownMode,
    SleepMode,
    CellBalanceToggle,
    AFEDelayDisable,
    SAFEToggle,
    PrechargeFET,
    ChargeFET,
    DischargeFET,
    Gauging,
    LifetimeDataCollection,
    PermanentFailure,
    BlackBoxRecorder,
    SAFE,
    LEDDisplayEnable,
    LifetimeDataReset,
    PermanentFailureDataReset,
    BlackBoxRecorderReset,
    LED,
    LEDDisplayPress,
    CalibrationMode,
    SealDevice,
    SecurityKeys,
    AuthenticationKey,
    DeviceReset,
    SafetyAlert,
    SafetyStatus,
    PFAlert,
    PFStatus,
    OperationStatus,
    ChargingStatus,
    GaugingStatus,
    ManufacturingStatus,
    AFEStatus,
    AFEConfig,
    AFEVCx,
    AFEData,
    LifetimeDataBlock1,
    LifetimeDataBlock2,
    LifetimeDataBlock3,
    LifetimeDataBlock4,
    LifetimeDataBlock5,
    LifetimeDataBlock6,
    LifetimeDataBlock7,
    ManufacturerInfo,
    DAStatus1,
    DAStatus2,
    CUVSnapshot,
    COVSnapshot,
    ROMMode,
    ExitCalibrationOutput,
    OutputCellVoltageforCalibration,
    OutputCellVoltageCCandTemp,
};
const ManufacturerAccesscommand BQ78350ManufacturerAccessCmd[] = {
    {DeviceType,                        READ_BLOCK,	2,	SMBUS_covSnapShot},
    {FirmwareVersion,                   READ_BLOCK,	12,	SMBUS_covSnapShot},
    {HardwareVersion,                   READ_BLOCK,	2,	SMBUS_covSnapShot},
    {IFChecksum,                        READ_BLOCK,	32,	SMBUS_covSnapShot},
    {StaticDFSignature,                 READ_BLOCK,	32,	SMBUS_covSnapShot},
    {ChemID,                            READ_BLOCK,	32,	SMBUS_covSnapShot},
    {StaticChemDFSignature,             READ_BLOCK,	32,	SMBUS_covSnapShot},
    {AllDFSignature,                    READ_BLOCK,	32,	SMBUS_covSnapShot},
    {ShutdownMode,                      READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SleepMode,                         READ_BLOCK,	0,	SMBUS_covSnapShot},
    {CellBalanceToggle,                 READ_BLOCK,	0,	SMBUS_covSnapShot},
    {AFEDelayDisable,                   READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SAFEToggle,                        READ_BLOCK,	0,	SMBUS_covSnapShot},
    {PrechargeFET,                      READ_BLOCK,	0,	SMBUS_covSnapShot},
    {ChargeFET,                         READ_BLOCK,	0,	SMBUS_covSnapShot},
    {DischargeFET,                      READ_BLOCK,	0,	SMBUS_covSnapShot},
    {Gauging,                           READ_BLOCK,	0,	SMBUS_covSnapShot},
    {LifetimeDataCollection,            READ_BLOCK,	0,	SMBUS_covSnapShot},
    {PermanentFailure,                  READ_BLOCK,	0,	SMBUS_covSnapShot},
    {BlackBoxRecorder,                  READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SAFE,                              READ_BLOCK,	0,	SMBUS_covSnapShot},
    {LEDDisplayEnable,                  READ_BLOCK,	0,	SMBUS_covSnapShot},
    {LifetimeDataReset,                 READ_BLOCK,	0,	SMBUS_covSnapShot},
    {PermanentFailureDataReset,         READ_BLOCK,	0,	SMBUS_covSnapShot},
    {BlackBoxRecorderReset,             READ_BLOCK,	0,	SMBUS_covSnapShot},
    {LED,                               READ_BLOCK,	0,	SMBUS_covSnapShot},
    {LEDDisplayPress,                   READ_BLOCK,	0,	SMBUS_covSnapShot},
    {CalibrationMode,                   READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SealDevice,                        READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SecurityKeys,                      READ_BLOCK,	2,	SMBUS_covSnapShot},
    {AuthenticationKey,                 READ_BLOCK,	2,	SMBUS_covSnapShot},
    {DeviceReset,                       READ_BLOCK,	0,	SMBUS_covSnapShot},
    {SafetyAlert,                       READ_BLOCK,	4,	SMBUS_safetyAlert},
    {SafetyStatus,                      READ_BLOCK,	4,	SMBUS_safetyStatus},
    {PFAlert,                           READ_BLOCK,	4,	SMBUS_pfAlert},
    {PFStatus,                          READ_BLOCK,	4,	SMBUS_pfStatus},
    {OperationStatus,                   READ_BLOCK,	4,	SMBUS_operationStatus},
    {ChargingStatus,                    READ_BLOCK,	2,	SMBUS_chargingStatus},
    {GaugingStatus,                     READ_BLOCK,	2,	SMBUS_gaugingStatus},
    {ManufacturingStatus,               READ_BLOCK,	2,	SMBUS_manufacturingStatus},
    {AFEStatus,                         READ_BYTE,	2,	SMBUS_afeStatus},
    {AFEConfig,                         READ_BLOCK,	11,	SMBUS_afeConfig},
    {AFEVCx,                            READ_BLOCK,	32,	SMBUS_afeVcx},
    {AFEData,                           READ_BLOCK,	14,	SMBUS_afeData},
    {LifetimeDataBlock1,                READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock1},
    {LifetimeDataBlock2,                READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock2},
    {LifetimeDataBlock3,                READ_BLOCK,	14,	SMBUS_lifeTimeDataBlock3},
    {LifetimeDataBlock4,                READ_BLOCK,	20,	SMBUS_lifeTimeDataBlock4},
    {LifetimeDataBlock5,                READ_BLOCK,	18,	SMBUS_lifeTimeDataBlock5},
    {LifetimeDataBlock6,                READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock6},
    {LifetimeDataBlock7,                READ_BLOCK,	32,	SMBUS_lifeTimeDataBlock7},
    {ManufacturerInfo,                  READ_BLOCK,	32,	SMBUS_manufactureInfo},
    {DAStatus1,                         READ_BLOCK,	32,	SMBUS_daStatus1},
    {DAStatus2,                         READ_BLOCK,	16,	SMBUS_daStatus2},
    {CUVSnapshot,                       READ_BLOCK,	32,	SMBUS_cuvSnapShot},
    {COVSnapshot,                       READ_BLOCK,	32,	SMBUS_covSnapShot},
    {ROMMode,                           READ_BLOCK,	32,	SMBUS_covSnapShot},
    {ExitCalibrationOutput,             READ_BLOCK,	32,	SMBUS_covSnapShot},
    {OutputCellVoltageforCalibration,   READ_BLOCK,	32,	SMBUS_covSnapShot},
    {OutputCellVoltageCCandTemp,        READ_BLOCK,	32,	SMBUS_covSnapShot},
};

uint16_t BQ78350SBSCmd_MAXSIZE = sizeof(smbusBQ78350_SBSCmdlist)/sizeof(smbusBQ78350_SBSCmdlist[0]);
uint16_t BQ78350ManufacturerAccessCmd_MAXSIZE = sizeof(smbusBQ78350_ManufacturerAccessCmdlist)/sizeof(smbusBQ78350_ManufacturerAccessCmdlist[0]);

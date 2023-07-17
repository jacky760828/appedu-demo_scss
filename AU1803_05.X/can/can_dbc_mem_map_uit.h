/******************************************************************************
 *
 * FILE: can_dbc_mem_map_uit.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_DBC_MEM_MAP_UIT_H
#define	CAN_DBC_MEM_MAP_UIT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "can_dbc_mem_map.h"

extern char *pPartNum;
extern const CanDbcMemMapUsageInfo_t BatteryManageSystem_usageInfoTable[];

bool BatteryManageSystem_VER_SoftwareVersionBuild (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SoftwareVersionMinor (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SoftwareVersionMajor (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_ProtocolVersion (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_ManufactureSerial (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_ManufactureDay (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_ManufactureYear (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SerialNumber1 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SerialNumber2 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SerialNumber3 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_PartNumber1 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_PartNumber2 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_PartNumber3 (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_DesignPower (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_DesignVoltage (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_DesignCapacity (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_ParallelConfiguration (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VER_SeriesConfiguration (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_NOUPdate(CanDbcMemMapUsageFuncParam_t *pUsageParam); //tsai 8/27
bool BatteryManageSystem_VER_BMFTP(CanDbcMemMapUsageFuncParam_t *pUsageParam); //tsai 9/1
bool BatteryManageSystem_VER_SW_Recongized(CanDbcMemMapUsageFuncParam_t *pUsageParam);// tsai 11/16 

bool BatteryManageSystem_ReservedUsage (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_SYSREQ_ExitRestMode (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_SYSREQ_AcknowledgeAlarm (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_Current (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_Voltage (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_FullChargeCapacity (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_RemainingCapacity (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_VCellAvg (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_VCellMin (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_VCellMax (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_BatteryStatus (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_TimeNow (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_TemperatureMin (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_TemperatureMax (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_ChargeCurrentMax (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_ChargeVoltageRequest (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_VoltageBus (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_MDAT_FWATFlashValid(CanDbcMemMapUsageFuncParam_t *pUsageParam); // tsai 7/29
bool BatteryManageSystem_MDAT_RemainingTimeAlarm(CanDbcMemMapUsageFuncParam_t *pUsageParam); // tsai 7/29
bool BatteryManageSystem_MDAT_SAFETY (CanDbcMemMapUsageFuncParam_t *pUsageParam);//frank 6/30
bool BatteryManageSystem_CAT0_Temperature(CanDbcMemMapUsageFuncParam_t *pUsageParam); //frank
bool BatteryManageSystem_CAT1_BalanceActiveForCell (CanDbcMemMapUsageFuncParam_t *pUsageParam); //frank
bool BatteryManageSystem_CAT2_VCell_Mx (CanDbcMemMapUsageFuncParam_t *pUsageParam); //frank
bool BatteryManageSystem_CHRRST (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_BMCCHG (CanDbcMemMapUsageFuncParam_t *pUsageParam);
bool BatteryManageSystem_VDAT_M1_SAFETY (CanDbcMemMapUsageFuncParam_t *pUsageParam); //frank
bool BatteryManageSystem_SAFETY_FLAGS(CanDbcMemMapUsageFuncParam_t *pUsageParam); //frank 6/30
bool BatteryManageSystem_MDAT_RemainingRunTime(CanDbcMemMapUsageFuncParam_t *pUsageParam); //tsai 8/27
bool BatteryManageSystem_MDAT_SensorFail(CanDbcMemMapUsageFuncParam_t *pUsageParam);// tsai 9/17
bool BatteryManageSystem_LDAT_Frames(CanDbcMemMapUsageFuncParam_t *pUsageParam);// tsai 11/11 
#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_MEM_MAP_UIT_H */


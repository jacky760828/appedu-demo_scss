/******************************************************************************
 *
 * FILE: can_dbc_usages_vdat.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"

bool BatteryManageSystem_SAFETY_FLAGS(CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;
    
    for( i=0; i < 5; i++) {
        *(pCanFrame+2+i) = sysProtection.fivebytes[i]; //start from bit16 so add 2 
    }
	*(pCanFrame+2+i) = sysProtection.MOSsState;//frank 9/18
    
    return true;
}

bool BatteryManageSystem_VDAT_M1_SAFETY(CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pTargetCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    for( i=0; i < 5; i++) {
        *(pTargetCanFrame+1+i) = sysProtection.fivebytes[i]; //start from bit8 so add 1 
    }
    // tsai 7/9 debug
    
    return true;
}
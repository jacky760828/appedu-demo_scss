/******************************************************************************
 *
 * FILE: can_dbc_usages_sysreq.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"


bool BatteryManageSystem_ReservedUsage
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    pUsageParam = pUsageParam;
    return true;
}

bool BatteryManageSystem_SYSREQ_ExitRestMode 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    if(pUsageParam->dataType != DBC_FLAG) {
        //data type is incorrect
        return false;
    }
    
    pUsageParam = pUsageParam;
    
    return true;
}

bool BatteryManageSystem_SYSREQ_AcknowledgeAlarm 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    if(pUsageParam->dataType != DBC_FLAG) {
        //data type is incorrect
        return false;
    }
    
    pUsageParam = pUsageParam;
    
    return true;
}

bool BatteryManageSystem_CHRRST
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t hibyte,lowbyte;
    
    lowbyte = GetFETsState();// bit 0 is DFET, bit 1 is CFET, bit2 is trickle FET
    hibyte = lowbyte;
    lowbyte <<= 1; //bit 1 is DFET
    lowbyte |= (hibyte >> 1) & 0x01; //bit 0 CFET
    lowbyte = 0x03; //left bit0,1
    lowbyte |= (hibyte & 0x04); //bit 2 is trickle
    pCanFrame[1] &= 0xF8;
    pCanFrame[1] |= lowbyte;
    
    return true;
}

bool BatteryManageSystem_BMCCHG
            (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    *(pCanFrame + 1) = d_deviceAddr;    
    *(pCanFrame + 3) = 0;
    
    return true;
}

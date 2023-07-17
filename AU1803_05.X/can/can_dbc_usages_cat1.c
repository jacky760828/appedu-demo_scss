/******************************************************************************
 *
 * FILE: can_dbc_usages_cat1.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"
#include "../sys_time/sys_time.h"

#define CAT1_UPDATE_PERIOD   25  //sec
uint32_t d_cat1Time = 5;//sec

uint16_t setCOV16Flags(void)
{
    uint16_t tempResult;
    uint16_t COV16Flags;
    int8_t i;
    
    tempResult = 0;
    for(i = 27; i >= 0; i--) {
        tempResult = (tempResult << 8);
        tempResult |= SMBUS_covSnapShot[i];
        if((i % 2) == 0) {
            if(tempResult >= COV_OVER_VMAX) {
                COV16Flags |= (0x0001 << i/2);
            }
            tempResult = 0;
        }
    }
    return COV16Flags;
}

uint16_t setCUV16Flags(void)
{
    uint16_t tempResult;
    int8_t i;
    uint16_t CUV16Flags;
    
    tempResult = 0;
    for(i = 27; i >= 0; i--) {
        tempResult = (tempResult << 8);
        tempResult |= SMBUS_cuvSnapShot[i];
        if((i % 2) == 0) {
            if((tempResult <= CUV_SET_TH)&&(tempResult >= 1)) {
                CUV16Flags |= (0x0001 << i/2);
            }
            tempResult = 0;
        }
    }
    return CUV16Flags;
}

bool BatteryManageSystem_CAT1_BalanceActiveForCell
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame; //M1
    uint16_t tempData=0;

    if ( !SysTime_secIntervalElapsed(d_cat1Time, CAT1_UPDATE_PERIOD)) {
        return false;
    }
    
    d_cat1Time = SysTime_getSec();
    tempData = SMBUS_getAFEConfig();
    
    pCanFrame[1] =(uint8_t)(tempData & 0xFF);
    pCanFrame[2] =(uint8_t)((tempData >> 8) & 0xFF);
    //
    pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx+1].ptrFrame; //M2
    tempData = setCOV16Flags();
    pCanFrame[1] =(uint8_t)(tempData & 0xFF);
    pCanFrame[2] =(uint8_t)((tempData >> 8) & 0xFF);
    
    pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx+2].ptrFrame; //M3
    tempData = setCUV16Flags();
    pCanFrame[1] =(uint8_t)(tempData & 0xFF);
    pCanFrame[2] =(uint8_t)((tempData >> 8) & 0xFF);
    return true;
}

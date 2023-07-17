/******************************************************************************
 *
 * FILE: can_dbc_usages_cat2.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"

//frank add
bool BatteryManageSystem_CAT2_VCell_Mx (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t cellVoltage = 0x0000;
    int16_t upperLimit, lowerLimit;
    uint8_t *pSmbusRam;
    uint8_t *pCanFrame;// = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;    
    int8_t i,j,index;
    uint32_t tempData;
    
    if (pUsageParam->frameIdx >= DCB_UIT[DCB_CMD_NAME_CAT2].startFrameNum+4) {
        return false;
    }
    lowerLimit = (pUsageParam->offsetValue + pUsageParam->minValue);
    upperLimit = (pUsageParam->offsetValue + pUsageParam->maxValue);
    pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;    
    index = SMBUS_FIND_CMDINDEX(SBS_CellVoltage1) - ((pUsageParam->frameIdx - DCB_UIT[DCB_CMD_NAME_CAT2].startFrameNum) * 4);
    for (j = 0; j < 2; j++) {
        tempData = 0x00000000;
        for (i = 1; i >= 0; i--) {
            cellVoltage = index - i - (j * 2);
            if ( cellVoltage <= SMBUS_FIND_CMDINDEX(SBS_CellVoltage15)) {
                continue;
            }
            pSmbusRam = smbusMatrixCmd[cellVoltage].ptrCommandData;
            cellVoltage = ((uint16_t)*(pSmbusRam + 1) << 8) | *(pSmbusRam);
            
            if (cellVoltage < lowerLimit) {
                cellVoltage = pUsageParam->minValue;
            } else if(cellVoltage > upperLimit) {
                cellVoltage = pUsageParam->maxValue;
            } else {
                cellVoltage = (uint16_t)(cellVoltage -lowerLimit);
            }
            
            tempData <<= 12; 
            tempData |= cellVoltage;
        }
        pCanFrame[2+j*3]=(uint8_t)(tempData & 0xFF);
        pCanFrame[3+j*3]=(uint8_t)((tempData >> 8) & 0xFF);
        pCanFrame[4+j*3]=(uint8_t)((tempData >> 16) & 0xFF);
    } 
    // debug tsai 7/13
    return true;
}


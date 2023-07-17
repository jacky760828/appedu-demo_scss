/******************************************************************************
 *
 * FILE: can_dbc_usages_cat0.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "can.h"


bool BatteryManageSystem_CAT0_Temperature 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    int16_t temperature = 0x0000;
    int16_t upperLimit, lowerLimit;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;

    lowerLimit = pUsageParam->offsetValue + pUsageParam->minValue;
    upperLimit = pUsageParam->offsetValue + pUsageParam->maxValue;
    for (i = 0; i < 6; i++) { //TS1,TS2,TS3,Cell,FET,Gauge
        temperature = ((uint16_t)(SMBUS_daStatus2[5 + (i * 2)] << 8) | SMBUS_daStatus2[4 + (i * 2)]);//temperature, in degree K       
        //transfer the reading to degree C
        
        temperature = ((temperature - 2731) / 10);
        if(temperature < lowerLimit) { // -40
            temperature = pUsageParam->minValue;
        } else if(temperature > upperLimit) { //210
            temperature = pUsageParam->maxValue;
        } else {
            temperature -= lowerLimit;      
        }
        pCanFrame[1 + i]=(uint8_t)temperature;
    }
    
    return true;
}
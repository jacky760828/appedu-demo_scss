/******************************************************************************
 *
 * FILE: can_dbc_usages_mdat.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"
#include "../i2c/i2c_a1006.h" // tsai 9/17

uint8_t AlarmReleaseCount;

void putCellVoltIntoCanDcbBuffer(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, int16_t cellVoltage)
{
    uint32_t tempResult = 0;
    int16_t upperLimit, lowerLimit;

    lowerLimit = (pUsageParam->offsetValue + pUsageParam->minValue);
    upperLimit = (pUsageParam->offsetValue + pUsageParam->maxValue);
    
    if(cellVoltage < lowerLimit) {
        tempResult = 0;
    } else if(cellVoltage > upperLimit) {
        tempResult = 4000;
    } else {
        tempResult = (uint32_t)(cellVoltage - lowerLimit);
    }
    
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
}


void putPackVoltIntoCanDcbBuffer(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, int16_t packVoltage)
{
    uint32_t tempResult = 0;
    int16_t upperLimit, lowerLimit;

    lowerLimit = pUsageParam->minValue;
    upperLimit = pUsageParam->maxValue;
    
    if(packVoltage < lowerLimit) {
        tempResult = (uint32_t)lowerLimit;
    } else if(packVoltage > upperLimit) {
        tempResult = (uint32_t)lowerLimit;
    } else {
        tempResult = (uint32_t)(packVoltage - lowerLimit);
    }
    
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
}


void putPackCurrentIntoCanDcbBuffer(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, int16_t packCurrent)
{
    uint32_t tempResult = 32500;
    int16_t upperLimit, lowerLimit;

    lowerLimit = pUsageParam->minValue;
    upperLimit = pUsageParam->maxValue;
    
    tempResult += packCurrent;
    if(packCurrent < lowerLimit) {
        tempResult = (uint32_t)lowerLimit;
    } else if(packCurrent > upperLimit) {
        tempResult = (uint32_t)lowerLimit;
    }
    
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
}

bool BatteryManageSystem_MDAT_Current 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    int16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_INT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = SMBUS_getCurrent();
    putPackCurrentIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);

    return true;
}

// tsai 7/29
bool BatteryManageSystem_MDAT_FWATFlashValid
            (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    bool tempData;
    
    if(pUsageParam->dataType != DBC_FLAG) {
        //data type is incorrect
        return false;
    }
    
    tempData = DBCFLAGS.ImageOK;
    CanDbcUtils_putFlagIntoDcbFrame(pUsageParam, pCanFrame, tempData);
    
    return true;
    
}
// tsai 7/29
bool BatteryManageSystem_MDAT_RemainingTimeAlarm
            (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    bool tempData;
    if(pUsageParam->dataType != DBC_FLAG) {
        //data type is incorrect
        return false;
    }
    
    tempData = DBCFLAGS.RemainingAlarm;
    CanDbcUtils_putFlagIntoDcbFrame(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_Voltage 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = (uint16_t)(vBat *10);
    putPackVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_FullChargeCapacity 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint32_t tempResult = 0x00000000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempResult = dsgFCC;
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
    
    return true;
}

bool BatteryManageSystem_MDAT_RemainingCapacity 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint32_t tempResult = 0x00000000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
//    Out_RC = Calculation_RC;
    tempResult = uiCalRCapacity;
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
    
    return true;
}

bool BatteryManageSystem_MDAT_VCellAvg 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = (uint16_t)(vBat * 1000 / CELL_IN_SERIES);
    putCellVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);

    return true;
}

bool BatteryManageSystem_MDAT_VCellMin 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = SMBUS_getMinCellVolt(); //frank 4/1
    putCellVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);

    return true;
}

bool BatteryManageSystem_MDAT_VCellMax 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = SMBUS_getMaxCellVolt(); //frank 4/1
    putCellVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);

    return true;
}

bool BatteryManageSystem_MDAT_BatteryStatus 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint16_t tempData;
    
    tempData = GetFETsState(); //frank 6/16
    if (BMS_Get_Operating_Flag(WAKEUP)) { // tsai 6/1
        tempData &= 0XF7;  //2022/8/24 switch
    } else {
        tempData |= 0x08;
    }
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_TimeNow 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint32_t timeSec;
    int8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT32) {
        //data type is incorrect
        return false;
    }
    
    timeSec = SysTime_getSec();

    for(i = pUsageParam->startByte; i < pUsageParam->endByte; i++) {
        *(pCanFrame + i) = (uint8_t)(timeSec & 0x000000FF);
        timeSec = (timeSec >> 8);
    }
    return true;
}

bool BatteryManageSystem_MDAT_TemperatureMin 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    int16_t tempData = 0x0000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT8) {
        //data type is incorrect
        return false;
    }
    
    tempData = SMBUS_getMinTemperature();
    CanDbcUtils_putTemperatureIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_TemperatureMax 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    int16_t tempData = 0x0000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT8) {
        return false;
    }
    
    tempData = SMBUS_getMaxTemperature();
    CanDbcUtils_putTemperatureIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_ChargeCurrentMax 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    int16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_INT16) {
        return false;
    }
    
    tempData = SMBUS_getChargingCurrent();
    putPackCurrentIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);

    return true;
}

bool BatteryManageSystem_MDAT_ChargeVoltageRequest 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempData = (SMBUS_getChargingVoltage() / 10);
    
    putPackVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_MDAT_VoltageBus 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }

    tempData = (uint16_t)(vBus * 10); // frank 4/16 unit 100mV
    putPackVoltIntoCanDcbBuffer(pUsageParam, pCanFrame, tempData);
    
    return true;
}

// frank 6/30
bool BatteryManageSystem_MDAT_SAFETY 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    return BatteryManageSystem_SAFETY_FLAGS( pUsageParam );
}


// tsai 8/31

bool BatteryManageSystem_MDAT_RemainingRunTime
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;

    if (RemainTimeCnt <= 90) { //90s
        DBCFLAGS.RemainingAlarm = 1;
        AlarmReleaseCount = 0;
    } else {
        if (DBCFLAGS.RemainingAlarm) {
           if (AlarmReleaseCount >= 5) {
                AlarmReleaseCount = 0;
                DBCFLAGS.RemainingAlarm = 0;
           } else {
               AlarmReleaseCount++;
           }
        } else {
            DBCFLAGS.RemainingAlarm=0;
        }
    }
    i = pUsageParam->startByte;
    *(pCanFrame + i) = (uint8_t)RemainTimeCnt; //low
    i++;
    *(pCanFrame + i) = (uint8_t)(RemainTimeCnt >> 8); //high    
    
    return true;
}

//tsai 9/17
bool BatteryManageSystem_MDAT_SensorFail(CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;
    
    i = pUsageParam->startByte; //start bit 48
    if (!A1006_Auth.Exist) {
        *(pCanFrame + i) |= 0x10;  //bit4, A1006 failes to read or write, etc.
    } else {
        *(pCanFrame + i) &= ~0x10;
    }
    // tsai 5/30 , APC request
    if(EventSOH < 70) {    //MDAT_ReplaceBatteryNearFuture
        *(pCanFrame + i) |= 0x08;
        if (EventSOH < 60) {  //MDAT_ReplaceBatteryNow
            *(pCanFrame + i) |= 0x04;
        } else if (EventSOH > 65) {
            *(pCanFrame + i) &= ~0x04;
        }
    } else if(EventSOH > 75) {
        *(pCanFrame + i) &= ~0x0C;
    }
    
    return true;
}

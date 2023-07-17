/******************************************************************************
 *
 * FILE: can_dbc_usages_ver.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "can.h"

const uint16_t monthDay[13] = { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
char *pPartNum;

void Put16Bits(CanDbcMemMapUsageFuncParam_t *pUsageParam, UINT16 value)
{
    uint16_t tempData = 0x0000;  
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    tempData += value;
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
}

bool BatteryManageSystem_VER_SoftwareVersionBuild 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam, fwVersionBuild[0]);
    
    return true;
}

bool BatteryManageSystem_VER_SoftwareVersionMinor 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam,fwVersionMinor[0]);
    return true;
}

bool BatteryManageSystem_VER_SoftwareVersionMajor 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam,fwVersionMajor[0]);
    return true;
}

bool BatteryManageSystem_VER_ProtocolVersion 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam,protocolVersion[0]);
    return true;
}

bool BatteryManageSystem_VER_ManufactureSerial 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint32_t tempResult = 0x00000000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    
    tempResult = (uint32_t)SMBUS_getSerialNumber();
    
    CanDbcUtils_putDataIntoDcbFrame_32Bits(pUsageParam, pCanFrame, tempResult);
    
    return true;
}

bool BatteryManageSystem_VER_ManufactureDay 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData = 0;
    uint8_t tempMonth = 0x00;
    uint8_t tempYear = 0x00;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT16) {
        //data type is incorrect
        return false;
    }
    tempData = SMBUS_getManufactureDate();
    tempMonth = (uint8_t)((tempData >> 5) & 0x000F);
    tempYear = (uint8_t)((tempData >> 9) & 0x0003);
    tempData = (uint8_t)(tempData & 0x001F) + monthDay[tempMonth];
    
    if((tempYear == 0) && (tempMonth > 2)) {
        tempData ++;
    }
    
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_VER_ManufactureYear 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData = 0x0000;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;

    if(pUsageParam->dataType != DBC_UINT8) {
        //data type is incorrect
        return false;
    }
    
    tempData = SMBUS_getManufactureDate();
    tempData = tempData >> 9;
    if(tempData <= 20) {
        tempData = 0;
    } else {
        tempData -= 20;
    }
    
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
            
    return true;
}

bool BatteryManageSystem_VER_SerialNumber1 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    if (SerialNum[17]==0x00) {
        MX25L_Flash_Read(SerialNum, 0x030000, 18);
    } else {
        for(i = 0; i < 18; i++) {
            SerialNum[i] = ' ';
        }
    }
    
    for(i = 2; i < 8; i++) {
        *(pCanFrame + i) = SerialNum[i - 2];
    }
    
    return true;
}

bool BatteryManageSystem_VER_SerialNumber2 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    for(i = 0; i < 6; i++) {
        *(pCanFrame + (i + 2)) = SerialNum[i + 6];
    }
    
    return true;
}

bool BatteryManageSystem_VER_SerialNumber3 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    for(i = 0; i < 4; i++) {
        *(pCanFrame + (i + 2)) = SerialNum[i + 12];
    }
    
    return true;
}

bool BatteryManageSystem_VER_PartNumber1 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    for(i = 0; i < 6; i++) {
        *(pCanFrame + (i + 2)) = pPartNum[i];
    }

    return true;
}

bool BatteryManageSystem_VER_PartNumber2 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    for(i = 0; i < 6; i++) {
        *(pCanFrame + (i + 2)) = pPartNum[i + 6];
    }
    return true;
}

bool BatteryManageSystem_VER_PartNumber3 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_CHAR) {
        //data type is incorrect
        return false;
    }
    
    for(i = 0; i < 4; i++) {
        if(d_deviceAddr == CAN_DEFAULT_RBC_ADDRESS) {
             *(pCanFrame + (i + 2)) = ' ';
        } else {
            if (i < 3) {
                *(pCanFrame + (i + 2)) = pPartNum[i + 12];
            } else {
                *(pCanFrame + (i + 2)) = ' ';
            }
        }
    }
    
    return true;
}

bool BatteryManageSystem_VER_DesignPower 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam, PACK_DESIGN_POWER_DEFAULT);
    return true;
}

bool BatteryManageSystem_VER_DesignVoltage 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam, PACK_VOLTAGE);
    return true;
}

bool BatteryManageSystem_VER_DesignCapacity 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(d_deviceAddr == CAN_DEFAULT_RBC_ADDRESS) {
        tempData = RBC_FULL_CHARGE_CAPACITY_WH_DEF;
    } else {
        tempData = XBP_FULL_CHARGE_CAPACITY_WH_DEF;
    }
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_VER_ParallelConfiguration 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint16_t tempData;
    uint8_t i;
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    
    if(pUsageParam->dataType != DBC_UINT8) {
        //data type is incorrect
        return false;
    }
    
    tempData = PACK_PARALLEL_RBC_NUMBER_DEFAULT;
    for(i = 0; i < 6; i++) {
        if(SMBUS_deviceName[i] != skuName[i]) {
            tempData = PACK_PARALLEL_XBP_NUMBER_DEFAULT;
            break;
        }
    }
    
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempData);
    
    return true;
}

bool BatteryManageSystem_VER_SeriesConfiguration 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    Put16Bits(pUsageParam, CELL_IN_SERIES);
    return true;
}

// tsai 8/27 dummy
bool BatteryManageSystem_NOUPdate 
     (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    return true;
}

// tsai 8/27 report FW update status
bool BatteryManageSystem_VER_BMFTP
  (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;
    
    i = pUsageParam->startByte;
    if (DBCFLAGS.ImageOK) {    // tsai 5/31
        *(pCanFrame + i) =  FlashVerion[0]; //major
        i++;
        *(pCanFrame + i) =  FlashVerion[1]; //minor
        i++;
        *(pCanFrame + i) =  FlashVerion[2]; //build
    } else {
        *(pCanFrame + i) = fwVersionMajor[0]; //major
        i++;
        *(pCanFrame + i) = fwVersionMinor[0]; //minor
        i++;
        *(pCanFrame + i) = fwVersionBuild[0]; //build
    }
    i++;
    *(pCanFrame + i) = 0x00; //low 4 bits flag  Flags , idle, in progress, in verify, ready install , 3 is CTECH
    i++;
    *(pCanFrame + i) = 0x03;
    
    return true;
}

bool BatteryManageSystem_VER_SW_Recongized
  (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    uint8_t *pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx].ptrFrame;
    uint8_t i;
    
    i = pUsageParam->startByte;
    *(pCanFrame + i) = FlashVerion[0]; //major
    i++;
    *(pCanFrame + i) = FlashVerion[1]; //minor
    i++;
    *(pCanFrame + i) = FlashVerion[2]; //build
    
    return true;
}
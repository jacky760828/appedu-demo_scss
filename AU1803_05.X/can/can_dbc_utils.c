/******************************************************************************
 *
 * FILE: can_dbc_utils.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "can_dbc_utils.h"

void CanDbcUtils_putDataIntoDcbFrame_16Bits(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, uint16_t tempResult)
{
    uint16_t frameData = 0;
    uint16_t mask = 0;
    int8_t i;
    
    mask = (1 << pUsageParam->size) - 1;
    for(i = pUsageParam->endByte; i >= pUsageParam->startByte; i--) {
        frameData = (frameData << 8);
        frameData += *(pCanFrame + i);
        if(i == 0) {
            break;
        }
    }
    
    for( i = 0; i < pUsageParam->startBit; i++) {
        mask = (mask << 1);
        tempResult = (tempResult << 1);
    }
    frameData &= ~mask;
    frameData |= tempResult;
    
    for(i = pUsageParam->startByte; i <= pUsageParam->endByte; i++) {
        *(pCanFrame + i) = (uint8_t)(frameData & 0x00FF);
        frameData = (frameData >> 8);
    }
}


void CanDbcUtils_putDataIntoDcbFrame_32Bits(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, uint32_t tempResult)
{
    uint32_t frameData = 0;
    uint32_t mask = 0;
    int8_t i;
    
    mask = (1 << pUsageParam->size) - 1;
    for(i = pUsageParam->endByte; i >= pUsageParam->startByte; i--) {
        frameData = (frameData << 8);
        frameData += *(pCanFrame + i);
        if(i == 0) {
            break;
        }
    }
    
    for( i = 0; i < pUsageParam->startBit; i++) {
        mask = (mask << 1);
        tempResult = (tempResult << 1);
    }
    frameData &= ~mask;
    frameData |= tempResult;
    
    for(i = pUsageParam->startByte; i <= pUsageParam->endByte; i++) {
        *(pCanFrame + i) = (uint8_t)(frameData & 0x00FF);
        frameData = (frameData >> 8);
    }
}

void CanDbcUtils_putFlagIntoDcbFrame(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, bool tempResult)
{
    uint8_t frameData = 0;
    uint8_t mask = 0;
    uint8_t i;
    
    mask = (1 << pUsageParam->size) - 1;
    frameData = *(pCanFrame + pUsageParam->startByte);

    for( i = 0; i < (pUsageParam->startBit); i++) {
        mask = (mask << 1);
        tempResult = (tempResult << 1);
    }
    frameData &= ~mask; //clear
    frameData |= tempResult; //set
    
    *(pCanFrame + pUsageParam->startByte) = frameData;
}

void CanDbcUtils_putTemperatureIntoCanDcbBuffer(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, int16_t temperature)
{
    uint16_t tempResult = 0;
    int16_t upperLimit, lowerLimit;
    uint32_t tempDiv;
    
    //transfer the reading to degree C
    lowerLimit = 2731 + ((pUsageParam->offsetValue + pUsageParam->minValue) * 10);
    upperLimit = 2731 + ((pUsageParam->offsetValue + pUsageParam->maxValue) * 10);
    
    if(temperature == 0x5A5A) {
        tempResult = 253;
    } else if(temperature < lowerLimit) {
        tempResult = 0;
    } else if(temperature > upperLimit) {
        tempResult = 250;
    } else {
        //tempResult = (uint8_t)((tempData - lowerLimit) / 10);
        tempDiv = (uint32_t)(temperature - lowerLimit) * 51;
        tempResult = (uint8_t)(tempDiv >> 9); //512
    }
    
    CanDbcUtils_putDataIntoDcbFrame_16Bits(pUsageParam, pCanFrame, tempResult);
}

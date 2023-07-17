/******************************************************************************
 *
 * FILE: can_utils.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_DBC_UTILS_H
#define	CAN_DBC_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "can_dbc_mem_map_uit.h"

void CanDbcUtils_putDataIntoDcbFrame_16Bits(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, uint16_t tempResult);
void CanDbcUtils_putDataIntoDcbFrame_32Bits(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, uint32_t tempResult);
void CanDbcUtils_putFlagIntoDcbFrame(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, bool tempResult);
void CanDbcUtils_putTemperatureIntoCanDcbBuffer(CanDbcMemMapUsageFuncParam_t *pUsageParam, uint8_t *pCanFrame, int16_t temperature);

#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_UTILS_H */


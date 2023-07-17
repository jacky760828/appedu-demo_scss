/******************************************************************************
 *
 * FILE: can_dbc_mem_map.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_DBC_MEM_MAP_H
#define	CAN_DBC_MEM_MAP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
//#include "../main_app/platform_types.h"

typedef enum {
    DBC_CHAR = 0,
    DBC_UINT8,
    DBC_INT8,
    DBC_UINT16,
    DBC_INT16,
    DBC_UINT32,
    DBC_INT32,
    DBC_FLAG
} CAN_DBC_DataType_t;

    
typedef struct {
    CAN_DBC_DataType_t dataType;
    uint8_t  frameIdx;
    uint8_t  startBit;
    uint8_t  size;
    int32_t  offsetValue;
    int32_t  minValue;
    int32_t  maxValue;
} CanDbcMemMapUsageLookup_t;


typedef struct {
    uint8_t isWriting   :1;
    uint8_t factoryMode :1;
    uint8_t spare       :6;
} MemMapUsageFuncFlagParams_t;


typedef struct __attribute__ ((__packed__)) {
    MemMapUsageFuncFlagParams_t flags;
    CAN_DBC_DataType_t dataType;
    uint8_t  frameIdx;
    uint8_t  startByte;
    uint8_t  endByte;
    uint8_t  startBit;
    uint8_t  size;
    int32_t  offsetValue;
    int32_t  minValue;
    int32_t  maxValue;
} CanDbcMemMapUsageFuncParam_t;


typedef  bool (CanDbcMemMapUsageFunc_t)(CanDbcMemMapUsageFuncParam_t *pUsageParam);

typedef const struct __attribute__ ((__packed__)) {
    uint16_t elementIdx;
    CAN_DBC_DataType_t dataType;
    uint8_t  frameIdx;
    uint8_t  startByte;
    uint8_t  startBit;
    uint8_t  size;
    uint8_t  endByte;
    int32_t  offsetValue;
    int32_t  minValue;
    int32_t  maxValue;
    CanDbcMemMapUsageFunc_t  *usageFunc;
} CanDbcMemMapUsageInfo_t;

#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_MEM_MAP_H */


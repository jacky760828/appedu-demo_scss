/******************************************************************************
 *
 * FILE: can_dbc_mem_map_uit.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "can_dbc_mem_map_uit.h"

#define CAN_DBC_USAGE_ITEM(element_index, data_type, frame_index, start_bit_pos, bit_size, offset_val, min_val, max_val, usage_function)    \
    { element_index,                                                                                                                        \
      data_type,                                                                                                                            \
      frame_index,                                                                                                                          \
      (uint8_t)(start_bit_pos / 8), /*(7 - (uint8_t)(start_bit_pos / 8)),*/                                                                                                     \
      (uint8_t)(start_bit_pos % 8),                                                                                                           \
      bit_size,                                                                                                                             \
      (uint8_t)(((bit_size - 1) / 8) + (start_bit_pos / 8)), /*((7 - (uint8_t)(start_bit_pos / 8)) - (uint8_t)(((bit_size - 1) + (start_bit_pos % 8)) / 8)),*/                                             \
      offset_val,                                                                                                                           \
      min_val,                                                                                                                              \
      max_val,                                                                                                                              \
      usage_function }

/*
NOTE:
Usage Info Table (UIT):
    The table is composed of items that contain the following elements
        [ 1] = Element Index
        [ 2] = Data Type
        [ 3] = Frame Index
        [ 4] = Start Byte Position
        [ 5] = Start Bit Position
        [ 6] = Data Bit Size
        [ 7] = End Byte Position
        [ 8] = Offset Value
        [ 9] = Min. Value
        [10] = Max. Value
        [11] - Function pointer to the usage

UIT row starts:
    Usage that starts the described data row. This a lookup table used to
    accelerate the finding of UI in UIT when given a row number. Given a row
    number it points to the UI(in UIT) that corresponds to the first usage
    on that row. This means that given a row number I can immediately find
    where in the UIT I should jump to find the usages for that row.
*/
const CanDbcMemMapUsageInfo_t BatteryManageSystem_usageInfoTable[] = {
/*
                    Element      Data Type   Frame Index     Start Bit        Bit Size         Offset      Min.        Max.            Function Pointer to the usage
                      Index                                   Position                         Value       Value       Value
 */
    CAN_DBC_USAGE_ITEM (0,         DBC_FLAG,      0,              30,             1,              0,          0,          1,           BatteryManageSystem_SYSREQ_ExitRestMode),
    CAN_DBC_USAGE_ITEM (1,         DBC_FLAG,      0,              31,             1,              0,          0,          1,           BatteryManageSystem_SYSREQ_AcknowledgeAlarm),
    CAN_DBC_USAGE_ITEM (2,         DBC_UINT8,     1,              32,             8,              0,          0,          0,           BatteryManageSystem_VER_SoftwareVersionBuild),
    CAN_DBC_USAGE_ITEM (3,         DBC_UINT8,     1,              24,             8,              0,          0,          0,           BatteryManageSystem_VER_SoftwareVersionMinor),
    CAN_DBC_USAGE_ITEM (4,         DBC_UINT8,     1,              16,             8,              0,          0,          0,           BatteryManageSystem_VER_SoftwareVersionMajor),
    CAN_DBC_USAGE_ITEM (5,         DBC_UINT8,     1,              8,              8,              0,          0,          255,         BatteryManageSystem_VER_ProtocolVersion),
    CAN_DBC_USAGE_ITEM (6,         DBC_UINT16,    2,              24,             16,             0,          0,          65535,       BatteryManageSystem_VER_ManufactureSerial),
    CAN_DBC_USAGE_ITEM (7,         DBC_UINT16,    2,              15,             9,              0,          0,          366,         BatteryManageSystem_VER_ManufactureDay),
    CAN_DBC_USAGE_ITEM (8,         DBC_UINT8,     2,              8,              7,              2000,       0,          127,         BatteryManageSystem_VER_ManufactureYear),
    CAN_DBC_USAGE_ITEM (9,         DBC_CHAR,      3,              16,             48,             0,          0,          0,           BatteryManageSystem_VER_SerialNumber1),
    CAN_DBC_USAGE_ITEM (10,        DBC_CHAR,      4,              16,             48,             0,          0,          0,           BatteryManageSystem_VER_SerialNumber2),
    CAN_DBC_USAGE_ITEM (11,        DBC_CHAR,      5,              16,             32,             0,          0,          0,           BatteryManageSystem_VER_SerialNumber3),
    CAN_DBC_USAGE_ITEM (12,        DBC_CHAR,      6,              16,             48,             0,          0,          0,           BatteryManageSystem_VER_PartNumber1),
    CAN_DBC_USAGE_ITEM (13,        DBC_CHAR,      7,              16,             48,             0,          0,          0,           BatteryManageSystem_VER_PartNumber2),
    CAN_DBC_USAGE_ITEM (14,        DBC_CHAR,      8,              16,             32,             0,          0,          0,           BatteryManageSystem_VER_PartNumber3),
    CAN_DBC_USAGE_ITEM (15,        DBC_UINT16,    9,              48,             16,             0,          0,          65534,       BatteryManageSystem_VER_DesignPower),
    CAN_DBC_USAGE_ITEM (16,        DBC_UINT16,    9,              36,             9,              0,          0,          510,         BatteryManageSystem_VER_DesignVoltage),
    CAN_DBC_USAGE_ITEM (17,        DBC_UINT16,    9,              24,             12,             0,          0,          4094,        BatteryManageSystem_VER_DesignCapacity),
    CAN_DBC_USAGE_ITEM (18,        DBC_UINT8,     9,              16,             8,              0,          0,          100,         BatteryManageSystem_VER_ParallelConfiguration),
    CAN_DBC_USAGE_ITEM (19,        DBC_UINT8,     9,              8,              8,              0,          0,          255,         BatteryManageSystem_VER_SeriesConfiguration),
    CAN_DBC_USAGE_ITEM (20,        DBC_UINT8,     10,              8,              8,             0,          0,          0,           BatteryManageSystem_VER_BMFTP), // Major,Minor,Build, vendor code 3 
    CAN_DBC_USAGE_ITEM (21,        DBC_UINT8,     11,             24,              8,             0,          0,          0,           BatteryManageSystem_VER_SW_Recongized), // Major,Minor,Build, vendor code 3 
    CAN_DBC_USAGE_ITEM (21,        DBC_INT16,     12,             48,             16,             -32500,     -32500,     32500,       BatteryManageSystem_MDAT_Current),
    CAN_DBC_USAGE_ITEM (22,        DBC_UINT16,    12,             36,             12,             0,          0,          3000,        BatteryManageSystem_MDAT_Voltage),
    CAN_DBC_USAGE_ITEM (23,        DBC_UINT16,    12,             24,             12,             0,          0,          4094,        BatteryManageSystem_MDAT_FullChargeCapacity),
    CAN_DBC_USAGE_ITEM (24,        DBC_UINT16,    12,             12,             12,             0,          0,          4094,        BatteryManageSystem_MDAT_RemainingCapacity),
    CAN_DBC_USAGE_ITEM (25,        DBC_FLAG,    12,             11,              1,             0,          0,          1,           BatteryManageSystem_MDAT_FWATFlashValid), // tsai 7/29
    CAN_DBC_USAGE_ITEM (26,        DBC_FLAG,    12,             10,              1,             0,          0,          1,           BatteryManageSystem_MDAT_RemainingTimeAlarm), // tsai 7/29
    CAN_DBC_USAGE_ITEM (27,        DBC_UINT16,    13,             48,             12,             500,        0,          4000,        BatteryManageSystem_MDAT_VCellAvg),
    CAN_DBC_USAGE_ITEM (28,        DBC_UINT16,    13,             36,             12,             500,        0,          4000,        BatteryManageSystem_MDAT_VCellMin),
    CAN_DBC_USAGE_ITEM (29,        DBC_UINT16,    13,             24,             12,             500,        0,          4000,        BatteryManageSystem_MDAT_VCellMax),
    CAN_DBC_USAGE_ITEM (30,        DBC_UINT16,    13,             12,             12,             0,          0,          4095,        BatteryManageSystem_MDAT_BatteryStatus),
    CAN_DBC_USAGE_ITEM (31,        DBC_UINT32,    14,             32,             32,             0,          0,          -1,          BatteryManageSystem_MDAT_TimeNow),
    CAN_DBC_USAGE_ITEM (32,        DBC_UINT8,     14,             24,             8,              -40,        0,          250,         BatteryManageSystem_MDAT_TemperatureMin),
    CAN_DBC_USAGE_ITEM (33,        DBC_UINT8,     14,             16,             8,              -40,        0,          250,         BatteryManageSystem_MDAT_TemperatureMax),
    CAN_DBC_USAGE_ITEM (34,        DBC_INT16,     15,             48,             16,             -32500,     -32500,     32500,       BatteryManageSystem_MDAT_ChargeCurrentMax),
    CAN_DBC_USAGE_ITEM (35,        DBC_UINT16,    15,             24,             12,             0,          0,          3000,        BatteryManageSystem_MDAT_ChargeVoltageRequest),
    CAN_DBC_USAGE_ITEM (36,        DBC_UINT16,    15,             12,             12,             0,          0,          3000,        BatteryManageSystem_MDAT_VoltageBus),
    CAN_DBC_USAGE_ITEM (37,        DBC_FLAG,      16,             16,             40,             0,          0,          0,           BatteryManageSystem_MDAT_SAFETY), //frank 6/30
    CAN_DBC_USAGE_ITEM (38,        DBC_UINT16,    17,             48,             16,             0,          0,          65534,       BatteryManageSystem_MDAT_SensorFail), // tsai 8/30
    CAN_DBC_USAGE_ITEM (39,        DBC_UINT16,    17,             32,             16,             0,          0,          65534,       BatteryManageSystem_MDAT_RemainingRunTime), //tsai
    //CAN_DBC_USAGE_ITEM (50,        DBC_UINT8,     18,             24,             8,              -40,        0,          250,         BatteryManageSystem_MDAT_TemperatureMin),
    //
    CAN_DBC_USAGE_ITEM (40,        DBC_UINT8,     18,             8,              56,              -40,        0,          250,        BatteryManageSystem_CAT0_Temperature),    //frank 
    CAN_DBC_USAGE_ITEM (41,        DBC_FLAG,      19,             0,              60,              0,          0,          1,          BatteryManageSystem_CAT1_BalanceActiveForCell), // frank
    CAN_DBC_USAGE_ITEM (42,        DBC_FLAG,      20,             0,              60,              0,          0,          1,          BatteryManageSystem_CAT1_BalanceActiveForCell),
    CAN_DBC_USAGE_ITEM (43,        DBC_FLAG,      21,             0,              60,              0,          0,          1,          BatteryManageSystem_CAT1_BalanceActiveForCell),
    CAN_DBC_USAGE_ITEM (44,        DBC_FLAG,      22,             0,              60,              0,          0,          1,          BatteryManageSystem_CAT1_BalanceActiveForCell),
    CAN_DBC_USAGE_ITEM (45,        DBC_UINT16,    23,             16,             48,             500,        0,          4500,        BatteryManageSystem_CAT2_VCell_Mx), //frank
    CAN_DBC_USAGE_ITEM (46,        DBC_UINT16,    24,             16,             48,             500,        0,          4500,        BatteryManageSystem_CAT2_VCell_Mx),
    CAN_DBC_USAGE_ITEM (47,        DBC_UINT16,    25,             16,             48,             500,        0,          4500,        BatteryManageSystem_CAT2_VCell_Mx),
    CAN_DBC_USAGE_ITEM (48,        DBC_UINT16,    26,             16,             48,             500,        0,          4500,        BatteryManageSystem_CAT2_VCell_Mx),    
    CAN_DBC_USAGE_ITEM (49,        DBC_FLAG,      27,             8,              3,              0,          0,          7,           BatteryManageSystem_CHRRST),
    CAN_DBC_USAGE_ITEM (50,        DBC_UINT8,     28,             0,              8,              0,          0,          255,         BatteryManageSystem_BMCCHG),
    CAN_DBC_USAGE_ITEM (51,        DBC_FLAG,      29,             8,              40,             0,          0,          0,           BatteryManageSystem_VDAT_M1_SAFETY), //frank add
    CAN_DBC_USAGE_ITEM (52,        DBC_UINT16,    30,             8,              56,             0,          0,          225,         BatteryManageSystem_LDAT_Frames), // 8 frames tsai 11/11
    /* keep the following line in the last line*/ 
    CAN_DBC_USAGE_ITEM (65535,     DBC_UINT16,    0,              0,              0,              0,          0,          0,           BatteryManageSystem_NOUPdate)
};

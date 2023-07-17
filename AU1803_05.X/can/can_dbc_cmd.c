/******************************************************************************
 *
 * FILE: can_dbc_cmd.c
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/

#include "can_dbc_cmd.h"

#define CAN_FRAME_ITEM(frame_name, ptrFrameData) {ptrFrameData}
#define DCB_CMD_FRAME_ITEM(cmd_name, total_frameNum, start_frameNum)        \
    { total_frameNum,                                                       \
      start_frameNum }

UINT8 CanDCB_dataFrame_01[8];
UINT8 CanDCB_dataFrame_02[8];
UINT8 CanDCB_dataFrame_03[8];
UINT8 CanDCB_dataFrame_04[8];
UINT8 CanDCB_dataFrame_05[8];
UINT8 CanDCB_dataFrame_06[8];
UINT8 CanDCB_dataFrame_07[8];
UINT8 CanDCB_dataFrame_08[8];
UINT8 CanDCB_dataFrame_09[8];
UINT8 CanDCB_dataFrame_10[8];
UINT8 CanDCB_dataFrame_11[8];
UINT8 CanDCB_dataFrame_12[8];
UINT8 CanDCB_dataFrame_13[8];
UINT8 CanDCB_dataFrame_14[8];
UINT8 CanDCB_dataFrame_15[8];
UINT8 CanDCB_dataFrame_16[8];
UINT8 CanDCB_dataFrame_17[8];
UINT8 CanDCB_dataFrame_18[8];
UINT8 CanDCB_dataFrame_19[8];
UINT8 CanDCB_dataFrame_20[8];
UINT8 CanDCB_dataFrame_21[8];
UINT8 CanDCB_dataFrame_22[8];
UINT8 CanDCB_dataFrame_23[8];
UINT8 CanDCB_dataFrame_24[8];
UINT8 CanDCB_dataFrame_25[8];
UINT8 CanDCB_dataFrame_26[8];
UINT8 CanDCB_dataFrame_27[8];
UINT8 CanDCB_dataFrame_28[8];
UINT8 CanDCB_dataFrame_29[8];

UINT8 CanDCB_dataFrame_30[8]; //frank 6/30

UINT8 CanDCB_dataFrame_31[8];
UINT8 CanDCB_dataFrame_32[8];
UINT8 CanDCB_dataFrame_33[8];
UINT8 CanDCB_dataFrame_34[8];
UINT8 CanDCB_dataFrame_35[8];
UINT8 CanDCB_dataFrame_36[8];
UINT8 CanDCB_dataFrame_37[8];
UINT8 CanDCB_dataFrame_38[8];
UINT8 CanDCB_dataFrame_39[8];
UINT8 CanDCB_dataFrame_40[8];
UINT8 CanDCB_dataFrame_41[8];
UINT8 CanDCB_dataFrame_42[8];
UINT8 CanDCB_dataFrame_43[8];
UINT8 CanDCB_dataFrame_44[8];
/*UINT8 CanDCB_dataFrame_45[8];
UINT8 CanDCB_dataFrame_46[8];
UINT8 CanDCB_dataFrame_47[8];
UINT8 CanDCB_dataFrame_48[8];
UINT8 CanDCB_dataFrame_49[8];*/

const canFrameNum CanDCB_dataFramePtr[CAN_FRAME_MAX] = {
        CAN_FRAME_ITEM(CAN_FRAME_01,    CanDCB_dataFrame_01),
        CAN_FRAME_ITEM(CAN_FRAME_02,    CanDCB_dataFrame_02),
        CAN_FRAME_ITEM(CAN_FRAME_03,    CanDCB_dataFrame_03),
        CAN_FRAME_ITEM(CAN_FRAME_04,    CanDCB_dataFrame_04),
        CAN_FRAME_ITEM(CAN_FRAME_05,    CanDCB_dataFrame_05),
        CAN_FRAME_ITEM(CAN_FRAME_06,    CanDCB_dataFrame_06),
        CAN_FRAME_ITEM(CAN_FRAME_07,    CanDCB_dataFrame_07),
        CAN_FRAME_ITEM(CAN_FRAME_08,    CanDCB_dataFrame_08),
        CAN_FRAME_ITEM(CAN_FRAME_09,    CanDCB_dataFrame_09),
        CAN_FRAME_ITEM(CAN_FRAME_10,    CanDCB_dataFrame_10),
        CAN_FRAME_ITEM(CAN_FRAME_11,    CanDCB_dataFrame_11),
        CAN_FRAME_ITEM(CAN_FRAME_12,    CanDCB_dataFrame_12),
        CAN_FRAME_ITEM(CAN_FRAME_13,    CanDCB_dataFrame_13),
        CAN_FRAME_ITEM(CAN_FRAME_14,    CanDCB_dataFrame_14),
        CAN_FRAME_ITEM(CAN_FRAME_15,    CanDCB_dataFrame_15),
        CAN_FRAME_ITEM(CAN_FRAME_16,    CanDCB_dataFrame_16),
        CAN_FRAME_ITEM(CAN_FRAME_17,    CanDCB_dataFrame_17),
        CAN_FRAME_ITEM(CAN_FRAME_18,    CanDCB_dataFrame_18),
        CAN_FRAME_ITEM(CAN_FRAME_19,    CanDCB_dataFrame_19),
        CAN_FRAME_ITEM(CAN_FRAME_20,    CanDCB_dataFrame_20),
        CAN_FRAME_ITEM(CAN_FRAME_21,    CanDCB_dataFrame_21),
        CAN_FRAME_ITEM(CAN_FRAME_22,    CanDCB_dataFrame_22),
        CAN_FRAME_ITEM(CAN_FRAME_23,    CanDCB_dataFrame_23),
        CAN_FRAME_ITEM(CAN_FRAME_24,    CanDCB_dataFrame_24),
        CAN_FRAME_ITEM(CAN_FRAME_25,    CanDCB_dataFrame_25),
        CAN_FRAME_ITEM(CAN_FRAME_26,    CanDCB_dataFrame_26),
        CAN_FRAME_ITEM(CAN_FRAME_27,    CanDCB_dataFrame_27),
        CAN_FRAME_ITEM(CAN_FRAME_28,    CanDCB_dataFrame_28),      
        CAN_FRAME_ITEM(CAN_FRAME_29,    CanDCB_dataFrame_29),
     
        CAN_FRAME_ITEM(CAN_FRAME_30,    CanDCB_dataFrame_30),  //frank 6/30
          
        CAN_FRAME_ITEM(CAN_FRAME_31,    CanDCB_dataFrame_31),
        CAN_FRAME_ITEM(CAN_FRAME_32,    CanDCB_dataFrame_32),
        CAN_FRAME_ITEM(CAN_FRAME_33,    CanDCB_dataFrame_33),
        CAN_FRAME_ITEM(CAN_FRAME_34,    CanDCB_dataFrame_34),
        CAN_FRAME_ITEM(CAN_FRAME_35,    CanDCB_dataFrame_35),
        CAN_FRAME_ITEM(CAN_FRAME_36,    CanDCB_dataFrame_36),
        CAN_FRAME_ITEM(CAN_FRAME_37,    CanDCB_dataFrame_37),
        CAN_FRAME_ITEM(CAN_FRAME_38,    CanDCB_dataFrame_38),
        CAN_FRAME_ITEM(CAN_FRAME_39,    CanDCB_dataFrame_39),
        CAN_FRAME_ITEM(CAN_FRAME_40,    CanDCB_dataFrame_40),
        CAN_FRAME_ITEM(CAN_FRAME_41,    CanDCB_dataFrame_41),
        CAN_FRAME_ITEM(CAN_FRAME_42,    CanDCB_dataFrame_42),
        CAN_FRAME_ITEM(CAN_FRAME_43,    CanDCB_dataFrame_43),
        CAN_FRAME_ITEM(CAN_FRAME_44,    CanDCB_dataFrame_44)
 /*       CAN_FRAME_ITEM(CAN_FRAME_45,    CanDCB_dataFrame_45),
        CAN_FRAME_ITEM(CAN_FRAME_46,    CanDCB_dataFrame_46),
        CAN_FRAME_ITEM(CAN_FRAME_47,    CanDCB_dataFrame_47),
        CAN_FRAME_ITEM(CAN_FRAME_48,    CanDCB_dataFrame_48),
        CAN_FRAME_ITEM(CAN_FRAME_49,    CanDCB_dataFrame_49)
  */
};

const dcbUsage DCB_UIT[DCB_CMD_MAX] = {
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_SYSREQ,     1,      0),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_VER,        11,      1),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_MDAT,       6,      12),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_CAT0,       1,      18),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_CAT1,       4,      19),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_CAT2,       4,      23),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_CHRST,      1,      27),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_BMCCHG,     1,      28),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_VDAT,       1,      29),
    DCB_CMD_FRAME_ITEM(DCB_CMD_NAME_LDAT,       10,     30)                  
};




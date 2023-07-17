/******************************************************************************
 *
 * FILE: can_dbc_cmd.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_DBC_CMD_H
#define	CAN_DBC_CMD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include  "../main_app/platform_types.h"

    
typedef enum {
    CAN_FRAME_01 = 0,
    CAN_FRAME_02,
    CAN_FRAME_03,
    CAN_FRAME_04,
    CAN_FRAME_05,
    CAN_FRAME_06,
    CAN_FRAME_07,
    CAN_FRAME_08,
    CAN_FRAME_09,
    CAN_FRAME_10,
    CAN_FRAME_11,
    CAN_FRAME_12,
    CAN_FRAME_13,
    CAN_FRAME_14,
    CAN_FRAME_15,
    CAN_FRAME_16,
    CAN_FRAME_17,
    CAN_FRAME_18,
    CAN_FRAME_19,
    CAN_FRAME_20,
    CAN_FRAME_21,
    CAN_FRAME_22,
    CAN_FRAME_23,
    CAN_FRAME_24,
    CAN_FRAME_25,
    CAN_FRAME_26,
    CAN_FRAME_27,
    CAN_FRAME_28,
    CAN_FRAME_29,
          
    CAN_FRAME_30,  //frank 6/30
              
    CAN_FRAME_31,
    CAN_FRAME_32,
    CAN_FRAME_33,
    CAN_FRAME_34,
    CAN_FRAME_35,
    CAN_FRAME_36,
    CAN_FRAME_37,
    CAN_FRAME_38,
    CAN_FRAME_39,
    CAN_FRAME_40,
    CAN_FRAME_41,
    CAN_FRAME_42,
    CAN_FRAME_43,
    CAN_FRAME_44,
//    CAN_FRAME_45,
//    CAN_FRAME_46,
//    CAN_FRAME_47,
    CAN_FRAME_MAX                       //Keep at the end.
} CAN_Frame_t;


typedef enum {
    DCB_CMD_NAME_SYSREQ = 0,
    DCB_CMD_NAME_VER,
    DCB_CMD_NAME_MDAT,
    DCB_CMD_NAME_CAT0,
    DCB_CMD_NAME_CAT1,
    DCB_CMD_NAME_CAT2,
    DCB_CMD_NAME_CHRST,
    DCB_CMD_NAME_BMCCHG,        
    DCB_CMD_NAME_VDAT,                    
    DCB_CMD_NAME_LDAT,   
    DCB_CMD_NAME_CAT3,
    DCB_CMD_NAME_CAT4,
    DCB_CMD_NAME_CAT5,
    DCB_CMD_NAME_CAT6,
    DCB_CMD_NAME_CAT7,            
    DCB_CMD_MAX                           //Keep at the end.
} DCB_AllCmdUsage_t;

//--------------------------------------------------
//	DCB Command Codes
//--------------------------------------------------
typedef struct {		
	unsigned char * ptrFrame;
} canFrameNum;
    
extern const canFrameNum CanDCB_dataFramePtr[CAN_FRAME_MAX] ;

typedef struct {		
	unsigned char totalFrameNum;
	unsigned char startFrameNum;
} dcbUsage;

extern const dcbUsage DCB_UIT[DCB_CMD_MAX] ;


#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_CMD_H */


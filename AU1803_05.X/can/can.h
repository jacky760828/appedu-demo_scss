/******************************************************************************
 *
 * FILE: can.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_H
#define	CAN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "platform_types.h"
#include "can_dbc.h"
#include "can_dbc_utils.h"
#include "can_dbc_usages_bmftp.h"
#include "../battery_manage_system/battery_manage_system.h"
#include "../main_app/mcu_init.h"
#include "../smbus/smbus.h"
#include "../flash/MX25L.h"
#include "../mcc_generated_files/can1.h"
#include "../mcc_generated_files/can_types.h"


/* CAN filter and mask defines */
//USE FOR STANDARD MESSAGES ONLY 
#define CAN_FILTERMASK2REG_SID(x) ((x & 0x07FF)<< 5)
/* the Macro will set the "MIDE" bit in CiRXMxSID */
#define CAN_SETMIDE(sid) (sid | 0x0008)
/* the macro will set the EXIDE bit in the CiRXFxSID to accept extended messages only */
#define CAN_FILTERXTD(sid) (sid | 0x0008)
/* the macro will clear the EXIDE bit in the CiRXFxSID to accept standard messages only */
#define CAN_FILTERSTD(sid) (sid & 0xFFF7)

    
typedef enum {
    CircBufState_EMPTY,     // the buffer is empty
    CircBufState_PARTIAL,   // the buffer is partially filled
    CircBufState_FULL       // the buffer is full
} CircBufState_t;


typedef struct {
    struct {
        uCAN_MSG msgBuf[64];
        UINT8 headIdx;
        UINT8 tailIdx;
        CircBufState_t state;
    } rx;
    struct {
        uCAN_MSG msgBuf[64];
        UINT8 headIdx;
        UINT8 tailIdx;
        CircBufState_t state;
    } tx;
} CommCanBuf_t;

// frank add 4/16 for tracking bms state
// 7/8/2021
//#define TRACK_STATES  // add debug BMS state debeug purpose


void ClearCAN_Buffer();
void CAN_init(void);
void CAN_interruptHandler_rxComplete(void);//UINT8 rxBufNum);
CircBufState_t CAN_getReceiveBufferState(void);
CircBufState_t CAN_getTransmitBufferState(void);
void CAN_getItemFromReceiveBuffer(uCAN_MSG *pDestBuf);
void CAN_putItemIntoTransmitBuffer(uCAN_MSG *pSrcBuf);
void tx_mainFunction(void);
#ifdef	__cplusplus
}
#endif

#endif	/* CAN_H */


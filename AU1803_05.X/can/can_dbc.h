/******************************************************************************
 *
 * FILE: can_dbc.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef CAN_DBC_H
#define	CAN_DBC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../battery_manage_system/battery_manage_system.h"
#include "can_dbc_mem_map_uit.h"
#include "can_dbc_cmd.h"
#include "../sys_time/sys_time.h"
#ifdef EVENT_LOG
#include "../EventLogData/EventLog.h" 
#endif   

#define IntoFET_300ms                   500

#define CAN_DBC_DEBUG_LOG               (0)

typedef enum {
    Comm_CanDcb_TX_IDLE = 0,                //Idle state
    Comm_CanDcb_TX_WAIT_BUFFER_EMPTY,
    Comm_CanDcb_TX_PUT_DATA_INTO_BUFFER,
    Comm_CanDcb_TX_OMPLETE
} Comm_CanDbc_TX_t;

typedef struct {
	Comm_CanDbc_TX_t    txState;
    uint32_t            cmdID;
    uint8_t             requestID;
    uint8_t             requestType;
    uint8_t             muxNum;
    uint8_t             frameIdx;
    uint8_t             stopFrame;
} CanDbc_TX_t;
    
typedef struct {
    uint32_t    cmdID;
    uint8_t     index;
    uint8_t     data[8];
} CanDbc_RX_t;

typedef struct {
    uint32_t    cmdID;
    uint8_t     index;
    uint8_t     data[8];
} Can_TX_t;


extern CanDbc_RX_t ian_canbus;
extern Can_TX_t ian_T_canbus[64];
extern uint8_t SerialNum[18];
extern uint8_t SaveFlashCmd;
extern uint16_t CanResponse30sCnt;
extern uint32_t CAN_Rx_EventTotalCnts;
extern uint8_t NoIntoEventRecentFlag;

bool CAN_DBC_Get_UPS_CTRL(void);
bool CAN_DBC_Get_Saveflash(void);
void CAN_DBC_Set_Saveflash(bool onoff);
void CanDbc_init(void);
void CanRx_EvnetFunction(void);
void CanDbc_mainFunction(void);
void ParserAuxReqCmd(uCAN_MSG *tempBuf); //frank 9/11
void RespAuxReqCmd(void); //frank 9/11
void BMFTPReqCmd(uCAN_MSG *tempBuf); //hugo 20210222

#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_H */


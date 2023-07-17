/******************************************************************************
 *
 * FILE: can_dbc_usages_auxresp.c
 *
 * Copyright(c) 2020 C-Technology United.
 * All Rights Reserved
 * Author : frank tsai
 * Overview: frank tsai
 *
 *****************************************************************************/

#include "can.h"
#include "../i2c/i2c_a1006.h"

#define DBC_CMD_AUXRESP  0x680   //can id
static uint8_t can_dbc_usages_auxresp_muxIdx = 0;

void sendDataToHost(uCAN_MSG *msgBuf, uint8_t *srcBuffer, uint8_t size )
{
    uint8_t i;

    if(CAN_getTransmitBufferState() == CircBufState_FULL) {
        return;
    }
    msgBuf->frame.data0 = can_dbc_usages_auxresp_muxIdx;
    msgBuf->frame.data0 |=  (A1006_Auth.reqAuthStep << 4);
    if (can_dbc_usages_auxresp_muxIdx == 0) {        
        if((size - 5) % 7) {
            i = (size - 5) / 7 + 1 + 1;
        } else {
            i = (size - 5) / 7 + 1;
        }
        
        A1006_Auth.frameRange = i;
        msgBuf->frame.data1 = A1006_Auth.reqID + (i << 4);
        msgBuf->frame.data2 = A1006_Auth.respID; //this byte undefine
        for (A1006_Auth.dataOffset=0; A1006_Auth.dataOffset < 5; A1006_Auth.dataOffset++) {
            msgBuf->array[7 + 3 + A1006_Auth.dataOffset] =  srcBuffer[A1006_Auth.dataOffset];
        }
    } else {
        for (i=1; i < msgBuf->frame.dlc; i++) {
            if (A1006_Auth.dataOffset < size) {
                msgBuf->array[7 + i] =  srcBuffer[A1006_Auth.dataOffset++];
            } else {
                msgBuf->array[7 + i] = 0 ;
            }
        }
    }
    CAN_putItemIntoTransmitBuffer(msgBuf);
    can_dbc_usages_auxresp_muxIdx++;
    if ((A1006_Auth.dataOffset >= size) || (can_dbc_usages_auxresp_muxIdx >= A1006_Auth.frameRange)) {
        A1006_Auth.StartreqComplete = 1;
        can_dbc_usages_auxresp_muxIdx = 0;
    }
}

void RespAuxReqCmd(void)
{
    uCAN_MSG msgBuf;
    if (A1006_Auth.StartreqComplete) {
            return;
    }
    msgBuf.frame.id = ((DBC_CMD_AUXRESP & 0xFF0) | d_deviceAddr);
    msgBuf.frame.idType = CAN_FRAME_STD;
    msgBuf.frame.dlc = 8;
    switch (A1006_Auth.reqAuthStep) {
        case GET_CERTIF_1 :
            // (2+128)/2=65
            sendDataToHost(&msgBuf, A1006_Auth.Certificate, ((2 + A1006_CERTIF_size) / 2)); 
            break;
            
        case GET_CERTIF_2 :
            // (2+128)/2=65
            sendDataToHost(&msgBuf, &A1006_Auth.Certificate[65], ((2 + A1006_CERTIF_size) / 2));
            break;
            
        case GET_UID :
            sendDataToHost(&msgBuf, A1006_Auth.Signature, (2 + A1006_UID_size));
            break;
            
        case GET_RESPONSE :
            sendDataToHost(&msgBuf, A1006_Auth.Response, (2 + A1006_RESPONSE_size));
            break;
        
        default:
            break;
    }
}
/******************************************************************************
 *
 * FILE: can_dbc_usages_auxreq.c
 *
 * Copyright(c) 2020 C-Technology United.
 * All Rights Reserved
 * Author : frank tsai
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"
#include "../sys_time/sys_time.h"
#include "../i2c/i2c_a1006.h"


typedef union{ 
    struct {
        uint8_t multiplexor : 4;
        uint8_t reqType     : 4;
        uint8_t reqID       : 4; 
        uint8_t frameRange  : 4;
        uint8_t respID      : 8; 
        uint8_t zeroFramedata[5];
    };
    uint8_t eightbytes[8];
} AuthFrame_t;


#define AUTH_REQUEST_TIMEOUT_MS     10000 //7000 ms
#define CHALLENGE_DATA_SIZE         (3 + 44)
UINT32 d_AuthReqTime;

void putToDataBuffer(uint8_t *targetBuf, uint8_t *sourceBuf,  uint8_t size, uint8_t total)
{
    uint8_t index;
    
    for (index = 0; index < size; index++) {
        if (A1006_Auth.dataOffset < total) {
            targetBuf[A1006_Auth.dataOffset++] = sourceBuf[index];
        } else {
            break;
        }
    }
}

void ParserAuxReqCmd(uCAN_MSG *tempBuf)
{
    AuthFrame_t AuthFrame;
    uint8_t index;
    
    if (!A1006_Auth.StartreqComplete) {
        if(SysTime_msecIntervalElapsed(d_AuthReqTime, AUTH_REQUEST_TIMEOUT_MS)) {
            A1006_Auth.StartreqComplete = 1; //reset
        } else {
            return;
        }
    }
    printf("eightbytes:");
    for (index = 0; index < 8; index++) {
        AuthFrame.eightbytes[index] = tempBuf->array[7 + index];
        printf("0x%02X ", AuthFrame.eightbytes[index]);
    }
    printf("\r\n");
    d_AuthReqTime =  SysTime_getMsec();
    if (AuthFrame.multiplexor == 0) {
        if ((AuthFrame.respID == d_deviceAddr) || (AuthFrame.respID == 0xFF)) {
            A1006_Auth.respID = AuthFrame.respID;
            A1006_Auth.reqID = AuthFrame.reqID;
            A1006_Auth.dataOffset = 0;
            A1006_Auth.reqAuthStep = AuthFrame.reqType;
            A1006_Auth.frameRange = AuthFrame.frameRange;
            if (AuthFrame.reqType != SEND_CHALLENGE) {
                A1006_Auth.StartreqComplete = 0;
            }
        } else {
            return;
        }
    }
    
    if (AuthFrame.reqType == SEND_CHALLENGE) {
        if (A1006_Auth.dataOffset  < CHALLENGE_DATA_SIZE ) {
            if (AuthFrame.multiplexor == 0) {
                putToDataBuffer( A1006_Auth.Challenge, AuthFrame.zeroFramedata, 5, CHALLENGE_DATA_SIZE);
            } else {
                putToDataBuffer( A1006_Auth.Challenge, &AuthFrame.eightbytes[1], 7, CHALLENGE_DATA_SIZE);
            }
        }
        if ((A1006_Auth.dataOffset >= CHALLENGE_DATA_SIZE) || (A1006_Auth.frameRange <= AuthFrame.multiplexor)) {
            setRecChallenge();
            A1006_Auth.getChallenge_flag = 1;
            A1006_Auth.StartreqComplete = 1;
            A1006_Auth.sendChallenge_flag = true;
        }
    }
}

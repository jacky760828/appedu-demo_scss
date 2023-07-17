/******************************************************************************
 *
 * FILE: i2c_a1006.c
 *
 * Copyright(c) 2020 C-Technology United.
 * All Rights Reserved
 * Author : frank tsai
 * Overview: for NXP A1006 authentication IC
 *
 *****************************************************************************/

#include "i2c_a1006.h"

#define debug 0
#define debug_getSignature_data 0
#define debug_getCertificate_data 0
#define debug_sendChallenge_data 0
#define debug_getResponse_data 0

Auth_TypIeDef A1006_Auth;
#if debug
uint8_t DEBUG_Challenge[47] = {0x00, 0x00, 0x2C, 0x05, 0xB5, 0xBF, 0xBB, 0x2C, 0xA7, 0x85, 0x5C, 0x5E, 0xE6, 0x31, 0x60, 0x75, 0xEF, 0xCF, 0xBC, 0x98, 0xA0, 0x10, 0x21, 0x5E, 0x36, 0x03, 0x8B, 0xD8, 0xEA, 0x40, 0xB6, 0x70, 0x34, 0xB7, 0x32, 0xF9, 0xEC, 0x78, 0x3D, 0x41, 0xEC, 0x73, 0xAB, 0xFE, 0xA4, 0xE7, 0x8F};
#endif


void i2cA1006Read(uint16_t addr, uint8_t rSize)
{
    A1006_I2C_BUFF.communication_format = I2C;
    
    I2C1_STATE_INT_FLAG = 0x0;
    I2C1_START_CONDITION_ENABLE_BIT = 0x1;              //send START
    A1006_I2C_BUFF.SMB_timer = SysTime_getMsec();
    A1006_I2C_BUFF.SLAVE_ADDRESS = A1006_I2C_ADDR;
    A1006_I2C_BUFF.WR = 0;
    A1006_I2C_BUFF.DATA_Type = BYTE;
    A1006_I2C_BUFF.READ_CMD_Size = 2;
    A1006_I2C_BUFF.READ_CMD_index = 0;
    A1006_I2C_BUFF.READ_CMD_Buffer[0] = (uint8_t)(addr >> 8);
    A1006_I2C_BUFF.READ_CMD_Buffer[1] = (uint8_t)addr;
    A1006_I2C_BUFF.RxDataSize = rSize;
    A1006_I2C_BUFF.Rxindex = 0;
    A1006_I2C_BUFF.READ_DATA_CRC = false;
    A1006_I2C_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS;
    A1006_I2C_BUFF.Error = I2C_MESSAGE_PENDING;
}

void i2cA1006Write(uint8_t *wBuffer)
{
    int i;
    
    A1006_I2C_BUFF.communication_format = I2C;
    I2C1_STATE_INT_FLAG = 0x0;
    I2C1_START_CONDITION_ENABLE_BIT = 0x1;              //send START
    A1006_I2C_BUFF.DATA_Type = BYTE;
    A1006_I2C_BUFF.SMB_timer = SysTime_getMsec();
    A1006_I2C_BUFF.SLAVE_ADDRESS = A1006_I2C_ADDR;
    A1006_I2C_BUFF.WR = 0;
    A1006_I2C_BUFF.WRITE_CMD_Size = 0;
    A1006_I2C_BUFF.TxDataSize = (wBuffer[2] + 3);
    for(i = 0; i < A1006_I2C_BUFF.TxDataSize; i++) {
        A1006_I2C_BUFF.TxBuffer[i] = wBuffer[i];
    }
    A1006_I2C_BUFF.READ_CMD_Size = 0;
    A1006_I2C_BUFF.RxDataSize = 0;
    A1006_I2C_BUFF.WRITE_DATA_CRC = 0;
    A1006_I2C_BUFF.SMB_state = SmbusState_CMD_WRITE_SLAVE_ADDRESS;
    A1006_I2C_BUFF.Error = I2C_MESSAGE_PENDING;
}

void initAuthentification(void)
{
    A1006_Auth.Exist = 0;
    A1006_Auth.AuthStep = GET_UID;
    A1006_Auth.StartreqComplete = 1;
    printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
}

void setRecChallenge(void)
{
    if ((A1006_Auth.reqAuthStep == SEND_CHALLENGE)) {
        A1006_Auth.AuthStep = SEND_CHALLENGE;
        printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
    }
}

void a1006Authentication(void)
{
    if(IDLE == A1006_I2C_BUFF.communication_format) {
        switch(A1006_Auth.AuthStep) {
            case GET_UID :
                i2cA1006Read(A1006_UID, A1006_UID_size + 2);
                break;
            case GET_CERTIF:
                i2cA1006Read(A1006_CERTIF, A1006_CERTIF_size + 2);
                break;
            case SEND_CHALLENGE:
                i2cA1006Write(A1006_Auth.Challenge);
                break;
            case GOT_RESPONSE:
                i2cA1006Read(A1006_RESPONSE, A1006_RESPONSE_size + 2);
                break;
        }
    } else if(SmbusState_CMD_READ_COMPLETE == A1006_I2C_BUFF.SMB_state) {
        if(I2C == A1006_I2C_BUFF.communication_format) {
            switch(A1006_Auth.AuthStep) {
                case GET_UID :
                    i2cstate_Get_RXBUFF(A1006_Auth.Signature, 0);
#if debug_getSignature_data
                    printf("getSignature:");
                    for(i = 0; i < A1006_UID_size+2; i++) {
                        printf("0x%02X, ", A1006_Auth.Signature[i]);
                    }
                    printf("\r\n");
#endif
                    A1006_Auth.getSignature_flag = 1;
                    A1006_Auth.Exist = 1;
                    A1006_Auth.AuthStep = GET_CERTIF;
                    printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
                    A1006_I2C_BUFF.SMB_state = SmbusState_IDLE;
                    break;
                case GET_CERTIF:
                    i2cstate_Get_RXBUFF(A1006_Auth.Certificate, 0);
#if debug_getCertificate_data
                    printf("getCertificate:");
                    for(i = 0; i < A1006_CERTIF_size+2; i++) {
                        printf("0x%02X, ", A1006_Auth.Certificate[i]);
                    }
                    printf("\r\n");
#endif
                    A1006_Auth.getCertificate_flag = 1;
                    A1006_Auth.AuthStep = WAIT_CHALLENGE_DATA;
                    printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
                    A1006_I2C_BUFF.SMB_state = SmbusState_IDLE;
                    break;

                case GOT_RESPONSE:
                    i2cstate_Get_RXBUFF(A1006_Auth.Response, 0);
#if debug_getResponse_data
                    printf("getResponse:");
                    for(i = 0; i < A1006_RESPONSE_size+2; i++) {
                        printf("0x%02X, ", A1006_Auth.Response[i]);
                    }
                    printf("\r\n");
#endif
                    A1006_Auth.getResponse_flag = 1;
                    A1006_Auth.AuthStep = AUTH_COMPLETE;
                    printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
                    A1006_I2C_BUFF.SMB_state = SmbusState_IDLE;
                    break;
            }
        }
    } else if(SmbusState_CMD_WRITE_COMPLETE == A1006_I2C_BUFF.SMB_state) {
        if(I2C == A1006_I2C_BUFF.communication_format) {
            switch(A1006_Auth.AuthStep) {
                case SEND_CHALLENGE:
                    if(A1006_Auth.sendChallenge_flag) {
                        A1006_Auth.AuthStep = GOT_RESPONSE;
                        printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
                        A1006_I2C_BUFF.SMB_state = SmbusState_IDLE;
                    }
                    break;
            }
        }
    } else {
        switch(A1006_Auth.AuthStep) {
            case WAIT_CHALLENGE_DATA:
                if(A1006_Auth.getChallenge_flag) {
                    A1006_Auth.AuthStep = SEND_CHALLENGE;
                    printf("AuthStep:%d \r\n", A1006_Auth.AuthStep);
                }
                break;
            case AUTH_COMPLETE:
                break;
        }
        
    }
}

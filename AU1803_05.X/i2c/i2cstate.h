/******************************************************************************
 *
 * FILE: i2cstate.h
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/


#ifndef _I2CSTATE_H
#define _I2CSTATE_H

/**
  Section: Included Files
*/
#include "stddef.h"
#include "i2chw.h"
#include "../sys_time/sys_time.h"
#include <stdbool.h>

#include "../smbus/smbus.h"
#include "../i2c/i2c_a1006.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define I2C_TX_BUFF_MAX     255
#define I2C_RX_BUFF_MAX     255
#define I2C_WRITE_CMD_MAX   50
#define I2C_READ_CMD_MAX    50
#define I2C_100Kbps         0x13C
#define I2C_TIME_OUT_MS     500//ms
/**
  I2C Master Driver State Enumeration

  @Summary
    Defines the different states of the i2c master.

  @Description
    This defines the different states that the i2c master
    used to process transactions on the i2c bus.
*/

typedef enum {
    IDLE,
    I2C,
    SMBUS,
} I2C_format;

typedef enum {
    I2C_START,
    I2C_MESSAGE_PENDING,
    I2C_MESSAGE_COMPLETE,
    I2C_STUCK_START,
    I2C_MESSAGE_ADDRESS_NO_ACK,
    I2C_DATA_NO_ACK,
    I2C_LOST_STATE,
    I2C_WRITE_COLLISION,
    I2C_TIME_OUT
} I2C_MESSAGE_STATUS;

typedef enum {
	SmbusState_IDLE,
    SmbusState_CMD_WRITE_SLAVE_ADDRESS,
    SmbusState_CMD_WRITE_CMD_CODE,
    SmbusState_CMD_WRITE_BYTE_COUNT,
    SmbusState_CMD_WRITE_DATA,
	SmbusState_CMD_WRITE_CRC,
    SmbusState_CMD_WRITE_STOP,
    SmbusState_CMD_WRITE_COMPLETE,
    SmbusState_CMD_READ_SLAVE_ADDRESS,
    SmbusState_CMD_READ_CMD_CODE,
    SmbusState_CMD_READ_SLAVE_START_REPEAT,
    SmbusState_CMD_READ_SLAVE_ADDRESS_REPEAT,
    SmbusState_CMD_READ_SLAVE_ADDRESS_ACK,
    SmbusState_CMD_READ_RECEIVE_DATA_BYTE_NUM,
    SmbusState_CMD_READ_DATA,
    SmbusState_CMD_READ_NEXT_DATA,
    SmbusState_CMD_READ_LAST_DATA,
	SmbusState_CMD_READ_CRC,
	SmbusState_CMD_READ_STOP,
    SmbusState_CMD_READ_COMPLETE,
    // error        
    SmbusState_CMD_WRITE_ERROR = 0x80,     //20
    SmbusState_CMD_WRITE_TIMEOUT,   //21
    SmbusState_CMD_READ_ERROR,      //22
    SmbusState_CMD_READ_TIMEOUT,    //23
    SmbusState_BUS_ERROR,           //24
    SmbusState_BUS_WAIT_OFF,
    SmbusState_BUS_WAIT_RESTART,
} SmbusState_t;

typedef struct {
    I2C_format communication_format;
    union{
        unsigned char SLAVE_ADDRESS;
        struct {
            unsigned WR:1 ;
            unsigned SADDR:7;
        };
    };
    uint8_t Txindex;
    uint8_t TxDataSize;
    uint8_t TxBuffer[I2C_TX_BUFF_MAX];
    uint8_t TxBuffer_CRC;
    uint8_t Rxindex;
    uint8_t RxDataSize;
    uint8_t RxBuffer[I2C_RX_BUFF_MAX];
    uint8_t RxBuffer_CRC;
    uint8_t REC_CRC;
    uint8_t WRITE_CMD_Buffer[I2C_WRITE_CMD_MAX];
    uint8_t WRITE_CMD_index;
    uint8_t WRITE_CMD_Size;
    uint8_t READ_CMD_Buffer[I2C_READ_CMD_MAX];
    uint8_t READ_CMD_index;
    uint8_t READ_CMD_Size;
    uint8_t MacCmdAccess:1;
    uint8_t MacCmdSet:1;
    uint8_t DATA_Type:2;
    uint8_t WRITE_DATA_CRC:1;
    uint8_t READ_DATA_CRC:1;
    uint32_t timer;
    uint32_t SMB_timer;
    SmbusState_t SMB_state;
    I2C_MESSAGE_STATUS Error;
} I2C_Buffer_TypIeDef;

extern I2C_Buffer_TypIeDef I2C_SMB_BUFF;
//extern I2C_Buffer_TypIeDef A1006_I2C_BUFF;
//extern I2C_Buffer_TypIeDef BQ78350_I2C_BUFF;

void I2C1_Initialize(uint16_t BRG);
void I2C1_ReStart(void);
void I2C1_Off(void);
void I2C1_Stop(I2C_MESSAGE_STATUS completion_code);
I2C_MESSAGE_STATUS I2C1_getError(void);
void i2c_Machine(void);
void i2cstate_Get_RXBUFF(uint8_t* dataRxBuf, uint8_t startbyte);

/**
 Section: Data Type Definitions
*/

/**
  I2C Driver Message Status Type Enumeration

  @Summary
    Defines the different message status when processing
    TRBs.

  @Description
    This type enumeration specifies the different types of status
    that an i2c request will have. For every submission of an i2c
    transaction, the status of that transaction is available.
    Based on the status, new transactions can be requested to the
    driver or a recovery can be performed to resend the transaction.

 */
   

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_I2CSTATE_H
    
/**
 End of File
*/

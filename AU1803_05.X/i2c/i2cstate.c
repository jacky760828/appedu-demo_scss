/******************************************************************************
 *
 * FILE: i2cstate.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 * Author : frank tsai
 * Overview: 
 *
 *****************************************************************************/
#include "i2cstate.h"

/**
 Section: Data Types
*/

/**
 Section: Macro Definitions
*/

/**
 Section: Local Functions
*/

/**
 Section: Local Variables
*/

I2C_Buffer_TypIeDef I2C_SMB_BUFF;

/*---------------------------------------------------------------------------------------------------------*/

/**
  Section: Interface
*/
void I2C1_Initialize(uint16_t BRG)
{
    I2C1BRG = BRG; //100Kbps
    I2C1CON = 0x8100; 
    I2C1STAT = 0x00;

    I2C_SMB_BUFF.SMB_state = SmbusState_IDLE;
    I2C_SMB_BUFF.Error = I2C_START;
    I2C_SMB_BUFF.timer = 0;
}

void I2C1_ReStart(void)
{
    // ACKEN disabled; STREN disabled; GCEN disabled; SMEN enable; DISSLW enabled; I2CSIDL disabled; ACKDT Sends ACK; SCLREL Holds; RSEN disabled; IPMIEN disabled; A10M 7 Bit; PEN disabled; RCEN disabled; SEN disabled; I2CEN enabled; 
    I2C1CON = 0x8100;
    // BCL disabled; P disabled; S disabled; I2COV disabled; IWCOL disabled; 
    I2C1STAT = 0x00;
    I2C_SMB_BUFF.Txindex = 0;
    I2C_SMB_BUFF.Rxindex = 0;
    I2C_SMB_BUFF.timer = SysTime_getMsec();
}

void I2C1_Start(void)
{
    I2C1CONbits.I2CEN = 1;
    I2C_SMB_BUFF.Error = I2C_STUCK_START;
    I2C_SMB_BUFF.timer = 0;
}

void I2C1_Off(void)
{
    I2C1CONbits.I2CEN = 0;
    I2C_SMB_BUFF.Error = I2C_STUCK_START;
    I2C_SMB_BUFF.timer = 0;
}

void i2c_Machine(void)
{
//    uint8_t i;
    
    if((I2C_SMB_BUFF.SMB_state != SmbusState_IDLE) && (I2C_SMB_BUFF.SMB_state >= SmbusState_CMD_WRITE_ERROR)) {
        if(I2C1_BCL_STATUS_BIT) {
            I2C1_BCL_STATUS_BIT = 0x0;    //clear bus collision flag
            I2C1_STATE_INT_FLAG = 0x0;    //just in case
            I2C_SMB_BUFF.SMB_state = SmbusState_BUS_ERROR;
            printf("\e[0;31m i2c -ERR(0x%02X) \033[0m \r\n", I2C_SMB_BUFF.SMB_state);
        }
    }
    
    switch(I2C_SMB_BUFF.SMB_state) {
        case SmbusState_IDLE:
            I2C_SMB_BUFF.communication_format = IDLE;
            break;
            
        case SmbusState_CMD_READ_SLAVE_ADDRESS:
            if(I2C1_STATE_INT_FLAG & I2C1_START_STATUS_BIT) {
                //START has been detected on bus
                I2C1_STATE_INT_FLAG = 0x0;
                I2C_SMB_BUFF.Rxindex = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.SLAVE_ADDRESS;
//                printf("READ_SLAVE_ADDRESS(0x%02X) \n\r", I2C_SMB_BUFF.SLAVE_ADDRESS);
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    I2C_SMB_BUFF.RxBuffer_CRC = I2C_crc8Calc(0, I2C_SMB_BUFF.SLAVE_ADDRESS);
                }
                if(I2C_SMB_BUFF.READ_CMD_Size) {
                    I2C_SMB_BUFF.READ_CMD_index = 0;
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_CMD_CODE;
                } else {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS_ACK;
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(SLAVE_ADDRESS) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_CMD_CODE:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //slave address has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.READ_CMD_Buffer[I2C_SMB_BUFF.READ_CMD_index];
//                printf("READ_CMD_CODE(0x%02X) \n\r", I2C_SMB_BUFF.READ_CMD_Buffer[I2C_SMB_BUFF.READ_CMD_index]);
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    I2C_SMB_BUFF.RxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.RxBuffer_CRC, I2C_SMB_BUFF.READ_CMD_Buffer[I2C_SMB_BUFF.READ_CMD_index]);
                }
                I2C_SMB_BUFF.READ_CMD_index ++;
                if(I2C_SMB_BUFF.READ_CMD_index < I2C_SMB_BUFF.READ_CMD_Size) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_CMD_CODE;
                } else {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_START_REPEAT;
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(READ_CMD_CODE) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_SLAVE_START_REPEAT:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //sbs command has been sent and received an acknowledge from slave device
                I2C1_REPEAT_START_CONDITION_ENABLE_BIT = 0x1;                 //send repeat START
                I2C_SMB_BUFF.WR = 1;
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS_REPEAT;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(START_REPEAT) \033[0m \r\n", I2C_SMB_BUFF.SLAVE_ADDRESS);
                }
            }
            break;
            
        case SmbusState_CMD_READ_SLAVE_ADDRESS_REPEAT:
            if(I2C1_STATE_INT_FLAG & I2C1_START_STATUS_BIT) {
                //Repeat-START has been detected on bus
                I2C1_STATE_INT_FLAG = 0x0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.SLAVE_ADDRESS;
//                printf("READ_SLAVE_ADDRESS_REPEAT(0x%02X) \n\r", I2C_SMB_BUFF.SLAVE_ADDRESS);
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    I2C_SMB_BUFF.RxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.RxBuffer_CRC, I2C_SMB_BUFF.SLAVE_ADDRESS);
                }
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS_ACK;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(ADDRESS_REPEAT) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_SLAVE_ADDRESS_ACK:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //slave address has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                
                I2C1_RECEIVE_ENABLE_BIT  = 0x1;                 //configure as a receiver
                if(I2C_SMB_BUFF.DATA_Type == BYTE || I2C_SMB_BUFF.DATA_Type == WORD) {
                    I2C_SMB_BUFF.Rxindex = 0;
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_DATA;
                } else {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_RECEIVE_DATA_BYTE_NUM;
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(SLAVE_ADDRESS_ACK) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_RECEIVE_DATA_BYTE_NUM:
            if(I2C1_STATE_INT_FLAG & I2C1_RBF_STATUS_BIT) {
                I2C1_ACKNOWLEDGE_ENABLE_BIT = 1;                  //enable acknowledge (ACK) here
                I2C1_ACKNOWLEDGE_DATA_BIT = 0;                    //send low at SDA
                
                I2C1_STATE_INT_FLAG = 0;
                I2C_SMB_BUFF.RxDataSize = I2C1_RECEIVE_REG;
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    I2C_SMB_BUFF.RxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.RxBuffer_CRC, I2C_SMB_BUFF.RxDataSize);
                }
                I2C_SMB_BUFF.Rxindex = 0;
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_NEXT_DATA;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(DATA_BYTE_NUM) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_DATA:
            if(I2C1_STATE_INT_FLAG & I2C1_RBF_STATUS_BIT) {
                I2C1_STATE_INT_FLAG = 0;
                
                I2C1_ACKNOWLEDGE_ENABLE_BIT = 1;              //enable acknowledge (ACK) here
                I2C1_ACKNOWLEDGE_DATA_BIT = 0;              //send low at SDA
                
                I2C_SMB_BUFF.RxBuffer[I2C_SMB_BUFF.Rxindex] = I2C1_RECEIVE_REG;
//                printf("READ_DATA(0x%02X) \n\r", I2C_SMB_BUFF.RxBuffer[I2C_SMB_BUFF.Rxindex]);
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    I2C_SMB_BUFF.RxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.RxBuffer_CRC, I2C_SMB_BUFF.RxBuffer[I2C_SMB_BUFF.Rxindex]);
//                    printf("RxBuffer_CRC(0x%02X) \n\r", I2C_SMB_BUFF.RxBuffer_CRC);
                }
                if(I2C_SMB_BUFF.Rxindex < I2C_SMB_BUFF.RxDataSize) {
                    I2C_SMB_BUFF.Rxindex ++;
                }
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_NEXT_DATA;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(READ_DATA) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_NEXT_DATA:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_ENABLE_BIT) {
                I2C1_STATE_INT_FLAG = 0;
                
                //acknowledge bit is sent
                I2C1_RECEIVE_ENABLE_BIT  = 1;                   //start next receive
                if(I2C_SMB_BUFF.Rxindex < I2C_SMB_BUFF.RxDataSize) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_DATA;
                } else {
                    if(I2C_SMB_BUFF.READ_DATA_CRC) {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_CRC;
                    } else {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_STOP;
                    }
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(READ_NEXT_DATA) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_CRC:
            if(I2C1_STATE_INT_FLAG & I2C1_RBF_STATUS_BIT) {
                I2C1_STATE_INT_FLAG = 0;
                
                I2C1_ACKNOWLEDGE_ENABLE_BIT = 1;              //enable acknowledge (NACK) here
                I2C1_ACKNOWLEDGE_DATA_BIT = 1;                //send high at SDA
                
                I2C_SMB_BUFF.REC_CRC = I2C1_RECEIVE_REG;
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_STOP;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(READ_CRC) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_STOP:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_ENABLE_BIT) {
                I2C1_STATE_INT_FLAG = 0;
                
                //Enable STOP bit
                I2C1_STOP_CONDITION_ENABLE_BIT = 1;                    //Enable STOP bit
                
                if(I2C_SMB_BUFF.READ_DATA_CRC) {
                    if(I2C_SMB_BUFF.REC_CRC == I2C_SMB_BUFF.RxBuffer_CRC) {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_COMPLETE;
                    } else {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_ERROR;
//                        printf("i2c(0x%02X) -ERR(0x%02X) \r\n", I2C_SMB_BUFF.SMB_state);
                        I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
                    }
                } else {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_COMPLETE;
                }
//                printf("READ_STOP \n\r\n\r");
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_TIMEOUT;
                    printf("\e[0;31m i2c(0x%02X) -ERR(READ_STOP) \033[0m \r\n", I2C_SMB_BUFF.READ_CMD_Buffer[0]);
                }
            }
            break;
            
        case SmbusState_CMD_READ_COMPLETE:
            I2C_SMB_BUFF.Error = I2C_MESSAGE_COMPLETE;
            break;
            
        case SmbusState_CMD_WRITE_SLAVE_ADDRESS:
            if(I2C1_STATE_INT_FLAG & I2C1_START_STATUS_BIT) {
                //START has been detected on bus
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C_SMB_BUFF.Txindex = 0;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.SLAVE_ADDRESS;
//                printf("WRITE_SLAVE_ADDRESS(0x%02X) \r\n", I2C_SMB_BUFF.SLAVE_ADDRESS);
                if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                    I2C_SMB_BUFF.TxBuffer_CRC = I2C_crc8Calc(0, I2C_SMB_BUFF.SLAVE_ADDRESS);
                }
                if(I2C_SMB_BUFF.WRITE_CMD_Size) {
                    I2C_SMB_BUFF.WRITE_CMD_index = 0;
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_CMD_CODE;
                } else {
                    if(I2C_SMB_BUFF.DATA_Type == BYTE || I2C_SMB_BUFF.DATA_Type == WORD) {
                        I2C_SMB_BUFF.Txindex = 0;
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_DATA;
                    } else {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_BYTE_COUNT;
                    }
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_CMD_CODE:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //slave address has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.WRITE_CMD_Buffer[I2C_SMB_BUFF.WRITE_CMD_index];
//                printf("WRITE_CMD_CODE(0x%02X) \r\n",I2C_SMB_BUFF.WRITE_CMD_Buffer[I2C_SMB_BUFF.WRITE_CMD_index]);
                if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                    I2C_SMB_BUFF.TxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.TxBuffer_CRC, I2C_SMB_BUFF.WRITE_CMD_Buffer[I2C_SMB_BUFF.WRITE_CMD_index]);
                }
                I2C_SMB_BUFF.WRITE_CMD_index ++;
                if(I2C_SMB_BUFF.WRITE_CMD_index < I2C_SMB_BUFF.WRITE_CMD_Size) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_CMD_CODE;
                } else if(I2C_SMB_BUFF.TxDataSize) {
                    if(I2C_SMB_BUFF.DATA_Type == BYTE || I2C_SMB_BUFF.DATA_Type == WORD) {
                        I2C_SMB_BUFF.Txindex = 0;
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_DATA;
                    } else {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_BYTE_COUNT;
                    }
                } else {
                    if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_CRC;
                    } else {
                        I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_STOP;
                    }
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                   I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_BYTE_COUNT:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //command has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT =1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.TxDataSize;
//                printf("WRITE_BYTE_COUNT(0x%02X) \r\n",I2C_SMB_BUFF.TxDataSize);
                if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                    I2C_SMB_BUFF.TxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.TxBuffer_CRC, I2C_SMB_BUFF.TxDataSize);
                }
                I2C_SMB_BUFF.Txindex = 0;
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_DATA;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_DATA:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //slave address or byte counts has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT =1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.TxBuffer[I2C_SMB_BUFF.Txindex];
//                printf("WRITE_DATA(0x%02X) \r\n",I2C_SMB_BUFF.TxBuffer[I2C_SMB_BUFF.Txindex]);
                if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                    I2C_SMB_BUFF.TxBuffer_CRC = I2C_crc8Calc(I2C_SMB_BUFF.TxBuffer_CRC, I2C_SMB_BUFF.TxBuffer[I2C_SMB_BUFF.Txindex]);
                }
                I2C_SMB_BUFF.Txindex ++;
                if(I2C_SMB_BUFF.Txindex != I2C_SMB_BUFF.TxDataSize) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_DATA;
                } else if(I2C_SMB_BUFF.WRITE_DATA_CRC) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_CRC;
                } else {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_STOP;
                }
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_CRC:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                //last data has been sent and received an acknowledge from slave device
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                I2C1_TRANSMIT_REG = I2C_SMB_BUFF.TxBuffer_CRC;
//                printf("WRITE_CRC(0x%02X) \r\n",I2C_SMB_BUFF.TxBuffer_CRC);
                I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_STOP;
                I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_STOP:
            if(I2C1_STATE_INT_FLAG & !I2C1_ACKNOWLEDGE_STATUS_BIT) {
                I2C1_STATE_INT_FLAG = 0;
                I2C1_ACKNOWLEDGE_STATUS_BIT = 1;
                
                //Enable STOP bit
                if(I2C_SMB_BUFF.RxDataSize) {
                    I2C1_STATE_INT_FLAG = 0x0;
                    I2C1_START_CONDITION_ENABLE_BIT = 0x1;              //send START
                    I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS;
                } else {
                    I2C1_STOP_CONDITION_ENABLE_BIT = 1;                    //Enable STOP bit
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_COMPLETE;
                }
            } else {
                if(SysTime_msecIntervalElapsed(I2C_SMB_BUFF.SMB_timer, I2C_MAX_COMM_TIME)) {
                    I2C_SMB_BUFF.SMB_state = SmbusState_CMD_WRITE_TIMEOUT;
                }
            }
            break;
            
        case SmbusState_CMD_WRITE_COMPLETE:
            I2C_SMB_BUFF.Error = I2C_MESSAGE_COMPLETE;
            break;
            
        case SmbusState_CMD_READ_TIMEOUT:
            //transmission time expired the ma. time
          
        case SmbusState_CMD_WRITE_TIMEOUT:
            //transmission time expired the ma. time
//            printf("i2c(0x%02X) -ERR(0x%02X) \r\n", I2C_SMB_BUFF.SMB_state);
            I2C1_START_CONDITION_ENABLE_BIT = 0x0;
            I2C1_REPEAT_START_CONDITION_ENABLE_BIT = 0x0;
            I2C1_STOP_CONDITION_ENABLE_BIT = 0x1; //frank 6/11
            I2C1_ACKNOWLEDGE_ENABLE_BIT = 0x0;
            I2C_SMB_BUFF.Error = I2C_TIME_OUT;
            I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            I2C1_Off();

            I2C_SMB_BUFF.SMB_state = SmbusState_BUS_WAIT_OFF;
            break;
            
                        
        case SmbusState_CMD_READ_ERROR:
        case SmbusState_CMD_WRITE_ERROR:
        case SmbusState_BUS_ERROR:
            //bus collision occurred
//            printf("i2c(0x%02X) -ERR(0x%02X) \r\n", I2C_SMB_BUFF.SMB_state);
            I2C1_START_CONDITION_ENABLE_BIT = 0x0;
            I2C1_REPEAT_START_CONDITION_ENABLE_BIT = 0x0;
            I2C1_RECEIVE_ENABLE_BIT  = 0x0;
            I2C1_STOP_CONDITION_ENABLE_BIT = 1;
            I2C1_ACKNOWLEDGE_ENABLE_BIT = 0x0;
            I2C_SMB_BUFF.SMB_timer = SysTime_getMsec();
            I2C1_Off();
            I2C_SMB_BUFF.SMB_state = SmbusState_BUS_WAIT_OFF;            
            break;
            
        case SmbusState_BUS_WAIT_OFF:
            I2C1_ReStart();
            I2C_SMB_BUFF.SMB_state = SmbusState_BUS_WAIT_RESTART;
            break;
            
        case SmbusState_BUS_WAIT_RESTART:            
        default:
            I2C_SMB_BUFF.SMB_state = SmbusState_IDLE;
            break;
    }
}

I2C_MESSAGE_STATUS I2C1_getError(void)
{
    return I2C_SMB_BUFF.Error;
}

void i2cstate_Get_RXBUFF(uint8_t* dataRxBuf, uint8_t startbyte)
{
    int i;
    
    for(i = 0; i < I2C_SMB_BUFF.RxDataSize; i++) {
        dataRxBuf[i] = I2C_SMB_BUFF.RxBuffer[i + startbyte];
    }
}

/**
 End of File
*/

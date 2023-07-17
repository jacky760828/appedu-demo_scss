/******************************************************************************
 *
 * FILE: can.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "xc.h"
#include "../main_app/hw_abstraction.h"
#include "../mcc_generated_files/can1.h"
#include "can.h"
#include "../sys_time/sys_time.h" //frank 4/16
#include "../EventLogData/EventLog.h"

#define RX_BUFFER_INDEX_MASK        63
#define TX_BUFFER_INDEX_MASK        63


CommCanBuf_t d_canMsg;
    
void ClearCAN_Buffer()
{
    d_canMsg.rx.headIdx = 0;
    d_canMsg.rx.tailIdx = 0;
    d_canMsg.rx.state = CircBufState_EMPTY;
    d_canMsg.tx.headIdx = 0;
    d_canMsg.tx.tailIdx = 0;
    d_canMsg.tx.state = CircBufState_EMPTY;    
}

/*******************************************************************************
* Function Name  : CAN_init
* Description    : CAN bus initialization
* Input          : None
* Return         : None
*******************************************************************************/
void CAN_init(void)
{
//    uint8_t tmpAddr;
    
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
    
    //Read device address from port setting
    
    
    C1CTRL1bits.REQOP  = CAN_CONFIGURATION_MODE;
    // Wait till desired mode is set.
    while( C1CTRL1bits.OPMODE != CAN_CONFIGURATION_MODE );
    
    C1CTRL1bits.WIN = 1;  
    C1RXM0SID = CAN_FILTERMASK2REG_SID(0x07FF); //mask 0
    C1RXM0SID = CAN_SETMIDE(C1RXM0SID);
 
    C1RXF0SID = CAN_FILTERMASK2REG_SID(0x180); // F0
    C1RXF0SID = CAN_FILTERSTD(C1RXF0SID);
    C1FMSKSEL1bits.F0MSK = 0;  // F0 select mask0
	C1BUFPNT1bits.F0BP = 0b0001; //buffer 1
	C1FEN1bits.FLTEN0 = 1;  //F0 enable
    C1RXF1SID = CAN_FILTERMASK2REG_SID(0x100); // F1
    C1RXF1SID = CAN_FILTERSTD(C1RXF1SID);
    C1FMSKSEL1bits.F1MSK = 0;  // F1 select mask0
	C1BUFPNT1bits.F1BP = 0b0010; //buffer 2
	C1FEN1bits.FLTEN1 = 1;  //F1 enable
    C1RXF2SID = CAN_FILTERMASK2REG_SID(0x080); // F2
    C1RXF2SID = CAN_FILTERSTD(C1RXF2SID);
    C1FMSKSEL1bits.F2MSK = 0;  // F2 select mask0
	C1BUFPNT1bits.F2BP = 0b0011; //buffer 3
	C1FEN1bits.FLTEN2 = 1;  //F2 enable
    C1RXF3SID = CAN_FILTERMASK2REG_SID(0x200); // F3
    C1RXF3SID = CAN_FILTERSTD(C1RXF3SID);
    C1FMSKSEL1bits.F3MSK = 0;  // F3 select mask0
	C1BUFPNT1bits.F3BP = 0b0100; //buffer 4
	C1FEN1bits.FLTEN3 = 1;  //F3 enable
    C1RXF4SID = CAN_FILTERMASK2REG_SID(0x300); // F4
    C1RXF4SID = CAN_FILTERSTD(C1RXF4SID);
    C1FMSKSEL1bits.F4MSK = 0;  // F4 select mask0
	C1BUFPNT2bits.F4BP = 0b0101; //buffer 5
	C1FEN1bits.FLTEN4 = 1;  //F4 enable
    C1RXF5SID = CAN_FILTERMASK2REG_SID(0x380); // F5
    C1RXF5SID = CAN_FILTERSTD(C1RXF5SID);
    C1FMSKSEL1bits.F5MSK = 0;  // F5 select mask0
	C1BUFPNT2bits.F5BP = 0b0110; //buffer 6
	C1FEN1bits.FLTEN5 = 1;  //F5 enable
    C1RXF6SID = CAN_FILTERMASK2REG_SID(0x400); // F6
    C1RXF6SID = CAN_FILTERSTD(C1RXF6SID);
    C1FMSKSEL1bits.F6MSK = 0;  // F6 select mask0
	C1BUFPNT2bits.F6BP = 0b0111; //buffer 7
	C1FEN1bits.FLTEN6 = 1;  //F6 enable
    C1RXF7SID = CAN_FILTERMASK2REG_SID(0x700); // F7
    C1RXF7SID = CAN_FILTERSTD(C1RXF7SID);
    C1FMSKSEL1bits.F7MSK = 0;  // F7 select mask0
	C1BUFPNT2bits.F7BP = 0b1000; //buffer 8
	C1FEN1bits.FLTEN7 = 1;  //F7 enable
    C1RXF8SID = CAN_FILTERMASK2REG_SID(0x780); // F8
    C1RXF8SID = CAN_FILTERSTD(C1RXF8SID);
    C1FMSKSEL2bits.F8MSK = 0;  // F8 select mask0
	C1BUFPNT3bits.F8BP = 0b1001; //buffer 9
	C1FEN1bits.FLTEN8 = 1;  //F8 enable
    
    
    C1RXM1SID = CAN_FILTERMASK2REG_SID(0x70F); //mask 1
    C1RXM1SID = CAN_SETMIDE(C1RXM1SID);
  
    C1RXF10SID = CAN_FILTERMASK2REG_SID(0x090); // F10
    C1RXF10SID = CAN_FILTERSTD(C1RXF10SID);
    C1FMSKSEL2bits.F10MSK = 1; // F10 select mask1
    C1BUFPNT3bits.F10BP = 0b1011;  //buffer 11
    C1FEN1bits.FLTEN10 = 1;  //F10 enable
    C1RXF11SID = CAN_FILTERMASK2REG_SID(0x0A0); // F11
    C1RXF11SID = CAN_FILTERSTD(C1RXF11SID);
    C1FMSKSEL2bits.F11MSK = 1; // F11 select mask1
    C1BUFPNT3bits.F11BP = 0b1100;  //buffer 12
    C1FEN1bits.FLTEN11 = 1;  //F11 enable
    C1RXF12SID = CAN_FILTERMASK2REG_SID(0x0B0); // F12
    C1RXF12SID = CAN_FILTERSTD(C1RXF12SID);
    C1FMSKSEL2bits.F12MSK = 1; // F12 select mask1
    C1BUFPNT4bits.F12BP = 0b1101;  //buffer 13
    C1FEN1bits.FLTEN12 = 1;  //F12 enable
    C1RXF13SID = CAN_FILTERMASK2REG_SID(0x0F1); // F13
    C1RXF13SID = CAN_FILTERSTD(C1RXF13SID);
    C1FMSKSEL2bits.F13MSK = 1; // F13 select mask1
    C1BUFPNT4bits.F13BP = 0b1110;  //buffer 14
    C1FEN1bits.FLTEN13 = 1;  //F13 enable
    C1RXF14SID = CAN_FILTERMASK2REG_SID(0x0F2); // F14
    C1RXF14SID = CAN_FILTERSTD(C1RXF14SID);
    C1FMSKSEL2bits.F14MSK = 1; // F14 select mask1
    C1BUFPNT4bits.F14BP = 0b1110;  //buffer 14
    C1FEN1bits.FLTEN14 = 1;  //F14 enable
    
    C1RXF15SID = CAN_FILTERMASK2REG_SID(0x190); // F15  , flash test
    C1RXF15SID = CAN_FILTERSTD(C1RXF15SID);
    C1FMSKSEL2bits.F15MSK = 1; // F15 select mask1
    C1BUFPNT4bits.F15BP = 0b1110;  //buffer 14
    C1FEN1bits.FLTEN15 = 1;  //F15 enable
    
    // others
    C1RXM2SID = CAN_FILTERMASK2REG_SID(0x07F0); //mask 2
    C1RXM2SID = CAN_SETMIDE(C1RXM2SID);
    
    C1RXF9SID = CAN_FILTERMASK2REG_SID(0x180); // F9
    C1RXF9SID = CAN_FILTERSTD(C1RXF9SID);
    C1FMSKSEL2bits.F9MSK = 2;  // F9 select mask2
	C1BUFPNT3bits.F9BP = 0b1010; //buffer 10
	C1FEN1bits.FLTEN9 = 1;  //F9 enable

    C1CTRL1bits.WIN = 0;
    //CanUtils_canSetOperationMode(CAN_NORMAL_OPERATION_MODE);
    C1CTRL1bits.REQOP  = CAN_NORMAL_OPERATION_MODE;
    // Wait till desired mode is set.
    while( C1CTRL1bits.OPMODE != CAN_NORMAL_OPERATION_MODE );
    
    ClearCAN_Buffer();
    HWA_CAN_BUS_DRIVER_NORMAL();
}

/*******************************************************************************
* Function Name  : CAN_getReceiveBufferState
* Description    : get rx buffer state
* Input          : None
* Return         : None
*******************************************************************************/
CircBufState_t CAN_getReceiveBufferState(void)
{
    return d_canMsg.rx.state;
}


/*******************************************************************************
* Function Name  : CAN_getTransmitBufferState
* Description    : get tx buffer state
* Input          : None
* Return         : None
*******************************************************************************/
CircBufState_t CAN_getTransmitBufferState(void)
{
    return d_canMsg.tx.state;
}


/*******************************************************************************
* Function Name  : interruptHandler_rxComplete
* Description    : CAN bus initialization
* Input          : None
* Return         : None
*******************************************************************************/
void CAN_interruptHandler_rxComplete(void) //uint8_t rxBufNum)
{
    uint8_t headIdx, tailIdx;
    CAN_MSG_OBJ msg;
    uCAN_MSG *pSrcBuf;
    int i;
    
    headIdx = d_canMsg.rx.headIdx;
    tailIdx = d_canMsg.rx.tailIdx; 
    pSrcBuf = &d_canMsg.rx.msgBuf[headIdx];
    
    if(d_canMsg.rx.state == CircBufState_FULL) {
        
    } else {
//        if(CAN1_Receive(&msg)) { //20230407  error
        if(CAN1_receive(pSrcBuf)) {
            //we got new message from buffer successfully
            //point to next avaliable space
            ian_canbus.cmdID = msg.msgId;
            ian_canbus.index = msg.field.dlc;
            for(i=0; i<msg.field.dlc; i++) {
                ian_canbus.data[i] = msg.data[i];
            }
            headIdx = (headIdx + 1) & RX_BUFFER_INDEX_MASK;
            d_canMsg.rx.headIdx = headIdx;
            if(headIdx == tailIdx) {
                //buffer is already full
                d_canMsg.rx.state = CircBufState_FULL;
            } else {
                d_canMsg.rx.state = CircBufState_PARTIAL;
            }
            EVENTLOG_CAN_Receive(&msg);
        }
    }
}

/*******************************************************************************
* Function Name  : CAN_getItemFromReceiveBuffer
* Description    : get receive item from the buffer
* Input          : data buffer for getting data
* Return         : None
*******************************************************************************/
void CAN_getItemFromReceiveBuffer(uCAN_MSG *pDestBuf)
{
    uint8_t headIdx, tailIdx;
    uint8_t byteCount, i;
    uCAN_MSG *pSrcBuf;
    
    headIdx = d_canMsg.rx.headIdx;
    tailIdx = d_canMsg.rx.tailIdx;
    pSrcBuf = &d_canMsg.rx.msgBuf[tailIdx];
//    pSrcBuf = &d_canMsg.rx.msgBuf[0];
    
    pDestBuf->frame.id = pSrcBuf->frame.id;
    pDestBuf->frame.dlc = pSrcBuf->frame.dlc;
    pDestBuf->frame.idType = pSrcBuf->frame.idType;
    byteCount = pSrcBuf->frame.dlc;
    for(i = 0; i < byteCount; i++) {
        pDestBuf->array[7 + i] = pSrcBuf->array[7 + i];
    }
    
    tailIdx = (tailIdx + 1) & RX_BUFFER_INDEX_MASK;
    if(tailIdx == headIdx) {
        //no more data in the buffer
        d_canMsg.rx.state = CircBufState_EMPTY;
    } else {
        d_canMsg.rx.state = CircBufState_PARTIAL;
    }
    
    //the following action should be put last line to avoid race condition occurring
    d_canMsg.rx.tailIdx = tailIdx;
    
//    pDestBuf->msgId = ian_canbus.cmdID;
//    pDestBuf->field.dlc = ian_canbus.index;
//    pDestBuf->field.idType = CAN_FRAME_STD;
//    for(i = 0; i < ian_canbus.index; i++)
//    {
//        pDestBuf->data[i] = ian_canbus.data[i];
//    }
}


/*******************************************************************************
* Function Name  : CAN_putItemIntoTransmitBuffer
* Description    : put transmit item into the buffer
* Input          : data buffer for transmitting data
* Return         : None
*******************************************************************************/
void CAN_putItemIntoTransmitBuffer(uCAN_MSG *pSrcBuf)
{
    uint8_t headIdx, tailIdx;
    uint8_t byteCount, i;
    uCAN_MSG *pDestBuf;
    
    headIdx = d_canMsg.tx.headIdx;
    tailIdx = d_canMsg.tx.tailIdx;
    pDestBuf = &d_canMsg.tx.msgBuf[headIdx];
    
//    ian_T_canbus[headIdx].cmdID = pSrcBuf->msgId;
//    ian_T_canbus[headIdx].index = pSrcBuf->field.dlc;
//    for(i=0; i<8; i++) {
//        ian_T_canbus[headIdx].data[i] = pSrcBuf->data[i];
//    }
    
//    pDestBuf->msgId = pSrcBuf->msgId;
//    pDestBuf->field.dlc = pSrcBuf->field.dlc;
//    byteCount = pSrcBuf->field.dlc;
//    pDestBuf->field.idType = CAN_FRAME_STD ;//STANDARD_CAN_MSG_ID_2_0B;
//    for(i = 0; i < byteCount; i++) {
//        pDestBuf->data[i] = pSrcBuf->data[i];
//    }
    pDestBuf->frame.id = pSrcBuf->frame.id;
    pDestBuf->frame.dlc = pSrcBuf->frame.dlc;
    byteCount = pSrcBuf->frame.dlc;
    pDestBuf->frame.idType = CAN_FRAME_STD ;//STANDARD_CAN_MSG_ID_2_0B;
    for(i = 0; i < byteCount; i++) {
        pDestBuf->array[7 + i] = pSrcBuf->array[7 + i];
    }
    
    headIdx = (headIdx + 1) & TX_BUFFER_INDEX_MASK;
    d_canMsg.tx.headIdx = headIdx;
    if(headIdx == tailIdx) {
        //no more space for new data
        d_canMsg.tx.state = CircBufState_FULL;
    } else {
        d_canMsg.tx.state = CircBufState_PARTIAL;
    }
}


/*******************************************************************************
* Function Name  : tx_mainFunction
* Description    : can transmit main function
* Input          : None
* Return         : None
*******************************************************************************/
void tx_mainFunction(void)
{
//    uint8_t tempData;
    uint8_t headIdx, tailIdx;
    uCAN_MSG *pSrcBuf;
    uint8_t msgObjData[8] = {0};
    CAN_MSG_OBJ msg;
    int ret;
    
    if(!poweron_while_time.over) {
        return;
    }
    
    headIdx = d_canMsg.tx.headIdx;
    tailIdx = d_canMsg.tx.tailIdx;
    pSrcBuf = &d_canMsg.tx.msgBuf[tailIdx];
//    pSrcBuf->data = msgObjData;
    
//    msg.msgId = d_canMsg.tx.msgBuf[tailIdx].msgId;
//    msg.field = d_canMsg.tx.msgBuf[tailIdx].field;
    
    if(d_canMsg.tx.state != CircBufState_EMPTY) {
        //new transmit data coming now
//        msg.data = ian_T_canbus[0].data;
        if((pSrcBuf->frame.id & 0xFF0) == 0x680) {
//            EVENTLOG_CAN_Transmit(&msg);
            EVENTLOG_CAN_Transmit(pSrcBuf);
        }
//        ret = CAN1_Transmit(CAN_PRIORITY_HIGH,&msg);
        ret = CAN1_transmit(CAN_PRIORITY_HIGH,pSrcBuf);
        if(ret==0) {
            //data is send to can bus buffer successfully
            tailIdx = (tailIdx + 1) & TX_BUFFER_INDEX_MASK;
            d_canMsg.tx.tailIdx = tailIdx;
            if(tailIdx == headIdx) {
                //no more data in transmit buffer
                d_canMsg.tx.state = CircBufState_EMPTY;
            } else {
                d_canMsg.tx.state = CircBufState_PARTIAL;
            }
        }
    }
}





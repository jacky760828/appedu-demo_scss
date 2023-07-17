 /******************************************************************************
 *
 * FILE: can_dbc.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "can_dbc.h"

#include "../mcc_generated_files/can1.h"
#include "../mcc_generated_files/watchdog.h"
#include "../sys_time/sys_time.h"

#define DBC_CMD_SYSREQ              0x080//0x01            //0x080
#define DBC_CMD_AUXREQ              0x090 //                 0x090
#define DBC_CMD_FW0SEND             0x0A0                  // used to send BMC's FW
#define DBC_CMD_FW1SEND             0x0A1                  // used to send XRCC's FW
#define DBC_CMD_FWINSTALL           0x0B0 
#define DBC_CMD_FTCMD               0x0F0
#define DBC_CMD_CFGCMD              0x0F1 //    tsai 11/16
#define DBC_CMD_EVENTLOG            0x0F2
#define DBC_CMD_VER                 0x100//0x02            //0x100
#define DBC_CMD_CHRG_CTRL           0x180//0x03            //0x180
#define DBC_CMD_FLASH_TEST          0x190//test flash
#define DBC_CMD_MDAT                0x200//0x04            //0x200
#define DBC_CMD_State_TEST          0x290//test state diagram
#define DBC_CMD_CAT0                0x300//0x06            //0x300
#define DBC_CMD_CAT1                0x380//0x07            //0x380
#define DBC_CMD_CAT2                0x400//0x08            //0x400
#define DBC_CMD_CHRG_STATUS         0x480//0x09            //0x480, CAT3 is replaced by charger status
#define DBC_CMD_LDAT                0x700//0x0E            //0x700
#define DBC_CMD_VDAT                0x780//0x0F            //0x780
//#define DBC_CMD_CAT3                0x09          //0x480
#define DBC_CMD_CAT4                0x500//0x0A            //0x500
#define DBC_CMD_CAT5                0x580//0x0B            //0x580
#define DBC_CMD_CAT6                0x600//0x0C            //0x600
#define DBC_CMD_CAT7                0x680//0x0D            //0x680

// frank 5/6
#define PENDING_BM_FW   14
#define TO_INS_BM_FW    16
// frank 7/14

#define DCB_TX_BUFFER_MAX           40

#ifdef EVENT_LOG
#define DBC_CMD_EVENTRESP           0X600
#define ERASE_ALL					0XFFFFFFFF	//.For Erase All Data
#endif
#define LOST_COMM_DETECT_TIME       13       // Unit: mSec     000 by hugo
#define swReset() __asm__ volatile ("reset")  // frank software reset 8/12

uint8_t swResetByte __attribute__ ((persistent)) __attribute__ ((address(0x2000)));

CanDbc_RX_t ian_canbus;
Can_TX_t ian_T_canbus[64];
UINT16 d_usageTabIdx;
CanDbc_TX_t d_dbcTxBuf[DCB_TX_BUFFER_MAX];
UINT8 d_ptrHeadTxBuf, d_ptrTailTxBuf;
CircBufState_t d_txCmdBufState;
Comm_CanDbc_TX_t d_txState;
bool d_canConnected;
volatile UINT32 d_timerCanConnected;
UINT8 SerialNum[18];
#ifdef EVENT_LOG
uCAN_MSG canEventBuf;
UINT8 CAN_ResponseCntFlag = 0;
UINT8 EventReadCnt = 0;
UINT8 SaveFlashCmd = 0;
CAN_RESPONSE EventCAN_Rx;
uint8_t NoIntoEventRecentFlag = 0;
UINT8 EventLogDataNum = 0;
UINT8 En_RecentFalg;
UINT8 LastBufData[32];
UINT32 EventLogContinuID = 0;
uint32_t EraseData = 0;
UINT32 RecentSecondCnt = 0;
UINT32 ReadLatestAddress = 0;
UINT32 EvnetSentDataCnt = 0;
UINT32 CAN_Rx_EventTotalCnts = 0;
UINT32 CAN_EventCnt = 0;
UINT8 twiceFlag = 0;
UINT8 ContinueFalg = 0;
UINT32 TxCAN_15msOnceCnt = 0;
UINT16 CanResponse30sCnt = 0;
bool SaveFlashFlag = false;
bool UPS_CTRL_FLAG = false;
#endif

bool CAN_DBC_Get_UPS_CTRL(void)
{
    return UPS_CTRL_FLAG;
}

void CAN_DBC_Set_Saveflash(bool onoff)
{
    SaveFlashFlag = onoff;
}

/*******************************************************************************
* Function Name  : CanDbc_init
* Description    : initial
* Input          : None
* Return         : None
*******************************************************************************/
void CanDbc_init(void)
{
    UINT8 i, j;
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
    
    d_usageTabIdx = 0;
    
    //clear all memory map
    for (i = 0; i < CAN_FRAME_MAX; i++) {
        for (j = 0; j < 8; j++) {
            CanDCB_dataFramePtr[i].ptrFrame[j] = 0x00;
        }
    }
    EventLog_Set_CanRxCmd(0);
    d_txCmdBufState = CircBufState_EMPTY;
    d_ptrHeadTxBuf = 0;
    d_ptrTailTxBuf = 0;
    
    d_txState = Comm_CanDcb_TX_IDLE;
    pSvdCmd->lostCommunication = false;
    UPS_CTRL_FLAG = false;
    BMS_Set_firstCanCommect(false);
    d_timerCanConnected = 0;
    TxCAN_15msOnceCnt = SysTime_getMsec(); 
    //BMFTPinit();
    IntoFET.time = IntoFET_300ms;
    IntoFET.cut = 0;
    IntoFET.Time_format = 0;
    IntoFET.over = 0;
    IntoFET.start = 0;
}

/*******************************************************************************
* Function Name  : updateMemoryMap
* Description    : Update memory map
* Input          : None
* Return         : None
*******************************************************************************/
void updateMemoryMap(void)
{
    CanDbcMemMapUsageFuncParam_t usageParam;
    CanDbcMemMapUsageInfo_t *pUIT = &BatteryManageSystem_usageInfoTable[d_usageTabIdx];
    
    if(pUIT->elementIdx < 65535) {
        //valid define item in the table
        usageParam.dataType = pUIT->dataType;
        usageParam.frameIdx = pUIT->frameIdx;
        usageParam.maxValue = pUIT->maxValue;
        usageParam.minValue = pUIT->minValue;
        usageParam.offsetValue = pUIT->offsetValue;
        usageParam.startByte = pUIT->startByte;
        usageParam.endByte = pUIT->endByte;
        usageParam.startBit = pUIT->startBit;
        usageParam.size = pUIT->size;
        usageParam.flags.isWriting = TRUE;
        
        pUIT->usageFunc(&usageParam);
        d_usageTabIdx ++;
    } else {
        //we got the last line and invalid data
        // reset the index to first data
        d_usageTabIdx = 0;
    }
}

/*******************************************************************************
* Function Name  : putRequestCmdIntoBuf
* Description    : Put request command into buffer
* Input          : None
* Return         : None
*******************************************************************************/
void putRequestCmdIntoBuf(uCAN_MSG *tempBuf, CanDbc_TX_t *tempSrcBuf)
{
    CanDbc_TX_t *pDestTxBuf = &d_dbcTxBuf[d_ptrHeadTxBuf];
    
    pDestTxBuf->cmdID = tempSrcBuf->cmdID;
    pDestTxBuf->frameIdx = tempSrcBuf->frameIdx;
    pDestTxBuf->stopFrame = tempSrcBuf->stopFrame;
    pDestTxBuf->requestID = (tempBuf->frame.data0 & 0xF0);
    pDestTxBuf->requestType = tempSrcBuf->requestType;
    pDestTxBuf->muxNum = 0;
    pDestTxBuf->txState = Comm_CanDcb_TX_IDLE;
    
    d_ptrHeadTxBuf = (d_ptrHeadTxBuf + 1 );
    if(d_ptrHeadTxBuf >= DCB_TX_BUFFER_MAX) {
        d_ptrHeadTxBuf = 0;
    }
    
    if(d_ptrHeadTxBuf == d_ptrTailTxBuf) {
        d_txCmdBufState =  CircBufState_FULL;
    } else {
        d_txCmdBufState =  CircBufState_PARTIAL;
    }
}

void resetCanConnect(void)
{
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
    
    BMS_Set_firstCanCommect(true);
    pSvdCmd->lostCommunication = false;
    d_timerCanConnected = SysTime_getSec(); //SysTime_getMsec() SysTime_getSec()
}

bool CheckCanRxData(void)
{
    if (CAN_getReceiveBufferState() != CircBufState_EMPTY) {
        resetCanConnect();
        //if (CAnRx_state==CircBufState_PARTIAL)
        return true;
    }
    return false;
}

void RxCAN_Msg(UINT32 CAN_ID, uCAN_MSG tempBuf, CanDbc_TX_t tempTxRqstBuf)
{
    UINT8 tempData;
    
    switch(CAN_ID) {
        case DBC_CMD_MDAT:
            tempTxRqstBuf.cmdID = CAN_ID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_MDAT].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_MDAT].startFrameNum + DCB_UIT[DCB_CMD_NAME_MDAT].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            break;

        case DBC_CMD_CAT0:
            tempTxRqstBuf.cmdID = CAN_ID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_CAT0].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_CAT0].startFrameNum + DCB_UIT[DCB_CMD_NAME_CAT0].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            break;

        case DBC_CMD_CAT1:
            tempTxRqstBuf.cmdID = CAN_ID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_CAT1].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_CAT1].startFrameNum + DCB_UIT[DCB_CMD_NAME_CAT1].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            break;

        case DBC_CMD_CAT2:
            tempTxRqstBuf.cmdID = CAN_ID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_CAT2].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_CAT2].startFrameNum + DCB_UIT[DCB_CMD_NAME_CAT2].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            break;

        case DBC_CMD_LDAT:
            tempTxRqstBuf.cmdID = CAN_ID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_LDAT].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_LDAT].startFrameNum + DCB_UIT[DCB_CMD_NAME_LDAT].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            break;

        case DBC_CMD_VDAT:
            if ( tempBuf.frame.dlc ==8 && tempBuf.frame.data0 == 0xCC) { // tsai 5/31 Lable serialNumber from CAN
                for (tempData = tempBuf.frame.data1; tempData < (tempBuf.frame.data1 + 6); tempData++) {
                    if (tempData < 18) {
                        SerialNum[tempData]=tempBuf.array[7 + 2 + tempData - tempBuf.frame.data1];
                    }
                }
                if (tempData == 18) {//end
                    sFLASH_EraseSector(0x030000);
                    sFLASH_WriteBuffer((uint8_t *)SerialNum, 0x030000, 18);
                    
//                    MX25L_Flash_Write_Enable();
//                    MX25L_FLASH_Erase(0x030000,0x20);
//                    while(MX25L_FLASH_Status()&0x01);
//                    MX25L_Flash_Write(SerialNum,0x030000,18);
//                    MX25L_Flash_Write_Disable();
                    WATCHDOG_TimerClear();
                    SerialNum[17]=0;
                }
            } else {
                tempTxRqstBuf.cmdID = CAN_ID;
                tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_VDAT].startFrameNum;
                tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_VDAT].startFrameNum + DCB_UIT[DCB_CMD_NAME_VDAT].totalFrameNum;
                tempTxRqstBuf.requestType = 0;
                putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            }
            break;
            
        case DBC_CMD_VER:
                tempTxRqstBuf.cmdID = CAN_ID;
                tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_VER].startFrameNum;
                tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_VER].startFrameNum + DCB_UIT[DCB_CMD_NAME_VER].totalFrameNum;
                tempTxRqstBuf.requestType = 0;
                putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);            
            break;
    }
}

void CAN_Response_Listdata()
{ 
    twiceFlag = 0;
    
    if(CanResponse30sCnt >= 60) {
        CAN_ResponseCntFlag = 0;
        CanResponse30sCnt = 0;
        ContinueFalg = 0;
        CAN_Rx_EventTotalCnts = CntEventLength;
    } if(CAN_Rx_EventTotalCnts > 0) {
        CAN_ResponseCntFlag = 0;
        CAN_Rx_EventTotalCnts = CAN_Rx_EventTotalCnts-1;       
        CAN_EventCnt = CAN_Rx_EventTotalCnts;
        ReadLatestAddress = EVENT_MSG_START_ADR + (CAN_Rx_EventTotalCnts<<5);    
    } else {
        CAN_ResponseCntFlag = 1;
        CAN_Rx_EventTotalCnts = 0;
        CAN_EventCnt = 0;
        EventLog_Set_CanRxCmd(0);
        SaveFlashCmd = 0;     
        ReadLatestAddress = EVENT_MSG_START_ADR;
    }
    EventLogContinuID = 0;
    NoIntoEventRecentFlag = 0;    
}

/*******************************************************************************
* Function Name  : dbcRx_mainFunction
* Description    : DBC receive main function
* Input          : None
* Return         : None
*******************************************************************************/
void dbcRx_mainFunction(void)
{
    uCAN_MSG tempBuf;
    uint32_t tempID;
    uint8_t assignedAddr;
    //uCAN_MSG pSrcBuf;
    CanDbc_TX_t tempTxRqstBuf;
    
    EventLog_Set_CANLoss(0);
    if (!CheckCanRxData()) { //frank 9/8
        return;
    }
    CAN_getItemFromReceiveBuffer(&tempBuf);
    
//    printf("_ID:0x%03lX", ian_canbus.cmdID);
//    printf(" DLC:%d DATA:", ian_canbus.index);
//    for(i=0; i<ian_canbus.index; i++) {
//        printf("0x%02X ", ian_canbus.data[i]);
//    }
//    printf("\n\r");
    
//    tempID = tempBuf.msgId;
//    tempID = ian_canbus.cmdID;
	tempID = tempBuf.frame.id;
    printf("dbcRx_mainFunction(0x%03lX) - ", tempID);
    switch(tempID) {
        case DBC_CMD_SYSREQ:
			if(0X01 == (tempBuf.frame.data1 & 0X01)) {
                UPS_CTRL_FLAG = true;
                printf("CTRL ");
            } else {
                UPS_CTRL_FLAG = false;
                printf("READ ");
			}
            switch(tempBuf.frame.data3 & 0x07) {
                case 0:
                    RxCAN_Msg(DBC_CMD_MDAT, tempBuf, tempTxRqstBuf);
                    break;
                    
                case 1:
                    RxCAN_Msg(DBC_CMD_MDAT, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT0, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT1, tempBuf, tempTxRqstBuf);
                break;
                
                case 2:
                    RxCAN_Msg(DBC_CMD_MDAT, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT0, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT1, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT2, tempBuf, tempTxRqstBuf);
                    break;
                    
                case 3:
                    RxCAN_Msg(DBC_CMD_MDAT, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_LDAT, tempBuf, tempTxRqstBuf);
                    break;
                
                case 4:
                    RxCAN_Msg(DBC_CMD_CAT0, tempBuf, tempTxRqstBuf);
                    RxCAN_Msg(DBC_CMD_CAT1, tempBuf, tempTxRqstBuf);
                    break;
                    
                case 5:
                    RxCAN_Msg(DBC_CMD_CAT2, tempBuf, tempTxRqstBuf);
                    break;
                
                case 6:
                    RxCAN_Msg(DBC_CMD_VER, tempBuf, tempTxRqstBuf);
                    break;

                case 7:
                    RxCAN_Msg(DBC_CMD_VDAT, tempBuf, tempTxRqstBuf);
                    break;
                    
                default:
                    break;
            }
            printf("\n\r");
//            EVENTLOG_BATTERY_MANAGE();
            break;
        case DBC_CMD_VER:
        case DBC_CMD_MDAT:  
        case DBC_CMD_CAT0:
        case DBC_CMD_CAT1: 
        case DBC_CMD_CAT2:  
        case DBC_CMD_LDAT: 
        case DBC_CMD_VDAT:
            RxCAN_Msg(tempID, tempBuf, tempTxRqstBuf);
            printf("TEST \n\r");
            break;
            
        case DBC_CMD_CHRG_CTRL:
            if(UPS_CTRL_FLAG) {
                if(((tempBuf.frame.id & 0x7F) != 0x00) && 
                   ((tempBuf.frame.id & 0x7F) != d_deviceAddr)) {
                    //this command is not from master
                    printf("d_deviceAddr ERR \n\r");
                } else {
                    EventLog_Set_CANRxEvent(1);
                    assignedAddr = tempBuf.frame.data1 & 0xFF;
                    if((assignedAddr == d_deviceAddr) || (assignedAddr == 0xFF)) {
                        if(0x10 == (tempBuf.frame.data2 & 0xF0)) {
                            switch (tempBuf.frame.data3) {
                                case 0x00:
                                    break;

                                case 0x01:
                                    // Enable Charge
                                    printf("Enable Charge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(ON,KEEP,DISCHRG);
                                    BMS_Set_Statu_CFET(1);
                                    BMS_Set_Statu_TRICKLE(0);
                                    break;

                                case 0x02:
                                    // Disable Charge
                                    printf("Disable Charge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                                    BMS_Set_Statu_CFET(0);
                                    break;

                                case 0x03:
                                    // Enable Charge of mission node but disable all others
                                    printf("Enable Charge of mission node but disable all others \n\r");
                                    if (assignedAddr == d_deviceAddr) {
                                        Mcu_set_CFET_DFET_TRICKLE(ON,KEEP,HOLDING);
                                        BMS_Set_Statu_CFET(1);
                                    } else {
                                        Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                                        BMS_Set_Statu_CFET(0);
                                    }
                                    break;

                                case 0x04:
                                    // Enable Discharge
                                    printf("Enable Discharge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(KEEP,ON,HOLDING);
                                    BMS_Set_Statu_DFET(1);
                                    break;

                                case 0x05:
                                    // Disable Discharge
                                    printf("Disable Discharge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                                    BMS_Set_Statu_DFET(0);
                                    break;

                                case 0x06:
                                    // Enable Discharge of mission node but disable all others
                                    printf("Enable Discharge of mission node but disable all others \n\r");
                                    if (assignedAddr == d_deviceAddr) {
                                        Mcu_set_CFET_DFET_TRICKLE(KEEP,ON,HOLDING);
                                        BMS_Set_Statu_DFET(1);
                                    } else {
                                        Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                                        BMS_Set_Statu_DFET(0);
                                    }
                                    break;

                                case 0x07:
                                    // Enable Trickle Charge
                                    printf("Enable Trickle Charge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,ENCHRG);
                                    BMS_Set_Statu_CFET(0);
                                    BMS_Set_Statu_TRICKLE(1);
                                    break;

                                case 0x08:
                                    // Disable Trickle Charge
                                    printf("Disable Trickle Charge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(KEEP,KEEP,DISCHRG);
                                    BMS_Set_Statu_TRICKLE(0);
                                    break;

                                case 0x09:
                                    // Disable Charge and Trickle Charge
                                    printf("Disable Charge and Trickle Charge \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,DISCHRG);
                                    BMS_Set_Statu_CFET(0);
                                    BMS_Set_Statu_TRICKLE(0);
                                    break;

                                case 0x10:
                                    printf("IntoFET_300msFlag \n\r");
                                    Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,DISCHRG);
                                    BMS_Set_Statu_CFET(0);
                                    BMS_Set_Statu_TRICKLE(0);
                                    IntoFET.cut = SysTime_getMsec();
                                    IntoFET.start = 1;
                                    break;

                                case 0xA5:
                                    //reset to bootloader  , frank 8/12
                                    printf("reset to bootloader \n\r");
                                    swResetByte=tempBuf.frame.data4;//0xA5;//0xBB;//test 0xA5;
                                    swReset();
                                    break;

                                case 0xAA: //shutdown system  , frank 6/11
                                    printf("shutdown system \n\r");
                                    SMBUS_setShutDownEnable(true);
                                    break;

                                case 0xBB:
                                    printf("CanDbc_setBQ78350Cmd(0x%02X) \n\r", tempBuf.frame.data4);
                                    SMBUS_BQ78350_SBS_Cmd(tempBuf.frame.data4); // tsai 9/1
                                    break;

                                case 0xCC: //can idle on/off 9/1
                                    printf("can idle \n\r");
                                    break;

                                default:
                                    printf("tempBuf.frame.data3(0x%02X) \n\r", tempBuf.frame.data3);
                                    break;
                            }
                            BMS_Get_Statu();
                            EVENTLOG_BATTERY_STATE_CHANG();
                        } else {
                            //receive an inquiry command
                            tempTxRqstBuf.cmdID = DBC_CMD_CHRG_STATUS;
                            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_CHRST].startFrameNum;
                            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_CHRST].startFrameNum + DCB_UIT[DCB_CMD_NAME_CHRST].totalFrameNum;
                            tempTxRqstBuf.requestType = 0;
                            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
                            printf("tempBuf.frame.data2 \n\r");
                        }
                    } else {
                        printf("d_deviceAddr NG \n\r");
                    }
                }
            } else {
                printf("UPS_CTRL_FLAG \n\r");
            }
            break;
            
        case DBC_CMD_CHRG_STATUS:
            tempTxRqstBuf.cmdID = tempID;
            tempTxRqstBuf.frameIdx = DCB_UIT[DCB_CMD_NAME_CHRST].startFrameNum;
            tempTxRqstBuf.stopFrame = DCB_UIT[DCB_CMD_NAME_CHRST].startFrameNum + DCB_UIT[DCB_CMD_NAME_CHRST].totalFrameNum;
            tempTxRqstBuf.requestType = 0;
            putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
            printf("DBC_CMD_CHRG_STATUS \n\r");
            break;
        
        case DBC_CMD_AUXREQ: //frank 8/20  0x090  receive authenticate request
            ParserAuxReqCmd(&tempBuf);
            printf("DBC_CMD_AUXREQ \n\r");
            break;
        
        case DBC_CMD_FW0SEND:
            BMFTPReqCmd(&tempBuf);
            printf("DBC_CMD_FW0SEND \n\r");
            break;
            
        //case DBC_CMD_FTCMD:
        case DBC_CMD_CFGCMD:    // tsai 11/16
            if((tempBuf.frame.data2 == d_deviceAddr) || (tempBuf.frame.data2 == 0xFF)) {
                if ((tempBuf.frame.data1 == PENDING_BM_FW) || (tempBuf.frame.data1 == TO_INS_BM_FW) ) {
                    DBCFLAGS.forceUpdate = tempBuf.frame.data6;// trick
                }
                if (tempBuf.frame.data1 == TO_INS_BM_FW ) {
                    DBCFLAGS.forceUpdate = 0;
                    tempBuf.frame.data1 = 0x01;
                    tempBuf.frame.data2 = 0x03;//C-tech
                    if (BMFTPReqInsall(&tempBuf.array[7 + 1])) { //update if same version as version in flash
                        DBCFLAGS.ImageOK = 1;
                        swResetByte = 0xA5;
                        swReset();
                    }
                }
            }
            printf("DBC_CMD_CFGCMD \n\r");
            break;
        
        case DBC_CMD_FWINSTALL:
            if((tempBuf.frame.data0 == d_deviceAddr) || (tempBuf.frame.data0 == 0xFF)) {  // tsai 6/1
                if (DBCFLAGS.ImageOK == 1 ) {
                    if (tempBuf.frame.dlc == 1 ) {//old method
                        swResetByte = 0xA5;
                        swReset();
                    } else if (tempBuf.frame.dlc == 5) { //tsai 12/3
                        DBCFLAGS.forceUpdate = 0;
                    //tempBuf.frame.data0=0x01;//version
                        if (BMFTPReqInsall(&tempBuf.array[7])) { //update if same version as version in flash
                            swResetByte = 0xA5;
                            swReset();
                        }
                    }
                }
            }
            printf("DBC_CMD_FWINSTALL \n\r");
            break;
        
        case DBC_CMD_EVENTLOG:
            EventLog_Set_CanRxCmd(1);
            SaveFlashCmd = 1;
            ContinueFalg = 1;
            canEventBuf.frame.id = DBC_CMD_EVENTRESP;
            canEventBuf.frame.id |= d_deviceAddr;
            canEventBuf.frame.idType = CAN_FRAME_STD;
            canEventBuf.frame.dlc = 8;
            canEventBuf.frame.data0 = tempBuf.frame.data0;
            canEventBuf.frame.data1 = tempBuf.frame.data1;
            canEventBuf.frame.data2 = tempBuf.frame.data2;
            canEventBuf.frame.data3 = tempBuf.frame.data3;
            canEventBuf.frame.data4 = tempBuf.frame.data4;
            canEventBuf.frame.data5 = tempBuf.frame.data5;
            canEventBuf.frame.data6 = tempBuf.frame.data6;
            canEventBuf.frame.data7 = tempBuf.frame.data7;
            
            switch(tempBuf.frame.data1) {
                case 0x01:
                    EventReadCnt = 1;
                    EvnetSentDataCnt = 1;
                    CAN_Response_Listdata();
                    break;
                    
                case 0x02:
                    EventReadCnt = 10;
                    EvnetSentDataCnt = 10;
                    CAN_Response_Listdata();
                    break;
                    
                case 0x04:
                    EventReadCnt = 100;
                    EvnetSentDataCnt = 100;
                    CAN_Response_Listdata();
                    break;
                    
                case 0x08:
                    EventCAN_Rx.flag.rewind = 1;
                    EventCAN_Rx.Status &= 0X0400;
                    break;
                    
                case 0x10:
                    EventCAN_Rx.flag.recent = 1;
                    EventCAN_Rx.Status &= 0X0800;
                    EventLogContinuID = 0;
                    RecentSecondCnt = tempBuf.frame.data5;
                    RecentSecondCnt = (RecentSecondCnt<<8) | tempBuf.frame.data4;
                    RecentSecondCnt = (RecentSecondCnt<<16) | tempBuf.frame.data3;
                    RecentSecondCnt = (RecentSecondCnt<<24) | tempBuf.frame.data2;
                    break;
                    
                case 0x40:
                    EventReadCnt = CntEventLength;
                    EventCAN_Rx.flag.all = 1;
                    EventCAN_Rx.Status &= 0X1000;
                    EventLogContinuID = 0;
                    RecentSecondCnt = ian_canbus.data[5];
                    RecentSecondCnt = (RecentSecondCnt<<8) | tempBuf.frame.data4;
                    RecentSecondCnt = (RecentSecondCnt<<16) | tempBuf.frame.data3;
                    RecentSecondCnt = (RecentSecondCnt<<24) | tempBuf.frame.data2;                
                    break;
                    
                case 0x80:
                    EventCAN_Rx.flag.abort = 1;
                    EventCAN_Rx.Status &= 0X2000;
                    EventLog_Set_CanRxCmd(0);
                    SaveFlashFlag = false;
                    EventCAN_Rx.Status = 0;
                    EvnetSentDataCnt = 0;            
                    EventReadCnt = 0;
                    break;
                    
                case 0xFE:
                    EventCAN_Rx.flag.erase = 1;
                    EventCAN_Rx.Status &= 0X4000;

//                    shif_data = canEventBuf.frame.data5;			// MSB
//                    EraseData = shif_data << 24;
//                    shif_data = canEventBuf.frame.data4;			// MSB
//                    EraseData |= shif_data << 16;
//                    shif_data = canEventBuf.frame.data3;			// LSB
//                    EraseData |= shif_data << 8;
//                    EraseData |= canEventBuf.frame.data2;			// LSB				
                    EraseData = ((uint32_t)canEventBuf.frame.data5 << 24 |
                                (uint32_t)canEventBuf.frame.data4 << 16 |
                                (uint32_t)canEventBuf.frame.data3 << 8 |
                                (uint32_t)canEventBuf.frame.data2);
                    break;
                    
                default:
                    EventCAN_Rx.Status = 0;
                    break;
            }
            printf("DBC_CMD_EVENTLOG \n\r");
            break;
        
        default:
            printf("ID_ERROR \n\r");
            break;
    }
}


/*******************************************************************************
* Function Name  : dbcCmdDecode
* Description    : Decode for DBC command
* Input          : None
* Return         : None
*******************************************************************************/
void dbcCmdDecode(CanDbc_TX_t *pSrcTxBuf, uCAN_MSG *pMsgBuf, uint8_t muxIdx)
{
    uint8_t i, totalByteNum;
    uint8_t *pFrameNum = CanDCB_dataFramePtr[pSrcTxBuf->frameIdx].ptrFrame;
    
    pMsgBuf->frame.id = (pSrcTxBuf->cmdID + d_deviceAddr);
    pMsgBuf->frame.idType = CAN_FRAME_STD;
    pMsgBuf->frame.dlc = 8;
    switch((uint16_t)(pSrcTxBuf->cmdID)) { 
        case DBC_CMD_VER:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            if(muxIdx == 11) { // tsai 6/1
                pMsgBuf->frame.data0 += 2;
            }
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_MDAT:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_CAT0:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_CAT1:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F); // 9/28 tsai
            if(muxIdx == 4) { // tsai 6/1
               pMsgBuf->frame.data0 += 1;
            }
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_CAT2:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;

        case DBC_CMD_LDAT:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_VDAT:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_CHRG_CTRL:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        case DBC_CMD_CHRG_STATUS:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;

         case DBC_CMD_FLASH_TEST:
            pMsgBuf->frame.data0 = pSrcTxBuf->requestID + (muxIdx & 0x0F);
            pFrameNum += 1;
            totalByteNum = 7;
            break;
            
        default:
            totalByteNum = 0;
            break;
    }
    
    for(i = (8 - totalByteNum); i < 8; i++) {
        pMsgBuf->array[i + 7] = *(pFrameNum);
        pFrameNum ++;
    }
}
    

/*******************************************************************************
* Function Name  : dbcTx_mainFunction
* Description    : DBC transmit main function
* Input          : None
* Return         : None
*******************************************************************************/
void dbcTx_mainFunction(void)
{
    static UINT8 muxIdx;
    CanDbc_TX_t *pSrcTxBuf = &d_dbcTxBuf[d_ptrTailTxBuf];
    uCAN_MSG msgBuf;
    
    switch(d_txState) {
        case Comm_CanDcb_TX_IDLE:
            if(d_txCmdBufState != CircBufState_EMPTY) {
                muxIdx = 1;
                d_txState =  Comm_CanDcb_TX_WAIT_BUFFER_EMPTY;
            }
            break;
            
        case Comm_CanDcb_TX_WAIT_BUFFER_EMPTY:
            if(CAN_getTransmitBufferState() != CircBufState_FULL) {
                d_txState = Comm_CanDcb_TX_PUT_DATA_INTO_BUFFER;
            }
            break;
            
        case Comm_CanDcb_TX_PUT_DATA_INTO_BUFFER:   
            if(CAN_getTransmitBufferState() != CircBufState_FULL) {
                //msgBuf.msgId |= d_deviceAddr;
                //msgBuf.field.idType = CAN_FRAME_STD;
                //msgBuf.field.dlc = 8;
                dbcCmdDecode(pSrcTxBuf, &msgBuf, muxIdx);
                CAN_putItemIntoTransmitBuffer(&msgBuf);
                muxIdx ++;
                pSrcTxBuf->frameIdx += 1;
                if(pSrcTxBuf->frameIdx >= pSrcTxBuf->stopFrame) {
                    d_txState = Comm_CanDcb_TX_OMPLETE;
                }
            }
            break;
            
        case Comm_CanDcb_TX_OMPLETE:
            d_ptrTailTxBuf = (d_ptrTailTxBuf + 1);
            if(d_ptrTailTxBuf >= DCB_TX_BUFFER_MAX) {
                d_ptrTailTxBuf = 0;
            }
            
            if(d_ptrTailTxBuf == d_ptrHeadTxBuf) {
                d_txCmdBufState =  CircBufState_EMPTY;
            } else {
                d_txCmdBufState =  CircBufState_PARTIAL;
            }
            d_txState = Comm_CanDcb_TX_IDLE;
            break;
            
        default:
            d_txState = Comm_CanDcb_TX_IDLE;
            break;
    }
}

/*******************************************************************************
* Function Name  : updateConnectionStatus
* Description    : check the connection status
* Input          : None
* Return         : None
*******************************************************************************/
void updateConnectionStatus(void)
{
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
    
    if (C1INTF & 0x3F00) {  //CAN error frank 9/4
        C1INTF &= ~0x00EC;
        resetCanConnect();
        UPS_CTRL_FLAG = false;
    } else {
        if(!pSvdCmd->lostCommunication) {
            if(SysTime_secIntervalElapsed(d_timerCanConnected, LOST_COMM_DETECT_TIME)) {   //SysTime_msecIntervalElapsed                    
                pSvdCmd->lostCommunication = true;
                UPS_CTRL_FLAG = false;
//                printf("updateConnectionStatus CTRL\n\r");
                EventLog_Set_CANLoss(1);
            }
        } else {
            EventLog_Set_CANLoss(0);
        }
    }
}

#ifdef EVENT_LOG
void ReadCAN_Address()
{
    if(CAN_Rx_EventTotalCnts == 0) {
        ReadLatestAddress = EVENT_MSG_START_ADR;
    } else {
        if(CAN_Rx_EventTotalCnts >= EventReadCnt) {
            CAN_Rx_EventTotalCnts = CAN_Rx_EventTotalCnts - EventReadCnt; 
        } else {
            CAN_Rx_EventTotalCnts = 0;
        }
        if(EventCAN_Rx.flag.all == 1) {
            EventCAN_Rx.flag.all = 0;
            CAN_Rx_EventTotalCnts = 0;
        }
        CAN_EventCnt = CAN_Rx_EventTotalCnts;
        ReadLatestAddress = EVENT_MSG_START_ADR + (CAN_Rx_EventTotalCnts * 32);
    }
}

void EraseFlashDataProcess()
{
    UINT32 EraseAddress = 0;
    UINT32 LatesReadAddress = 0;
    UINT16 i = 0;
    UINT8 read_data[4096];
    UINT16 erase_sector = 0;
    UINT16 sector_cnt= 0;
    UINT16 sector_mod = 0;
    UINT16 read_len = 0;
    UINT16 GetRemainDataCnt = 0;
    
    for(i = 0;i < 4096;i ++)
        read_data[i] = 0;
    
    if(CntEventLength > EraseData) {
        GetRemainDataCnt = CntEventLength - EraseData;
        erase_sector = (EraseData >> 7) + 1;
        sector_cnt = GetRemainDataCnt >> 7;
        sector_mod = GetRemainDataCnt % 128;
        read_len = sector_mod << 5;
        LatesReadAddress = EVENT_MSG_START_ADR + (sector_cnt << 12);
        EraseAddress = LatesReadAddress;
        MX25L_Flash_Read(&read_data[0],LatesReadAddress,read_len);
        for(i = 0; i < erase_sector; i++) {
            Erase_Flash_SE(EraseAddress);
            EraseAddress = EraseAddress + 0X1000;
        }
        Nop();
        WriteMemoryData(&read_data[0], LatesReadAddress, read_len);
        Nop();
//        ReadLatestAddress = EVENT_MSG_START_ADR + (CntEventLength-1)*32;
//MX25L_Flash_Read(&read_data[0],LatesReadAddress,read_len);
        CntEventLength = GetRemainDataCnt;
        CAN_Rx_EventTotalCnts = CntEventLength;
    } else {
        erase_sector = (EraseData >> 7) + 1;
        for(i = 0; i < erase_sector; i++) {
            Erase_Flash_SE(EVENT_MSG_START_ADR);
            EraseAddress = EraseAddress + 0X1000;            
        }
        CAN_EventCnt = 0;
        CAN_Rx_EventTotalCnts = 0;
    }
    SaveEventHeadIndex();
}


void CanRx_EvnetFunction(void)
{   
    UINT8 i;
    UINT8 data_null =  0XFF;
    UINT32 time_second = 0;
    UINT32 time_cnt = 0;
    uCAN_MSG tempBuf;  
    
    if(ContinueFalg == 0) {
        CAN_Rx_EventTotalCnts = CntEventLength;
    }
    if(EventLog_Get_CanRxCmd() == 0) {
        return;
    }
    if(!SaveFlashFlag) {
        return;
    }
             
    if(EventCAN_Rx.flag.erase == 1) {     // Erease Flash IC Memory
		twiceFlag = 0;
        CAN_ResponseCntFlag = 0;
		if((EraseData == ERASE_ALL)&&(EraseData > 0X7FFFFFFF)) {
            EraseAllFlag = 1;
            Erase_Flash(EVENT_MSG_START_ADR, ERASE_2M);
            EraseData = 0;
            EvnetSentDataCnt = 0;
            NoIntoEventRecentFlag = 0;
		} else if(EraseData <= 0X7FFFFFFF) {
            NoIntoEventRecentFlag = 2;
            EraseFlashDataProcess();
        }
    }
    if(EventCAN_Rx.flag.recent == 1) {                    // Recent (Second)
        twiceFlag = 0;
        CAN_ResponseCntFlag = 0;
        EventCAN_Rx.flag.recent = 0;
        NoIntoEventRecentFlag = 1;
        EventLogContinuID = 0;
        En_RecentFalg = 1;    
        ReadCAN_Address();
        if((RecentSecondCnt > 0)&&(RecentSecondCnt < 0X80000000)) {
            RecentSecondCnt = RecentSecondCnt << 1;      // 1second = 2*(0.5ms)
        }
        EvnetSentDataCnt = CntEventLength;       // CntEventIndex:Event Total cnt
    }
    
    if(EventCAN_Rx.flag.rewind == 1) {                    // Rewind
        twiceFlag = 0;
        CAN_ResponseCntFlag = 0;
        NoIntoEventRecentFlag = 0;
        EventLogContinuID = 0;
        EvnetSentDataCnt = 0;
        EventCAN_Rx.flag.rewind = 0;
        CAN_Rx_EventTotalCnts = CntEventLength;
    }
    
    if(EventCAN_Rx.flag.all == 1) {                      // Send All Event Data
        twiceFlag = 0;
        CAN_ResponseCntFlag = 0;
        NoIntoEventRecentFlag = 0;
        EvnetSentDataCnt = CntEventLength;
        ReadCAN_Address();
        EventLogContinuID = 0;
        CanResponse30sCnt = 0;
    }
    if(CAN_EventCnt == 0) {
        if((CAN_ResponseCntFlag == 1) || (EventCAN_Rx.flag.erase == 1))
            EvnetSentDataCnt = 0;
    }

    if((EventCAN_Rx.flag.erase == 0) && (EventCAN_Rx.flag.abort == 0)) {
        if(EvnetSentDataCnt > 0) {
            while(EvnetSentDataCnt--) {
                if(twiceFlag == 0) {
                    twiceFlag = 1;
                    MX25L_Flash_Read(&LastBufData[0],ReadLatestAddress,32);              
                }
                MX25L_Flash_Read(&LastBufData[0],ReadLatestAddress,32);
                for(i = 0 ;i < 32;i ++) {
                   data_null &= LastBufData[i];
                }
                if(data_null == 0XFF) {
                    EventLog_Set_CanRxCmd(0);
                    SaveFlashFlag = false;
                    EvnetSentDataCnt = 0;
                    EventCAN_Rx.Status = 0;
                    break;
                }

                time_second = LastBufData[3];
                time_second <<= 8;
                time_second |= LastBufData[2];
                time_second <<= 8;
                time_second |= LastBufData[1];
                time_second <<= 8;        
                time_second |= LastBufData[0];        
                //time_second
                if(En_RecentFalg == 1) {
                    En_RecentFalg = 0;
                    time_cnt = LastBufData[3];
                    time_cnt <<= 8;
                    time_cnt |= LastBufData[2];
                    time_cnt <<= 8;
                    time_cnt |= LastBufData[1];
                    time_cnt <<= 8;        
                    time_cnt |= LastBufData[0];
                    if(time_cnt > RecentSecondCnt) {
                        time_cnt = time_cnt - RecentSecondCnt;
                    }
                }
                if(time_second < time_cnt) {
                    EvnetSentDataCnt = 0;
                    RecentSecondCnt = 0;
                    break;
                }
                for(i = 0; i < 6; i++) {
                    tempBuf.frame.id = DBC_CMD_EVENTRESP;
					tempBuf.frame.id |= d_deviceAddr;
                    tempBuf.frame.idType = CAN_FRAME_STD;
                    tempBuf.frame.dlc = 8;  	
                    tempBuf.frame.data0 = canEventBuf.frame.data0 & 0XF0;
                    tempBuf.frame.data0 |= i+1;            
                    if(i == 0) {
                        if(EventLogContinuID < 0X00100000) {
                            EventLogContinuID ++;
                        } else {
                            EventLogContinuID = 0;
                        }
                        tempBuf.frame.data1 = ((EventLogContinuID & 0X0000000F) << 4);
                        tempBuf.frame.data2 = ((EventLogContinuID & 0X00000FF0) >> 4);
                        tempBuf.frame.data3 = ((EventLogContinuID & 0X000FF000) >> 12);            
                        tempBuf.frame.data1 |= 0X01;
                        tempBuf.frame.data4 = LastBufData[0];
                        tempBuf.frame.data5 = LastBufData[1];
                        tempBuf.frame.data6 = LastBufData[2];
                        tempBuf.frame.data7 = LastBufData[3];    
                        EventLogDataNum = 3;
                    } else if(i == 5) {
                        if(ReadLatestAddress == EVENT_MSG_START_ADR) {
                            EvnetSentDataCnt = 0;
                        } else {
                            if(CAN_EventCnt > 0) {
                                CAN_EventCnt --;
                                ReadLatestAddress = EVENT_MSG_START_ADR + CAN_EventCnt * 32;
                            }
                        }
                        if(EvnetSentDataCnt > 0) {           
                            tempBuf.frame.data1 = 0X01;
                        } else {
                            tempBuf.frame.data1 = 0X02;
                        }
                        tempBuf.frame.data2 = LastBufData[28];
                        tempBuf.frame.data3 = LastBufData[29];
                        tempBuf.frame.data4 = LastBufData[30];
                        tempBuf.frame.data5 = LastBufData[31];
                        tempBuf.frame.data6 = 0;
                        tempBuf.frame.data7 = 0;                  
                    } else {
                        tempBuf.frame.data1 = 0x01;                    // EventLog Response Continue
                        EventLogDataNum ++;                             // matrix number
                        tempBuf.frame.data2 = LastBufData[EventLogDataNum];
                        EventLogDataNum ++; 
                        tempBuf.frame.data3 = LastBufData[EventLogDataNum];
                        EventLogDataNum ++; 
                        tempBuf.frame.data4 = LastBufData[EventLogDataNum];
                        EventLogDataNum ++; 
                        tempBuf.frame.data5 = LastBufData[EventLogDataNum];
                        EventLogDataNum ++; 
                        tempBuf.frame.data6 = LastBufData[EventLogDataNum];
                        EventLogDataNum ++; 
                        tempBuf.frame.data7 = LastBufData[EventLogDataNum];                 
                    }
                    while(CAN1_Transmit(CAN_PRIORITY_HIGH,&tempBuf));
                    //putRequestCmdIntoBuf(&tempBuf, &tempTxRqstBuf);
                    //CAN_putItemIntoTransmitBuffer(&tempBuf);
                }
            }
            EventCAN_Rx.flag.erase = 0;
        }
    }
    EventCAN_Rx.Status = 0;
    EventLog_Set_CanRxCmd(0);
    twiceFlag = 0;
}


#endif
/*******************************************************************************
* Function Name  : CanDbc_mainFunction
* Description    : DBC main function
* Input          : None
* Return         : None
*******************************************************************************/
void CanDbc_mainFunction(void)
{  
    updateMemoryMap();
    dbcRx_mainFunction();
	if(SysTime_msecIntervalElapsed(TxCAN_15msOnceCnt, 15)) {
		TxCAN_15msOnceCnt = SysTime_getMsec();
        dbcTx_mainFunction();
	}
    updateConnectionStatus();
    RespAuxReqCmd(); //frank 9/11
}

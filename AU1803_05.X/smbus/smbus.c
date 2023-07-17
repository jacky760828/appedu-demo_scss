/******************************************************************************
 *
 * FILE: smbus.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "smbus.h"


typedef enum {
	MftNone_STEP0,
    MftShutDown_STEP1,
    MftShutDown_STEP2,
    MftShutDown_STEP2_1,
    MftShutDown_STEP3,
    MftPFRelease_STEP1,
    MftPFRelease_STEP2,
    MftPFRelease_STEP3,
    MftPFRelease_wiate,
} MfrShutDownState_t;


typedef enum {
	SEAL,
    Unseal_Key_LSW,
    Unseal_Key_MSW,
    UNSEAL,
    Full_Access_Key_LSW,
    Full_Access_Key_MSW,
    UN_FullSEAL,
    CHECK_UNSEAL,
} BQ78350_UNSEAL_STATE;

typedef struct
{
    bool                ShutDownEnable;
    bool                ShutDownRun;
    bool                OffByState;
    MfrShutDownState_t  executeStep;
} __attribute__ ((__packed__)) SMB_GAGE_Info_t; //16bytes

union {  //frank 7/7
    struct {
        unsigned char smbusWriteProcessPending  :1;
        unsigned char smbusInProgress           :1;
        unsigned char smbusAllCmdPolled         :1;
        unsigned char reserved                  :5;
        };
    unsigned char byte;
} d_smbusProcessFlags;

struct {
    unsigned char smbusCmdType_MAC          :1;
    unsigned char smbusCmdMacCmdSet         :1;
    unsigned char smbusCmdMacCmdAccess      :1;
    unsigned char smbusCmdMacCmdDone        :1;
    unsigned char smbusCmdMacWriteRead      :1;
    unsigned char reserved                  :3;
} d_smbusCmdMacFlags = {0,0,0,0,0,0};

typedef struct {
    uint8_t run         :1;
    uint8_t err         :1;
    uint8_t end         :1;
    uint8_t wr          :1;
    uint8_t all_info    :1;
    uint8_t rev         :3;
    BQ78350_UNSEAL_STATE seal;
    uint16_t BQ78350_SBSCmdindex;
    uint8_t BQ78350_SBScmd;
} BQ78350Request_CANBUS;

uint8_t d_smbusCmdIdx;

uint16_t d_maxCellVolt;
uint16_t d_minCellVolt;
int16_t d_cellTemperature;
int16_t d_fetTemperature; 
int16_t d_gaugeTemperature;
uint16_t d_maxTemperature;
uint16_t d_minTemperature;

uint8_t d_smbusPendingIdxCmdByteNo;

BQ78350Request_CANBUS d_BQ78350SBSCmd;

SMB_GAGE_Info_t d_smbusGageState;
uint32_t d_ExecutTime ;
#define EXECUTE_TIMEOUT_MS  3000

uint8_t log_same = 0;

#define I2C_250MS_TIME   250 //ms
uint32_t d_250msTime = -I2C_250MS_TIME;
uint8_t cmd250msIndex = 0;


uint16_t IsSBSCmd(uint8_t bq78450cmd)
{
    uint16_t ret = 0x8000;
    uint16_t i;
    
    for (i = 0; i < BQ78350SBSCmd_MAXSIZE; i++) {
        if (bq78450cmd == smbusBQ78350_SBSCmdlist[i]) {
            ret = (i & 0x7FFF);
            break;
        }
    }
    return ret;
}

void SMBUS_BQ78350_SBS_Cmd(uint8_t cmd)
{
    uint16_t ret;
    
    ret = IsSBSCmd(cmd);
    if(0x8000 != ret) {
        d_BQ78350SBSCmd.run = true;
        d_BQ78350SBSCmd.err = false;
        d_BQ78350SBSCmd.end = false;
        d_BQ78350SBSCmd.BQ78350_SBSCmdindex = ret;
        d_BQ78350SBSCmd.BQ78350_SBScmd = smbusBQ78350_SBSCmdlist[ret];
    }
}

bool SMBUS_is_ShutDown_Run(void)
{
    return d_smbusGageState.ShutDownRun;
}

void SMBUS_setShutDownEnable(bool onoff)
{
    printf("d_smbusShutDownEnable(%d) \r\n",onoff);
    d_smbusGageState.ShutDownEnable = onoff;
}

bool SMBUS_getShutDownEnable(void)
{
    return d_smbusGageState.ShutDownEnable;
}

uint8_t SMBUS_getStateOfHealth(void)
{
    return SMBUS_stateOfHeath[0];
}

uint16_t SMBUS_getCycleCount(void)
{
    return ((uint16_t)SMBUS_cycleCount[1] << 8 ) | SMBUS_cycleCount[0];
}

uint16_t SMBUS_getVoltage(void)
{
    return ((uint16_t)SMBUS_voltage[1] << 8 ) | SMBUS_voltage[0];
}

int16_t SMBUS_getCurrent(void)
{
    return ((uint16_t)SMBUS_current[1] << 8 ) | SMBUS_current[0];
}

uint16_t SMBUS_getChargingCurrent(void)
{
    return ((uint16_t)SMBUS_chargingCurrent[1] << 8 ) | SMBUS_chargingCurrent[0];
}

uint16_t SMBUS_getChargingVoltage(void)
{
    return ((uint16_t)SMBUS_chargingVoltage[1] << 8 ) | SMBUS_chargingVoltage[0];
}

uint16_t SMBUS_getSerialNumber(void)
{
    return ((uint16_t)SMBUS_serialNumber[1] << 8 ) | SMBUS_serialNumber[0];
}

uint16_t SMBUS_getManufactureDate(void)
{
    return ((uint16_t)SMBUS_manufactureDate[1] << 8 ) | SMBUS_manufactureDate[0];
}

uint16_t SMBUS_getDesignCapacity(void)
{
    return ((uint16_t)SMBUS_designCapacity[1] << 8 ) | SMBUS_designCapacity[0];
}

uint16_t SMBUS_getAFEConfig(void)
{
    uint16_t ret = 0;
    
    ret = SMBUS_afeStatus[2] & 0x1F;
    ret <<= 5;
    ret |= SMBUS_afeStatus[1] & 0x1F;
    ret <<= 5;
    ret |= SMBUS_afeStatus[0] & 0x1F;
    
    return ret;
}

uint16_t SMBUS_getGaugeRC(void)
{
    return ((uint16_t)SMBUS_remainingCapacity[1] << 8 ) | SMBUS_remainingCapacity[0];
}

uint16_t SMBUS_getGaugeFCC(void)
{
    return ((uint16_t)SMBUS_fullChargeCapacity[1] << 8 ) | SMBUS_fullChargeCapacity[0];
}

/*******************************************************************************
* Function Name  : SMBUS_getCDFETs
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
uint16_t SMBUS_getCDFETs(void)
{
    return ((uint16_t)SMBUS_operationStatus[1] << 8 ) | SMBUS_operationStatus[0];
}

/*******************************************************************************
* Function Name  : SMBUS_init
* Description    : SMBUS initial
* Input          : None
* Return         : None
*******************************************************************************/
void SMBUS_init(void)
{
    // Baud Rate Generator Value
    //    
    BQ78350_I2C_BUFF.SMB_state = SmbusState_IDLE;
   
    d_smbusProcessFlags.byte = 0;
    BQ78350_I2C_BUFF.SMB_timer =  -I2C_INTERVAL_TIME;//0;
    BQ78350_I2C_BUFF.DATA_Type = 0;
    BQ78350_I2C_BUFF.MacCmdAccess = false;
    d_smbusCmdMacFlags.smbusCmdType_MAC = false;
    d_smbusCmdIdx = 0;
    d_maxCellVolt = 3500;//0;
    d_minCellVolt = 3500;//0xFFFF;
    d_cellTemperature = 2731+250;
    d_fetTemperature = 2731+250;
    d_gaugeTemperature = 2731+250;
    d_maxTemperature = 2731+250;
    d_minTemperature = 2731+250;
    d_smbusGageState.OffByState = false;
    d_smbusGageState.ShutDownEnable = false;
    d_smbusGageState.ShutDownRun = false;
    d_smbusGageState.executeStep = MftNone_STEP0;
    d_BQ78350SBSCmd.BQ78350_SBScmd = SBS_Voltage;
    d_BQ78350SBSCmd.seal = CHECK_UNSEAL;
    d_BQ78350SBSCmd.all_info = false;
}

/*******************************************************************************
* Function Name  : SMBUS_isFree
* Description    : 
* Input          : the number of command
* Return         : return true if no any command collision
*******************************************************************************/
bool SMBUS_isFree(void)
{
    if ((d_smbusProcessFlags.byte & 0x0F) == 0x08) {
        return true;//FALSE
    } else {
        return false;//TRUE;
    }
}

uint16_t SMBUS_FIND_CMDINDEX(uint8_t cmd)
{
    uint16_t cmdindex;
    
    for(cmdindex = 0; cmdindex < BQ78350SBSCmd_MAXSIZE;cmdindex++) {
        if(cmd == smbusMatrixCmd[cmdindex].commandCode) {
            break;
        }
    }
    if(BQ78350SBSCmd_MAXSIZE < cmdindex) {
        cmdindex = -1;
    }
    return cmdindex;
}

void BQ78350Read(uint8_t commandlist, uint8_t MANUFACTURER)
{
    BQ78350_I2C_BUFF.communication_format = SMBUS;
    
    I2C1_STATE_INT_FLAG = 0x0;
    I2C1_START_CONDITION_ENABLE_BIT = 0x1;              //send START
    BQ78350_I2C_BUFF.SMB_timer = SysTime_getMsec();
    BQ78350_I2C_BUFF.SLAVE_ADDRESS = SLAVE_DEVICE_ADDR;
    BQ78350_I2C_BUFF.DATA_Type = smbusMatrixCmd[commandlist].protocol;
    if(MANUFACTURER_ACCESS_CMD == MANUFACTURER) {
        BQ78350_I2C_BUFF.WR = 0;
        BQ78350_I2C_BUFF.READ_CMD_Size = 1;
        BQ78350_I2C_BUFF.READ_CMD_index = 0;
        BQ78350_I2C_BUFF.READ_CMD_Buffer[0] = MANUFACTURER_BLOCK_ACCESS_CMD;
        BQ78350_I2C_BUFF.RxDataSize = smbusMatrixCmd[commandlist].byteNumber;
    } else if(MANUFACTURER_BLOCK_ACCESS_CMD == MANUFACTURER) {
    } else {
        BQ78350_I2C_BUFF.WR = 0;
        BQ78350_I2C_BUFF.READ_CMD_Size = 1;
        BQ78350_I2C_BUFF.READ_CMD_index = 0;
        BQ78350_I2C_BUFF.READ_CMD_Buffer[0] = smbusMatrixCmd[commandlist].commandCode;
        BQ78350_I2C_BUFF.RxDataSize = smbusMatrixCmd[commandlist].byteNumber;
    }
    BQ78350_I2C_BUFF.Rxindex = 0;
    BQ78350_I2C_BUFF.READ_DATA_CRC = true;
    BQ78350_I2C_BUFF.SMB_state = SmbusState_CMD_READ_SLAVE_ADDRESS;
    BQ78350_I2C_BUFF.Error = I2C_MESSAGE_PENDING;
}

void BQ78350Write(uint8_t commandlist, uint8_t MANUFACTURER)
{
    int i;
    
    BQ78350_I2C_BUFF.communication_format = SMBUS;
    I2C1_STATE_INT_FLAG = 0x0;
    I2C1_START_CONDITION_ENABLE_BIT = 0x1;              //send START
    BQ78350_I2C_BUFF.SMB_timer = SysTime_getMsec();
    BQ78350_I2C_BUFF.SLAVE_ADDRESS = SLAVE_DEVICE_ADDR;
    BQ78350_I2C_BUFF.WR = 0;
    if(MANUFACTURER_BLOCK_ACCESS_CMD == MANUFACTURER) {
        BQ78350_I2C_BUFF.WRITE_CMD_Size = 1;
        BQ78350_I2C_BUFF.WRITE_CMD_index = 0;
        BQ78350_I2C_BUFF.WRITE_CMD_Buffer[0] = MANUFACTURER_BLOCK_ACCESS_CMD;
        BQ78350_I2C_BUFF.DATA_Type = BQ78350ManufacturerAccessCmd[commandlist].protocol;
        BQ78350_I2C_BUFF.TxDataSize = BQ78350ManufacturerAccessCmd[commandlist].byteNumber;
        for(i = 0; i < BQ78350_I2C_BUFF.TxDataSize; i++) {
            BQ78350_I2C_BUFF.TxBuffer[i] = BQ78350ManufacturerAccessCmd[commandlist].ptrCommandData[i];
        }
        BQ78350_I2C_BUFF.READ_CMD_Size = 0;
        BQ78350_I2C_BUFF.RxDataSize = 0;
    } else {
        if(commandlist < 23) {
            BQ78350_I2C_BUFF.WRITE_CMD_Size = 3;
            BQ78350_I2C_BUFF.WRITE_CMD_index = 0;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[0] = MANUFACTURER_ACCESS_CMD;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[1] = BQ78350ManufacturerAccessCmd[commandlist].commandCode;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[2] = BQ78350ManufacturerAccessCmd[commandlist].commandCode >> 8;
            BQ78350_I2C_BUFF.RxDataSize = 0;
        } else {
            BQ78350_I2C_BUFF.READ_CMD_Size = 1;
            BQ78350_I2C_BUFF.READ_CMD_index = 0;
            BQ78350_I2C_BUFF.READ_CMD_Buffer[0] = MANUFACTURER_BLOCK_ACCESS_CMD;
            BQ78350_I2C_BUFF.RxDataSize = BQ78350ManufacturerAccessCmd[commandlist].byteNumber;
            BQ78350_I2C_BUFF.WRITE_CMD_Size = 3;
            BQ78350_I2C_BUFF.WRITE_CMD_index = 0;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[0] = MANUFACTURER_ACCESS_CMD;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[1] = BQ78350ManufacturerAccessCmd[commandlist].commandCode;
            BQ78350_I2C_BUFF.WRITE_CMD_Buffer[2] = BQ78350ManufacturerAccessCmd[commandlist].commandCode >> 8;
        }
    }
    BQ78350_I2C_BUFF.WRITE_DATA_CRC = 1;
    BQ78350_I2C_BUFF.SMB_state = SmbusState_CMD_WRITE_SLAVE_ADDRESS;
    BQ78350_I2C_BUFF.Error = I2C_MESSAGE_PENDING;
}

BQ78350_UNSEAL_STATE BQ78350_UNSEAL(BQ78350_UNSEAL_STATE newseal)
{
    uint16_t cmdindex = 0;
    
    if(d_BQ78350SBSCmd.seal == newseal) {
        return d_BQ78350SBSCmd.seal;
    }
    switch(d_BQ78350SBSCmd.seal) {
        case SEAL:
            for(cmdindex = 0; cmdindex < BQ78350ManufacturerAccessCmd_MAXSIZE;cmdindex++) {
                if(SecurityKeys == BQ78350ManufacturerAccessCmd[cmdindex].commandCode) {
                    break;
                }
            }
            if(BQ78350ManufacturerAccessCmd_MAXSIZE > cmdindex) {
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[0] = (uint8_t)UNSEAL_KEY1;
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[1] = (uint8_t)(UNSEAL_KEY1 >> 8);
                BQ78350Write(cmdindex, MANUFACTURER_BLOCK_ACCESS_CMD);
                d_BQ78350SBSCmd.seal = Unseal_Key_LSW;
            }
            break;
        case Unseal_Key_LSW:
            for(cmdindex = 0; cmdindex < BQ78350ManufacturerAccessCmd_MAXSIZE;cmdindex++) {
                if(SecurityKeys == BQ78350ManufacturerAccessCmd[cmdindex].commandCode) {
                    break;
                }
            }
            if(BQ78350ManufacturerAccessCmd_MAXSIZE > cmdindex) {
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[0] = (uint8_t)UNSEAL_KEY2;
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[1] = (uint8_t)(UNSEAL_KEY2 >> 8);
                BQ78350Write(cmdindex, MANUFACTURER_BLOCK_ACCESS_CMD);
                d_BQ78350SBSCmd.seal = Unseal_Key_MSW;
            }
            break;
        case UNSEAL:
            for(cmdindex = 0; cmdindex < BQ78350ManufacturerAccessCmd_MAXSIZE;cmdindex++) {
                if(AuthenticationKey == BQ78350ManufacturerAccessCmd[cmdindex].commandCode) {
                    break;
                }
            }
            if(BQ78350ManufacturerAccessCmd_MAXSIZE > cmdindex) {
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[0] = (uint8_t)UNFullSEAL_KEY1;
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[1] = (uint8_t)(UNFullSEAL_KEY1 >> 8);
                BQ78350Write(cmdindex, MANUFACTURER_BLOCK_ACCESS_CMD);
                d_BQ78350SBSCmd.seal = Full_Access_Key_LSW;
            }
            break;
        case Full_Access_Key_LSW:
            for(cmdindex = 0; cmdindex < BQ78350ManufacturerAccessCmd_MAXSIZE;cmdindex++) {
                if(AuthenticationKey == BQ78350ManufacturerAccessCmd[cmdindex].commandCode) {
                    break;
                }
            }
            if(BQ78350ManufacturerAccessCmd_MAXSIZE > cmdindex) {
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[0] = (uint8_t)UNFullSEAL_KEY2;
                BQ78350ManufacturerAccessCmd[cmdindex].ptrCommandData[1] = (uint8_t)(UNFullSEAL_KEY2 >> 8);
                BQ78350Write(cmdindex, MANUFACTURER_BLOCK_ACCESS_CMD);
                d_BQ78350SBSCmd.seal = CHECK_UNSEAL;
            }
            break;
        case UN_FullSEAL:
            break;
        case CHECK_UNSEAL:
            switch(smbusMatrixCmd[0].ptrCommandData[0] & 0x03) {
                case 0x00:
                    BQ78350Read(SMBUS_FIND_CMDINDEX(MANUFACTURER_ACCESS_CMD), SBS_CMD);
                    break;
                case 0x01:
                    d_BQ78350SBSCmd.seal = UN_FullSEAL;
                    break;
                case 0x02:
                    d_BQ78350SBSCmd.seal = UNSEAL;
                    break;
                case 0x03:
                    d_BQ78350SBSCmd.seal = SEAL;
                    break;
            }
            break;
        default:
            break;
    }
    return d_BQ78350SBSCmd.seal;
}

bool BQ78350_all_info_ready(void)
{
    return d_BQ78350SBSCmd.all_info;
}

void BQ78350Authentication(void)
{
    if(d_smbusGageState.OffByState) {
        //pause smbus polling let gas-gauge enter idle
    } else if(IDLE == BQ78350_I2C_BUFF.communication_format) {
//        if(UNSEAL != BQ78350_UNSEAL(SEAL)) {
        if(UNSEAL != BQ78350_UNSEAL(UNSEAL)) {
            return;
        } else if(d_BQ78350SBSCmd.run) {
        } else if(d_smbusGageState.ShutDownRun) {
            BQ78350Write(8,MANUFACTURER_ACCESS_CMD);
        } else {
            BQ78350Read(d_smbusCmdIdx, SBS_CMD);
//            BQ78350Read(SMBUS_FIND_CMDINDEX(SBS_AFEStatus), SBS_CMD);
        }
    } else if(SmbusState_CMD_READ_COMPLETE == BQ78350_I2C_BUFF.SMB_state) {
        if(SMBUS == BQ78350_I2C_BUFF.communication_format) {
            if(d_BQ78350SBSCmd.run) {
            } else if(CHECK_UNSEAL == d_BQ78350SBSCmd.seal) {
                i2cstate_Get_RXBUFF(smbusMatrixCmd[0].ptrCommandData, 0);
                BQ78350_I2C_BUFF.SMB_state = SmbusState_IDLE;
            } else {
                i2cstate_Get_RXBUFF(smbusMatrixCmd[d_smbusCmdIdx].ptrCommandData, 0);
                d_smbusCmdIdx++;
//                if(d_smbusCmdIdx >= SMBUS_FIND_CMDINDEX(SBS_StateOfHealth)) {
                if(d_smbusCmdIdx >= BQ78350SBSCmd_MAXSIZE) {
//                    d_smbusCmdIdx = SMBUS_FIND_CMDINDEX(SBS_StateOfHealth);
                    d_smbusCmdIdx = 0;
                    d_BQ78350SBSCmd.all_info = true;
                    printf("\e[1;32m d_smbusCmdIdx ALL \033[0m \n\r");
                }
                BQ78350_I2C_BUFF.SMB_state = SmbusState_IDLE;
            }
        }
    } else if(SmbusState_CMD_WRITE_COMPLETE == BQ78350_I2C_BUFF.SMB_state) {
        if(SMBUS == BQ78350_I2C_BUFF.communication_format){
            if(d_BQ78350SBSCmd.run) {
                BQ78350_I2C_BUFF.SMB_state = SmbusState_IDLE;
            } else if(Unseal_Key_MSW == d_BQ78350SBSCmd.seal) {
                d_BQ78350SBSCmd.seal = CHECK_UNSEAL;
                BQ78350Read(SMBUS_FIND_CMDINDEX(MANUFACTURER_ACCESS_CMD), SBS_CMD);
            } else {
                BQ78350_I2C_BUFF.SMB_state = SmbusState_IDLE;
            }
        }
    } else {
    }
}

/*******************************************************************************
* Function Name  : SMBUS_getMaxCellVolt
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
uint16_t SMBUS_getMaxCellVolt(void)
{
    return d_maxCellVolt;
}

/*******************************************************************************
* Function Name  : SMBUS_getMinCellVolt
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
uint16_t SMBUS_getMinCellVolt(void)
{
    return d_minCellVolt;
}

// frank add
void SMBUS_sortMinMaxCellVolt(void)
{
    uint8_t index;
    uint16_t tempResult, tempMax, tempMin;
    
    tempMax = 0; //frank 7/8/2021
    tempMin =0xFFFF;
    
    for (index = 1; index <= CELL_IN_SERIES; index++) {
        tempResult = (uint16_t)SMBUS_cellVoltage[index][1] << 8 | SMBUS_cellVoltage[index][0];
        tempResult = tempResult;
        if (tempResult < tempMin) {
            tempMin = tempResult;
        }
        if (tempResult > tempMax) {
            tempMax = tempResult;
        }
    }
    if (tempMin > 1000 && tempMin < 4600) { //tsai 7/8/2021
        d_minCellVolt = tempMin;
    }
    if (tempMax > 1000 && tempMax < 4600) {
        d_maxCellVolt = tempMax;
    }
}

/*******************************************************************************
* Function Name  : SMBUS_getMaxTemperature
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
uint16_t SMBUS_getMaxTemperature(void)
{
    return d_maxTemperature;
}

/*******************************************************************************
* Function Name  : SMBUS_getMinTemperature
* Description    : 
* Input          : None
* Return         : None
*******************************************************************************/
uint16_t SMBUS_getMinTemperature(void)
{
    //SMBUS_Temperature_info();
    return d_minTemperature;
}

void SMBUS_Temperature_info(void)
{
    uint16_t tempReading;
    int8_t i;
    
    tempReading = 0;
    d_maxTemperature = 0;
    d_minTemperature = 0xFFFF; //frank 4/1  5/15
    for(i = 13-6; i >= 4; i--) { //frank 5 -> 4 -> 6 TS1,TS2 //,Cell   frank 7/3 
        tempReading = (tempReading << 8);
        tempReading += SMBUS_daStatus2[i];
        if((i % 2) == 0) {
            if(tempReading > d_maxTemperature) {
                d_maxTemperature = tempReading;
            }
            if (tempReading < d_minTemperature) {
                d_minTemperature = tempReading;
            }
            tempReading = 0;
        }
    } // frank 7/3 T3
    tempReading = SMBUS_daStatus2[13];              //pointer to high byte of FET temperature
    tempReading = (tempReading << 8);
    tempReading += SMBUS_daStatus2[12];              //pointer to low byte of FET temperature
//    if (tempReading <= 2731+1500) {
    d_fetTemperature = tempReading;
//    }
    
    d_gaugeTemperature = 0;
    d_gaugeTemperature += SMBUS_daStatus2[15];            //pointer to high byte of gauge temperature
    d_gaugeTemperature = (d_gaugeTemperature << 8);
    d_gaugeTemperature += SMBUS_daStatus2[14];            //pointer to low byte of gauge temperature
    
    d_cellTemperature = 0;
    d_cellTemperature += SMBUS_daStatus2[11];             //pointer to high byte of cell temperature
    d_cellTemperature = (d_cellTemperature << 8);
    d_cellTemperature += SMBUS_daStatus2[10];             //pointer to low byte of cell temperature
}

/*******************************************************************************
* Function Name  : SMBUS_getCellTemperature
* Description    : 
* Input          : None
* Return         : temperature reading
*******************************************************************************/
int16_t SMBUS_getCellTemperature(void)
{   
    return d_cellTemperature;
}

/*******************************************************************************
* Function Name  : SMBUS_getFetTemperature
* Description    : 
* Input          : None
* Return         : temperature reading
*******************************************************************************/
int16_t SMBUS_getFetTemperature(void) 
{
    return d_fetTemperature; 
}

/*******************************************************************************
* Function Name  : SMBUS_getGaugeTemperature
* Description    : 
* Input          : None
* Return         : temperature reading
*******************************************************************************/
int16_t SMBUS_getGaugeTemperature(void)
{
    return d_gaugeTemperature;
}

static void CheckPowerOffRequest(void)
{
//    printf("CheckPowerOffRequest \r\n");
    if(!d_smbusGageState.ShutDownRun) {
        if (d_smbusGageState.ShutDownEnable && d_smbusGageState.executeStep == MftNone_STEP0 ) {
            d_smbusGageState.ShutDownEnable = false;
            d_smbusGageState.ShutDownRun = true;
            printf("ShutDownEnableCmd ");
            d_ExecutTime = SysTime_getMsec();
            if ((SMBUS_operationStatus[1] & 0x03) == 0x03) { //sealed, send twice of shutdown command
                BQ78350_I2C_BUFF.communication_format = SMBUS;
                BQ78350Write(8,MANUFACTURER_ACCESS_CMD);
                d_smbusProcessFlags.smbusWriteProcessPending = true;
                d_smbusGageState.executeStep = MftShutDown_STEP1;
                printf("executeStep = MftShutDown_STEP1 \r\n");
                return;
            } else {
                d_smbusGageState.executeStep = MftShutDown_STEP2_1; //1
                printf("executeStep = MftShutDown_STEP2 \r\n");
            }
        }
    }
    switch (d_smbusGageState.executeStep) {
        case MftShutDown_STEP1:	
            printf("SMBUS UNSEAL KEY2 \r\n");
            BQ78350_I2C_BUFF.communication_format = SMBUS;
            d_smbusProcessFlags.smbusWriteProcessPending = true;
            d_smbusGageState.executeStep = MftShutDown_STEP2;
            break;
            
        case MftShutDown_STEP2:
            printf("SMBUS MftShutDown_STEP2_1 \r\n");
            BQ78350_I2C_BUFF.communication_format = SMBUS;
            BQ78350Write(8,MANUFACTURER_ACCESS_CMD);
            d_smbusProcessFlags.smbusWriteProcessPending = true;
            d_smbusGageState.ShutDownRun = true;
            d_smbusGageState.executeStep = MftShutDown_STEP2_1;
            break;

        case MftShutDown_STEP2_1:
            printf("SMBUS MftShutDown_STEP3 \r\n");
            BQ78350_I2C_BUFF.communication_format = SMBUS;
            BQ78350Write(8,MANUFACTURER_ACCESS_CMD);
            d_smbusProcessFlags.smbusWriteProcessPending = true;
            d_smbusGageState.ShutDownRun = true;
            d_smbusGageState.executeStep = MftPFRelease_wiate;
            break;
            
        case MftShutDown_STEP3:
            if(!log_same) {
            printf("SMBUS Wait SDM and CDFET off \r\n");
                log_same = 1;
            }
            if ( (SMBUS_operationStatus[2] & 0x01) && ((SMBUS_operationStatus[0] & 0x06) == 0x00)) { //SDM set and CDFET off
                log_same = 0;
                printf("SMBUS SDM and CDFET off \r\n");
                d_smbusGageState.OffByState = true;
                d_smbusGageState.ShutDownRun = true;
                d_smbusGageState.executeStep = MftNone_STEP0;
                printf("executeStep = MftNone_STEP0 \r\n");
            }
            break;
            
        default:
            break;
    }
}

static void CheckWriteBq78350Request(void)
{
    if (d_BQ78350SBSCmd.run) {
        d_BQ78350SBSCmd.run = false;
        if(!IsSBSCmd(d_BQ78350SBSCmd.BQ78350_SBScmd)) { //not find
            d_BQ78350SBSCmd.err = true;
            printf("NO BQ78350 Cmd\r\n");
            return;
        }
        d_ExecutTime = SysTime_getMsec();
        printf("bq78350wCmd(0x%02X) \r\n", d_BQ78350SBSCmd.BQ78350_SBScmd);
        BQ78350_I2C_BUFF.communication_format = SMBUS;
        d_smbusProcessFlags.smbusWriteProcessPending = true;
    }
}

/*******************************************************************************
* Function Name  : SMBUS_mainFunction
* Description    : SMBUS main function
* Input          : None
* Return         : None
*******************************************************************************/
void SMBUS_mainFunction(void)
{
    BQ78350Authentication();
    if (!BMS_Get_Operating_Flag(WAKEUP)) {
        CheckPowerOffRequest(); //frank 6/11
    }
    CheckWriteBq78350Request(); //tsai 8/16  
}

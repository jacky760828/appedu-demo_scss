 /*******************************************************************************
 * File:   Eventlog.c
 * Author: C-TECH
 *
 * Created on 2022/05/17
 *******************************************************************************/
#ifdef EVENT_LOG
//====Include File===============================
#include "EventLog.h"
#include "../main_app/scheduler.h"
#include "../battery_manage_system/battery_manage_system.h"
#endif
//====Define Various and constant==============
 
#ifdef EVENT_LOG
#define NO_DISCHARGE_5MIN   300000L

uint16_t ErrorCodeTable[56] = {
0x1001, 0x1003, 0x1005, 0x1007, 0x1009, 0x100B, 0x1107, 0x1101, 
0x1103, 0x1105, 0x1201, 0x1205, 0x1203, 0x1207, 0x1209, 0x1300,
0x1301, 0x1302, 0x1303, 0x1304, 0x1305, 0x1306, 0x1307, 0x1308,
0x1400, 0x1401, 0x1402, 0x1403, 0x1404, 0x1405, 0x1406, 0x1407, 
0x1501, 0x1500, 0x0100, 0x0100, 0x0100, 0x0101, 0x0000, 0x0001,
0x1000, 0x1002, 0x1004, 0x1006, 0x1008, 0x100A, 0x1106, 0x1100,
0x1102, 0x1104, 0x1200, 0x1204, 0x1202, 0x1206, 0x1208, 0X1408};

uint8_t EventString[5] = {0X45, 0X76, 0x65, 0X6E, 0X74};     //String "Event"
uint8_t life_data[LDAT_TOTAL_BYTES];
//====Gobal Various============================
uint8_t EnterWakeupWriteFlash = 0;
uint32_t NoDischargeCnt = 0;
uint8_t Save_Ldat_Address;
uint8_t Save_Event_Address;
//====Macro Typdef=============================
RECORDER_LDATA  RecorderLdat;
FET_STATE EventFetState;
//===Extern============

uint8_t EventDataBuf[10][33];
uint8_t Event500msFlag = 0;
uint8_t TimeStampFlag = 0;
uint8_t LdatLog_RegularTimeFlag = 0;
uint8_t EventLog_RegularTimeFlag = 0;
uint8_t EventNumber = 0;

uint8_t PowerOnOffFalg = 0;
uint8_t CAN_Rx_EventFlag = 0;
uint8_t BeforeCAN_Loss_Flag = 0;
uint8_t DischargeFlag = 0;
uint8_t Discharge30sFlag = 0;
uint8_t  Ldat_TemperatureAvg;         
uint8_t  Ldat_TemperatureMax;       
uint8_t  Ldat_TemperatureMin;
uint16_t BeforState = 0;
uint16_t BeforeIBatt = 0;
uint16_t Ldat_CellMax;
uint16_t Ldat_CellMin;

uint32_t EventLog_RegularTime = 0;
uint32_t FaultDataProcess = 0;

uint32_t EventTimeStampTotalCnt = 0;
uint32_t Ldat_ChargeEnergyTotal;
uint16_t Ldat_ChargeCurrentMax;
uint32_t Ldat_DischargeEnergyTotal;
uint16_t Ldat_DischargeCurrentMax;
uint16_t Ldat_InternalVoltageMax;        // default
uint16_t Ldat_InternalVoltageMin;        // default
uint16_t Ldat_DischargePowerMax;       // default
uint16_t Ldat_NDischargeEvents;        // default
uint16_t Ldat_NDeepDischargeEvent;     // default
uint16_t Ldat_DayReamingDay = 3723;
uint16_t Ldat_ShippingTotal;
uint32_t Ldat_TimeBrainsUpTotal;
uint8_t EraseAllFlag = 0;
FLASH_INDEX Event_Index1;
bool CanRxCmdFlag = false;
bool CAN_Loss_Flag = false;

void EventLog_Set_CANRxEvent(bool onoff)
{
    CAN_Rx_EventFlag = onoff;
}

void EventLog_Set_CANLoss(bool onoff)
{
    CAN_Loss_Flag = onoff;
}

bool EventLog_Get_CanRxCmd(void)
{
    return CanRxCmdFlag;
}

void EventLog_Set_CanRxCmd(bool onoff)
{
    CanRxCmdFlag = onoff;
}

uint16_t CHECKSUM(void *dataptr, uint16_t len)
{
  uint32_t acc;
  uint16_t src;
  uint8_t *octetptr;

  acc = 0;
  octetptr = (uint8_t*)dataptr;
  while (len > 1) {
    src = (*octetptr) << 8;
    octetptr++;
    src |= (*octetptr);
    octetptr++;
    acc += src;
    len -= 2;
  }
  if (len > 0) {
    src = (*octetptr) << 8;
    acc += src;
  }

  acc = (acc >> 16) + (acc & 0x0000ffffUL);
  if ((acc & 0xffff0000UL) != 0) {
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
  }

  src = (uint16_t)acc;
  return ~src;
}

void SaveEventHeadIndex()
{
    Event_Index1.lenth = CntEventLength;
    Event_Index1.timestamp = EventTimeStampTotalCnt;
    if(Save_Event_Address <= 1) {
        Save_Event_Address = 2;
        WriteMemoryData(&Event_Index1.arry[0], EVENT_HEAD_INDEX_ADR1, 8);
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR2);
    } else if(Save_Event_Address == 2) {
        Save_Event_Address = 1;
        WriteMemoryData(&Event_Index1.arry[0], EVENT_HEAD_INDEX_ADR2, 8);
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR1);
    }
}

void TimeAreaProcess()
{
    if(SysTime_msecIntervalElapsed(NoDischargeCnt, NO_DISCHARGE_5MIN)) {
        updateLdatData();
        if(Save_Ldat_Address <= 1) {
            WriteMemoryData(&RecorderLdat.arry[0], LDAT_MSG_START_ADR, LDAT_TOTAL_BYTES);
            Save_Ldat_Address = 2;
            Erase_Flash_SE(LDAT_MSG_START_ADR2);
        } else if(Save_Ldat_Address == 2) {
            WriteMemoryData(&RecorderLdat.arry[0], LDAT_MSG_START_ADR2, LDAT_TOTAL_BYTES);
            Save_Ldat_Address = 1;
            Erase_Flash_SE(LDAT_MSG_START_ADR);
        }
        NoDischargeCnt = SysTime_getMsec();//Save_Event_Address
    }
      
    if(Event500msFlag == 1) {
        Ldat_TimeBrainsUpTotal++;
        TimeStampFlag = 1;
        Event500msFlag = 0; // Clear 500mS Event
        EventTimeStampTotalCnt ++; // TimeStamp ++
        EventLog_RegularTime ++; // 2^22 Counter for regular 1
        RecorderLdat.TimeBrainsUpTotal ++;
        if(Calculation_SOC ==100) {
            RecorderLdat.TimeRestingTotal ++;
        }
        if(CanResponse30sCnt < 60) {
            CanResponse30sCnt ++;
        } else {
            CanResponse30sCnt = 60;
        }
        if(EventTimeStampTotalCnt >=0XFFFFFFFF) {
            EventTimeStampTotalCnt = 0;
        }
        if(EventLog_RegularTime > RECORDER_LDATA_TIME) {
            LdatLog_RegularTimeFlag = 1;
            EventLog_RegularTimeFlag = 1;
            EventLog_RegularTime = 0;
        }
    }
}

void ProcessEventData()
{
    uint8_t i,j,k;
    uint8_t finish = 0; 
    uint8_t data = 0;
    uint8_t value = 0;
    uint32_t time_stamp;

    if(CanResponse30sCnt >= 60) {     // CAN receive no ack arrive 30s clear ptr
       CAN_Rx_EventTotalCnts = CntEventLength; 
    }
    if(TimeStampFlag == 0) {      
        if(CanRxCmdFlag == 0)
            return;
    }
    
    TimeStampFlag = 0; 
//    updateLdatData();
    if(NoIntoEventRecentFlag > 0) {
        return; 
    }
    if(EraseAllFlag == 1) {
        EraseAllFlag = 0;
        CntEventLength = 0;
        CAN_Rx_EventTotalCnts = 0;
        EventTimeStampTotalCnt = 0;
        for(i = 0;i < 10;i++) {
            for(j = 0;j < 33;j ++) {
                EventDataBuf[i][j]  = 0;
            }
        }
    }
    UpdateFaultFlag(EventNumber);
    if(EventNumber > 9) {
        EventNumber = 10;
    } else {
        EventNumber ++;
    }
    
    if((EventNumber == 10) || (SaveFlashCmd == 1)) {// Receive CAN EventLog CMD.First Write Flash
        if(SaveFlashCmd == 1) {
            SaveFlashCmd = 0;
            CAN_DBC_Set_Saveflash(1);                
        }
        for(i = 0;i < EventNumber;i ++) {
            finish = 1;
            time_stamp = EventTimeStampTotalCnt - (9 - i);
            for(j = 0; j < 7; j++) {
                if(EventDataBuf[i][j] != 0) {
                    data = EventDataBuf[i][j];
                    for(k = 0; k < 8; k++) {
                        value = data & 0x01;
                        if(value) {
                            value = 0;
                            WriteFlashFunction(time_stamp, i, j, k);
                            CntEventLength++;
                        }
                        data = data >> 1;
                    }
                }
                EventDataBuf[i][j] = 0;
            }
        }
        EventNumber = 0;   
        SaveEventHeadIndex();
    }
}

void UpdateFaultFlag(uint8_t num)
{  
    uint16_t voltage = 0;
    uint32_t IBatt = 0;
    uint32_t i;
    uint8_t cnt = 0;
    uint8_t status1 = 0;
    uint8_t status2 = 0;

    uint8_t FaultLastData = 0;
    uint32_t temp_fault_data = 0;
    uint32_t temp_process_data = 0;
    uint32_t FaultDataAfter = 0;
    
    uint32_t data_calculate = 0;
    uint32_t FaultProcessData = 0;
    uint32_t sss = 0;

    temp_process_data &= 0x0003FFFF;        // 18cnts need to recover protected

    status1 = GetFETsState() & 0X03;
    status2 = EventFetState.data & 0X03;
    if(status1 != status2) {
       EventFetState.data <<= 4;
       EventFetState.data |= status1;
    }
    
    cnt = 4;
    while(cnt--) {
        FaultDataAfter <<= 8;//sysProtection
        FaultDataAfter |= sysProtection.fivebytes[cnt];
    }
    FaultLastData = sysProtection.fivebytes[4] & 0X0F;   
    
    if(FaultDataAfter != 0) {
        cnt = 4;
    }
    for(i = 0; i < 32; i++) {
        if(FaultDataAfter & 1) {
            if(i < 8) {
                sss = 1 << i;
                FaultProcessData |= sss;
            } else if((i >= 7) && (i <= 10)) {
                FaultProcessData |= 0x80;
            } else {
                sss = 1;
                sss <<= i;
                sss >>= 3;
                FaultProcessData |= sss;
            }
        }
        FaultDataAfter >>= 1;
    }
    sss = FaultLastData;
    FaultDataAfter |= (sss << 29);
    for(i = 0; i < 32; i++) {
        temp_fault_data = (FaultProcessData >> i) & 0x01;
        temp_process_data = (FaultDataProcess >> i) & 0x01;        
        if((temp_fault_data == 1) && (temp_process_data == 0)) {
            sss = 1;
            sss <<= i;
            FaultDataProcess |= sss;
            if(i < 8) {
                EventDataBuf[num][0] |= 0x01 << i;                  // set protected bit
                EventDataBuf[num][5] &= ~(0x01 << i);               // clear revover bit
            } else if(i < 16) {
                EventDataBuf[num][1] |= 0x01 << (i - 8);            // set protected bit
                EventDataBuf[num][6] &= ~(0x01 << (i - 8));         // clear revover bit
            } else if(i < 24) {
                EventDataBuf[num][2] |= 0x01 << (i - 16);           // set protected bit
            } else {
                EventDataBuf[num][3] |= 0x01 << (i - 24);           // set protected bit
            }
        } else if((temp_fault_data == 0) && (temp_process_data == 1)) {
            sss = 1;
            sss <<= i;
            data_calculate = ~sss;
            FaultDataProcess &= data_calculate;
            if(i < 8) {
                EventDataBuf[num][0] &= ~(0x01 << i);               // clear protected bit
                EventDataBuf[num][5] |= 0x01 << i ;                 // set recover bit
            } else if(i < 16) {
                EventDataBuf[num][1] &= ~(0x01 << (i - 8));         // clear protected bit
                EventDataBuf[num][6] |= 0x01 << (i - 8);            // set recover bit
            } else if(i < 24) {
                EventDataBuf[num][2] &= ~(0x01 << (i - 16));        // clear protected bit
            } else {
                EventDataBuf[num][3] &= ~(0x01 << (i - 24));        // clear protected bit
            }
        }
    }
    if(iBatt >= 0XFFFF) {
        IBatt = ~iBatt + 1;
    } else {
        if(iBatt < 0) {
            IBatt = 0X8000 - (~iBatt + 1);  // Discharge Netive
        } else if(iBatt > 0) {
            IBatt = 0X8000 + iBatt;         // Charge Postive
        } else {
            IBatt = 0X8000;                 // 0A
        }
    }
//=====Boot & CMD & State Converter=====================   
    if((PowerOnOffFalg == 0) && BMS_Get_Operating_Flag(WAKEUP)) {
        PowerOnOffFalg = 1;
        EventDataBuf[num][4] |= POWER_ON;                       // Enable POWER_ON
    } else if((PowerOnOffFalg == 1) && !BMS_Get_Operating_Flag(WAKEUP)) {
        PowerOnOffFalg = 0;
        EventDataBuf[num][4] |= POWER_OFF;                      // Enable POWER_OFF
    }
    
    if(BeforState != d_thisBmsState.currentState) {
        BeforState = d_thisBmsState.currentState;
        EventDataBuf[num][4] |= STATE_CHANGE;                   // Enable STATE_CHANGE
    }

    if(CAN_Rx_EventFlag == 1) {     
        CAN_Rx_EventFlag = 0;
        EventDataBuf[num][4] |= FOLLOW_CMD;                     // Enable FOLLOW_CMD
    }
    
    if(EventLog_RegularTimeFlag == 1) {     
        EventLog_RegularTimeFlag = 0;
        EventDataBuf[num][4] |= REGULAR1;                       // Enable REGULAR1
    }
    
    if(LdatLog_RegularTimeFlag == 1) {
        LdatLog_RegularTimeFlag = 0;
        EventDataBuf[num][4] |= REGULAR2;                       // Enable REGULAR2
    }
    //=== CAN Communication Loss ======================================== 
    if((CAN_Loss_Flag == 1) && (BeforeCAN_Loss_Flag == 0)) {    // CAN Communication Loss
		BeforeCAN_Loss_Flag = 1;
        EventDataBuf[num][4] |= CAN_LOSS;                        // Enable CAN_LOSS
    } else if((CAN_Loss_Flag == 0) && (BeforeCAN_Loss_Flag == 1)) {   		// CAN Communication Recovered
        BeforeCAN_Loss_Flag = 0;
        EventDataBuf[num][4] &= 0XFE;                        // Disable CAN_LOSS
        EventDataBuf[num][4] |= CAN_RECOVER;                        // Enable CAN_RECOVER
    }

//==Before State=1byte=========================
//    EventDataBuf[num][7]= d_thisBmsState.previousState;
//==After State=1byte==========================
//    EventDataBuf[num][8] = d_thisBmsState.currentState;
//==FET State (High Nibble: Before Low Nibble: After)=1byte
//    FetState = GetFETsState(); 
    EventDataBuf[num][9] = EventFetState.data;
//==CMD (2nd, 4th byte)=2bytes====================    
//    EventDataBuf[num][10] = 0X00;
//    EventDataBuf[num][11] = 0X00;
//==Vbatt=2bytes===================================
    voltage = (uint16_t)(vBat*100);
//    voltage = vBatTempData;
    EventDataBuf[num][12] = voltage & 0X00FF;               // Vbatt LSB
    EventDataBuf[num][13] = (voltage & 0XFF00) >> 8;        // Vbatt MSB        
//==Vbus=2bytes====================================
    voltage = (uint16_t)(vBus * 100);
    EventDataBuf[num][14] = voltage & 0X00FF;               // Vbus LSB
    EventDataBuf[num][15] = (voltage & 0XFF00) >> 8;        // Vbus MSB  
//==Vmax_Cell=2bytes===============================
    EventDataBuf[num][16] = (uint16_t)cellVmax & 0X00FF;               // cellVmax LSB
    EventDataBuf[num][17] = ((uint16_t)cellVmax & 0XFF00) >> 8;        // cellVmax MSB                  
//==Vavg_Cell=2bytes===============================    
    voltage = (cellVmax + cellVmin)/2;
    EventDataBuf[num][18] = voltage & 0X00FF;               // cellVavg LSB
    EventDataBuf[num][19] = (voltage & 0XFF00) >> 8;        // cellVavg MSB    
//==Vmin_Cell=2bytes===============================
    EventDataBuf[num][20] = (uint16_t)cellVmin & 0X00FF;              // cellVmin LSB
    EventDataBuf[num][21] = ((uint16_t)cellVmin & 0XFF00) >> 8;       // cellVmin MSB            
//==Ibatt_before=2bytes===============================
    EventDataBuf[num][22] = BeforeIBatt & 0X00FF;           // cellVmin LSB
    EventDataBuf[num][23] = (BeforeIBatt & 0XFF00) >> 8;    // cellVmin MSB 
//==Ibatt_after=2bytes================================
    EventDataBuf[num][24] = IBatt & 0X00FF;                 // iBatt LSB
    EventDataBuf[num][25] = (IBatt & 0XFF00) >> 8;          // iBatt MSB 
//==SOH (%)=1byte================================
    EventDataBuf[num][26] = EventSOH;
//==RSOC(%)=1byte================================
    EventDataBuf[num][27] = Calculation_SOC;
//==Full Charge Capacity (FCC) (WH)=2bytes=======
    EventDataBuf[num][28] = Gague_FCC & 0X00FF;
    EventDataBuf[num][29] = (Gague_FCC & 0XFF00) >> 8;
//==Avg Temp © 0C = 40=1byte=====================
    EventDataBuf[num][30] = CellTempature_Avg;
//==FET Temp © 0C = 40=1byte=====================
    EventDataBuf[num][31] = FetTempature;
//==Cell Temp © 0C = 40=1byte====================
    EventDataBuf[num][32] = CellTempature;//0.1 dgree k

     if(BeforeIBatt != IBatt) {
        BeforeIBatt = IBatt;
     }
}

/*******************************************************************************
* Function Name  : CurPowerCalculation
* Description    : Calculate current power energy Process
* Input          : None
* Return         : None
*******************************************************************************/
void updateLdatData()
{    
    if((DischargeFlag == 1) && (Discharge30sFlag == 0)) {
        Discharge30sFlag = 1;
        Ldat_NDischargeEvents ++;
    }
    if(Ldat_CellMin == 0) {
        Ldat_CellMin = cellVmin;
    }
    if(Ldat_CellMax < cellVmax) {
        Ldat_CellMax = cellVmax;
    }
    if(Ldat_CellMin > cellVmin) {
        Ldat_CellMin = cellVmin;
    }
    if(Ldat_TemperatureMax < CellTempature_Max) {
        Ldat_TemperatureMax = CellTempature_Max;
    }
    if(Ldat_TemperatureMin > CellTempature_Min) {
        Ldat_TemperatureMin = CellTempature_Min;
    }
    Ldat_TemperatureAvg = CellTempature_Avg;
    RecorderLdat.DaysReamingLifeExpired = Ldat_DayReamingDay;
//    =======LDATA Information==========================
//==Timestamp(0.5 second)=4bytes======
    RecorderLdat.Timestamp = EventTimeStampTotalCnt;
//==ID(hex)=2bytes====================
    RecorderLdat.ID = 0X0001;   
//== SG_ LDAT_ChargeEnergy_Total=4bytes=====
    RecorderLdat.ChargerEnergyTotal = Ldat_ChargeEnergyTotal;
//===SG_ LDAT_ChargeCurrent_Max=2bytes=======
    RecorderLdat.ChargeCurrentMax = Ldat_ChargeCurrentMax;
//==SG_ LDAT_DischargeEnergy_Total=4bytes====
    RecorderLdat.DischargeEnergyTotal = Ldat_DischargeEnergyTotal;
//==SG_ LDAT_DischargeCurrent_Max=2bytes=====  
    RecorderLdat.DischargeCurrentMax = Ldat_DischargeCurrentMax;
//==SG_ LDAT_InternalVoltage_Max=2bytes=====  
    RecorderLdat.InternalVoltageMax = Ldat_InternalVoltageMax;
//==SG_ LDAT_InternalVoltage_Min=2bytes======
    RecorderLdat.InternalVoltageMin = Ldat_InternalVoltageMin;
//==SG_ LDAT_DischargePower_Max=2bytes=======
    RecorderLdat.DischargePowerMax = Ldat_DischargePowerMax;
//==SG_ LDAT_NDischargeEvents=2bytes=========//M7 The number of discharges that last longer than 30 seconds.
    RecorderLdat.NDischargeEvents = Ldat_NDischargeEvents;
//==SG_ LDAT_NDeepDischargeEvents=2bytes=====
    RecorderLdat.NDeepDischargeEvent = Ldat_NDeepDischargeEvent;
//==SG_ LDAT_SOHInternal=1byte===============
    RecorderLdat.InternalSOH = EventSOH;
//==SG_ LDAT_Temperature_Avg=1byte=========== , 
    RecorderLdat.TemperatureAvg = Ldat_TemperatureAvg;
//==SG_ LDAT_Temperature_Max=1byte===========
    RecorderLdat.TemperatureMax = Ldat_TemperatureMax;
//==SG_ LDAT_Temperature_Min=1byte===========
    RecorderLdat.TemperatureMin = Ldat_TemperatureMin;  
    RecorderLdat.DCIRString = Ldat_DCIRString;
    RecorderLdat.DCIRStringMin = Ldat_DCIRStringMin;    
    RecorderLdat.DaysReamingLifeExpired = Ldat_DayReamingDay;
    
    if(DISCHARGE_TimeS >= LDAT_DISCHARGING_MAX_TIME) {
        RecorderLdat.TimeDischargingTotal = 0;
        DISCHARGE_TimeS = 0;
    }
    if(DISCHARGE_TimeS >= LDAT_CHARGING_MAX_TIME) {
        RecorderLdat.TimeChargingTotal = 0;
        CHARGE_TimeS = 0;
    }    
    if(IDLE_TimeS >= LDAT_CHARGING_MAX_TIME) {
        RecorderLdat.TimeRestingTotal = 0;
        IDLE_TimeS = 0;
    }
    if(Ldat_TimeBrainsUpTotal >= LDAT_CHARGING_MAX_TIME) {
        RecorderLdat.TimeBrainsUpTotal = 0;
        Ldat_TimeBrainsUpTotal = 0;
    }
    if(Ldat_ShippingTotal >= LDAT_CHARGING_MAX_TIME) {
        RecorderLdat.ShippingTotal = 0;
        Ldat_ShippingTotal = 0;
    }
}

void WriteFlashFunction(uint32_t time_lable, uint8_t head, uint8_t sub_matrix, uint8_t sub_bit)
{
    uint8_t sentbuf[6];
    uint8_t table_number = (sub_matrix * 8) + sub_bit;
    uint16_t code_num = 0;
    uint32_t addr = 0;
    
//    DebugEventActionFlag = 1;
    code_num = ErrorCodeTable[table_number];
//    DebugEventID = code_num;
//    DebugIndex = CntEventLength;
    sentbuf[0] = time_lable & 0X000000FF;     // LSB
    sentbuf[1] = (time_lable & 0X0000FF00) >> 8;
    sentbuf[2] = (time_lable & 0X00FF0000) >> 16;
    sentbuf[3] = (time_lable & 0XFF000000) >> 24; 
    sentbuf[4] = code_num & 0X00FF;
    sentbuf[5] = (code_num & 0XFF00) >> 8;    
    
    if(table_number == 34) {        //34=>PowerON, 35=>powerOFF, 36=>state change, 37=>cmd,
        EventDataBuf[head][7] = 0XFF;
        EventDataBuf[head][8] = 0X00;
        EventDataBuf[head][10] = 0X00;
        EventDataBuf[head][11] = 0X00;          
    } else if(table_number == 35) {     // Power OFF
        EventDataBuf[head][7] = 0X00;
        EventDataBuf[head][8] = 0XFF;
        EventDataBuf[head][10] = 0X00;
        EventDataBuf[head][11] = 0X00;           
    } else if(table_number == 37) {
        EventDataBuf[head][7]= d_thisBmsState.previousState; 
        EventDataBuf[head][8] = d_thisBmsState.currentState; 
        EventDataBuf[head][10] = 0X01;
        EventDataBuf[head][11] = 0XFF;        
    } else {
        EventDataBuf[head][7]= d_thisBmsState.previousState;      
        EventDataBuf[head][8] = d_thisBmsState.currentState; 
        EventDataBuf[head][10] = 0X00;
        EventDataBuf[head][11] = 0X00;    
    }
    
    addr = EVENT_MSG_START_ADR + (CntEventLength * 32);
    WriteMemoryData(&sentbuf[0], addr, 6);
    addr = addr + 6;
    WriteMemoryData(&EventDataBuf[head][7], addr, 26); 
    addr = addr- 6;
    addr = addr + 32;
    WATCHDOG_TimerClear();
}

bool Check_EventLog_Lable()
{
    bool ret = true;
    uint8_t CheckData1[5];
    uint8_t i;

    sFLASH_ReadBuffer((uint8_t *)&CheckData1, EVENT_LABEL1_ADR, 5);
    printf("(R)EventLog_Lable:");
    for(i = 0; i < 5; i ++) {
        printf("%c",CheckData1[i]);
        if(EventString[i] != CheckData1[i]) {
            ret = false;
        }
    }
    printf("\r\n");
    return ret;
}

void Write_EventLog_Lable(void)
{
    sFLASH_WriteBuffer((uint8_t *)&EventString, EVENT_LABEL1_ADR, 5);
}

void WriteMemoryData(uint8_t *ptrData, uint32_t address, uint16_t length)
{
//    uint32_t TimeOut = SysTime_getMsec();
//    uint8_t FlashState = 0;
    
    sFLASH_WriteBuffer((uint8_t *)ptrData, address, length);
    
    
//    MX25L_Flash_Write_Enable();
//    MX25L_Flash_Write(ptrData, address, length);
//    FlashState = MX25L_FLASH_Status() & 0x03;
//    TimeOut = SysTime_getMsec();   
//    while(FlashState == 0x03){
//        FlashState = MX25L_FLASH_Status() & 0x03;
//        if(SysTime_msecIntervalElapsed(TimeOut, 10000))
//             FlashState = 0;                    
//    }     
}

uint8_t MoveFlashData(uint32_t adr)
{
    uint8_t i;
    uint8_t Checksum = 0XFF;
    uint8_t HeadIndex[8];
    
    MX25L_Flash_Read(&HeadIndex[0], adr, 8);
    for(i = 0; i < 8; i++) {
        Checksum &= HeadIndex[i];
    }
    if(Checksum == 0XFF) {
       Event_Index1.lenth = 0;
       Event_Index1.timestamp = 0;           
    }

    for(i = 0; i < 4; i++) {
        if(i == 0) {
            Event_Index1.lenth = HeadIndex[3];
            Event_Index1.timestamp = HeadIndex[7];
        } else {
            Event_Index1.lenth |= HeadIndex[3 - i];
            Event_Index1.timestamp |= HeadIndex[7 - i];
        }
        
        if(i < 3) {
            Event_Index1.lenth <<= 8;   
            Event_Index1.timestamp <<= 8;
        }
    }
//    Addr = EVENT_MSG_START_ADR +Event_Index1.lenth *32;
//    MX25L_Flash_Read();
    return Checksum;
}

void Get_Event_Index()
{
    EraseAllFlag = 0;
    
    if(MoveFlashData(EVENT_HEAD_INDEX_ADR1) != 0XFF) {
        Save_Event_Address = 2;
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR2);
    } else if(MoveFlashData(EVENT_HEAD_INDEX_ADR2) != 0XFF) {
        Save_Event_Address = 1;
        Erase_Flash_SE(EVENT_HEAD_INDEX_ADR1);
    } else {
        Save_Event_Address = 1;
        Event_Index1.lenth = 0;
        Event_Index1.timestamp = 0;         
    }
    CntEventLength = Event_Index1.lenth;
}

void ReadLdataData()
{
    uint8_t i;
    
    for(i = 0; i < LDAT_TOTAL_BYTES; i++) {
        RecorderLdat.arry[i] = life_data[i];
    }
    EventTimeStampTotalCnt = RecorderLdat.Timestamp;
    Ldat_ChargeEnergyTotal = RecorderLdat.ChargerEnergyTotal;
    Ldat_ChargeCurrentMax = RecorderLdat.ChargeCurrentMax;
    Ldat_DischargeEnergyTotal = RecorderLdat.DischargeEnergyTotal;
    Ldat_DischargeCurrentMax = RecorderLdat.DischargeCurrentMax;
    Ldat_InternalVoltageMax = RecorderLdat.InternalVoltageMax;
    Ldat_InternalVoltageMin = RecorderLdat.InternalVoltageMin; 
    Ldat_DischargePowerMax = RecorderLdat.DischargePowerMax;
    Ldat_NDischargeEvents = RecorderLdat.NDischargeEvents;
    Ldat_NDeepDischargeEvent = RecorderLdat.NDeepDischargeEvent;
    Ldat_TemperatureAvg = RecorderLdat.TemperatureAvg;
    Ldat_TemperatureMax = RecorderLdat.TemperatureMax;
    Ldat_TemperatureMin = RecorderLdat.TemperatureMin;
    DISCHARGE_TimeS = RecorderLdat.TimeDischargingTotal;   
    CHARGE_TimeS = RecorderLdat.TimeChargingTotal; 
    IDLE_TimeS = RecorderLdat.TimeRestingTotal;
    Ldat_TimeBrainsUpTotal = RecorderLdat.TimeBrainsUpTotal;   
    Ldat_DCIRString = RecorderLdat.DCIRString;
    Ldat_DCIRStringMin = RecorderLdat.DCIRStringMin;
    Ldat_ShippingTotal = RecorderLdat.ShippingTotal;
    Ldat_DayReamingDay = RecorderLdat.DaysReamingLifeExpired;
    Ldat_CellMax = RecorderLdat.CellVMax;      
    Ldat_CellMin = RecorderLdat.CellVMin;    
}

void ClearLdatData()
{
    EventTimeStampTotalCnt = 0;
    Ldat_ChargeEnergyTotal = 0;
    Ldat_ChargeCurrentMax = 0;
    Ldat_DischargePowerMax = 0;
    Ldat_DischargeEnergyTotal = 0;
    Ldat_DischargeCurrentMax = 0;
    Ldat_InternalVoltageMin = (uint16_t)vBat *10;
    Ldat_InternalVoltageMax = (uint16_t)vBat *10;
    Ldat_NDischargeEvents = 0;
    Ldat_NDeepDischargeEvent = 0;
    Ldat_DayReamingDay = 3723;
    Ldat_DCIRStringMin = 0XFFFF;
    Ldat_ShippingTotal = 1;
    Ldat_TemperatureAvg = 65;
    Ldat_TemperatureMax = 65;
    Ldat_TemperatureMin = 65;
    RecorderLdat.Discharge0Counter = 0;
    RecorderLdat.Discharge20Counter = 0;
    RecorderLdat.Discharge40Counter = 0;
    RecorderLdat.Discharge60Counter = 0;
    RecorderLdat.Discharge80Counter = 0;
    DISCHARGE_TimeS = 0;
    RecorderLdat.TimeDischargingTotal = 0;
    RecorderLdat.TimeChargingTotal = 0;
    CHARGE_TimeS = 0;
    RecorderLdat.TimeRestingTotal = 0;
    IDLE_TimeS = 0;	
    RecorderLdat.TimeBrainsUpTotal = 0;
    Ldat_TimeBrainsUpTotal = 0;		
    Ldat_CellMin = 3500;
    RecorderLdat.CellVMin = Ldat_CellMin;
    Ldat_CellMax = 3500;
    RecorderLdat.CellVMax = Ldat_CellMax;    
}

void Get_Ldat_Index(void)
{
    uint8_t i ;
    uint8_t checksum = 0XFF;
    
    MX25L_Flash_Read(&life_data[0], LDAT_MSG_START_ADR, LDAT_TOTAL_BYTES); 
    for(i = 0; i < LDAT_TOTAL_BYTES; i++) {
        checksum = checksum & life_data[i];
    }
    if(checksum != 0XFF) {
        Save_Ldat_Address = 2;
        ReadLdataData();
    } else {
        MX25L_Flash_Read(&life_data[0], LDAT_MSG_START_ADR2, LDAT_TOTAL_BYTES); 
        for(i = 0; i < LDAT_TOTAL_BYTES; i++) {
            checksum = checksum & life_data[i];
        }
        if(checksum != 0XFF) {
            Save_Ldat_Address = 1;
            ReadLdataData();
        } else {
            Save_Ldat_Address = 0;
            ClearLdatData();
        }
    }
}

void Erase_Flash_SE(uint32_t erase_Address)
{
    uint32_t TimeOut = SysTime_getMsec();
    uint8_t FlashState = 0;    
    
    MX25L_Flash_Write_Enable();
    MX25L_FLASH_Erase(erase_Address ,COM_SE);        // To Erease the selected 4K block   
    FlashState = MX25L_FLASH_Status() & 0x03;
    TimeOut = SysTime_getMsec();
    while(FlashState == 0x03) {
        FlashState = MX25L_FLASH_Status() & 0x03;
        if(SysTime_msecIntervalElapsed(TimeOut, 3000))
             FlashState = 0;                    
    }
}

void Erase_Flash(uint32_t erase_Address, uint8_t size)
{
    uint8_t i = 0;
    uint8_t FlashState = 0;
    uint32_t TimeOut = SysTime_getMsec();   
    uint32_t EraseAddress = erase_Address;    
    
    for(i = 0; i < size; i++) {                          // Erase 2M =32*64kb
        MX25L_Flash_Write_Enable();
        MX25L_FLASH_Erase(EraseAddress ,COM_BE);        // To Erease the selected 64K block 
        FlashState = MX25L_FLASH_Status() & 0x03;
        TimeOut = SysTime_getMsec(); 
        while(FlashState == 0x03) {
            FlashState = MX25L_FLASH_Status() & 0x03;
            if(SysTime_msecIntervalElapsed(TimeOut, 6000)) {
                 FlashState = 0;
            }
        }
        
        EraseAddress = EraseAddress + 0X10000;                         // address+64k
    }      
}

void EVENTLOG_BATTERY_MANAGE(void)
{
#if LOG_BATTERY_MANAGE
    int i;
    
    printf("\n\r");
    printf("d_smbusCmdIdx:0x%02X \n\r", d_smbusCmdIdx);
    printf("VBATT_BUS:%3.2fV VBATT:%3.2fV \n\r", (double)vBus, (double)vBat);
    if(iBatt > 0) {
        printf("\e[1;32m IBATT:+%ldmA/%ldmS RC:%ldmAh \033[0m \n\r",iBatt ,CHARGE_TimeS ,RC_calculated_gain);
    } else if(iBatt < 0) {
        printf("\e[0;31m IBATT:%ldmA/%ldmS RC:%ldmAh \033[0m \n\r",iBatt ,DISCHARGE_TimeS ,RC_calculated_gain);
    } else {
        printf("IBATT:%ldmA/%ldmS \n\r",iBatt ,IDLE_TimeS);
    }
//    printf("SOC:%d%% FCC:%dWH RC:%dWH Remain Time:%dS \n\r", Out_SOC, Out_FCC, Out_RC, RemainTimeCnt);
    printf("SOC:%d%% FCC:%dWH RC:%ldWH Remain Time:%dS \n\r", rsoc, dsgFCC, uiCalRCapacity, RemainTimeCnt);
    printf("K_VALUE: ");
    for(i = 0;i < 14;i++) {
        printf("%d ",K_VALUE_STATE.K_VALUE[i]);
    }
    printf("\n\r");
//    printf("K1803_5: ");
//    for(i = 0;i < 14;i++) {
//        printf("%d ",kVal5oC1803[i]);
//    }
//    printf("\n\r");
    printf("K_5oC_VALUE: ");
    for(i = 0;i < 14;i++) {
        printf("%d ",K_VALUE_STATE.K_5oC_VALUE[i]);
    }
    printf("\n\r");
    printf("RC_MAX:%d%% RC_MIN:%d%% SOH:%d \n\r", RC_MAX, RC_MIN, SMBUS_stateOfHeath[0]);
    printf("SMBUS GaugeSOC:%d%% getGaugeFCC:%d getGaugeRC:%d \n\r", Gague_SOC, Gague_FCC,  Gague_RC);
    printf("cellVmax:%3.2fV cellVmin:%3.2fV cellVavg:%3.2fV \n\r", (double)cellVmax, (double)cellVmin, (double)cellVavg);
    printf("cellTmax:%dC cellTemp:%dC cellTmin:%dC \n\r", CellTempature_Max, CellTempature_Min, CellTempature);
    printf("DEVICE_NUMBER:0x%X \n\r", d_deviceAddr);
    if(BMS_Get_Operating_Flag(WAKEUP)) {
        printf("\e[1;32m WAKEUP \033[0m \n\r");
    } else {
        printf("\e[0;31m NO WAKEUP \033[0m \n\r");
    }
    printf("BMSState:0x%02X \n\r", d_thisBmsState.currentState);
    
    if(HWA_76200_DFET()) {
        printf("\e[0;31mN_DFET \033[0m");
    } else {
        printf("\e[1;32mDFET \033[0m");
    }
    if(HWA_76200_CFET()) {
        printf("\e[0;31m N_CFET \033[0m");
    } else {
        printf("\e[1;32m CFET \033[0m");
    }
    if(HWA_TRICKLE_CHRG()) {
        printf("\e[1;32m TRICKLE_CHRG");
    } else {
        printf("\e[0;31m N_TRICKLE_CHRG");
    }
    if(HWA_TRICKLE_CHRG_MODE()) {
        printf("_FAST \033[0m");
    } else {
        printf("_NORMAL \033[0m");
    }
    printf("\n\r");
    
    printf("FET Temperature:%dC \n\r", FetTempature);
    EVENTLOG_LED();
    if(CAN_DBC_Get_UPS_CTRL()) {
        printf("CAN CTRL\n\r");
    }
#endif
}

void EVENTLOG_PROTECTION(void)
{
#if LOG_PROTECTION
    if(sysProtection.MOSsState & 0x08) {
        printf("\e[0;31m FET \033[0m");
    }
    
    if(sysProtection.COVBit) {
        printf("\e[0;31m COV \033[0m");
    }
    
    if(sysProtection.POVBit) {
        printf("\e[0;31m POV \033[0m");
    }
    
    if(sysProtection.EOCVBit) {
        printf("\e[0;31m EOCV \033[0m");
    }
    
    if(sysProtection.CUVBit) {
        printf("\e[0;31m CUV \033[0m");
    }
    
    if(sysProtection.PUVBit) {
        printf("\e[0;31m PUV \033[0m");
    }
    
    if(sysProtection.OIMBVBit) {
        printf("\e[0;31m OIMBV \033[0m");
    }
    
    if(sysProtection.OCIBit) {
        printf("\e[0;31m OCI \033[0m");
    }
    
    if(sysProtection.ODI_1Bit) {
        printf("\e[0;31m ODI_1 \033[0m");
    }
    
    if(sysProtection.ODI_2Bit) {
        printf("\e[0;31m ODI_2 \033[0m");
    }
    
    if(sysProtection.ODI_3Bit) {
        printf("\e[0;31m ODI_3 \033[0m");
    }
    
    if(sysProtection.ODI_4Bit) {
        printf("\e[0;31m ODI_4 \033[0m");
    }
    
    if(sysProtection.H_ODIBit) {
        printf("\e[0;31m H_ODI \033[0m");
    }
    
    if(sysProtection.H_SCIBit) {
        printf("\e[0;31m H_SCI \033[0m");
    }
    
    if(sysProtection.COTBit) {
        printf("\e[0;31m COT \033[0m");
    }
    
    if(sysProtection.DOTBit) {
        printf("\e[0;31m DOT \033[0m");
    }
    
    if(sysProtection.CUTBit) {
        printf("\e[0;31m CUT \033[0m");
    }
    
    if(sysProtection.DUTBit) {
        printf("\e[0;31m DUT \033[0m");
    }
    
    if(sysProtection.MOTBit) {
        printf("\e[0;31m MOT \033[0m");
    }
    
    if(sysProtection.UVPFBit) {
        printf("\e[0;31m UVPF \033[0m");
    }
    
    if(sysProtection.OVPFBit) {
        printf("\e[0;31m OVPF \033[0m");
    }
    
    if(sysProtection.OTPFBit) {
        printf("\e[0;31m OTPF \033[0m");
    }
    
    if(sysProtection.CIMPFBit) {
        printf("\e[0;31m CIMPF \033[0m");
    }
    
    if(sysProtection.THOPFBit) {
        printf("\e[0;31m THOPF \033[0m");
    }
    
    if(sysProtection.THSPFBit) {
        printf("\e[0;31m THSPF \033[0m");
    }
    
    if(sysProtection.CFETPFBit) {
        printf("\e[0;31m CFETPF \033[0m");
    }
    
    if(sysProtection.DFETPFBit) {
        printf("\e[0;31m DFETPF \033[0m");
    }
    
    if(sysProtection.ULP_BQ_F) {
        printf("\e[0;31m BQ_F \033[0m");
    }
    printf("\n\r");
#endif    
}

void EVENTLOG_CAN_Receive(CAN_MSG_OBJ *pDestBuf)
{
#if LOG_CAN_Receive
    int i;
    
    printf("R_CAN_ID:0x%03lX", pDestBuf->msgId);
    printf(" DLC:%d DATA:", pDestBuf->field.dlc);
    for(i=0; i<pDestBuf->field.dlc; i++) {
        printf("0x%02X ", pDestBuf->data[i]);
    }
    printf("\n\r");    
#endif
}

#if 1
void EVENTLOG_CAN_Transmit(uCAN_MSG *pDestBuf)
{
#if LOG_CAN_Transmit
    int i;
    
    printf("T_CAN_ID:0x%03lX", pDestBuf->frame.id);
    printf(" DLC:%d DATA:", pDestBuf->frame.dlc);
    for(i=0; i<pDestBuf->frame.dlc; i++) {
        printf("0x%02X ", pDestBuf->array[7 + i]);
    }
    printf("\n\r");
#endif
}
#else
void EVENTLOG_CAN_Transmit(CAN_MSG_OBJ *pDestBuf)
{
#if LOG_CAN_Transmit
    int i;
    
    printf("T_CAN_ID:0x%03lX", pDestBuf->msgId);
    printf(" DLC:%d DATA:", pDestBuf->field.dlc);
    for(i=0; i<pDestBuf->field.dlc; i++) {
        printf("0x%02X ", pDestBuf->data[i]);
    }
    printf("\n\r");
#endif
}
#endif
void EVENTLOG_CFET_DFET_TRICKLE(void)
{
#if LOG_CFET_DFET_TRICKLE
    if(HWA_76200_DFET()) {
        printf("\e[0;31mN_DFET \033[0m");
    } else {
        printf("\e[1;32mDFET \033[0m");
    }
    if(HWA_76200_CFET()) {
        printf("\e[0;31m N_CFET \033[0m");
    } else {
        printf("\e[1;32m CFET \033[0m");
    }
    if(HWA_TRICKLE_CHRG()) {
        printf("\e[1;32m TRICKLE_CHRG");
    } else {
        printf("\e[0;31m N_TRICKLE_CHRG");
    }
    if(HWA_TRICKLE_CHRG_MODE()) {
        printf("_FAST \033[0m");
    } else {
        printf("_NORMAL \033[0m");
    }
    printf("\n\r");
#endif
}

void EVENTLOG_BATTERY_STATE_CHANG(void)
{
#if LOG_BATTERY_STATE_CHANG
    uint16_t State_old;
    uint16_t State_new;
    
    State_old = d_thisBmsState.previousState;
    State_new = d_thisBmsState.currentState;

    
    printf("BMSState:%d", State_old);
    printf("-> BMSState:%d", State_new);
    printf("\n\r");
#endif
}

void EVENTLOG_SOH(void)
{
#if LOG_SOH
    printf("\r\n");
    printf("SOH:%d%% ", SMBUS_getStateOfHealth());
    printf("DEEP DISCHARGE TIME:%d ", SOH_Get_DeepDISChargeCycle());
    printf("RemainingCapacity:%dWH ", SOH_Get_TotalCoulomb());
    printf("\n\r");
//    printf("VBATT_BUS:%3.2fV VBATT:%3.2fV \n\r", (double)vBus, (double)vBat);
//    printf("IBATT:%ldmA ",iBatt);
//    printf("IDLE_TimeS:%.2fS ", (IDLE_TimeS / 1000.0));
//    printf("CHARGE_Time:%.2fS ", (CHARGE_TimeS / 1000.0));
//    printf("DISCHARGE_Time:%.2fS ", (DISCHARGE_TimeS / 1000.0));
//    printf("\n\r");
//    printf("USABLE_BATTERY_CAPACITY_mWh:%ldmWH ", USABLE_BATTERY_CAPACITY_mWS /60 /60);
//    printf("USABLE_BATTERY_CAPACITY_Wh:%ldWH ", USABLE_BATTERY_CAPACITY_WS / 60 / 60);
    printf("uiCalRCapacityWH:%ldWH ", uiCalRCapacityWH);
    printf("uiCalRCapacity100mWS:%ldmWS ", BMS_Get_uiCalRCapacity100mWS());
    printf("\n\r");
    printf("RC_operation(%ld) \r\n",RC_operation);
#endif
}

void EVENTLOG_LED(void)
{
#if LOG_LED
    if(d_flagRedLedSolidCtrl) {
        printf("Red Solid ");
    } else if(d_flagRedLedBlinkCtrl) {
        printf("Red Blink ");
    } else {
        printf("Red OFF ");
    }

    if(d_flagRedLedSolidCtrl) {
        printf("Green Solid");
    } else if(d_flagRedLedBlinkCtrl) {
        printf("Green Blink");
    } else {
        printf("Green OFF");
    }
    
    printf("\n\r");
#endif
}

#endif
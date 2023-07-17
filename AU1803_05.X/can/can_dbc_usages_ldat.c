/******************************************************************************
 *
 * FILE: can_dbc_usages_cat0.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/
#include "can.h"
#include "../EventLogData/EventLog.h"

uint16_t Ldat_ChargePowerMax = 0;

bool BatteryManageSystem_LDAT_Frames 
    (CanDbcMemMapUsageFuncParam_t *pUsageParam)
{
    UINT8 *pCanFrame;
    UINT8 i;
	UINT8 data;
    UINT32 doubleTime = 0;
    
    if (pUsageParam->frameIdx >= DCB_UIT[DCB_CMD_NAME_LDAT].startFrameNum + DCB_UIT[DCB_CMD_NAME_LDAT].totalFrameNum) {
        return FALSE;
    }
    for (i = 0; i < DCB_UIT[DCB_CMD_NAME_LDAT].totalFrameNum; i++) { //TS1,TS2,TS3,Cell,FET,Gauge
        pCanFrame = CanDCB_dataFramePtr[pUsageParam->frameIdx+i].ptrFrame;
        switch (i) {
            case  0: //frame M=1
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x01;
                pCanFrame[1] = Ldat_DischargeEnergyTotal & 0X000000FF;
                pCanFrame[2] = (Ldat_DischargeEnergyTotal & 0X0000FF00) >> 8;
                pCanFrame[3] = (Ldat_DischargeEnergyTotal & 0X00FF0000) >> 16;
                pCanFrame[4] = (Ldat_DischargeEnergyTotal & 0XFF000F00) >> 24;
                pCanFrame[5] = Ldat_CellMin & 0XFF;
                data = Ldat_CellMin >> 8;
                pCanFrame[6] = data & 0X0F;
                data = Ldat_CellMax & 0x0F;
                pCanFrame[6] |= (data<<4);
                data = (Ldat_CellMax & 0X0FF0)>> 4;
                pCanFrame[7] = data;
                break;
            case  1: //frame M=2
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x02;
                pCanFrame[1] = RecorderLdat.ChargerEnergyTotal & 0X000000FF;
                pCanFrame[2] = (RecorderLdat.ChargerEnergyTotal & 0X0000FF00) >> 8;
                pCanFrame[3] = (RecorderLdat.ChargerEnergyTotal & 0X00FF0000) >> 16;
                pCanFrame[4] = (RecorderLdat.ChargerEnergyTotal & 0XFF000000) >> 24;
                pCanFrame[5] = (Ldat_InternalVoltageMin & 0X00FF);
                pCanFrame[6] = (Ldat_InternalVoltageMin & 0X0F00) >> 8;
                data = (Ldat_InternalVoltageMax & 0x000F);
                pCanFrame[6] |= (data << 4);
                data = (Ldat_InternalVoltageMax & 0X0FF0) >> 4;
                pCanFrame[7] = data;      
                break;
            case  2: //frame M=3 
                doubleTime = DISCHARGE_TimeS >> 1;
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x03;
                pCanFrame[1] = doubleTime & 0X000000FF;
                pCanFrame[2] = (doubleTime & 0X0000FF00) >> 8;
                pCanFrame[3] = (doubleTime & 0X00FF0000) >> 16;
                pCanFrame[4] = (doubleTime & 0XFF000000) >> 24;
                pCanFrame[5] = Ldat_DischargeCurrentMax & 0X00FF;
                pCanFrame[6] = (Ldat_DischargeCurrentMax & 0XFF00) >> 8;
                pCanFrame[7] = Ldat_TemperatureMax;        
                break;
            case  3: //frame M=4
                doubleTime = CHARGE_TimeS >> 1;
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x04;  
                pCanFrame[1] = doubleTime & 0X000000FF;
                pCanFrame[2] = (doubleTime & 0X0000FF00) >> 8;
                pCanFrame[3] = (doubleTime & 0X00FF0000) >> 16;
                pCanFrame[4] = (doubleTime & 0XFF000000) >> 24;
                pCanFrame[5] = Ldat_ChargeCurrentMax & 0X00FF;
                pCanFrame[6] = (Ldat_ChargeCurrentMax & 0XFF00) >> 8;
                pCanFrame[7] = Ldat_TemperatureMin;      
                break;
            case  4: //frame M=5
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x05; 
                pCanFrame[1] = IDLE_TimeS & 0X000000FF;
                pCanFrame[2] = (IDLE_TimeS & 0X0000FF00) >> 8;
                pCanFrame[3] = (IDLE_TimeS & 0X0000FF00) >> 16;
                pCanFrame[4] = (IDLE_TimeS & 0X0000FF00) >> 24;
                pCanFrame[5] = Ldat_ChargePowerMax & 0X00FF;
                pCanFrame[6] = (Ldat_ChargePowerMax & 0XFF00) >> 8;
                pCanFrame[7] = Ldat_TemperatureAvg; 
                break;
            case  5: //frame M=6
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x06; 
                pCanFrame[1] = Ldat_TimeBrainsUpTotal & 0X000000FF;
                pCanFrame[2] = (Ldat_TimeBrainsUpTotal & 0X0000FF00) >> 8;
                pCanFrame[3] = (Ldat_TimeBrainsUpTotal & 0X0000FF00) >> 16;
                pCanFrame[4] = (Ldat_TimeBrainsUpTotal & 0X0000FF00) >> 24;
                pCanFrame[5] = Ldat_DischargePowerMax & 0X00FF;
                pCanFrame[6] = (Ldat_DischargePowerMax & 0XFF00) >> 8;
                pCanFrame[7] = 0X00;    
                break;
            case  6: //frame M=7
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x07;   
                pCanFrame[1] = Ldat_NDischargeEvents & 0X00FF;
                pCanFrame[2] = (Ldat_NDischargeEvents & 0XFF00) >> 8;
                pCanFrame[3] = Ldat_NDeepDischargeEvent & 0X00FF;
                pCanFrame[4] = (Ldat_NDeepDischargeEvent & 0XFF00) >> 8;  
                pCanFrame[5] = Ldat_DayReamingDay & 0X00FF;
                pCanFrame[6] = (Ldat_DayReamingDay & 0XFF00) >> 8;
                pCanFrame[7] = 0X00;
                break;
            case  7: //frame M=8
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x08;  
                pCanFrame[1] = Ldat_DCIRStringMin & 0X00FF;
                pCanFrame[2] = (Ldat_DCIRStringMin & 0XFF00) >> 8;
                pCanFrame[3] = Ldat_DCIRString & 0X00FF;
                pCanFrame[4] = (Ldat_DCIRString & 0XFF00) >> 8;
                pCanFrame[5] = Ldat_ShippingTotal & 0X00FF;
                pCanFrame[6] = (Ldat_ShippingTotal & 0XFF00) >> 8;
                pCanFrame[7] = EventSOH; // tsai 5/31
                break;    
            case  8: //frame M=9
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x09;
                pCanFrame[1] = RecorderLdat.Discharge0Counter & 0X00FF;
                pCanFrame[2] = (RecorderLdat.Discharge0Counter & 0XFF00) >> 8; 
                pCanFrame[3] = RecorderLdat.Discharge20Counter & 0X00FF;
                pCanFrame[4] = (RecorderLdat.Discharge20Counter & 0XFF00) >> 8; 
                pCanFrame[5] = RecorderLdat.Discharge40Counter & 0X00FF;
                pCanFrame[6] = (RecorderLdat.Discharge40Counter & 0XFF00) >> 8;     
                pCanFrame[7] = 0X00; 
                break;
            case  9: //frame M=10
                pCanFrame[0] &= 0XF0;
                pCanFrame[0] |= 0x0A;  
                pCanFrame[1] = RecorderLdat.Discharge60Counter & 0X00FF;
                pCanFrame[2] = (RecorderLdat.Discharge60Counter & 0XFF00) >> 8; 
                pCanFrame[3] = RecorderLdat.Discharge80Counter & 0X00FF;
                pCanFrame[4] = (RecorderLdat.Discharge80Counter & 0XFF00) >> 8; 
                pCanFrame[5] = 0X00;
                pCanFrame[6] = 0X00;        
                pCanFrame[7] = 0X00;  				
                break;    
            default:
                break;
        }
    } 
    return TRUE;
}

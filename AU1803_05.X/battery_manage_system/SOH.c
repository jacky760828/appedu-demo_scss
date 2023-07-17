/*
 * File:   soh.c
 * Author: frank.tsai
 *
 * Created on 2021?12?12?, ?? 4:45
 */

#include "SOH.h"

SOHInfo_t SOH_STATE;
uint16_t BATT_Voltage_OCV_Table[12] = {4760, 4837, 4914, 4991, 5069, 5146, 5223, 5300, 5377, 5454, 5531, 5605};
uint8_t SOC[12] = {0,10,20,30,40,50,60,70,80,90,100,105};
uint32_t RC_calculated_gain = 0;
    
uint32_t uiCalRCapacity;
uint32_t uiCalRCapacity100mWs;
uint32_t rc_0_endout = -1;
uint32_t rc_0_endout1 = -1;

uint16_t LastKvalue = 0;

void SOH_Log_SOHInfo(void)
{
    printf("SOH_STATE.efficient:0x%02X \r\n", SOH_STATE.efficient);
    printf("SOH_STATE.flashaddress:%ld \r\n", SOH_STATE.flashaddress);
    printf("SOH_STATE.DecaySOH:%d \r\n", SOH_STATE.DecaySOH);
    printf("SOH_STATE.cycles:%d \r\n", SOH_STATE.cycles);
    printf("SOH_STATE.DSGFCC:%d \r\n", SOH_STATE.DSGFCC);
    printf("SOH_STATE.Coulomb_WH:%d \r\n", SOH_STATE.Coulomb_WH);
    printf("SOH_STATE.Coulomb_100mWS:%ld \r\n", SOH_STATE.Coulomb_100mWS);
    printf("SOH_STATE.Crc:0x%04X \r\n", SOH_STATE.Crc);
}

int32_t SOH_Write_Flash(uint32_t adrress, SOHInfo_t *data)
{
    int32_t ret;
    uint16_t crc16 = 0xFFFF;
    
    if((adrress < SOHFlashAddress) && (adrress > (SOHFlashAddress + SOH_FLASH_AREA_SIZE))) {    //adrress is not 
        ret = -1;
        printf("adrress erro \r\n");
    } else if((adrress + sizeof(SOHInfo_t)) >= (SOHFlashAddress + SOH_FLASH_AREA_SIZE)) {       //over
        adrress = SOHFlashAddress;
        data->flashaddress = SOHFlashAddress + sizeof(SOHInfo_t);
        data->Crc = updateCRC16(crc16, (uint8_t *)&SOH_STATE, sizeof(SOHInfo_t) - 2);
        sFLASH_EraseSector(adrress);
        sFLASH_WriteBuffer((uint8_t *)data, adrress, sizeof(SOHInfo_t));
        ret = adrress + sizeof(SOHInfo_t);
        printf("over adrress(%ld) \r\n", ret);
    } else {
        data->flashaddress += sizeof(SOHInfo_t);
        data->Crc = updateCRC16(crc16, (uint8_t *)&SOH_STATE, sizeof(SOHInfo_t) - 2);
        SOH_Log_SOHInfo();
        sFLASH_WriteBuffer((uint8_t *)data, adrress, sizeof(SOHInfo_t));
        ret = adrress + sizeof(SOHInfo_t);
        printf("adrress(%ld) \r\n", ret);
    }
    
    return ret;
}

int32_t SOH_Read_Flash(void)
{
    int32_t ret;
    uint16_t crc16 = 0xFFFF;
    int i;
    
    for(i = (SOH_FLASH_AREA_SIZE / sizeof(SOHInfo_t)); i >= 0; i --) {
//        printf("i:%d ",i);
        ret = SOHFlashAddress + (i * sizeof(SOHInfo_t));
        sFLASH_ReadBuffer((uint8_t *)&SOH_STATE,ret,sizeof(SOHInfo_t));
        if(SOH_STATE.efficient == 0xAA) {
            crc16 = updateCRC16(crc16, (uint8_t *)&SOH_STATE, sizeof(SOHInfo_t) - 2);
            if(crc16 == SOH_STATE.Crc) {
            break;
            } else {
                printf("crc -error(0x%04X 0x%04X)",crc16 ,SOH_STATE.Crc);
            }
        } else {
//            printf("SOH_STATE.efficient -error(0x%02X)",SOH_STATE.efficient);
        }
    }
    printf("\r\n");
    if(i < 0) {
        ret = -1;
    }
    
    return ret;
}

uint16_t SOH_GetDecySOH(void)
{
    return (SOH_STATE.DecaySOH);
}

uint16_t SOH_GetDecayFCC(void)
{
    uint16_t ret;
    uint16_t ret1;
    
    ret = (uint32_t)Gague_FCC * RC_MAX / 100;
    ret1 = ((uint32_t)Gague_FCC * RC_MIN / 100);
    if(ret > ret1) {
        ret = ret - ret1;
    } else {
        ret = 0;
    }
    Calculation_FCC = ret;
//    printf("ret:%d Gague_FCC:%d d_DSGFCC:%d \r\n", ret, Gague_FCC, d_DSGFCC);
    
    return ret;
}

uint16_t SOH_GetDecayFC(void)
{
    uint16_t ret;
    
    ret = (uint32_t)Gague_FCC * RC_MIN / 100;
    if(Gague_RC > ret) {
        ret = Gague_RC - ret;
    } else {
        ret = 0;
    }
    if(Out_FCC < ret) {
        ret = Out_FCC;
    }
    if((300 < iBatt) && ret == 0) {
        ret = 1;
    }
//    printf("%d = (GaugeRC(%d) - %d) * DSGFCC(%d) / dsgFCC(%d) \r\n", ret, SMBUS_getGaugeRC(), 35, d_DSGFCC, dsgFCC);
    return ret;
}

uint16_t SOH_Get_TotalCoulomb(void)
{
    return SOH_STATE.Coulomb_WH;
}

uint16_t SOH_Get_DeepDISChargeCycle(void)
{
    return SOH_STATE.cycles;
}

void SOH_Init_while(void)
{
    Gague_FCC = SMBUS_getGaugeFCC();
    SOH_STATE.DSGFCC = Gague_FCC;
    Calculation_RC = SMBUS_getGaugeRC();
    SOH_STATE.Coulomb_WH = Calculation_RC;
    dsgFCC = ((uint16_t)SMBUS_fullChargeCapacity[1] <<8 ) | SMBUS_fullChargeCapacity[0];
    uiCalRCapacity = ((uint16_t)SMBUS_remainingCapacity[1] << 8) | SMBUS_remainingCapacity[0];
    uiCalRCapacity100mWs = uiCalRCapacity * 36000L;
//    SOH_STATE.Coulomb_100mWS = BMS_Get_uiCalRCapacity100mWS();
    printf("SOH_Init uiCalRCapacityWH:%d \r\n", Calculation_RC);
}

void SOH_Init(void)
{
    int32_t ret;
    
    ret = SOH_Read_Flash();
    if(ret == -1) { // no data
        SOH_STATE.efficient = 0xAA;
        SOH_STATE.flashaddress = SOHFlashAddress;
        SOH_STATE.DecaySOH = SMBUS_getStateOfHealth();
        SOH_STATE.cycles = SMBUS_getCycleCount();
        SOH_STATE.DSGFCC = 326;
        if(d_deviceAddr == CAN_DEFAULT_RBC_ADDRESS) {
            Calculation_RC = RBC_FULL_CHARGE_CAPACITY_WH_DEF;
        } else {
            Calculation_RC = XBP_FULL_CHARGE_CAPACITY_WH_DEF;
        }
        sFLASH_EraseSector(SOHFlashAddress);
        ret = SOH_Write_Flash(SOH_STATE.flashaddress, &SOH_STATE);
        printf("no SOH data %ld \r\n", ret);
    } else {
        printf("read SOH data form Flash %ld \r\n", ret);
        Calculation_RC = SOH_STATE.Coulomb_WH;
    }
    uiCalRCapacity = ((uint16_t)SMBUS_current[1] << 8) | SMBUS_current[0];
    uiCalRCapacity100mWs = uiCalRCapacity*36000L;
    SOH_Log_SOHInfo();
    EVENTLOG_SOH();
}

#define DEFAULT_WATT    33000           // 3500W
#define DecountSOC2     90000
#define AU1803_BASE_TEMPATURE           2981
#define AU1803_RESOLUTION               100000L    
#define IdleLostmWhPerHr   1598 //unit mWh ,  1.53192f //Wh  18% 35.25hr
#define T_IDLE_1HR   60
#define iEfTemp      0
#define iSfTemp      0
    
uint8_t RecorderDischargeWhs[8];
uint32_t TemperatureCompensationK = 0;
uint16_t ReadSecond = 0;
uint32_t RemaingTimeRvalue = 0;
uint32_t kValue;
const uint16_t disPwr[DYNAMIC_K_SIZE]={34500,31000,27500,24000,20500,17000,13500,10000,6500,3300,2300,1300,1000,500};
uint16_t kVal[DYNAMIC_K_SIZE];
uint16_t kVal5oC[DYNAMIC_K_SIZE];
uint8_t AfterWattStatus = 0;
uint8_t SaveDynamicNumber;

const uint16_t kVal1803_Default[DYNAMIC_K_SIZE]={153,138,128,117,113,111,110,110,107,104,103,102,101,100};
const uint16_t kVal5oC1803_Default[DYNAMIC_K_SIZE]={153,138,128,117,113,111,110,110,107,104,103,102,101,100};
const uint16_t kVal1805_Default[DYNAMIC_K_SIZE]={110,128,125,122,121,120,119,117,117,115,115,112,108,106};
const uint16_t kVal5oC1805_Default[DYNAMIC_K_SIZE]={110,128,100,122,121,118,117,115,113,110,110,112,108,106};
uint16_t kVal1checksum; //  = kVal1803_Default[1]~[14]+kVal1805_Default[1]~[14]checksum
K_VALUE_Info_t K_VALUE_STATE;

void SOH_Log_kVal1Info(void)
{
    uint8_t i;
    
    printf("K_VALUE_STATE.efficient:0x%02X \r\n", K_VALUE_STATE.efficient);
    printf("K_VALUE_STATE.flashaddress:%ld \r\n", K_VALUE_STATE.flashaddress);
    printf("K_VALUE_STATE.K_VALUE:");
    for(i = 0; i < DYNAMIC_K_SIZE; i++) {
        printf("%d ", K_VALUE_STATE.K_VALUE[i]);
    }
    printf("\r\n");
    printf("K_VALUE_STATE.K_5oC_VALUE:");
    for(i = 0; i < DYNAMIC_K_SIZE; i++) {
        printf("%d ", K_VALUE_STATE.K_5oC_VALUE[i]);
    }
    printf("\r\n");
    printf("K_VALUE_STATE.Crc:0x%04X \r\n", K_VALUE_STATE.Crc);
}

int32_t kVal1_Write_Flash(uint32_t adrress, K_VALUE_Info_t *data)
{
    int32_t ret;
    
    if((adrress < DYNAMIC_K_VALUE_ADR) && (adrress > (DYNAMIC_K_VALUE_ADR + DYNAMIC_K_FLASH_AREA_SIZE))) {    //adrress is not 
        ret = -1;
        printf("adrress erro \r\n");
    } else if((adrress + sizeof(K_VALUE_Info_t)) >= (DYNAMIC_K_VALUE_ADR + DYNAMIC_K_FLASH_AREA_SIZE)) {       //over
        adrress = DYNAMIC_K_VALUE_ADR;
        data->flashaddress = DYNAMIC_K_VALUE_ADR + sizeof(K_VALUE_Info_t);
        printf("kVal1checksum(0x%04X)",kVal1checksum);
        data->Crc = updateCRC16(kVal1checksum, (uint8_t *)&data, sizeof(K_VALUE_Info_t) - 2);
        sFLASH_EraseSector(adrress);
        SOH_Log_kVal1Info();
        sFLASH_WriteBuffer((uint8_t *)data, adrress, sizeof(K_VALUE_Info_t));
        ret = adrress + sizeof(K_VALUE_Info_t);
        printf("over adrress(%ld) \r\n", ret);
    } else {
        data->flashaddress += sizeof(K_VALUE_Info_t);
        printf("kVal1checksum(0x%04X) \r\n",kVal1checksum);
        data->Crc = updateCRC16(kVal1checksum, (uint8_t *)data, sizeof(K_VALUE_Info_t) - 2);
        SOH_Log_kVal1Info();
        sFLASH_WriteBuffer((uint8_t *)data, adrress, sizeof(K_VALUE_Info_t));
        ret = adrress + sizeof(K_VALUE_Info_t);
        printf("adrress(%ld) \r\n", ret);
    }
    return ret;
}

int32_t kVal1_Read_Flash(void)
{
    int32_t ret;
    uint16_t crc16 = kVal1checksum;
    int i;
    
    for(i = (DYNAMIC_K_FLASH_AREA_SIZE / sizeof(K_VALUE_Info_t)); i >= 0; i --) {
//        printf("i:%d ",i);
        ret = DYNAMIC_K_VALUE_ADR + (i * sizeof(K_VALUE_Info_t));
        sFLASH_ReadBuffer((uint8_t *)&K_VALUE_STATE,ret,sizeof(K_VALUE_Info_t));
        if(K_VALUE_STATE.efficient == 0xAA) {
            SOH_Log_kVal1Info();
            printf("kVal1checksum(0x%04X) \r\n",kVal1checksum);
            crc16 = updateCRC16(kVal1checksum, (uint8_t *)&K_VALUE_STATE, sizeof(K_VALUE_Info_t) - 2);
            if(crc16 == K_VALUE_STATE.Crc) {
            break;
            } else {
                printf("crc -error(0x%04X 0x%04X)",crc16 ,K_VALUE_STATE.Crc);
            }
        } else {
//            printf("K_VALUE_STATE.efficient -error(0x%02X)",K_VALUE_STATE.efficient);
        }
    }
    printf("\r\n");
    if(i < 0) {
        ret = -1;
    }
    
    return ret;
}

void Get_Dynamic_Data(void)
{
    int32_t ret;
    uint8_t i;
    
    kVal1checksum = 0;
    if(FixDeviceID == CAN_DEFAULT_RBC_ADDRESS) {
        for(i = 0; i < DYNAMIC_K_SIZE; i++) {
            K_VALUE_STATE.K_VALUE[i] = kVal1803_Default[i];
            K_VALUE_STATE.K_5oC_VALUE[i] = kVal5oC1803_Default[i];
            kVal1checksum += K_VALUE_STATE.K_VALUE[i] + K_VALUE_STATE.K_5oC_VALUE[i];
        }
    } else {
        for(i = 0; i < DYNAMIC_K_SIZE; i++) {
            K_VALUE_STATE.K_VALUE[i] = kVal1805_Default[i];
            K_VALUE_STATE.K_5oC_VALUE[i] = kVal5oC1805_Default[i];
            kVal1checksum += K_VALUE_STATE.K_VALUE[i] + K_VALUE_STATE.K_5oC_VALUE[i];
        }
    }
    ret = kVal1_Read_Flash();
    if(ret == -1) { // no data
        K_VALUE_STATE.efficient = 0xAA;
        K_VALUE_STATE.flashaddress = DYNAMIC_K_VALUE_ADR;
        kVal1checksum = 0;
        if(FixDeviceID == CAN_DEFAULT_RBC_ADDRESS) {
            for(i = 0; i < DYNAMIC_K_SIZE; i++) {
                K_VALUE_STATE.K_VALUE[i] = kVal1803_Default[i];
                K_VALUE_STATE.K_5oC_VALUE[i] = kVal5oC1803_Default[i];
                kVal1checksum += K_VALUE_STATE.K_VALUE[i] + K_VALUE_STATE.K_5oC_VALUE[i];
            }
        } else {
            for(i = 0; i < DYNAMIC_K_SIZE; i++) {
                K_VALUE_STATE.K_VALUE[i] = kVal1805_Default[i];
                K_VALUE_STATE.K_5oC_VALUE[i] = kVal5oC1805_Default[i];
                kVal1checksum += K_VALUE_STATE.K_VALUE[i] + K_VALUE_STATE.K_5oC_VALUE[i];
            }
        }
        sFLASH_EraseSector(DYNAMIC_K_VALUE_ADR);
        ret = kVal1_Write_Flash(K_VALUE_STATE.flashaddress, &K_VALUE_STATE);
        printf("no K VALUE data %ld \r\n", ret);
    } else {
        printf("read K VALUE data form Flash %ld \r\n", ret);
        printf("K_VALUE:");
        for(i = 0; i < DYNAMIC_K_SIZE; i++) {
            kVal[i] = K_VALUE_STATE.K_VALUE[i];
            printf("%d ", K_VALUE_STATE.K_VALUE[i]);
        }
        printf("\r\n");
        printf("K_5oC_VALUE:");
        for(i = 0; i < DYNAMIC_K_SIZE; i++) {
            kVal5oC[i] = K_VALUE_STATE.K_5oC_VALUE[i];
            printf("%d ", K_VALUE_STATE.K_5oC_VALUE[i]);
        }
        printf("\r\n");
    }
}

void SmartStudyAdjKvalue(uint8_t device_id)
{ 
    uint8_t i;
    uint16_t pwrup = 0;
    uint16_t pwrdn = 0;
    int16_t ReadCellTempatureDischarge = 0;
    uint16_t newkvalue;
    
    ReadCellTempatureDischarge = SMBUS_getCellTemperature(); 
    if(ReadCellTempatureDischarge >= 0) {
        
        if(ReadCellTempatureDischarge < AU1803_BASE_TEMPATURE) {    // < 25oC  
            TemperatureCompensationK = AU1803_BASE_TEMPATURE - ReadCellTempatureDischarge;
            if (device_id == CAN_DEFAULT_RBC_ADDRESS) {             //AU1803
                TemperatureCompensationK = AU1803_RESOLUTION - TemperatureCompensationK * 952; 
            } else {                                                //AU1805
                TemperatureCompensationK = AU1803_RESOLUTION - TemperatureCompensationK * 510;
            }
        } else {                                                    // >= 25oC AU1803/5
            TemperatureCompensationK = ReadCellTempatureDischarge - AU1803_BASE_TEMPATURE;
            TemperatureCompensationK = AU1803_RESOLUTION - TemperatureCompensationK * 336;
        }
        TemperatureCompensationK = TemperatureCompensationK / 1000;
    }      
    
//    RemaingTimeRvalue = ((rc_0_endout + (rc_0_endout - rc_0_endout1)) / 2) / rc_0_endout; -> rc_0_endout1 / 2 / rc_0_endout
    RemaingTimeRvalue = rc_0_endout1 * 100 / 2 / rc_0_endout;
    printf("RC_0:%ldS 42.4V:%ldS \n\r", rc_0_endout, rc_0_endout1);
    newkvalue = (((LastKvalue * 100) / TemperatureCompensationK) * RemaingTimeRvalue) / 10000;
    printf("NEW_K:%d old_K:%d T_K:%ld RT_R:%ld \n\r", newkvalue, LastKvalue, TemperatureCompensationK, RemaingTimeRvalue);

    if(newkvalue > 250) {
        newkvalue = 255;
    } else if(newkvalue < 100) {
        newkvalue = 100;
    }
    
    for(i = 0; i < DYNAMIC_K_SIZE; i++) {
        pwrup = disPwr[i] /1000L;
        if(i != (DYNAMIC_K_SIZE - 1)) {
            pwrdn = disPwr[i+1] /1000L;
        } else {
            pwrdn = 80;     // 8W =0.15A * 53V
        }
        if((AfterWattStatus < pwrup) && (AfterWattStatus >= pwrdn)) {
            printf("NEW_K:%d old_K:%d \r\n",newkvalue ,kVal[i]);
            if(CellTempature_Min > 41) {
                kVal[i] = newkvalue;
            } else {
                kVal5oC[i] = newkvalue;
            }
            break;
        }
    }
    
    K_VALUE_STATE.efficient = 0xAA;
    printf("K_VALUE:");
    for(i = 0; i < DYNAMIC_K_SIZE; i++) {
        K_VALUE_STATE.K_VALUE[i] = kVal[i];
        printf("%d ", K_VALUE_STATE.K_VALUE[i]);
    }
    printf("\n\r");
    
    printf("K_5oC_VALUE:");
    for(i = 0; i < DYNAMIC_K_SIZE; i++) {
        K_VALUE_STATE.K_5oC_VALUE[i] = kVal5oC[i];
        printf("%d ", K_VALUE_STATE.K_5oC_VALUE[i]);
    }
    printf("\n\r");
    kVal1_Write_Flash(K_VALUE_STATE.flashaddress, &K_VALUE_STATE);
}

uint16_t get_value_from_list(uint16_t Target, const uint16_t Xaxis[], const uint16_t Yaxis[], uint8_t Size)
{
	uint16_t XValue0,XValue1,YValue0,YValue1;
	int8_t i=0,j=1,x;//X is increase type

	if (Xaxis[0]> Xaxis[Size-1]) { //X is decrease type
		i=Size-1;
		j=-1;
	}	
	XValue0=Xaxis[i];
	YValue0=Yaxis[i];
	for (x=0;x<Size;x++) {					
		XValue1=Xaxis[i];
		YValue1=Yaxis[i];
		if (XValue1 < Target) {
			XValue0=XValue1;
			YValue0=YValue1;
		} else {
			if (XValue1 != XValue0) //XValue1 != XValue0)
				YValue1=(uint16_t)((short)(YValue1-YValue0)*(long)(Target-XValue0)/(XValue1-XValue0)+YValue0);
			break;
		}
		i+=j;
	} 
	
	return YValue1;
}

uint8_t rsoc = 0;
uint16_t dsgFCC=0;
uint32_t tempWatt;
bool OnceEnterSaveKvalueFlag = false;
bool EndterDynmicFlag = false;
uint16_t FccFactor = 0;
uint32_t BeforeWatt = 0;
uint16_t LightToHeavy = 0;
uint32_t IdleLostmWh;
uint32_t d_timerIdle=0;
uint32_t power = 0;
int16_t LastSecondCnt = 0;
bool NotEnterFlag = false;
bool EnterWattUpdateFlag = false;
uint8_t DCIR_Counter = 0;
uint32_t DCIR_V1 = 0;
float vSfTemp=0;

void IdleDischargeLostCheck(void) 
{
    static uint16_t idlesmbRM=0xFFFF;
    uint16_t smbRM;
    
    if (((iBatt <= 150) && (iBatt >= -150))) { //12/3
        if (idlesmbRM==0xFFFF) {
            idlesmbRM = SMBUS_getGaugeRC();
        }
        if ( SysTime_minuteIntervalElapsed(d_timerIdle, T_IDLE_1HR)) { //1 hr
            if (IdleLostmWh >= IdleLostmWhPerHr) {
                IdleLostmWh -= IdleLostmWhPerHr;
            } else {
                IdleLostmWh =0;
            }
            smbRM =SMBUS_getGaugeRC();
            if ( smbRM < idlesmbRM ) { //gaugeIC change and decrease
                if (smbRM <= uiCalRCapacity) {
                    uiCalRCapacity = smbRM;
                } else {
                    uiCalRCapacity = (IdleLostmWh+500)/1000; // to Wh
                }
            } else {
                uiCalRCapacity = (IdleLostmWh+500)/1000; // to Wh
            }
            uiCalRCapacity100mWs = uiCalRCapacity*36000L;//12/3
            printf("uiCalRCapacity100mWs %ld \r\n", uiCalRCapacity100mWs);
            d_timerIdle = SysTime_getMinute();
        }
   } else {
        idlesmbRM=0xFFFF;
        //IdleLostmWh=uiCalRCapacity*1000L; // to mWh
        d_timerIdle = SysTime_getMinute();
   }

}

void RC_old(void)
{
    uint32_t DoubleValue = 0;
    uint32_t ChangeWatt = 0;
    uint32_t OperationChange = 0;
    uint32_t instant100mWs=0;
    uint16_t current = 0;
    uint32_t DCIR_I = 0;
    uint32_t DCIR_V2 = 0;
    uint32_t DCIR_Result = 0;
    int16_t ibatt;
    float vbat = 0;
    
    if ( SMBUS_getVoltage() > 100)
        vbat = SMBUS_getVoltage()/100.0f;
    else
        vbat =  49.0f;
    ibatt = SMBUS_getCurrent();
    //1sec
    rsoc = (uiCalRCapacity * 100 + (dsgFCC/2))/dsgFCC; // tsai 5/29

    if (ibatt < -15 ) {// -150mA    discharge
        if(uiCalRCapacity != 0) {
            tempWatt = (uint32_t)(-ibatt * vbat/10); // unit 100mW , so larer should *100 to mWh
            OnceEnterSaveKvalueFlag = false;
            if(EndterDynmicFlag == 0) {
                if(FccFactor != 0) {
                    kValue = (FccFactor * kValue)/100;
                    printf("kValue %ld FccFactor:%d \r\n", kValue, FccFactor);
                } else if(CellTempature_Min > 41) {
                    kValue = get_value_from_list(tempWatt, disPwr, kVal, DYNAMIC_K_SIZE);
                    printf("Temp > 41 get_KValue %ld tempWatt:%ld \r\n", kValue, tempWatt);
                } else {
                    kValue = get_value_from_list(tempWatt, disPwr, kVal5oC, DYNAMIC_K_SIZE);
                    printf("get_kVal5oC %ld tempWatt:%ld \r\n", kValue, tempWatt);
//                        printf("get1803_5oCKValue %ld tempWatt:%ld \r\n", kValue, tempWatt);
                }
                if((dsgFCC > d_DSGFCC - 10)&&(FccFactor == 0)) {
                    FccFactor = (dsgFCC * 100)/dsgFCC;
                }
            }
            
            if((BeforeWatt == 0)&&(rsoc <= 60)) {
                BeforeWatt = tempWatt;
            }
            if(BeforeWatt > 0) {
                LightToHeavy = (tempWatt*100) / BeforeWatt;
            }
            
//            kValue = 106; //test 1000W 20230619
            
            if((LightToHeavy >= 120)&&(BeforeWatt != tempWatt)&&(rsoc <= 60)) {         // Light Change To Heavy Load`
                EndterDynmicFlag = true;
                DoubleValue = (tempWatt * 100)/DEFAULT_WATT;
                DoubleValue = DoubleValue * DoubleValue;
                ChangeWatt = (DoubleValue * tempWatt)/ BeforeWatt;
                ChangeWatt = (ChangeWatt + 10000) /100;
                OperationChange = kValue;
                kValue = (OperationChange * ChangeWatt) / 100 + 4;
                BeforeWatt = tempWatt;
                printf("kValue %ld ChangeWatt:%ld \r\n", kValue, ChangeWatt);
            }
            
            instant100mWs = (kValue * tempWatt) / 100;
            if (uiCalRCapacity100mWs >= instant100mWs) {
                uiCalRCapacity100mWs = uiCalRCapacity100mWs - instant100mWs;
                uiCalRCapacity = uiCalRCapacity100mWs/36000L;
                if(RemainTimeCnt > 0) {
                    RemainTimeCnt = (uiCalRCapacity100mWs-DecountSOC2) / instant100mWs;
//                            CaluateRCapacity = CaluateRCapacity-tempWatt;
                }
            } else {
                printf("uiCalRCapacity100mWs %ld instant100mWs:%ld \r\n", uiCalRCapacity100mWs, instant100mWs);
                uiCalRCapacity = 0;
                uiCalRCapacity100mWs = 0;
                RemainTimeCnt = 0;
            }
            IdleLostmWh=uiCalRCapacity*1000L; // to mWh
#ifdef EVENT_LOG
            DischargeFlag = 1;
            current = ~ibatt + 1;
            power = tempWatt / 10;
            if(current > Ldat_DischargeCurrentMax) {
                Ldat_DischargeCurrentMax = current;
            }
            if(power > Ldat_DischargePowerMax) {
                Ldat_DischargePowerMax = power;
            }
            if(Ldat_DischargePowerMax < power) {
                Ldat_DischargePowerMax = power;
            }
#endif
        }// RC !=0
        
        if(FixDeviceID == CAN_DEFAULT_RBC_ADDRESS) {
            if((vbat >= 42.6f) && (RemainTimeCnt == 0)) {
                LastSecondCnt --;
            } else if((vbat < 42.6f) && (RemainTimeCnt > 0)) {
                LastSecondCnt ++;
            } else {
                LastSecondCnt = 0;
            }
            if((vbat<= 43.0f)&&(vbat >= 42.6f)&&(NotEnterFlag == 0)) {
                NotEnterFlag = true;
                EnterWattUpdateFlag = true;
                AfterWattStatus = tempWatt /1000L;
                LastSecondCnt = 0;
                LastKvalue = (uint16_t)kValue;
//                        LastWatt = tempWatt;  //ian
            }
        } else {
            if((vbat >= 45.5f) && (RemainTimeCnt == 0)) {
                LastSecondCnt --;
            } else if((vbat < 45.5f) && (RemainTimeCnt > 0)) {
                LastSecondCnt ++;
            } else {
                LastSecondCnt = 0;
            }
            if((vbat<= 46.0f)&&(vbat >= 45.5f)&&(NotEnterFlag == 0)) {
                NotEnterFlag = true;
                EnterWattUpdateFlag = true;
                AfterWattStatus = tempWatt /1000L;
                LastSecondCnt = 0;
                LastKvalue = (uint16_t)kValue;
//                        LastWatt = tempWatt;  //ian
            }
        }
        if((0 == uiCalRCapacity) && (-300 >= ibatt) && (-1 == rc_0_endout)) {
            rc_0_endout = SysTime_getSec();
        } else if((0 == uiCalRCapacity) && (-300 >= ibatt) && (-1 != rc_0_endout) && (42.5f <= vbat)) {
            rc_0_endout1 = SysTime_getSec();
        }
        
        if(ibatt <= -5000) {
            switch(DCIR_Counter) {
                case 1:
                    DCIR_Counter = 2;
                    DCIR_V1 = (uint16_t)(vbat * 10);
                    break;
                
                case 2:
                    DCIR_Counter = 0;
                    DCIR_I = (uint16_t)(ibatt/(-10));
                    DCIR_V2 = (uint16_t)(vbat * 10);
                    if(DCIR_V1 >= DCIR_V2) {
                        DCIR_Result = ((DCIR_V2 - DCIR_V1)*1000L)/DCIR_I;
                    } else {
                        DCIR_Result = ((DCIR_V1 - DCIR_V2)*1000L)/DCIR_I;
                    }
                    Ldat_DCIRString = (uint16_t)DCIR_Result;
                    if(DCIR_Result < Ldat_DCIRStringMin) {
                        Ldat_DCIRStringMin = (uint16_t)DCIR_Result;
                    }
                    break;
                default:
                    break;
            }
        }
    } else { //if(iBatt>=0 )
        if ((ibatt <= 15)&&(ibatt > 0)) { //150mA idle
            vSfTemp=0;
            RemainTimeCnt = 0XFFFF;
            tempWatt = (uint32_t)(ibatt * vbat/10);
            if (sysProtection.EOCVBit) { //end of charge
                dsgFCC = ((uint16_t)SMBUS_fullChargeCapacity[1] <<8 ) | SMBUS_fullChargeCapacity[0];   //update  	
                uiCalRCapacity = dsgFCC;
                uiCalRCapacity100mWs = uiCalRCapacity * 36000L;
                printf("uiCalRCapacity100mWs %ld \r\n", uiCalRCapacity100mWs);
                IdleLostmWh = uiCalRCapacity * 1000L; // to mWh
                BeforeWatt = 0;
                FccFactor = 0;
                DischargeFlag = 0;
                DCIR_Counter = 1;
                if((EnterWattUpdateFlag == 1)&&(LastSecondCnt != 0)&&(EndterDynmicFlag == 0)) {
                    NotEnterFlag = false;
                    EnterWattUpdateFlag = false;
                    SmartStudyAdjKvalue(FixDeviceID);
                    printf("SmartStudyAdjKvalue %ld \r\n", kValue);
                }
                EndterDynmicFlag = false;
            }
        } else if(ibatt > 15) {// charge
#ifdef EVENT_LOG
            DCIR_Counter = 1;
            DischargeFlag = 0;
            Discharge30sFlag = 0;
            CanResponse30sCnt = 0;
            RemainTimeCnt = 0XFFFF;
            BeforeWatt = 0;
            if(ibatt > Ldat_ChargeCurrentMax) {
                Ldat_ChargeCurrentMax = ibatt;                  // Get Charge Current Max
            }
            FccFactor = 0;
            tempWatt = (uint32_t)(ibatt * vbat/10);
            power = tempWatt / 10;
            if(Ldat_ChargePowerMax < power) {
                Ldat_ChargePowerMax = power;
            }
#endif
            uiCalRCapacity = ((uint16_t)SMBUS_remainingCapacity[1] << 8) | SMBUS_remainingCapacity[0];
            uiCalRCapacity100mWs = uiCalRCapacity*36000L;
            IdleLostmWh=uiCalRCapacity*1000L; // to mWh
        }
    }
}
/* 
 * File:   EventLog.h
 * Author: leicaser.liao
 *
 * Created on 2022?5?17?, ?? 10:33
 */

#ifndef EVENTLOG_H
#define	EVENTLOG_H


#ifdef EVENT_LOG
#include "../mcc_generated_files/can_types.h"
#include <stdbool.h>
#include <stdint.h>
//===Memory Address Organization===============================
#define CTECH_HEAD_AREA1            0X00100000      // C-TECH Header index begine (sector 1)
#define CTECH_HEAD_AREA2            0X00101000      // C-TECH Header index begine (sector 2)
#define LDAT_HEAD_AREA1             0X00102000      // LDAT Header index begine (sector 1)
#define LDAT_HEAD_AREA2             0X00103000      // LDAT Header index begine (sector 2)
#define EVENT_HEAD_AREA1            0X00106000      // EVENT Header index begine (sector 1)
#define CTECH_RBC_KVALUE_HEAD       0X0010A000      // C-TECH AU1803 K-Value Scale Address
#define CTECH_RBC_KVALUE_ADR        0X0010B000      // C-TECH AU1803 K-Value Scale Address
#define CTECH_RBC_KVALUE_BK_ADR     0X0010C000
#define CTECH_XBC_KVALUE_HEAD       0X0010D000      // C-TECH AU1805 K-Value Scale Address
#define CTECH_XBC_KVALUE_ADR        0X0010E000      // C-TECH AU1805 K-Value Scale Address
#define CTECH_XBC_KVALUE_BK_ADR     0X0010F000
#define CTECH_MSG_START_ADR         0X00200000      // C-TECH Recorder Message start address for 1MB
#define LDAT_MSG_START_ADR          0X00300000      // LDAT Recorder Message start address (sector 1)
#define LDAT_MSG_START_ADR2         0X00301000      // LDAT Recorder Message start address (sector 2)
#define DYNAMIC_K_VALUE_ADR1        0X00303000      // Dynamic K-Value Start address(sector 1)
#define DYNAMIC_K_VALUE_ADR2        0X00304000      // Dynamic K-Value Start address(sector 2)
#define EVENT_HEAD_INDEX_ADR1       0X00306000      // EVENT Head Index Start Address (secor 1)
#define EVENT_HEAD_INDEX_ADR2       0X00307000      // EVENT Head Index Start Address (secor 2)
#define EVENT_MSG_START_ADR         0X00400000      // EVENT Recorder Message start address for 2MB
#define EVENT_LABEL1_ADR            0X00108000
#define EVENT_LABEL2_ADR            0X00109000
#define RECODER_LDAT_ERASE_ADR      0X0010A000
#define RECODER_EVENT_ERASE_ADR     0X0010B000
#define SOC_RECORDER_ADR_1          0X00106000
#define SOC_RECORDER_ADR_2          0X00107000
//EventLog ID from SE
//other Condition Change

#define EVENT_HEAD_INDEX_ADR1_SIZE     0x1000          //4K

#define ERASE_1M                    16
#define ERASE_2M                    32


/*******Register / Setting Commands******************/
#define COM_READ					0X03			// Normal Read:n-byte read out until CS# goes high
#define COM_FAST_READ				0X0B			// Fast Read:n-byte read out until CS# goes high
#define COM_2READ					0XBB			// 2*I/O Read Command
#define COM_DREAD					0X3B			// 1 | I/O Read Command
#define COM_4READ					0XEB			// 4 * I/O Read:Quad I/O read with 6 dummy cycles
#define COM_QREAD					0X6B			// 1 | I/O Read
#define COM_PP						0X02			// To Page Program selected page
#define COM_4PP						0X38			// Quad input to program the selected page
#define COM_SE						0X20			// To Erease the selected 4K sector
#define COM_BE32K					0X52			// To Erease the selected 32K block
#define COM_BE						0XD8			// To Erease the selected 64K block
#define COM_CE						0X60			// To Erase whole chip
#define COM_RDSFDP					0X5A			// Read SFDP mode
#define COM_WREN					0X06			// Sets the (WEL) write enable latch bit
#define COM_WRDI					0X04			// Reset the (WEL) write enable latch bit
#define COM_RDSR					0X05			// To read out the values of the status register
#define COM_RDCR					0X15			// To read out the values of the configuration register-1 & configuration register-2
#define COM_WRSR					0X01			// To write new values of the configuration/status register
#define COM_PGM_ERS_SUSPEND			0X75			// Program/Erase operation is interrupted by suspend command
#define COM_PGM_ERS_RESUME			0X7A			// To continue performing the suspended program/erase sequence
#define COM_DEEP_POWER				0XB9			// Enter deep power down mode
#define COM_SBL						0XC0			// To set Burst Length
/*******ID / Reset Commands**************************/
#define COM_RDID					0X9F			// Outputs JEDEC ID:1-byte Manufacturer ID & 2-byte Device ID
#define COM_RES						0XAB			// To read out 1-byte Device ID
#define COM_REMS					0X90			// Output the Manufacturer ID & Device ID
#define COM_ENSO					0XB1			// To enter the 8K-bit secured OTP mode
#define COM_EXSO					0XC1			// To exit the 8K-bit secure OTP mode
#define COM_RDSCUR					0X2B			// To Read value of security register
#define COM_WRSCUR					0X2F			// To set the lock-down bit as "1" (once lock-down, cannot be update)
#define COM_RSTEN					0X66			// Reset Enable
#define COM_RST						0X99			// Reset Memory
//====Command End=================================================
#define RECORDER_LDATA_TIME			0X00200000		// 每隔2^22/2 = 2^21 = 2097152秒
#define EventTimeStampTop           0XFFFFFFFF      // Reset counter
//=====Index Algorithm Address====================================
extern uint8_t Discharge30sFlag;

void EventLog_Set_CANLoss(bool onoff);
bool EventLog_Get_CanRxCmd(void);
void EventLog_Set_CanRxCmd(bool onoff);
void Erase_Flash(uint32_t erase_Address,uint8_t size);
bool Check_EventLog_Lable();
void Write_EventLog_Lable();
void Get_Event_Index();
void SaveEventHeadIndex();
void WriteMemoryData(uint8_t *ptrData, uint32_t address, uint16_t length);
void Erase_Flash_SE(uint32_t erase_Address);
void WriteFlashFunction(uint32_t time_lable, uint8_t head, uint8_t sub_matrix, uint8_t sub_bit);
void Get_Ldat_Index(void);
void UpdateFaultFlag(uint8_t num);
void TimeAreaProcess();
#endif

#ifdef	__cplusplus
extern "C" {
#endif
      
//====Macro Typedf===================
#ifdef EVENT_LOG
typedef struct
{
    union{
     uint8_t data;
     struct{
         uint8_t after_Dfet:1;
         uint8_t after_Cfet:1;
         uint8_t after_trike:1;
         uint8_t b3:1;
         uint8_t before_Dfet:1;
         uint8_t before_Cfet:1;
         uint8_t before_trike:1;
         uint8_t b7:1;
     };
    };   
}FET_STATE;
    
typedef union
{
    uint8_t arry[8];
    struct{
    uint32_t lenth;
    uint32_t timestamp;
    };
}FLASH_INDEX;

typedef union
{
    uint8_t     efficient;      // AA is useing
    uint32_t    flashaddress;   // flash next address
    uint16_t    data[14];          //
    uint16_t    Checksum;       // data chack sum
}FLASH_kVal1;

typedef struct
{
    union
    {
        uint16_t Status;
        struct
        {
            uint16_t lates_cnt:8;
            uint16_t lates:1;
            uint16_t read:1;
            uint16_t rewind:1;
            uint16_t recent:1;
            uint16_t all:1;
            uint16_t abort:1;
            uint16_t erase:1;
            uint16_t null:1;
        }flag;
    };
}CAN_RESPONSE;

typedef union
{
	uint8_t arry[72];
	struct {

		uint32_t Timestamp;                   // 4 Bytes
		uint16_t ID;                          // 2 Bytes
		uint32_t ChargerEnergyTotal;          // 4 Bytes
		int16_t ChargeCurrentMax;             // 2 Bytes
		uint32_t DischargeEnergyTotal;        // 4 Bytes
		int16_t DischargeCurrentMax;          // 2 Bytes
		uint16_t InternalVoltageMax;          // 2 Bytes
		uint16_t InternalVoltageMin;          // 2 Bytes
		uint16_t DischargePowerMax;           // 2 Bytes
		uint16_t NDischargeEvents;            // 2 Bytes
		uint16_t NDeepDischargeEvent;         // 2 Bytes       
		uint8_t  InternalSOH;                 // 1 Bytes
		uint8_t  TemperatureAvg;              // 1 Bytes
		uint8_t  TemperatureMax;              // 1 Bytes
		uint8_t  TemperatureMin;              // 1 Bytes
        // 32 bytes UP-Total 32
        uint32_t TimeDischargingTotal;        // 4 bytes
        uint32_t TimeChargingTotal;           // 4 bytes
        uint32_t TimeRestingTotal;            // 4 bytes        
        uint32_t TimeBrainsUpTotal;           // 4 bytes
        uint16_t ChargePowerMax;              // 2 bytes
        uint16_t Discharge0Counter;           // 2 bytes
        uint16_t Discharge20Counter;          // 2 bytes
        uint16_t Discharge40Counter;          // 2 bytes
        uint16_t Discharge60Counter;          // 2 bytes
        uint16_t Discharge80Counter;          // 2 bytes  
		uint16_t DCIRString;                  // 2 Bytes
        uint16_t DCIRStringMin;               // 2 Bytes
        uint16_t ShippingTotal;               // 2 Bytes  
        uint16_t DaysReamingLifeExpired;       // 2 Bytes
        uint16_t CellVMax;                    // 2 Bytes
        uint16_t CellVMin;                    // 2 Bytes
        // 36 Bytes UP-Total 36
	};
}RECORDER_LDATA;

typedef union
{
	uint8_t arry[32];
	struct{
		uint32_t Timestamp;
		uint16_t ID;
		uint8_t  BeforeSate;
		uint8_t  AfterState; 
		uint8_t  FETState;
		uint16_t CMD;
		uint16_t Vbatt;
		uint16_t Vbus;
		uint16_t Vmax;
		uint16_t Vavg;
		uint16_t Vmin;
		int16_t  IbattBeforeState;
		int16_t  IbattAfterState;
		uint8_t  SOH;
		uint8_t  RSOC; 
		uint16_t FCC;
		uint8_t  TemperatureAvg;
		uint8_t  TemperatureFET;
		uint8_t  TemperatureCell;
	};
}RECORDER_EVENT;

extern uint32_t NoDischargeCnt;
extern uint32_t Ldat_DischargeEnergyTotal;
extern uint16_t Ldat_CellMin;
extern uint16_t Ldat_CellMax;
extern RECORDER_LDATA  RecorderLdat;
extern uint16_t Ldat_InternalVoltageMin;
extern uint16_t Ldat_InternalVoltageMax;
extern uint16_t Ldat_DischargeCurrentMax;
extern uint8_t Ldat_TemperatureMax;
extern uint8_t DischargeFlag;
extern uint16_t Ldat_ChargeCurrentMax;
extern uint8_t Ldat_TemperatureMin;
extern uint8_t Ldat_TemperatureAvg;
extern uint32_t Ldat_TimeBrainsUpTotal;
extern uint16_t Ldat_DischargePowerMax;
extern uint16_t Ldat_NDischargeEvents;
extern uint16_t Ldat_NDeepDischargeEvent;
extern uint16_t Ldat_DayReamingDay;
extern uint16_t Ldat_ShippingTotal;
extern uint8_t Event500msFlag;
extern uint8_t EraseAllFlag;
extern uint16_t Ldat_ChargePowerMax;

#define LOG_BATTERY_MANAGE      (1)
#define LOG_PROTECTION          (1)
#define LOG_CAN_Receive         (0)
#define LOG_CAN_Transmit        (0)
#define LOG_CFET_DFET_TRICKLE   (0)
#define LOG_BATTERY_STATE_CHANG (0)
#define LOG_SOH                 (0)
#define LOG_LED                 (0)

//====Externd Various==============
//extern ELogFlashMem ELGetAdr;
//extern ElogFlashMem ELEndAdr;
//extern ElogFlashMem ELReadAdr;

void EventLog_Set_CANRxEvent(bool onoff);
void EVENTLOG_BATTERY_MANAGE(void);
void EVENTLOG_PROTECTION(void);
void EVENTLOG_CAN_Receive(CAN_MSG_OBJ *pDestBuf);
//void EVENTLOG_CAN_Transmit(CAN_MSG_OBJ *pDestBuf);
void EVENTLOG_CAN_Transmit(uCAN_MSG *pDestBuf);
void EVENTLOG_CFET_DFET_TRICKLE(void);
void EVENTLOG_BATTERY_STATE_CHANG(void);
void EVENTLOG_SOH(void);
void EVENTLOG_LED(void);
void updateLdatData();
void ProcessEventData();
#endif

#ifdef	__cplusplus
}
#endif

#endif /* EVENTLOG_H */


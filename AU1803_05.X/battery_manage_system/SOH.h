/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _SOH_H
#define _SOH_H
#include <stdint.h>
#include "../battery_manage_system/battery_manage_system.h"
#include "../flash/MX25L.h"
#include "../can/can.h"


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */


typedef struct
{
    uint8_t     efficient;      // AA is useing
    uint32_t    flashaddress;   // flash next address
    uint16_t    DecaySOH;       // FCC value last time learning from gauge
    uint16_t    cycles;         // discharge cycle
    uint16_t    DSGFCC;     // total capacitance WH
    uint16_t    Coulomb_WH;     // total capacitance WH
    uint32_t    Coulomb_100mWS; // total capacitance 100mWS
    uint16_t    Crc;            // data chack crc
} __attribute__ ((__packed__)) SOHInfo_t; //16bytes

#define SOHFlashAddress             (uint32_t)0x010000        //
#define SOH_FLASH_AREA_SIZE         0x1000          //4K

#define DYNAMIC_K_VALUE_ADR         (uint32_t)0x020000
#define DYNAMIC_K_FLASH_AREA_SIZE   0x1000          //4K
#define DYNAMIC_K_SIZE              14
typedef struct
{
    uint8_t     efficient;                      // AA is useing
    uint32_t    flashaddress;                   // flash next address
    uint16_t    K_VALUE[DYNAMIC_K_SIZE];        // FCC value last time learning from gauge
    uint16_t    K_5oC_VALUE[DYNAMIC_K_SIZE];    // FCC value last time learning from gauge
    uint16_t    Crc;                            // data chack crc
} __attribute__ ((__packed__)) K_VALUE_Info_t;  //16bytes

#define SOH_INITIAL             100             //100%
//**********************************
// Variable Declare
//**********************************
extern SOHInfo_t SOH_STATE;
extern uint32_t RC_calculated_gain;
extern uint16_t dsgFCC;
extern uint8_t rsoc;
extern uint32_t uiCalRCapacity;
extern uint32_t uiCalRCapacity100mWs;;
extern uint16_t kVal[DYNAMIC_K_SIZE];
extern uint16_t kVal5oC[DYNAMIC_K_SIZE];
extern const uint16_t kVal1803_Default[DYNAMIC_K_SIZE];
extern const uint16_t kVal5oC1803_Default[DYNAMIC_K_SIZE];
extern const uint16_t kVal1805_Default[DYNAMIC_K_SIZE];
extern const uint16_t kVal5oC1805_Default[DYNAMIC_K_SIZE];
extern uint16_t kVal1checksum;
extern K_VALUE_Info_t K_VALUE_STATE;
//**********************************
// Funcition Declare
//**********************************
int32_t SOH_Write_Flash(uint32_t adrress, SOHInfo_t *data);
uint16_t SOH_GetDecySOH(void);
uint16_t SOH_GetDecayFCC(void);
uint16_t SOH_GetDecayFC(void);
uint16_t SOH_Get_TotalCoulomb(void);
uint16_t SOH_Get_DeepDISChargeCycle(void);
void SOH_Init_while(void);
void SOH_Init(void);

void Get_Dynamic_Data(void);
void SmartStudyAdjKvalue(uint8_t device_id);
void IdleDischargeLostCheck(void);
void RC_old(void);

#endif	//_SOH_H__

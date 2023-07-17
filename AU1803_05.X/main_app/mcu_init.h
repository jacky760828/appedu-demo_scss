/******************************************************************************
 *
 * FILE: mcu_init.h
 *
 * DESCRIPTION: It is responsible for the initialization of the peripheral
 *              of micro-controller.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * Many aspects of the MCU are initialized by using functions exposed through
 * this module.
 *
 *****************************************************************************/
#ifndef MCU_INIT_H
#define MCU_INIT_H
#include "hw_abstraction.h"
#include"../mcc_generated_files/traps.h"
#include "../battery_manage_system/battery_manage_system.h"

typedef enum {
    ON,
    OFF,
    KEEP,
    TOGGLE,
} FET_SETTING;

typedef enum {
    DISCHRG         = 0x00,
    DISCHRG_FAST    = 0x01,
    DISCHRG_NORMAL  = 0x02,
    ENCHRG          = 0x10,
    ENCHRG_FAST     = 0x11,
    ENCHRG_NORMAL   = 0x12,
    HOLDING,
} TRICKLE_MODE;

typedef struct {
    uint8_t CFET            :1;
    uint8_t DFET            :1;
    uint8_t TRICKLE         :1;
    uint8_t TRICKLE_MODE    :1;
}FETBits_t;

#define CAN_DEFAULT_RBC_ADDRESS     0x0B
#define CAN_DEFAULT_XLBP_ADDRESS    0x0A
#define CAN_HW_Error_ADDRESS        0xFF

#define DFET_Pre_time               5
#define DFET_ON_time                1

extern CutTime_t DFET_Pre;
extern CutTime_t DFET_ON;
extern uint8_t d_deviceAddr;
extern uint8_t FixDeviceID;

void Read_Port(void);
uint16_t GetFETsState(void);
void McuInit_port(void);
void TRAPS_halt_on_error(uint16_t code);
void Mcu_set_CFET_DFET_TRICKLE(FET_SETTING CFET, FET_SETTING DFET, TRICKLE_MODE mode);

#endif	// MCU_INIT_H
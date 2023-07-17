/******************************************************************************
 *
 * FILE: smbus.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/

#ifndef SMBUS_H
#define	SMBUS_H

#include <stdint.h>
#include <stdbool.h>
#include "smbus_cmd.h"
#include "i2c_crc_table.h"
#include "../i2c/i2cstate.h"
#include "../battery_manage_system/battery_manage_system.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define BQ78350_I2C_BUFF I2C_SMB_BUFF

#define SLAVE_DEVICE_ADDR                   0x16        // 7bits , 8bit 0x16
#define I2C_INTERVAL_TIME                   15 //50  //15        // in mini-second  per smbus request
#define I2C_MAX_COMM_TIME                   50          // in mini-second

extern uint8_t d_smbusCmdIdx;
    
void SMBUS_BQ78350_SBS_Cmd(uint8_t cmd);
bool SMBUS_is_ShutDown_Run(void);
void SMBUS_setShutDownEnable(bool onoff);
bool SMBUS_getShutDownEnable(void);
uint8_t SMBUS_getStateOfHealth(void);
uint16_t SMBUS_getCycleCount(void);
uint16_t SMBUS_getGaugingStatus(void);
void SMBUS_init(void);
void SMBUS_mainFunction(void);
bool SMBUS_isFree(void);
uint16_t SMBUS_FIND_CMDINDEX(uint8_t cmd);
bool BQ78350_all_info_ready(void);
void SMBUS_sortMinMaxCellVolt(void);
uint16_t SMBUS_getMinCellVolt(void);
uint16_t SMBUS_getMaxCellVolt(void);
int16_t SMBUS_getCellTemperature(void);
int16_t SMBUS_getFetTemperature(void);
uint16_t SMBUS_getGaugeRC(void);
uint16_t SMBUS_getGaugeFCC(void);
uint16_t SMBUS_getMinTemperature(void);
uint16_t SMBUS_getMaxTemperature(void);
void SMBUS_Temperature_info(void);
uint16_t SMBUS_getAFEConfig(void);
void SMBUS_sortRecMinMaxCellVolt(void);
uint16_t SMBUS_getVoltage(void);
int16_t SMBUS_getCurrent(void);
uint16_t SMBUS_getChargingCurrent(void);
uint16_t SMBUS_getChargingVoltage(void);
uint16_t SMBUS_getSerialNumber(void);
uint16_t SMBUS_getManufactureDate(void);
uint16_t SMBUS_getDesignCapacity(void);
uint16_t SMBUS_getCDFETs(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SMBUS_H */


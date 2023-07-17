/******************************************************************************
 *
 * FILE: i2c_crc_table.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef _I2C_CRC_TABLE_H
#define _I2C_CRC_TABLE_H

#include <stdint.h>

uint8_t I2C_crc8Calc(uint8_t crcResult, uint8_t newData);

#endif  // _I2C_CRC_TABLE_H
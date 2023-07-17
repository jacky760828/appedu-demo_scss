/******************************************************************************
 *
 * FILE: pack_config.h
 *
 * DESCRIPTION: 
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 *
 *****************************************************************************/
#ifndef PACK_CONFIG_H
#define	PACK_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../battery_manage_system/battery_manage_system.h"

    
#define SKU_NAME_DEFAULT_RBC_STRING         "AU1803" //AU1803, AU1805
#define SKU_NAME_DEFAULT_XBP_STRING         "AU1805" //AU1803, AU1805
#define PACK_DESIGN_POWER_DEFAULT           3200        // Unit: W
#define PACK_PARALLEL_RBC_NUMBER_DEFAULT    3
#define PACK_PARALLEL_XBP_NUMBER_DEFAULT    14

#define CELL_SPEC_VOLTAGE                   3.6f                                            //3.6V
#define CELL_SPEC_CAPACITY                  (2000 * 0.9)                                    //2000mAh*0.9 = 1800mAh Before and after 5% do not use
#define CELL_IN_SERIES                      14                                              //14 strings form VBAT
#define PACK_VOLTAGE                        (CELL_SPEC_VOLTAGE * CELL_IN_SERIES)            //total battery voltage 3.6V*14 = 50.4V
#define RBC_FULL_CHARGE_CAPACITY_WH_DEF     272                                             //50.4V * 5400mAh = 272160mWh = 272Wh
#define XBP_FULL_CHARGE_CAPACITY_WH_DEF     1270                                            //50.4V * 25200mAh = 1270080mWh = 1270Wh


#ifdef	__cplusplus
}
#endif

#endif	/* PACK_CONFIG_H */


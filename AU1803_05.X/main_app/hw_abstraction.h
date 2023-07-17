/******************************************************************************
 *
 * FILE: hw_abstraction.h
 *
 * DESCRIPTION: It is responsible for the initialization of the peripheral
 *              of micro-controller.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * This Header file contains all the hardware pin definitions.
 *
 *****************************************************************************/
#ifndef HW_ABSTRACTION_H
#define HW_ABSTRACTION_H

#include "../mcc_generated_files/pin_manager.h"
#define FCY    _XTAL_FREQ/2  //  frank add  6/11
#include "Libpic30.h"
/******************************************************************************
* Pin definition
******************************************************************************/
#define HWA_76200_DFET_OFF()                HWA_76200_DFET_SetHigh()
#define HWA_76200_DFET_FREE()                 HWA_76200_DFET_SetLow()
#define HWA_76200_DFET_TOGGLE()             HWA_76200_DFET_Toggle()
#define HWA_76200_DFET()                    HWA_76200_DFET_GetValue()
#define HWA_76200_CFET_OFF()                HWA_76200_CFET_SetHigh()
#define HWA_76200_CFET_FREE()                 HWA_76200_CFET_SetLow()
#define HWA_76200_CFET_TOGGLE()             HWA_76200_CFET_Toggle()
#define HWA_76200_CFET()                    HWA_76200_CFET_GetValue()
#define HWA_CAN_BUS_DRIVER_STANDBY()        HWA_CAN_BUS_STANDBY_SetHigh()
#define HWA_CAN_BUS_DRIVER_NORMAL()         HWA_CAN_BUS_STANDBY_SetLow()
#define HWA_CAN_BUS_DRIVER_TOGGLE()         HWA_CAN_BUS_STANDBY_Toggle()
#define HWA_CAN_BUS_DRIVER()                HWA_CAN_BUS_STANDBY_GetValue()
#define HWA_TRICKLE_CHRG_MODE_FAST()        HWA_TRICKLE_CHRG_FAST_SetHigh()
#define HWA_TRICKLE_CHRG_MODE_NORMAL()      HWA_TRICKLE_CHRG_FAST_SetLow()
#define HWA_TRICKLE_CHRG_MODE_TOGGLE()      HWA_TRICKLE_CHRG_FAST_Toggle()
#define HWA_TRICKLE_CHRG_MODE()             HWA_TRICKLE_CHRG_FAST_GetValue()
#define HWA_TRICKLE_CHRG_DISABLE()          HWA_TRICKLE_CHRG_ENABLE_SetLow()
#define HWA_TRICKLE_CHRG_ENABLE()           HWA_TRICKLE_CHRG_ENABLE_SetHigh()
#define HWA_TRICKLE_CHRG_TOGGLE()           HWA_TRICKLE_CHRG_ENABLE_Toggle()
#define HWA_TRICKLE_CHRG()                  HWA_TRICKLE_CHRG_ENABLE_GetValue()
#define HWA_LED2_OFF()                      HWA_LED2_SetLow()////HWA_LED2_SetHigh() //20200630 Jason Lin LED pin define change
#define HWA_LED2_ON()                       HWA_LED2_SetHigh()////HWA_LED2_SetLow()
#define HWA_LED2_TOGGLE()                   HWA_LED2_Toggle()
#define HWA_LED2()                          HWA_LED2_GetValue()
#define HWA_LED1_OFF()                      HWA_LED1_SetLow()////HWA_LED1_SetHigh() //20200630 Jason Lin LED pin define change
#define HWA_LED1_ON()                       HWA_LED1_SetHigh()////HWA_LED1_SetLow()
#define HWA_LED1_TOGGLE()                   HWA_LED1_Toggle()
#define HWA_LED1()                          HWA_LED1_GetValue()
#define HWA_SMBUS_CMD_OFF()                 HWA_SMBUS_DISABLE_GetValue()
//20200615 Jason Lin add pre-discharge function
#define HWA_Pre_Discharge_ENABLE()       HWA_Predischarge_SetHigh()
#define HWA_Pre_Discharge_DISABLE()        HWA_Predischarge_SetLow()

// test frank
#define HWA_Fuse_cut_off_ON()                        HWA_Fuse_cut_off_SetHigh()
#define HWA_Fuse_cut_off_OFF()                       HWA_Fuse_cut_off_SetLow()
#define HWA_Fuse_cut_off_TOGGLE()                    HWA_Fuse_cut_off_Toggle()

#define HWA_WAKEUP_PIN()                    HWA_SYS_WAKE_UP_ACTIVE_GetValue()
//#define HWA_RB9_ON()                        HWA_CAN_BUS_STANDBY_SetLow()
//#define HWA_RB9_OFF()                       HWA_CAN_BUS_STANDBY_SetHigh()
//#define HWA_RB9_TOGGLE()                    HWA_CAN_BUS_STANDBY_Toggle()


//#define HWA_S3_BTN_PRESS()                 !IO_RA8_GetValue()
//#define HWA_S6_BTN_PRESS()                 !IO_RB4_GetValue()

/******************************************************************************
* DEBUG
******************************************************************************/
// DEBUG_SW
//#define HWA_DEBUG_0_SW_STATUS()         HWA_DEBUG_0_GetValue()
//#define HWA_DEBUG_0_SW_ON()             HWA_DEBUG_0_SetHigh()
//#define HWA_DEBUG_0_SW_OFF()            HWA_DEBUG_0_SetLow()
//#define HWA_DEBUG_0_SW_TOGGLE()         HWA_DEBUG_0_Toggle()

//#define HWA_DEBUG_2_SW_STATUS()         HWA_DEBUG_2_GetValue()
//#define HWA_DEBUG_2_SW_ON()             HWA_DEBUG_2_SetHigh()
//#define HWA_DEBUG_2_SW_OFF()            HWA_DEBUG_2_SetLow()
//#define HWA_DEBUG_2_SW_TOGGLE()         HWA_DEBUG_2_Toggle()

//#define HWA_DEBUG_1_SW_STATUS()         HWA_DEBUG_1_GetValue()
//#define HWA_DEBUG_1_SW_ON()             HWA_DEBUG_1_SetHigh()
//#define HWA_DEBUG_1_SW_OFF()            HWA_DEBUG_1_SetLow()
//#define HWA_DEBUG_1_SW_TOGGLE()         HWA_DEBUG_1_Toggle()
//
//#define HWA_DEBUG_3_SW_STATUS()         HWA_DEBUG_3_GetValue()
//#define HWA_DEBUG_3_SW_ON()             HWA_DEBUG_3_SetHigh()
//#define HWA_DEBUG_3_SW_OFF()            HWA_DEBUG_3_SetLow()
//#define HWA_DEBUG_3_SW_TOGGLE()         HWA_DEBUG_3_Toggle()

#endif	// HW_ABSTRACTION_H
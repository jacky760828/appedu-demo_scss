/******************************************************************************
 *
 * FILE: version_config.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "version_config.h"

const uint8_t protocolVersion[1]  __attribute__((space(psv),  address(0x6000))) = {PROTOCOL_VERSION} ; //1
const uint8_t fwVersionMajor[1]  __attribute__((space(psv), address(0x6002))) = {FIRMWARE_VERSION_MAJOR}; //2
const uint8_t fwVersionMinor[1]  __attribute__((space(psv), address(0x6004))) = {FIRMWARE_VERSION_MINOR}; //3
const uint8_t fwVersionBuild[1] __attribute__((space(psv), address(0x6006))) = {FIRMWARE_VERSION_BUILD}; //4
const uint8_t fwBuildDate[] __attribute__((space(psv), address(0x6008))) = __DATE__; //frank 8/6 , 12 bytes  "Jul 04 2021" //4
const uint8_t fwBuildTime[] __attribute__((space(psv), address(0x6014))) = __TIME__; //frank 8/6 , 9 bytes   "15:05:45" //8
/*
 * 
 * 
 *  Take External Oscillator : 16 Mhz
 *  PLL x 4 => System frequency: 64000000
    ECAN clock frequency: 64000000
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *  
Firmware Revision / Version History ========================================================================
Date:       Rev:   Author:    Checksum / Description:
+-----------+------+----------+-----------------------+-----------------------
 * 08/07 Alan Chen merge UL 60730 test code with T6_29ModifyV03_frank_00 <= keep rev 44
 *  Alan_20200712 Add Class B Library for UL60730 test
 *  This code shoule be current testing only.  It should not go to this DVT2 Build.
 *
 * 08/04
 *  base on the merge code from T6_29Modify03 => Become T6_29ModifyV03_frank_00
 *  1. removed  problem codes (interrupt rontine cause delay . (jason,Jammar))
 *  2. remove duplicate code in battery_manage_system.c 
 *  3. remove redunant code that change lostCommunication flag, could case learning preocess been interrupted (C/DFETturn off).
 *
 * 20/Jul/2020 T6_29ModifyV03 FW rev 044. <= keep rev 44
 * Alan Merge new code of Jammar (Jammar_20200717_17 FW rev 044).
 * Alan Fix previous lost while merging (Frank_20200713_1) : 1. Code about SMBUS_sortMinMaxTemperature(); //frank 7/8
 *                                                           2. Code about d_safeTime
 * 
 * *30/JUN/2020 T6_29ModifyV02 FW rev 044.
 * 20200717 Jammar Chiang add power off in idle state
 * 20200717 Jammar Chiang fix No 15 pSvdVolt->vBusTooLowLongTime = FALSE
 * 
 * 15/Jul/2020 T6_29ModifyV02 FW rev 043.
 * ReMerge  Frank_20200713_1 and Jason_20200713_17 (base on Frank_20200713_1)

 * 14/Jul/2020 T6_29ModifyV01 FW rev 043.
 * Merge  Frank_20200713_1 and Jason_20200713_17 (base on Jason_20200713_17)

 * *13/JUL/2020 T6_26_ModifyV23 FW rev 042.
 * 20200713 Jason Lin fix ODC current -120A
 * 
 * *10/JUL/2020 T6_26_ModifyV23 FW rev 041.
 * 20200710 Jason Lin disable FirstDetection function for inproof
 * 
 * *08/JUL/2020 T6_26_ModifyV22 FW rev 040.
 * frank 6/29 Avg value has problem need check
 * 
 * *08/JUL/2020 T6_26_ModifyV21 FW rev 039.
 * 20200708 Jason Lin fix ODC current -150A change -120A 
 * 
 * *07/JUL/2020 T6_26_ModifyV20 FW rev 038.
 * 20200707 Jason Lin fix it
 * 
 * *30/JUN/2020 T6_26_ModifyV19 FW rev 037.
 * 20200701 Jason Lin delete NO.32 & NO.41
 * 20200701 Jason Lin add ODC function
 * 20200701 Jason Lin change state diagram
 * 
 * *30/JUN/2020 T6_26_ModifyV16 FW rev 034.
 * 20200630 Jason Lin LED pin define change
 * 
 * * 26/JUN/2020 T6_26_ModifyV15  FW rev 033.
 * 20200629 Jason Lin disable
 * 
 * * 26/May/2020 T6_26_ModifyV14  FW rev 032.
 * ESD test
 * 
 * * 26/May/2020 T6_26_ModifyV13  FW rev 031.
 * 20200615 Jason Lin add pre-discharge function
 * 20200618 Jason Lin modify
 * 
 * For PIC33. Jason_20200622_09:  //20200622 Jason Lin change 10 min => 10 sec
 * 
 * 
 * For PIC33. Alan_20200619_14:  Alan merge Jason_20200618_11 to current code (Improve FW of Class B Library for UL60730 test)

* For PIC33.  Jason_20200618_11
 *  * * 18/Jun/2020 T6_27_ModifyV10  FW rev 029. For PIC33.
 * 20200618 Jason Lin fix RB10 disable Pull Down function.
 * 
 * 
 * 
 * For PIC33.  Frank_20200617_17
 6/17   search "frank 6/' key word

1. add shutdown command to let gas-gauge turn off power safely through REG out pin of AFE. 
   a. update all files in smbus folder
   b. update can_dbc.c, can_dbc.h 

2. add clock failure safe function of pic33
   update MyConfig.mc3 file, change configure word to enable this function
   update all files in MCC Generated Files  folder
   update   mcu_init.c, mcu_init.h  in main_app folder

 3. modify can_dbc_usages_vda.c and can_dbc_usages_mdat.c files in can folder to report FETs status
 
 
 * For PIC33. 15/Jun/2020 Alan_20200615_15 Add Class B Library for UL60730 test
 * 
 * For PIC33. 09/Jun/2020 Alan_20200609_09 : Merge Jason_20200608_15 + Alan_20200605_17
 * 
 * For PIC33. 05/Jun/2020 Alan_20200605_17 : Merge Jason_20200605_17 + Alan_20200605_16 Add Class B Library for basic build code testing

 * * 28/May/2020 T6_27_ModifyV09  FW rev 027. For PIC33.
 * 20200528 Jason Lin modify it for New State Diagram, Add
 * 
 * * 27/May/2020 T6_26_ModifyV08  FW rev 026. For PIC18.
 * 20200527 Jason Lin modify the EOCC level due to not able to get EOCV flag. Instead we use this workaround method
 * 
 * * 26/May/2020 T6_26_ModifyV07  FW rev 025.
 * 20200526 Jason Lin add reset_all_status() in bmsidle
 * 
 * * 26/May/2020 T6_26_ModifyV06  FW rev 024.
 * 20200526 Jason Lin add pSvdCmd->cmdChrgEnable = FALSE; d_cmdChargeEnable = FALSE; d_cmdDischargeEnable = FALSE; pSvdCmd->cmdDisChrgEnable = FALSE; in path No 15
 * change priority path no 12 > path no 64 ==> path no 64 > path no 12
 * 
 * 26/May/2020 T6_26_ModifyV05  FW rev 023.
 * 20200526 Frank Tsai fix CanDbcUtils_putDataIntoDcbFrame_MoveOneBit
 * 
26/May/2020 T6_26_ModifyV04  FW rev 022.
 * 20200526 Alan Chen fix EOCV flag
 * Also remove those 36 bits CanBus Protection Bytes due to not fully confirmed yet.
 * 
15/May/2020 T6_26_ModifyV03  FW rev 021.
 * 20200522 for fix init vol out bug.  Is mainly for "Lost Communication Test"
 * 
15/May/2020 T6_26_ModifyV01  FW rev 020.
 * Add pSvdCmd->cmdDisChrgEnable,  add for current test requirement at vendor and Factory (need to find root cause why no response to command in the future)
 * 
 * 
14/May/2020 T6_26_ModifyV01  FW rev 019.
 * Set d_canConnected = TRUE; for default value
 * Set packOverVoltage for bmsIs_C_FaultOccurr.
 * Set packUnderVoltage for bmsIs_D_FaultOccurr
 * Update FW of 3 APC samples
 * 
8/May/2020 T6_25_ModifyV01 
 * Release this FW rev 018 to Factory for DVT 25 pcs production
 * Send 3 Sample Units (EVT 3 hardware sample + this FW by rev no = 017) to APC

T6_24_ModifyV04.  Fix some issues.
   EE finish State Diagram test with all items pass.

T6_22 Fix Battery voltage reading.  Also add bmsIsUL60730FaultOccurr() and its judgment. FW rev Protocol/ Major/ Minor/ Build = 1013

T6_20 Fix some typo.  Also Set Brown out reset to be triggered by FW. FW rev Protocol/ Major/ Minor/ Build = 1012

T6_19 20200318 Alan Chen modify for new State Diagram
 Update FW to for Fisher Cat - BMS state Machine v1.7_20200324.vsd
 can_dbc_usages_vdat.c (20200331 17:48 VDAT CAN message issue fixed : be merged)
 APC_Fisher Cat_Protection List C-Tech 20200327_SK_Udpdate.xlsx be implemented. For those protection that will be done by AFE (said: Active Device=AFE). We need new AFE parameters be applied to AFE.
 FW rev Protocol/ Major/ Minor/ Build = 1011.
 
T6_17_Merge_T6_13_Frank_and_T6_15
 
  
T6_14_A 20200318 Alan Chen modify for new State Diagram
 
20/Feb/2020 T6_13_ModifyUARTPinForNewPCBA 23/Mar/2020 be released to for production test. FW rev Protocol/ Major/ Minor/ Build = 1001.  Keep status of : Fisher Cat - BMS state Machine v0c - 20191212.pdf
  Set Device as AU1803 
  CAN_DBC_LOST_DETECT_OFF_DEBUG_ON disabled.
 
T6_13_Check_UART2 Works for New PCBA (UART <pin 44: RC6 & Pin 1:RC7> parallel connect to <Pin 10:TXD:CanH & Pin 11:RXD:CanL>)  




 
+-----------+------+----------+-----------------------+-----------------------
20200204 Alan Chen collect rearrange all fault condition

20200206 Alan Chen modify prevent from keep state changing after Low Battery Shutdown, Add

20200120 Alan Chen remove due to not use SMBUS_resetMaxCellVolt, Remove

20200120 Alan Chen update to add Low Temperature strategy, Remove & Add

20200116 Alan Chen remove PWM2 due to not use it (pin 36) and AN4 (pin 24)) (Hardware not implement)

20200116 Alan Chen Update to have time buffer to response data reading, Add

20200116 Alan Chen remove due to not use SMBUS_getFetTemperature, Remove

20200116 Alan Chen remove due to not use SMBUS_retriveFetTemperature, Remove

20200116 Alan Chen add limit: cellHiTemperature to meet new state diagram, Add

20200116 Alan Chen change SMBus from 0x9F to 0xFF for baudrate from 100 KHz to 62.5KHz
+-----------+------+----------+-----------------------+-----------------------
*/
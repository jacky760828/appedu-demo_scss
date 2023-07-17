/******************************************************************************
 *
 * FILE: checkSafety.c
 *
 * It is responsible for the initialization of the peripheral
 * of micro-controller.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: This component contains all the peripheral initialization
 * functions of the MCU.
 *
 *****************************************************************************/

#include "../sys_time/sys_time.h"
#include "../battery_manage_system/battery_manage_system.h"
#include "../main_app/hw_abstraction.h"
#include "checkSafety.h"
#include "../main_app/mcu_init.h"

int16_t EOCV_OVER_IBATT;
int16_t OCI_OVER_IBATT;

uint16_t log_COV_time = -1;
uint16_t log_POV_time = -1;
uint16_t log_EOCV_time = -1;
uint16_t log_CUV_time = -1;
uint16_t log_PUV_time = -1;
uint16_t log_OIMBV_time = -1;
uint16_t log_OCI_time = -1;
uint16_t log_ODI_1_time = -1;
uint16_t log_ODI_2_time = -1;
uint16_t log_ODI_3_time = -1;
uint16_t log_ODI_4_time = -1;
uint16_t log_H_ODI_time = -1;
uint16_t log_H_SCI_time = -1;
uint16_t log_COT_time = -1;
uint16_t log_COT_time1 = -1;
uint16_t log_DOT_time = -1;
uint16_t log_DOT_time1 = -1;
uint16_t log_CUT_time = -1;
uint16_t log_CUT_time1 = -1;
uint16_t log_DUT_time = -1;
uint16_t log_MOT_time = -1;
uint16_t log_UVPF_time = -1;
uint16_t log_OVPF_time = -1;
uint16_t log_OTPF_time = -1;
uint16_t log_CIMPF_time = -1;
uint16_t log_THOPF_time = -1;
uint16_t log_THSPF_time = -1;
uint16_t log_CFETPF_time = -1;
uint16_t log_DFETPF_time = -1;
uint16_t log_BQ_F_time = -1;

//typedef  void (MCUSafeFunc_t)(void);
//BmsSysProtectionBits_t sysProtection __attribute__ ((address(0x4000)));
BmsSysProtectionBits_t sysProtection;

MCU_FAULT MCU_Fault;

void COVSafeFun(void)
{
    if(!sysProtection.COVBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (cellVmax >= COV_OVER_VMAX)) {
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.COVBit = 1;
            Timer_Protection_stop_wait_timers(&COV_time);
            printf("\e[0;31m COV ON (WAKEUP && Cell_Vmax(%3.2fV) >= %3.2fV) \033[0m \r\n",(double)cellVmin ,(double)COV_OVER_VMAX);
        } else if(cellVmax >= COV_OVER_VMAX) {
            printf("\e[0;31m COV Cell_Vmax(%3.2fV) >= %3.2fV \033[0m",(double)cellVmin ,(double)COV_OVER_VMAX);
            if(Timer_Protection_timers_Fun(&COV_time, msec, COV_PROTECTION_DELAY, &log_COV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                sysProtection.COVBit = 1;
                Timer_Protection_stop_wait_timers(&COV_time);
                printf("\e[0;31m COV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&COV_time);
        }
    } else {
        if(iBatt < COV_UNOVER_IBATT) {
            printf("\e[1;32m COV OFF(iBatt(%ldmA) < %ldmA) \033[0m \r\n",(long int)iBatt ,(long int)COV_UNOVER_IBATT);
            sysProtection.COVBit = 0;
            Timer_Protection_stop_wait_timers(&COV_time);
        } else if((cellVmax <= COV_UNOVER_VMAX) && (Calculation_SOC <= COV_UNOVER_RSOC)) {
            printf("\e[0;31m COV OFF((Cell_Vmax(%3.2fV) <= %3.2fV) && (rsoc(%d%%) <= %d%%) \033[0m",(double)cellVmax ,(double)COV_UNOVER_VMAX ,Calculation_SOC ,COV_UNOVER_RSOC);
            if(Timer_Protection_timers_Fun(&COV_time, msec, COV_RELEASE_DELAY, &log_COV_time)) {
                printf("\e[1;32m COV OFF((Cell_Vmax(%3.2fV) <= %3.2fV) && (rsoc(%d) <= %d) \033[0m \r\n",(double)cellVmax ,(double)COV_UNOVER_VMAX ,Calculation_SOC ,COV_UNOVER_RSOC);
                sysProtection.COVBit = 0;
                Timer_Protection_stop_wait_timers(&COV_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&COV_time);
        }
    }
}

void POVSafeFun(void)
{
    if(!sysProtection.POVBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (vBat >= POV_OVER_VMAX)) {
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.POVBit = 1;
            Timer_Protection_stop_wait_timers(&POV_time);
            printf("\e[0;31m POV ON (WAKEUP && Pack(%3.2fV) >= %3.2fV) \033[0m \r\n",(double)vBat ,(double)POV_OVER_VMAX);
        } else if(vBat >= POV_OVER_VMAX) {
            printf("\e[0;31m POV Pack(%3.2fV) >= %3.2fV \033[0m",(double)vBat ,(double)POV_OVER_VMAX);
            if(Timer_Protection_timers_Fun(&POV_time, msec, POV_PROTECTION_DELAY, &log_POV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                sysProtection.POVBit = 1;
                printf("\e[0;31m POV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&POV_time);
        }
    } else {
        if(iBatt < POV_UNOVER_IBATT) {
            printf("\e[1;32m POV OFF(iBatt(%ldmA) < %ldA) \033[0m \r\n",(long int)iBatt ,(long int)POV_UNOVER_IBATT);
            sysProtection.POVBit = 0;
            Timer_Protection_stop_wait_timers(&POV_time);
        } else if(vBat >= POV_UNOVER_VMAX) {
            printf("\e[0;31m POV OFF Pack(%3.2fV) >= %3.2fV \033[0m",(double)vBat ,(double)POV_UNOVER_VMAX);
            if(Timer_Protection_timers_Fun(&POV_time, msec, POV_RELEASE_DELAY, &log_POV_time)) {
                printf("\e[1;32m POV OFF(Pack(%3.2fV) >= %3.2fV) \033[0m \r\n",(double)vBat ,(double)POV_UNOVER_VMAX);
                sysProtection.POVBit = 0;
                Timer_Protection_stop_wait_timers(&POV_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&POV_time);
        }
    }
}

void EOCVSafeFun(void)
{
    if(!sysProtection.EOCVBit) {
        if((cellVmin >= EOCV_SET_MIN_VOLT_TH) && ((iBatt > EOCV_SET_MIN_BITT) && (iBatt <= EOCV_OVER_IBATT))) {  // Cell_Vmin >= 3920mV and (1803)0A<I<=300mA?(1805)0A<I<1100mA  1S
            printf("\e[0;31m EOCV Cell_Vmin(%3.2fV) >= %3.2fV and %ldmA<%ldmA<=%ldmA \033[0m",(double)cellVmin ,(double)EOCV_SET_MIN_VOLT_TH ,(long int)EOCV_SET_MIN_BITT ,(long int)iBatt ,(long int)EOCV_OVER_IBATT);
            if(Timer_Protection_timers_Fun(&EOCV_time, msec, EOCV_PROTECTION_DELAY, &log_EOCV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                sysProtection.EOCVBit =  1;
                Timer_Protection_stop_wait_timers(&EOCV_time);
                Out_RC = Out_FCC;
                SOH_STATE.Coulomb_WH = Calculation_RC;
//                SmartStudyAdjKvalue(FixDeviceID);
                printf("\e[0;31m EOCV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&EOCV_time);
        }
    } else {
        if(iBatt < EOCV_UNSET_BITT) {                                                   // I < -3A
            printf("\e[1;32m EOCV OFF(iBatt(%ldmA) < %ldmA) \033[0m \r\n",(long int)iBatt ,(long int)EOCV_UNSET_BITT);
            sysProtection.EOCVBit = 0;
        } else if(Out_SOC <= EOCV_UNSET_RSOC) {                                                     // RSOC <= 95%
            printf("\e[1;32m EOCV OFF(RSOC%d <= %d%%) \033[0m \r\n",Out_SOC ,EOCV_UNSET_RSOC);
            sysProtection.EOCVBit = 0;
        } else {
        }
    }
}

void CUVSafeFun(void)
{
    if(!sysProtection.CUVBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (cellVmin <= CUV_SET_TH)) {
            Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
            sysProtection.CUVBit = 1;
            Timer_Protection_stop_wait_timers(&CUV_time);
            uiCalRCapacity = 0;
            uiCalRCapacity100mWs = 0;
            printf("\e[0;31m CUV ON (WAKEUP && CellVmin(%3.2fV) <= %3.2fV) \033[0m \r\n",(double)cellVmin ,(double)CUV_SET_TH);
        } else if(cellVmin <= CUV_SET_TH){
            printf("\e[0;31m CUV ON CellVmin(%3.2fV) <= %3.2fV \033[0m",(double)cellVmin ,(double)CUV_SET_TH);
            if(Timer_Protection_timers_Fun(&CUV_time, msec, CUV_PROTECTION_DELAY, &log_CUV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.CUVBit =  1;
                Timer_Protection_stop_wait_timers(&CUV_time);
                uiCalRCapacity = 0;
                uiCalRCapacity100mWs = 0;
                printf("\e[0;31m CUV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&CUV_time);
        }
    } else {
        if ((iBatt >= CUV_UNSET_IBATT) && (cellVmin >= CUV_UNSET_TH)) {
            printf("\e[0;31m CUV OFF iBatt(%ldmA) >= %ldA && Cell_Vmin(%3.2fV) >= %3.2fV \033[0m",(long int)iBatt ,(long int)CUV_UNSET_IBATT ,(double)cellVmin, (double)CUV_UNSET_TH);
            if(Timer_Protection_timers_Fun(&CUV_time, msec, CUV_RELEASE_DELAY, &log_CUV_time)) {
                printf("\e[1;32m CUV OFF \033[0m \r\n");
                sysProtection.CUVBit = 0;
                Timer_Protection_stop_wait_timers(&CUV_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&CUV_time);
        }
    }
}

void PUVSafeFun(void)
{
    if(!sysProtection.PUVBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (vBat <= PUV_SET_TH)) {
            Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
            sysProtection.PUVBit = 1;
            Timer_Protection_stop_wait_timers(&PUV_time);
            printf("\e[0;31m PUV ON (WAKEUP && Pack(%3.2fV) <= %3.2fV) \033[0m \r\n",(double)vBat ,(double)PUV_SET_TH);
        } else if(vBat <= PUV_SET_TH){
            printf("\e[0;31m PUV ON Pack(%3.2fV) <= %3.2fV \033[0m",(double)vBat ,(double)PUV_SET_TH);
            if(Timer_Protection_timers_Fun(&PUV_time, msec, PUV_PROTECTION_DELAY, &log_PUV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.PUVBit =  1;
                Timer_Protection_stop_wait_timers(&PUV_time);
                printf("\e[0;31m PUV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&PUV_time);
        }
    } else {
        if ((iBatt >= PUV_UNSET_IBATT) && (vBat >= PUV_UNSET_TH)) {
            printf("\e[0;31m PUV OFF iBatt(%ldmA) >= %ldA && Pack(%3.2fV) >= %3.2fV \033[0m",(long int)iBatt ,(long int)PUV_UNSET_IBATT ,(double)vBat, (double)PUV_UNSET_TH);
            if(Timer_Protection_timers_Fun(&PUV_time, msec, PUV_RELEASE_DELAY, &log_PUV_time)) {
                printf("\e[1;32m PUV OFF \033[0m \r\n");
                sysProtection.PUVBit = 0;
                Timer_Protection_stop_wait_timers(&PUV_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&PUV_time);
        }
    }
}

void OIMBVSafeFun(void)
{
    if(!sysProtection.OIMBVBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (cellVmax >= OIMBV_SET_MAX_VOLT) &&
          (cellVmaxmin >= OIMBV_SET_DELTA_VOLT) && (iBatt >= OIMBV_SET_IBATT)) {
            Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
            sysProtection.OIMBVBit = 1;
            Timer_Protection_stop_wait_timers(&OIMBV_time);
            printf("\e[0;31m OIMBV ON (WAKEUP && (CellVmax(%3.2fV) >= %3.2fV) && ((CellVmax - CellVmin(%3.2fV)) >= %3.2fV) && (iBatt(%ldmA) >= %ldmA) )\033[0m \r\n",
                    (double)cellVmax ,(double)OIMBV_SET_MAX_VOLT ,(double)cellVmaxmin ,(double)OIMBV_SET_DELTA_VOLT ,(long int)iBatt ,(long int)OIMBV_SET_IBATT);
        } else if((cellVmax >= OIMBV_SET_MAX_VOLT) && (cellVmaxmin >= OIMBV_SET_DELTA_VOLT) && (iBatt >= OIMBV_SET_IBATT)){
            printf("\e[0;31m OIMBV ON (CellVmax(%3.2fV) >= %3.2fV) && ((CellVmax - CellVmin(%3.2fV)) >= %3.2fV) && (iBatt(%ldmA) >= %ldmA) )\033[0m \r\n",
                    (double)cellVmax ,(double)OIMBV_SET_MAX_VOLT ,(double)cellVmaxmin ,(double)OIMBV_SET_DELTA_VOLT ,(long int)iBatt ,(long int)OIMBV_SET_IBATT);
            if(Timer_Protection_timers_Fun(&OIMBV_time, msec, OIMBV_PROTECTION_DELAY, &log_OIMBV_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.OIMBVBit =  1;
                Timer_Protection_stop_wait_timers(&OIMBV_time);
                printf("\e[0;31m OIMBV ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&OIMBV_time);
        }
    } else {
        if (cellVmaxmin <= OIMBV_UNSET_DELTA_VOLT) {
            printf("\e[0;31m OIMBV OFF (CellVmax - CellVmin(%3.2fV)) <= %3.2fV \033[0m",(double)cellVmaxmin, (double)OIMBV_UNSET_DELTA_VOLT);
            if(Timer_Protection_timers_Fun(&OIMBV_time, msec, OIMBV_RELEASE_DELAY, &log_OIMBV_time)) {
                printf("\e[1;32m OIMBV OFF \033[0m \r\n");
                sysProtection.OIMBVBit = 0;
                Timer_Protection_stop_wait_timers(&OIMBV_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&OIMBV_time);
        }
    }
}

void OCI_SafeFun(void)
{
    if(!sysProtection.OCIBit) {
        if(iBatt >=  OCI_OVER_IBATT) {
            printf("\e[0;31m OCI ON iBatt(%ldmA) >= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)OCI_OVER_IBATT);
            if(Timer_Protection_timers_Fun(&OCI_time, msec, OCI_PROTECTED_DELAY, &log_OCI_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
                sysProtection.OCIBit =  1;
                Timer_Protection_stop_wait_timers(&OCI_time);
                printf("\e[0;31m OCI ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&OCI_time);
        }
    } else {
        printf("\e[0;31m OCI OFF \033[0m");
        if(Timer_Protection_timers_Fun(&OCI_time, msec, OCI_RELEASE_DELAY, &log_OCI_time)) {
            printf("\e[1;32m OCI OFF \033[0m \r\n");
            sysProtection.OCIBit = 0;
            Timer_Protection_stop_wait_timers(&OCI_time);
        }
    }
}

void ODI_1SafeFun(void)
{
    if(!sysProtection.ODI_1Bit) {
        if(iBatt <=  ODI_1_SET_TH) {
            printf("\e[0;31m ODI_1 ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)ODI_1_SET_TH);
            if(Timer_Protection_timers_Fun(&ODI_1_time, msec, ODI_1_PROTECTION_DELAY, &log_ODI_1_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.ODI_1Bit =  1;
                Timer_Protection_stop_wait_timers(&ODI_1_time);
                printf("\e[0;31m ODI_1 ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&ODI_1_time);
        }
    } else {
        printf("\e[0;31m ODI_1 OFF \033[0m");
        if(Timer_Protection_timers_Fun(&ODI_1_time, msec, ODI_1_RELEASE_DELAY, &log_ODI_1_time)) {
            printf("\e[1;32m ODI_1 OFF \033[0m \r\n");
            sysProtection.ODI_1Bit = 0;
            Timer_Protection_stop_wait_timers(&ODI_1_time);
        }
    }
}

void ODI_2SafeFun(void)
{
    if(!sysProtection.ODI_2Bit) {
        if(iBatt <=  ODI_2_SET_TH) {
            printf("\e[0;31m ODI_2 ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)ODI_2_SET_TH);
            if(Timer_Protection_timers_Fun(&ODI_2_time, msec, ODI_2_PROTECTION_DELAY, &log_ODI_2_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.ODI_2Bit =  1;
                Timer_Protection_stop_wait_timers(&ODI_2_time);
                printf("\e[0;31m ODI_2 ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&ODI_2_time);
        }
    } else {
        printf("\e[0;31m ODI_2 OFF \033[0m");
        if(Timer_Protection_timers_Fun(&ODI_2_time, msec, ODI_2_RELEASE_DELAY, &log_ODI_2_time)) {
            printf("\e[1;32m ODI_2 OFF \033[0m \r\n");
            sysProtection.ODI_2Bit = 0;
            Timer_Protection_stop_wait_timers(&ODI_2_time);
        }
    }
}

void ODI_3SafeFun(void)
{
    if(!sysProtection.ODI_3Bit) {
        if(iBatt <=  ODI_3_SET_TH) {
            printf("\e[0;31m ODI_3 ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)ODI_3_SET_TH);
            if(Timer_Protection_timers_Fun(&ODI_3_time, msec, ODI_3_PROTECTION_DELAY, &log_ODI_3_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.ODI_3Bit =  1;
                Timer_Protection_stop_wait_timers(&ODI_3_time);
                printf("\e[0;31m ODI_3 ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&ODI_3_time);
        }
    } else {
        printf("\e[0;31m ODI_3 OFF \033[0m");
        if(Timer_Protection_timers_Fun(&ODI_3_time, msec, ODI_3_RELEASE_DELAY, &log_ODI_3_time)) {
            printf("\e[1;32m ODI_3 OFF \033[0m \r\n");
            sysProtection.ODI_3Bit = 0;
            Timer_Protection_stop_wait_timers(&ODI_3_time);
        }
    }
}

void ODI_4SafeFun(void)
{
    if(!sysProtection.ODI_4Bit) {
        if(iBatt <=  ODI_4_SET_TH) {
            printf("\e[0;31m ODI_4 ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)ODI_4_SET_TH);
            if(Timer_Protection_timers_Fun(&ODI_4_time, msec, ODI_4_PROTECTION_DELAY, &log_ODI_4_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.ODI_4Bit =  1;
                Timer_Protection_stop_wait_timers(&ODI_4_time);
                printf("\e[0;31m ODI_4 ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&ODI_4_time);
        }
    } else {
        printf("\e[0;31m ODI_4 OFF \033[0m");
        if(Timer_Protection_timers_Fun(&ODI_4_time, msec, ODI_4_RELEASE_DELAY, &log_ODI_4_time)) {
            printf("\e[1;32m ODI_4 OFF \033[0m \r\n");
            sysProtection.ODI_4Bit = 0;
            Timer_Protection_stop_wait_timers(&ODI_4_time);
        }
    }
}

void H_ODI_SafeFun()
{
    if(!sysProtection.H_ODIBit) {
        if(iBatt <=  H_ODI_SET_TH) {
            printf("\e[0;31m H_ODI ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)H_ODI_SET_TH);
            if(Timer_Protection_timers_Fun(&H_ODI_time, msec, H_ODI_PROTECTION_DELAY, &log_H_ODI_time)) {
                Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
                sysProtection.H_ODIBit =  1;
                Timer_Protection_stop_wait_timers(&H_ODI_time);
                printf("\e[0;31m H_ODI ON \033[0m \r\n");
            }
        } else {
            Timer_Protection_stop_wait_timers(&H_ODI_time);
        }
    } else {
        printf("\e[0;31m H_ODI OFF \033[0m");
        if(Timer_Protection_timers_Fun(&H_ODI_time, msec, H_ODI_RELEASE_DELAY, &log_H_ODI_time)) {
            printf("\e[1;32m H_ODI OFF \033[0m \r\n");
            sysProtection.H_ODIBit = 0;
            Timer_Protection_stop_wait_timers(&H_ODI_time);
        }
    }
}

void H_SCI_SafeFun()
{
    if(!sysProtection.H_SCIBit) {
        if(iBatt <=  H_SCI_SET_TH) {
            printf("\e[0;31m H_SCI ON iBatt(%ldmA) <= %ldmA )\033[0m \r\n",(long int)iBatt , (long int)H_SCI_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(KEEP,OFF,HOLDING);
            sysProtection.H_SCIBit =  1;
        }
    } else {
        printf("\e[0;31m H_SCI OFF \033[0m");
        if(Timer_Protection_timers_Fun(&H_SCI_time, msec, H_SCI_RELEASE_DELAY, &log_H_SCI_time)) {
            printf("\e[1;32m H_SCI OFF \033[0m \r\n");
            sysProtection.H_SCIBit = 0;
            Timer_Protection_stop_wait_timers(&H_SCI_time);
        }
    }
}

void COTSafeFun(void)
{
    BOOL ret_temp = 0,ret_ibatt = 0;
    BmsSvdTempBits_t *pSvdTemp = &d_thisBmsState.svdTemperature.bit;
    
    if(!sysProtection.COTBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (CellTempature_Max >= COT_SET_TH)) {
            printf("\e[0;31m COT ON (WAKEUP && Cell_T(%dC) >= %dC) \033[0m \r\n",CellTempature_Max , COT_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.COTBit =  1;
            Timer_Protection_stop_wait_timers(&COT_time);
            Timer_Protection_stop_wait_timers(&COT_time1);
        }
        if(iBatt > COT_SET_IBATT) {
            printf("\e[0;31m COT ON (iBatt(%ldmA) > %ldmA)\033[0m ",(long int)iBatt , (long int)COT_SET_IBATT);
            ret_ibatt = Timer_Protection_timers_Fun(&COT_time, msec, COT_PROTECTION_DELAY_TEMP, &log_COT_time);
        } else {
            Timer_Protection_stop_wait_timers(&COT_time);
        }
        if(CellTempature_Max >= COT_SET_TH) {
            printf("\e[0;31m COT ON (Cell_T(%dC) >= %dC)\033[0m ",CellTempature_Max , COT_SET_TH);
            ret_temp = Timer_Protection_timers_Fun(&COT_time1, msec, COT_PROTECTION_DELAY_IBATT, &log_COT_time1);
        } else {
            Timer_Protection_stop_wait_timers(&COT_time1);
        }
        if(ret_temp && ret_ibatt) {
            printf("\e[0;31m COT ON ((Cell_T(%dC) >= %dC over %dS) && (iBatt(%ldmA) > %ldmA over %dS) \033[0m \r\n",
                    CellTempature_Max , COT_SET_TH ,COT_PROTECTION_DELAY_TEMP ,(long int)iBatt , (long int)COT_SET_IBATT ,COT_PROTECTION_DELAY_IBATT);
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.COTBit =  1;
            Timer_Protection_stop_wait_timers(&COT_time);
            Timer_Protection_stop_wait_timers(&COT_time1);
        }
    } else {
        if(CellTempature_Max <= COT_UNSET_TH) {
            printf("\e[0;31m COT OFF (Cell_T(%dC) <= %dC)\033[0m \r\n",CellTempature_Max , COT_SET_TH);
            if(Timer_Protection_timers_Fun(&COT_time, msec, COT_RELEASE_DELAY, &log_COT_time)) {
                printf("\e[1;32m COT OFF \033[0m \r\n");
                sysProtection.COTBit = 0;
                Timer_Protection_stop_wait_timers(&COT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&COT_time);
        }
    }
    pSvdTemp->chrgHiTemperature = sysProtection.COTBit;
    pSvdTemp->cellHiTemperature = pSvdTemp->chrgHiTemperature;
}

void DOTSafeFun(void)
{
    BmsSvdTempBits_t *pSvdTemp = &d_thisBmsState.svdTemperature.bit;
    
    if(!sysProtection.DOTBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (CellTempature_Max >= DOT_SET_TH)) {
            printf("\e[0;31m DOT ON (WAKEUP && Cell_T(%dC) >= %dC) \033[0m \r\n",CellTempature_Max , DOT_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
            sysProtection.DOTBit =  1;
            Timer_Protection_stop_wait_timers(&DOT_time);
            Timer_Protection_stop_wait_timers(&DOT_time1);
        }
        if((CellTempature_Max >= DOT_SET_TH) && (iBatt <  DOT_SET_IBATT_TH)) {
            printf("\e[0;31m DOT ON ((Cell_T(%dC) >= %dC) && (iBatt(%ldmA) < %ldmA)) \033[0m \r\n",CellTempature_Max ,DOT_SET_TH ,(long int)iBatt ,(long int)DOT_SET_IBATT_TH);
            if(Timer_Protection_timers_Fun(&DOT_time, msec, DOT_PROTECTION_DELAY, &log_DOT_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.DOTBit =  1;
                Timer_Protection_stop_wait_timers(&DOT_time);
                Timer_Protection_stop_wait_timers(&DOT_time1);
            }
        } else {
            Timer_Protection_stop_wait_timers(&DOT_time);
        }
        if(CellTempature_Max >= DOT_IDLE_SET_TH) {
            printf("\e[0;31m DOT ON (Cell_T(%d?) >= %d?) \033[0m \r\n",CellTempature_Max ,DOT_IDLE_SET_TH);
            if(Timer_Protection_timers_Fun(&DOT_time1, msec, DOT_PROTECTION_DELAY, &log_DOT_time1)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.DOTBit =  1;
                Timer_Protection_stop_wait_timers(&DOT_time);
                Timer_Protection_stop_wait_timers(&DOT_time1);
            }
        } else {
            Timer_Protection_stop_wait_timers(&DOT_time1);
        }
    } else {
        if(CellTempature_Max <= DOT_UNSET_TH) {
            printf("\e[0;31m DOT OFF (Cell_T(%dC) <= %dC)\033[0m \r\n",CellTempature_Max , DOT_UNSET_TH);
            if(Timer_Protection_timers_Fun(&DOT_time, msec, DOT_PROTECTION_DELAY, &log_DOT_time)) {
                printf("\e[1;32m DOT OFF \033[0m \r\n");
                sysProtection.DOTBit = 0;
                Timer_Protection_stop_wait_timers(&DOT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&DOT_time);
        }
    }
    pSvdTemp->disChrgHiTemperature = sysProtection.DOTBit;
}
void CUTSafeFun(void)
{
    BOOL ret_temp,ret_ibatt;
    BmsSvdTempBits_t *pSvdTemp = &d_thisBmsState.svdTemperature.bit;
    
    if(!sysProtection.CUTBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (CellTempature_Max <= CUT_SET_TH)) {
            printf("\e[0;31m CUT ON (WAKEUP && Cell_T(%dC) <= %dC) \033[0m \r\n",CellTempature_Max , CUT_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.CUTBit =  1;
            Timer_Protection_stop_wait_timers(&CUT_time);
            Timer_Protection_stop_wait_timers(&CUT_time1);
        }
        
        if(CellTempature_Max <= CUT_SET_TH) {
            printf("\e[0;31m CUT ON (Cell_T(%dC) <= %dC) \033[0m \r\n",CellTempature_Max ,CUT_SET_TH);
            ret_temp = Timer_Protection_timers_Fun(&CUT_time, msec, CUT_PROTECTION_DELAY_TEMP, &log_CUT_time);
        } else {
            ret_temp = 0;
            Timer_Protection_stop_wait_timers(&CUT_time);
        }
        
        if(iBatt <  CUT_SET_IBATT_TH) {
            printf("\e[0;31m CUT ON (iBatt(%ldmA) < %ldmA) \033[0m \r\n",(long int)iBatt ,(long int)CUT_SET_IBATT_TH);
            ret_ibatt = Timer_Protection_timers_Fun(&CUT_time1, msec, CUT_PROTECTION_DELAY_IBATT, &log_CUT_time1);
        } else {
            ret_ibatt = 0;
            Timer_Protection_stop_wait_timers(&CUT_time1);
        }
        
        if(ret_temp && ret_ibatt) {
            printf("\e[0;31m CUT ON (((Cell_T(%dC) <= %dC) over %dS) && (iBatt(%ldmA) < %ldmA) over %dS \033[0m \r\n",
                    CellTempature_Max ,CUT_SET_TH, CUT_PROTECTION_DELAY_TEMP, (long int)iBatt, (long int)CUT_SET_IBATT_TH, CUT_PROTECTION_DELAY_IBATT);
            Mcu_set_CFET_DFET_TRICKLE(OFF,KEEP,HOLDING);
            sysProtection.CUTBit =  1;
            Timer_Protection_stop_wait_timers(&CUT_time);
            Timer_Protection_stop_wait_timers(&CUT_time1);
        }
    } else {
        if(CellTempature_Max >= CUT_UNSET_TH) {
            printf("\e[0;31m CUT OFF Cell_T(%dC) >= %dC )\033[0m \r\n",CellTempature_Max , CUT_UNSET_TH);
            if(Timer_Protection_timers_Fun(&CUT_time, msec, CUT_RELEASE_DELAY, &log_CUT_time)) {
                printf("\e[1;32m CUT OFF \033[0m \r\n");
                sysProtection.CUTBit = 0;
                Timer_Protection_stop_wait_timers(&CUT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&CUT_time);
        }
    }
    pSvdTemp->chrgLoTemperature = sysProtection.CUTBit;
}

void DUTSafeFun(void)
{
    BmsSvdTempBits_t *pSvdTemp = &d_thisBmsState.svdTemperature.bit;
    
    if(!sysProtection.DUTBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (CellTempature_Max <= DUT_SET_TH)) {
            printf("\e[0;31m DUT ON (WAKEUP && Cell_T(%dC) <= %dC) \033[0m \r\n",CellTempature_Max , DUT_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
            sysProtection.DUTBit =  1;
            Timer_Protection_stop_wait_timers(&DUT_time);
        } else if(CellTempature_Max <= DUT_SET_TH) {
            printf("\e[0;31m DUT ON (Cell_T(%dC) <= %dC) \033[0m \r\n",CellTempature_Max ,DUT_SET_TH);
            if(Timer_Protection_timers_Fun(&DUT_time, msec, DUT_PROTECTION_DELAY, &log_DUT_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.DUTBit =  1;
                Timer_Protection_stop_wait_timers(&DUT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&DUT_time);
        }
    } else {
        if(CellTempature_Max >= DUT_UNSET_TH) {
            printf("\e[0;31m DUT OFF Cell_T(%dC) >= %dC )\033[0m \r\n",CellTempature_Max , DUT_UNSET_TH);
            if(Timer_Protection_timers_Fun(&DUT_time, msec, DUT_RELEASE_DELAY, &log_DUT_time)) {
                printf("\e[1;32m DUT OFF \033[0m \r\n");
                sysProtection.DUTBit = 0;
                Timer_Protection_stop_wait_timers(&DUT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&DUT_time);
        }
    }
    pSvdTemp->disChrgLoTemperature = sysProtection.DUTBit;
}

void MOTSafeFun(void)
{
    if(!sysProtection.MOTBit) {
        if(BMS_Get_Operating_Flag(WAKEUP) && (FetTempature >= MOT_SET_TH)) {
            printf("\e[0;31m MOT ON (WAKEUP && Cell_T(%dC) >= %dC) \033[0m \r\n",FetTempature , MOT_SET_TH);
            Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
            sysProtection.MOTBit =  1;
            Timer_Protection_stop_wait_timers(&MOT_time);
        } else if(FetTempature >= MOT_SET_TH) {
            printf("\e[0;31m MOT ON (Cell_T(%dC) >= %dC) \033[0m \r\n",FetTempature ,MOT_SET_TH);
            if(Timer_Protection_timers_Fun(&MOT_time, msec, MOT_PROTECTION_DELAY, &log_MOT_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.MOTBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&MOT_time);
        }
    } else {
        if(FetTempature <= MOT_UNSET_TH) {
            printf("\e[0;31m MOT OFF (Cell_T(%dC) <= %dC)\033[0m \r\n",FetTempature , MOT_UNSET_TH);
            if(Timer_Protection_timers_Fun(&MOT_time, msec, MOT_RELEASE_DELAY, &log_MOT_time)) {
                printf("\e[1;32m MOT OFF \033[0m \r\n");
                sysProtection.MOTBit = 0;
                Timer_Protection_stop_wait_timers(&MOT_time);
            }
        } else {
            Timer_Protection_stop_wait_timers(&MOT_time);
        }
    }
}

void UVPFSafeFun(void)
{
    if(!sysProtection.UVPFBit) {
        if((iBatt >  UVPF_SET_IBATT) && (cellVmin <=  UVPF_SET_VCELL_MIN)) {
            printf("\e[0;31m CUVPF ON ((iBatt(%ldmA) < %ldmA) && (Cell_Vmin(%3.2fV) <= %3.2fV)) \033[0m \r\n",(long int)iBatt ,(long int)UVPF_SET_IBATT ,(double)cellVmin ,(double)UVPF_SET_VCELL_MIN);
            if(Timer_Protection_timers_Fun(&UVPF_time, sec, UVPF_PROTECTION_DELAY, &log_UVPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.UVPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&UVPF_time);
        }
    } else {
    }
}

void OVPFSafeFun(void)
{
    if(!sysProtection.OVPFBit) {
        if((iBatt >  OVPF_SET_IBATT) && (cellVmax >=  OVPF_SET_CELL_VMAX)) {
            printf("\e[0;31m CUVPF ON ((%ldmA < iBatt(%ldmA) && ((CellVmax(%3.2fV) >= %3.2fV))) \033[0m \r\n",(long int)iBatt ,(long int)OVPF_SET_IBATT ,(double)cellVmax ,(double)OVPF_SET_CELL_VMAX);
            if(Timer_Protection_timers_Fun(&OVPF_time, sec, OVPF_PROTECTION_DELAY, &log_OVPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.OVPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&OVPF_time);
        }
    } else {
    }
}

void OTPFSafeFun(void)
{
    if(!sysProtection.OTPFBit) {
        if((CellTempature_Max >= OTPF_SET_TH) && (iBatt <= OTPF_SET_IBATT_TH)) {
            printf("\e[0;31m OTPF ON ((Cell_T(%dC) >= %dC) && (iBatt(%ldmA) <= %ldmA) \033[0m \r\n",CellTempature_Max ,OTPF_SET_TH ,(long int)iBatt ,(long int)OTPF_SET_IBATT_TH);
            if(Timer_Protection_timers_Fun(&OTPF_time, sec, OTPF_PROTECTION_DELAY, &log_OTPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.OTPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&OTPF_time);
        }
    } else {
    }
}

void CIMPFSafeFun(void)
{
    if(!sysProtection.CIMPFBit) {
        if((cellVmax >= CIMPF_SET_VOLT_TH) && (cellVmaxmin >= CIMPF_SET_DELTA_VOLT)) {
            printf("\e[0;31m CIMPF ON ((cellVmax(%3.2fV) >= %3.2fV) && (Cell_Vmax - Cell_Vmin(%3.2fV) >= %3.2fV) \033[0m \r\n",(double)cellVmax ,(double)CIMPF_SET_VOLT_TH ,(double)cellVmaxmin ,(double)CIMPF_SET_DELTA_VOLT);
            if(Timer_Protection_timers_Fun(&CIMPF_time, sec, CIMPF_PROTECTION_DELAY, &log_CIMPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.CIMPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&CIMPF_time);
        }
    } else {
    }
}

void THOPFSafeFun(void)
{
    if(!sysProtection.THOPFBit) {
        if(CellTempature_Min == THOPF_SET_TEMP) {
            printf("\e[0;31m THOPF ON (Cell_T(%dC) == %dC) \033[0m \r\n",CellTempature_Min ,THOPF_SET_TEMP);
            if(Timer_Protection_timers_Fun(&THOPF_time, sec, THOPF_PROTECTION_DELAY, &log_THOPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.THOPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&THOPF_time);
        }
    } else {
    }
}

void THSPFSafeFun(void)
{
    if(!sysProtection.THSPFBit) {
        if(CellTempature_Min >= THSPF_SET_TH) {
            printf("\e[0;31m THSPF ON (Cell_T(%dC) >= %dC) \033[0m \r\n",CellTempature_Min ,THSPF_SET_TH);
            if(Timer_Protection_timers_Fun(&THSPF_time, sec, THSPF_PROTECTION_DELAY, &log_THSPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.THSPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&THSPF_time);
        }
    } else {
    }
}


void CFETPFSafeFun(void) // tsai 9/13
{    
    if(!sysProtection.CFETPFBit) {
        if((!HWA_76200_CFET() && (iBatt > CFETPF_SET_TH))) {
            printf("\e[0;31m CFETPF ON (CFET_OFF(%d) && (iBatt(%ldmA) > %ldmA) \033[0m \r\n",!HWA_76200_CFET() ,(long int)iBatt ,(long int)OTPF_SET_IBATT_TH);
            if(Timer_Protection_timers_Fun(&CFETPF_time, sec, CFETPF_PROTECTION_DELAY, &log_CFETPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.CFETPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&CFETPF_time);
        }
    } else {
    }
}

void DFETPFSafeFun(void) // tsai 9/13
{    
    if(!sysProtection.DFETPFBit) {
        if((!HWA_76200_DFET() && (iBatt < DFETPF_SET_TH))) {
            printf("\e[0;31m DFETPF ON (DFET_OFF(%d) && (iBatt(%ldmA) < %ldmA) \033[0m \r\n",!HWA_76200_DFET() ,(long int)iBatt ,(long int)DFETPF_SET_TH);
            if(Timer_Protection_timers_Fun(&DFETPF_time, sec, DFETPF_PROTECTION_DELAY, &log_DFETPF_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.DFETPFBit =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&DFETPF_time);
        }
    } else {
    }
}

// MCU Falut
void BQ_FSafeFun(void)
{
    if(!sysProtection.ULP_BQ_F) {
        if(MCU_Fault.BQFbit) {
            printf("\e[0;31m BQ_F ON (MCU_Fault.BQFbit(%d)) \033[0m \r\n",MCU_Fault.BQFbit);
            if(Timer_Protection_timers_Fun(&BQ_F_time, sec, BQ_F_PROTECTION_DELAY, &log_BQ_F_time)) {
                Mcu_set_CFET_DFET_TRICKLE(OFF,OFF,HOLDING);
                sysProtection.ULP_BQ_F =  1;
            }
        } else {
            Timer_Protection_stop_wait_timers(&BQ_F_time);
        }
    } else {
    }
}

void Protection_init(void)
{
    sysProtection.fivebytes[0]=0;
    sysProtection.fivebytes[1]=0;
    sysProtection.fivebytes[2]=0;
    sysProtection.fivebytes[3]=0;
    sysProtection.fivebytes[4]=0;
}

//-------------------------------------------------------------------------------------
void EventProtectedFunction(void)
{
    COVSafeFun();
    POVSafeFun();
    EOCVSafeFun();
    CUVSafeFun();
    PUVSafeFun();
    OIMBVSafeFun();
    OCI_SafeFun();
    ODI_1SafeFun();
    ODI_2SafeFun();
    ODI_3SafeFun();
    ODI_4SafeFun();
    H_ODI_SafeFun();
    H_SCI_SafeFun();
//    COTSafeFun();
//    DOTSafeFun();
//    CUTSafeFun();
//    DUTSafeFun();
    MOTSafeFun();
    UVPFSafeFun();
    OVPFSafeFun();
    OTPFSafeFun();
    CIMPFSafeFun();
//    THOPFSafeFun();
    THSPFSafeFun(); 
//    CFETPFSafeFun();
//    DFETPFSafeFun();
    BQ_FSafeFun();
}

void ProtectionMonitor(void)
{
    uint8_t index;
    BmsFaultFlag_t *pSvdFaultFlag;
    
    sysProtection.MOSsState = GetFETsState();
    EventProtectedFunction();
    if (sysProtection.fault_PFP)    
        sysProtection.MOSsState |= 0x08; // pf flag;
    // copy
    pSvdFaultFlag = &d_thisBmsState.svdFaultFlag;//.AllBytes.array[0];
    for( index=0; index < 5; index++) {
        pSvdFaultFlag->AllBytes.array[index] = sysProtection.fivebytes[index];
    }
}

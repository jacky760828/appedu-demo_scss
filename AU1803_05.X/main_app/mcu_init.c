/******************************************************************************
 *
 * FILE: mcu_init.c
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
#include "mcu_init.h"

uint8_t d_deviceAddr;
CutTime_t DFET_Pre;
CutTime_t DFET_ON;
char parnumRBC[]={'A','P','C','R','B','C','1','7','3','-','L','I'};//12 bytes
char parnumXBP[]={'S','R','T','L','5','0','R','M','B','P','1','U','-','L','I'}; //(XBP). 15 bytes
uint8_t FixDeviceID = 0;

/******************************************************************************
 *
 *****************************************************************************/
void Read_Port(void)
{
    FixDeviceID = CAN_ADDR_3_GetValue();
    FixDeviceID = (FixDeviceID << 1) | CAN_ADDR_2_GetValue();
    FixDeviceID = (FixDeviceID << 1) | CAN_ADDR_1_GetValue();
    FixDeviceID = (FixDeviceID << 1) | CAN_ADDR_0_GetValue();
        
    if(FixDeviceID == 0X0F) {
        FixDeviceID = CAN_DEFAULT_RBC_ADDRESS;      //0x0B
        EOCV_OVER_IBATT = EOCV_SET_MAX_BITT_RBC;
        OCI_OVER_IBATT = OCI_SET_IBATT_RBC;
        iOverTrickleChrgCurr = RBC_TRICKLE_CHARGE_CURRENT_20_PERCENT;
        pPartNum = parnumRBC;
        Ldat_DCIRString = 54;
        Ldat_DCIRStringMin = 54;
        Ldat_DCIRString = 54;
        d_DSGFCC = RBC_DSGFCC;
    } else {
        if(FixDeviceID == 0) {
            FixDeviceID = CAN_DEFAULT_XLBP_ADDRESS; //0x0A
        }
        EOCV_OVER_IBATT = EOCV_SET_MAX_BITT_XBP;
        OCI_OVER_IBATT = OCI_SET_IBATT_XBC;
        iOverTrickleChrgCurr = XBP_TRICKLE_CHARGE_CURRENT_20_PERCENT;
        pPartNum = parnumXBP;
        Ldat_DCIRString = 26;
        Ldat_DCIRStringMin = 26;
		Ldat_DCIRString = 26;
        d_DSGFCC = XBP_DSGFCC;
    }
    d_deviceAddr = FixDeviceID;
}

uint16_t GetFETsState(void)
{
    uint16_t smb_cdfets;
    uint16_t ret = 0x0000;
    
    smb_cdfets = SMBUS_getCDFETs();
    if(!HWA_76200_DFET()) { //low, free
        ret |= 0x0001; //can be on
    }
    if(!HWA_76200_CFET()) { //low, free 
        ret |= 0x0002; //can be on
    }
    
    if((smb_cdfets & 0x0004) == 0) { //DFET off
        ret &= 0xFFFE; //bit0
    }
    
    if((smb_cdfets & 0x0002) == 0) { //CFET off
        ret &= 0xFFFD; //bit1
    }
 
    if(d_chrgStatus) {
        ret |= 0x0004; //bit2
    }
    
    return ret;
}

void McuInit_port(void)
{
    HWA_Fuse_cut_off_OFF();
    HWA_CAN_BUS_DRIVER_STANDBY();
    HWA_Pre_Discharge_DISABLE();//tsai
    HWA_TRICKLE_CHRG_MODE_NORMAL();
    HWA_LED1_OFF();
    HWA_LED2_OFF();
    Read_Port();
    
    HWA_76200_DFET_OFF();
    HWA_76200_CFET_OFF();
    HWA_TRICKLE_CHRG_DISABLE();
    
    DFET_Pre.start = false;
    DFET_ON.start = false; 
}

// use FRC, fosc =64.02 Mhz, fcy=fosc/2=32.01 Mhz
static void CLOCK_FRCInitialize(void)
{
    // FRCDIV FRC/2; PLLPRE 4; DOZE 1:8; PLLPOST 1:2; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3102;
    // TUN Center frequency; 
    OSCTUN = 0x00;
    // ROON disabled; ROSEL FOSC; RODIV 0; ROSSLP disabled; 
    REFOCON = 0x00;
    // PLLDIV 137; 
    PLLFBD = 0x89;
    // AD1MD enabled; PWMMD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; QEI1MD enabled; SPI2MD enabled; SPI1MD enabled; C1MD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // IC4MD enabled; IC3MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; 
    PMD2 = 0x00;
    // CMPMD enabled; CRCMD enabled; I2C2MD enabled; 
    PMD3 = 0x00;
    // CTMUMD enabled; REFOMD enabled; 
    PMD4 = 0x00;
    // PWM2MD enabled; PWM1MD enabled; PWM3MD enabled; 
    PMD6 = 0x00;
    // PTGMD enabled; DMA0MD enabled; 
    PMD7 = 0x00;
    // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x01));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}

void TRAPS_halt_on_error(uint16_t code)
{
    if (code == TRAPS_OSC_FAIL) {
        CLOCK_FRCInitialize();
    } else {
        while(1);
    }
}

void Mcu_set_CFET_DFET_TRICKLE(FET_SETTING CFET, FET_SETTING DFET, TRICKLE_MODE mode)
{
    BmsSvdChrgCmdBits_t *pSvdChrgCmd = &d_thisBmsState.svdChargeCmd.bit;
    BmsSvdCmdBits_t *pSvdCmd = &d_thisBmsState.svdCommand.bit;
    bool ret;
    
    switch(CFET) {
        case ON:
            HWA_76200_CFET_FREE();
            break;
        case OFF:
            HWA_76200_CFET_OFF();
            break;
        case KEEP:
            break;
        case TOGGLE:
            HWA_76200_CFET_TOGGLE();
            break;
        default:
            break;
    }
    
    switch(DFET) {
        case ON:
            if((!DFET_Pre.start) && (vBus < VBUS_LOW_CHECK_TH)) {
                DFET_Pre.cut = SysTime_getMsec();
                DFET_Pre.start = 1;
                HWA_Pre_Discharge_ENABLE();
            }
            break;
        case OFF:
            HWA_76200_DFET_OFF();
            break;
        case KEEP:
            break;
        case TOGGLE:
            HWA_76200_DFET_TOGGLE();
            break;
        default:
            break;
    }
    
    switch(mode) {
        case DISCHRG:
            HWA_TRICKLE_CHRG_DISABLE();
            break;
        case DISCHRG_FAST:
            HWA_TRICKLE_CHRG_MODE_FAST();
            HWA_TRICKLE_CHRG_DISABLE();
            break;
        case DISCHRG_NORMAL:
            HWA_TRICKLE_CHRG_MODE_NORMAL();
            HWA_TRICKLE_CHRG_DISABLE();
            break;
        case ENCHRG:
            HWA_TRICKLE_CHRG_ENABLE();
            break;
        case ENCHRG_FAST:
            HWA_TRICKLE_CHRG_MODE_FAST();
            HWA_TRICKLE_CHRG_ENABLE();
            break;
        case ENCHRG_NORMAL:
            HWA_TRICKLE_CHRG_MODE_NORMAL();
            HWA_TRICKLE_CHRG_ENABLE();
            break;
        case HOLDING:
            break;
        default:
            break;
    }
    
    ret = HWA_TRICKLE_CHRG();
    pSvdChrgCmd->cmdTrickleChargerON = ret;
    d_chrgStatus = ret;
    pSvdCmd->cmdDisChrgEnable = ret;
    pSvdCmd->cmdChrgEnable = ret;
    EVENTLOG_CFET_DFET_TRICKLE();
}

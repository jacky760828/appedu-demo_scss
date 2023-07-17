/******************************************************************************
 *
 * FILE: daq_adc.c
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview: 
 *
 *****************************************************************************/

#include "daq_adc.h"

ADC1_CHANNEL adcPointer;

uint16_t DaqAdc_VBATT;
uint16_t DaqAdc_VBATT_BUS;
uint16_t DaqAdc_AN3;
/*******************************************************************************
* Function Name  : DaqAdc_init
* Description    : ADC initial
* Input          : None
* Return         : None
*******************************************************************************/
void DaqAdc_init(void)
{
    DaqAdc_VBATT = 0;
    DaqAdc_VBATT_BUS = 0;
    DaqAdc_AN3 = 0;
    adcPointer = channel_AN1_BAT;
    ADC1_ChannelSelect(adcPointer);
    ADC1_Enable();
}

// Executed every PWM cycle
void DaqAdc_mainFunction()
{
	switch(adcPointer) {
		case channel_AN1_BAT:	// AN1, guiTpsRegVal
            DaqAdc_VBATT = ((ADC1_ConversionResultGet(channel_AN1_BAT) * 5) + (DaqAdc_VBATT * 3) + 4) / 8;
			adcPointer = channel_AN2_VPAK;
			break;
	
		case channel_AN2_VPAK:	// AN2, guiChargeCurrentRegVal
            DaqAdc_VBATT_BUS = ((ADC1_ConversionResultGet(channel_AN2_VPAK) * 5) + (DaqAdc_VBATT_BUS * 3) + 4) / 8;
			adcPointer = channel_AN3;
			break;
        
        case channel_AN3:	// AN2, guiChargeCurrentRegVal
            DaqAdc_AN3 = ((ADC1_ConversionResultGet(channel_AN3) * 5) + (DaqAdc_AN3 * 3) + 4) / 8;
			adcPointer = channel_AN1_BAT;
			break;
            
        default :
            DaqAdc_VBATT = ((ADC1_ConversionResultGet(channel_AN1_BAT) * 5) + (DaqAdc_VBATT * 3) + 4) / 8; 
			adcPointer = channel_AN2_VPAK;
            break;
	}
    ADC1_ChannelSelect(adcPointer);
}

float DagAdc_get_VBUS(void)
{
    float ret;
    
//    ret = (float)(((uint32_t)DaqAdc_VBATT_BUS * GAIN_VBUS_INSIDE_GND + OFFSET_VBUS_INSIDE_GND)/100000.0f);
    ret = ((((float)DaqAdc_VBATT_BUS * GAIN_VBUS_INSIDE_GND) + OFFSET_VBUS_INSIDE_GND) / 100000.0f);
    return ret;
}

float DagAdc_get_VBATT(void)
{
    float ret;
    
//    ret = (float)(((uint32_t)DaqAdc_VBATT * GAIN_VBUS_INSIDE_GND + OFFSET_VBUS_INSIDE_GND)/100000.0f);
    ret = ((((float)DaqAdc_VBATT * GAIN_VBUS_INSIDE_GND) + OFFSET_VBUS_INSIDE_GND) / 100000.0f);
    return ret;
}

int DagAdc_get_AN3(void)
{
    int ret;
    
    ret = DaqAdc_AN3;
    return ret;
}

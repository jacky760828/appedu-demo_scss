/******************************************************************************
 *
 * FILE: daq_adc.h
 *
 * DESCRIPTION:
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 *****************************************************************************/
#ifndef DAQ_ADC_H
#define	DAQ_ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../mcc_generated_files/adc1.h"

#define GAIN_VBUS_INSIDE_GND        1607    //1951      //(65.6f * 32)
#define OFFSET_VBUS_INSIDE_GND      1066    //-13480    //(65.6f * 32)
    

void DaqAdc_init(void);
void DaqAdc_mainFunction(void);
float DagAdc_get_VBUS(void);
float DagAdc_get_VBATT(void);
int DagAdc_get_AN3(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DAQ_ADC_H */


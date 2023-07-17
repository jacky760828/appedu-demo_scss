/*
 * File:   SOC.c
 * Author: frank
 *
 * Created on 2021?5?9?, ?? 2:59
 */


#include "SOC.h"
#include "battery_manage_system.h"

uint8_t SOC_get_soc(void)
{
    uint32_t ret;
    
    ret = ((uint32_t)Calculation_RC * 100) / Calculation_FCC;    
    return (uint8_t)ret;
}

/* 
 * File:   ulclassbif.h
 * Author: frank.tsai
 *
 * Created on 2020?6?19?, ?? 4:00
 */

#ifndef ULCLASSBIF_H
#define	ULCLASSBIF_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    UL_REGISTER_B_CMD = 0x11,
    UL_INSTRUCTION_C_CMD = 0x12,
    UL_PC_B_CMD = 0x13,
    UL_ADDRESS_C_CMD = 0x14,
    UL_DATA_FETCH_C_CMD = 0x15,
    UL_INTERRUPT_B_CMD = 0x20,
    UL_CLOCK_B_CMD = 0x30,
    UL_NVRAM_B_CMD = 0x41,
    UL_RAM_B_CMD = 0x42,
    UL_ADDRESS_B_MEM_CMD = 0x43,
    UL_DATA_B_CMD = 0x51,
    UL_ADDRESS_B_1_CMD = 0x52,        
    UL_EXTERNAL_COMM_B_CMD = 0x60,
    UL_DATA_C_CMD = 0x61,
    UL_ADDRESS_B_2_CMD = 0x62,
    UL_WRONG_TIMING_B_CMD = 0x63,
    UL_WRONG_ORDER_B_CMD = 0x64,
    UL_IN_OUT_PERIPHERY_B_CMD = 0x70,        
    UL_IO_C_CMD = 0x71,
    // reserved        
    UL_ADC_DAC_B_CMD = 0x73,
    UL_ANALOG_MULTI_B_CMD = 0x74,
    //UL_MONITOR_COMPARE_C_CMD = 0x80,
    //UL_CUSTOMER_CHIP_B_CMD = 0x90
} ULClassBCmdUsage_t;    

void ulClassBFunctions(ULClassBCmdUsage_t code);

#ifdef	__cplusplus
}
#endif

#endif	/* ULCLASSBIF_H */


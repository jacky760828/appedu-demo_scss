/******************************************************************************
 *
 * FILE: int.c
 *
 * 
 * 
 * DESCRIPTION: This file declares the API functions and variables for int 
 *              (interrupt) service routine container component.
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 * Int is a component that contains all the isr functions of the application.
 *
 *****************************************************************************/
#include "int.h"


/*******************************************************************************
 * Function: INT_initInterruptService
 * Description: Re-define interrupt service routine
 * Input:	 	
 * Output: 
 * Others:
 *******************************************************************************/
void INT_initInterruptService(void)
{
    
    //timer1 for system time
    TMR1_SetInterruptHandler(Scheduler_mainFunction_foreground);
    
	//timer3 for 10ms
    TMR3_SetInterruptHandler(TimerFunction10ms);
    
    //adc
    ADC1_SetInterruptHandler(DaqAdc_mainFunction);
	
	//can bus receive
    CAN1_SetRxBufferInterruptHandler(CAN_interruptHandler_rxComplete);
}

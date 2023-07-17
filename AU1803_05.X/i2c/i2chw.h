/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef I2CHW_H
#define	I2CHW_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
/* defined for I2C1 */

#define I2C1_TRANSMIT_REG                       I2C1TRN			// Defines the transmit register used to send data.
#define I2C1_RECEIVE_REG                        I2C1RCV	// Defines the receive register used to receive data.

#define I2C1_ACKNOWLEDGE_STATUS_BIT             I2C1STATbits.ACKSTAT	// I2C ACK status bit.
#define I2C1_TRSNOWLEDGE_STATUS_BIT             I2C1STATbits.TRSTAT 	// I2C ACK status bit.
//
#define I2C1_STATE_INT_FLAG                     IFS1bits.MI2C1IF
#define I2C1_START_STATUS_BIT                   I2C1STATbits.S
#define I2C1_TBF_STATUS_BIT                     I2C1STATbits.TBF
#define I2C1_RBF_STATUS_BIT                     I2C1STATbits.RBF
#define I2C1_BCL_STATUS_BIT                     I2C1STATbits.BCL          // Master Bus Collision Detect bit
//
#define I2C1_START_CONDITION_ENABLE_BIT         I2C1CONbits.SEN		// I2C START control bit.
#define I2C1_REPEAT_START_CONDITION_ENABLE_BIT  I2C1CONbits.RSEN	// I2C Repeated START control bit.
#define I2C1_RECEIVE_ENABLE_BIT                 I2C1CONbits.RCEN	// I2C Receive enable control bit.
#define I2C1_STOP_CONDITION_ENABLE_BIT          I2C1CONbits.PEN		// I2C STOP control bit.
#define I2C1_ACKNOWLEDGE_ENABLE_BIT             I2C1CONbits.ACKEN 	// I2C ACK start control bit.
#define I2C1_ACKNOWLEDGE_DATA_BIT               I2C1CONbits.ACKDT	// I2C ACK data control bit.
#define I2C1_CE_BIT                             I2C1CONbits.I2CEN

#define I2C1_WRITE_COLLISION_STATUS_BIT         I2C1STATbits.IWCOL	// the write collision status bit.
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


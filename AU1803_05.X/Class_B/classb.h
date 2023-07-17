/*************************************************************************
 *  © 2014 Microchip Technology Inc.                                       
 *  
 *  Project Name:    Class B Library
 *  Version:         2.4 
 *  FileName:        classb.h
 *  Dependencies:    xc.h, stdint.h
 *  Processor:       PIC24, dsPIC
 *  Compiler:        XC16
 *  IDE:             MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This file contains declarations for the class B functions.
 *
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives hereto. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/

#ifndef _CLASSB_H_
#define _CLASSB_H_

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
  Enumeration:
    CLASSBRESULT

  Description:
    This enumeration is used by the class B test functions to return the results:
 
    CLASSB_TEST_PASS    - the test is finished successfully,
    CLASSB_TEST_FAIL    - the test is failed,

  ***************************************************************************/
typedef enum _CLASSBRESULT_
{
  CLASSB_TEST_PASS = 0,
  CLASSB_TEST_FAIL,
} CLASSBRESULT; 

/****************************************************************************
  Macro:
    CLASSBRESULT CLASSB_RAMMarchCTest(startAddress, length, bufferAddress, minus)

  Summary:
    Tests RAM using march C or march C minus algorithms.

  Description:
    Tests RAM using march C or march C minus algorithms.
 
  Precondition:
    None.

  Parameters:
    startAddress  - the first address of the tested memory,
    length        - the byte length of the tested memory,
    bufferAddress - the first address of the buffer to store the memory
                    content, if it is NULL then the test will clear the 
                    tested memory,
    minus         - if the parameter is TRUE the "minus" algorithm is used.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts are disabled during this test.
    The address and length must be even (aligned by 2 bytes).
  ***************************************************************************/
CLASSBRESULT CLASSB_RAMMarchCTestF(uint16_t* startAddress, uint16_t length, uint16_t* bufferAddress, uint16_t minus);
#define CLASSB_RAMMarchCTest(startAddress, length, bufferAddress, minus)  CLASSB_RAMMarchCTestF((uint16_t*)(startAddress), (uint16_t)(length), (uint16_t*)(bufferAddress), (uint16_t)minus)

/****************************************************************************
  Macro:
    CLASSBRESULT CLASSB_EntireRAMMarchCTest()

  Summary:
    Tests entire RAM using march C algorithm.

  Description:
    Tests entire RAM using march C algorithm.
 
  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts are disabled during this test. After this RAM destructive test
    MCU is reset, the macro executed second time to return status of the test.
    The macro will reset the MCU (will be executed twice), so, it should be
    called at the top of main() right after oscillator initialization.
  ***************************************************************************/
#define CLASSB_EntireRAMMarchCTest()  CLASSB_RAMMarchCTestF((uint16_t*)__DATA_BASE,(uint16_t)__DATA_LENGTH, (uint16_t*)0, 0x8000)

/****************************************************************************
  Macro:
    CLASSBRESULT CLASSB_RAMMarchBTest(startAddress, length, bufferAddress)

  Summary:
    Tests RAM using march B algorithm.

  Description:
    Tests RAM using march B algorithm.
 
  Precondition:
    None.

  Parameters:
    startAddress  - the first address of the tested memory,
    length        - the byte length of the tested memory,
    bufferAddress - the first address of the buffer to store the memory
                    content, if it is NULL then the test will clear the 
                    tested memory.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts are disabled during this test.
    The address and length must be even (aligned by 2 bytes).
  ***************************************************************************/
CLASSBRESULT CLASSB_RAMMarchBTestF(uint16_t* startAddress, uint16_t length, uint16_t* bufferAddress, uint16_t mustbezero);
#define CLASSB_RAMMarchBTest(startAddress, length, bufferAddress)  CLASSB_RAMMarchBTestF((uint16_t*)(startAddress), (uint16_t)(length), (uint16_t*)(bufferAddress), (uint16_t)0)


/****************************************************************************
  Macro:
    CLASSBRESULT CLASSB_EntireRAMMarchBTest()

  Summary:
    Tests entire RAM using march B algorithm.

  Description:
    Tests entire RAM using march B algorithm.
 
  Precondition:
    None.

  Parameters:
    None.
 
  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    Interrupts are disabled during this test. After this RAM destructive test
    MCU is reset, the macro executed second time to return status of the test.
    The macro will reset the MCU (will be executed twice), so, it should be
    called at the top of main() right after oscillator initialization.
  ***************************************************************************/
#define CLASSB_EntireRAMMarchBTest()  CLASSB_RAMMarchBTestF((uint16_t*)__DATA_BASE,(uint16_t)__DATA_LENGTH, (uint16_t*)0, 0x8000)

/****************************************************************************
  Macro:
    CLASSBRESULT CLASSB_RAMCheckerboardTest(startAddress, length)

  Summary:
    Tests RAM region using checker board pattern.

  Description:
    Tests RAM region using checker board pattern.
 
  Precondition:
    None.

  Parameters:
    startAddress - the first address of the tested memory,
    length       - the byte length of the tested memory.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    The address must be even (aligned by 2 bytes).
    The length must be aligned by 4 bytes.
  ***************************************************************************/
CLASSBRESULT CLASSB_RAMCheckerboardTestF(uint16_t* startAddress, uint16_t length);
#define CLASSB_RAMCheckerboardTest(startAddress, length) CLASSB_RAMCheckerboardTestF((uint16_t*)(startAddress), (uint16_t)(length))

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_CPURegistersTest()

  Summary:
    Tests CPU registers.

  Description:
    Tests CPU registers.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    None.
  ***************************************************************************/
CLASSBRESULT CLASSB_CPURegistersTest();

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_CPUPCTest()

  Summary:
    Tests CPU program counter.

  Description:
    Tests CPU program counter. 
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns result for the previous test. Zero if successful. Non zero means - failed.

  Remarks:
    None.
 
  ***************************************************************************/
CLASSBRESULT CLASSB_CPUPCTest();


/****************************************************************************
  Macro:
    uint16_t CLASSB_CRCFlashTest(startAddress, length, crcSeed)

  Summary:
    The function calculates the CRC check sum for the flash memory region.

  Description:
    The function calculates the CRC check sum for the flash memory region.
 
  Precondition:
    None.

  Parameters:
    startAddress - the first address of the tested flash memory in bytes,
    length       - the length address of the tested flash memory in PC units,
    crcSeed      - initial value of the CRC check sum.

  Returns:
    The function returns the standard 16-bit CRC.

  Remarks:
    The start address and length must be even (aligned by 2).
    "length" must be specified in PC units (PC units are used in map files).
  ***************************************************************************/
uint16_t  CLASSB_CRCFlashTestF(__prog__ uint16_t* startAddress, uint32_t length, uint16_t crcSeed);
#define   CLASSB_CRCFlashTest(startAddress, length, crcSeed) CLASSB_CRCFlashTestF((__prog__ uint16_t*)(startAddress), (uint32_t)(length), (uint16_t)(crcSeed))

/****************************************************************************
  Macro:
    uint16_t  CLASSB_CRCEEPROMTest(startAddress, length, crcSeed)

  Summary:
    The function calculates the CRC check sum for the EEPROM memory region.

  Description:
    The function calculates the CRC check sum for the EEPROM memory region.
 
  Precondition:
    None.

  Parameters:
    startAddress - the first address of the tested EEPROM memory in bytes,
    length       - the length address of the tested EEPROM memory,
    crcSeed      - initial value of the CRC check sum.

  Returns:
    The function returns the standard 16-bit CRC.

  Remarks:
    The start address and length must be even (aligned by 2).
  ***************************************************************************/
uint16_t  CLASSB_CRCEEPROMTestF(__prog__ uint16_t* startAddress, uint32_t length, uint16_t crcSeed);
#define   CLASSB_CRCEEPROMTest(startAddress, length, crcSeed) CLASSB_CRCEEPROMTestF((__prog__ uint16_t*)(startAddress), (uint32_t)(length), (uint16_t)(crcSeed))

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_ClockTest(uint32_t clockFrequency,
                                  uint32_t referenceFrequency,
                                  uint16_t tolerance,
								  referenceCounterSFRAddress)
  Summary:
    Tests the CPU clock source.

  Description:
    Tests the CPU clock source.The reference
    clock should be connected to a timer. The timer must be
    initialized by the application code to count the reference
    clock pulses.  
 
  Precondition:
    None.

  Parameters:
    clockFrequency     - frequency of the clock source,
    referenceFrequency - frequency of the reference clock
                         (such as power line or secondary oscillator),
    tolerance          - maximum valid frequency tolerance, can be from
                         1(0.1%) to 100(10%).
    referenceCounterSFRAddress - address of counter connected to
                                 the reference clock source.

  Returns:
    Returns zero if successful. Non zero means - failed. See CLASSBRESULT
    enumeration for details.

  Remarks:
    Interrupts are disabled during this test. The test takes approximately 
    4 reference clock periods.
  ***************************************************************************/
CLASSBRESULT CLASSB_ClockTest(uint32_t clockFrequency, uint32_t referenceFrequency, uint16_t tolerance, volatile uint16_t* referenceCounterSFRAddress);

#endif

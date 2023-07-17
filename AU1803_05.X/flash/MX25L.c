#include "MX25L.h"
#include "xc.h"

#include <stdlib.h>
#include <stdio.h>

UINT32 ulEraseStart;

//MX25L  mx25l6473fm2i-08g  SERIAL NOR 8SOP 64Mbit  8MBytes
//=========================
//	MT25Q init
//=========================
void MX25lInit(void)
{
	
}

//==========================
//	Flaslh_Enable
//==========================
void MX25L_Flash_Deep_PowerDown(void)
{
	SS2OUT_SetLow();
	SPI2_Exchange8bit(WREN);
	SS2OUT_SetHigh();
}

//==========================
//	Flaslh_Enable
//==========================
void MX25L_Flash_Write_Enable(void)
{
	SS2OUT_SetLow();
	SPI2_Exchange8bit(WREN);
	SS2OUT_SetHigh();
}

//==========================
//	Flash_Disable
//==========================
void MX25L_Flash_Write_Disable(void)
{
	SS2OUT_SetLow();
	SPI2_Exchange8bit(WRDI);
	SS2OUT_SetHigh();
}

void MX25L_CE(void)
{
    SS2OUT_SetLow();
	SPI2_Exchange8bit(EChip);
	SS2OUT_SetHigh();
}

//==========================
//	Flash_Status
//==========================
UINT8 MX25L_FLASH_Status(void)
{
	UINT8 ucFlag;
	SS2OUT_SetLow();
	SPI2_Exchange8bit(RDSR);
	ucFlag = SPI2_Exchange8bit(FlashTurnBack);
	SS2OUT_SetHigh();
	return ucFlag;
}

//==========================
//	Flash_Erase
//==========================
void MX25L_FLASH_Erase(UINT32 SectorAddress,UINT8 Command)
{
    SS2OUT_SetLow();
	SPI2_Exchange8bit(Command);
    SPI2_Exchange8bit(SectorAddress >> 16);
    SPI2_Exchange8bit(SectorAddress >> 8);
	SPI2_Exchange8bit(SectorAddress);
    SS2OUT_SetHigh();
}
//===========================
//	Flash_Read
//===========================
void MX25L_Flash_Read(UINT8 *pBuffer,UINT32 ReadAddress,UINT16 ReadByteNum)
{
	SS2OUT_SetLow();
	SPI2_Exchange8bit(RD);
    SPI2_Exchange8bit(ReadAddress >> 16);
	SPI2_Exchange8bit(ReadAddress >> 8);
    SPI2_Exchange8bit(ReadAddress);
    while(ReadByteNum--) {
		*(pBuffer++) = SPI2_Exchange8bit(FlashTurnBack);
    }
    SS2OUT_SetHigh();
}
//===========================
//  Read Security Register
//===========================
void MX25L_Read_Security(void)
{
    SS2OUT_SetLow();
    SPI2_Exchange8bit(RDSCUR);
    SPI2_Exchange8bit(FlashTurnBack);
    SPI2_Exchange8bit(FlashTurnBack);
    SS2OUT_SetHigh();
}

//==========================
//      Flash_Write
//==========================
void  MX25L_Flash_Write(UINT8 *pBuffer, UINT32 WriteAddress,UINT16 WriteByteNum)
{
    SS2OUT_SetLow();
    SPI2_Exchange8bit(PPage);
    SPI2_Exchange8bit(WriteAddress >> 16);
	SPI2_Exchange8bit(WriteAddress >> 8);
    SPI2_Exchange8bit(WriteAddress);
    if(WriteByteNum > 256) {
        WriteByteNum = 256;
    }
    while(WriteByteNum--) {
        printf("0x%02X ",*pBuffer);
        SPI2_Exchange8bit(*(pBuffer++));
    }
	SS2OUT_SetHigh();
}

//===========================
// Testing flash memory
//===========================
UINT8 Flash_test(void)
{
    struct MemoryType temp;
    UINT8 tempBuffer[8]={0,1,2,3,4,5,6,7};
    UINT8 ReadBuffer[8]={0};
    UINT8 i,checksum=0;
    //temp.uADRS.Address=0x0a;
    temp.uADRS.Address = 0x030000 + 0x1000; //tsai 6/2 change to safe location
    
    MX25L_Flash_Write_Enable();
	ulEraseStart=SysTime_getMsec();
  	do{
        if(SysTime_msecIntervalElapsed(ulEraseStart, EraseTimout))
            return FALSE;
    } while((MX25L_FLASH_Status() & 0x02) != 0x02);
    MX25L_Flash_Write(tempBuffer, temp.uADRS.Address, 8);
    ulEraseStart = SysTime_getMsec();
  	do{
        if(SysTime_msecIntervalElapsed(ulEraseStart, EraseTimout))
            return FALSE;
    } while(MX25L_FLASH_Status() & 0x01);
    MX25L_Read_Security();
    MX25L_Flash_Read(ReadBuffer, temp.uADRS.Address, 8);
    MX25L_Flash_Write_Enable();
    ulEraseStart = SysTime_getMsec();
  	do{
        if(SysTime_msecIntervalElapsed(ulEraseStart, EraseTimout))
            return FALSE;
    } while((MX25L_FLASH_Status() & 0x02) != 0x02);
    MX25L_FLASH_Erase(temp.uADRS.Address, ESector);
    ulEraseStart = SysTime_getMsec();
  	do{
        if(SysTime_msecIntervalElapsed(ulEraseStart, EraseTimout))
            return FALSE;
    } while(MX25L_FLASH_Status() & 0x01);

    MX25L_Flash_Write_Disable();
    
    for(i=0;i<8;i++) {
        if(tempBuffer[i] == ReadBuffer[i])
            checksum++;
    }
    
    if(checksum==8) {
        return TRUE;
    } else {
        return FALSE;
    }
}




/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/spi_flash.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   This file provides a set of functions needed to manage the SPI M25Pxxx
  *          FLASH memory.
  *
  *          ===================================================================
  *          Notes:
  *           - There is no SPI FLASH memory available in STM322xG-EVAL board,
  *             to use this driver you have to build your own hardware.
  *          ===================================================================
  *
  *          It implements a high level communication layer for read and write
  *          from/to this memory. The needed STM32 hardware resources (SPI and
  *          GPIO) are defined in spi_flash.h file, and the initialization is
  *          performed in sFLASH_LowLevel_Init() function.
  *
  *          You can easily tailor this driver to any development board, by just
  *          adapting the defines for hardware resources and sFLASH_LowLevel_Init()
  *          function.
  *
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+---------------+-------------+
  *          |  STM32 SPI Pins             |     sFLASH    |    Pin      |
  *          +-----------------------------+---------------+-------------+
  *          | sFLASH_CS_PIN               | ChipSelect(/S)|    1        |
  *          | sFLASH_SPI_MISO_PIN / MISO  |   DataOut(Q)  |    2        |
  *          |                             |   VCC         |    3 (3.3 V)|
  *          |                             |   GND         |    4 (0 V)  |
  *          | sFLASH_SPI_MOSI_PIN / MOSI  |   DataIn(D)   |    5        |
  *          | sFLASH_SPI_SCK_PIN / SCK    |   Clock(C)    |    6        |
  *          |                             |    VCC        |    7 (3.3 V)|
  *          |                             |    VCC        |    8 (3.3 V)|
  *          +-----------------------------+---------------+-------------+
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_FLASH
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void sFLASH_EraseSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();
  
  while((MX25L_FLASH_Status() & 0x02) != 02);
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();
  /*!< Send Sector Erase instruction */
  SPI2_Exchange8bit(ESector);
  /*!< Send SectorAddr high nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPI2_Exchange8bit(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();

  while((MX25L_FLASH_Status() & 0x01) != 00);
  /*!< Wait the end of Flash writing */
//  sFLASH_WaitForWriteEnd();
}

void sFLASH_EraseBlock32(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();
  
  while((MX25L_FLASH_Status() & 0x02) != 02);
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();
  /*!< Send Sector Erase instruction */
  SPI2_Exchange8bit(EBlock32k);
  /*!< Send SectorAddr high nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPI2_Exchange8bit(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();

  while((MX25L_FLASH_Status() & 0x01) != 00);
  /*!< Wait the end of Flash writing */
//  sFLASH_WaitForWriteEnd();
}

void sFLASH_EraseBlock64(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();
  
  while((MX25L_FLASH_Status() & 0x02) != 02);
  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();
  /*!< Send Sector Erase instruction */
  SPI2_Exchange8bit(EBlock64K);
  /*!< Send SectorAddr high nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  SPI2_Exchange8bit((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  SPI2_Exchange8bit(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();

  while((MX25L_FLASH_Status() & 0x01) != 00);
  /*!< Wait the end of Flash writing */
//  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseChip(void)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();
  /*!< Send Bulk Erase instruction  */
  SPI2_Exchange8bit(sFLASH_CMD_BE);
  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();

  /*!< Wait the end of Flash writing */
//  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable();

  while((MX25L_FLASH_Status() & 0x02) != 02);
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();
  /*!< Send "Write to Memory " instruction */
  SPI2_Exchange8bit(sFLASH_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  SPI2_Exchange8bit((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  SPI2_Exchange8bit((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  SPI2_Exchange8bit(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
//  printf("WritePage:");
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    SPI2_Exchange8bit(*pBuffer);
//    printf("%02X ",*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }
//  printf("\r\n");

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();
  while((MX25L_FLASH_Status() & 0x01) != 00);
  /*!< Wait the end of Flash writing */
//  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
  count = sFLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        sFLASH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

      sFLASH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "Read from Memory " instruction */
  SPI2_Exchange8bit(sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  SPI2_Exchange8bit((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  SPI2_Exchange8bit((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  SPI2_Exchange8bit(ReadAddr & 0xFF);

//  printf("ReadBuffer(%ld):",ReadAddr);
  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = SPI2_Exchange8bit(sFLASH_DUMMY_BYTE);
//    printf("%02X ",*pBuffer);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }
//  printf("\r\n");

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t sFLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "RDID " instruction */
  SPI2_Exchange8bit(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = SPI2_Exchange8bit(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = SPI2_Exchange8bit(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = SPI2_Exchange8bit(sFLASH_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void sFLASH_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "Read from Memory " instruction */
  SPI2_Exchange8bit(sFLASH_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  SPI2_Exchange8bit((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  SPI2_Exchange8bit((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  SPI2_Exchange8bit(ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t sFLASH_ReadByte(void)
{
  return (SPI2_Exchange8bit(sFLASH_DUMMY_BYTE));
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "Write Enable" instruction */
  SPI2_Exchange8bit(sFLASH_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();
}

/**
  * @brief  Disable the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteDisable(void)
{
  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "Write Disable" instruction */
  SPI2_Exchange8bit(sFLASH_CMD_WRDI);

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  SS2OUT_SetLow();

  /*!< Send "Read Status Register" instruction */
  SPI2_Exchange8bit(sFLASH_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = SPI2_Exchange8bit(sFLASH_DUMMY_BYTE);

  }
  while ((flashstatus & sFLASH_WIP_FLAG) == sFLASH_WIP_FLAG); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  SS2OUT_SetHigh();
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
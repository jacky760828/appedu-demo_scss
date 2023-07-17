#ifndef MX25L_H
#define MX25L_H


#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/spi2.h"
#include "../main_app/platform_types.h"
#include "../sys_time/sys_time.h"

#define EraseTimout 200

#define SectorSize 4096L
#define PageSize   256
struct MemoryType{
	 UINT8		status;
	 union{
		 UINT32	ECount;
		 struct sCNT{
			 UINT8 byte0;
			 UINT8 byte1;
			 UINT8 byte2;
			 UINT8 byte3;
		 }cntbytes;
	 }uCNT;
	 union{
		 UINT32	Address;
		 struct sADRS{
		 			 UINT8 byte0;
		 			 UINT8 byte1;
		 			 UINT8 byte2;
                     UINT8 byte3;
		 }adrsbytes;
	 }uADRS;

 };

#define FlashTurnBack 	(0x00)
#define WREN			(0x06)
#define WRDI			(0x04)
#define RDSR			(0x05)
#define RDSRFG			(0X70)
#define RDSCUR          (0X2B)
#define EBlock32k		(0x52)
#define ESector			(0x20)
#define EChip			(0xC7)
#define EBlock64K       (0xD8)
#define E8k				(0x40)
#define RD				(0x03)
#define RDFST			(0x0B)
#define RDID			(0X9F)
#define PPage			(0x02)

#define DP              (0xB9) // tsai

void MX25L_Flash_Write_Enable(void);
void MX25L_Flash_Write_Disable(void);
UINT8 MX25L_FLASH_Status(void);
void MX25L_FLASH_Erase(UINT32 SectorAddress,UINT8 Command);
void MX25L_Flash_Read(UINT8 *pBuffer,UINT32 ReadAddress,UINT16 ReadByteNum);
void  MX25L_Flash_Write(UINT8 *pBuffer, UINT32 WriteAddress,UINT16 WriteByteNum);
UINT8 Flash_test(void);
void MX25L_CE(void);



/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   This file contains all the functions prototypes for the spi_flash
  *          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* M25P SPI Flash supported commands */
#define sFLASH_CMD_WRITE          0x02  /* Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /* Write enable instruction */
#define sFLASH_CMD_WRDI           0x04  /* Write disable instruction */
#define sFLASH_CMD_READ           0x03  /* Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /* Read identification */
#define sFLASH_CMD_SE             0xD8  /* Sector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /* Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0x00
#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P64_ID          0x202017

/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
void sFLASH_DeInit(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBlock32(uint32_t SectorAddr);
void sFLASH_EraseBlock64(uint32_t SectorAddr);
void sFLASH_EraseChip(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
uint8_t sFLASH_ReadByte(void);
void sFLASH_WriteEnable(void);
void sFLASH_WriteDisable(void);
void sFLASH_WaitForWriteEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif

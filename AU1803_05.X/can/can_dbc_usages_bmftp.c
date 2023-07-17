/******************************************************************************
 *
 * FILE: can_dbc_usages_bmftp.c
 *
 * Copyright(c) 2021 C-Technology United.
 * All Rights Reserved
 * Author : Frank Tsai
 * Overview: 
 *
 *****************************************************************************/

#include "can.h"
#include "../flash/MX25L.h"
#include "../security/aes.h"
#include "../security/sha256.h"
#include "../mcc_generated_files/watchdog.h"

#define USE_AESIV_IN_FIRST_128BYTES // 11/16 tsai APC new change
#define FW_VERSION_ADDR_OFFSET  0x4000
#define FW_IMAGE_CHECK_DELAY    1000 //ms

UINT32 d_CheckImageReqTime;
UINT32 FileSizeMax;
UINT32 FileSizeCurrent;
UINT16 BlockSizeMax;
UINT16 BlockCounter;
UINT8  UpgradeAdmit;
UINT8  FragmentCnt;
UINT16 BlockCurrent;
UINT8 twoByteErase;
UINT8 FlashVerion[3];
UINT8 WriteFlash[8]={0xFC,0x42,0x04,0x00,0x00,0x00,0x00,0x00};

FileReq_t FileFrame;
DBCFLAGS_t DBCFLAGS;

static uint8_t flashBuffer[AES_BLOCKLEN];//16 bytes
static struct MemoryType Point2Flash;
extern uint8_t swResetByte __attribute__ ((persistent)) __attribute__ ((address(0x2000)));


void ReadImageVersionInFlash(void)
{   
    MX25L_Flash_Read(WriteFlash,FWAddrBase,2);
    if (WriteFlash[0]==0xFC && WriteFlash[1]==0x42) {//image ok
        MX25L_Flash_Read(WriteFlash,FWAddrBase+FW_VERSION_ADDR_OFFSET+4,1);//major information
        FlashVerion[0]=WriteFlash[0]; //major
        MX25L_Flash_Read(WriteFlash,FWAddrBase+FW_VERSION_ADDR_OFFSET+8,1);//minor information
        FlashVerion[1]=WriteFlash[0]; //minor
        MX25L_Flash_Read(WriteFlash,FWAddrBase+FW_VERSION_ADDR_OFFSET+12,1);//build information
        FlashVerion[2]=WriteFlash[0]; //build
        return;
    }
    //fail or not c-tech
    FlashVerion[0]=0xFF;
    FlashVerion[1]=0x00;
    FlashVerion[2]=0x00;
}

void BMFTPinit(void)
{
    FileSizeMax=0;
    FileSizeCurrent=0;
    BlockSizeMax=0;
    UpgradeAdmit=0;
    BlockCounter=0;

    FragmentCnt=0;
    BlockCurrent=0;

    DBCFLAGS.ImageOK=0; 
    twoByteErase=0xFF;
#ifdef USE_AESIV_IN_FIRST_128BYTES 
    ReadImageVersionInFlash();
#endif    
    Point2Flash.uADRS.Address=FWAddrBase;
}

void BMFTP_Erase_Flash(void) // erase 4K one time
{
    MX25L_Flash_Write_Enable();
    MX25L_FLASH_Erase(Point2Flash.uADRS.Address,ESector);
    while(MX25L_FLASH_Status()&0x01);
}

static void isNeedUpdate(UINT8 *data, uint8_t ifSameAsFlash )
{
    if (data[1]==0x03) { //Ctech
        if (DBCFLAGS.forceUpdate) {//don't check version
            return;
        }
        if ((fwVersionMajor[0]==data[2]) && (fwVersionMinor[0]==data[3]) && (fwVersionBuild[0]==data[4])) {
            UpgradeAdmit=0;//stop
        } else if ((FlashVerion[0]==data[2]) && (FlashVerion[1]==data[3]) && (FlashVerion[2]==data[4])) {
            UpgradeAdmit=ifSameAsFlash; //stop 
        }
    } else {
        UpgradeAdmit=0; //stop
    }
}

static void set2bytesType(UINT8 *data )
{
    twoByteErase=0x00;//0x55;
    Point2Flash.uADRS.Address = EncryptFWAddrBase;
    if ((data[0]==0xFF) && (data[1]==0xFF)) {
        if (data[4]==data[5]) {
            if (data[5]==data[6]) {
                if (data[5]==0) {
                    twoByteErase=0x42;
                    Point2Flash.uADRS.Address=FWAddrBase;
                }
            }
        }
    } else {
        if ((data[0]==0xFC) && (data[1]==0x42)) {
            if (data[4]==data[5]) {
                if (data[5]==data[6]) {
                    if (data[5]==0) {
                        twoByteErase=0xAA;
                    }
                }
            }
        }
    }
}

void Flash_WriteMX(UINT8 *pBuffer, UINT32 WriteAddress,UINT16 WriteByteNum)
{
    UINT16 i;
    for (i=0;i< WriteByteNum;i++)
    {
        MX25L_Flash_Write_Enable();
        MX25L_Flash_Write(pBuffer+i, WriteAddress+i, 1);
    }
}

void BMFTPReqCmd(uCAN_MSG *tempBuf)
{
    UINT8 index;
    UINT8 FTPStep;
    
    for (index=0;index < 8;index++) {
        FileFrame.eightbytes[index] = tempBuf->array[7 + index];
    }
    
    FTPStep = FileFrame.eightbytes[0]&0xF;
    
    switch(FTPStep) {
        case FileStartCMD:
            if (FileFrame.fileStart.StartDestAdrs == ( d_deviceAddr & 0xf) || FileFrame.fileStart.StartDestAdrs == 0xFF) {                
                BMFTPinit();
                FileSizeMax=((uint32_t)(FileFrame.fileStart.highside)<<12) + ((uint16_t)(FileFrame.fileStart.StartSize)<<4)+(FileFrame.fileStart.StartCMD>>4);
                BlockSizeMax=FileFrame.fileStart.BlkSize;
                UpgradeAdmit=1;
                DBCFLAGS.GetFileOK=0;
                // tsai 9/15
                Point2Flash.uADRS.Address = EncryptFWAddrBase;
                BMFTP_Erase_Flash();
                Point2Flash.uADRS.Address = FWAddrBase;
            }
            break;
            
        case BlockStartCMD:
            if (UpgradeAdmit==1 && (BlockCounter <= (FileSizeMax/BlockSizeMax))) {
                if (BlockCounter==FileFrame.blockStart.Counter) {
                    BlockCounter++;
                    FragmentCnt = 0;//tsai 
                    BlockCurrent =0;
                    if ((Point2Flash.uADRS.Address % 0x1000) ==0) { //4K boundary start , 0x1000 must can be divided by BlockSizeMax (now is 256) 
                        BMFTP_Erase_Flash();
                    }
                } else {
                    MX25L_Flash_Write_Disable();
                    UpgradeAdmit=0;
                }
            }
            break;
            
        case BlockFragment:
           if (UpgradeAdmit == 1) {
               if(FragmentCnt == FileFrame.blkfragmnt.FragmntCNT) {
                    //MX25L_Flash_Write_Enable();
                    if (twoByteErase == 0xFF) { //tsai 9/15
                        DBCFLAGS.ImageOK=0;
                        set2bytesType(FileFrame.eightbytes+1);
#ifdef USE_AESIV_IN_FIRST_128BYTES                        
                        if (twoByteErase == 0x00) {
                            isNeedUpdate(FileFrame.eightbytes + 1,0);//input image version
                        }
#endif
                    }

                    UINT8 i;
                    if ((BlockCurrent + 7 > BlockSizeMax) || (FileSizeCurrent==FileSizeMax)) {
                        i=BlockSizeMax-BlockCurrent;
                        Flash_WriteMX(FileFrame.eightbytes+1,Point2Flash.uADRS.Address,i);
                        Point2Flash.uADRS.Address += i;                        
                        BlockCurrent = 0;
                        FragmentCnt = 0;
                    } else {
                        Flash_WriteMX(FileFrame.eightbytes+1,Point2Flash.uADRS.Address,7);
                        Point2Flash.uADRS.Address +=7;                       
                        FragmentCnt+=1;
                        BlockCurrent+=7;
                    }
                    
                    FileSizeCurrent += (tempBuf->frame.dlc-1);
                    if (FragmentCnt > 0xF) {
                        FragmentCnt=0;
                    }
                   
                    if(FileSizeCurrent==FileSizeMax) {
                        d_CheckImageReqTime = SysTime_getMsec();
                        DBCFLAGS.GetFileOK=1;
                        MX25L_Flash_Write_Disable();
                    }
               } else {
                   MX25L_Flash_Write_Disable();
                   UpgradeAdmit=0;
               }
           }
           break;
            
        default:
            break;
    }
}

UINT16 updateCRC16(UINT16 input, UINT8 *data, UINT8 len)
{
    UINT8 i,j;
    UINT16 crc = input;
    for ( i = 0; i < len; i++) {
        crc ^= (UINT16)(data[i] << 8);
        for (j = 0; j < 8; j++) {
            if ((crc & 0x8000) > 0) {
                crc = (UINT16)((crc << 1) ^ 0x1021);
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

UINT8 Verify_FWimage(void)
{
    uint32_t index;
    uint16_t crc16 = 0xFFFF;
    uint16_t len;
    FileSizeMax -= 2;//last two bytes is crc16
    Point2Flash.uADRS.Address=FWAddrBase;
    for(index=0;index<FileSizeMax;index +=8) {
        if((index+8) > FileSizeMax) {
            len=(FileSizeMax-index);
        } else {
            len = 8;
        }
        MX25L_Flash_Read(WriteFlash,Point2Flash.uADRS.Address,len);
        if(Point2Flash.uADRS.Address==FWAddrBase) {
            if(twoByteErase == 0x42) {
                WriteFlash[0] = 0xFC;
                WriteFlash[1] = 0x42;
            }
        }
        Point2Flash.uADRS.Address += len;
        crc16 = updateCRC16(crc16,WriteFlash,len);
        WATCHDOG_TimerClear();
    }
    MX25L_Flash_Read(WriteFlash,Point2Flash.uADRS.Address,2);//last two bytes
    len = WriteFlash[1];
    len <<= 8;
    len += WriteFlash[0];
    if(len == crc16) {
        if(twoByteErase==0x42) {
            WriteFlash[0]=0xFC;
            WriteFlash[1]=0x42;
            
            MX25L_Flash_Write_Enable(); //  need erase ?
            MX25L_Flash_Write(WriteFlash, FWAddrBase, 2);
        }
        len = 1;
    } else {
        len=0;
    }
    MX25L_Flash_Write_Disable();
    
    return len;
}

uint8_t SHA256_Vertify(void)
{
    SHA256_CTX shactx;
    uint8_t sha256buf[SHA256_BLOCK_SIZE];
    uint32_t idx;
    uint8_t onebyte,index;
    
    // check whole fw
    sha256_init(&shactx);
    for (idx = 0; idx < FileSizeMax; idx += AES_BLOCKLEN) {
        if ((FileSizeMax-idx) >=AES_BLOCKLEN) {
            index = AES_BLOCKLEN;
        } else {
            index = FileSizeMax-idx;
        }
        MX25L_Flash_Read(flashBuffer,  FWAddrBase+idx, index);
        WATCHDOG_TimerClear();
        sha256_update(&shactx, flashBuffer, index);//index is size
    }
    sha256_final(&shactx, sha256buf);
    idx = FWAddrBase + FileSizeMax + 64;
    for (index = 0; index < SHA256_BLOCK_SIZE; index++) {
        MX25L_Flash_Read(&onebyte,idx+index,1);
        if (onebyte != sha256buf[index]) {
            break;
        }
    }
    if (index == SHA256_BLOCK_SIZE) { //ok
        return 1;
    } else {
        return 0;
    }
}

uint8_t DecryptAndVerifyData(void)
{
    struct AES_ctx aesctx;
    uint32_t index;
    uint8_t offset;
    
    // erase flash of FW temp area
#ifdef USE_AESIV_IN_FIRST_128BYTES    
    //FileSizeMax -= AES_BLOCKLEN;  // skip the first 16 IV bytes
    FileSizeMax -= 128;  // skip the first 128  bytes
#endif    
    FileSizeMax -= AES_BLOCKLEN; //skip the last 16 paded bytes
    offset =  (FileSizeMax % 0x1000) ? (FileSizeMax/0x1000 + 1) : FileSizeMax/0x1000 ;
    Point2Flash.uADRS.Address = FWAddrBase;
    for (index=0; index < offset; index++) {
        BMFTP_Erase_Flash();
        Point2Flash.uADRS.Address +=0x1000; //4k
        WATCHDOG_TimerClear();
    }   
    // decrypt flash data to FW temp area
    Point2Flash.uADRS.Address = EncryptFWAddrBase;
#ifdef USE_AESIV_IN_FIRST_128BYTES    
    MX25L_Flash_Read(aesiv,Point2Flash.uADRS.Address+64,AES_BLOCKLEN);//16 bytes are IV
    Point2Flash.uADRS.Address += 128;//AES_BLOCKLEN;
#endif    
    AES_init_ctx_iv(&aesctx, aeskey, aesiv);
    //write to FWAddrBase
    for (index=0;index < FileSizeMax;index += AES_BLOCKLEN) { //tool make sure FileSizeMax is multiple of 16 (AES_BLOCKLEN)
        MX25L_Flash_Read(flashBuffer, Point2Flash.uADRS.Address+index, AES_BLOCKLEN);
        WATCHDOG_TimerClear();
        AES_CBC_decrypt_buffer(&aesctx, flashBuffer, AES_BLOCKLEN);
        if (index!=128) {
            if (index < 128) {
                Flash_WriteMX(flashBuffer, FWAddrBase + index + FileSizeMax -128, AES_BLOCKLEN);
            } else {
                Flash_WriteMX(flashBuffer, FWAddrBase + index -128, AES_BLOCKLEN);
            }
        } else {
            WriteFlash[0] = flashBuffer[0];
            WriteFlash[1] = flashBuffer[1];
            Flash_WriteMX(flashBuffer+2, FWAddrBase + index -128 + 2, AES_BLOCKLEN-2);
        }
    }
    //last write first two bytes
    Flash_WriteMX(WriteFlash, FWAddrBase , 2);
    
    FileSizeMax -= 128;
    // below is SHA256 check if need 
    if (SHA256_Vertify()==0) {//SHA256 check fail
        twoByteErase=0x00;
        Point2Flash.uADRS.Address = FWAddrBase;
        BMFTP_Erase_Flash();
    } else { //ok
        Point2Flash.uADRS.Address = FWAddrBase;
        MX25L_Flash_Read(flashBuffer,Point2Flash.uADRS.Address,8);
        set2bytesType(flashBuffer);
    }

    return twoByteErase;
}

UINT8 Verify_FlashData(void)
{
    if (twoByteErase==0x00) { //encrypted data
        DecryptAndVerifyData();//decrypt and copy to final FW location
        if (twoByteErase==0x00) { //image not correct
            MX25L_Flash_Write_Disable();
            return 0;
        }
        //if (twoByteErase !=0x42) //no two byte erase ==0xAA
        if (twoByteErase == 0xAA) {
            return 1;
        } else {
            return 0;
        }
    }
    
    return Verify_FWimage();
}

void ISImageOK(void)
{
    MX25L_Flash_Read(WriteFlash,FWAddrBase,2);
    if (WriteFlash[0]==0xFC && WriteFlash[0]==0x42) {
        DBCFLAGS.ImageOK=1;
    } else {
        DBCFLAGS.ImageOK=0;
    }
}

/******************************************************************************
 * Function Name : GetUpgradeFlag
 * Description   : Finishing to upgrade FW, and erase flash
 
 ******************************************************************************/
void GetUpgradeFlag(void)
{
    if (DBCFLAGS.GetFileOK) {
        if (SysTime_msecIntervalElapsed(d_CheckImageReqTime, FW_IMAGE_CHECK_DELAY)) {
            if (Verify_FlashData()==1) {
                DBCFLAGS.ImageOK=1;
                ReadImageVersionInFlash(); // tsai 6/2 for ACP FW install command
            }
            DBCFLAGS.GetFileOK=0;
        }
    } else {
        if (swResetByte==0x5A) { //bootloader update ok
            swResetByte=0;
        }
    }
}

uint8_t BMFTPReqInsall(uint8_t *candata)
{
    uint8_t retb = UpgradeAdmit;
    
    ReadImageVersionInFlash();
    isNeedUpdate( candata, 1);
    retb =UpgradeAdmit;
    UpgradeAdmit=0;

    return retb;
}
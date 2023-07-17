/* 
 * File:   can_dbc_usages_bmftp.h
 * Author: hugo.huang
 *
 * Created on 2021?2?23?, ?? 10:53
 */

#ifndef CAN_DBC_USAGES_BMFTP_H
#define	CAN_DBC_USAGES_BMFTP_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FWAddrBase 0x040000 // 0x3000 tsai 7/23/2021 256K
#define EncryptFWAddrBase (FWAddrBase + 0x040000) //0x080000 tsai 256K

    
#define FileStartCMD    1
#define BlockStartCMD   5
#define BlockFragment   2
    
typedef union { 
    struct {
        uint8_t StartCMD;
        uint8_t StartSize;
        uint8_t  highside;
        uint16_t BlkSize; 
        uint8_t Timeout;
        uint8_t StartDestAdrs; 
        uint8_t REVS;
    } __attribute__ ((__packed__)) fileStart;
    struct {
        uint8_t StartCMD;
        uint16_t Counter;
        
        uint8_t REVS;
    } __attribute__ ((__packed__)) blockStart;
    struct {
        uint8_t FragmntCMD    :4;
        uint8_t FragmntCNT    :4;
        uint8_t FragmntData[7];
    } __attribute__ ((__packed__)) blkfragmnt;
    uint8_t eightbytes[8];
} FileReq_t;

typedef union { 
    struct {
        uint8_t RemainingAlarm : 1;
        uint8_t ImageOK : 1;
        uint8_t GetFileOK : 1;
        uint8_t forceUpdate :1;
        uint8_t   : 4; 
    } __attribute__ ((__packed__));
    uint8_t byte;
} DBCFLAGS_t ;

//void BMFTPinit(void);
//extern UINT8  GetFileOK;
extern  DBCFLAGS_t DBCFLAGS ;
extern UINT8 FlashVerion[];
UINT16 updateCRC16(UINT16 crc, UINT8 *data, UINT8 len); //tsai 8/3
void GetUpgradeFlag(void); //tsai 8/5
void ISImageOK(void); //tsai 8/5
uint8_t BMFTPReqInsall(uint8_t *candata);
uint8_t DecryptAndVerifyData(void);
void ReadImageVersionInFlash(void);
#ifdef	__cplusplus
}
#endif

#endif	/* CAN_DBC_USAGES_BMFTP_H */


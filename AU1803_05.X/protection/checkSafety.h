/* 
 * File:   checkSafety.h
 * Author: frank.tsai
 *
 * 
 */

#ifndef CHECKSAFETY_H
#define	CHECKSAFETY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define COV_PROTECTION_DELAY        1000    // ms
#define COV_OVER_VMAX               4.1f    // 4100mv
#define COV_RELEASE_DELAY           1000    // ms
#define COV_UNOVER_IBATT            0       // 0A
#define COV_UNOVER_VMAX             3.9f    // 3900mv
#define COV_UNOVER_RSOC             95      // 95%
    
#define POV_PROTECTION_DELAY        1000    // ms
#define POV_OVER_VMAX               57.4f   // v
#define POV_RELEASE_DELAY           1000    // ms
#define POV_UNOVER_IBATT            0       // 0A
#define POV_UNOVER_VMAX             56.0f   // v
    
#define EOCV_PROTECTION_DELAY       1000    // ms
#define EOCV_SET_MAX_BITT_RBC       290     // 300mA
#define EOCV_SET_MAX_BITT_XBP       1100    // 1100mA
#define EOCV_SET_MIN_BITT           0       // 0A
#define EOCV_SET_MIN_VOLT_TH        3.92f   // 3920mV
#define EOCV_UNSET_BITT             -3000   // 3A
#define EOCV_UNSET_RSOC             95      // 95%
    
#define CUV_PROTECTION_DELAY        1000    // ms
#define CUV_SET_TH                  2.5f    // mv
#define CUV_RELEASE_DELAY           1000    // ms
#define CUV_UNSET_IBATT             0       // 0mA
#define CUV_UNSET_TH                3.5f    // mv
    
#define PUV_PROTECTION_DELAY        1000    // ms
#define PUV_SET_TH                  35.0f   // v
#define PUV_RELEASE_DELAY           1000    // ms
#define PUV_UNSET_IBATT             0       // 0mA
#define PUV_UNSET_TH                49.0f   // v

#define OIMBV_PROTECTION_DELAY      3000    // ms
#define OIMBV_SET_MAX_VOLT          3.8f    // v
#define OIMBV_SET_DELTA_VOLT        0.3f    // v
#define OIMBV_SET_IBATT             0       // 0mA
#define OIMBV_RELEASE_DELAY         3000    // ms
#define OIMBV_UNSET_DELTA_VOLT      0.25f   // v

#define OCI_PROTECTED_DELAY         2000    // 2000mS
#define OCI_SET_IBATT_RBC           15500   // 15.5A
#define OCI_SET_IBATT_XBC           29000   // 29A
#define OCI_RELEASE_DELAY           10000   // 10000mS

#define ODI_1_PROTECTION_DELAY      5000    // 5000mS
#define ODI_1_SET_TH                -120000 // -120A
#define ODI_1_RELEASE_DELAY         10000   // 10000mS

#define ODI_2_PROTECTION_DELAY      1000    // 1000mS
#define ODI_2_SET_TH                -150000 // -150A
#define ODI_2_RELEASE_DELAY         10000   // 10000mS

#define ODI_3_PROTECTION_DELAY      750     // 750mS
#define ODI_3_SET_TH                -200000 // -200A
#define ODI_3_RELEASE_DELAY         10000   // 10000mS

#define ODI_4_PROTECTION_DELAY      750     // 750mS
#define ODI_4_SET_TH                -200000 // -200A
#define ODI_4_RELEASE_DELAY         10000   // 10000mS

#define H_ODI_PROTECTION_DELAY      160     // 160mS
#define H_ODI_SET_TH                -268000 // -268A
#define H_ODI_RELEASE_DELAY         10000   // 10000mS

#define H_SCI_PROTECTION_DELAY      400     // 400uS
#define H_SCI_SET_TH                -356000 // -356A
#define H_SCI_RELEASE_DELAY         10000   // 10000mS

#define COT_PROTECTION_DELAY_TEMP   3000    // 3000mS
#define COT_PROTECTION_DELAY_IBATT  500     // 500mS
#define COT_SET_TH                  60      // 60C
#define COT_SET_IBATT               100     // 100mA
#define COT_RELEASE_DELAY           3000    // 3000mS
#define COT_UNSET_TH                55      // 55C

#define DOT_PROTECTION_DELAY        3000    // 3000mS
#define DOT_SET_TH                  80      // 80C
#define DOT_IDLE_SET_TH             87      // 87C
#define DOT_SET_IBATT_TH            -1000   // -1A
#define DOT_RELEASE_DELAY           3000    // 3000mS
#define DOT_UNSET_TH                70      // 70C

#define CUT_PROTECTION_DELAY_TEMP   3000    // 3000mS
#define CUT_PROTECTION_DELAY_IBATT  500     // 500mS
#define CUT_SET_TH                  0       // 0C
#define CUT_SET_IBATT_TH            0       // 0A
#define CUT_RELEASE_DELAY           3000    // 3000mS
#define CUT_UNSET_TH                1       // 1C

#define DUT_PROTECTION_DELAY        3000    // 3000mS
#define DUT_SET_TH                  -20     // -20C
#define DUT_RELEASE_DELAY           3000    // 3000mS
#define DUT_UNSET_TH                -15     // -15C

#define MOT_PROTECTION_DELAY        3000    // 3000mS
#define MOT_SET_TH                  110     // 110C
#define MOT_RELEASE_DELAY           3000    // 3000mS
#define MOT_UNSET_TH                70      // 70C

#define UVPF_PROTECTION_DELAY       1800    // 1800S
#define UVPF_SET_IBATT              0       // 0A
#define UVPF_SET_VCELL_MIN          2.0f    // 2.0V

#define OVPF_PROTECTION_DELAY       10      // 10S
#define OVPF_SET_IBATT              0       // 0A
#define OVPF_SET_CELL_VMAX          4.2f    // 4.2V

#define OTPF_PROTECTION_DELAY       20      // 20S
#define OTPF_SET_TH                 90      // 90C
#define OTPF_SET_IBATT_TH           -200    // -200mA

#define CIMPF_PROTECTION_DELAY      300     // 300S
#define CIMPF_SET_VOLT_TH           3.8     // 3.8V
#define CIMPF_SET_DELTA_VOLT        0.5     // 0.5V

#define THOPF_PROTECTION_DELAY      20      // 20S
#define THOPF_SET_TEMP              0       // 0C

#define THSPF_PROTECTION_DELAY      20      // 20S
#define THSPF_SET_TH                90      // 90C

#define CFETPF_PROTECTION_DELAY     20      // 20S
#define CFETPF_SET_TH               500     // 500mA

#define DFETPF_PROTECTION_DELAY     20      // 20S
#define DFETPF_SET_TH               -500    // -500mA

#define BQ_F_PROTECTION_DELAY       2500    // 2500ms

typedef union {
    struct {
    unsigned long COVBit     :1;     //0    CFET OFF
    unsigned long POVBit     :1;     //1    CFET OFF
    unsigned long EOCVBit    :1;     //2    CFET OFF
    unsigned long CUVBit     :1;     //3                DFET OFF
    unsigned long PUVBit     :1;     //4                DFET OFF
    unsigned long OIMBVBit   :1;     //5    CFET OFF    DFET OFF
    unsigned long OCIBit     :1;  // x  6   CFET OFF
    unsigned long ODI_1Bit   :1;     //7                DFET OFF
    unsigned long ODI_2Bit   :1;  //?  8                DFET OFF
    unsigned long ODI_3Bit   :1;  // xx 9               DFET OFF
    unsigned long ODI_4Bit   :1;     //10               DFET OFF
    unsigned long H_ODIBit   :1;  // x  11              DFET OFF
    unsigned long H_SCIBit   :1;  // x  12              DFET OFF
    unsigned long COTBit     :1;     //13   CFET OFF            
    unsigned long DOTBit     :1;     //14   CFET OFF    DFET OFF
    unsigned long CUTBit     :1;     //15   CFET OFF
    unsigned long DUTBit     :1;     //16   CFET OFF    DFET OFF
    unsigned long MOTBit     :1;  //TS3 17  CFET OFF    DFET OFF
    unsigned long UVPFBit    :1;     //18    , 0    CFET OFF    DFET OFF
    unsigned long OVPFBit    :1;     //19    , 1    CFET OFF    DFET OFF
    unsigned long OTPFBit    :1;     //20    , 2    CFET OFF    DFET OFF 
    unsigned long FUSEPFBit  :1;  // xx  21  , 3    CFET OFF    DFET OFF 
    unsigned long CIMPFBit   :1;     //22    , 4    CFET OFF    DFET OFF
    unsigned long THOPFBit   :1;     //23    , 5    CFET OFF    DFET OFF
    unsigned long THSPFBit   :1;     //24    , 6    CFET OFF    DFET OFF
    unsigned long CFETPFBit  :1;  //x  25    , 7    CFET OFF    DFET OFF
    unsigned long DFETPFBit  :1;  //x  26    , 8    CFET OFF    DFET OFF
    unsigned long ULP_BQ_F         :1;    // 27:    CFET OFF    DFET OFF
    unsigned long ULP_MCU_IO_F     :1;    // 28     CFET OFF    DFET OFF
    unsigned long ULP_MCU_CLK_F    :1;    // 29     CFET OFF    DFET OFF
    unsigned long ULP_MCU_PC_F     :1;    // 30     CFET OFF    DFET OFF
    unsigned long ULP_MCU_REG_F    :1;    // 31     CFET OFF    DFET OFF
    unsigned char ULP_MCU_WDT_F    :1;    // 32     CFET OFF    DFET OFF
    unsigned char ULP_MCU_RAM_F    :1;    // 33     CFET OFF    DFET OFF
    unsigned char ULP_MCU_FLASH_F  :1;    // 34     CFET OFF    DFET OFF
    unsigned char ULP_MCU_EE_F     :1;    // 35     CFET OFF    DFET OFF
    unsigned char MOSsState   :4;
    } __attribute__ ((__packed__));
    struct {
        unsigned long fault_VP : 6;
        unsigned long fault_CP : 7;
        unsigned long fault_TP : 5;
        unsigned long fault_PFP : 9;
        unsigned long fault_ULP0 : 5;
        unsigned char fault_ULP1 : 4;
        unsigned char bmsState : 4;
    } __attribute__ ((__packed__)); 
    struct {
        unsigned long allFaults32;
        unsigned char fault8_UL4bits;
    } __attribute__ ((__packed__));
    unsigned char fivebytes[5] ;
} BmsSysProtectionBits_t;

extern BmsSysProtectionBits_t sysProtection;
extern int16_t EOCV_OVER_IBATT;
extern int16_t OCI_OVER_IBATT;

void Protection_init(void);
void ProtectionMonitor(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CHECKSAFETY_H */


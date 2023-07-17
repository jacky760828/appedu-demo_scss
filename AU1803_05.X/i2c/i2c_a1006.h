/* 
 * File:   i2c_cmd.h
 * Author: frank.tsai
 *
 * Created on 2020?7?6?, ?? 1:04
 */

#ifndef I2C_CMD_H
#define	I2C_CMD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stddef.h"
#include "../smbus/smbus.h"
#include "i2cstate.h"
#include "../sys_time/sys_time.h"

#define A1006_I2C_BUFF I2C_SMB_BUFF

#define A1006_I2C_ADDR          0xA0
#define A1006_DETECT_MAX_TIMES  5
#define A1006_UID               0x0310
#define A1006_UID_size          16
#define A1006_CERTIF            0x0200
#define A1006_CERTIF_size       128
#define A1006_CHALLENGE         0x0800
#define A1006_CHALLENGE_size    44
#define A1006_RESPONSE          0x0801
#define A1006_RESPONSE_size     44
    
typedef enum {
    SEND_CHALLENGE      = 1,
    GET_CERTIF_1        = 2,
    GET_CERTIF_2        = 3,
    GET_UID             = 4,
    GET_RESPONSE        = 5,
    GET_CERTIF          = 7,
    WAIT_CHALLENGE_DATA = 8,        
    GOT_RESPONSE        = 9,
    SET_TIMEOUT         = 10,
    AUTH_COMPLETE       = 12,
} REQ_TYPE;


typedef struct {
    uint8_t Signature[2 + A1006_UID_size];
    uint8_t Certificate[2 + A1006_CERTIF_size];
    uint8_t Challenge[3 + A1006_CHALLENGE_size];
    uint8_t Response[2 + A1006_RESPONSE_size];
    uint8_t reqAuthStep;
    uint8_t respID;
    uint8_t frameRange :4;
    uint8_t reqID :4;
    uint8_t dataOffset;
    uint8_t AuthStep;
    uint8_t getResponse_flag    :1;
    uint8_t sendChallenge_flag  :1;
    uint8_t getChallenge_flag   :1;
    uint8_t getSignature_flag   :1;
    uint8_t getCertificate_flag :1;
    uint8_t StartreqComplete    :1;
    uint8_t Exist               :1;
    uint8_t nop                 :1;
} Auth_TypIeDef;

extern Auth_TypIeDef A1006_Auth;

void initAuthentification(void);   
void setRecChallenge(void);
void a1006Authentication(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_CMD_H */


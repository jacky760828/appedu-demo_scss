/******************************************************************************
 *
 * FILE: version_config.h
 *
 * DESCRIPTION: 
 *
 * Copyright(c) 2019 C-Technology United.
 * All Rights Reserved
 *
 * Overview:
 *
 *****************************************************************************/
#ifndef VERSION_CONFIG_H
#define	VERSION_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
// 12/7 tsai move here
#define PROTOCOL_VERSION                27 //0x02 0x01 frank 7/30 for test
#define FIRMWARE_VERSION_MAJOR          0x00  // 
#define FIRMWARE_VERSION_MINOR          0x08
#define FIRMWARE_VERSION_BUILD          0x03  //

extern const uint8_t protocolVersion[];
extern const uint8_t fwVersionMajor[];
extern const uint8_t fwVersionMinor[];
extern const uint8_t fwVersionBuild[];
extern const uint8_t fwBuildDate[];
#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_CONFIG_H */


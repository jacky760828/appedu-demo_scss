#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

typedef unsigned char   UINT8;
typedef unsigned int    UINT16;
typedef unsigned long   UINT32;
typedef	signed char	    INT8;
typedef	signed int	    INT16;
typedef	signed long	    INT32;

typedef unsigned char   UBYTE;

typedef UINT8           BOOL;

#define FALSE           ((BOOL)0)
#define TRUE            ((BOOL)1)

typedef struct {
    UINT8 bit0 : 1;
    UINT8 bit1 : 1;
    UINT8 bit2 : 1;
    UINT8 bit3 : 1;
    UINT8 bit4 : 1;
    UINT8 bit5 : 1;
    UINT8 bit6 : 1;
    UINT8 bit7 : 1;
} BITS;

typedef struct {
    BITS byte0;
    BITS byte1;
    BITS byte2;
    BITS byte3;
} BYTES;

#endif  // PLATFORM_TYPES_H
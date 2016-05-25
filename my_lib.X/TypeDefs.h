#ifndef __GENERIC_TYPE_DEFS_H_
#define __GENERIC_TYPE_DEFS_H_


typedef enum { FALSE = 0, TRUE } bool_t;    /* Undefined size */
typedef enum { CLEAR = 0, SET } bit_t;

#include <stdint.h>

typedef union
{
    uint8_t Val;
    struct
    {
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    } bits;
} uint8_val_t, uint8_bits_t;


#endif /* __GENERIC_TYPE_DEFS_H_ */

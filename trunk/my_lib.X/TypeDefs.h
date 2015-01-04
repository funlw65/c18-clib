/*******************************************************************

                  Generic Type Definitions

********************************************************************
 FileName:        GenericTypeDefs.h
 Dependencies:    None
 Processor:       PIC10, PIC12, PIC16, PIC18, PIC24, dsPIC, PIC32
 Compiler:        MPLAB C Compilers for PIC18, PIC24, dsPIC, & PIC32
                  Hi-Tech PICC PRO, Hi-Tech PICC18 PRO
 Company:         Microchip Technology Inc.

 Software License Agreement

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") is intended and supplied to you, the Company's
 customer, for use solely and exclusively with products manufactured
 by the Company.

 The software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************/

#ifndef __GENERIC_TYPE_DEFS_H_
#define __GENERIC_TYPE_DEFS_H_

/* Specify an extension for GCC based compilers */
#if defined(__GNUC__)
#define __EXTENSION __extension__
#else
#define __EXTENSION
#endif

#if !defined(__PACKED)
    #define __PACKED
#endif

/* get compiler defined type definitions (NULL, size_t, etc) */
#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	/* _STDDEF */

#ifndef	NULL
#define	NULL	(0)
#endif	/* NULL */

extern int	errno;			/* system error number */
/* end get compiler defined type definitions (NULL, size_t, etc) */

typedef enum _BOOL { FALSE = 0, TRUE } BOOL;    /* Undefined size */
typedef enum _BIT { CLEAR = 0, SET } BIT;

#define PUBLIC                                  /* Function attributes */
#define PROTECTED
#define PRIVATE   static

/* INT is processor specific in length may vary in size */
typedef signed int          INT;
typedef signed char         INT8;
typedef signed short int    INT16;
typedef signed long int     INT32;

/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
__EXTENSION typedef signed long long    INT64;
#endif

/* UINT is processor specific in length may vary in size */
typedef unsigned int        UINT;
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
/* 24-bit type only available on C18 */
#if defined(__18CXX)
typedef unsigned short long UINT24;
#endif
typedef unsigned long int   UINT32;     /* other name for 32-bit integer */
/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
__EXTENSION typedef unsigned long long  UINT64;
#endif

typedef union
{
    UINT8 Val;
    struct
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
    } bits;
} UINT8_VAL, UINT8_BITS;

typedef union 
{
    UINT16 Val;
    UINT8 v[2] __PACKED;
    struct __PACKED
    {
        UINT8 LB;
        UINT8 HB;
    } byte;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
    } bits;
} UINT16_VAL, UINT16_BITS;

/* 24-bit type only available on C18 */
#if defined(__18CXX)
typedef union
{
    UINT24 Val;
    UINT8 v[3] __PACKED;
    struct __PACKED
    {
        UINT8 LB;
        UINT8 HB;
        UINT8 UB;
    } byte;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
    } bits;
} UINT24_VAL, UINT24_BITS;
#endif

typedef union
{
    UINT32 Val;
    UINT16 w[2] __PACKED;
    UINT8  v[4] __PACKED;
    struct __PACKED
    {
        UINT16 LW;
        UINT16 HW;
    } word;
    struct __PACKED
    {
        UINT8 LB;
        UINT8 HB;
        UINT8 UB;
        UINT8 MB;
    } byte;
    struct __PACKED
    {
        UINT16_VAL low;
        UINT16_VAL high;
    }wordUnion;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
        __EXTENSION UINT8 b24:1;
        __EXTENSION UINT8 b25:1;
        __EXTENSION UINT8 b26:1;
        __EXTENSION UINT8 b27:1;
        __EXTENSION UINT8 b28:1;
        __EXTENSION UINT8 b29:1;
        __EXTENSION UINT8 b30:1;
        __EXTENSION UINT8 b31:1;
    } bits;
} UINT32_VAL;

/* MPLAB C Compiler for PIC18 does not support 64-bit integers */
#if !defined(__18CXX)
typedef union
{
    UINT64 Val;
    UINT32 d[2] __PACKED;
    UINT16 w[4] __PACKED;
    UINT8 v[8]  __PACKED;
    struct __PACKED
    {
        UINT32 LD;
        UINT32 HD;
    } dword;
    struct __PACKED
    {
        UINT16 LW;
        UINT16 HW;
        UINT16 UW;
        UINT16 MW;
    } word;
    struct __PACKED
    {
        __EXTENSION UINT8 b0:1;
        __EXTENSION UINT8 b1:1;
        __EXTENSION UINT8 b2:1;
        __EXTENSION UINT8 b3:1;
        __EXTENSION UINT8 b4:1;
        __EXTENSION UINT8 b5:1;
        __EXTENSION UINT8 b6:1;
        __EXTENSION UINT8 b7:1;
        __EXTENSION UINT8 b8:1;
        __EXTENSION UINT8 b9:1;
        __EXTENSION UINT8 b10:1;
        __EXTENSION UINT8 b11:1;
        __EXTENSION UINT8 b12:1;
        __EXTENSION UINT8 b13:1;
        __EXTENSION UINT8 b14:1;
        __EXTENSION UINT8 b15:1;
        __EXTENSION UINT8 b16:1;
        __EXTENSION UINT8 b17:1;
        __EXTENSION UINT8 b18:1;
        __EXTENSION UINT8 b19:1;
        __EXTENSION UINT8 b20:1;
        __EXTENSION UINT8 b21:1;
        __EXTENSION UINT8 b22:1;
        __EXTENSION UINT8 b23:1;
        __EXTENSION UINT8 b24:1;
        __EXTENSION UINT8 b25:1;
        __EXTENSION UINT8 b26:1;
        __EXTENSION UINT8 b27:1;
        __EXTENSION UINT8 b28:1;
        __EXTENSION UINT8 b29:1;
        __EXTENSION UINT8 b30:1;
        __EXTENSION UINT8 b31:1;
        __EXTENSION UINT8 b32:1;
        __EXTENSION UINT8 b33:1;
        __EXTENSION UINT8 b34:1;
        __EXTENSION UINT8 b35:1;
        __EXTENSION UINT8 b36:1;
        __EXTENSION UINT8 b37:1;
        __EXTENSION UINT8 b38:1;
        __EXTENSION UINT8 b39:1;
        __EXTENSION UINT8 b40:1;
        __EXTENSION UINT8 b41:1;
        __EXTENSION UINT8 b42:1;
        __EXTENSION UINT8 b43:1;
        __EXTENSION UINT8 b44:1;
        __EXTENSION UINT8 b45:1;
        __EXTENSION UINT8 b46:1;
        __EXTENSION UINT8 b47:1;
        __EXTENSION UINT8 b48:1;
        __EXTENSION UINT8 b49:1;
        __EXTENSION UINT8 b50:1;
        __EXTENSION UINT8 b51:1;
        __EXTENSION UINT8 b52:1;
        __EXTENSION UINT8 b53:1;
        __EXTENSION UINT8 b54:1;
        __EXTENSION UINT8 b55:1;
        __EXTENSION UINT8 b56:1;
        __EXTENSION UINT8 b57:1;
        __EXTENSION UINT8 b58:1;
        __EXTENSION UINT8 b59:1;
        __EXTENSION UINT8 b60:1;
        __EXTENSION UINT8 b61:1;
        __EXTENSION UINT8 b62:1;
        __EXTENSION UINT8 b63:1;
    } bits;
} UINT64_VAL;
#endif /* __18CXX */

/***********************************************************************************/

#undef __EXTENSION

#endif /* __GENERIC_TYPE_DEFS_H_ */

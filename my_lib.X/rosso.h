/* 
 * File:   rosso.h
 * Author: Vasile Guta Ciucur
 *
 * Created on December 10, 2014, 4:36 PM
 */

#ifndef ROSSO_H
#define	ROSSO_H

/* Definitions for _HTC_EDITION_ values */
//#define __LITE__ 0
//#define __STD__ 1
//#define __PRO__ 2

/* common definitions */

#define	___mkstr1(x)	#x
#define	___mkstr(x)	___mkstr1(x)

#define _OMNITARGET	((void *)0xFFFFFFFF)

extern const char __xc8_OPTIM_SPEED;

#pragma intrinsic(__builtin_software_breakpoint)
extern void __builtin_software_breakpoint(void);


// flash_write is no longer supported. Use the peripheral library implementation
// --runtime=+plib must be used
#define _FLASH_UNSUPPORTED __attribute__((__unsupported__("The flash_write routine is no longer supported. Please use the peripheral library functions: WriteBytesFlash, WriteBlockFlash or WriteWordFlash")))
_FLASH_UNSUPPORTED void flash_write(const unsigned char *, unsigned int, __far unsigned char *);
#define FLASH_WRITE(src, size, dest)	flash_write(src,size,dest)
#define flash_erase(addr)		EraseFlash(addr,(addr)+1)
#define FLASH_ERASE(addr)		EraseFlash(addr,(addr)+1)

/* Get definitions for errata codes that may be
 * defined in preprocessor symbol _ERRATA_TYPES */
#define	ERRATA_4000	(1<<0)	// Program mem accesses/jumps across 4000h address boundary
#define	ERRATA_FASTINTS (1<<1)	// Fast interrupt shadow registers corruption
#define	ERRATA_LFSR	(1<<2)	// Broken LFSR instruction
#define	ERRATA_MINUS40	(1<<3)	// Program memory reads at -40 degrees
#define	ERRATA_RESET	(1<<4)	// GOTO instruction cannot exist at reset vector
#define	ERRATA_BSR15	(1<<5)	// Flag problems when BSR holds value 15
#define	ERRATA_DAW	(1<<6)	// Broken DAW instruction
#define	ERRATA_EEDATARD	(1<<7)	// Read EEDAT in immeadiate instruction after RD set
#define	ERRATA_EEADR	(1<<8)	// Don't set RD bit immediately after loading EEADR
#define	ERRATA_EE_LVD	(1<<9)	// LVD must stabilise before writing EEPROM
#define	ERRATA_FL_LVD	(1<<10)	// LVD must stabilise before writing Flash
#define	ERRATA_TBLWTINT	(1<<11)	// Clear interrupt registers before tblwt instruction
#define	ERRATA_FW4000	(1<<12)	// Flash write exe must act on opposite side of 4000h boundary.
#define	ERRATA_RESETRAM	(1<<13)	// RAM contents can corrupt if async. reset occur during write access.
#define	ERRATA_FETCH	(1<<14)	// Corruptable instruction fetch. Apply FFFFh at required locations.


/* Macros to access bytes within words and words within longs */
#define LOW_BYTE(x)     ((unsigned char)((x)&0xFF))
#define HIGH_BYTE(x)    ((unsigned char)(((x)>>8)&0xFF))
#define LOW_WORD(x)     ((unsigned short)((x)&0xFFFF))
#define HIGH_WORD(x)    ((unsigned short)(((x)>>16)&0xFFFF))

/* C access to assembler insructions */
#define	CLRWDT()	asm(" clrwdt")
#define	ClrWdt()	asm(" clrwdt")
// function version of nop
#pragma intrinsic(__nop)
extern void __nop(void);
#define NOP()		__nop()
#define	Nop()		__nop()
//#define	RESET()		asm(" reset")
//#define	Reset()		asm(" reset")
#define SLEEP()		asm(" sleep")
#define Sleep()		asm(" sleep")

#define	__PROG_CONFIG(a, x)	\
			__config(___mkstr(__PROG_CONFIG), ___mkstr(pic18), a, x)

#define __CONFIG(a, b) \
			__config(___mkstr(__CONFIG), ___mkstr(pic18), ___mkstr(b))

#ifndef __J_PART
#define __IDLOC(w) \
			__config(___mkstr(__IDLOC), ___mkstr(pic18), ___mkstr(w))
#endif

/* Initialise device EEPROM (if available) with 8 bytes of data at a time eg.
 * // store initial values to first 16-bytes of EEPROM address range.
 * __EEPROM_DATA(0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07);
 * __EEPROM_DATA(0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F);*/
#if _EEPROMSIZE > 0
 #define __EEPROM_DATA(a, b, c, d, e, f, g, h) \
			 asm("\tpsect eeprom_data,class=EEDATA,noexec"); \
			 asm("\tdb\t" ___mkstr(a) "," ___mkstr(b) "," ___mkstr(c) "," ___mkstr(d) "," \
				      ___mkstr(e) "," ___mkstr(f) "," ___mkstr(g) "," ___mkstr(h))
#else
 #define __EEPROM_DATA(a, b, c, d, e, f, g, h)	// Added only for code portability
#endif

// Various helper macros used in EEPROM routines
#if	_EEPROMSIZE > 256
 #define _LOAD_EEADR(addr)	(EEADRH=(((addr)>>8)&0xFF),EEADR=((addr)&0xFF))
#else
 #define _LOAD_EEADR(addr)	(EEADR=((addr)&0xFF))
#endif

#if	_EEPROMSIZE > 0
 #if defined(_18F86K90_FAMILY_) || defined(_18F87K22_FAMILY_)
  #define _CLEAR_EEIF()	PIR6bits.EEIF=0
 #elif defined(_18F66K80_FAMILY_)
  #define _CLEAR_EEIF()	PIR4bits.EEIF=0
 #else
  #define _CLEAR_EEIF()	PIR2bits.EEIF=0
 #endif
#else
 #define _CLEAR_EEIF()
#endif

#if defined(SMALL_DATA)
 #define _LOAD_TBLPTR(addr)	                                                \
			TBLPTRU=0;                                                      \
			*((far unsigned char**)&TBLPTR)=(far unsigned char*)(addr)
#else
 #define _LOAD_TBLPTR(addr)	                                                \
			*((far unsigned char**)&TBLPTR)=(far unsigned char*)(addr)
#endif

// MACROS for EEPROM Access
/* macro versions of EEPROM read and write */

/* NOTE WELL:

   EEPROM_READ() is NOT safe to use immediately after any
   write to EEPROM, as it does NOT wait for WR to clear.  This is by
   design, to allow minimal code size if a sequence of reads is
   desired.  To guarantee uncorrupted writes insert
	while(WR)continue;
   before calling EEPROM_READ().
*/
#if _EEPROMSIZE > 0 && defined(_PLIB)
 #define EEPROM_READ(addr)	Read_b_eep(addr)
 #define eeprom_read(addr)	Read_b_eep(addr)
#else
 #define EEPROM_READ(addr)	0	// Added only for code portability
 #define eeprom_read(addr)	0
#endif

#if _EEPROMSIZE > 0 && defined(_PLIB)
 #define EEPROM_WRITE(addr, value)	(Busy_eep(), Write_b_eep(addr,value))
 #define eeprom_write(addr, value)	(Busy_eep(), Write_b_eep(addr,value))
#else
 #define EEPROM_WRITE(addr, value)	// Added only for code portability
 #define eeprom_write(addr, value)
#endif

/* Accurate read/write macros for 16-Bit timers */
/*** please note, the timer needs to be enabled ***
 *** to handle 16-Bit read/write operations for ***
 *** these routines to be of benefit ***/
#define T1RD16ON  T1CON|=0x80
#define T3RD16ON  T3CON|=0x80
#define WRITETIMER0(x) ((void)(TMR0H=((x)>>8),TMR0L=((x)&0xFF)))
#define WRITETIMER1(x) ((void)(TMR1H=((x)>>8),TMR1L=((x)&0xFF)))
#define WRITETIMER3(x) ((void)(TMR3H=((x)>>8),TMR3L=((x)&0xFF)))
#define READTIMER0() (TMR0)
#define READTIMER1() (TMR1)
#define READTIMER3() (TMR3)

/////////////////////
// sei()= Set interrupts (Activate global interrupts on high/low priority system)
#define sei() RCON=0;RCONbits.IPEN=1;INTCONbits.PEIE=1;INTCONbits.GIE=1;
// cli()= Clear Interrupts (Deactivate global interrupts)
#define cli() INTCONbits.GIE=0;

#define _XTAL_FREQ 64000000
#include <pic18f46k22.h>

#define AllDigital() ANSELA=0;ANSELB=0;ANSELC=0;ANSELD=0;ANSELE=0;ADCON0=0;ADCON1=0;ADCON2=0;CM1CON0=0;CM2CON0=0;CM2CON1=0;
#ifdef ONBOARD // onboard definitions for "Il Pinguino Rosso" dev. board
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RA4
#define OnBoardLED_dir     TRISCbits.RC2
#define OnBoardButton_dir  TRISAbits.RA4
#endif


#ifdef NOBOOT // then set the configuration bits for 16MHz crystal with PLL at 64MHz maximum speed
#pragma config FOSC = HSMP      // Oscillator Selection bits (HS oscillator (medium power 4-16 MHz))
#pragma config PLLCFG = ON      // 4X PLL Enable (Oscillator multiplied by 4)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
#endif //NOBOOT

#endif	/* ROSSO_H */


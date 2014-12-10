/* 
 * File:   rosso_old.h
 * Author: Worker
 *
 * Created on December 10, 2014, 4:36 PM
 */

#ifndef ROSSO_OLD_H
#define	ROSSO_OLD_H


#ifdef ROSSO_SER
#define _XTAL_FREQ 64000000
#include <p18f46k22.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x300
#define APP_HINT           0x308
#define APP_LINT           0x318
#endif
#endif
#define AllDigital()       ANSELA=0;ANSELB=0;ANSELC=0;ANSELD=0;ANSELE=0;ADCON0=0;ADCON1=0;ADCON2=0;CM1CON0=0;CM2CON0=0;CM2CON1=0;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RA4
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISA4
#endif
#endif

#ifdef PINGUINOP8B2_HID
#define _XTAL_FREQ 48000000
#include <p18f4550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x1000
#define APP_HINT           0x1008
#define APP_LINT           0x1018
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RC2
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISC2
#endif
#endif

#ifdef PINGUINOP8B2_CDC
#define _XTAL_FREQ 48000000
#include <p18f4550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x800
#define APP_HINT           0x808
#define APP_LINT           0x818
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RC2
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISC2
#endif
#endif

#ifdef PINGUINOP8B3_HID
#define _XTAL_FREQ 48000000
#include <p18f4550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x1000
#define APP_HINT           0x1008
#define APP_LINT           0x1018
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RA4
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISA4
#endif
#endif

#ifdef PINGUINOP8B3_CDC
#define _XTAL_FREQ 48000000
#include <p18f4550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x800
#define APP_HINT           0x808
#define APP_LINT           0x818
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RA4
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISA4
#endif
#endif

#ifdef FREEJALDUINO_CDC
#define _XTAL_FREQ 48000000
#include <p18f2550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x800
#define APP_HINT           0x808
#define APP_LINT           0x818
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardLED2        LATAbits.LATA4
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardLED2_dir    TRISAbits.TRISA4
#endif
#endif

#ifdef FREEJALDUINO_HID
#define _XTAL_FREQ 48000000
#include <p18f2550.h>
#ifdef __18CXX
#ifndef NOBOOT
#define APP_START          0x1000
#define APP_HINT           0x1008
#define APP_LINT           0x1018
#endif
#endif
#define AllDigital()       ADCON0=0;ADCON1=15;ADCON2=0;CMCON=7;
#ifdef ONBOARD // onboard definitions
#define OnBoardLED         LATCbits.LATC2
#define OnBoardButton      PORTAbits.RA4
#define OnBoardLED_dir     TRISCbits.TRISC2
#define OnBoardButton_dir  TRISAbits.TRISA4
#endif
#endif

#ifdef NOBOOT
#if (_XTAL_FREQ == 64000000)
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
#elif (_XTAL_FREQ == 48000000)
#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = ON
#pragma config BORV     = 3
#pragma config VREGEN   = ON		//USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF		//NOTE: modifying this value here won't have an effect
    //      #pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
    //      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
    //      #pragma config CP2      = OFF
    //      #pragma config CP3      = OFF
#pragma config CPB      = OFF
    //      #pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
    //      #pragma config WRT2     = OFF
    //      #pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
    //      #pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
    //      #pragma config EBTR2    = OFF
    //      #pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF
#endif //F_CPU
#endif //NOBOOT

#endif	/* ROSSO_OLD_H */


/* 
 * File:   main.c
 * Author: texan
 *
 * Created on June 21, 2014, 4:18 AM
 */

#ifdef __XC8
#error Only for MPLAB C18 compiler!
#endif

//#define NOBOOT 1 // uncomment if you don't use a bootloader

// enable one of the following seven boards
#define ROSSO_SER 1 //(18F46K22, 16MHz crystal, 64MHz, BootBTN = RA4, BootLED = RC2)
//#define PINGUINOP8B2_HID 2 //(18F4550, 20MHz crystal, 48MHz, BootBTN and BootLED uses RC2)
//#define PINGUINOP8B2_CDC 3 //(18F4550, 20MHz crystal, 48MHz, BootBTN and BootLED uses RC2)
//#define PINGUINOP8B3_HID 4 //(18F4550, 20MHz crystal, 48MHz, BootBTN = RA4, BootLED = RC2)
//#define PINGUINOP8B3_CDC 5 //(18F4550, 20MHz crystal, 48MHz, BootBTN = RA4, BootLED = RC2)
//#define FREEJALDUINO_CDC 6 //(18F2550, 20MHz crystal, 48MHz, UserLED1 = RA4, UserLED2 = RC2)
//#define FREEJALDUINO_HID 7 //(18F2550, 20MHz crystal, 48MHz, UserLED1 = RA4, UserLED2 = RC2)

// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions

// enable one of the following two
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#include <my_delays.h>

/*
 * 
 */
void high_isr(void);
void low_isr(void);

#ifndef NOBOOT
#pragma romdata bootloader = 0x2A
const rom char bootloader[APP_START - 0x2A];
extern void _startup(void);

#pragma code AppVector = APP_START
void AppVector(void) {
    _asm GOTO _startup _endasm
}

#pragma code AppHighIntVector = APP_HINT
void AppHighIntVector(void) {
    _asm
        GOTO high_isr // branch to the high_isr()
    _endasm
}

#pragma code AppLowIntVector = APP_LINT
void low_vector(void) {
    _asm
        GOTO low_isr // branch to the low_isr()
    _endasm
}
#endif

#pragma code  // return to the default // code section
void main() {
    AllDigital();
    OnBoardLED_dir    = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
    while (1) {
        OnBoardLED = ~OnBoardLED;
        delay_150ms();
        delay_150ms();
        delay_100ms();
        delay_100ms(); //total of 500ms delay
    }
}

#pragma interrupt high_isr
void high_isr(void) {
}

//#pragma interruptlow low_isr
void low_isr(void) {
}


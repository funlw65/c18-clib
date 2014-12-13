/*
 * File:   main.c
 * Author: texan
 *
 * Created on June 21, 2014, 4:18 AM
 */

#include <GenericTypeDefs.h>
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
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#define TMR0_MILLIS // automatically sets the RATE to 1000
#include <rosso_isr_delays.h>
UINT32 previous,current;

// declaring the headers for the interrupt functions
void
#ifdef __XC8
interrupt
#endif
high_isr(void);

void
#ifdef __XC8
interrupt low_priority
#endif
low_isr(void);

#ifdef __18CXX
#ifndef NOBOOT
#pragma romdata bootloader = 0x6
const rom char bootloader[APP_START - 0x6];
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
#endif
void main() {
    AllDigital();
#ifdef ONBOARD
    OnBoardLED_dir    = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    ei(); // enable general interrupts
    tmr0_isr_init();
    previous = 0;
    while (1) {
        current = tmr0_millis();
        if((current - previous) >= 250){
            previous = current;
            OnBoardLED = ~OnBoardLED;
        }
    }
}


#ifdef __18CXX
#pragma interrupt high_isr
#endif
void
#ifdef __XC8
interrupt
#endif
high_isr(void) {
    //add here any high priority interrupt management functions
    tmr0_isr_intr();
}

#ifdef __18CXX
#pragma interruptlow low_isr
#endif
void
#ifdef __XC8
interrupt low_priority
#endif
low_isr(void) {
    //add here any low priority interrupt management functions

}


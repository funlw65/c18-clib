/*
 * File:   main_example.c
 * Author: @YourNameHere
 *
 * Created on @YourCurrentDate
 * Description: @What ever your app is doing, but for now is:
 *    This is a template for your "Il Pinguino Rosso" project.
 * 
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
// add here other definitions and include header files if needed

// add here your global variables and other fuctions you need


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
#pragma romdata bootloader = 0x06
const rom char bootloader[APP_START - 0x06];
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
#endif // let this be close to main function with nothing between
void main() {
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; // set pin as output
    OnBoardButton_dir = 1; // set pin as input
    OnBoardLED = 0; // set the LED off
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    while (1) {
        // Add your repeating code...
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
    //
}

#ifdef __18CXX
#pragma interruptlow low_isr
#endif
void
#ifdef __XC8
interrupt low_priority
#endif
low_isr(void) {
    //
}

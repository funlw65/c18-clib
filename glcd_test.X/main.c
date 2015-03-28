/* 
 * File:   main.c
 * Author: texan
 *
 * Created on February 6, 2015, 8:34 PM
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define PINGUINOP8B2_HID
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso_old.h> // processor type, speed, configuration bits, hardware, app_offset.
// add here other definitions and include header files if needed
#include <rosso_delays_old.h>
#include <rosso_glcd.h>

// add here your global variables, definitions and other fuctions you may need

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
#pragma code  // Let this code pragma
#endif // to be close to main function with nothing between!!!

void main() {
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; // set pin as output
    OnBoardButton_dir = 1; // set pin as input
    OnBoardLED = 0; // set the LED off
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    // Initialize the LCD
    GLCD_Init(0);

    // Select a font
    //GLCD_SelectFont(Arial_Bold_14, GLCD_ReadFontData, GLCD_BLACK);
    // Set a position
    GLCD_GotoXY(15, 10);
    // Print some text
    //GLCD_Puts_P(PSTR("KS0108-Treiber"));
    // a nice little round rect
    GLCD_DrawRoundRect(5, 5, 117, 20, 8, GLCD_BLACK);

    // Once again :)
    // Select a font
    //GLCD_SelectFont(Corsiva_12, GLCD_ReadFontData, GLCD_BLACK);
    // Set a position
    GLCD_GotoXY(5, 30);
    // Print some text
    //GLCD_Puts_P(PSTR("http://www.apetech.de\nmailto:me@apetech.de"));

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

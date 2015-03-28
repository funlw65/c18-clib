/* 
 * File:   main.c
 * Author: Vasile Guta-Ciucur
 *
 * Created on December 16, 2014, 9:34 PM
 */

/*
 * Description: a standard "Hello world" program for testing the 16x2 LCD.
 * It also tests the reading and displaying a string stored in the FLASH (pro-
 * gram space) opposed to one stored in RAM (well, I guess is just me, learning
 * how to do things in C18 and XC8...).
 *
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#include <rosso_delays.h>
#include <rosso_lcd4.h>
#include <rosso_conversion.h>

UINT8 s[4]; // buffer for number conversion
#ifdef __18CXX // this is for MPLAB C18 compiler
const rom UINT8 sf[]=" ENG\0"; // this should be stored in FLASH
#else // __XC8
const UINT8 sf[]=" ENG\0"; // this should be stored in FLASH
#endif
UINT8 sr[]="Hello World!\0";

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
#pragma code  // This must be close to main() function, with nothing between;
#endif        // Man, C18 is a real PITA regarding this.
void main() {
    UINT8 i = 0;
    UINT8 counter = 0;
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    lcd_init(LCD_HD44780);
    // signal the start four times
    for (i = 0; i < 4; i++) {
        OnBoardLED = 1;
        delay_100ms();
        delay_100ms();
        OnBoardLED = 0;
        delay_100ms();
        delay_100ms();
    }
    lcd_cursor_position(0, 0);
    lcd_write_str(sr);  // reading the string from the RAM
    lcd_write_strF(sf); // reading the string from the FLASH
    while (1) {
        // Add your repeating code...
        counter += 1; // count up to 255 and start again from zer0
        lcd_cursor_position(1, 0);
        byte2dec(counter, s);
        for (i = 0; i < 3; i++) _lcd_write_data(s[i]);
        delay_150ms();
        delay_150ms();
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


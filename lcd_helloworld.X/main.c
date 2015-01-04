/* 
 * File:   main.c
 * Author: Vasile Guta-Ciucur
 *
 * Created on December 16, 2014, 9:34 PM
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#include <rosso_delays.h>
#include <rosso_lcd4.h>
#include <rosso_conversion.h>

UINT8 s[4];

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
    UINT8 i = 0;
    UINT8 counter = 0;
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    //ei(); // enable general interrupts if needed
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
    _lcd_write_data('H');
    _lcd_write_data('e');
    _lcd_write_data('l');
    _lcd_write_data('l');
    _lcd_write_data('o');
    _lcd_write_data(' ');
    _lcd_write_data('W');
    _lcd_write_data('o');
    _lcd_write_data('r');
    _lcd_write_data('l');
    _lcd_write_data('d');
    _lcd_write_data('!');
    while (1) {
        // Add your repeating code...
        counter += 1;
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


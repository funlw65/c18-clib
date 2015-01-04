/*
 * File:   main.c
 * Author: Vasile Guta Ciucur
 *
 * Created on December 21, 2014, 1:39 AM
 */

/*
 * This program demonstrate the Weekly alarm feature of PCF8583,
 * exactly, Sunday and Saturday OFF and the rest of the days ON.
 * The INT pin of PCF8583 is connected to RC0 pin of 18F46K22 along
 * with a 10K pull-up resistor. The OnBoardLED (RC2) will simulate
 * the alarm and the OnBoardButton (RA4) will stop the alarm.
 * The clock and date is displayed on a 2x16 4bit LCD display (data
 * on the upper part of port B [B4-B7], RS on RE1 and E to RE2 pins).
 * PCF8583 is connected to I2C at 0xA2 address (to make room for an
 * eventual I2C EEPROM).
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#include <rosso_conversion.h>
#include <rosso_i2c_master.h>
#define PCF8583_USE_I2C
#define PCF8583_PHYSICAL_ADDRESS 0xA2 // 0x00 may be used by I2C EEPROM
#include <rosso_pcf8583.h>
//this time we are using the lcd library defaults for pinout
#include <rosso_delays.h> // required by LCD lib. bellow
#include <rosso_lcd4.h>

UINT8 s[4]; // buffer used for conversions

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
    UINT8 current_sec = 0, old_sec = 0;
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0; //set the LED off
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    i2c_init(I2C_100KHZ);
    lcd_init(LCD_HD44780);
    // set the pin who sense the interrupt from RTC Alarm
    TRISCbits.RC0 = 1; // set as input
    // =====================================================================
    // if you need to set the RTC, uncomment the following and run once
    RTC_seconds = 55; // 0 to 59
    RTC_minutes = 59; // 0 to 59
    RTC_hours = 23; // 0 to 23
    RTC_day = 13; // 1 to 31
    RTC_month = 5; // 1 to 12
    RTC_century = 20; //
    RTC_year = 14; // 0 to 99
    RTC_dayofweek = 0; // 0 to 6 (Sun=0, Mon=1, etc..)
    RTC_leapyear = 2; // 0 to 3 (0 - is leapyear and 1,2,3 not)
    // 2012 was leapyear    ( leapyear = 0)
    // 2014 is not leapyear ( leapyear = 2)
    //
    pcf8583_set_datetime(RTC_hours, RTC_minutes, RTC_seconds, RTC_dayofweek, RTC_day, RTC_month, RTC_leapyear, RTC_century, RTC_year);
    // =====================================================================

    // =================== SET THE ALARM ===================================
    pcf8583_en_dis_alarm(PCF8583_WEEKDAYS_ALARM);
    pcf8583_set_alarm_time(0,0,0);
    //In which days alarm is on: SU   MO   TU   WE   TH   FR     SA
    pcf8583_set_alarm_weekdays(CLEAR, SET, SET, SET, SET, SET, CLEAR);
    // =====================================================================

    while (1) {
        // Add your repeating code...
        pcf8583_get_datetime(&RTC_hours, &RTC_minutes, &RTC_seconds, &RTC_dayofweek, &RTC_day, &RTC_month, &RTC_leapyear, &RTC_century, &RTC_year);
        current_sec = RTC_seconds;
        if(current_sec != old_sec){
            old_sec = current_sec;
            // show time
            lcd_cursor_position(0, 0);
            byte2dec(RTC_hours, s);
            if(s[1] == ' ') _lcd_write_data('0');
            else _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            _lcd_write_data(':');
            byte2dec(RTC_minutes, s);
            if(s[1] == ' ') _lcd_write_data('0');
            else _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            _lcd_write_data(':');
            byte2dec(RTC_seconds, s);
            if(s[1] == ' ') _lcd_write_data('0');
            else _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            // show date on the second line
            lcd_cursor_position(1, 0);
            byte2dec(RTC_day, s);
            if(s[1] == ' ') _lcd_write_data('0');
            else _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            _lcd_write_data('/');
            byte2dec(RTC_month, s);
            if(s[1] == ' ') _lcd_write_data('0');
            else _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            _lcd_write_data('/');
            byte2dec(RTC_century, s);
            _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
            byte2dec(RTC_year, s);
            _lcd_write_data(s[1]);
            _lcd_write_data(s[2]);
        }
        if(PORTCbits.RC0 == 0) OnBoardLED = 1; // signal the alarm
        else OnBoardLED = 0;
        if(OnBoardButton == 0){ // if user btn pressed, stop alarm
            OnBoardLED = 0;
            pcf8583_stop_alarm();// it only stops the "sound" of the alarm,
            //doesn't disable it.
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

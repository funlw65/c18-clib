/* 
 * File:   main.c
 * Author: Vasile Guta Ciucur
 *
 * Created on December 18, 2014, 8:22 PM
 */

/*
 * Demonstrating I2C master, LCD, PCF8583, Delays, Conversions
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
interrupt
high_isr(void);

void
interrupt low_priority
low_isr(void);

void main() {
    UINT8 current_sec = 0, old_sec = 0;
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    i2c_init(I2C_100KHZ);
    lcd_init(LCD_HD44780);
    // =====================================================================
    // if you need to set the RTC, uncomment the following and run once
    RTC_seconds = 0; // 0 to 59
    RTC_minutes = 5; // 0 to 59
    RTC_hours = 17; // 0 to 23
    RTC_day = 13; // 1 to 31
    RTC_month = 5; // 1 to 12
    RTC_century = 20; //
    RTC_year = 14; // 0 to 99
    RTC_dayofweek = 2; // 0 to 6 (Sun, Mon, etc..)
    RTC_leapyear = 2; // 0 to 3 (o - is leapyear and 1,2,3 not)
    // 2012 was leapyear    ( leapyear = 0)
    // 2014 is not leapyear ( leapyear = 2)
    //
    pcf8583_set_datetime(RTC_hours, RTC_minutes, RTC_seconds, RTC_dayofweek, RTC_day, RTC_month, RTC_leapyear, RTC_century, RTC_year);
    // =====================================================================

    while (1) {
        // Add your repeating code...
        OnBoardLED = ~OnBoardLED; // blink seconds
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
        delay_150ms();
        delay_150ms();
        delay_100ms();
        delay_100ms();
    }
}

void
interrupt
high_isr(void) {
    //
}

void
interrupt low_priority
low_isr(void) {
    //
}


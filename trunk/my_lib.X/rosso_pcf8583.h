/* 
 * File:   rosso_pcf8583.h
 * Author: Vasile Guta-Ciucur
 *
 * Created on December 13, 2014, 6:50 PM
 */

/*
 * Requirements:
 * You have to configure and include the header for I2C or I2C2
 * before including this header.
 *
 * You also ave to make some definitions for the PCF8583 I2C address
 * and to define which I2C is used, like bellow:
 *
 * #define PCF8583_USE_I2C
 * #define PCF8583_PHYSICAL_ADDRESS 0xA0 // 0x00 may be used by I2C EEPROM
 * #include <rosso_pcf8583.h>
 *
 */

#ifndef ROSSO_PCF8583_H
#define	ROSSO_PCF8583_H

#if defined(PCF8583_USE_I2C)
  #if defined(PCF8583_USE_I2C2)
    #undef PCF8583_USE_I2C2
  #endif
#elif defined(PCF8583_USE_I2C2)
  #if defined(PCF8583_USE_I2C)
    #undef PCF8583_USE_I2C
  #endif
#else
  //#error "You must define which I2C use your PCF8583 RTC"
#endif

#if !defined(PCF8583_PHYSICAL_ADDRESS)
  #define PCF8583_PHYSICAL_ADDRESS 0xA2
#endif

#define PCF8583_W_ADDR   PCF8583_PHYSICAL_ADDRESS
#define PCF8583_R_ADDR   (PCF8583_W_ADDR + 1)
#define PCF8583_RAM_ADDR 0x10 // from $10 to $FF NVRAM

// Register addresses
#define PCF8583_CTRL_STATUS_REG   0x00
#define PCF8583_100S_REG          0x01
#define PCF8583_SECONDS_REG       0x02
#define PCF8583_MINUTES_REG       0x03
#define PCF8583_HOURS_REG         0x04
#define PCF8583_DATE_REG          0x05
#define PCF8583_MONTHS_REG        0x06
#define PCF8583_TIMER_REG         0x07
//
#define PCF8583_ALARM_CONTROL_REG 0x08
#define PCF8583_ALARM_100S_REG    0x09
#define PCF8583_ALARM_SECS_REG    0x0A
#define PCF8583_ALARM_MINS_REG    0x0B
#define PCF8583_ALARM_HOURS_REG   0x0C
#define PCF8583_ALARM_DATE_REG    0x0D
#define PCF8583_ALARM_MONTHS_REG  0x0E
#define PCF8583_ALARM_TIMER_REG   0x0F
// Commands for the Control/Status register.
#define PCF8583_START_COUNTING    0x00
#define PCF8583_STOP_COUNTING     0x80
// Constants for Alarm Type
#define PCF8583_NO_ALARM          0
#define PCF8583_DAILY_ALARM       1
#define PCF8583_WEEKDAYS_ALARM    2
// PCF8583_MONTH_ALARM // not implemented, TODO.

// TODO: Timer alarm not implemented yet
// TODO: Event counter not implemented yet


#endif	/* ROSSO_PCF8583_H */


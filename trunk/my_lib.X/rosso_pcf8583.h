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

volatile UINT8 RTC_seconds; // 00 to 59
volatile UINT8 RTC_minutes; // 00 to 59
volatile UINT8 RTC_hours; // 00 to 23
//--
volatile UINT8 RTC_day; // 1 to 31
volatile UINT8 RTC_month; // 1 to 12
volatile UINT8 RTC_century; // 20
volatile UINT8 RTC_year; // 00 to 99
volatile UINT8 RTC_dayofweek; // 0 to 6 (Sun = 0, Mon = 1, ..., Sat = 6)
volatile UINT8 RTC_leapyear; // 0 to 3 (0 - leapyear and 1,2,3 not)
// 2012 was leapyear    ( leapyear = 0)
// 2014 is not leapyear ( leapyear = 2)

//

void pcf8583_getyear(UINT8 * cn, UINT8 * yr) {
    BOOL err;
#if defined(PCF8583_USE_I2C)
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_RAM_ADDR);
    i2c_restart();
    err = i2c_write(PCF8583_R_ADDR);
    cn = i2c_read(TRUE);
    yr = i2c_read(FALSE);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_RAM_ADDR);
    i2c2_restart();
    err = i2c2_write(PCF8583_R_ADDR);
    cn = i2c2_read(TRUE);
    yr = i2c2_read(FALSE);
    i2c2_stop();
#endif
}

void pcf8583_setyear(UINT8 cn, UINT8 yr) {
    BOOL err;
#if defined(PCF8583_USE_I2C)
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_RAM_ADDR);
    err = i2c_write(cn);
    err = i2c_write(yr);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_RAM_ADDR);
    err = i2c2_write(cn);
    err = i2c2_write(yr);
    i2c2_stop();
#endif
}

void pcf8583_set_datetime(UINT8 hours, UINT8 minutes, UINT8 seconds, UINT8 dayofweek, UINT8 day, UINT8 month, UINT8 leapyear, UINT8 century, UINT8 year) {
    UINT8 LyDd, WdMo;
    INT16 err;
    LyDd = (UINT8) ((dectobcd(leapyear) << 6) | dectobcd(day));
    WdMo = (UINT8) ((dectobcd(dayofweek) << 5) | dectobcd(month));
#if defined(PCF8583_USE_I2C)
    // Stop the RTC from counting, before writing to the time and date registers
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_CTRL_STATUS_REG);
    err = i2c_write(PCF8583_STOP_COUNTING);
    i2c_stop();

    // Write to the date and time registers
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_100S_REG);
    err = i2c_write(dectobcd(0));
    err = i2c_write(dectobcd(seconds));
    err = i2c_write(dectobcd(minutes));
    err = i2c_write(dectobcd(hours));
    err = i2c_write(LyDd);
    err = i2c_write(WdMo);
    i2c_stop();

    // allow the PCF8583 to start counting again
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_CTRL_STATUS_REG);
    err = i2c_write(PCF8583_START_COUNTING);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    // Stop the RTC from counting, before writing to the time and date registers
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_CTRL_STATUS_REG);
    err = i2c2_write(PCF8583_STOP_COUNTING);
    i2c2_stop();

    // Write to the date and time registers
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_100S_REG);
    err = i2c2_write(dectobcd(0));
    err = i2c2_write(dectobcd(seconds));
    err = i2c2_write(dectobcd(minutes));
    err = i2c2_write(dectobcd(hours));
    err = i2c2_write(LyDd);
    err = i2c2_write(WdMo);
    i2c2_stop();

    // allow the PCF8583 to start counting again
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_CTRL_STATUS_REG);
    err = i2c2_write(PCF8583_START_COUNTING);
    i2c2_stop();
#endif
    pcf8583_setyear(century, year);
}

void pcf8583_get_datetime(UINT8 * hr, UINT8 * mn, UINT8 * sc, UINT8 * dow, UINT8 * dy, UINT8 * mt, UINT8 * lp, UINT8 * ct, UINT8 * yr) {
    UINT8 LyDd, WdMo, tmp;
    INT8 err;
#if defined(PCF8583_USE_I2C)
    // read the date and time registers
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_SECONDS_REG);
    i2c_restart();
    err = i2c_write(PCF8583_R_ADDR);
    sc = bcdtodec(i2c_read(TRUE));
    mn = bcdtodec(i2c_read(TRUE));
    hr = bcdtodec((i2c_read(TRUE) & 0b00111111));
    LyDd = i2c_read(TRUE);
    WdMo = i2c_read(FALSE);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    // read the date and time registers
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_SECONDS_REG);
    i2c2_restart();
    err = i2c2_write(PCF8583_R_ADDR);
    sc = bcdtodec(i2c2_read(TRUE));
    mn = bcdtodec(i2c2_read(TRUE));
    hr = bcdtodec((i2c2_read(TRUE) & 0b00111111));
    LyDd = i2c2_read(TRUE);
    WdMo = i2c2_read(FALSE);
    i2c2_stop();
#endif
    tmp = ((WdMo & 0b11100000) >> 5);
    dow = bcdtodec(tmp);

    tmp = (WdMo & 0b00011111);
    mt = bcdtodec(tmp);

    tmp = ((LyDd & 0b11000000) >> 6);
    lp = bcdtodec(tmp);

    tmp = (LyDd & 0b00111111);
    dy = bcdtodec(tmp);

    pcf8583_getyear(&ct, &yr);

    // check if RTC incremented the leapyear so we update our year in NVRAM
    if (lp != (yr % 4)) {
        yr = yr + 1;
        pcf8583_setyear(ct, yr);
    }
}

UINT8 pcf8583_read_reg(UINT8 reg) {
    INT8 tmp, err;
#if defined(PCF8583_USE_I2C)
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(reg);
    i2c_restart();
    err = i2c_write(PCF8583_R_ADDR);
    tmp = i2c_read(false);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(reg);
    i2c2_restart();
    err = i2c2_write(PCF8583_R_ADDR);
    tmp = i2c2_read(false);
    i2c2_stop();
#endif
    return (tmp);
}

// Set the alarm type:
// [atype] parameter have following values:
//    0 = no alarm
//    1 = daily alarm
//    2 = weekdays alarm
// other types are not implemented yet.

void pcf8583_en_dis_alarm(UINT8 atype) {
    UINT8_BITS cfg, alarmcfg;
    INT8 err;
    cfg.Val = pcf8583_read_reg(PCF8583_CTRL_STATUS_REG);
    if (atype > 2) atype = 1;
    if (atype == 0) cfg.bits.b2 = 0;
    else {
        cfg.bits.b0 = 0;
        cfg.bits.b1 = 0;
        cfg.bits.b2 = 1;
        alarmcfg.Val = 0;
        alarmcfg.bits.b4 = (UINT8) (atype);
        alarmcfg.bits.b7 = 1;
    }
#if defined(PCF8583_USE_I2C)
    // writing into CONTROL STATUS REG
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_CTRL_STATUS_REG);
    err = i2c_write(cfg.Val);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    // writing into CONTROL STATUS REG
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_CTRL_STATUS_REG);
    err = i2c2_write(cfg.Val);
    i2c2_stop();
#endif
    if (atype != 0) {
#if defined(PCF8583_USE_I2C)
        // writing into ALARM CONTROL REG
        i2c_start();
        err = i2c_write(PCF8583_W_ADDR);
        err = i2c_write(PCF8583_ALARM_CONTROL_REG);
        err = i2c_write(alarmcfg.Val);
        i2c_stop();
#elif defined(PCF8583_USE_I2C2)
        // writing into ALARM CONTROL REG
        i2c2_start();
        err = i2c2_write(PCF8583_W_ADDR);
        err = i2c2_write(PCF8583_ALARM_CONTROL_REG);
        err = i2c2_write(alarmcfg.Val);
        i2c2_stop();
#endif
    }
}

void pcf8583_set_alarm_weekdays(BOOL d0, BOOL d1, BOOL d2, BOOL d3, BOOL d4, BOOL d5, BOOL d6) {
    UINT8_BITS wd;
    INT8 err;
    wd.bits.b0 = d0;
    wd.bits.b1 = d1;
    wd.bits.b2 = d2;
    wd.bits.b3 = d3;
    wd.bits.b4 = d4;
    wd.bits.b5 = d5;
    wd.bits.b6 = d6;
#if defined(PCF8583_USE_I2C)
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_ALARM_MONTHS_REG);
    err = i2c_write(wd.Val);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_ALARM_MONTHS_REG);
    err = i2c2_write(wd.Val);
    i2c2_stop();
#endif
}

void pcf8583_set_alarm_time(UINT8 hours, UINT8 minutes, UINT8 seconds) {
    // you need a pull-up 10K resistor on INT pin of PCF8583
    INT8 err;
#if defined(PCF8583_USE_I2C)
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_ALARM_100S_REG);
    err = i2c_write(dectobcd(0));
    err = i2c_write(dectobcd(seconds));
    err = i2c_write(dectobcd(minutes));
    err = i2c_write(dectobcd(hours));
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_ALARM_100S_REG);
    err = i2c2_write(dectobcd(0));
    err = i2c2_write(dectobcd(seconds));
    err = i2c2_write(dectobcd(minutes));
    err = i2c2_write(dectobcd(hours));
    i2c2_stop();
#endif
}

void pcf8583_stop_alarm(void) { // It only stops the "sound of alarm", it doesn't disable it!
    UINT8_BITS cfg;
    INT8 err;
    cfg.Val = pcf8583_read_reg(PCF8583_CTRL_STATUS_REG);
    cfg.bits.b0 = FALSE; // clears timer alarm flag
    cfg.bits.b1 = FALSE; // clears clock alarm flag
#if defined(PCF8583_USE_I2C)
    //  writing into ALARM CONTROL REG
    i2c_start();
    err = i2c_write(PCF8583_W_ADDR);
    err = i2c_write(PCF8583_CTRL_STATUS_REG);
    err = i2c_write(cfg.Val);
    i2c_stop();
#elif defined(PCF8583_USE_I2C2)
    //  writing into ALARM CONTROL REG
    i2c2_start();
    err = i2c2_write(PCF8583_W_ADDR);
    err = i2c2_write(PCF8583_CTRL_STATUS_REG);
    err = i2c2_write(cfg.Val);
    i2c2_stop();
#endif
}

#endif	/* ROSSO_PCF8583_H */


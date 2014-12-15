/* 
 * File:   rosso_ow.h
 * Author: Worker
 *
 * Created on December 13, 2014, 8:32 PM
 */

/*
 * Requires the following definitions before including the header
 * in the main program:
 *
 * #define d1w_bus_rd        PORTCbits.RC0
 * #define d1w_bus_wr        LATCbits.LATC0
 * #define d1w_bus_direction TRISCbits.RC0
 * #include <rosso_ow.h>
 *
 */

#ifndef ROSSO_OW_H
#define	ROSSO_OW_H

#include <rosso_delays.h>

#if !defined(d1w_bus_rd)
#define d1w_bus_rd        PORTCbits.RC0
#endif
#if !defined(d1w_bus_wr)
#define d1w_bus_wr        LATCbits.LATC0
#endif
#if !defined(d1w_bus_direction)
#define d1w_bus_direction TRISCbits.RC0
#endif

// Recovery time (T_Rec) minimum 1usec - increase for long lines
// 5 usecs is a value given in some Maxim AppNotes
// 30u secs seem to be reliable for longer lines
// OW_RECOVERY_TIME      =  5  -- usec
// OW_RECOVERY_TIME      = 30  -- usec
#define  OW_RECOVERY_TIME       10  // usec
#define  OW_CONF_DELAYOFFSET     0
//
#define  OW_MATCH_ROM    0x55
#define  OW_SKIP_ROM     0xCC
#define  OW_SEARCH_ROM   0xF0
//
#define  OW_SEARCH_FIRST 0xFF  // start new search
#define  OW_PRESENCE_ERR 0xFF
#define  OW_DATA_ERR     0xFE
#define  OW_DATA_OK      0x10
#define  OW_LAST_DEVICE  0x00  // last device found
// rom-code size including CRC
#define  OW_ROMCODE_SIZE 8

#define  OW_CRC8INIT     0x00
#define  OW_CRC8POLY     0x18  // 0x18 = X xor 8 + X xor 5 + X xor 4 + X xor 0

UINT8 ow_input_pin_state(void) {
    return d1w_bus_rd;
}

void ow_parasite_enable(void) {
    d1w_bus_wr = 1;
    d1w_bus_direction = 0;
}

void ow_parasite_disable(void) {
    d1w_bus_direction = 1;
    d1w_bus_wr = 0;
}

UINT8 ow_reset(void) {
    UINT8 err;
    d1w_bus_wr = 0;
    d1w_bus_direction = 0;
    __delay_us(480);
    d1w_bus_direction = 1;
    __delay_us(64);
    err = d1w_bus_rd; // if err not 0, then nobody pulled to low; no presence then!
    // after a delay, the clients should release the line
    // and input-pin gets back to high because of pull-up-resistor
    __delay_us(480 - 64);
    if (d1w_bus_rd == 0) err = 1; // short circuit
    return err;
}

UINT8 ow_bit_io_intern(UINT8 b, UINT8 with_parasite_enable) {
    d1w_bus_direction = 0;
    __delay_us(2);
    if (b) d1w_bus_direction = 1;
    //"Output data from the DS18B20 is valid for 15usec after the falling
    // edge that initiated the read time slot. Therefore, the master must
    // release the bus and then sample the bus state within 15usec from
    // the start of the slot."
    __delay_us(15 - 2 - OW_CONF_DELAYOFFSET);
    if (d1w_bus_rd == 0) b = 0;
    // sample at end of read - timeslot
    __delay_us(60 - 15 - 2 + OW_CONF_DELAYOFFSET);
    d1w_bus_direction = 1;
    if (with_parasite_enable) ow_parasite_enable();
    __delay_us(OW_RECOVERY_TIME);
    return (b);
}

UINT8 ow_bit_io(UINT8 b) {
    return ow_bit_io_intern(b & 1, 0);
}

UINT8 ow_byte_wr(UINT8 b) {
    UINT8 i = 8, j;
    do {
        j = ow_bit_io(b & 1);
        b >>= 1;
        if (j) b |= 0x80;
    } while (--i);
    return b;
}

UINT8 ow_byte_wr_with_parasite_enable(UINT8 b) {
    UINT8 i = 8, j;
    do {
        if (i != 1)
            j = ow_bit_io_intern(b & 1, 0);
        else
            j = ow_bit_io_intern(b & 1, 1);
        b >>= 1;
        if (j) b |= 0x80;
    } while (--i);
    return b;
}

UINT8 ow_byte_rd(void) {
    // read by sending only "1"s, so bus gets released
    // after the init low - pulse in every slot
    return ow_byte_wr(0xFF);
}

UINT8 ow_byte_rd_with_parasite_enable(void) {
    //read by sending only "1"s, so bus gets released
    //after the init low - pulse in every slot
    return ow_byte_wr_with_parasite_enable(0xFF);
}

UINT8 ow_rom_search(UINT8 diff, UINT8 *id) {
    UINT8 i, j, next_diff;
    UINT8 b;
    if (ow_reset()) {
        return OW_PRESENCE_ERR; // error, no device found <--- early exit!
    }
    ow_byte_wr(OW_SEARCH_ROM); // ROM search command
    next_diff = OW_LAST_DEVICE; // unchanged on last device
    i = OW_ROMCODE_SIZE * 8; // 8 bytes
    do {
        j = 8; // 8 bits
        do {
            b = ow_bit_io(1); // read bit
            if (ow_bit_io(1)) { // read complement bit
                if (b) { // 0b11
                    return OW_DATA_ERR; // data error <--- early exit!
                }
            } else {
                if (!b) { // 0b00 = 2 devices
                    if (diff > i || ((*id & 1) && diff != i)) {
                        b = 1; // now 1
                        next_diff = i; // next pass 0
                    }
                }
            }
            ow_bit_io(b); // write bit
            *id >>= 1;
            if (b) {
                *id |= 0x80; // store bit
            }
            i--;
        } while (--j);
        id++; // next byte
    } while (i);
    return next_diff; // to continue search
}

void ow_command_intern(UINT8 command, UINT8 *id, UINT8 with_parasite_enable) {
    UINT8 i;
    ow_reset();
    if (id) {
        ow_byte_wr(OW_MATCH_ROM); // to a single device
        i = OW_ROMCODE_SIZE;
        do {
            ow_byte_wr(*id);
            id++;
        } while (--i);
    } else {
        ow_byte_wr(OW_SKIP_ROM); // to all devices
    }
    if (with_parasite_enable) {
        ow_byte_wr_with_parasite_enable(command);
    } else {
        ow_byte_wr(command);
    }
}

void ow_command(UINT8 command, UINT8 *id) {
    ow_command_intern(command, id, 0);
}

void ow_command_with_parasite_enable(UINT8 command, UINT8 *id) {
    ow_command_intern(command, id, 1);
}




#endif	/* ROSSO_OW_H */


/* 
 * File:   rosso_conversion.h
 * Author: Vasile Guta Ciucur
 *
 * Created on December 13, 2014, 7:55 PM
 */

#ifndef ROSSO_CONVERSION_H
#define	ROSSO_CONVERSION_H

uint8_t dectobcd(uint8_t pValue) {
    return (((pValue / 10) << 4) | (pValue % 10));
}

uint8_t bcdtodec(uint8_t pValue) {
    return ((pValue >> 4) * 10 + (pValue & 0x0F));
}

uint8_t nibble2hex(uint8_t val) {
    uint8_t s;
    s = '0' + (val & 0xf);
    if (s > '9')
        s += 'A' - '9' - 1;
    return s;
}

void byte2dec(uint8_t val, uint8_t *s) {
    if (val > 99) {
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + (val % 10);
        val /= 10;
        s[0] = '0' + val;
        s[3] = 0;
    } else if (val > 9) {
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + val;
        s[0] = ' ';
        s[3] = 0;
    } else {
        s[2] = '0' + val;
        s[1] = ' ';
        s[0] = ' ';
        s[3] = 0;
    }
}

void word2dec(uint16_t val, uint8_t *s) {
    if (val > 9999) {
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + (val % 10);
        val /= 10;
        s[0] = '0' + val;
        s[5] = 0;
    } else if (val > 999) {
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + val;
        s[0] = ' ';
        s[5] = 0;
    } else if (val > 99) {
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + val;
        s[1] = ' ';
        s[0] = ' ';
        s[5] = 0;
    } else if (val > 9) {
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + val;
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[5] = 0;
    } else {
        s[4] = '0' + val;
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[5] = 0;
    }
}

void double2dec(uint32_t val, uint8_t *s) {
    if (val > 999999999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + (val % 10);
        val /= 10;
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + (val % 10);
        val /= 10;
        s[0] = '0' + val;
        s[10] = 0;
    } else if (val > 99999999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + (val % 10);
        val /= 10;
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + val;
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 9999999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + (val % 10);
        val /= 10;
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + (val % 10);
        val /= 10;
        s[2] = '0' + val;
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 999999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + (val % 10);
        val /= 10;
        s[4] = '0' + (val % 10);
        val /= 10;
        s[3] = '0' + val;
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 99999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + (val % 10);
        val /= 10;
        s[4] = '0' + val;
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 9999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + (val % 10);
        val /= 10;
        s[5] = '0' + val;
        s[4] = ' ';
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 999) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + (val % 10);
        val /= 10;
        s[6] = '0' + val;
        s[5] = ' ';
        s[4] = ' ';
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 99) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + (val % 10);
        val /= 10;
        s[7] = '0' + val;
        s[6] = ' ';
        s[5] = ' ';
        s[4] = ' ';
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else if (val > 9) {
        s[9] = '0' + (val % 10);
        val /= 10;
        s[8] = '0' + val;
        s[7] = ' ';
        s[6] = ' ';
        s[5] = ' ';
        s[4] = ' ';
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    } else {
        s[9] = '0' + val;
        s[8] = ' ';
        s[7] = ' ';
        s[6] = ' ';
        s[5] = ' ';
        s[4] = ' ';
        s[3] = ' ';
        s[2] = ' ';
        s[1] = ' ';
        s[0] = ' ';
        s[10] = 0;
    }
}

void double2hex(uint32_t val, uint8_t *s) {
    s[0] = nibble2hex(val >> 28);
    s[1] = nibble2hex(val >> 24);
    s[2] = nibble2hex(val >> 20);
    s[3] = nibble2hex(val >> 16);
    s[4] = nibble2hex(val >> 12);
    s[5] = nibble2hex(val >> 8);
    s[6] = nibble2hex(val >> 4);
    s[7] = nibble2hex(val);
    s[8] = 0;
}

void word2hex(uint16_t val, uint8_t *s) {
    s[0] = nibble2hex(val >> 12);
    s[1] = nibble2hex(val >> 8);
    s[2] = nibble2hex(val >> 4);
    s[3] = nibble2hex(val);
    s[4] = 0;
}

void byte2hex(uint8_t val, uint8_t *s) {
    s[0] = nibble2hex(val >> 4);
    s[1] = nibble2hex(val);
    s[2] = 0;
}


#endif	/* ROSSO_CONVERSION_H */


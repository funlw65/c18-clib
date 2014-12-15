/* 
 * File:   rosso_conversion.h
 * Author: Worker
 *
 * Created on December 13, 2014, 7:55 PM
 */

#ifndef ROSSO_CONVERSION_H
#define	ROSSO_CONVERSION_H

UINT8 dectobcd(UINT8 pValue) {
    return (((pValue / 10) << 4) | (pValue % 10));
}

UINT8 bcdtodec(UINT8 pValue) {
    return ((pValue >> 4) * 10 + (pValue & 0x0F));
}

UINT8 nibble2hex(UINT8 val) {
    UINT8 s;
    s = '0' + (val & 0xf);
    if (s > '9')
        s += 'A' - '9' - 1;
    return s;
}

void byte2dec(UINT8 val, UINT8 *s) {
    if (val > 99) {
        s[2] = '0' + (val % 10);
        val /= 10;
        s[1] = '0' + (val % 10);
        val /= 10;
        s[0] = '0' + val;
        //
        s[3] = 0;
    } else if (val > 9) {
        s[1] = '0' + (val % 10);
        val /= 10;
        s[0] = '0' + val;
        s[2] = 0;
        //
        //s[3] = 0;
    } else {
        s[0] = '0' + val;
        //s[2] = 0;
        s[1] = 0;
        //
        ///s[3] = 0;
    }
}

void double2hex(UINT32 val, UINT8 *s) {
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

void word2hex(UINT16 val, UINT8 *s) {
    s[0] = nibble2hex(val >> 12);
    s[1] = nibble2hex(val >> 8);
    s[2] = nibble2hex(val >> 4);
    s[3] = nibble2hex(val);
    s[4] = 0;
}

void byte2hex(UINT8 val, UINT8 *s) {
    s[0] = nibble2hex(val >> 4);
    s[1] = nibble2hex(val);
    s[2] = 0;
}


#endif	/* ROSSO_CONVERSION_H */


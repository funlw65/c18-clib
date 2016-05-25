/* 
 * File:   rosso_glcd.h
 * Author: Fabian Maximilian Thiele
 * Adapted by: Vasile Guta Ciucur
 *
 * Created on January 5, 2015, 5:51 PM
 */

/*
 * It depends on rosso_delays.h header which must be included
 * in the main.c before this one.
 *
 */

#ifndef ROSSO_GLCD_H
#define	ROSSO_GLCD_H

#ifndef GLCD_DATA_IN
#define GLCD_DATA_IN    PORTB		// Data Input Register
#endif
#ifndef GLCD_DATA_OUT
#define GLCD_DATA_OUT   LATB		// Data Output Register
#endif
#ifndef GLCD_DATA_DIR
#define GLCD_DATA_DIR   TRISB		// Data Direction Register for Data Port
#endif

// Command Port Bits
#ifndef GLCD_DI
#define GLCD_DI         LATEbits.LATE0		// D/I Pin
#endif
#ifndef GLCD_RW
#define GLCD_RW         LATEbits.LATE1		// R/W Pin
#endif
#ifndef GLCD_EN
#define GLCD_EN         LATEbits.LATE2		// EN  Pin
#endif
#ifndef GLCD_CS1
#define GLCD_CS1        LATCbits.LATC0		// CS1 Pin
#endif
#ifndef GLCD_CS2
#define GLCD_CS2        LATCbits.LATC1		// CS2 Pin
#endif
#ifndef GLCD_RS
#define GLCD_RS         LATDbits.LATD2		// RS  Pin
#endif

// READ Command Port Bits
#ifndef GLCD_DI_IN
#define GLCD_DI_IN         PORTEbits.RE0		// D/I Pin
#endif
#ifndef GLCD_RW_IN
#define GLCD_RW_IN         PORTEbits.RE1		// R/W Pin
#endif
#ifndef GLCD_EN_IN
#define GLCD_EN_IN         PORTEbits.RE2		// EN  Pin
#endif
#ifndef GLCD_CS1_IN
#define GLCD_CS1_IN        PORTCbits.RC0		// CS1 Pin
#endif
#ifndef GLCD_CS2_IN
#define GLCD_CS2_IN        PORTCbits.RC1		// CS2 Pin
#endif
#ifndef GLCD_RS_IN
#define GLCD_RS_IN         PORTDbits.RD2		// RS  Pin
#endif

// Command Port Bits Direction
#ifndef GLCD_DI_DIR
#define GLCD_DI_DIR         TRISEbits.RE0		
#endif
#ifndef GLCD_RW_DIR
#define GLCD_RW_DIR         TRISEbits.RE1		
#endif
#ifndef GLCD_EN_DIR
#define GLCD_EN_DIR         TRISEbits.RE2		
#endif
#ifndef GLCD_CS1_DIR
#define GLCD_CS1_DIR        TRISCbits.RC0		
#endif
#ifndef GLCD_CS2_DIR
#define GLCD_CS2_DIR        TRISCbits.RC1		
#endif
#ifndef GLCD_RS_DIR
#define GLCD_RS_DIR         TRISDbits.RD2		
#endif

#define GLCD_CHIP1            0x00
#define GLCD_CHIP2            0x01

// Commands
#define GLCD_ON               0x3F
#define GLCD_OFF              0x3E
#define GLCD_SET_ADD          0x40
#define GLCD_SET_PAGE         0xB8
#define GLCD_DISP_START       0xC0

// Colors
#define GLCD_BLACK            0xFF
#define GLCD_WHITE            0x00

// Font Indices
#define GLCD_FONT_LENGTH         0
#define GLCD_FONT_FIXED_WIDTH    2
#define GLCD_FONT_HEIGHT         3
#define GLCD_FONT_FIRST_CHAR     4
#define GLCD_FONT_CHAR_COUNT     5
#define GLCD_FONT_WIDTH_TABLE    6

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t page;
} glcdCoord;
glcdCoord ks0108Coord;

uint8_t ks0108Inverted = 0;
uint8_t ks0108FontColor;
#ifdef __18CXX
const rom UINT8* ks0108Font;
#else
const uint8_t* ks0108Font;
#endif

inline void GLCD_Enable(void) {
    GLCD_EN = 1; // EN high level width: min. 450ns
    delay_500ns();
    GLCD_EN = 0;
    delay_1us();
}

void GLCD_WriteCommand(uint8_t cmd, uint8_t chip) {
    if (chip == GLCD_CHIP1) {
        GLCD_CS2 = 0; // deselect chip 2
        GLCD_CS1 = 1; // select chip 1
    } else if (chip == GLCD_CHIP2) {
        GLCD_CS1 = 0; // deselect chip 1
        GLCD_CS2 = 1; // select chip 2
    }
    GLCD_DI = 0;
    GLCD_RW = 0;
    GLCD_DATA_DIR = 0x00; // data port is output
    GLCD_DATA_OUT = cmd; // write command
    GLCD_Enable(); // enable
    GLCD_DATA_OUT = 0x00;
}

void GLCD_GotoXY(uint8_t x, uint8_t y) {
    uint8_t chip = GLCD_CHIP1, cmd;
    if (x > 127)
        x = 0; // ensure that coordinates are legal
    if (y > 63)
        y = 0;
    ks0108Coord.x = x; // save new coordinates
    ks0108Coord.y = y;
    ks0108Coord.page = y / 8;
    if (x >= 64) { // select the right chip
        x -= 64;
        chip = GLCD_CHIP2;
    }
    cmd = GLCD_SET_ADD | x;
    GLCD_WriteCommand(cmd, chip); // set x address on active chip
    cmd = GLCD_SET_PAGE | ks0108Coord.page; // set y address on both chips
    GLCD_WriteCommand(cmd, GLCD_CHIP1);
    GLCD_WriteCommand(cmd, GLCD_CHIP2);
}

uint8_t GLCD_DoReadData(uint8_t first) {
    uint8_t data;
    volatile uint8_t i;
    GLCD_DATA_OUT = 0x00;
    GLCD_DATA_DIR = 0xFF; // data port is input
    if (ks0108Coord.x < 64) {
        GLCD_CS2 = 0; // deselect chip 2
        GLCD_CS1 = 1; // select chip 1
    } else if (ks0108Coord.x >= 64) {
        GLCD_CS1 = 0; // deselect chip 1
        GLCD_CS2 = 1; // select chip 2
    }
    if (ks0108Coord.x == 64 && first) { // chip2 X-address = 0
        GLCD_WriteCommand(GLCD_SET_ADD, GLCD_CHIP2); // wuff wuff
    }
    GLCD_DI = 1; // D/I = 1
    GLCD_RW = 1; // R/W = 1
    GLCD_EN = 1; // EN high level width: min. 450ns
    delay_500ns();
    data = GLCD_DATA_IN; // read Data
    delay_1us();
    GLCD_DATA_DIR = 0x00; // data port is output
    GLCD_GotoXY(ks0108Coord.x, ks0108Coord.y);
    if (ks0108Inverted)
        data = ~data;
    return data;
}

inline uint8_t GLCD_ReadData(void) {
    GLCD_DoReadData(1); // dummy read
    return GLCD_DoReadData(0); // "real" read
}

UINT8 GLCD_SaveCmdPort() {
    UINT8_BITS tmp;
    //
    tmp.Val = 0;
    tmp.bits.b0 = GLCD_DI_IN;
    tmp.bits.b1 = GLCD_RW_IN;
    tmp.bits.b2 = GLCD_EN_IN;
    tmp.bits.b3 = GLCD_CS1_IN;
    tmp.bits.b4 = GLCD_CS2_IN;
    tmp.bits.b5 = GLCD_RS_IN;
    return tmp.Val;
}

void GLCD_RestoreCmdPort(uint8_bits_t data) {
    //
    GLCD_DI = data.bits.b0;
    GLCD_RW = data.bits.b1;
    GLCD_EN = data.bits.b2;
    GLCD_CS1 = data.bits.b3;
    GLCD_CS2 = data.bits.b4;
    GLCD_RS = data.bits.b5;
}

void GLCD_WriteData(uint8_t data) {
    uint8_t displayData, yOffset;
    uint8_bits_t cmdPort;
    if (ks0108Coord.x >= 128)
        return;
    if (ks0108Coord.x < 64) {
        GLCD_CS2 = 0; // deselect chip 2
        GLCD_CS1 = 1; // select chip 1
    } else if (ks0108Coord.x >= 64) {
        GLCD_CS1 = 0; // deselect chip 1
        GLCD_CS2 = 1; // select chip 2
    }
    if (ks0108Coord.x == 64) // chip2 X-address = 0
        GLCD_WriteCommand(GLCD_SET_ADD, GLCD_CHIP2);
    GLCD_DI = 1; // D/I = 1
    GLCD_RW = 0; // R/W = 0
    GLCD_DATA_DIR = 0x00; // data port is output
    yOffset = ks0108Coord.y % 8;
    if (yOffset != 0) {
        // first page
        cmdPort.Val = GLCD_SaveCmdPort(); // save command port
        displayData = GLCD_ReadData();
        GLCD_RestoreCmdPort(cmdPort); // restore command port
        GLCD_DATA_DIR = 0x00; // data port is output
        displayData |= data << yOffset;
        if (ks0108Inverted)
            displayData = ~displayData;
        GLCD_DATA_OUT = displayData; // write data
        GLCD_Enable(); // enable
        // second page
        GLCD_GotoXY(ks0108Coord.x, ks0108Coord.y + 8);
        displayData = GLCD_ReadData();
        GLCD_RestoreCmdPort(cmdPort); // restore command port
        GLCD_DATA_DIR = 0x00; // data port is output
        displayData |= data >> (8 - yOffset);
        if (ks0108Inverted)
            displayData = ~displayData;
        GLCD_DATA_OUT = displayData; // write data
        GLCD_Enable(); // enable
        GLCD_GotoXY(ks0108Coord.x + 1, ks0108Coord.y - 8);
    } else {
        if (ks0108Inverted)
            data = ~data;
        GLCD_DATA_OUT = data; // write data
        GLCD_Enable(); // enable
        ks0108Coord.x++;
    }
    GLCD_DATA_OUT = 0x00;
}

void GLCD_FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        uint8_t color) {
    uint8_t mask, pageOffset, h, i, data;
    //
    height++;
    pageOffset = y % 8;
    y -= pageOffset;
    mask = 0xFF;
    if (height < 8 - pageOffset) {
        mask >>= (8 - height);
        h = height;
    } else {
        h = 8 - pageOffset;
    }
    mask <<= pageOffset;
    GLCD_GotoXY(x, y);
    for (i = 0; i <= width; i++) {
        data = GLCD_ReadData();
        if (color == GLCD_BLACK) {
            data |= mask;
        } else {
            data &= ~mask;
        }
        GLCD_WriteData(data);
    }
    while (h + 8 <= height) {
        h += 8;
        y += 8;
        GLCD_GotoXY(x, y);
        for (i = 0; i <= width; i++) {
            GLCD_WriteData(color);
        }
    }
    if (h < height) {
        mask = ~(0xFF << (height - h));
        GLCD_GotoXY(x, y + 8);
        for (i = 0; i <= width; i++) {
            data = GLCD_ReadData();
            if (color == GLCD_BLACK) {
                data |= mask;
            } else {
                data &= ~mask;
            }
            GLCD_WriteData(data);
        }
    }
}

#define GLCD_DrawVLine(x, y, length, color) {GLCD_FillRect(x, y, 0, length, color);}
#define GLCD_DrawHLine(x, y, length, color) {GLCD_FillRect(x, y, length, 0, color);}
#define GLCD_ClearScreen() {GLCD_FillRect(0, 0, 127, 63, GLCD_WHITE);}

void GLCD_SetDot(uint8_t x, uint8_t y, uint8_t color) {
    uint8_t data;
    GLCD_GotoXY(x, y - y % 8); // read data from display memory
    data = GLCD_ReadData();
    if (color == GLCD_BLACK) {
        data |= 0x01 << (y % 8); // set dot
    } else {
        data &= ~(0x01 << (y % 8)); // clear dot
    }
    GLCD_WriteData(data); // write data back to display
}

void GLCD_DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        uint8_t radius, uint8_t color) {
    int16_t tSwitch, x1 = 0, y1 = radius;
    tSwitch = 3 - 2 * radius;
    while (x1 <= y1) {
        GLCD_SetDot(x + radius - x1, y + radius - y1, color);
        GLCD_SetDot(x + radius - y1, y + radius - x1, color);

        GLCD_SetDot(x + width - radius + x1, y + radius - y1, color);
        GLCD_SetDot(x + width - radius + y1, y + radius - x1, color);

        GLCD_SetDot(x + width - radius + x1, y + height - radius + y1, color);
        GLCD_SetDot(x + width - radius + y1, y + height - radius + x1, color);

        GLCD_SetDot(x + radius - x1, y + height - radius + y1, color);
        GLCD_SetDot(x + radius - y1, y + height - radius + x1, color);

        if (tSwitch < 0) {
            tSwitch += (4 * x1 + 6);
        } else {
            tSwitch += (4 * (x1 - y1) + 10);
            y1--;
        }
        x1++;
    }
    GLCD_DrawHLine(x + radius, y, width - (2 * radius), color); // top
    GLCD_DrawHLine(x + radius, y + height, width - (2 * radius), color); // bottom
    GLCD_DrawVLine(x, y + radius, height - (2 * radius), color); // left
    GLCD_DrawVLine(x + width, y + radius, height - (2 * radius), color); // right
}

void GLCD_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
        uint8_t color) {
    GLCD_DrawHLine(x, y, width, color); // top
    GLCD_DrawHLine(x, y + height, width, color); // bottom
    GLCD_DrawVLine(x, y, height, color); // left
    GLCD_DrawVLine(x + width, y, height, color); // right
}

#define GLCD_DrawCircle(xCenter, yCenter, radius, color) {GLCD_DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius, 2*radius, radius, color);}

void GLCD_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Color) {
    uint8_t dy, dx;
    int8_t addx = 1, addy = 1;
    int8_t P, diff;
    uint8_t i = 0;
    diff = ((INT8) x2 - x1);
    if (diff < 0) diff *= -1;
    dx = diff;
    diff = ((INT8) y2 - y1);
    if (diff < 0) diff *= -1;
    dy = diff;
    if (x1 > x2)addx = -1;
    if (y1 > y2)addy = -1;
    if (dx >= dy) {
        dy *= 2;
        P = dy - dx;
        diff = P - dx;
        for (; i <= dx; ++i) {
            GLCD_SetDot(x1, y1, Color);
            if (P < 0) {
                P += dy;
                x1 += addx;
            } else {
                P += diff;
                x1 += addx;
                y1 += addy;
            }
        }
    } else {
        dx *= 2;
        P = dx - dy;
        diff = P - dy;
        for (; i <= dy; ++i) {
            GLCD_SetDot(x1, y1, Color);
            if (P < 0) {
                P += dx;
                y1 += addy;
            } else {
                P += diff;
                x1 += addx;
                y1 += addy;
            }
        }
    }
}

void GLCD_InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    uint8_t mask, pageOffset, h, i, data, tmpData;
    height++;
    pageOffset = y % 8;
    y -= pageOffset;
    mask = 0xFF;
    if (height < 8 - pageOffset) {
        mask >>= (8 - height);
        h = height;
    } else {
        h = 8 - pageOffset;
    }
    mask <<= pageOffset;
    GLCD_GotoXY(x, y);
    for (i = 0; i <= width; i++) {
        data = GLCD_ReadData();
        tmpData = ~data;
        data = (tmpData & mask) | (data & ~mask);
        GLCD_WriteData(data);
    }
    while (h + 8 <= height) {
        h += 8;
        y += 8;
        GLCD_GotoXY(x, y);
        for (i = 0; i <= width; i++) {
            data = GLCD_ReadData();
            GLCD_WriteData(~data);
        }
    }
    if (h < height) {
        mask = ~(0xFF << (height - h));
        GLCD_GotoXY(x, y + 8);
        for (i = 0; i <= width; i++) {
            data = GLCD_ReadData();
            tmpData = ~data;
            data = (tmpData & mask) | (data & ~mask);
            GLCD_WriteData(data);
        }
    }
}

void GLCD_SetInverted(uint8_t invert) {
    if (ks0108Inverted != invert) {
        GLCD_InvertRect(0, 0, 127, 63);
        ks0108Inverted = invert;
    }
}

void GLCD_Init(uint8_t invert) {
    // command port is output
    GLCD_DI_DIR = 0;
    GLCD_RW_DIR = 0;
    GLCD_EN_DIR = 0;
    GLCD_CS1_DIR = 0;
    GLCD_CS2_DIR = 0;
    GLCD_RS_DIR = 0;
    // data port is output
    GLCD_DATA_DIR = 0;
    //
    ks0108Coord.x = 0;
    ks0108Coord.y = 0;
    ks0108Coord.page = 0;
    ks0108Inverted = invert;
    GLCD_WriteCommand(GLCD_ON, GLCD_CHIP1); // power on
    GLCD_WriteCommand(GLCD_ON, GLCD_CHIP2);
    GLCD_WriteCommand(GLCD_DISP_START, GLCD_CHIP1); // display start line = 0
    GLCD_WriteCommand(GLCD_DISP_START, GLCD_CHIP2);
    GLCD_ClearScreen(); // display clear
    GLCD_GotoXY(0, 0);
}

//
// Font Functions
//

// On hold until I get a graphic LCD - not very soon :( , sorry!


#endif	/* ROSSO_GLCD_H */


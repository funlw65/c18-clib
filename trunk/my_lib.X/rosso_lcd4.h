/* 
 * File:   rosso_lcd4.h
 * Author: Rob Hamerling
 * Adapted by: Vasile Guta-Ciucur
 *
 * Created on December 16, 2014, 3:32 PM
 */

#ifndef ROSSO_LCD4_H
#define	ROSSO_LCD4_H

// check LCD RS pin
#ifndef LCD_RS
#define LCD_RS LATEbits.LATE1
#define LCD_RS_DIR TRISEbits.RE1
#endif

// check LCD EN pin
#ifndef LCD_EN
#define LCD_EN LATEbits.LATE2
#define LCD_EN_DIR TRISEbits.RE2
#endif

// check the LCD 4 pins data port

// check LCD D4 pin
#ifndef LCD_D4
#define LCD_D4 LATBbits.LATB4
#define LCD_D4_DIR TRISBbits.RB4
#endif

// check LCD D5 pin
#ifndef LCD_D5
#define LCD_D5 LATBbits.LATB5
#define LCD_D5_DIR TRISBbits.RB5
#endif

// check LCD D6 pin
#ifndef LCD_D6
#define LCD_D6 LATBbits.LATB6
#define LCD_D6_DIR TRISBbits.RB6
#endif

// check LCD D7 pin
#ifndef LCD_D7
#define LCD_D7 LATBbits.LATB7
#define LCD_D7_DIR TRISBbits.RB7
#endif

#ifndef LCD_NR_CHARS
#define LCD_NR_CHARS 16
#elif ((LCD_NR_CHARS != 8) && (LCD_NR_CHARS != 16) && (LCD_NR_CHARS != 20))
#error "valid values for LCD Nr. Chars are 8, 16, 20"
#endif

#ifndef LCD_NR_LINES
#define LCD_NR_LINES 2
#elif ((LCD_NR_LINES != 1) && (LCD_NR_LINES != 2) && (LCD_NR_LINES != 4))
#error "valid values for LCD Nr. Lines are 1, 2, 4"
#endif

// Define some chipsets
#define LCD_HD44780 0 // standard
#define LCD_ST7066U 1 // A - E

#define LCD_CLEAR_DISPLAY        0b00000001
#define LCD_RETURN_HOME          0b00000010
#define LCD_DISPLAY_ONOFF        0b00001000
#define LCD_CURSOR_SHIFT_R_VAL   0b00010100
#define LCD_CURSOR_SHIFT_L_VAL   0b00010000
#define LCD_DISPLAY_SHIFT_RIGHT  0b00011100
#define LCD_DISPLAY_SHIFT_LEFT   0b00011000
#define LCD_SET_DDRAM_ADDRESS    0b10000000
#define LCD_SET_CGRAM_ADDRESS    0b01000000

// global variable for cursor position
UINT8 LCD_POS;

void __lcd_write_nibble(UINT8 value) {
    UINT8_BITS nibble;
    nibble.Val = value;
    LCD_D4 = nibble.bits.b0;
    LCD_D5 = nibble.bits.b1;
    LCD_D6 = nibble.bits.b2;
    LCD_D7 = nibble.bits.b3;
    LCD_EN = 1;
    delay_500ns(); // delay (> 400 ns)
    LCD_EN = 0;
}

void __lcd_write(UINT8 value) {
    __lcd_write_nibble(value >> 4); // write high nibble
    __lcd_write_nibble(value); // write low nibble
    delay_35us(); // > 37 us
    delay_3us();
}

void _lcd_write_data(UINT8 value) {
    LCD_RS = 1; // select data mode
    __lcd_write(value); // write byte
}

void _lcd_write_command(UINT8 value) {
    LCD_RS = 0; // select command mode
    __lcd_write(value); // write byte
}

UINT8 _lcd_line2index(UINT8 line) {
    //-- force valid line number
    if (line < LCD_NR_LINES) {
    if (line == 0) return 0x00;
    else if (line == 1) return 0x40;
    else if (line == 2) return 0x00 + LCD_NR_CHARS;
    else if (line == 3) return 0x40 + LCD_NR_CHARS;
    }
    return 0x00;
}

void _lcd_restore_cursor(void) {
    _lcd_write_command(LCD_SET_DDRAM_ADDRESS | LCD_POS);
}

void lcd_write_char(UINT8 data) {
    _lcd_write_data(data);
}

void lcd_cursor_position(UINT8 line, UINT8 pos) {
    LCD_POS = pos + _lcd_line2index(line);
    _lcd_restore_cursor();
}

void lcd_shift_left(UINT8 nr) {
    UINT8 i;
    if (nr > 0) {
        for (i = 0; i < nr; i++) _lcd_write_command(LCD_DISPLAY_SHIFT_LEFT);
    }
}

void lcd_shift_right(UINT8 nr) {
    UINT8 i;
    if (nr > 0) {
        for (i = 0; i < nr; i++) _lcd_write_command(LCD_DISPLAY_SHIFT_RIGHT);
    }
}

void lcd_cursor_shift_left(UINT8 nr) {
    UINT8 i;
    if (nr > 0) {
        for (i = 0; i < nr; i++) _lcd_write_command(LCD_CURSOR_SHIFT_L_VAL);
    }
}

void lcd_cursor_shift_right(UINT8 nr) {
    UINT8 i;
    if (nr > 0) {
        for (i = 0; i < nr; i++) _lcd_write_command(LCD_CURSOR_SHIFT_R_VAL);
    }
}

void lcd_clear_screen(void) {
    _lcd_write_command(LCD_CLEAR_DISPLAY);
    delay_1ms();
    delay_500us();
    delay_300us();
}

void lcd_cursor_blink_display(BIT cursor, BIT blink, BIT display) {
    UINT8 reg;
    reg = LCD_DISPLAY_ONOFF;
    if (display) reg = reg + 4;
    if (cursor) reg = reg + 2;
    if (blink) reg = reg + 1;
    _lcd_write_command(reg);
}

void lcd_home() {
    _lcd_write_command(LCD_RETURN_HOME);
    delay_1ms();
    delay_500us();
    delay_300us();
}

void lcd_clear_line(UINT8 line) {
    UINT8 i;
    // set LCD-cursor at start of line
    LCD_POS = _lcd_line2index(line);
    _lcd_restore_cursor();
    // now fill line with spaces
    for (i = 0; i < LCD_NR_CHARS; i++) lcd_write_char(' ');
    // set LCD back to normal operation
    _lcd_restore_cursor();
}

void lcd_progress(UINT8 line, UINT8 amount, UINT8 pattern) {
    UINT8 i;
    LCD_POS = _lcd_line2index(line);
    _lcd_restore_cursor();
    for (i = 0; i < amount; i++) lcd_write_char(pattern);
    for (i = 0; i < LCD_NR_CHARS - amount; i++) lcd_write_char(' ');
}

void lcd_init(UINT8 chipset) {
#ifndef LCD_RS_DIR
#error "you must define LCD_RS_DIR"
#endif
#ifndef LCD_EN_DIR
#error "you must define LCD_EN_DIR"
#endif
#ifndef LCD_D4_DIR
#error "you must define LCD_D4_DIR"
#endif
#ifndef LCD_D5_DIR
#error "you must define LCD_D5_DIR"
#endif
#ifndef LCD_D6_DIR
#error "you must define LCD_D6_DIR"
#endif
#ifndef LCD_D7_DIR
#error "you must define LCD_D7_DIR"
#endif
    LCD_RS_DIR = 0;
    LCD_EN_DIR = 0;
    LCD_D4_DIR = 0;
    LCD_D5_DIR = 0;
    LCD_D6_DIR = 0;
    LCD_D7_DIR = 0;
    //
    if (chipset > 1) chipset = 0;
    if (chipset == LCD_HD44780) {
        LCD_RS = 0; // set to control char mode
        delay_25ms(); // power-up delay (> 15 ms)
        __lcd_write_nibble(0b00000011); // function set
        delay_5ms(); // > 4.1 milliseconds
        __lcd_write_nibble(0b00000011); // function set
        delay_100us(); // > 100 us
        __lcd_write_nibble(0b00000011); // function set
        delay_35us(); // > 37 us
        delay_3us();
        __lcd_write_nibble(0b00000010); // select 4-bits mode
        delay_35us(); // > 37 us
        delay_3us();
        _lcd_write_command(0b00101000); // 2 lines, 5x8 dots font
        _lcd_write_command(0b00011100); // cursor (not data) move right
        _lcd_write_command(0b00001100); // display on, cursor off, no blink
        _lcd_write_command(0b00000110); // cursor shift right, no data shift
        // during read/write operations
        lcd_clear_screen(); // clear display
    } else {
        __lcd_write_nibble(0b00000011);
        delay_40us();
        __lcd_write_nibble(0b00000010);
        __lcd_write_nibble(0b00001100);
        delay_40us();
        __lcd_write_nibble(0b00000010);
        __lcd_write_nibble(0b00001100);
        delay_35us(); // > 37 us
        delay_3us();
        __lcd_write_nibble(0b00000000); // display on / off
        __lcd_write_nibble(0b00001100);
        delay_35us(); // > 37 us
        delay_3us();
        __lcd_write_nibble(0b00000000); // display clear
        __lcd_write_nibble(0b00000001);
        delay_1ms();
        delay_500us();
        delay_40us();
        __lcd_write_nibble(0b00000000); // entry mode set
        __lcd_write_nibble(0b00000110);
        delay_35us(); // > 37 us
        delay_3us();
        __lcd_write_nibble(0b00000000); // display clear
        __lcd_write_nibble(0b00000001);
        delay_1ms();
        delay_500us();
        delay_40us();
    }
}

#endif	/* ROSSO_LCD4_H */


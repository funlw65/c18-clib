/* 
 * File:   rosso_i2c2.h
 * Author: Worker
 *
 * Created on December 10, 2014, 9:46 AM
 */

#ifndef ROSSO_I2C2_MASTER_H
#define	ROSSO_I2C2_MASTER_H

// I2C Baud rate
#define	I2C2_SLOWEST  127 //-- (40 kHz @ 20 MHz Fosc)
#define	I2C2_100KHZ   (_XTAL_FREQ /   100000 / 4 - 1)
#define	I2C2_400KHZ   (_XTAL_FREQ /   400000 / 4 - 1)
#define	I2C2_1MHZ     (_XTAL_FREQ /  1000000 / 4 - 1)
#define I2C2_LEVEL    TRUE // this should be user set-able.. is ok for now...

#ifndef	I2C_SCL2_DIR
#error "define I2C_SCL2_DIR before including this header"
#endif
#ifndef	I2C_SDA2_DIR
#error "define I2C_SCL2_DIR before including this header"
#endif

void i2c2_init(UINT8 speed) {
    I2C_SCL2_DIR = 1;
    I2C_SDA2_DIR = 1;
    SSP2CON1 = 0b00101000;
    SSP2CON2 = 0b00100000;
    if (speed > I2C2_SLOWEST) SSP2ADD = I2C2_SLOWEST;
    else SSP2ADD = speed;
    #if defined(I2C2_LEVEL)
        SSP2STAT = 0b00000000;
    #else
        SSP2STAT = 0b01000000;
    #endif
}

void i2c2_start(void) {
    SSP2CON2bits.SEN = 1;
    while (SSP2CON2bits.SEN == 1);
}

void i2c2_restart() {
    SSP2CON2bits.RSEN = 1;
    while (SSP2CON2bits.RSEN == 1);
}

void i2c2_stop() {
    SSP2CON2bits.PEN = 1;
    while (SSP2CON2bits.PEN == 1);
}

BOOL i2c2_write(UINT8 data) {
    PIR3bits.SSP2IF = FALSE;
    SSP2BUF = data;
    while (!PIR3bits.SSP2IF);
    if (SSP2CON2bits.ACKSTAT == 0) return (TRUE);
    else {
        SSP2CON1bits.SSPEN = FALSE;
        SSP2CON1bits.SSPEN = TRUE;
        return (FALSE);
    }
}

UINT8 i2c2_read(BOOL myack) {
    SSP2CON2bits.RCEN = 1;
    while (SSP2STATbits.BF == 0);
    SSP2CON2bits.ACKDT = ~myack;
    SSP2CON2bits.ACKEN = 1;
    while (SSP2CON2bits.ACKEN == 1);
    return (SSP2BUF);
}

#endif	/* ROSSO_I2C2_MASTER_H */


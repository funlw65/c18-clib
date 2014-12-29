/* 
 * File:   rosso_i2c.h
 * Author: Worker
 *
 * Created on December 9, 2014, 11:36 PM
 */

#ifndef ROSSO_I2C_MASTER_H
#define	ROSSO_I2C_MASTER_H

#ifndef I2C_LEVEL
#define I2C_LEVEL    // this should be user set-able.. is ok for now...

typedef enum{
    I2C_1MHZ    = (_XTAL_FREQ /  1000000 / 4 - 1),
    I2C_400KHZ  = (_XTAL_FREQ /   400000 / 4 - 1),
    I2C_100KHZ  = (_XTAL_FREQ /   100000 / 4 - 1),
    I2C_SLOWEST = 127
} I2CSPEED;
#endif

#ifndef I2C_SCL_DIR
#error "define I2C_SCL_DIR before including this header"
#endif
#ifndef I2C_SDA_DIR
#error "define I2C_SDA_DIR before including this header"
#endif

void i2c_init(I2CSPEED speed) {
    I2C_SCL_DIR = 1;
    I2C_SDA_DIR = 1;
    SSPCON1 = 0b00101000;
    SSPCON2 = 0b00100000;
    SSPADD = speed;
    #if defined(I2C_LEVEL)
        SSPSTAT = 0b00000000;
    #else
        SSPSTAT = 0b01000000;
    #endif
}

void i2c_start(void) {
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN == 1);
}

void i2c_restart(void) {
    SSPCON2bits.RSEN = 1;
    while (SSPCON2bits.RSEN == 1);
}

void i2c_stop(void) {
    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN == 1);
}

BOOL i2c_write(UINT8 data) {
    PIR1bits.SSPIF = FALSE;
    SSPBUF = data;
    while (!PIR1bits.SSPIF);
    if (SSPCON2bits.ACKSTAT == 0)return (TRUE);
    else {
        SSPCON1bits.SSPEN = FALSE;
        SSPCON1bits.SSPEN = TRUE;
        return (FALSE);
    }
}

UINT8 i2c_read(BOOL myack) {
    SSPCON2bits.RCEN = 1;
    while (SSPSTATbits.BF == 0);
    SSPCON2bits.ACKDT = ~myack;
    SSPCON2bits.ACKEN = 1;
    while (SSPCON2bits.ACKEN == 1);
    return (SSPBUF);
}


#endif	/* ROSSO_I2C_MASTER_H */


/* 
 * File:   rosso_i2c2.h
 * Author: Vasile Guta Ciucur
 *
 * Created on December 10, 2014, 9:46 AM
 */

#ifndef ROSSO_I2C2_MASTER_H
#define	ROSSO_I2C2_MASTER_H

#define I2C_SCL2_DIR TRISDbits.RD0
#define I2C_SDA2_DIR TRISDbits.RD1

#ifndef I2C_LEVEL
#define I2C_LEVEL    // this should be user set-able.. is ok for now...

typedef enum{
    I2C_1MHZ    = (_XTAL_FREQ /  1000000 / 4 - 1),
    I2C_400KHZ  = (_XTAL_FREQ /   400000 / 4 - 1),
    I2C_100KHZ  = (_XTAL_FREQ /   100000 / 4 - 1),
    I2C_SLOWEST = 127
} I2CSPEED;
#endif

void i2c2_init(I2CSPEED speed) {
    I2C_SCL2_DIR = 1;
    I2C_SDA2_DIR = 1;
    SSP2CON1 = 0b00101000;
    SSP2CON2 = 0b00100000;
    if (speed > I2C_SLOWEST) SSP2ADD = I2C_SLOWEST;
    else SSP2ADD = speed;
    #if defined(I2C_LEVEL)
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

bool_t i2c2_write(uint8_t data) {
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

uint8_t i2c2_read(bool_t myack) {
    SSP2CON2bits.RCEN = 1;
    while (SSP2STATbits.BF == 0);
    SSP2CON2bits.ACKDT = ~myack;
    SSP2CON2bits.ACKEN = 1;
    while (SSP2CON2bits.ACKEN == 1);
    return (SSP2BUF);
}

#endif	/* ROSSO_I2C2_MASTER_H */


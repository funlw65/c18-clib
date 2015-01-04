/* 
 * File:   rosso_hw2serial.h
 * Author: Stef Mientki, Copyright (c) 2002..2006, all rights reserved.
 * Adapted by: Vasile Guta Ciucur
 * Created on December 12, 2014, 4:23 PM
 */

/*
 * It requires from you a definition of desired baudrate before including this header. 
 * By example:
 *
 * #define USART_BAUDRATE2 19200
 * #include <rosso_hw2serial.h>
 */

#ifndef ROSSO_HW2SERIAL_H
#define	ROSSO_HW2SERIAL_H

#ifndef USART_BAUDRATE2
#define USART_BAUDRATE2 19200
#endif

void USART_HW2_init(void) {
    UINT16 usart_div = ((_XTAL_FREQ / USART_BAUDRATE2) / 4) - 1;
    TXSTA2 = 0b00000000; // reset (8 databits, async)
    RCSTA2 = 0b00000000; // reset (8 databits, async)
    //
    BAUDCON2bits.BRG16 = TRUE;
    TXSTA2bits.BRGH = TRUE;
    SPBRG2 = (UINT8) usart_div;
    SPBRGH2 = (UINT8) (usart_div >> 8);
    //
    //_calculate_baudrate();  // transmit and receive speed
    PIE3bits.RC2IE = FALSE; // disable receive interrupts
    PIE3bits.TX2IE = FALSE; // disable transmit interrupts
    //USART_RX2_TRIS = 1; // make receive pin input
    //USART_TX2_TRIS = 0;
    TXSTA2bits.TXEN = TRUE; // Enable transmitter
    // (makes transmit pin output)
    RCSTA2bits.SPEN = 1; // activate serial port
    RCSTA2bits.CREN = 1; // continuous receive
}

void USART_HW2_disable(void) {
    while (!TXSTA2bits.TRMT); // wait while transmission pending
    RCSTA2bits.SPEN = FALSE; // disable serial port
}

#define USART_HW2_enable() RCSTA2bits.SPEN=TRUE; // enable serial port

void USART_HW2_write(UINT8 data) {
    while (!PIR3bits.TX2IF); // wait while transmission pending
    TXREG2 = data; // transfer data
}

BOOL USART_HW2_read(UINT8 *data) {
    if (PIR3bits.RC2IF) { // check if data available
        *data = RCREG2; // pass received byte to caller
        PIR3bits.RC2IF = FALSE; // eur@fiwhex.nl 12-sept-08
    } else return (FALSE); // no data available
    if (RCSTA2bits.OERR) { // reset USART after overrun
        RCSTA2bits.CREN = FALSE;
        RCSTA2bits.CREN = TRUE;
    }
    return (TRUE);
}

#define USART_HW2_datardy()  (PIR3bits.RC2IF)
#define USART_HW2_databusy() (!TXSTA2bits.TRMT)

#endif	/* ROSSO_HWSERIAL_H */


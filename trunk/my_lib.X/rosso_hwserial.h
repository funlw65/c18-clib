/* 
 * File:   rosso_hwserial.h
 * Author: Stef Mientki, Copyright (c) 2002..2006, all rights reserved.
 * Adapted by: Vasile Guta Ciucur
 * Created on December 12, 2014, 4:23 PM
 */

/*
 * It requires from you a definition of desired baudrate before including this header. 
 * By example:
 * #define USART_BAUDRATE 19200
 * #include <rosso_hwserial.h>
 */

#ifndef ROSSO_HWSERIAL_H
#define	ROSSO_HWSERIAL_H

#ifndef USART_BAUDRATE
#define USART_BAUDRATE 19200
#endif

void USART_HW_init(void) {
    UINT16 usart_div = ((_XTAL_FREQ / USART_BAUDRATE) / 4) - 1;
    TXSTA = 0b00000000; // reset (8 databits, async)
    RCSTA = 0b00000000; // reset (8 databits, async)
    //
    BAUDCONbits.BRG16 = TRUE;
    TXSTAbits.BRGH = TRUE;
    SPBRG = (UINT8) usart_div;
    SPBRGH = (UINT8) (usart_div >> 8);
    //
    //_calculate_baudrate();  // transmit and receive speed
    PIE1bits.RCIE = FALSE; // disable receive interrupts
    PIE1bits.TXIE = FALSE; // disable transmit interrupts
    //USART_RX_TRIS = 1; // make receive pin input
    //USART_TX_TRIS = 1; // make transmit pin input! (required by some older .. PICs like 16f628)
    TXSTAbits.TXEN = TRUE; // Enable transmitter
    // (makes transmit pin output)
    RCSTAbits.SPEN = 1; // activate serial port
    RCSTAbits.CREN = 1; // continuous receive
}

void USART_HW_disable(void) {
    while (!TXSTAbits.TRMT); // wait while transmission pending
    RCSTAbits.SPEN = FALSE; // disable serial port
}

#define USART_HW_enable() RCSTAbits.SPEN=TRUE; // enable serial port

void USART_HW_write(UINT8 data) {
    while (!PIR1bits.TXIF); // wait while transmission pending
    TXREG = data; // transfer data
}

BOOL USART_HW_read(UINT8 *data) {
    if (PIR1bits.RCIF) { // check if data available
        *data = RCREG; // pass received byte to caller
        PIR1bits.RCIF = FALSE; // eur@fiwhex.nl 12-sept-08
    } else return (FALSE); // no data available
    if (RCSTAbits.OERR) { // reset USART after overrun
        RCSTAbits.CREN = FALSE;
        RCSTAbits.CREN = TRUE;
    }
    return (TRUE);
}

#define USART_HW_datardy()  (PIR1bits.RCIF)
#define USART_HW_databusy() (!TXSTAbits.TRMT)

//#define USART_HW_isr() // to do!

#endif	/* ROSSO_HWSERIAL_H */


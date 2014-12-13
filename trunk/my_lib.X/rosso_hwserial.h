/* 
 * File:   rosso_hwserial.h
 * Author: Stef Mientki, Copyright (c) 2002..2006, all rights reserved.
 * Adapted by: Vasile Guta Ciucur
 * Created on December 12, 2014, 4:23 PM
 */

/*
 * It requires from you a definition of desired baudrate and to define
 * your TX and RX pins before including this header. 
 * By example:
 *
 * #define USART_BAUDRATE 19200
 * #define USART_RX_TRIS TRISCbits.RC7
 * #define USART_TX_TRIS TRISCbits.RC6
 * #include <rosso_hwserial.h>
 */

#ifndef ROSSO_HWSERIAL_H
#define	ROSSO_HWSERIAL_H

/* THIS IS NOT WORKING
void _calculate_baudrate(void){
#define max_deviation 5
#ifndef USART_BAUDRATE
  #error "please, define USART_BAUDRATE"
#endif
  #ifdef USART_ASYNC //if asyncronous
    #ifdef IS_BRG16
      #define usart_div (((5 + (( 10 * _XTAL_FREQ ) / ( 4 * USART_BAUDRATE ))) / 10 ) - 1)
      #define real_baud  (_XTAL_FREQ / 4 / ((usart_div & 0xffff) + 1))
      #if (real_baud > USART_BAUDRATE)
        #if (100 * (real_baud - USART_BAUDRATE) / USART_BAUDRATE >= max_deviation)
          #error "asynchronous baudrate deviation is too large"
        #endif
      #else
        #if ((100 * (USART_BAUDRATE - real_baud) / USART_BAUDRATE) >= max_deviation)
          #error "asynchronous baudrate deviation is too large"
        #endif
      #endif
      BAUDCONbits.BRG16 = TRUE;
      TXSTAbits.BRGH = TRUE;
      SPBRG =(UINT8)usart_div;
      SPBRGH = (UINT8)(usart_div >> 8);
    #else // not BRG16
      #define usart_div (((5 + (( 10 * _XTAL_FREQ ) / ( 16 * USART_BAUDRATE ))) / 10 ) - 1)
      #define real_baud  (_XTAL_FREQ / 16 / (usart_div + 1))
      #if (usart_div <= 0)
        #if ((100 * (USART_BAUDRATE - (_XTAL_FREQ / 16) )) / USART_BAUDRATE >= max_deviation)
          #error "asynchronous baudrate is too high"
        #endif
      #endif
      #if (usart_div <= 255)
        #if (real_baud > USART_BAUDRATE)
          #if (100 * (real_baud - USART_BAUDRATE) / USART_BAUDRATE >= max_deviation)
            #error "asynchronous baudrate deviation is too large"
          #endif
        #else
          #if (100 * (USART_BAUDRATE - real_baud) / USART_BAUDRATE >= max_deviation)
            #error "asynchronous baudrate deviation is too large"
          #endif
        #endif
        if(usart_div >= 0) 
          SPBRG = (UINT8)usart_div;
        else
          SPBRG = 0;
        TXSTAbits.BRGH := TRUE;
      // try the low-speed mode
      #else
        #define usart_div_low = (((((10 * _XTAL_FREQ) / ( 64 * USART_BAUDRATE )) + 5 ) / 10) - 1)
        #if ((usart_div_low > 255) && (100 * ((_XTAL_FREQ / (64 * 256 )) - USART_BAUDRATE)) / USART_BAUDRATE < max_deviation)
          SPBRG = 255;
          TXSTAbits.BRGH = FALSE;
          // now calculate divider and set high-speed / low-speed bit
        #elif (usart_div_low <= 255)
          if (usart_div_low >= 0) 
            SPBRG = (UINT8)usart_div_low;
          else
            SPBRG = 0;
          TXSTAbits.BRGH = FALSE;
        #else
          #error "asynchronous baudrate is too low"
        #endif
      #endif  // usart_div <= 255
    #endif // BRG16
  #else   // USART_ASYNC = false - syncronous comm.
    #define usart_div_sync (( _XTAL_FREQ / ( 4 * USART_BAUDRATE )) - 1)
    #if (usart_div_sync <= 0)
      #if ((100 * (USART_BAUDRATE - (_XTAL_FREQ / 4 ) )) / USART_BAUDRATE >= max_deviation)
        #error "synchronous baudrate is too high"
      #endif
    #endif
    // special case to consider, if baudrate is just a little too high
    #if ((usart_div_sync > 255) && (100 * ((_XTAL_FREQ / (4 * 256)  ) - USART_BAUDRATE)) / USART_BAUDRATE < max_deviation)
      SPBRG = (UINT8) 255;
    #elif (usart_div_sync <= 255)
      if (usart_div_sync >= 0) 
        SPBRG = (UINT8)usart_div_sync;
      else
        SPBRG = 0;
    #else
      #error "synchronous baudrate is too low"
    #endif
  #endif
}
 */
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
    USART_RX_TRIS = 1; // make receive pin input
    USART_TX_TRIS = 1; // make transmit pin input! (required by some older .. PICs like 16f628)
    TXSTAbits.TXEN = TRUE; // Enable transmitter
    // (makes transmit pin output)
    RCSTAbits.SPEN = 1; // activate serial port
    RCSTAbits.CREN = 1; // continuous receive
}

void USART_HW_disable(void) {
    while (!TXSTAbits.TRMT); // wait while transmission pending
    RCSTAbits.SPEN = FALSE; // disable serial port
}

void USART_HW_enable(void) {
    RCSTAbits.SPEN = TRUE; // enable serial port
}

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


#endif	/* ROSSO_HWSERIAL_H */


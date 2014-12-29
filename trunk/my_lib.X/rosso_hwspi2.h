/* 
 * File:   rosso_hwspi2.h
 * Author: MCHP
 *
 * Created on December 29, 2014, 2:51 PM
 */

/*
 * Before including this header, define the pins for SPI2 peripheral
 * like this:
 *
 * #define SPI_SDO2_DIR TRISXbits.RXn
 * #define SPI_SDI2_DIR TRISXbits.RXn
 * #define SPI_CLK2_DIR TRISXbits.RXn
 * #define SPI_SS2_DIR  TRISXbits.RXn
 *
 * where 'X' is the port letter and 'n' is the pin number.
 *
 */

#ifndef ROSSO_HWSPI2_H
#define	ROSSO_HWSPI2_H

#ifndef SSPENB
typedef enum{
    SMPMID, // Input data sample at middle of data out
    SMPEND = 0b10000000 // Input data sample at end of data out
} SPIPHASE;

typedef enum{
    MODE_00,
    MODE_01,
    MODE_10,
    MODE_11
} SPIMODE;

/* SSPCON1 REGISTER */
#define   SSPENB        0b00100000           // Enable serial port and configures SCK, SDO, SDI

typedef enum{
    SPI_FOSC_4,
    SPI_FOSC_16,
    SPI_FOSC_64,
    SPI_FOSC_TMR2,
    SLV_SSON,
    SLV_SSOFF
} SPISPEED;
#endif

#define EnableIntSPI2 (PIE3bits.SSP2IE = 1)
#define DisableIntSPI2 (PIE3bits.SSP2IE = 0)
#define SetPriorityIntSPI2(priority) (PIE3bits.SSP2IE = 0)
#define SPI2_Clear_Intr_Status_Bit (PIR3bits.SSP2IF = 0)
#define SPI2_Intr_Status (PIR3bits.SSP2IF)
#define SPI2_Clear_Recv_OV (SSP2CONbits.SSPOV = 0)
#define CloseSPI2() (SSP2CON1 &=0xDF)
#define DataRdySPI2() (SSP2STATbits.BF)

void SPI2_init(SPISPEED spi_rate, SPIMODE bus_mode, SPIPHASE smp_phase) {
    SSP2STAT &= 0x3F;                // power on state
    SSP2CON1 = 0x00;                 // power on state
    SSP2CON1 |= spi_rate;            // select serial mode
    SSP2STAT |= smp_phase;           // select data input sample phase

    if(bus_mode == 0) SSP2STATbits.CKE = 1; // data transmitted on falling edge
    else if(bus_mode == 2){
        SSP2STATbits.CKE = 1; // data transmitted on rising edge
        SSP2CON1bits.CKP = 1; // clock idle state high
    }
    else if(bus_mode == 3) SSP2CON1bits.CKP = 1; // clock idle state high
    //set SPI pins directions;
#ifdef SPI_SDO2_DIR
    error "define SPI_SDO2_DIR - the TRIS, ya know?"
#endif
#ifdef SPI_SDI2_DIR
    error "define SPI_SDI2_DIR - the TRIS, ya know?"
#endif
#ifdef SPI_CLK2_DIR
    error "define SPI_CLK2_DIR - the TRIS, ya know?"
#endif
#ifdef SPI_SS2_DIR
    error "define SPI_SS2_DIR - the TRIS, ya know?"
#endif
    if(spi_rate == SLV_SSON){
        SPI_CLK2_DIR = 1;
        SPI_SS2_DIR = 1;
    }
    else if(spi_rate == SLV_SSOFF) SPI_CLK2_DIR = 1;
    else SPI_CLK2_DIR = 0;
    SPI_SDO2_DIR = 0;
    SPI_SDI2_DIR = 1;
    SSP2CON1 |= SSPENB; // enable synchronous serial port
}

UINT8 SPI2_read(void) {
    UINT8 TempVar;
    TempVar = SSP2BUF;       //Clear BF
    PIR3bits.SSP2IF = 0;     //Clear interrupt flag
    SSP2BUF = 0x00;          // initiate bus cycle
    //while ( !SSP2STATbits.BF );                // wait until cycle complete
    while(!PIR3bits.SSP2IF); //wait until cycle complete
    return ( SSP2BUF );      // return with byte read
}

INT8 SPI2_write(UINT8 data_out) {
    UINT8 TempVar;
    TempVar = SSP2BUF; // Clears BF
    PIR3bits.SSP2IF = 0; // Clear interrupt flag
    SSP2CON1bits.WCOL = 0; //Clear any previous write collision
    SSP2BUF = data_out; // write byte to SSP2BUF register
    if (SSP2CON1 & 0x80) // test if write collision occurred
        return ( -1); // if WCOL bit is set return negative #
    else
        // while( !SSP2STATbits.BF ); // wait until bus cycle complete
        while (!PIR3bits.SSP2IF); //wait until bus cycle complete
    return ( 0); // if WCOL bit is not set return non-negative#
}



#endif	/* ROSSO_HWSPI2_H */


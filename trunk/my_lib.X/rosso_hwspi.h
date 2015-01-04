/* 
 * File:   rosso_spi_master.h
 * Author: MCHP
 * Adapted by: Vasile Guta Ciucur
 *
 * Created on December 28, 2014, 3:15 PM
 */

#ifndef ROSSO_HWSPI_H
#define	ROSSO_HWSPI_H

#define SPI_SDO_DIR TRISCbits.RC5
#define SPI_SDI_DIR TRISCbits.RC4
#define SPI_CLK_DIR TRISCbits.RC3
#define SPI_SS_DIR  TRISAbits.RA5

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

#define EnableIntSPI (PIE1bits.SSP1IE = 1)
#define DisableIntSPI (PIE1bits.SSP1IE = 0)
#define SetPriorityIntSPI(priority) (IPR1bits.SSP1IP = priority)
#define SPI_Clear_Intr_Status_Bit (PIR1bits.SSP1IF = 0)
#define SPI_Intr_Status (PIR1bits.SSP1IF)
#define SPI_Clear_Recv_OV (SSP1CONbits.SSPOV = 0)
#define CloseSPI() (SSP1CON1 &=0xDF)
#define DataRdySPI() (SSP1STATbits.BF)

void SPI_init(SPISPEED spi_rate, SPIMODE bus_mode, SPIPHASE smp_phase) {
    SSPSTAT &= 0x3F;                // power on state
    SSPCON1 = 0x00;                 // power on state
    SSPCON1 |= spi_rate;            // select serial mode
    SSPSTAT |= smp_phase;           // select data input sample phase

    if(bus_mode == 0) SSPSTATbits.CKE = 1; // data transmitted on falling edge
    else if(bus_mode == 2){
        SSPSTATbits.CKE = 1; // data transmitted on rising edge
        SSPCON1bits.CKP = 1; // clock idle state high
    }
    else if(bus_mode == 3) SSPCON1bits.CKP = 1; // clock idle state high
    //set SPI pins directions;
    if(spi_rate == SLV_SSON){
        SPI_CLK_DIR = 1;
        SPI_SS_DIR = 1;
    }
    else if(spi_rate == SLV_SSOFF) SPI_CLK_DIR = 1;
    else SPI_CLK_DIR = 0;
    SPI_SDO_DIR = 0;
    SPI_SDI_DIR = 1;
    SSPCON1 |= SSPENB; // enable synchronous serial port
}

#define SPI_Low_Speed()  CloseSPI();SPI_init(SPI_FOSC_64, MODE_00, SMP_END);
#define SPI_High_Speed() CloseSPI();SPI_init(SPI_FOSC_4,  MODE_00, SMP_END);

UINT8 SPI_read(void) {
    UINT8 TempVar;
    TempVar = SSPBUF; // Clear BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPBUF = 0x00; // initiate bus cycle
    //while ( !SSPSTATbits.BF );                  // wait until cycle complete
    while (!PIR1bits.SSPIF); // wait until cycle complete
    return ( SSPBUF); // return with byte read
}

INT8 SPI_write(UINT8 data_out) {
    UINT8 TempVar;
    TempVar = SSPBUF; // Clears BF
    PIR1bits.SSPIF = 0; // Clear interrupt flag
    SSPCON1bits.WCOL = 0; //Clear any previous write collision
    SSPBUF = data_out; // write byte to SSPBUF register
    if (SSPCON1 & 0x80) // test if write collision occurred
        return ( -1); // if WCOL bit is set return negative #
    else
        //while( !SSPSTATbits.BF );  // wait until bus cycle complete
        while (!PIR1bits.SSPIF); // wait until bus cycle complete
    return ( 0); // if WCOL bit is not set return non-negative#
}

#endif	/* ROSSO_HWSPI_H */

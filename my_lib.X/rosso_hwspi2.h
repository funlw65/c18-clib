/* 
 * File:   rosso_hwspi2.h
 * Author: MCHP
 * Adapted by: Vasile Guta Ciucur
 *
 * Created on December 29, 2014, 2:51 PM
 */

#ifndef ROSSO_HWSPI2_H
#define	ROSSO_HWSPI2_H

#define SPI_SDO2_DIR TRISDbits.RD2
#define SPI_SDI2_DIR TRISDbits.RD1
#define SPI_CLK2_DIR TRISDbits.RD0
#define SPI_SS2_DIR  TRISDbits.RD3

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

#define EnableIntSPI2 (PIE3bits.SSP2IE = 1)
#define DisableIntSPI2 (PIE3bits.SSP2IE = 0)
#define SetPriorityIntSPI2(priority) (PIE3bits.SSP2IE = 0)
#define SPI2_Clear_Intr_Status_Bit (PIR3bits.SSP2IF = 0)
#define SPI2_Intr_Status (PIR3bits.SSP2IF)
#define SPI2_Clear_Recv_OV (SSP2CONbits.SSPOV = 0)
#define CloseSPI2() (SSP2CON1 &=0xDF);
#define DataRdySPI2() (SSP2STATbits.BF);

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

#define SPI2_Low_Speed()  CloseSPI2();SPI2_init(SPI_FOSC_64,MODE_00,SMPEND);
#define SPI2_High_Speed() CloseSPI2();SPI2_init(SPI_FOSC_4,MODE_00,SMPEND);

uint8_t SPI2_read(void) {
    uint8_t TempVar;
    TempVar = SSP2BUF;       //Clear BF
    PIR3bits.SSP2IF = 0;     //Clear interrupt flag
    SSP2BUF = 0x00;          // initiate bus cycle
    //while ( !SSP2STATbits.BF );                // wait until cycle complete
    while(!PIR3bits.SSP2IF); //wait until cycle complete
    return ( SSP2BUF );      // return with byte read
}

int8_t SPI2_write(uint8_t data_out) {
    uint8_t TempVar;
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


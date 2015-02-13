/* 
 * File:   main.c
 * Author: texan
 *
 * Created on February 11, 2015, 6:54 PM
 */

/*
 * Testing the creation of a text file on FAT32 file system.
 * In this file we add our data. If the file exist, it will be
 * opened and new data appended at the end of the file.
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
// add here other definitions and include header files if needed
#include <rosso_delays.h>
#define USART_BAUDRATE 19200
#include <rosso_hwserial.h>
#include <rosso_hwspi2.h>
#define SD_CS_DDR  TRISBbits.RB0
#define SD_CS_PIN  LATBbits.LATB0
#include <rosso_sdcard.h>
#include <rosso_f32.h>

// add here your global variables, definitions and other fuctions you may need
UINT8 fileName[12] = "DATALOGSTXT"; //exactly 11 chars long, no more, no less!
// Exactly 8 chars for name, and 3 chars for extension + NULL character.
// The name and/or extension must be completed with spaces if the name is less
// than 8 chars and extension less than 3 chars.

UINT8 temp_buffer[] = "This is a record and a number follows 0\r\n";
const UINT8 test_start[] = "\r\nSD Card Test Just Started";

const UINT8 err1[] = "\r\nSD card not detected..";
const UINT8 err2[] = "\r\nCard Initialization failed..";
const UINT8 err3[] = "\r\nFAT32 not found!";
const UINT8 err4[] = "\r\nUnknown SD Card Detected!";

const UINT8 msg1[] = "\r\nStandard Capacity Card (Ver.1.x) Detected!";
const UINT8 msg2[] = "\r\nHigh Capacity Card Detected!";
const UINT8 msg3[] = "\r\nStandard Capacity Card (Ver.2.x) Detected!";

void blinkErrorLED(void) {
    while (1) { //blink red LED continuously, if error
        OnBoardLED = ~OnBoardLED;
        delay_100ms();
    }
}

// declaring the headers for the interrupt functions
void
interrupt
high_isr(void);

void
interrupt low_priority
low_isr(void);

void main() {
    UINT8 error, i, mySD_type;
    AllDigital(); // all pins digital
#ifdef ONBOARD
    OnBoardLED_dir = 0; // set pin as output
    OnBoardButton_dir = 1; // set pin as input
    OnBoardLED = 1; // set the LED on -- indicating the card access.
#endif
    //sei(); // enable general interrupts if needed
    // Add other initializations you may have...
    USART_HW_init();
    SPI2_Low_Speed();
    //initialize the dataString buffer
    for (i = 0; i < MAX_STRING_SIZE; i++) {
        //
        dataString[i] = 0;
    }
    USART_HW_putstr(test_start);

    // reset the sd_card type variable
    for (i = 0; i < 10; i++) {
        error = SD_init();
        if (!error)
            break;
    }

    if (error) {
        if (error == 1) 
            USART_HW_putstr(err1);
        if (error == 2) 
            USART_HW_putstr(err2);
        blinkErrorLED();
    }

    mySD_type = SD_card_type();
    switch (mySD_type) {
        case 1:
            USART_HW_putstr(msg1);
            break;
        case 2:
            USART_HW_putstr(msg2);
            break;
        case 3:
            USART_HW_putstr(msg3);
            break;
        default:
            USART_HW_putstr(err4);
            break;
    }

    error = F32_getBootSectorData(); //read boot sector and keep necessary data in global variables
    if (error) {
        USART_HW_putstr(err3);
        //FAT32 incompatible drive
        blinkErrorLED();
    }
    // Change the gear of the SPI2 speed :)
    SPI2_High_Speed();
    delay_1ms(); //some delay for settling the new spi speed
    F32_findFiles(0, 0);

    for (i = 0; i < 41; i++)
        dataString[i] = temp_buffer[i];

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '1';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '2';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '3';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '4';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '5';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '6';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '7';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();

    dataString[38] = '8';

    error = F32_writeFile(fileName, (UINT8 *) dataString);
    if (error)
        blinkErrorLED();
#ifdef ONBOARD
    OnBoardLED = 0; // now you can remove the SD Card.
#endif
    while (1) {
        // Add your repeating code...
    }
}

void
interrupt
high_isr(void) {
    //
}

void
interrupt low_priority
low_isr(void) {
    //
}

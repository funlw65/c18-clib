/*
 * File:   main.c
 * Author: xuser
 *
 * Created on June 9, 2016, 2:40 PM
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define memcpypgm2ram(s,d,n) memcpy(s,d,n) 
#define strcpypgm2ram(a,b)	strcpy(a,b)
#include <rosso_pff_diskio.h>
// define some slots...
#define TMR0_MILLIS 1
#define TMR0_DELAY_SLOTS 1
#define TMR0_SLOTED 1
#include <rosso_isr_delays.h>
#include <rosso_pff.h>
#include <rosso_conversion.h>

static uint8_t sector_buffer[767];


uint8_t  pas;

uint32_t pointeur;

FATFS    fs;
uint32_t fileSize;
uint8_t  data_buffer_32[32];
uint8_t  sir[6];
uint8_t  u2s[11];
uint32_t reste;

void
interrupt
high_isr(void);

void
interrupt low_priority
low_isr(void);

void BlinkSeconds(void);
void printhex(uint8_t);

FRESULT scan_files(uint8_t* path);
uint32_t get_fattime(void);

/*******************************************/
// File to read 
uint8_t fileName[32] = {"TESTFILE.TXT"};
uint8_t folder[48] = {""};

void CRLF(void) {
    USART_HW_putstrF((const uint8_t*) "\r\n\0");
}

void Tempo(long L) {
    while (L-- > 0);
}

// Blink with a LED at 0,5Hz

void BlinkSeconds() {
    if (pff_Seconds & 1)
        LED1 = 1;
    else
        LED1 = 0;
}

/********************************************************************
 * Function:        void printhex( BYTE i )
 * PreCondition:    None
 * Input:           8-bit variable
 * Output:          sends data as hex over USART
 *******************************************************************/
void printhex(uint8_t i) {
    uint8_t hi, lo;

    hi = i & 0xF0; // High nibble
    hi = hi >> 4;
    hi = hi + '0';

    if (hi > '9')
        hi = hi + 7;

    lo = (i & 0x0F) + '0'; // Low nibble

    if (lo > '9')
        lo = lo + 7;
    USART_HW_write(hi);
    USART_HW_write(lo);
}

/********************************************************************
 * Function:        void PrintSectorData( BYTE* data )
 * PreCondition:    None
 * Input:           Pointer to a 512 byte buffer
 * Output:          Humen readble data
 * Side Effects:    None
 * Overview:        Data is outputed in goups of 16 bytes per row
 *******************************************************************/
void PrintSectorData(uint8_t* data) {
    uint16_t k, px;

    for (k = 0; k < 512; k++) {
        printhex(data[k]);
        USART_HW_write(' ');

        if (((k + 1) % 16) == 0) {
            USART_HW_write(' ');

            for (px = (k - 15); px <= k; px++) {
                if (((data[px] > 33) && (data[px] < 126)) || (data[px] == 0x20)) {
                    USART_HW_write(data[px]);
                } else {
                    USART_HW_write('.');
                }
            }

            USART_HW_write(13);
            USART_HW_write(10);
        }
    }
}

/********************************************************************
 * Function:        FRESULT scan_files (char* path)
 * PreCondition:    None
 * Input:           makes a directory listing of all files on the SD-card
 * Output:          directory listin over USART
 * Overview:        http://elm-chan.org/fsw/ff/pf/readdir.html
 *******************************************************************/
/*
FRESULT scan_files(uint8_t* path) {
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;

    res = pf_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = pf_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fattrib & AM_DIR) {
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {
                printf("%s/%s\r\n", path, fno.fname);
            }
        }
    }
    return res;
}
*/
/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */
// Not really used in this project

uint32_t get_fattime(void) {
    uint32_t tmr;
    uint8_t INTCONold;
    INTCONold = INTCON;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    /* Pack date and time into a DWORD variable */
    tmr = (((uint32_t) disk_rtcYear - 80) << 25)
            | ((uint32_t) disk_rtcMon << 21)
            | ((uint32_t) disk_rtcMday << 16)
            | (uint16_t) (disk_rtcHour << 11)
            | (uint16_t) (disk_rtcMin << 5)
            | (uint16_t) (disk_rtcSec >> 1);

    INTCON = INTCONold;
    return tmr;
}


//////////

void show_report(FRESULT r) {
    //
    utoa(sir, reste, 10);
    USART_HW_putstrF((const uint8_t*) "FResult = \0");
    USART_HW_write(r + 0x30);
    USART_HW_putstrF((const uint8_t*) " Ecriture effective de \0");
    USART_HW_putstr(sir);
    USART_HW_putstrF((const uint8_t*) " bytes sur \0");
    USART_HW_putstr(fileName);
    CRLF();
}

void show_pointer2file(void) {
    //
    utoa(u2s, fs.fptr, 10);
    USART_HW_putstrF((const uint8_t*) " Nouveau pointer de fichier = ");
    USART_HW_putstr(u2s);
    CRLF();
}

void main(void) {
    FRESULT Result, FResult;
    uint8_t temp = 'A';
    uint8_t txt[20];
    unsigned int i, j, k;
    pff_Ticks = 0;
    pff_Seconds = 0;
    AllDigital();
#ifdef ONBOARD
    OnBoardLED_dir = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    sei(); // enable general interrupts
    tmr0_isr_init();
    USART_HW_init();
    LED1_TRIS	= OUTPUT;
    LED2_TRIS	= OUTPUT;
    Tempo(200000L);

    USART_HW_write(12); // cls
    Tempo(300000L);
    USART_HW_putstrF((const uint8_t*) "\r\n Test Petite FAT  rev Tue 07, 2016  XC8\r\n\0");
    USART_HW_putstrF((const uint8_t*) "PIC18F46k22 Q=16Mhz x 4 (PLL) \r\nInit SD-card Sandisk 2GB.\r\n\0");

    // Initialize SD-card with FatFs diskio functions
    Result = disk_initialize();

    if (Result != RES_OK) {
        USART_HW_putstrF((const uint8_t*) "Erreur Init SD-Card \0");
        USART_HW_write(Result + '0');
        USART_HW_putstrF((const uint8_t*) " au pas \0");
        USART_HW_write(pas + '0');
        CRLF();
        while (1);
    } else {
        USART_HW_putstrF((const uint8_t*) "SD-card initialisee.\r\n\0");
    }


    // lecture d'un morceau de fichier 
    pas = 0;
    USART_HW_putstrF((const uint8_t*) "Essai de monter le system de fichier\r\n\0");
    FResult = pf_mount(&fs);
    if (FResult == FR_OK) {
        USART_HW_putstrF((const uint8_t*) "Montage systeme de fichier OK\r\n\0");
        // do a directory listing and list all files on the SD-card
        USART_HW_putstrF((const uint8_t*) "Liste des fichiers : \r\n\0");
        //scan_files(folder);

        pas = 0;
        // open file
        FResult = pf_open(fileName);
        pas = 1;
        if (FResult == FR_OK) {
            USART_HW_putstrF((const uint8_t*) "Ouverture fichier \0");
            fileName[12] = 0;
            USART_HW_putstr(fileName);
            CRLF();
            // read file and print it until it ends
            do {
                // Read 32 bytes from the file
                //	void* buff,	 Pointer to the read buffer (NULL:Forward data to the stream)*/
                //  WORD btr,	 Number of bytes to read */
                //  WORD* reste	 Pointer to number of bytes read */
                FResult = pf_read(
                        data_buffer_32, /*Pointer to the read buffer (NULL:Forward data to the stream)*/
                        32, /* Number of bytes to read */
                        &reste /*Pointer to number of bytes read */
                        );
                if (FResult == FR_OK) {
                    USART_HW_putstrF((const uint8_t*) "Lecture OK de 31 bytes.. reste=\0");
                    word2dec(reste, sir);
                    USART_HW_putstr(sir);
                    // PutStr_RS needs a C-string (NULL terminated)
                    data_buffer_32[reste] = 0;
                    USART_HW_putstr(data_buffer_32);
                    CRLF();
                } else {
                    USART_HW_putstrF((const uint8_t*) "Erreur \0");
                    USART_HW_write(FResult + 0x30);
                    USART_HW_putstrF((const uint8_t*) " sur lecture de fichier \0");
                    USART_HW_putstr(fileName);
                    CRLF();
                    while (1);
                }
            } while (reste == 31); // if the pf_Read reads less then 31 bytes the file has ended
        } else {
            USART_HW_putstrF((const uint8_t*) "Erreur Ouverture Fichier \0");
            USART_HW_putstr(fileName);
            USART_HW_putstrF((const uint8_t*) " au pas \0");
            USART_HW_write(pas + '0');
            CRLF();
            while (1);
        }
    } else {
        USART_HW_putstrF((const uint8_t*) "Erreur \0");
        USART_HW_write(FResult + 0x30);
        USART_HW_putstrF((const uint8_t*) " sur essai de monter le system de fichier \r\n");
        CRLF(); 
        while (1);
    }
    CRLF();

    //******************************************

    // le systeme de fichier est encore ouvert et mont?
    // Le fichier doit deja exister et avoir sa taille maximale
    // preparation 	: remplissage de blancs ou zeros
    //  
    // FResult=pf_mount(0); NE PAS DEMONTER LE SYSTEME DE FICHIER .pour acceder a un autre fichier

    strcpypgm2ram(fileName, "SAUVE003.TXT");
    USART_HW_putstrF((const uint8_t*) "Essai Overture fichier SAUVE003.TXT \r\n\0");
    FResult = pf_open(fileName);
    pas = 1;
    if (FResult == FR_OK) {
        USART_HW_putstrF((const uint8_t*) "Ouverture fichier OK \0");
        fileName[12] = 0;
        USART_HW_putstr(fileName);
        CRLF();
    } else {
        USART_HW_putstrF((const uint8_t*) "Erreur Ouverture Fichier \0");
        USART_HW_putstr(fileName);
        USART_HW_putstrF((const uint8_t*) " au pas \0");
        USART_HW_write(pas + '0');
        CRLF();
        while (1);
    }
    //FResult=pf_lseek(0);  // gives an error ?
    //k=fprintf(_H_USART,(const rom char*)" FResult = %c Deplace pointer au debut\n\r",FResult + 0x30);  

    j = 0;
    for (i = 0; i < 511; i++) {
        j = i & 0x00FF;
        sector_buffer[i] = (uint8_t) (j);
    }
    USART_HW_putstrF((const uint8_t*) " buffer initialise?\r\n\0");

    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    FResult = pf_write(sector_buffer, 512, &reste);
    show_report(FResult);
    show_pointer2file();
    USART_HW_putstrF((const uint8_t*) "Finalise ecriture = \0");
    show_pointer2file();
    FResult = pf_write(0, 0, &reste);
    show_report(FResult);

    // ***************************************

    // unmount drive
    USART_HW_putstrF((const uint8_t*) " Demontage du system de fichier...\0");
    FResult = pf_mount(NULL);
    if (FResult != FR_OK) {
        USART_HW_putstrF((const uint8_t*) "Erreur \0");
        USART_HW_write(FResult + 0x30);
        CRLF();
        while (1);
    }
    USART_HW_putstrF((const uint8_t*) "OK .\r\n\0");


    //  voir Test_ecriture_lecture_512bytes.txt
    // ecriture 96 bytes -> secteur 1 et relecture..OK
    // ecriture 512 bytes sect 3  et relecture .OK

    while (1);
    {
        // Blink with the LED at 1 Hz so we know if we are alive
        BlinkSeconds();
    }
}

void
interrupt
high_isr(void) {
    //add here any high priority interrupt management functions
    tmr0_isr_intr();
}

void
interrupt low_priority
low_isr(void) {
    //add here any low priority interrupt management functions

}

/*
 * File:   main.c
 * Author: texan
 *
 * Created on June 21, 2014, 4:18 AM
 */

//#define NOBOOT 1 // uncomment if you don't use a bootloader

// enable one of the following seven boards
#define ROSSO_SER 1 //(18F46K22, 16MHz crystal, 64MHz, BootBTN = RA4, BootLED = RC2)
//#define PINGUINOP8B2_HID 2 //(18F4550, 20MHz crystal, 48MHz, BootBTN and BootLED uses RC2)
//#define PINGUINOP8B2_CDC 3 //(18F4550, 20MHz crystal, 48MHz, BootBTN and BootLED uses RC2)
//#define PINGUINOP8B3_HID 4 //(18F4550, 20MHz crystal, 48MHz, BootBTN = RA4, BootLED = RC2)
//#define PINGUINOP8B3_CDC 5 //(18F4550, 20MHz crystal, 48MHz, BootBTN = RA4, BootLED = RC2)
//#define FREEJALDUINO_CDC 6 //(18F2550, 20MHz crystal, 48MHz, UserLED1 = RA4, UserLED2 = RC2)
//#define FREEJALDUINO_HID 7 //(18F2550, 20MHz crystal, 48MHz, UserLED1 = RA4, UserLED2 = RC2)

#include <GenericTypeDefs.h>
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#define TMR0_DELAY_SLOTS 1
#define TMR0_ISR_RATE 1000
#define TMR0_SLOTED
#include <rosso_isr_delays.h>

// declaring the headers for the interrupt functions
void
interrupt
high_isr(void);

void
interrupt low_priority
low_isr(void);

void main() {
    AllDigital();
#ifdef ONBOARD
    OnBoardLED_dir    = 0; //output
    OnBoardButton_dir = 1; //input
    OnBoardLED = 0;
#endif
    ei(); // enable general interrupts
    tmr0_isr_init();
    tmr0_set_delay(0, 250);
    while (1) {
        if(tmr0_check_delay(0) == TRUE){
            tmr0_set_delay(0, 250);
            OnBoardLED = ~OnBoardLED;
        }
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


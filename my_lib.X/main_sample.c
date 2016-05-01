/*
 * File:   %<%NAME%>%.%<%EXTENSION%>%
 * Author: %<%USER%>%
 *
 * Created on %<%DATE%>%, %<%TIME%>%
 */

#include <TypeDefs.h>
/* #define NOBOOT 1 */ /*uncomment if you don't use a bootloader*/
/* disable following line if your hardware differs */
#define ONBOARD 1 /* enable the default onboard definitions*/
#include <rosso.h> /* processor type, speed, configuration bits, hardware, app_offset.*/
/* add here other definitions and include header files if needed*/

/* add here your global variables, definitions and other functions you may need*/

/* declaring the headers for the interrupt functions */
void interrupt high_isr(void);
void interrupt low_priority low_isr(void);

void main() {
    AllDigital(); /* all pins digital*/
#ifdef ONBOARD
    OnBoardLED_dir = 0; /* set pin as output */
    OnBoardButton_dir = 1; /* set pin as input */
    OnBoardLED = 0; /* set the LED off */
#endif
    /*sei();*/ /* enable general interrupts if needed*/
    /* Add other initializations you may have...*/
    while (1) {
        /* Add your repeating code...*/
    }
}

void interrupt high_isr(void) {
    /* add code if needed */
}

void interrupt low_priority low_isr(void) {
    /* add code if needed */
}

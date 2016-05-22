/* 
 * File:   main.c
 * Author: Vasile Guta-Ciucur
 *
 */

/*
 * Description: Reading analog values of RA0 pin and displaying on 
 *              16x2 LCD
 *
 */

#include <TypeDefs.h>
//#define NOBOOT 1 // uncomment if you don't use a bootloader
// disable following line if your hardware differs
#define ONBOARD 1 // enable the default onboard definitions
#include <rosso.h> // processor type, speed, configuration bits, hardware, app_offset.
#include <stdlib.h>
#include <rosso_delays.h>
#include <rosso_lcd4.h>
//#define USE_OR_MASKS
#include <rosso_adc.h>

UINT8 s[5]; /* buffer for number conversion */

const UINT8 sf[]="Analog RA0:\0"; /* this should be stored in FLASH */
const UINT8 s3[]="   ";
const UINT8 s2[]="  ";
const UINT8 s1[]=" ";

#ifdef USE_OR_MASKS
UINT8 config = ADC_FOSC_64 | ADC_RIGHT_JUST | ADC_20_TAD;
UINT8 config2 = ADC_CH0 | ADC_INT_OFF;
UINT8 config3 = ADC_TRIG_CCP5 | ADC_REF_VDD_VDD | ADC_REF_VDD_VSS;
#else
UINT8 config = ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_20_TAD;
UINT8 config2 = ADC_CH0 & ADC_INT_OFF;
UINT8 config3 = ADC_TRIG_CCP5 & ADC_REF_VDD_VDD & ADC_REF_VDD_VSS;
#endif

void main() {
    UINT16 adcval = 0;
    AllDigital(); /* all pins digital */
#ifdef ONBOARD
    OnBoardLED_dir = 0; /* output */
    OnBoardButton_dir = 1; /* input */
    OnBoardLED = 0;
#endif
	/* Setting RA0 as input and analog */
    TRISAbits.RA0 = 1;
    ANSELAbits.ANSA0 = 1;
    /* end setting RA0 */
    adc_init(config, config2, config3);
    lcd_init(LCD_HD44780);
    lcd_cursor_position(0, 0);
    lcd_write_strF(sf); /* reading the string from the FLASH */
    adc_setch(ADC_CH0);
    while (1) {
        /* Add your repeating code... */
        adc_conv();
        do{;}while(adc_busy());
        adcval = adc_read();
        lcd_cursor_position(1, 0);
        utoa(s, adcval, 10);
        /* formatting - aligning the number to right */
		if(adcval < 10) { 
			lcd_write_strF(s3);
		}
		if((adcval > 9) && (adcval < 100)) { 
			lcd_write_strF(s2);
		}
		if((adcval > 99) && (adcval < 1000)) { 
			lcd_write_strF(s1);
		}
		/* end formating */
		lcd_write_str(s);
		_delay_ms(1000);
    }
}



/* 
 * File:   rosso_isr_delays.h
 * Author: Worker
 *
 * Created on December 8, 2014, 7:32 PM
 */

#ifndef ROSSO_ISR_DELAYS_H
#define	ROSSO_ISR_DELAYS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef TMR0_DELAY_SLOTS
#define TMR0_DELAY_SLOTS 1
#endif
#ifndef TMR0_ISR_RATE
#define TMR0_ISR_RATE 1000
#endif

    unsigned int tmr0_isr_countdown[TMR0_DELAY_SLOTS];
    unsigned int tmr0_isr_counter;
    unsigned char tmr0_load;

    unsigned int tmr0_isr_counter(void) {
        unsigned int temp;
        INTCONbits.TMR0IE = 0;
        temp = tmr0_isr_counter;
        INTCONbits.TMR0IE = 1;
        return (temp);
    }

    void tmr0_set_delay(unsigned char slot, unsigned int ticks) {
        if (slot >= TMR0_DELAY_SLOTS) slot = 0;
        INTCONbits.TMR0IE = 0;
        tmr0_isr_countdown[slot] = ticks;
        INTCONbits.TMR0IE = 1;
    }

    bool tmr0_check_delay(unsigned char slot) {
        if (slot >= TMR0_DELAY_SLOTS) return (true);
        if (tmr0_isr_countdown[slot] == 0) {
            if (tmr0_isr_countdown[slot] == 0) {
                //-- note: double checking is done to cope with the isr
                //-- decrementing from 0x100 to 0x0ff without disabling the isr.
                return (true); //-- delay passed
            }
        }
        return (false); //-- still waiting
    }
#define _XTAL_FREQ 64000000

void tmr0_isr_init(void) {
        const unsigned long tmr0_div = ((unsigned long) _XTAL_FREQ / 4 / TMR0_ISR_RATE) - 1;
        unsigned char i;
#if (tmr0_div > ((256 * 256) - 1)) 
#error "requested ISR rate is too low"
#elif (tmr0_div > ((128 * 256) - 1)) 
        T0CONbits.T0PS = 7; // prescaler 256
        tmr0_load = 255 - (unsigned char) (tmr0_div / 256);

#elif (tmr0_div > ((64 * 256) - 1))
        T0CONbits.T0PS = 6; // prescaler 128
        tmr0_load = 255 - (unsigned char) (tmr0_div / 128);

#elif (tmr0_div > ((32 * 256) - 1))
        T0CONbits.T0PS = 5; // prescaler 64
        tmr0_load = 255 - (unsigned char) (tmr0_div / 64);

#elif (tmr0_div > ((16 * 256) - 1)) 
        T0CONbits.T0PS = 4; // prescaler 32
        tmr0_load = 255 - (unsigned char) (tmr0_div / 32);

#elif (tmr0_div > ((8 * 256) - 1)) 
        T0CONbits.T0PS = 3; // prescaler 16
        tmr0_load = 255 - (unsigned char) (tmr0_div / 16);

#elif (tmr0_div > ((4 * 256) - 1))
        T0CONbits.T0PS = 2; // prescaler 8
        tmr0_load = 255 - (unsigned char) (tmr0_div / 8);

#elif (tmr0_div > ((2 * 256) - 1)) 
        T0CONbits.T0PS = 1; // prescaler 4
        tmr0_load = 255 - (unsigned char) (tmr0_div / 4);

#else
        T0CONbits.T0PS = 0; //prescaler 2
        tmr0_load = 255; //- tmr0_div / 2
#endif

        T0CONbits.T0CS = 0; // internal clock
        T0CONbits.PSA = 0; // assign prescaler to timer0

        INTCONbits.TMR0IF = 0;
        INTCONbits.TMR0IE = 1;
        INTCONbits.GIE = 1; // enable global interrupts
        INTCONbits.PEIE = 1;

        for (i = 0; i < TMR0_DELAY_SLOTS; i++) tmr0_isr_countdown[i] = 0;
    }

    //call the following from the inside high_isr() or low_isr() from main.c
    void tmr0_isr_int(void) {
        unsigned char index;
        if (INTCONbits.TMR0IF) {
            TMR0L = tmr0_load;
            //-- counters
            tmr0_isr_counter = tmr0_isr_counter + 1;
            for (index = 0; index < TMR0_DELAY_SLOTS; index++) {
                if (tmr0_isr_countdown[index] != 0)
                    tmr0_isr_countdown[index] = tmr0_isr_countdown[index] - 1;
            }
            INTCONbits.TMR0IF = 0;
        } //end if
    }

#ifdef	__cplusplus
}
#endif

#endif	/* ROSSO_ISR_DELAYS_H */


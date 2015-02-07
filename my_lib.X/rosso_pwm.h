/* 
 * File:   rosso_pwm.h
 * Author: MCHP
 *
 * Created on January 3, 2015, 3:18 PM
 */

#ifndef ROSSO_PWM_H
#define	ROSSO_PWM_H

union PWMDC {
    UINT16 lpwm;
    UINT8 bpwm[2];
};

#ifndef USE_OR_MASKS

#define SINGLE_OUT     		0b00111111 /*Single output: P1A modulated; P1B, P1C, P1D assigned as port pins*/
#define FULL_OUT_FWD   		0b01111111 /*Full-bridge output forward: P1D modulated; P1A active; P1B, P1C inactive*/
#define HALF_OUT       		0b10111111 /*Half-bridge output: P1A, P1B modulated with dead-band control; P1C, P1D assigned as port pins*/
#define FULL_OUT_REV   		0b11111111 /*Full-bridge output reverse: P1B modulated; P1C active; P1A, P1D inactive*/

#else

#define SINGLE_OUT     		0b00000000 /*Single output: P1A modulated; P1B, P1C, P1D assigned as port pins*/
#define FULL_OUT_FWD   		0b01000000 /*Full-bridge output forward: P1D modulated; P1A active; P1B, P1C inactive*/
#define HALF_OUT       		0b10000000 /*Half-bridge output: P1A, P1B modulated with dead-band control; P1C, P1D assigned as port pins*/
#define FULL_OUT_REV   		0b11000000 /*Full-bridge output reverse: P1B modulated; P1C active; P1A, P1D inactive*/
#define PWM_OP_MODE_MASK	(~FULL_OUT_REV)	//Mask PWM Output Configuration bits

#endif


#define IS_DUAL_PWM(config) ((config) == HALF_OUT)

#define IS_QUAD_PWM(config) ((config) == FULL_OUT_FWD || \
                             (config) == FULL_OUT_REV)


/* The PWM mode is passed as the second parameter to the
 * 'SetOutput' routines. */
#ifndef USE_OR_MASKS

#define PWM_MODE_1     0b11111100 /* PxA,PxC active high, PxB,PxD active high */
#define PWM_MODE_2     0b11111101 /* PxA,PxC active high, PxB,PxD active low */
#define PWM_MODE_3     0b11111110 /* PxA,PxC active low, PxB,PxD active high */
#define PWM_MODE_4     0b11111111 /* PxA,PxC active low, PxB,PxD active low */

#else

#define PWM_MODE_1     0b00001100 /* PxA,PxC active high, PxB,PxD active high */
#define PWM_MODE_2     0b00001101 /* PxA,PxC active high, PxB,PxD active low */
#define PWM_MODE_3     0b00001110 /* PxA,PxC active low, PxB,PxD active high */
#define PWM_MODE_4     0b00001111 /* PxA,PxC active low, PxB,PxD active low */
#define PWM_MODE_MASK	(~PWM_MODE_4)	//Mask PWM Output Configuration bits

#endif

#ifndef USE_OR_MASKS
#define ECCP_1_SEL_TMR12		0b11001111  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR34		0b11011111  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_1_SEL_TMR56		0b11101111  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define ECCP_2_SEL_TMR12		0b11001111  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_2_SEL_TMR34		0b11011111  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_2_SEL_TMR56		0b11101111  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define ECCP_3_SEL_TMR12		0b11001111  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_3_SEL_TMR34		0b11011111  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_3_SEL_TMR56		0b11101111  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define CCP_4_SEL_TMR12			0b11001111  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_4_SEL_TMR34			0b11011111  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_4_SEL_TMR56			0b11101111  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#define CCP_5_SEL_TMR12			0b11001111  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_5_SEL_TMR34			0b11011111  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_5_SEL_TMR56			0b11101111  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM

#else
#define ECCP_1_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_1_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_1_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define ECCP_1_SEL_TMR_MASK		(~0b00110000)  	//Maks ECCP TIMER Source selection bits

#define ECCP_2_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_2_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_2_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define ECCP_2_SEL_TMR_MASK		(~0b00110000)  	//Maks ECCP TIMER Source selection bits

#define ECCP_3_SEL_TMR12	    0b00000000  	//ECCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define ECCP_3_SEL_TMR34	    0b00010000  	//ECCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define ECCP_3_SEL_TMR56	    0b00100000  	//ECCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define ECCP_3_SEL_TMR_MASK		(~0b00110000)  	//Maks ECCP TIMER Source selection bits

#define CCP_4_SEL_TMR12	    	0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_4_SEL_TMR34	    	0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_4_SEL_TMR56	    	0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define CCP_4_SEL_TMR_MASK		(~0b00110000)  	//Maks CCP TIMER Source selection bits

#define CCP_5_SEL_TMR12	    	0b00000000  	//CCP selects TIMER1 for Capture & Compare and TIMER2 for PWM
#define CCP_5_SEL_TMR34	    	0b00010000  	//CCP selects TIMER3 for Capture & Compare and TIMER4 for PWM
#define CCP_5_SEL_TMR56	    	0b00100000  	//CCP selects TIMER5 for Capture & Compare and TIMER6 for PWM
#define CCP_5_SEL_TMR_MASK		(~0b00110000)  	//Maks CCP TIMER Source selection bits
#endif

#define PWM4_TRIS    TRISBbits.RB0
#define PWM5_TRIS    TRISAbits.RA4


void OpenEPWM1(UINT8 period, UINT8 timer_source) {
    ECCP1CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode

    //configure timer source for CCP
    CCPTMRS0 &= 0b11111100;
    CCPTMRS0 |= ((timer_source & 0b00110000) >> 4);

    if ((CCPTMRS0 & 0x03) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0x03) == 0x01) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0x03) == 0x02) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}

void OpenEPWM2(UINT8 period, UINT8 timer_source) {
    ECCP2CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode

    //configure timer source for CCP
    CCPTMRS0 &= 0b11100111;
    CCPTMRS0 |= ((timer_source & 0b00110000) >> 1);

    if ((CCPTMRS0 & 0x18) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0x18) == 0x08) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0x18) == 0x10) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}

void OpenEPWM3(unsigned char period, unsigned char timer_source) {
    CCP3CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode

    //configure timer source for CCP
    CCPTMRS0 &= 0b00111111;
    CCPTMRS0 |= ((timer_source & 0b00110000) << 2);

    if ((CCPTMRS0 & 0xC0) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0xC0) == 0x40) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    } else if ((CCPTMRS0 & 0xC0) == 0x80) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}

void CloseEPWM1(void) {
    ECCP1CON = 0x00; // Turn off PWM
}

void SetDCEPWM1(UINT16 dutycycle) {
    union PWMDC DCycle;

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into ECCPR1L
    CCPR1L = DCycle.bpwm[1];

    // Write the low byte into ECCP1CON5:4
    ECCP1CON = (ECCP1CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
}

void SetOutputEPWM1(UINT8 outputconfig, UINT8 outputmode) {
    /* set P1M1 and P1M0 */
    outputconfig |= 0b00111111;
    outputmode |= 0b11111100;
    ECCP1CON = (ECCP1CON | 0b11000000) & outputconfig;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    ECCP1CON = (ECCP1CON | 0b00001111) & outputmode;

    //--------------------------------------
    if (SINGLE_OUT == outputconfig) {
        TRISCbits.TRISC2 = 0;
    } else if (IS_DUAL_PWM(outputconfig)) {
        TRISCbits.TRISC2 = 0;
        TRISBbits.TRISB2 = 0;
    } else if (IS_QUAD_PWM(outputconfig)) {
        TRISBbits.TRISB4 = 0;
        TRISBbits.TRISB1 = 0;
        TRISCbits.TRISC2 = 0;
        TRISBbits.TRISB2 = 0;
    }
    //---------------------------------------
}

void CloseEPWM2(void) {
    ECCP2CON = 0x00; // Turn off PWM
}

void SetDCEPWM2(UINT16 dutycycle) {
    union PWMDC DCycle;

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into ECCPR1L
    CCPR2L = DCycle.bpwm[1];

    // Write the low byte into ECCP1CON5:4
    ECCP2CON = (ECCP2CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
}

void SetOutputEPWM2(UINT8 outputconfig, UINT8 outputmode) {
#ifndef _OMNI_CODE_
    UINT8 TBLPTR_U, TBLPTR_L;
    _asm
        movff TBLPTRU, TBLPTR_U
        movff TBLPTRL, TBLPTR_L
    _endasm
#endif
    /* set P1M1 and P1M0 */
    outputconfig |= 0b00111111;
    outputmode |= 0b11111100;
    ECCP2CON = (ECCP2CON | 0b11000000) & outputconfig;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    ECCP2CON = (ECCP2CON | 0b00001111) & outputmode;

    //--------------------------------------
    if (SINGLE_OUT == outputconfig) {
        if (((*(unsigned char far rom *)0x300005) & 0b00000001)) {
            TRISCbits.TRISC1 = 0;
        } else {
            TRISBbits.TRISB3 = 0;
        }
    } else if (IS_DUAL_PWM(outputconfig)) {

        if (((*(unsigned char far rom *)0x300005) & 0b00000001)) {
            TRISCbits.TRISC1 = 0;
            TRISCbits.TRISC0 = 0;
        } else {
            TRISBbits.TRISB5 = 0;
            TRISBbits.TRISB3 = 0;
        }
    }
    else if (IS_QUAD_PWM(outputconfig)) {
        if (((*(unsigned char far rom *)0x300005) & 0b00000001)) {
            TRISCbits.TRISC1 = 0;
            TRISCbits.TRISC0 = 0;
        } else {
            TRISBbits.TRISB5 = 0;
            TRISBbits.TRISB3 = 0;
        }
        TRISDbits.TRISD3 = 0;
        TRISDbits.TRISD4 = 0;
    }
#ifndef _OMNI_CODE_
    _asm
        movff TBLPTR_U, TBLPTRU
        movff TBLPTR_L, TBLPTRL
    _endasm
#endif
}

void CloseEPWM3(void) {
    CCP3CON = 0x0; // Turn off PWM
}

void SetDCEPWM3(UINT16 dutycycle) {
    union PWMDC DCycle;

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into ECCPR1L
    CCPR3L = DCycle.bpwm[1];

    // Write the low byte into ECCP1CON5:4
    CCP3CON = (CCP3CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
}

void SetOutputEPWM3(UINT8 outputconfig, UINT8 outputmode) {
#ifndef _OMNI_CODE_
    UINT8 TBLPTR_U, TBLPTR_L;
    _asm
        movff TBLPTRU, TBLPTR_U
        movff TBLPTRL, TBLPTR_L
    _endasm
#endif
    /* set P1M1 and P1M0 */
    outputconfig |= 0b00111111;
    outputmode |= 0b11111100;
    CCP3CON = (CCP3CON | 0b11000000) & outputconfig;
    /* set CCP1M3, CCP1M2, CCP1M1, CCP1M0 */
    CCP3CON = (CCP3CON | 0b00001111) & outputmode;

    if (SINGLE_OUT == outputconfig) {
        if (((*(unsigned char far rom *)0x300005) & 0b00000001))
            TRISBbits.TRISB5 = 0;
        else
            TRISEbits.TRISE0 = 0;
    } else if (IS_DUAL_PWM(outputconfig)) {
        if (((*(unsigned char far rom *)0x300005) & 0b00000001))
            TRISBbits.TRISB5 = 0;
        else
            TRISEbits.TRISE0 = 0;
        TRISEbits.TRISE1 = 0;
    }
}

// we've done with E...

void OpenPWM4(UINT8 period, UINT8 timer_source) {

    CCP4CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode

    //configure timer source for CCP
    CCPTMRS1 &= 0b11111100;
    CCPTMRS1 |= ((timer_source & 0b00110000) >> 4);

    PWM4_TRIS = 0;

    if ((CCPTMRS1 & 0x03) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    }
    else if ((CCPTMRS1 & 0x03) == 0x01) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    }
    else if ((CCPTMRS1 & 0x03) == 0x02) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}

void ClosePWM4(void) {
    CCP4CON = 0; // Turn off PWM4
}

void SetDCPWM4(UINT16 dutycycle) {
    union PWMDC DCycle;

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into CCPR4L
    CCPR4L = DCycle.bpwm[1];

    // Write the low byte into CCP4CON5:4
    CCP4CON = (CCP4CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
}

void OpenPWM5(UINT8 period, UINT8 timer_source) {
    CCP5CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode
    //configure timer source for CCP
    CCPTMRS1 &= 0b11110011;
    CCPTMRS1 |= ((timer_source & 0b00110000) >> 2);

    PWM5_TRIS = 0;

    if ((CCPTMRS1 & 0x0C) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    }
    else if ((CCPTMRS1 & 0x0C) == 0x04) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    } else if ((CCPTMRS1 & 0x0C) == 0x08) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}

void ClosePWM5(void) {
    CCP5CON = 0; // Turn off PWM5
}

void SetDCPWM5(UINT16 dutycycle) {
    union PWMDC DCycle;

    // Save the dutycycle value in the union
    DCycle.lpwm = dutycycle << 6;

    // Write the high byte into CCPR5L
    CCPR5L = DCycle.bpwm[1];

    // Write the low byte into CCP5CON5:4
    CCP5CON = (CCP5CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);
}

#endif	/* ROSSO_PWM_H */


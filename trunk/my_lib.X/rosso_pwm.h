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

void OpenEPWM1(UINT8 period, UINT8 timer_source) {
    ECCP1CON = 0b00001100; //ccpxm3:ccpxm0 11xx=pwm mode

    //configure timer source for CCP
    CCPTMRS0 &= 0b11111100;
    CCPTMRS0 |= ((timer_source & 0b00110000) >> 4);

    if ((CCPTMRS0 & 0x03) == 0x00) {
        T2CONbits.TMR2ON = 0; // STOP TIMERx registers to POR state
        PR2 = period; // Set period
        T2CONbits.TMR2ON = 1; // Turn on PWMx
    }
    else if ((CCPTMRS0 & 0x03) == 0x01) {
        T4CONbits.TMR4ON = 0; // STOP TIMERx registers to POR state
        PR4 = period; // Set period
        T4CONbits.TMR4ON = 1; // Turn on PWMx
    }
    else if ((CCPTMRS0 & 0x03) == 0x02) {
        T6CONbits.TMR6ON = 0; // STOP TIMERx registers to POR state
        PR6 = period; // Set period
        T6CONbits.TMR6ON = 1; // Turn on PWMx
    }
}


/* Work In Progress */


#endif	/* ROSSO_PWM_H */


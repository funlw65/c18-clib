#ifndef ROSSO_ADC_H
#define	ROSSO_ADC_H


#define ADC_INT_ENABLE()     (PIR1bits.ADIF=0,INTCONbits.PEIE=1,PIE1bits.ADIE=1) 
#define ADC_INT_DISABLE()    (PIE1bits.ADIE=0)

#ifndef USE_OR_MASKS
//*************** A/D Conversion Clock Selection *****************************
#define ADC_FOSC_2       0b10001111 			//A/D conversion clock source is Fosc/2
#define ADC_FOSC_4       0b11001111 			//A/D conversion clock source is Fosc/4
#define ADC_FOSC_8       0b10011111 			//A/D conversion clock source is Fosc/8
#define ADC_FOSC_16      0b11011111 			//A/D conversion clock source is Fosc/16
#define ADC_FOSC_32      0b10101111 			//A/D conversion clock source is Fosc/32
#define ADC_FOSC_64      0b11101111 			//A/D conversion clock source is Fosc/64
#define ADC_FOSC_RC      0b11111111 			//A/D conversion clock source is Internal RC OSC

//************** A/D Acquisition Time Selection *******************************
#define ADC_0_TAD        0b11110001				//A/D Acquisition Time is 0 TAD
#define ADC_2_TAD        0b11110011				//A/D Acquisition Time is 2 TAD
#define ADC_4_TAD        0b11110101				//A/D Acquisition Time is 4 TAD
#define ADC_6_TAD        0b11110111				//A/D Acquisition Time is 6 TAD
#define ADC_8_TAD        0b11111001				//A/D Acquisition Time is 8 TAD
#define ADC_12_TAD       0b11111011				//A/D Acquisition Time is 12 TAD
#define ADC_16_TAD       0b11111101				//A/D Acquisition Time is 16 TAD
#define ADC_20_TAD       0b11111111				//A/D Acquisition Time is 20 TAD

//*************** ADC Interrupt Enable/Disable *******************************
#define ADC_INT_ON       0b11111111				//A/D Interrupt Enable
#define ADC_INT_OFF      0b01111111				//A/D Interrupt Disable

#else // USE_OR_MASKS
//*************** A/D Conversion Clock Select *****************************
#define ADC_FOSC_2       0b00000000 			//A/D conversion clock source is Fosc/2
#define ADC_FOSC_4       0b01000000 			//A/D conversion clock source is Fosc/4
#define ADC_FOSC_8       0b00010000 			//A/D conversion clock source is Fosc/8
#define ADC_FOSC_16      0b01010000 			//A/D conversion clock source is Fosc/16
#define ADC_FOSC_32      0b00100000 			//A/D conversion clock source is Fosc/32
#define ADC_FOSC_64      0b01100000 			//A/D conversion clock source is Fosc/64
#define ADC_FOSC_RC      0b01110000 			//A/D conversion clock source is Internal RC OSC
#define ADC_FOSC_MASK	 (~ADC_FOSC_RC)			//Mask A/D conversion clock source bits

//************** A/D Acquisition Time Selection *******************************
#define ADC_0_TAD        0b00000000				//A/D Acquisition Time is 0 TAD
#define ADC_2_TAD        0b00000010				//A/D Acquisition Time is 2 TAD
#define ADC_4_TAD        0b00000100				//A/D Acquisition Time is 4 TAD
#define ADC_6_TAD        0b00000110				//A/D Acquisition Time is 6 TAD
#define ADC_8_TAD        0b00001000				//A/D Acquisition Time is 8 TAD
#define ADC_12_TAD       0b00001010				//A/D Acquisition Time is 12 TAD
#define ADC_16_TAD       0b00001100				//A/D Acquisition Time is 16 TAD
#define ADC_20_TAD       0b00001110				//A/D Acquisition Time is 20 TAD
#define ADC_TAD_MASK	(~ADC_20_TAD)			//Mask A/D Acquisition Time bits

//*************** ADC Interrupt Enable/Disable *******************************
#define ADC_INT_ON       0b10000000				//A/D Interrupt Enable
#define ADC_INT_OFF      0b00000000				//A/D Interrupt Disable
#define ADC_INT_MASK     (~ADC_INT_ON)			//Mask A/D Interrupt
#endif	// USE_OR_MASKS

#ifndef	USE_OR_MASKS
//*************** A/D Result Format Select ***********************************
#define ADC_RIGHT_JUST   0b11111111 			// Right justify A/D result
#define ADC_LEFT_JUST    0b01111111 			// Left justify A/D result
#else // USE_OR_MASKS
//*************** A/D Result Format Select ***********************************
#define ADC_RIGHT_JUST   0b10000000 			// Right justify A/D result
#define ADC_LEFT_JUST    0b00000000 			// Left justify A/D result
#define ADC_RESULT_MASK	 (~ADC_RIGHT_JUST)		//Mask ADC Result adjust bit
#endif // USE_OR_MASKS

#ifndef USE_OR_MASKS
//************** Positive Voltage Reference Configuration *************************
#define ADC_REF_VDD_VDD			0b11110011 	// ADC voltage positive source VREF+ = AVDD
#define ADC_REF_VDD_VREFPLUS	0b11110111 	// ADC voltage positive source VREF+ = ext. source at VREF+
#define ADC_REF_FVR_BUF			0b11111011 	// ADC voltage positive source VREF+ = FVR BUF
//************** Negetive Voltage Reference Configuration *************************
#define ADC_REF_VDD_VSS		    0b11111100 	// ADC voltage negetive source VREF- = AVSS
#define ADC_REF_VDD_VREFMINUS	0b11111101 	// ADC voltage negetive source VREF- = ext. source at VREF-
#else //USE_OR_MASKS
//************** Positive Voltage Reference Configuration *************************
#define ADC_REF_VDD_VDD			0b00000000 	// ADC voltage positive source VREF+ = AVDD
#define ADC_REF_VDD_VREFPLUS	0b00000100 	// ADC voltage positive source VREF+ = ext. source at VREF+
#define ADC_REF_FVR_BUF			0b00001000 	// ADC voltage positive source VREF+ = FVR BUF
#define ADC_REF_POS_MASK		(~0b00001100)			//Mask ADC positive voltage source selection bits
//************** Negetive Voltage Reference Configuration *************************
#define ADC_REF_VDD_VSS		    0b00000000 	// ADC voltage negetive source VREF- = AVSS
#define ADC_REF_VDD_VREFMINUS	0b00001001 	// ADC voltage negetive source VREF- = ext. source at VREF-
#define ADC_REF_NEG_MASK		(~0b00000011)			//Mask ADC negetive voltage source selection bits
#endif //USE_OR_MASKS

#ifndef USE_OR_MASKS
//**************** Special Trigger Select bit *********************************
#define ADC_TRIG_CTMU           0b11111111 	//Special trigger from the CTMU
#define ADC_TRIG_CCP5           0b01111111 	//Special trigger from CCP5
#else //USE_OR_MASKS
//**************** Special Trigger Select bit *********************************
#define ADC_TRIG_CTMU           0b10000000 	//Special trigger from the CTMU
#define ADC_TRIG_CCP5           0b00000000 	//Special trigger from CCP5
#define ADC_TRIG_MASK           (~ADC_TRIG_CTMU)	//Mask ADC Special Trigger Select bit
#endif //USE_OR_MASKS

#ifndef USE_OR_MASKS
#define ADC_CH0          0b10000011  			//Select Channel 0
#define ADC_CH1          0b10000111  			//Select Channel 1
#define ADC_CH2          0b10001011  			//Select Channel 2
#define ADC_CH3          0b10001111  			//Select Channel 3
#define ADC_CH4          0b10010011  			//Select Channel 4
#define ADC_CH5          0b10010111  			//Select Channel 5
#define ADC_CH6          0b10011011  			//Select Channel 6
#define ADC_CH7          0b10011111  			//Select Channel 7
#define ADC_CH8          0b10100011  			//Select Channel 8
#define ADC_CH9          0b10100111  			//Select Channel 9
#define ADC_CH10         0b10101011  			//Select Channel 10
#define ADC_CH11         0b10101111  			//Select Channel  11
#define ADC_CH12         0b10110011  			//Select Channel 12
#define ADC_CH13         0b10110111  			//Select Channel 13
#define ADC_CH14         0b10111011  			//Select Channel 14
#define ADC_CH15         0b10111111  			//Select Channel 15
#define ADC_CH16         0b11000011  			//Select Channel 16
#define ADC_CH17         0b11000111  			//Select Channel  17
#define ADC_CH18         0b11001011  			//Select Channel 18
#define ADC_CH19         0b11001111  			//Select Channel 19
#define ADC_CH20         0b11010011  			//Select Channel 20
#define ADC_CH21         0b11010111  			//Select Channel 21
#define ADC_CH22         0b11011011  			//Select Channel 22
#define ADC_CH23         0b11011111  			//Select Channel 23
#define ADC_CH24         0b11100011  			//Select Channel 24
#define ADC_CH25         0b11100111  			//Select Channel 25
#define ADC_CH26         0b11101011  			//Select Channel 26
#define ADC_CH27         0b11101111  			//Select Channel 27
#define ADC_CH_CTMU      0b11110111  			//Select Channel 29 for CTMU
#define ADC_CH_DAC       0b11111011  			//Select Channel 30 for DAC
#define ADC_CH_FRV       0b11111111  			//Select Channel 31 for FRV

#else
#define ADC_CH0          0b00000000  			//Select Channel 0
#define ADC_CH1          0b00000100  			//Select Channel 1
#define ADC_CH2          0b00001000  			//Select Channel 2
#define ADC_CH3          0b00001100  			//Select Channel 3
#define ADC_CH4          0b00010000  			//Select Channel 4
#define ADC_CH5          0b00010100  			//Select Channel 5
#define ADC_CH6          0b00011000  			//Select Channel 6
#define ADC_CH7          0b00011100  			//Select Channel 7
#define ADC_CH8          0b00100000  			//Select Channel 8
#define ADC_CH9          0b00100100  			//Select Channel 9
#define ADC_CH10         0b00101000  			//Select Channel 10
#define ADC_CH11         0b00101100  			//Select Channel  11
#define ADC_CH12         0b00110000  			//Select Channel 12
#define ADC_CH13         0b00110100  			//Select Channel 13
#define ADC_CH14         0b00111000  			//Select Channel 14
#define ADC_CH15         0b00111100  			//Select Channel 15
#define ADC_CH16         0b01000000  			//Select Channel 16
#define ADC_CH17         0b01000100  			//Select Channel  17
#define ADC_CH18         0b01001000  			//Select Channel 18
#define ADC_CH19         0b01001100  			//Select Channel 19
#define ADC_CH20         0b01010000  			//Select Channel 20
#define ADC_CH21         0b01010100  			//Select Channel 21
#define ADC_CH22         0b01011000  			//Select Channel 22
#define ADC_CH23         0b01011100  			//Select Channel 23
#define ADC_CH24         0b01100000  			//Select Channel 24
#define ADC_CH25         0b01100100  			//Select Channel 25
#define ADC_CH26         0b01101000  			//Select Channel 26
#define ADC_CH27         0b01101100  			//Select Channel 27
#define ADC_CH_CTMU      0b01110100  			//Select Channel 29 for CTMU
#define ADC_CH_DAC       0b01111000  			//Select Channel 30 for DAC
#define ADC_CH_FRV       0b01111100  			//Select Channel 31 for FRV
#define ADC_CH_MASK		 (~0b01111100)			//Mask ADC channel selection bits
#endif


void adc_init( uint8_t config,
              uint8_t config2,
              uint8_t config3)
{
    ADCON0 = 0;
	ADCON1 = 0;
    ADCON2 = 0;

    ADCON0 = (config2) & 0b01111100;	// GET CHANNELS 	
    ADCON1 = config3; // Vref+/- & Trigger & Negetive channel select
    ADCON2 = (config & 0b10000000)	|	// RESULT
             ((config >> 4) & 0b00000111) |	// CONV CLK
             ((config << 2) & 0b00111000);	// ACQ TIME
	
    if( config2 & 0b10000000 )			// ADC INT.
    {
      PIR1bits.ADIF = 0;
      PIE1bits.ADIE = 1;
      INTCONbits.PEIE = 1;
    }
    ADCON0bits.ADON = 1;
}


void adc_setch(uint8_t channel)
{
  ADCON0 = (ADCON0 & 0b10000011)|
           ((channel) & 0b01111100);
}

void adc_conv(void)
{
  ADCON0bits.GO = 1;
}

char adc_busy(void)
{
  return(ADCON0bits.GO);
}

void adc_setchconv(uint8_t channel)
{
  ADCON0 = (ADCON0 & 0b10000011)|
           ((channel) & 0b01111100);
  ADCON0bits.GO = 1;
}

uint16_t adc_read(void)
{
  return (((uint16_t)ADRESH)<<8)|(ADRESL);
}

void adc_close(void)
{
  ADCON0bits.ADON = 0;
  PIE1bits.ADIE = 0;
}

#endif //ROSSO_ADC_H

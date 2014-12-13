/* 
 * File:   rosso_conversion.h
 * Author: Worker
 *
 * Created on December 13, 2014, 7:55 PM
 */

#ifndef ROSSO_CONVERSION_H
#define	ROSSO_CONVERSION_H

UINT8 dectobcd(UINT8 pValue){
   return (((pValue / 10) << 4) | (pValue % 10));
}

UINT8 bcdtodec(UINT8 pValue){
   return ((pValue >> 4) * 10 + (pValue & 0x0F));
}



#endif	/* ROSSO_CONVERSION_H */


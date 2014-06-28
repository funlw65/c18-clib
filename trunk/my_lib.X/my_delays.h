/* 
 * File:   my_delays.h
 * Author: texan
 *
 * Created on June 21, 2014, 1:49 AM
 */

#ifndef MY_DELAYS_H
#define	MY_DELAYS_H

#ifdef __18CXX // if MPLAB C18 Toolchain is used
#include <delays.h>
#elif __XC8         // if XC8 Toolchain is used
#include <plib/delays.h>
#endif


#if (F_CPU == 48000000) // 4/48MHz 1/12 = 0.0833
#define delay_1us() Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
#define delay_2us() delay_1us();delay_1us();
#define delay_3us() delay_1us();delay_2us();
#define delay_4us() delay_1us();delay_3us();
#define delay_5us() delay_1us();delay_4us();
#define delay_6us() delay_1us();delay_5us();
#define delay_7us() delay_1us();delay_6us();
#define delay_8us() delay_1us();delay_7us();
#define delay_9us() delay_1us();delay_8us();

#define delay_10us() Delay10TCYx(12)  //= 10 x 12 / 12 = 10us
#define delay_15us() Delay10TCYx(18)  //= 10 x 18 / 12 = 15us
#define delay_20us() Delay10TCYx(24)  //= 10 x 24 / 12 = 20us
#define delay_25us() Delay10TCYx(30)  //= 10 x 30 / 12 = 25us
#define delay_30us() Delay10TCYx(36)  //= 10 x 36 / 12 = 30us
#define delay_35us() Delay10TCYx(42)  //= 10 x 42 / 12 = 35us
#define delay_40us() Delay10TCYx(48)  //= 10 x 48 / 12 = 40us
#define delay_45us() Delay10TCYx(54)  //= 10 x 54 / 12 = 45us
#define delay_50us() Delay10TCYx(60)  //= 10 x 60 / 12 = 50us
#define delay_55us() Delay10TCYx(66)  //= 10 x 66 / 12 = 55us
#define delay_60us() Delay10TCYx(72)  //= 10 x 72 / 12 = 60us
#define delay_65us() Delay10TCYx(78)  //= 10 x 78 / 12 = 65us
#define delay_70us() Delay10TCYx(84)  //= 10 x 84 / 12 = 70us
#define delay_75us() Delay10TCYx(90)  //= 10 x 90 / 12 = 75us
#define delay_80us() Delay10TCYx(96)  //= 10 x 96 / 12 = 80us
#define delay_85us() Delay10TCYx(102)  //= 10 x 102 / 12 = 85us
#define delay_90us() Delay10TCYx(108)  //= 10 x 108 / 12 = 90us
#define delay_95us() Delay10TCYx(114)  //= 10 x 114 / 12 = 95us
#define delay_100us() Delay10TCYx(120)  //= 10 x 120 / 12 = 100us

#define delay_200us() Delay100TCYx(24) //= 100 x 24 / 12 = 200us
#define delay_300us() Delay100TCYx(36) //= 100 x 36 / 12 = 300us
#define delay_400us() Delay100TCYx(48) //= 100 x 48 / 12 = 400us
#define delay_500us() Delay100TCYx(60) //= 100 x 60 / 12 = 500us
#define delay_1ms() Delay100TCYx(120) //= 100 x 120 / 12 = 1000us = 1ms
#define delay_2ms() Delay100TCYx(240) //= 100 x 240 / 12 = 2ms

#define delay_3ms() Delay1KTCYx(36) //= 1000 x 36 / 12 = 3ms
#define delay_4ms() Delay1KTCYx(48) //= 1000 x 48 / 12 = 4ms
#define delay_5ms() Delay1KTCYx(60) //= 1000 x 60 / 12 = 5ms
#define delay_6ms() Delay1KTCYx(72) //= 1000 x 72 / 12 = 6ms
#define delay_7ms() Delay1KTCYx(84) //= 1000 x 84 / 12 = 7ms
#define delay_8ms() Delay1KTCYx(96) //= 1000 x 96 / 12 = 8ms
#define delay_9ms() Delay1KTCYx(108) //= 1000 x 108 / 12 = 9ms
#define delay_10ms() Delay1KTCYx(120) //= 1000 x 120 / 12 = 10ms
#define delay_15ms() Delay1KTCYx(180) //= 1000 x 180 / 12 = 15ms
#define delay_20ms() Delay1KTCYx(240) //= 1000 x 240 / 12 = 20ms

#define delay_25ms() Delay10KTCYx(30) //= 10000 x 30 / 12 = 25ms
#define delay_30ms() Delay10KTCYx(36) //= 10000 x 36 / 12 = 30ms
#define delay_35ms() Delay10KTCYx(42) //= 10000 x 42 / 12 = 35ms
#define delay_40ms() Delay10KTCYx(48) //= 10000 x 48 / 12 = 40ms
#define delay_45ms() Delay10KTCYx(54) //= 10000 x 54 / 12 = 45ms
#define delay_50ms() Delay10KTCYx(60) //= 10000 x 60 / 12 = 50ms
#define delay_55ms() Delay10KTCYx(66) //= 10000 x 66 / 12 = 55ms
#define delay_60ms() Delay10KTCYx(72) //= 10000 x 72 / 12 = 60ms
#define delay_65ms() Delay10KTCYx(78) //= 10000 x 78 / 12 = 65ms
#define delay_70ms() Delay10KTCYx(84) //= 10000 x 84 / 12 = 70ms
#define delay_75ms() Delay10KTCYx(90) //= 10000 x 90 / 12 = 75ms
#define delay_80ms() Delay10KTCYx(96) //= 10000 x 96 / 12 = 80ms
#define delay_85ms() Delay10KTCYx(102) //= 10000 x 102 / 12 = 85ms
#define delay_90ms() Delay10KTCYx(108) //= 10000 x 108 / 12 = 90ms
#define delay_95ms() Delay10KTCYx(114) //= 10000 x 114 / 12 = 95ms
#define delay_100ms() Delay10KTCYx(120) //= 10000 x 120 / 12 = 100ms
#define delay_150ms() Delay10KTCYx(180) //= 10000 x 180 / 12 = 150ms
//#define delay_200ms() Delay10KTCYx(240) //= 10000 x 240 / 12 = 200ms

#elif (F_CPU == 64000000)  // 4/64MHz = 1/16 = 0.0625
#define delay_1us() Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
#define delay_2us() delay_1us();delay_1us();
#define delay_3us() delay_1us();delay_2us();
#define delay_4us() delay_1us();delay_3us();
#define delay_5us() delay_1us();delay_4us();
#define delay_6us() delay_1us();delay_5us();
#define delay_7us() delay_1us();delay_6us();
#define delay_8us() delay_1us();delay_7us();
#define delay_9us() delay_1us();delay_8us();

#define delay_10us() Delay10TCYx(16)  //= 10 x 16 / 16 = 10us
#define delay_15us() Delay10TCYx(24)  //= 10 x 24 / 16 = 15us
#define delay_20us() Delay10TCYx(32)  //= 10 x 32 / 16 = 20us
#define delay_25us() Delay10TCYx(40)  //= 10 x 40 / 16 = 25us
#define delay_30us() Delay10TCYx(48)  //= 10 x 48 / 16 = 30us
#define delay_35us() Delay10TCYx(56)  //= 10 x 56 / 16 = 35us
#define delay_40us() Delay10TCYx(64)  //= 10 x 64 / 16 = 40us
#define delay_45us() Delay10TCYx(72)  //= 10 x 72 / 16 = 45us
#define delay_50us() Delay10TCYx(80)  //= 10 x 80 / 16 = 50us
#define delay_55us() Delay10TCYx(88)  //= 10 x 88 / 16 = 55us
#define delay_60us() Delay10TCYx(96)  //= 10 x 96 / 16 = 60us
#define delay_65us() Delay10TCYx(104)  //= 10 x 104 / 16 = 65us
#define delay_70us() Delay10TCYx(112)  //= 10 x 112 / 16 = 70us
#define delay_75us() Delay10TCYx(120)  //= 10 x 120 / 16 = 75us
#define delay_80us() Delay10TCYx(128)  //= 10 x 128 / 16 = 80us
#define delay_85us() Delay10TCYx(136)  //= 10 x 136 / 16 = 85us
#define delay_90us() Delay10TCYx(144)  //= 10 x 144 / 16 = 90us
#define delay_95us() Delay10TCYx(152)  //= 10 x 152 / 16 = 95us
#define delay_100us() Delay10TCYx(160)  //= 10 x 160 / 16 = 100us

#define delay_200us() Delay100TCYx(32) //= 100 x 32 / 16 = 200us
#define delay_300us() Delay100TCYx(48) //= 100 x 48 / 16 = 300us
#define delay_400us() Delay100TCYx(64) //= 100 x 64 / 16 = 400us
#define delay_500us() Delay100TCYx(80) //= 100 x80 / 16 = 500us = 0.5ms
#define delay_1ms() Delay100TCYx(160) //= 100 x160 / 16 = 1000us = 1ms

#define delay_2ms() Delay1KTCYx(32) //= 1000 x 32 / 16 = 2000us = 2ms
#define delay_3ms() Delay1KTCYx(48) //= 1000 x 48 / 16 = 3000us = 3ms
#define delay_4ms() Delay1KTCYx(64) //= 1000 x 64 / 16 = 4000us = 4ms
#define delay_5ms() Delay1KTCYx(80) //= 1000 x 80 / 16 = 5000us = 5ms
#define delay_6ms() Delay1KTCYx(96) //= 1000 x 96 / 16 = 6000us = 6ms
#define delay_7ms() Delay1KTCYx(112) //= 1000 x 112 / 16 = 7000us = 7ms
#define delay_8ms() Delay1KTCYx(128) //= 1000 x 128 / 16 = 8000us = 8ms
#define delay_9ms() Delay1KTCYx(144) //= 1000 x 144 / 16 = 9000us = 9ms
#define delay_10ms() Delay1KTCYx(160) //= 1000 x 160 / 16 = 10000us = 10ms
#define delay_15ms() Delay1KTCYx(240) //= 1000 x 240 / 16 = 15000us = 15ms

#define delay_20ms() Delay10KTCYx(32) //= 10000 x 32 / 16 = 20000us = 20ms
#define delay_25ms() Delay10KTCYx(40) //= 10000 x 40 / 16 = 25000us = 25ms
#define delay_30ms() Delay10KTCYx(48) //= 10000 x 48 / 16 = 30000us = 30ms
#define delay_35ms() Delay10KTCYx(56) //= 10000 x 56 / 16 = 35000us = 35ms
#define delay_40ms() Delay10KTCYx(64) //= 10000 x 64 / 16 = 40000us = 40ms
#define delay_45ms() Delay10KTCYx(72) //= 10000 x 72 / 16 = 45000us = 45ms
#define delay_50ms() Delay10KTCYx(80) //= 10000 x 80 / 16 = 50000us = 50ms
#define delay_55ms() Delay10KTCYx(88)  //= 10000 x 88 / 16 = 55ms
#define delay_60ms() Delay10KTCYx(96)  //= 10000 x 96 / 16 = 60ms
#define delay_65ms() Delay10KTCYx(104)  //= 10000 x 104 / 16 = 65ms
#define delay_70ms() Delay10KTCYx(112)  //= 10000 x 112 / 16 = 70ms
#define delay_75ms() Delay10KTCYx(120)  //= 10000 x 120 / 16 = 75ms
#define delay_80ms() Delay10KTCYx(128)  //= 10000 x 128 / 16 = 80ms
#define delay_85ms() Delay10KTCYx(136)  //= 10000 x 136 / 16 = 85ms
#define delay_90ms() Delay10KTCYx(144)  //= 10000 x 144 / 16 = 90ms
#define delay_95ms() Delay10KTCYx(152)  //= 10000 x 152 / 16 = 95ms
#define delay_100ms() Delay10KTCYx(160)  //= 10000 x 160 / 16 = 100ms
#define delay_150ms() Delay10KTCYx(240)  //= 10000 x 240 / 16 = 150ms
#endif

#endif	/* MY_DELAYS_H */


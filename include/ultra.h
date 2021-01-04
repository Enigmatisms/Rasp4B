/*******************************
*    Ultra Sonic Raning module Pin VCC should
*  be connected to 5V power.
******************************/
#ifndef __ULTRA_H__ 
#define __ULTRA_H__ 
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

#define Trig    3
#define Echo    1

void ultraInit(void);
float disMeasure(void);

#endif //__ULTRA_H__
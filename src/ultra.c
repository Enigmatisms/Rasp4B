/*******************************
*    Ultra Sonic Raning module Pin VCC should
*  be connected to 5V power.
******************************/
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#include "../include/ultra.h"


#define Trig    3
#define Echo    1

void ultraInit(void)
{
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
}

float disMeasure(void)
{
	struct timeval tv1;
	struct timeval tv2;
	long time1, time2;
    float dis;

	digitalWrite(Trig, LOW);
	delayMicroseconds(2);

	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);      //发出超声波脉冲
	digitalWrite(Trig, LOW);
	while(!(digitalRead(Echo) == 1));

	gettimeofday(&tv1, NULL);           //获取当前时间

	while(!(digitalRead(Echo) == 0));

	gettimeofday(&tv2, NULL);           //获取当前时间

	time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间
	time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(time2 - time1) / 1000000 * 34000 / 2;  //求出距离

	return dis;
}

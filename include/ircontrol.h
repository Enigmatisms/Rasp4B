#ifndef __IR_CTRL_HPP__
#define __IR_CTRL_HPP__
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>

#define uchar unsigned char

#define LedPinRed    0
#define LedPinGreen  1
#define LedPinBlue   2

void ledInit(void);

void ledColorSet();

int key(char *code);

int statusMap(int i);

#endif  //__IR_CTRL_HPP__
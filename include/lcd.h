#ifndef __LCD_H__ 
#define __LCD_H__ 
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>

extern int LCDAddr;
extern int BLEN;
extern int fd;

void write_word(int data);
void send_command(int comm);
void send_data(int data);

void init();

void clear();

void writeStr(int x, int y, const char data[]);

#endif //__LCD_H__ 
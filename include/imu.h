#ifndef __IMU_H__
#define __IMU_H__
#include <wiringPiI2C.h>
#include <stdio.h>
#include <math.h>

extern int fd2;

int readWord2c(int addr);

double dist(double a, double b);
double get_y_rotation(double x, double y, double z);
double get_x_rotation(double x, double y, double z);

#endif //__XL_HPP__
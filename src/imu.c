#include "../include/imu.h"

int fd2;

int readWord2c(int addr)
{
  int val;
  val = wiringPiI2CReadReg8(fd2, addr);
  val = val << 8;
  val += wiringPiI2CReadReg8(fd2, addr+1);
  if (val >= 0x8000)
    val = -(65536 - val);

  return val;
}

double dist(double a, double b)
{
  return sqrt((a*a) + (b*b));
}

double get_y_rotation(double x, double y, double z)
{
  double radians;
  radians = atan2(x, dist(y, z));
  return -(radians * (180.0 / M_PI));
}

double get_x_rotation(double x, double y, double z)
{
  double radians;
  radians = atan2(y, dist(x, z));
  return (radians * (180.0 / M_PI));
}
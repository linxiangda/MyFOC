#ifndef _AS5047P_H
#define _AS5047P_H

#include "main.h"
typedef struct{
  float angle;
  float angle_pre;
  float angle_revolution;
  float angle_revolution_pre;
  float full_rotations;
  float velocity;
}AS5047P;
float getAngle(AS5047P *encode);
float getAngle_revolution(AS5047P *encode);
void AS5047P_updata(AS5047P *encode, float ts);
void AS5047P_Init(void);

#endif

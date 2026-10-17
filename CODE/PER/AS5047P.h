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
float getAngle(void);
float getAngle_revolution(void);
float getVelocity(void);
void AS5047P_updata(void);
void AS5047P_Init(void);
extern AS5047P as5047p;
#endif

#ifndef _FOC_H
#define _FOC_H

#include "main.h"

typedef struct {
  float voltage_power_supply;//供电电压
  float electric_angle;//电角度
  float zero_electric_angle;//初始电角度
  float Iq,Id;//实际两项旋转电流
  float Ia,Ib,Ic;//实际三项静止电流
  float target_position;
  float target_velocity;
  float target_current;
}FOC;

float electricalAngle(float angle);
void Get_I(float angle_el);
void setPhaseVoltage(float Uq,float Ud,float angle_el);
void calibration(void);


extern FOC foc;

#endif

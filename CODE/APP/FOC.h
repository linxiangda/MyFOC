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
  volatile uint8_t calibration_flag;//校准完成标志位
  volatile uint8_t calibration_running;//校准中，不进入周期控制程序
  volatile uint8_t change_once_set;//串口CAN改变运行模式时执行一次
}FOCStruct;

float electricalAngle(FOCStruct *FOC, float angle);
void Get_I(FOCStruct *FOC, float angle_el);
void setPhaseVoltage(FOCStruct *FOC, float Uq,float Ud,float angle_el);


#endif

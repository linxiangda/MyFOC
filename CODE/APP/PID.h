#ifndef _PID_H
#define _PID_H

#include "main.h"

typedef struct {
    float Kp;           // 比例系数
    float Ki;           // 积分系数
    float Kd;           // 微分系数
    
    float output;       // 输出值
    
    float integral;     // 积分项
    float prev_input;   // 上一次误差
    
    float out_limit;      // 输出上限
    
    float integral_limit; // 积分限幅
	
	uint8_t firstPID;
}PID;

void PID_Init(PID *pid,float Kp,float Ki,float Kd,float out_limit);
void PID_Set_integral_limit(PID *pid); 
void PID_Set(PID *pid,float Kp,float Ki,float Kd);
float PID_Compute(PID *pid,float target,float input,float ts);
void PID_Reset(PID *pid);

extern PID position_pid;
extern PID velocity_pid;
extern PID Iq_pid;
extern PID Id_pid;

#endif

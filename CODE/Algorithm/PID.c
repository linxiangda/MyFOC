#include "PID.h"
/**
 * @brief PID初始化
 * @param PID *pid,float Kp,float Ki,float Kd,float out_limit
 * @return none
 * @attention 积分限幅与输出限幅KI挂钩。积分项最大能单独到输出限幅
 */
void PID_Init(PID *pid,float Kp,float Ki,float Kd,float out_limit) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->output = 0;
    
    pid->integral = 0;
    pid->prev_input = 0;
    
    pid->out_limit = out_limit;
    
	pid->firstPID = 1;
}

void PID_Set(PID *pid,float Kp,float Ki,float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}
/**
 * @brief PID计算
 * @param PID *pid, float target, float input, float ts
 * @return output 输出值
 * @attention none
 */
float PID_Compute(PID *pid, float target, float input, float ts) {
    float error = target - input;

    float p_term = pid->Kp * error;

    // 对测量值微分，首次调用不计算
    float d_term = 0.0f;
    if (pid->firstPID) {
        pid->firstPID = 0;
        pid->prev_input = input;
    } else {
        d_term = -pid->Kd * (input - pid->prev_input) / ts;
        pid->prev_input = input;
    }

    if (pid->Ki == 0.0f){
        pid->integral = 0.0f;
        pid->output = p_term + d_term;

        if (pid->output > pid->out_limit)
            pid->output = pid->out_limit;
        if (pid->output < -pid->out_limit)
            pid->output = -pid->out_limit;

        return pid->output;
    }

    pid->integral += pid->Ki * error * ts;
    float output = p_term + pid->integral + d_term;

    // 抗积分饱和
    if(output > pid->out_limit){
        output = pid->out_limit;
        if(error < 0){
            pid->integral = output - p_term - d_term;
        }
        else{
            pid->integral -= pid->Ki * error * ts;
        }
    } 
    else if(output < -pid->out_limit){
        output = -pid->out_limit;
        if(error > 0){
            pid->integral = output - p_term - d_term;
        }
        else{
            pid->integral -= pid->Ki * error * ts;
        }
    } 
    pid->output = output;
    return pid->output;
}
/**
 * @brief PID复位
 * @param PID *pid
 * @return none
 * @attention none
 */
void PID_Reset(PID *pid) {
    pid->integral = 0;
    pid->prev_input = 0;
    pid->output = 0;
	pid->firstPID = 1;
}

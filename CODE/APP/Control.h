#ifndef _CONTROL_H
#define _CONTROL_H

#include "main.h"

typedef enum {
    Motor_MODE_NONE = 0,
    Motor_MODE_C = 1,
	Motor_MODE_V = 2,
	Motor_MODE_P = 3,
    Motor_MODE_Calibration = 4,
    Motor_MODE_FAULT = -1,
} MOTOR_Mode;

typedef enum {
	No_error = 0,
    No_calibration = 1,
    DRV_error = 2,
    CAN_Mode_Set_error = 3,
} Error_type;
void Control(void);
void Error_Check(void);
void FOC_OpenOut(void);
void FOC_CloseOut(void);

extern MOTOR_Mode motor_mode;
extern Error_type error_type;
extern uint8_t Calibration_flag;
extern volatile uint8_t calibration_running;
extern uint8_t Change_once;
#endif

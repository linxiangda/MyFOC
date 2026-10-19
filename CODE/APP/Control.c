#include "Control.h"
#include <stdbool.h>
MOTOR_Mode motor_mode = Motor_MODE_NONE;
Error_type error_type = No_error;
uint8_t Calibration_flag = 0;
volatile uint8_t calibration_running = 0;
uint8_t Change_once=0;
/**
 * @brief 电流环
 * @param none
 * @return none
 * @attention none
 */
void current_loop(void){
	PID_Compute(&Iq_pid,foc.target_current,foc.Iq,Ts_currentLoop);
	PID_Compute(&Id_pid,0,foc.Id,Ts_currentLoop);
	setPhaseVoltage(Iq_pid.output,Id_pid.output,foc.electric_angle);
}
/**
 * @brief 速度-电流环
 * @param uint8_t once 首次清除i
 * @return none
 * @attention 级联比10：1
 */
void velocity_loop(uint8_t once){
	static uint8_t i=0;
	if(once == 1)i=0;
	i++;
	if(i % 10 == 0){
		i = 0;
		PID_Compute(&velocity_pid,foc.target_velocity,as5047p.velocity*DIR,Ts_velocityLoop);
	}
	PID_Compute(&Iq_pid,velocity_pid.output,foc.Iq,Ts_currentLoop);
	PID_Compute(&Id_pid,0,foc.Id,Ts_currentLoop); //暂且不用id
	setPhaseVoltage(Iq_pid.output,Id_pid.output,foc.electric_angle);
}
/**
 * @brief 位置-速度-电流环
 * @param uint8_t once 首次清除i
 * @return none
 * @attention 级联比100：10：1
 */
void position_loop(uint8_t once){
	static uint8_t i=0;
	if(once == 1)i=0;
	i++;
	if(i % 100 == 0){
		i = 0;
		// if(foc.target_position > POS_LIMIT_MAX) foc.target_position = POS_LIMIT_MAX;
    	// if(foc.target_position < POS_LIMIT_MIN) foc.target_position = POS_LIMIT_MIN;
		PID_Compute(&position_pid,foc.target_position,as5047p.angle_revolution*DIR,Ts_positionLoop);
	}
	if(i % 10 == 0){
		PID_Compute(&velocity_pid,position_pid.output,as5047p.velocity*DIR,Ts_velocityLoop);
	}
	PID_Compute(&Iq_pid,velocity_pid.output,foc.Iq,Ts_currentLoop);
	PID_Compute(&Id_pid,0,foc.Id,Ts_currentLoop);
	setPhaseVoltage(Iq_pid.output,Id_pid.output,foc.electric_angle);
}

/**
 * @brief 控制程序
 * @param none
 * @return none
 * @attention 根据模式进行控制
			    none 关闭输出
			    c 开启输出 电流环控制
				v 开启输出 速度-电流环控制
				p 开启输出 位置-速度-电流环控制
 */
void Control(void){	
	switch(motor_mode){
		case Motor_MODE_NONE:{
			if(Change_once == 1){FOC_CloseOut();Change_once = 0;}
			break;
		}
		case Motor_MODE_C:{
			if(Calibration_flag == 1){
				if(Change_once == 1)FOC_OpenOut();
				current_loop();
				if(Change_once == 1)Change_once = 0;
			}else error_type = No_calibration;
			break;
		}
		case Motor_MODE_V:{
			if(Calibration_flag == 1){
				if(Change_once == 1)FOC_OpenOut();
				velocity_loop(Change_once);
				if(Change_once == 1)Change_once = 0;
			}else error_type = No_calibration;
			break;
		}
		case Motor_MODE_P:{
			if(Calibration_flag == 1){
				if(Change_once == 1)FOC_OpenOut();
				position_loop(Change_once);
				if(Change_once == 1)Change_once = 0;
			}else error_type = No_calibration;
			break;
		}
		default:break;
	}
	
}

/**
 * @brief FOC输出开
 * @param none
 * @return none
 * @attention 设置pwm输出为timer_period >> 1；要等待pwm一个周期，存在延时；开启drv输出
 */
extern uint16_t timer_period;
void FOC_OpenOut(void){
	TIMER_CH2CV(PWM_TIMER) = (timer_period >> 1);
	TIMER_CH1CV(PWM_TIMER) = (timer_period >> 1);
	TIMER_CH0CV(PWM_TIMER) = (timer_period >> 1);

	Pwm_EnableOutput();
	timer_flag_clear(PWM_TIMER, TIMER_FLAG_UP);
	while (RESET == timer_flag_get(PWM_TIMER, TIMER_FLAG_UP));
	timer_flag_clear(PWM_TIMER, TIMER_FLAG_UP);

	DRV8323_enable_gd();
}
/**
 * @brief FOC输出关
 * @param none
 * @return none
 * @attention 设置pwm输出为timer_period >> 1；要等待pwm一个周期，存在延时；关闭drv输出
 */
void FOC_CloseOut(void){
	DRV8323_disable_gd();

	TIMER_CH2CV(PWM_TIMER) = (timer_period >> 1);
	TIMER_CH1CV(PWM_TIMER) = (timer_period >> 1);
	TIMER_CH0CV(PWM_TIMER) = (timer_period >> 1);

	Pwm_EnableOutput();
	timer_flag_clear(PWM_TIMER, TIMER_FLAG_UP);
	while (RESET == timer_flag_get(PWM_TIMER, TIMER_FLAG_UP));
	timer_flag_clear(PWM_TIMER, TIMER_FLAG_UP);
	
	Pwm_DisableOutput();
}
/**
 * @brief 错误检测
 * @param none
 * @return none
 * @attention 没校准进行控制，报错；DRV_nFAULT置0，报错，尝试reset
 */
void Error_Check(void){
	if (error_type == No_calibration){
		error_type = No_error;
		motor_mode = Motor_MODE_NONE;
		Serial_SendString("no calibration\n");
	}
	if (error_type == CAN_Mode_Set_error){
		error_type = No_error;
		Serial_SendString("CAN mode set error\n");
	}
	if (gpio_input_bit_get(DRV_nFAULT_PORT,DRV_nFAULT_PIN) == 0) {
		error_type = DRV_error;
		motor_mode = Motor_MODE_FAULT;
		FOC_CloseOut();
		DRV8323_reset();
		//发送一次报错
		static bool first = true;
		if(first){
			first = false;
			Serial_SendString("drv_fault\n");
			uint32_t f = DRV8323_getFault();
			Serial_SendString("FSR:");
			Serial_SendFloatNumber((float)(f >> 11));
			Serial_SendByte(',');
			Serial_SendFloatNumber((float)(f & 0x7FF));
			Serial_SendByte('\n');
		}
		delay_1ms(1);

		if (gpio_input_bit_get(DRV_nFAULT_PORT,DRV_nFAULT_PIN) == 1) {
			first = true;
			Serial_SendString("Reset\n");
			error_type = No_error;
			motor_mode = Motor_MODE_NONE;
			/*错误后目标值设0*/
			foc.target_position=0;
			foc.target_velocity=0;
			foc.target_current=0;
		}
	}	
}

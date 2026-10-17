#include "TimerTask.h"
/**
 * @brief 定时器0，PWM顶点中断
 * @attention 母线电压ADC Iq/Id计算 编码器读取 电角度计算 控制程序
 */
void Timer0_Test(void){
	if(calibration_running) return;
	ADC_Get();         
	AS5047P_updata();
	electricalAngle(as5047p.angle);
    Get_I(foc.electric_angle);//放电角度获取后
	Control();
}
/**
 * @brief 定时器5中断 1ms
 * @attention RGB灯计时
 */
void Timer_1ms_Test(void){
	RGB_Set();
}

/**
 * @brief 定时器5中断 5ms
 * @attention 
 */
void Timer_5ms_Test(void){


}	
/**
 * @brief 定时器5中断 10ms
 * @attention 
 */
void Timer_10ms_Test(void){
		
}	

/**
 * @brief 定时器5中断 20ms
 * @attention 
 */
void Timer_20ms_Test(void){

}
/**
 * @brief 定时器5中断 1s
 * @attention
 */
void Timer_1s_Test(void){

	
}



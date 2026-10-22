#include "TimerTask.h"
#include "Control.h"
#include "WS2812B.h"

extern volatile  MOTOR_Mode motor_mode;
extern uint16_t time_main;
extern void CAN_SendFeedback(void);
// /**
//  * @brief 定时器0中断服务函数
//  * @param none
//  * @return none
//  * @attention none
//  */
// void TIMER0_UP_IRQHandler(void){
//     if(timer_interrupt_flag_get(TIMER0, TIMER_INT_FLAG_UP) == SET){

		
// 		timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
//     }
// }
/**
 * @brief 定时器5中断服务函数
 * @param none
 * @return none
 * @attention none
 */
void TIMER5_IRQHandler(void){
	static uint16_t tim_count = 0;
	if(timer_interrupt_flag_get(TIMER5, TIMER_INT_UP) == SET){
		time_main++;
		tim_count++;
		RGB_Set(motor_mode);
		if(tim_count % 5 == 0){
			CAN_SendFeedback();
		}
		if(tim_count % 10 == 0){
			
		}
		if(tim_count % 20 == 0){
			
		}
		if(tim_count % 1000 == 0){
			
			tim_count=0;
		}
		timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
	}
}





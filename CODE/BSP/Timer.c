#include "Timer.h"

/**
 * @brief 定时器5初始化
 * @param none
 * @return none
 * @attention none
 */
void TIMER5_Init(void){
	// TIMER5CLK = SystemCoreClock(120Mhz)/1000/120 = 1KHz
	rcu_periph_clock_enable(RCU_TIMER5);        //开启TIMER5时钟
	
	timer_parameter_struct TIMER5_StructureInit;
	timer_deinit(TIMER5);
	TIMER5_StructureInit.counterdirection = TIMER_COUNTER_UP;// 计数方向：向上计数
	TIMER5_StructureInit.period = 1000 - 1;                  // 自动重装载值：999（计数到1000溢出）
	TIMER5_StructureInit.prescaler = 120-1;                  // 预分频值：119
	timer_init(TIMER5, &TIMER5_StructureInit);               // 初始化TIMER5
	
	nvic_irq_enable(TIMER5_IRQn, 2, 1);                      // NVIC配置：TIMER5中断通道，抢占优先级1，子优先级1
	timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);   // 清除更新中断标志（避免刚初始化就触发中断）
	timer_interrupt_enable(TIMER5, TIMER_INT_UP);	           // 使能更新中断（计数溢出时触发）
	
//	timer_enable(TIMER5);
}


#include "PWM.h"
#include "TimerTask.h"
uint16_t timer_period = 0;
/**
 * @brief 定时器0PWM初始化
 * @param none
 * @return none
 * @attention none
 */
void TIMER0_PWM_Init(void){	
    uint32_t timer_clock = 120000000; //时钟频率
    uint32_t pwm_freq = 20000;  //PWM频率20kHz
    uint32_t prescaler = 0;     //时基单元分频
    timer_period = (timer_clock / (prescaler + 1) / pwm_freq /2) - 1;
	/*定时器配置*/                                                        
	timer_deinit(PWM_TIMER);                                                         //TIMER0复位
	timer_parameter_struct TIMER_StructInit;										 //定义定时器初始化结构体
	TIMER_StructInit.alignedmode = TIMER_COUNTER_CENTER_UP;						//对齐模式：中心对齐模式
	TIMER_StructInit.counterdirection = TIMER_COUNTER_UP;							 //计数模式：向上计数
	TIMER_StructInit.clockdivision = TIMER_CKDIV_DIV1;								 //时钟分频
	TIMER_StructInit.period = timer_period;												 //自动重装载值(计数周期)
	TIMER_StructInit.prescaler = prescaler;											 //预分频器值
	TIMER_StructInit.repetitioncounter = 0;                                          //重复计数器不重复
	timer_init(PWM_TIMER, &TIMER_StructInit);                                        //TIMER0初始化
	
	/*初始化通道输出比较模式*/
	timer_oc_parameter_struct timer0_oc_parameter_struct_Init;                        //定义通道输出比较结构体
	timer_channel_output_struct_para_init(&timer0_oc_parameter_struct_Init);
	timer0_oc_parameter_struct_Init.outputstate = TIMER_CCX_ENABLE;					 //使能PWM通道：
	timer0_oc_parameter_struct_Init.ocpolarity = TIMER_OC_POLARITY_HIGH;              //有效时为高电平
	timer_channel_output_config(PWM_TIMER, PWM_CHA, &timer0_oc_parameter_struct_Init);
    timer_channel_output_config(PWM_TIMER, PWM_CHB, &timer0_oc_parameter_struct_Init);
	timer_channel_output_config(PWM_TIMER, PWM_CHC, &timer0_oc_parameter_struct_Init);
	timer_channel_output_config(PWM_TIMER, TIMER_CH_3, &timer0_oc_parameter_struct_Init);
	
	/*影子寄存器配置 占空比同时更新*/
	timer_channel_output_shadow_config(PWM_TIMER, PWM_CHA, TIMER_OC_SHADOW_ENABLE);
	timer_channel_output_shadow_config(PWM_TIMER, PWM_CHB, TIMER_OC_SHADOW_ENABLE);
	timer_channel_output_shadow_config(PWM_TIMER, PWM_CHC, TIMER_OC_SHADOW_ENABLE);
	timer_channel_output_shadow_config(PWM_TIMER, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
	/*PWM0模式  CNT<CCR时有效 CNT>=CCR时有效（CNT为固定三角波，CCR决定PWM波）*/
	timer_channel_output_mode_config(PWM_TIMER, PWM_CHA, TIMER_OC_MODE_PWM0);  
	timer_channel_output_mode_config(PWM_TIMER, PWM_CHB, TIMER_OC_MODE_PWM0); 
	timer_channel_output_mode_config(PWM_TIMER, PWM_CHC, TIMER_OC_MODE_PWM0);     
	timer_channel_output_mode_config(PWM_TIMER, TIMER_CH_3, TIMER_OC_MODE_PWM0);	
	/*初始50%占空比*/
	timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHA, timer_period >> 1);
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHB, timer_period >> 1);
	timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHC, timer_period >> 1);
	timer_channel_output_pulse_value_config(PWM_TIMER, TIMER_CH_3, timer_period - 20); //接近顶点时触发ADC电流采集
	
	/*定时器0中断初始化*/
	nvic_irq_enable(TIMER0_UP_IRQn, 1, 0);
	timer_interrupt_flag_clear(PWM_TIMER, TIMER_INT_FLAG_UP);
	timer_interrupt_enable(PWM_TIMER, TIMER_INT_UP);
//	timer_enable(PWM_TIMER);												         //使能TIMER0，计数器开始计数
}
/**
 * @brief 定时器0中断服务函数
 * @param none
 * @return none
 * @attention none
 */
void TIMER0_UP_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER0, TIMER_INT_FLAG_UP) == SET){
		Timer0_Test();
		timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief PWM输出启用
 * @param none
 * @return none
 * @attention none
 */
void Pwm_EnableOutput(void){
	timer_primary_output_config(PWM_TIMER, ENABLE);
}

/**
 * @brief PWM输出禁用
 * @param none
 * @return none
 * @attention none
 */
void Pwm_DisableOutput(void){
	timer_primary_output_config(PWM_TIMER, DISABLE);
}

/**
 * @brief 设置三项PWM输出
 * @param float duty1
 * @param float duty2
 * @param float duty3
 * @return none
 * @attention none
 */
void Pwm_Set(float duty1,float duty2,float duty3){
	float dc_a = constrain(duty1 / foc.voltage_power_supply, 0.0f , 1.0f );
	float dc_b = constrain(duty2 / foc.voltage_power_supply, 0.0f , 1.0f );
	float dc_c = constrain(duty3 / foc.voltage_power_supply, 0.0f , 1.0f );
	
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHA, (uint16_t)(dc_a * timer_period));
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHB, (uint16_t)(dc_b * timer_period));
    timer_channel_output_pulse_value_config(PWM_TIMER, PWM_CHC, (uint16_t)(dc_c * timer_period));
}

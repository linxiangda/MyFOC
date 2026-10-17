#include "main.h"

int aa = 0;
int main(void){
	systick_config();
//	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // 使能 DWT
//	DWT->CYCCNT = 0;                                  // 清零周期计数
//	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;              // 使能周期计数器
	rcu_config();
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);//分组2：抢占2位/响应2位
	/*  注入组ADC完成中断 0-0
		定时器0中断 1-0
		串口空闲中断 2-0
		定时器5中断 2-1
		systick 3-3
	*/
	delay_1ms(500);//等1s，启动电压稳定
    GPIO_Init();//GPIO初始化
	USART2_Init();//串口初始化
	TIMER5_Init();//定时器5初始化，1ms，先不使能
	TIMER0_PWM_Init();//定时器1/PWM初始化，50us/20khz，先不使能
	
	WS2812B_Init();//RGB灯初始化，使用定时器3
	AS5047P_Init();//编码器初始化，使用SPI0
	DRV8323_Init();//驱动芯片初始化，使用SPI1
	ADC0_Init();//ADC0初始化
	
	//PID初始化
	PID_Init(&position_pid, 5, 0, 0.1, 80);
	PID_Init(&velocity_pid, 0.2, 0.3, 0, 8.0);
	PID_Init(&Iq_pid, 4, 40, 0, 12);
	PID_Init(&Id_pid, 4, 40, 0, 12);
	
	//定时器启用
	timer_enable(TIMER0);
	timer_enable(TIMER5);
	//PWM输出启动
	timer_primary_output_config(PWM_TIMER, ENABLE);
	//让注入组先跑一会儿，开启ADC校准
	delay_1ms(100);          
	ADC_Calibrate();	
	Serial_SendString("Init Over\n");
	
	//Flash读入，若已校准过，可直接运行
	if (Flash_Cal_Read(&foc.zero_electric_angle)) {
        Calibration_flag = 1;  
    }
	while(1){
		if(motor_mode == Motor_MODE_Calibration){//主函数中校准，校准期间关闭定时器中断，上电后只用校准一次
			calibration_running = 1; 
			FOC_OpenOut();//FOC输出开启
			calibration();//开环校准程序
			Calibration_flag = 1;//校准成功一次，标志位置1
			motor_mode = Motor_MODE_NONE;//校准后置为Motor_MODE_NONE
			calibration_running	= 0;		
		}
		
		if(time_main >= 20){//20ms执行
			time_main = 0;
			if(motor_mode != Motor_MODE_NONE && motor_mode != Motor_MODE_FAULT){
				sendJustFloatPacket();//vofa发送程序，在闭环情况下发送,稍微费时
			}
			Error_Check();//错误检测，不费时
		}
		Serial_ProcessCommand();//串口数据解析
	}
}


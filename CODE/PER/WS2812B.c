#include "WS2812B.h"

uint16_t LEDBuf[LEDBUFLEN];// PWM数据缓冲区，存储每个位对应的占空比值
volatile uint32_t LED_RGB = COLOR_WHITE;// 当前LED颜色值，默认蓝色

volatile MOTOR_Mode led_mode = Motor_MODE_NONE;
volatile uint16_t led_timer = 0;                        // 剩余毫秒数
volatile uint8_t led_phase = 0;                         // 0=亮灯阶段, 1=灭灯阶段

//PWM频率=定时器时钟/(period+1)/(prescaler+1)。对应定时器溢出周期的倒数(定时器更新频率)
//=120Mhz/150=800khz                   1.25us
//PWM分辨率=1/(period+1)
//=1/150
//PWM占空比=pulse/(period+1)。当CNT<=CCR，输出1，当CNT>CCR，输出0。
//=pulse/150
/**
  * @brief  WS2812B初始化函数
  * @note   配置TIMER3的PWM输出和DMA传输，用于驱动WS2812B LED
  * @retval 无
  */
void WS2812B_Init(void){	
	timer_deinit(TIMER3);                                       //TIMER3复位
	timer_parameter_struct TIMER3_StructInit;										//定义定时器初始化结构体
	TIMER3_StructInit.alignedmode = TIMER_COUNTER_EDGE;					//对齐模式：边沿对齐模式
	TIMER3_StructInit.counterdirection = TIMER_COUNTER_UP;			//计数模式：向上计数
	TIMER3_StructInit.clockdivision = TIMER_CKDIV_DIV1;					//时钟分频
	TIMER3_StructInit.period = 150 - 1;												//自动重装载值(计数周期)
	TIMER3_StructInit.prescaler = 0;														//预分频器值
	TIMER3_StructInit.repetitioncounter = 0;
	timer_init(TIMER3, &TIMER3_StructInit);                     //TIMER3初始化
	
	timer_oc_parameter_struct timer3_oc_parameter_struct_Init;                        //定义通道输出比较结构体
	timer_channel_output_struct_para_init(&timer3_oc_parameter_struct_Init);
	timer3_oc_parameter_struct_Init.outputstate = TIMER_CCX_ENABLE;					 // 通道输出使能
	timer3_oc_parameter_struct_Init.ocpolarity = TIMER_OC_POLARITY_HIGH;              // 输出极性高有效
	timer_channel_output_config(TIMER3, TIMER_CH_2, &timer3_oc_parameter_struct_Init);//初始化通道输出比较模式
	
	
	timer_channel_output_shadow_config(TIMER3, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);  //TIMER3通道输出影子功能
	timer_channel_output_mode_config(TIMER3, TIMER_CH_2, TIMER_OC_MODE_PWM0);        //通道输出比较模式：PWM0模式
    timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, 0);                  //设置输出比较值CCR=0
	timer_primary_output_config(TIMER3,ENABLE);                                       // 使能主输出

	timer_dma_transfer_config(TIMER3,TIMER_DMACFG_DMATA_CH2CV,TIMER_DMACFG_DMATC_1TRANSFER);// 配置TIMER3的DMA功能
	timer_channel_dma_request_source_select(TIMER3,TIMER_DMAREQUEST_UPDATEEVENT);
	timer_dma_enable(TIMER3,TIMER_DMA_CH2D);
	
	dma_parameter_struct DMA_InitStruct;							//定义DMA初始化结构体
	dma_deinit(DMA0, DMA_CH4);										//复位DMA0的CH4通道高
	DMA_InitStruct.periph_addr = (uint32_t)(&TIMER_CH2CV(TIMER3));//配置外设基地址
	DMA_InitStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;		//配置外设地址是否自增
	DMA_InitStruct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;	//配置外设数据传输宽度
	DMA_InitStruct.memory_addr = (uint32_t)&LEDBuf;//配置存储器基地址
	DMA_InitStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;   //配置存储器地址是否自增
	DMA_InitStruct.memory_width = DMA_MEMORY_WIDTH_16BIT;     //配置存储器数据传输宽度
	DMA_InitStruct.direction = DMA_MEMORY_TO_PERIPHERAL;			//配置传输方向
	DMA_InitStruct.number = LEDBUFLEN;//配置通道数据传输数量
	DMA_InitStruct.priority = DMA_PRIORITY_HIGH;				//配置通道传输软件优先级
	dma_init(DMA0, DMA_CH4, &DMA_InitStruct);						      //初始化DMA0的CH4通道

	dma_circulation_disable(DMA0, DMA_CH4);					//使能循环模式
	dma_memory_to_memory_disable(DMA0,DMA_CH4);	    //存储器到存储器传输模式失能（硬件触发，需要外设触发）
	
	//初始化缓冲区为0
    for(int i = 0; i < LEDBUFLEN; i++) LEDBuf[i] = 0;
	
	//初始化发送一次DMA
	dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_G);
    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);
    dma_channel_enable(DMA0, DMA_CH4);
    timer_counter_value_config(TIMER3, 0);
    timer_enable(TIMER3);
}

/**
  * @brief  转换颜色数据到PWM缓冲区（内部函数）
  */
static void ConvertColorToBuffer(uint32_t color){
    uint8_t r = (color >> 16) & 0xFF;  // 红色
    uint8_t g = (color >> 8) & 0xFF;   // 绿色
    uint8_t b = color & 0xFF;          // 蓝色
    
    //发送绿色8位
    uint8_t mask = 0x80;
    for(int i = 0; i < 8; i++)
    {
        LEDBuf[i] = (g & mask) ? HIBi : LWBi;
        mask >>= 1;
    }
    
    //发送红色8位
    mask = 0x80;
    for(int i = 0; i < 8; i++)
    {
        LEDBuf[8 + i] = (r & mask) ? HIBi : LWBi;
        mask >>= 1;
    }
    
    //发送蓝色8位
    mask = 0x80;
    for(int i = 0; i < 8; i++)
    {
        LEDBuf[16 + i] = (b & mask) ? HIBi : LWBi;
        mask >>= 1;
    }
    //剩下240位为0
}

/**
  * @brief  更新显示（设置颜色后调用这个）
  */
void WS2812B_Update(void){
	if(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF)) return;
	
    static uint32_t last_sent = 0xFFFFFFFFU;
	uint32_t color = LED_RGB;
    if (color == last_sent)return;

    dma_channel_disable(DMA0, DMA_CH4);
    timer_disable(TIMER3);
	
    ConvertColorToBuffer(color);
	last_sent = color;
	
    dma_transfer_number_config(DMA0, DMA_CH4, LEDBUFLEN);
	dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_G);
    dma_flag_clear(DMA0, DMA_CH4, DMA_FLAG_FTF);
    dma_channel_enable(DMA0, DMA_CH4);
    timer_counter_value_config(TIMER3, 0);
    timer_enable(TIMER3);
}

/**
  * @brief  设置颜色
  * @param  color: 颜色值
  */
void WS2812B_SetColor(uint32_t color){
    LED_RGB = color;
}

/**
  * @brief  设置RGB分量值
  * @param  r: 红色值(0-255)
  * @param  g: 绿色值(0-255)
  * @param  b: 蓝色值(0-255)
  */
void WS2812B_SetRGB(uint8_t r, uint8_t g, uint8_t b){
    LED_RGB = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

/**
  * @brief  关闭LED
  */
void WS2812B_Clear(void){
    LED_RGB = COLOR_OFF;
    WS2812B_Update();  // 立即更新显示
}

/**
  * @brief  无阻塞发送颜色
  */
void RGB_Set(void){
	static uint16_t time = 2000;
	time--;
	if(time == 0){
		time = 2000;
	}

	if(time > 1500){
		WS2812B_SetColor(COLOR_OFF);
		WS2812B_Update();
		return;
	}

	led_mode = motor_mode;

	switch(led_mode){
		case Motor_MODE_FAULT:
			WS2812B_SetColor(COLOR_RED);
			break;
		case Motor_MODE_NONE:
			WS2812B_SetColor(COLOR_WHITE);
			break;
		case Motor_MODE_C:
			WS2812B_SetColor(COLOR_YELLOW);
			break;
		case Motor_MODE_V:
			WS2812B_SetColor(COLOR_GREEN);
			break;
		case Motor_MODE_P:
			WS2812B_SetColor(COLOR_BLUE);
			break;
		case Motor_MODE_Calibration:
			WS2812B_SetColor(COLOR_PURPLE);
			break;
		default:
			WS2812B_SetColor(COLOR_OFF);
			break;
	}
	WS2812B_Update();
}


#include "ADC.h"
#include "main.h"
#include "Control.h"
#include "Serial.h"
uint16_t ADC_buff[1]; // 母线电压，DMA 循环刷新
static volatile float ADC_data[4]; // 数据暂存

float offset_ia = 0;
float offset_ib = 0;
float offset_ic = 0;
float offset_V = 0;

#define SHUNT_RESISTENCE	0.005f						
#define V_SCALE (19.0000f * 3.3f / 4095.0f)     					
#define I_SCALE (3.3f / 4095.0f) / SHUNT_RESISTENCE / 40.0f		

void ADC0_Init(void){
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);   // 20MHz

	gpio_init(ADC0_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC0_IN0_PIN);     //PA0(ADC0_IN0)配置为模拟输入
	gpio_init(ADC0_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC0_IN1_PIN);     //PA1(ADC0_IN1)配置为模拟输入
	gpio_init(ADC0_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC0_IN2_PIN);     //PA2(ADC0_IN2)配置为模拟输入
	gpio_init(ADC0_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ADC0_IN3_PIN);     //PA3(ADC0_IN3)配置为模拟输入

	adc_deinit(ADC0);
	adc_resolution_config(ADC0, ADC_RESOLUTION_12B);   //12位分辨率
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);  //数据右对齐

	/* 规则组,母线电压CH0，连续+DMA */
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);       //扫描模式，注入组用
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE); //连续模式
	
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);      //规则组采单通道
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_7POINT5); //规则组排序/通道/采样时间
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);//软件触发
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	
	/* 注入组,Ia(CH3)、Ib(CH2)，由TIMER0_CH3硬件触发 */
	adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 2);     //注入组采双通道
	adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_3, ADC_SAMPLETIME_7POINT5);
	adc_inserted_channel_config(ADC0, 1, ADC_CHANNEL_2, ADC_SAMPLETIME_7POINT5);
	adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_T0_CH3);//触发条件T0通道3
	adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, ENABLE);

	/* DMA：规则组数据循环搬入 ADC_buff */
	dma_parameter_struct DMA_InitStruct;
	dma_deinit(DMA0, DMA_CH0);
	DMA_InitStruct.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
	DMA_InitStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	DMA_InitStruct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	DMA_InitStruct.memory_addr = (uint32_t)ADC_buff;
	DMA_InitStruct.memory_inc = DMA_MEMORY_INCREASE_DISABLE;
	DMA_InitStruct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	DMA_InitStruct.direction = DMA_PERIPHERAL_TO_MEMORY;
	DMA_InitStruct.number = 1;
	DMA_InitStruct.priority = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &DMA_InitStruct);
	dma_circulation_enable(DMA0, DMA_CH0);
	dma_channel_enable(DMA0, DMA_CH0);
	adc_dma_mode_enable(ADC0);

	/* ADC启动与校准 */
	adc_enable(ADC0);
	delay_1ms(1);
	adc_calibration_enable(ADC0);
	while (ADC_CTL1(ADC0) & ADC_CTL1_CLB);

	/* 注入组转换完成中断 */
	nvic_irq_enable(ADC0_1_IRQn, 0, 0);
	adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
	adc_interrupt_enable(ADC0, ADC_INT_FLAG_EOIC);
    /* 规则组转换开启 */
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

/* 零电流偏移校准：必须在 timer_enable(TIMER0) 之后调用 */
void ADC_Calibrate(void){
	const int times = 1000;
	uint32_t sum0 = 0, sum1 = 0;

	Serial_SendString("cal_start\n");

	for(int i = 0; i < times; i++){
		sum0 += adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
		sum1 += adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_1);
		delay_1ms(1);
	}
	offset_ia = (float)sum0 / times;
	offset_ib = (float)sum1 / times;

	Serial_SendString("cal_done off:");
	Serial_SendFloatNumber(offset_ia);
	Serial_SendByte(',');
	Serial_SendFloatNumber(offset_ib);
	Serial_SendString("\n");
}

void ADC_Get(float data[4]){
	ADC_data[3] = ADC_buff[0] * V_SCALE;
	for(uint8_t i=0;i<4;i++){
		data[i] = ADC_data[i];
	}
}

/* 注入组采完：硬件同步，控制中断里零等待 */
void ADC0_1_IRQHandler(void){
	if(adc_interrupt_flag_get(ADC0, ADC_INT_FLAG_EOIC) != RESET){
		adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
		ADC_data[0] = (float)(adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0) - offset_ia) * I_SCALE;
		ADC_data[1] = (float)(adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_1) - offset_ib) * I_SCALE;
		ADC_data[2] = -ADC_data[0] - ADC_data[1];	
		Control_Loop();
	}	
}

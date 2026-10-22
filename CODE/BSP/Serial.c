#include "Serial.h"
#include <stdio.h>
#include <string.h>

#define RX_BUF_LEN 64
#define RX_DMA_LEN 128
static char rx_buf[RX_BUF_LEN];

static volatile uint8_t rx_dma_buf[RX_DMA_LEN];
static volatile uint16_t rx_dma_head = 0;

extern void Serial_Parse(char *str);
/**
 * @brief USART2初始化
 * @param none
 * @return none
 * @attention 输入配置DMA：串口空闲读取，数据完整，环型缓冲，抗干扰好
			  输出配置DMA：
 */
void USART2_Init(void){	
	gpio_init(Serial_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, Serial_TX_PIN);  	  //PB10(TX)配置为复用推挽输出
	gpio_init(Serial_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, Serial_RX_PIN);  //PB11(RX)配置为浮空输入

	usart_deinit(USART2);					  			//复位USART2
	usart_baudrate_set(USART2, 115200);		  			//设置USART2波特率：
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);//使能USART2发送器
	usart_receive_config(USART2, USART_RECEIVE_ENABLE); //使能USART2接收器	

	dma_parameter_struct DMA_InitStruct;
	dma_deinit(DMA0, DMA_CH2);
	DMA_InitStruct.periph_addr  = (uint32_t)&USART_DATA(USART2);
	DMA_InitStruct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	DMA_InitStruct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	DMA_InitStruct.memory_addr  = (uint32_t)rx_dma_buf;
	DMA_InitStruct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	DMA_InitStruct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	DMA_InitStruct.direction    = DMA_PERIPHERAL_TO_MEMORY;
	DMA_InitStruct.number       = RX_DMA_LEN;
	DMA_InitStruct.priority     = DMA_PRIORITY_MEDIUM;
	dma_init(DMA0, DMA_CH2, &DMA_InitStruct);
	dma_circulation_enable(DMA0, DMA_CH2);
	usart_dma_receive_config(USART2, USART_DENR_ENABLE);
	dma_channel_enable(DMA0, DMA_CH2);
	
	dma_deinit(DMA0, DMA_CH1);
	DMA_InitStruct.periph_addr  = (uint32_t)&USART_DATA(USART2);
	DMA_InitStruct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	DMA_InitStruct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	DMA_InitStruct.memory_addr  = 0;
	DMA_InitStruct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	DMA_InitStruct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	DMA_InitStruct.direction    = DMA_MEMORY_TO_PERIPHERAL;
	DMA_InitStruct.number       = 0;
	DMA_InitStruct.priority     = DMA_PRIORITY_MEDIUM;
	dma_init(DMA0, DMA_CH1, &DMA_InitStruct);
	dma_circulation_disable(DMA0, DMA_CH1);
	usart_dma_transmit_config(USART2, USART_DENT_ENABLE);
	
	rx_dma_head = 0;
	
	nvic_irq_enable(USART2_IRQn,2,0);					//设置USART2 抢占优先级：1，响应优先级：1
	usart_interrupt_flag_clear(USART2, USART_INT_FLAG_IDLE);
	usart_interrupt_enable(USART2, USART_INT_IDLE);
	usart_enable(USART2);                 				//使能USART2 	        				//使能USART2 	
	
}

/**
 * @brief USART2空闲中断服务函数
 * @param none
 * @return none
 * @attention none
 */
void USART2_IRQHandler(void){
	if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE)){
		(void)usart_data_receive(USART2);
		usart_interrupt_flag_clear(USART2, USART_INT_FLAG_IDLE);
		Serial_ProcessCommand();
	}
}
/**
 * @brief 主函数串口读取
 * @param none
 * @return none
 * @attention none
 */
void Serial_ProcessCommand(void){
	uint16_t tail = (uint16_t)(RX_DMA_LEN - dma_transfer_number_get(DMA0, DMA_CH2)); 
	if(tail >= RX_DMA_LEN){
		tail = 0;
	}

	static uint16_t rx_index = 0;
	while(rx_dma_head != tail){//处理到的位置（读指针）！= 写到的最新位置（写指针）
		char c = (char)rx_dma_buf[rx_dma_head];//c为读到的位置的值
		rx_dma_head = (uint16_t)((rx_dma_head + 1) % RX_DMA_LEN);//读指针向后

		if(c == '\n' || rx_index >= RX_BUF_LEN - 1){//读到\n或者到队尾，截断buff
			rx_buf[rx_index] = '\0';
			rx_index = 0;	
			Serial_Parse(rx_buf);
		}else{//没读到尾巴，读入buff
			rx_buf[rx_index++] = c;
		}
	}
}

/**
  * @brief DMA发送
  * @param uint8_t *buf, uint16_t len
  * @return none
  * @attention none
  */
void Serial_DMA_Send(uint8_t *buf, uint16_t len){
	if(len == 0) return;

	dma_channel_disable(DMA0, DMA_CH1);
	dma_memory_address_config(DMA0, DMA_CH1, (uint32_t)buf);
	dma_transfer_number_config(DMA0, DMA_CH1, len);
	dma_flag_clear(DMA0, DMA_CH1, DMA_FLAG_G);
	dma_flag_clear(DMA0, DMA_CH1, DMA_FLAG_FTF);
	dma_channel_enable(DMA0, DMA_CH1);

	uint32_t timeout = 1000000;
	while(dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF) == RESET && timeout--);

	if(timeout == 0){
		dma_channel_disable(DMA0, DMA_CH1);
	}

	dma_flag_clear(DMA0, DMA_CH1, DMA_FLAG_FTF);
}
/**
  * @brief 发送一个字节
  * @param uint8_t Byte
  * @return none
  * @attention none
  */
void Serial_SendByte(uint8_t Byte){
	Serial_DMA_Send(&Byte, 1);
}

/**
  * @brief 发送一个数组
  * @param uint8_t *Array
  * @param uint16_t Length
  * @return none
  * @attention none
  */
void Serial_SendArray(uint8_t *Array,uint16_t Length){
	Serial_DMA_Send(Array, Length);
}

/**
  * @brief 发送一个字符串
  * @param char *String
  * @return none
  * @attention none
  */
void Serial_SendString(char *String){
	Serial_DMA_Send((uint8_t *)String, (uint16_t)strlen(String));
}

/**
  * @brief 发送一个数字
  * @param int32_t Number
  * @return none
  * @attention none
  */
void Serial_SendNumber(int32_t Number){
	char buf[12];
	snprintf(buf, sizeof(buf), "%ld", (long)Number);
	Serial_SendString(buf);
}

/**
  * @brief 发送一个浮点数
  * @param float Number
  * @return none
  * @attention none
  */
void Serial_SendFloatNumber(float Number){
	if (Number < 0.0f) 
	{
		Serial_SendByte('-');
		Number = -Number;
	}
			
	int32_t scaled = (int32_t)(Number * 100.0f + 0.5f);	
	Serial_SendNumber(scaled / 100);	
	Serial_SendByte('.');	
	uint8_t decimal = scaled % 100;
	if (decimal < 10) {
		Serial_SendByte('0');
	}
	Serial_SendNumber(decimal);
}

#ifndef _SERIAL_H
#define _SERIAL_H

#include "main.h"

#define Serial_TX_PIN   GPIO_PIN_10
#define Serial_RX_PIN   GPIO_PIN_11
#define Serial_PORT     GPIOB

void USART2_Init(void);
void Serial_ProcessCommand(void);

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(int32_t Number);
void Serial_SendFloatNumber(float Number);
	
#endif

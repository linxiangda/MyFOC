#ifndef _SERIAL_H
#define _SERIAL_H

#include "main.h"

void USART2_Init(void);
void Serial_ProcessCommand(void);
void Serial_Parse(char *str);

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(int32_t Number);
void Serial_SendFloatNumber(float Number);
	
#endif

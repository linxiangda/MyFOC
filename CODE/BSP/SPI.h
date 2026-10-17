#ifndef _SPI_H
#define _SPI_H

#include "main.h"
#define ENC_SPI         SPI0
#define DRV_SPI         SPI1
void SPI0_Init(void);
void SPI1_Init(void);
void SPI0_Start(void);
void SPI0_Stop(void);
uint16_t SPI0_SwapByte(uint16_t ByteSend);
void SPI1_Start(void);
void SPI1_Stop(void);
uint16_t SPI1_SwapByte(uint16_t ByteSend);
	
#endif

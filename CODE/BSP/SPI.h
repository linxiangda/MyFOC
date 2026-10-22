#ifndef _SPI_H
#define _SPI_H

#include "main.h"
#define ENC_SPI         SPI0
#define DRV_SPI         SPI1

#define ENC_SCK_PIN     GPIO_PIN_3
#define ENC_SCK_PORT    GPIOB
#define ENC_MISO_PIN    GPIO_PIN_4
#define ENC_MISO_PORT   GPIOB
#define ENC_MOSI_PIN    GPIO_PIN_5
#define ENC_MOSI_PORT   GPIOB
#define ENC_CS_PIN      GPIO_PIN_15
#define ENC_CS_PORT     GPIOA

//DRV
#define DRV_SCK_PIN     GPIO_PIN_13
#define DRV_SCK_PORT    GPIOB
#define DRV_MISO_PIN    GPIO_PIN_14
#define DRV_MISO_PORT   GPIOB
#define DRV_MOSI_PIN    GPIO_PIN_15
#define DRV_MOSI_PORT   GPIOB
#define DRV_CS_PIN      GPIO_PIN_12
#define DRV_CS_PORT     GPIOB

void SPI0_Init(void);
void SPI1_Init(void);
void SPI0_Start(void);
void SPI0_Stop(void);
uint16_t SPI0_SwapByte(uint16_t ByteSend);
void SPI1_Start(void);
void SPI1_Stop(void);
uint16_t SPI1_SwapByte(uint16_t ByteSend);
	
#endif

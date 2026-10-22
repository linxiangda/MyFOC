#ifndef _GPIO_H
#define _GPIO_H

#include "main.h"
void GPIO_Init(void);
//DRV
#define DRV_nFAULT_PIN  GPIO_PIN_2
#define DRV_nFAULT_PORT GPIOB
#define DRV_EN_PIN      GPIO_PIN_7
#define DRV_EN_PORT     GPIOB

void DRV_EN_reset(void);
void DRV_EN_set(void);
uint8_t DRV_nFAULT_get(void);
#endif

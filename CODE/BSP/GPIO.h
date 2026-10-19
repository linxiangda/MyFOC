#ifndef _GPIO_H
#define _GPIO_H

#include "main.h"

void GPIO_Init(void);
//串口引脚
#define Serial_TX_PIN   GPIO_PIN_10
#define Serial_RX_PIN   GPIO_PIN_11
#define Serial_PORT     GPIOB

//CAN引脚
#define CAN_TX_PIN   GPIO_PIN_12
#define CAN_RX_PIN   GPIO_PIN_11
#define CAN_PORT     GPIOA

//DRV使能引脚
#define DRV_nFAULT_PIN  GPIO_PIN_2
#define DRV_nFAULT_PORT GPIOB
#define DRV_EN_PIN      GPIO_PIN_7
#define DRV_EN_PORT     GPIOB

//PWM引脚
#define PWM_PHASE_A_PIN GPIO_PIN_10
#define PWM_PHASE_B_PIN GPIO_PIN_9
#define PWM_PHASE_C_PIN GPIO_PIN_8
#define PWM_PORT        GPIOA

//ADC引脚
#define ADC0_IN0_PIN    GPIO_PIN_0
#define ADC0_IN1_PIN    GPIO_PIN_1
#define ADC0_IN2_PIN    GPIO_PIN_2
#define ADC0_IN3_PIN    GPIO_PIN_3
#define ADC0_PORT       GPIOA

//编码器SPI引脚
#define ENC_SCK_PIN     GPIO_PIN_3
#define ENC_SCK_PORT    GPIOB
#define ENC_MISO_PIN    GPIO_PIN_4
#define ENC_MISO_PORT   GPIOB
#define ENC_MOSI_PIN    GPIO_PIN_5
#define ENC_MOSI_PORT   GPIOB
#define ENC_CS_PIN      GPIO_PIN_15
#define ENC_CS_PORT     GPIOA

//WS2812B引脚
#define WS2812B_PIN GPIO_PIN_8
#define WS2812B_PORT GPIOB

//DRV-SPI引脚
#define DRV_SCK_PIN     GPIO_PIN_13
#define DRV_SCK_PORT    GPIOB
#define DRV_MISO_PIN    GPIO_PIN_14
#define DRV_MISO_PORT   GPIOB
#define DRV_MOSI_PIN    GPIO_PIN_15
#define DRV_MOSI_PORT   GPIOB
#define DRV_CS_PIN      GPIO_PIN_12
#define DRV_CS_PORT     GPIOB
#endif

#ifndef _WS2812B_H
#define _WS2812B_H

#include "main.h"
#include "Control.h"
//WS2812B引脚
#define WS2812B_PIN GPIO_PIN_8
#define WS2812B_PORT GPIOB

#define LEDNUM          1
#define LEDBUFLEN       (LEDNUM * 24 + 240)//240*1.25=300

// 时序参数
#define HIBi            104      // 1码高电平
#define LWBi            20      // 0码高电平

// 常用颜色定义
#define COLOR_OFF       0x000000      // 关闭/黑色
#define COLOR_RED       0xFF0000      // 红色
#define COLOR_GREEN     0x00FF00      // 绿色  
#define COLOR_BLUE      0x0000FF      // 蓝色
#define COLOR_WHITE     0xFFFFFF      // 白色
#define COLOR_YELLOW    0xFFFF00      // 黄色
#define COLOR_CYAN      0x00FFFF      // 青色
#define COLOR_MAGENTA   0xFF00FF      // 品红
#define COLOR_ORANGE    0xFF8000      // 橙色
#define COLOR_PURPLE    0x800080      // 紫色
#define COLOR_PINK      0xFFC0CB      // 粉红

// 函数声明
void WS2812B_Init(void);
void WS2812B_SetColor(uint32_t color);
void WS2812B_Clear(void);
void WS2812B_SetRGB(uint8_t r, uint8_t g, uint8_t b);
void WS2812B_Update(void);
void RGB_Set(MOTOR_Mode motor_mode);
#endif

#ifndef __ADC_H__
#define __ADC_H__

//ADC
#define ADC0_IN0_PIN    GPIO_PIN_0
#define ADC0_IN1_PIN    GPIO_PIN_1
#define ADC0_IN2_PIN    GPIO_PIN_2
#define ADC0_IN3_PIN    GPIO_PIN_3
#define ADC0_PORT       GPIOA

void ADC0_Init(void);
void ADC_Calibrate(void);
void ADC_Get(float data[4]);

#endif


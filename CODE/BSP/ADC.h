#ifndef __ADC_H__
#define __ADC_H__

#include "main.h"

#define SHUNT_RESISTENCE	0.005f						
#define V_SCALE (19.0000f * 3.3f / 4095.0f)     					
#define I_SCALE (3.3f / 4095.0f) / SHUNT_RESISTENCE / 40.0f		

void ADC0_Init(void);
void ADC_Calibrate(void);
void ADC_Get(void);

#endif


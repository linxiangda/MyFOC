#ifndef _PWM_H
#define _PWM_H

#include "main.h"

//PWMÍ¨µÀ
#define PWM_TIMER       TIMER0
#define PWM_CHA         TIMER_CH_2      
#define PWM_CHB         TIMER_CH_1
#define PWM_CHC         TIMER_CH_0

void TIMER0_PWM_Init(void);
void Pwm_EnableOutput(void);
void Pwm_DisableOutput(void);
void Pwm_Set(float duty1,float duty2,float duty3);	


#endif

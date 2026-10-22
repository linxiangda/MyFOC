#ifndef _PWM_H
#define _PWM_H

#include "main.h"

//PWM
#define PWM_TIMER       TIMER0
#define PWM_CHA         TIMER_CH_2      
#define PWM_CHB         TIMER_CH_1
#define PWM_CHC         TIMER_CH_0

//PWM
#define PWM_PHASE_A_PIN GPIO_PIN_10
#define PWM_PHASE_B_PIN GPIO_PIN_9
#define PWM_PHASE_C_PIN GPIO_PIN_8
#define PWM_PORT        GPIOA

void TIMER0_PWM_Init(void);
void Pwm_EnableOutput(void);
void Pwm_DisableOutput(void);
void Pwm_Set(float voltage, float duty1, float duty2, float duty3);	
void Pwm_DataReset(void);

#endif

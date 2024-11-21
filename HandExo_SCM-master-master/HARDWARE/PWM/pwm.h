#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx.h"                  // Device header
	
void PWM_Init(u32 arr, u32 psc);

void motor_v(int v);

void TIM14_PWM_Init(uint32_t arr, uint32_t psc);

void PWM_SetCompareLow(void);

void PWM_SetCompareMid(void); 
	
void PWM_SetCompareHigh(void); 

void PWM_SetCompare( u8 ch,u16 count);

void TIM1_PWM_Init(void);

void Set_TIM1_PWM_DutyCycle(uint16_t channel1_duty, uint16_t channel2_duty, uint16_t channel3_duty);
#endif

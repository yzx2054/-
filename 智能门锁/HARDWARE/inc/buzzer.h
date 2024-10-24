#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "stm32f4xx.h"                  // Device header

void BUZZER_Init(void);
void BUZZER_PWM_Init(void);

void BUZZER_ON(void);
void BUZZER_OFF(void);
void BUZZER_PWM_Size(uint32_t size);

#endif


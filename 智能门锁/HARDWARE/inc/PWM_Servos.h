#ifndef __SERVOS_H__
#define __SERVOS_H__

#include "stm32f4xx.h"                  // Device header

void PWM_Servos_Init(void);
void Servo_SetAngle(float Angle);
void Servo_ON(void);
void Servo_OFF(void);


#endif


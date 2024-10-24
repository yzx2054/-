#include "PWM_Servos.h"


void PWM_Servos_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // ����GPIOE��TIM1��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // GPIOE14����Ϊ���ù���ģʽ������TIM1 CH4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // ����GPIOE14��TIM1 CH4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    // ��ʱ����������
    TIM_TimeBaseStructure.TIM_Period = 19999; // 20000 - 1 = 10ms������
    TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1; // 168MHz / 168 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // PWMģʽ����
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; // �м�λ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // ʹ�ܶ�ʱ����PWM���
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Servo_SetAngle(float Angle)
{
    // ���ƽǶȷ�Χ
    if(Angle < 0.0f) Angle = 0.0f;
    if(Angle > 180.0f) Angle = 180.0f;

    // ����PWMռ�ձ�
    uint16_t Compare = (uint16_t)((Angle / 180.0f * 2000.0f) + 500.0f);

    // ���ñȽ�ֵ�Ըı�PWMռ�ձ�
    TIM_SetCompare4(TIM1, Compare);
}

void Servo_ON(void)
{
	Servo_SetAngle(180.0f);
}

void Servo_OFF(void)
{
	Servo_SetAngle(0.0f);
}




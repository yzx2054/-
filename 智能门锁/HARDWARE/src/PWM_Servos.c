#include "PWM_Servos.h"


void PWM_Servos_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 开启GPIOE和TIM1的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // GPIOE14配置为复用功能模式，用于TIM1 CH4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // 连接GPIOE14到TIM1 CH4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

    // 定时器基本配置
    TIM_TimeBaseStructure.TIM_Period = 19999; // 20000 - 1 = 10ms的周期
    TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1; // 168MHz / 168 = 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // PWM模式配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; // 中间位置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    // 使能定时器和PWM输出
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void Servo_SetAngle(float Angle)
{
    // 限制角度范围
    if(Angle < 0.0f) Angle = 0.0f;
    if(Angle > 180.0f) Angle = 180.0f;

    // 计算PWM占空比
    uint16_t Compare = (uint16_t)((Angle / 180.0f * 2000.0f) + 500.0f);

    // 设置比较值以改变PWM占空比
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




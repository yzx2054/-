#include "buzzer.h"
#include "sys.h"


void BUZZER_Init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStruct;
	
	//1. 打开时钟开关，给GPIOF设备提供时钟信号，驱动工作
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);

	//配置GPIO引脚
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_OUT;										//输出模式
	GPIO_InitStruct.GPIO_OType	=	GPIO_OType_PP;										//推挽输出
	GPIO_InitStruct.GPIO_Pin		=	GPIO_Pin_8;												//8号引脚
	GPIO_InitStruct.GPIO_PuPd		=	GPIO_PuPd_NOPULL;									//浮空
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Low_Speed;										//低速输出， 速率越高， 设备响应速度越快， 功耗越高， 
	GPIO_Init( GPIOF, &GPIO_InitStruct);														//PF8引脚为输出模式
	
	
	//设置引脚输出的默认电平状态---设置设备默认工作状态---灯熄灭--高电平
	GPIO_ResetBits( GPIOF, GPIO_Pin_8);				//设置输出高电平
}


//可调大小
void BUZZER_PWM_Init(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStruct;

  /* TIM13 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

  /* GPIOF clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  /* GPIOC Configuration:TIM13 CH1 (PF8) */
  GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP ;
  GPIO_Init(GPIOF, &GPIO_InitStructure); 

  /* 将PF9引脚功能复用为定时13 */  
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
  
  /* 设置定时器的定时时间 -- 脉冲信号的周期10ms */
  TIM_TimeBaseStructure.TIM_Period      = 100;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Prescaler   = 8400-1;     //定时器的频率 = APB1 * 2 / TIM_Prescaler = 84000000 / 8400 =  10000HZ
                                                      //  10000HZ => 10000计数值1000000微秒   1个计数值100微妙
  TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

  /* 配置定时器14的通道1的参数 */
  TIM_OCInitStruct.TIM_OCMode         = TIM_OCMode_PWM1;        //输出比较模式--PWM1模式
  TIM_OCInitStruct.TIM_OutputState    = TIM_OutputState_Enable; //允许输出PWM脉冲信号
  TIM_OCInitStruct.TIM_Pulse          = 0;                      //比较值,初始化时不输出有效电平 -- 设备不要工作
  TIM_OCInitStruct.TIM_OCPolarity     = TIM_OCPolarity_Low;     //输出极性,有效状态设置
  TIM_OC1Init( TIM13, &TIM_OCInitStruct);
  
  TIM_Cmd( TIM13, ENABLE);

}

void BUZZER_ON(void)
{
	PFout(8) = 1;
}

void BUZZER_OFF(void)
{
	PFout(8) = 0;
}

//
void BUZZER_PWM_Size(uint32_t size)
{
	TIM_SetCompare1( TIM14, size);
}



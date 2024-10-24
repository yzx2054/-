#include "buzzer.h"
#include "sys.h"


void BUZZER_Init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStruct;
	
	//1. ��ʱ�ӿ��أ���GPIOF�豸�ṩʱ���źţ���������
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);

	//����GPIO����
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_OUT;										//���ģʽ
	GPIO_InitStruct.GPIO_OType	=	GPIO_OType_PP;										//�������
	GPIO_InitStruct.GPIO_Pin		=	GPIO_Pin_8;												//8������
	GPIO_InitStruct.GPIO_PuPd		=	GPIO_PuPd_NOPULL;									//����
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Low_Speed;										//��������� ����Խ�ߣ� �豸��Ӧ�ٶ�Խ�죬 ����Խ�ߣ� 
	GPIO_Init( GPIOF, &GPIO_InitStruct);														//PF8����Ϊ���ģʽ
	
	
	//�������������Ĭ�ϵ�ƽ״̬---�����豸Ĭ�Ϲ���״̬---��Ϩ��--�ߵ�ƽ
	GPIO_ResetBits( GPIOF, GPIO_Pin_8);				//��������ߵ�ƽ
}


//�ɵ���С
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

  /* ��PF9���Ź��ܸ���Ϊ��ʱ13 */  
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
  
  /* ���ö�ʱ���Ķ�ʱʱ�� -- �����źŵ�����10ms */
  TIM_TimeBaseStructure.TIM_Period      = 100;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Prescaler   = 8400-1;     //��ʱ����Ƶ�� = APB1 * 2 / TIM_Prescaler = 84000000 / 8400 =  10000HZ
                                                      //  10000HZ => 10000����ֵ1000000΢��   1������ֵ100΢��
  TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

  /* ���ö�ʱ��14��ͨ��1�Ĳ��� */
  TIM_OCInitStruct.TIM_OCMode         = TIM_OCMode_PWM1;        //����Ƚ�ģʽ--PWM1ģʽ
  TIM_OCInitStruct.TIM_OutputState    = TIM_OutputState_Enable; //�������PWM�����ź�
  TIM_OCInitStruct.TIM_Pulse          = 0;                      //�Ƚ�ֵ,��ʼ��ʱ�������Ч��ƽ -- �豸��Ҫ����
  TIM_OCInitStruct.TIM_OCPolarity     = TIM_OCPolarity_Low;     //�������,��Ч״̬����
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



#include "KEY.h"
#include "myincludes.h"
#include "fingerprint.h"



void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�򿪶˿�A��Ӳ��ʱ�ӣ�������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//��SYSCFGӲ��ʱ�ӣ����ǹ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//ָ����Ҫ���õ����� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	
//	//���������ӵ��ж���
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
//	
//	
//	//�ⲿ�ж�
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//�ⲿ�ж�0
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�жϴ���
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش��������ڼ�ⰴ���İ���
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ�ܹ���
//	EXTI_Init(&EXTI_InitStructure);//��ʼ��EXTI
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	//��ʼ���жϷ���
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0�жϺ�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//�ж���ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//�ж���Ӧ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//���ⲿ�ж�0����ͨ��
//	NVIC_Init(&NVIC_InitStructure);//��ʼ��NVIC
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	
}
//

uint8_t get_KEY_Num(void)
{
	//��ȡPA0���� -- ���¿���LED������
	if( KEY0 == 0)	//��������״̬ -- ����͵�ƽ
	{
		delay_ms(15);
		while( KEY0 == 0);
		return 0;
	}
	if( KEY1 == 0)	//��������״̬ -- ����͵�ƽ
	{
		delay_ms(15);
		while( KEY1 == 0);
		return 1;
	}
	if( KEY2 == 0)	//��������״̬ -- ����͵�ƽ
	{
		delay_ms(15);
		while( KEY2 == 0);
		return 2;
	}
	if( KEY3 == 0)	//��������״̬ -- ����͵�ƽ
	{
		delay_ms(15);
		while( KEY0 == 0);
		return 3;
	}
	
	return 9;
}


//void EXTI0_IRQHandler(void)
//{
//	uint32_t ulReturn;
//	
//	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	//����־λ
//	if(EXTI_GetITStatus(EXTI_Line0) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY0_DOWN,NULL);
//		
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//	/* �˳��ٽ�� */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}

//void EXTI2_IRQHandler(void)
//{

//	uint32_t ulReturn;
//	
//	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();		
//	
//	//����־λ
//	if(EXTI_GetITStatus(EXTI_Line2) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY1_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
//	/* �˳��ٽ�� */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}

//void EXTI3_IRQHandler(void)
//{
//	uint32_t ulReturn;
//	
//	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	
//	//����־λ
//	if(EXTI_GetITStatus(EXTI_Line3) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY2_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}
//	
//	/* �˳��ٽ�� */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}


//void EXTI4_IRQHandler(void)
//{
//	uint32_t ulReturn;
//	
//	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	
//	//����־λ
//	if(EXTI_GetITStatus(EXTI_Line4) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY3_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line4);
//	}
//	
//	/* �˳��ٽ�� */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
//}







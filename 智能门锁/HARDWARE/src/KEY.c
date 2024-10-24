#include "KEY.h"
#include "myincludes.h"
#include "fingerprint.h"



void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//打开端口A的硬件时钟，即供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//打开SYSCFG硬件时钟，就是供电
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//指定需要配置的引脚 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	
//	//将引脚连接到中断线
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
//	
//	
//	//外部中断
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//外部中断0
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断触发
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发，用于检测按键的按下
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能工作
//	EXTI_Init(&EXTI_InitStructure);//初始化EXTI
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
//	//初始化中断分组
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0中断号
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//中断抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;//中断响应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//打开外部中断0请求通道
//	NVIC_Init(&NVIC_InitStructure);//初始化NVIC
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
	//读取PA0引脚 -- 按下控制LED灯亮灭
	if( KEY0 == 0)	//按键按下状态 -- 输入低电平
	{
		delay_ms(15);
		while( KEY0 == 0);
		return 0;
	}
	if( KEY1 == 0)	//按键按下状态 -- 输入低电平
	{
		delay_ms(15);
		while( KEY1 == 0);
		return 1;
	}
	if( KEY2 == 0)	//按键按下状态 -- 输入低电平
	{
		delay_ms(15);
		while( KEY2 == 0);
		return 2;
	}
	if( KEY3 == 0)	//按键按下状态 -- 输入低电平
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
//	/* 进入临界段，临界段可以嵌套 */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	//检测标志位
//	if(EXTI_GetITStatus(EXTI_Line0) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY0_DOWN,NULL);
//		
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//	/* 退出临界段 */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}

//void EXTI2_IRQHandler(void)
//{

//	uint32_t ulReturn;
//	
//	/* 进入临界段，临界段可以嵌套 */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();		
//	
//	//检测标志位
//	if(EXTI_GetITStatus(EXTI_Line2) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY1_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
//	/* 退出临界段 */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}

//void EXTI3_IRQHandler(void)
//{
//	uint32_t ulReturn;
//	
//	/* 进入临界段，临界段可以嵌套 */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	
//	//检测标志位
//	if(EXTI_GetITStatus(EXTI_Line3) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY2_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}
//	
//	/* 退出临界段 */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
//}


//void EXTI4_IRQHandler(void)
//{
//	uint32_t ulReturn;
//	
//	/* 进入临界段，临界段可以嵌套 */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
//	
//	
//	//检测标志位
//	if(EXTI_GetITStatus(EXTI_Line4) == SET)
//	{
//		xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_KEY3_DOWN,NULL);
//		EXTI_ClearITPendingBit(EXTI_Line4);
//	}
//	
//	/* 退出临界段 */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
//}









#include "exti.h"
#include "key.h"
#include "delay.h"
#include "fingerprint.h"
#include "oled.h"
#include "myincludes.h"
#include "myprintf.h"



void EXTIX_finger_Init(uint8_t data)
{__stdout.USARTx = USART1;
    EXTI_InitTypeDef EXTI_InitStructure;
    if (FINGER_INT == 0)  // 触摸感应后产生低电平
    {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
        if (data == ENABLE)
        {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
            SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);  // WEL_OUTPUT = PA1 指纹感应输出
            EXTI_InitStructure.EXTI_Line = EXTI_Line1;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;
            EXTI_Init(&EXTI_InitStructure);
        }
        else
        {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
            SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);  // WEL_OUTPUT = PA1 指纹感应输出
            EXTI_InitStructure.EXTI_Line = EXTI_Line1;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
            EXTI_InitStructure.EXTI_LineCmd = DISABLE;
            EXTI_Init(&EXTI_InitStructure);
        }
    }
    else  // 触摸感应后产生高电平
    {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
        if (data == ENABLE)
        {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
            SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);  // WEL_OUTPUT = PA1 指纹感应输出
            EXTI_InitStructure.EXTI_Line = EXTI_Line1;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;
            EXTI_Init(&EXTI_InitStructure);
        }
        else
        {printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
            SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);  // WEL_OUTPUT = PA1 指纹感应输出
            EXTI_InitStructure.EXTI_Line = EXTI_Line1;
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
            EXTI_InitStructure.EXTI_LineCmd = DISABLE;
            EXTI_Init(&EXTI_InitStructure);
        }
    }printf("%s-%s-%d\r\n", __FILE__, __FUNCTION__, __LINE__);
}

void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  // 使能SYSCFG时钟

    // 配置 PA1 (指纹感应 WEL_OUTPUT) 引脚的外部中断
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
	
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = (FINGER_INT == 0) ? EXTI_Trigger_Falling : EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC 中断优先级和使能中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;  // PA1外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 配置其他引脚的外部中断（类似的配置）
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//    EXTI_Init(&EXTI_InitStructure);

//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
//    EXTI_Init(&EXTI_InitStructure);

//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_Init(&EXTI_InitStructure);

//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
//    EXTI_Init(&EXTI_InitStructure);

//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC 中断优先级和使能其他外部中断
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//    NVIC_Init(&NVIC_InitStructure);

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//    NVIC_Init(&NVIC_InitStructure);

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//    NVIC_Init(&NVIC_InitStructure);

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//    NVIC_Init(&NVIC_InitStructure);
}

//-----------------------------------------------------------------------------------
//void EXTI0_IRQHandler(void)  
//{
//			if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//		  {	
//	         EXTI_ClearITPendingBit(EXTI_Line0);
//			}							
//} 

void EXTI1_IRQHandler(void)  //PA1 指纹感应 WEL_OUTPUT
{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)
		{	
				EXTI_ClearITPendingBit(EXTI_Line1);	  
				if(MACHINE_STAT1.field.fringer_power_on==TURN_OFF)
				{
						if(FINGER_INT==0)//触摸感应后产生低电平
						{
								if( WEL_OUTPUT == 0 )
								{
										 delay_ms(10);
										 if( WEL_OUTPUT == 0 )
										 {
													MACHINE_STAT1.field.fringer_first_send=1;	//第一次按下			
													MACHINE_STAT1.field.wait_key_flag=0;//清空超时定时器
												
										 }
								}
							
						}
            else
						{						
								if( WEL_OUTPUT == 1 )
								{
										 delay_ms(10);
										 if( WEL_OUTPUT == 1 )
										 {
													MACHINE_STAT1.field.fringer_first_send=1;	//第一次按下			
													MACHINE_STAT1.field.wait_key_flag=0;//清空超时定时器
												
										 }
								}
					  }
				}
		}
} 
//void EXTI2_IRQHandler(void) 
//{
//		if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//		{	
//       EXTI_ClearITPendingBit(EXTI_Line2);	  
//		}
//	  
//} 

//void EXTI3_IRQHandler(void)  
//{
//		if(EXTI_GetITStatus(EXTI_Line3) != RESET)
//		{
//        EXTI_ClearITPendingBit(EXTI_Line3);				
//		}	
//	
//} 


//void EXTI4_IRQHandler(void)  
//{
//		if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//		{
//        EXTI_ClearITPendingBit(EXTI_Line4);			
//		}

//} 
void EXTI9_5_IRQHandler(void) 
{
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line5);  
		}
		if(EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line6);  
		}
		if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line7);  
		}
		if(EXTI_GetITStatus(EXTI_Line8) != RESET)//MODULE_CTL
		{
          EXTI_ClearITPendingBit(EXTI_Line8); 		
		}
		if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line9);  
		} 
}
//==========================================================================================================
void EXTI15_10_IRQHandler(void) 
{
		if(EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line10);  
		}
		if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line11);  
		}
		if(EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line12);  
		}
		if(EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line13);	
		}
		if(EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line14);  
		}
		if(EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
          EXTI_ClearITPendingBit(EXTI_Line15);  
		}		
}





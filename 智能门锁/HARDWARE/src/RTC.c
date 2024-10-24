#include "RTC.h"
#include "myincludes.h"


RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

//BCD码转DEC，二进制转十进制
uint8_t BCD2DEC(uint8_t bcd)  
{  
    return (bcd-(bcd>>4)*6);
} 

//DEC转BCD，十进制转二进制
uint8_t DEC2BCD(uint8_t dec)  
{  
    return (dec+(dec/10)*6);
} 


void RTC_Config(void)
{
		RTC_InitTypeDef RTC_InitStruct;
   
    if( RTC_ReadBackupRegister( RTC_BKP_DR0) != 0x7F)
    {
    
        //使能RTC时钟
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);

        //关闭RTC寄存器的写保护功能
        PWR_BackupAccessCmd( ENABLE);

        //选择RTC的运行时钟 -- LSE晶振 32768HZ
        RCC_LSEConfig( RCC_LSE_ON);                                     //启动LSE时钟源
        while( RCC_GetFlagStatus( RCC_FLAG_LSERDY) != SET);             //等待LSE时钟源启动稳定
        RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE);                        //设置RTC使用LSE时钟

        //使能RTC的时钟配置
        RCC_RTCCLKCmd( ENABLE);

        //等待RTC寄存器同步频率
        RTC_WaitForSynchro();

        //初始化RTC， 降频后RTC计数频率1Hz， 并且使用24小时制
        RTC_InitStruct.RTC_SynchPrediv      =   255;                    //同步预分频值 ，自动+1 ， 实际设置256
        RTC_InitStruct.RTC_AsynchPrediv     =   127;                    //异步预分频值 ，自动+1 ， 实际设置128
        RTC_InitStruct.RTC_HourFormat       =   RTC_HourFormat_24;      //24小时进制
        RTC_Init( &RTC_InitStruct);

        //设置日期与时间
        RTC_TimeStruct.RTC_H12      =    RTC_H12_AM;   //12小时进制
        RTC_TimeStruct.RTC_Hours    =    17;           //17点
        RTC_TimeStruct.RTC_Minutes  =    52;           //分钟
        RTC_TimeStruct.RTC_Seconds  =    00;           //秒
        RTC_SetTime( RTC_Format_BIN, &RTC_TimeStruct);
            
        RTC_DateStruct.RTC_Year     =   24;                     	 //年
        RTC_DateStruct.RTC_Month    =   RTC_Month_October;    		 //月    
        RTC_DateStruct.RTC_Date     =   18;                     	 //日
        RTC_DateStruct.RTC_WeekDay  =   RTC_Weekday_Saturday ;     //星期
        RTC_SetDate( RTC_Format_BIN, &RTC_DateStruct);
        
        RTC_WriteBackupRegister( RTC_BKP_DR0, 0x7F);
    }
}


void RTC_get_time(void)
{

	RTC_GetDate( RTC_Format_BIN, &RTC_DateStruct);
  RTC_GetTime( RTC_Format_BIN, &RTC_TimeStruct);
//  printf("%d-%d-%d  %d:%d:%d\n",  RTC_DateStruct.RTC_Year,
//                                  RTC_DateStruct.RTC_Month,
//                                  RTC_DateStruct.RTC_Date,
//                                  RTC_TimeStruct.RTC_Hours,
//                                  RTC_TimeStruct.RTC_Minutes,
//                                  RTC_TimeStruct.RTC_Seconds);


}



void RTC_WKUP_IRQHandler(void)
{
//		uint32_t ulReturn;
//		/* 进入临界段，临界段可以嵌套 */
//		ulReturn = taskENTER_CRITICAL_FROM_ISR();

//		if(RTC_GetITStatus(RTC_IT_WUT) == SET)
//		{
//			xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_RTC_WAKEUP,NULL);
//			RTC_ClearITPendingBit(RTC_IT_WUT);
//			EXTI_ClearITPendingBit(EXTI_Line22);
//		}
//		/* 退出临界段 */
//		taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

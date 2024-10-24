#include "RTC.h"
#include "myincludes.h"


RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

//BCD��תDEC��������תʮ����
uint8_t BCD2DEC(uint8_t bcd)  
{  
    return (bcd-(bcd>>4)*6);
} 

//DECתBCD��ʮ����ת������
uint8_t DEC2BCD(uint8_t dec)  
{  
    return (dec+(dec/10)*6);
} 


void RTC_Config(void)
{
		RTC_InitTypeDef RTC_InitStruct;
   
    if( RTC_ReadBackupRegister( RTC_BKP_DR0) != 0x7F)
    {
    
        //ʹ��RTCʱ��
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);

        //�ر�RTC�Ĵ�����д��������
        PWR_BackupAccessCmd( ENABLE);

        //ѡ��RTC������ʱ�� -- LSE���� 32768HZ
        RCC_LSEConfig( RCC_LSE_ON);                                     //����LSEʱ��Դ
        while( RCC_GetFlagStatus( RCC_FLAG_LSERDY) != SET);             //�ȴ�LSEʱ��Դ�����ȶ�
        RCC_RTCCLKConfig( RCC_RTCCLKSource_LSE);                        //����RTCʹ��LSEʱ��

        //ʹ��RTC��ʱ������
        RCC_RTCCLKCmd( ENABLE);

        //�ȴ�RTC�Ĵ���ͬ��Ƶ��
        RTC_WaitForSynchro();

        //��ʼ��RTC�� ��Ƶ��RTC����Ƶ��1Hz�� ����ʹ��24Сʱ��
        RTC_InitStruct.RTC_SynchPrediv      =   255;                    //ͬ��Ԥ��Ƶֵ ���Զ�+1 �� ʵ������256
        RTC_InitStruct.RTC_AsynchPrediv     =   127;                    //�첽Ԥ��Ƶֵ ���Զ�+1 �� ʵ������128
        RTC_InitStruct.RTC_HourFormat       =   RTC_HourFormat_24;      //24Сʱ����
        RTC_Init( &RTC_InitStruct);

        //����������ʱ��
        RTC_TimeStruct.RTC_H12      =    RTC_H12_AM;   //12Сʱ����
        RTC_TimeStruct.RTC_Hours    =    17;           //17��
        RTC_TimeStruct.RTC_Minutes  =    52;           //����
        RTC_TimeStruct.RTC_Seconds  =    00;           //��
        RTC_SetTime( RTC_Format_BIN, &RTC_TimeStruct);
            
        RTC_DateStruct.RTC_Year     =   24;                     	 //��
        RTC_DateStruct.RTC_Month    =   RTC_Month_October;    		 //��    
        RTC_DateStruct.RTC_Date     =   18;                     	 //��
        RTC_DateStruct.RTC_WeekDay  =   RTC_Weekday_Saturday ;     //����
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
//		/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//		ulReturn = taskENTER_CRITICAL_FROM_ISR();

//		if(RTC_GetITStatus(RTC_IT_WUT) == SET)
//		{
//			xEventGroupSetBitsFromISR(g_event_group1_handle,EVENT_GROUP_RTC_WAKEUP,NULL);
//			RTC_ClearITPendingBit(RTC_IT_WUT);
//			EXTI_ClearITPendingBit(EXTI_Line22);
//		}
//		/* �˳��ٽ�� */
//		taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

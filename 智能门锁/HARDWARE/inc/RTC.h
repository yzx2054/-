#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f4xx.h"                  // Device header

#define EVENT_GROUP_RTC_WAKEUP		0x20

//BCD��תDEC��������תʮ����
uint8_t BCD2DEC(uint8_t bcd);
//DECתBCD��ʮ����ת������
uint8_t DEC2BCD(uint8_t dec);

extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;


void RTC_Config(void);
void RTC_get_time(void);
void RTC_WKUP_IRQHandler(void);

#endif


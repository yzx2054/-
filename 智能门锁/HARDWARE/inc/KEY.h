#ifndef _KEY_H_
#define	_KEY_H_

#include "stm32f4xx.h"                  // Device header


#define EVENT_GROUP_KEY0_DOWN		0x01//短按
#define EVENT_GROUP_KEY1_DOWN		0x02//短按
#define EVENT_GROUP_KEY2_DOWN		0x04//短按
#define EVENT_GROUP_KEY3_DOWN		0x08//短按
#define EVENT_GROUP_KEYALL_DOWN		0x0F//短按


#define KEY0 PAin(0)	
#define KEY1 PEin(2)	
#define KEY2 PEin(3)	
#define KEY3 PEin(4)	

//#define  KEY_SETD 	    PAin(0)//设置键
//#define  KEY_UPD 	      PEin(2)//+键
//#define  KEY_DOWND 	    PEin(3)//-键
//#define  KEY_BACKD 	    PEin(4) //返回键

#define  KEY_LONG_PERIOD      150 
#define  KEY_CONTINUE_PERIOD  2 
#define  KEY_DOWN             0x80 
#define  KEY_LONG             0x40 
#define  KEY_CONTINUE         0x20 
#define  KEY_UP               0x10 //定义按键状态
#define  KEY_STATE_INIT       0 
#define  KEY_STATE_WOBBLE     1 
#define  KEY_STATE_PRESS      2 
#define  KEY_STATE_LONG       3 
#define  KEY_STATE_CONTINUE   4 
#define  KEY_STATE_RELEASE    5 
#define  KEY_NULL          0xff


extern void KEY_Init(void);
uint8_t get_KEY_Num(void);
extern void TIM4_Init(void);

#endif


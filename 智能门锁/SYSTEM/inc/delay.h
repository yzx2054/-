#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f4xx.h"
#include "stdint.h"
  

/**
 *  @berif  ��ʼ����ʱ����
 *	@param  SYSCLK: ϵͳ����Ƶ��   168: 168Mhz  
 *
 ****/
void delay_init( int SYSCLK);

int delay_us( int nus);
int delay_ms( int nms);
int delay_s ( int ns);



#endif

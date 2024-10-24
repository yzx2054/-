#include "delay.h"

static u8 fac_us = 0;	//保存1us需要使用的计数值
static u16 fac_ms = 0;	//保存1ms需要使用的计数值  

/**
 *  @berif  初始化延时函数
 *	@param  SYSCLK: 系统运行频率   168: 168Mhz  
 *
 ****/
void delay_init( int SYSCLK)
{
	//设置系统定时器的时钟源 -- AHB的8分频作为时钟源 -- 21MHz
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK_Div8);
	
	//计算出1us和1ms定时时间的计数值
	fac_us = SYSCLK / 8;		// 1us的计数值   168 / 8  = 21;
	fac_ms = fac_us * 1000;
}


int delay_us( int nus)	
{
#if 0
	uint32_t temp;
	
	SysTick->CTRL  = 0;											 //关闭系统定时器
	SysTick->LOAD  = fac_us * nus - 1;			 //设置定时的计数值
	SysTick->VAL   = 0;											 //清空计数器的值,并设置COUNTFLAG标志位为0(标记定时未完成的状态)
	SysTick->CTRL  = 1;											 //启动定时器工作,并设置系统时钟源为21MHz, 不产生中断
	//while( (SysTick->CTRL & (1<<16)) == 0);//等待定时器定时完成
  while(1)
	{
		//获取出CTRL寄存器的值
		temp = SysTick->CTRL;
	
		//判断定时器是否定时完成
		if( temp & (1<<16) )
			break;
		
		//在定时未完成的情况下, 定时器被意外关闭
		if( (temp & 0x01) == 0)
			return -1;
	}
	SysTick->CTRL  = 0;											//关闭系统定时器
#else

	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;											//关闭系统定时器
	SysTick->LOAD  = fac_us * nus - 1;															//设置定时的计数值
	SysTick->VAL   = 0;																							//清空计数器的值,并设置COUNTFLAG标志位为0(标记定时未完成的状态)
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;											//启动定时器工作,并设置系统时钟源为21MHz, 不产生中断
  while( (SysTick->CTRL & (1<<16)) == 0);													//等待COUNTFLAG标志为1,如果不为1则循环等待( 等待定时器定时完成)
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;											//关闭系统定时器
	
#endif

	return 0;
}

int delay_ms( int nms)
{
	
#if 0	
	uint32_t temp;
	
	SysTick->CTRL  = 0;												//关闭系统定时器
	while( nms--)															//有误差范围(几微秒)
	{
		SysTick->LOAD  = fac_ms - 1;						//设置定时的计数值
		SysTick->VAL   = 0;											//清空计数器的值,并设置COUNTFLAG标志位为0(标记定时未完成的状态)
		SysTick->CTRL  = 1;											//启动定时器工作,并设置系统时钟源为21MHz, 不产生中断
		//while( (SysTick->CTRL & (1<<16)) == 0);	//等待COUNTFLAG标志为1,如果不为1则循环等待( 等待定时器定时完成)
		while(1)
		{
			//获取出CTRL寄存器的值
			temp = SysTick->CTRL;

			//判断定时器是否定时完成
			if( temp & (1<<16) )
			break;

			//在定时未完成的情况下, 定时器被意外关闭
			if( (temp & 0x01) == 0)
			return -1;
		}
		SysTick->CTRL  = 0;											//关闭系统定时器
	}
#else
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;												//关闭系统定时器
	while( nms--)																											//有误差范围(几微秒)
	{
		SysTick->LOAD  = fac_ms - 1;																		//设置定时的计数值
		SysTick->VAL   = 0;																							//清空计数器的值,并设置COUNTFLAG标志位为0(标记定时未完成的状态)
		SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;											//启动定时器工作,并设置系统时钟源为21MHz, 不产生中断
		while( (SysTick->CTRL & (1<<16)) == 0);													//等待COUNTFLAG标志为1,如果不为1则循环等待( 等待定时器定时完成)
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;											//关闭系统定时器
	}
#endif
	
	return 0;
}


int delay_s ( int ns)
{
	for( int i = 0; i < ns; i++) 
		delay_ms( 1000);
	
	return 0;
}


#include "delay.h"

static u8 fac_us = 0;	//����1us��Ҫʹ�õļ���ֵ
static u16 fac_ms = 0;	//����1ms��Ҫʹ�õļ���ֵ  

/**
 *  @berif  ��ʼ����ʱ����
 *	@param  SYSCLK: ϵͳ����Ƶ��   168: 168Mhz  
 *
 ****/
void delay_init( int SYSCLK)
{
	//����ϵͳ��ʱ����ʱ��Դ -- AHB��8��Ƶ��Ϊʱ��Դ -- 21MHz
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK_Div8);
	
	//�����1us��1ms��ʱʱ��ļ���ֵ
	fac_us = SYSCLK / 8;		// 1us�ļ���ֵ   168 / 8  = 21;
	fac_ms = fac_us * 1000;
}


int delay_us( int nus)	
{
#if 0
	uint32_t temp;
	
	SysTick->CTRL  = 0;											 //�ر�ϵͳ��ʱ��
	SysTick->LOAD  = fac_us * nus - 1;			 //���ö�ʱ�ļ���ֵ
	SysTick->VAL   = 0;											 //��ռ�������ֵ,������COUNTFLAG��־λΪ0(��Ƕ�ʱδ��ɵ�״̬)
	SysTick->CTRL  = 1;											 //������ʱ������,������ϵͳʱ��ԴΪ21MHz, �������ж�
	//while( (SysTick->CTRL & (1<<16)) == 0);//�ȴ���ʱ����ʱ���
  while(1)
	{
		//��ȡ��CTRL�Ĵ�����ֵ
		temp = SysTick->CTRL;
	
		//�ж϶�ʱ���Ƿ�ʱ���
		if( temp & (1<<16) )
			break;
		
		//�ڶ�ʱδ��ɵ������, ��ʱ��������ر�
		if( (temp & 0x01) == 0)
			return -1;
	}
	SysTick->CTRL  = 0;											//�ر�ϵͳ��ʱ��
#else

	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;											//�ر�ϵͳ��ʱ��
	SysTick->LOAD  = fac_us * nus - 1;															//���ö�ʱ�ļ���ֵ
	SysTick->VAL   = 0;																							//��ռ�������ֵ,������COUNTFLAG��־λΪ0(��Ƕ�ʱδ��ɵ�״̬)
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;											//������ʱ������,������ϵͳʱ��ԴΪ21MHz, �������ж�
  while( (SysTick->CTRL & (1<<16)) == 0);													//�ȴ�COUNTFLAG��־Ϊ1,�����Ϊ1��ѭ���ȴ�( �ȴ���ʱ����ʱ���)
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;											//�ر�ϵͳ��ʱ��
	
#endif

	return 0;
}

int delay_ms( int nms)
{
	
#if 0	
	uint32_t temp;
	
	SysTick->CTRL  = 0;												//�ر�ϵͳ��ʱ��
	while( nms--)															//����Χ(��΢��)
	{
		SysTick->LOAD  = fac_ms - 1;						//���ö�ʱ�ļ���ֵ
		SysTick->VAL   = 0;											//��ռ�������ֵ,������COUNTFLAG��־λΪ0(��Ƕ�ʱδ��ɵ�״̬)
		SysTick->CTRL  = 1;											//������ʱ������,������ϵͳʱ��ԴΪ21MHz, �������ж�
		//while( (SysTick->CTRL & (1<<16)) == 0);	//�ȴ�COUNTFLAG��־Ϊ1,�����Ϊ1��ѭ���ȴ�( �ȴ���ʱ����ʱ���)
		while(1)
		{
			//��ȡ��CTRL�Ĵ�����ֵ
			temp = SysTick->CTRL;

			//�ж϶�ʱ���Ƿ�ʱ���
			if( temp & (1<<16) )
			break;

			//�ڶ�ʱδ��ɵ������, ��ʱ��������ر�
			if( (temp & 0x01) == 0)
			return -1;
		}
		SysTick->CTRL  = 0;											//�ر�ϵͳ��ʱ��
	}
#else
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;												//�ر�ϵͳ��ʱ��
	while( nms--)																											//����Χ(��΢��)
	{
		SysTick->LOAD  = fac_ms - 1;																		//���ö�ʱ�ļ���ֵ
		SysTick->VAL   = 0;																							//��ռ�������ֵ,������COUNTFLAG��־λΪ0(��Ƕ�ʱδ��ɵ�״̬)
		SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;											//������ʱ������,������ϵͳʱ��ԴΪ21MHz, �������ж�
		while( (SysTick->CTRL & (1<<16)) == 0);													//�ȴ�COUNTFLAG��־Ϊ1,�����Ϊ1��ѭ���ȴ�( �ȴ���ʱ����ʱ���)
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;											//�ر�ϵͳ��ʱ��
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


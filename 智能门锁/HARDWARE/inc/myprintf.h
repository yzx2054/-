#ifndef __MPRINTF_H__
#define __MPRINTF_H__

#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"


#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE 
{ 
	int handle; /* Add whatever you need here */ 
	USART_TypeDef* USARTx;  // ���浱ǰ USART �ӿ�
	
	//**** -> �ɸ�������׷�ӳ�Ա
};



//����printf����
int fputc(int ch, FILE *f);
// �Զ�����пɱ�����Ĵ�ӡ����
void custom_print(const char *format, ...);

#endif


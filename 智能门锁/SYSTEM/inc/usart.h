#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"                  // Device header

#define USART2_MAX_RECV_LEN		200				//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		200				//最大发送缓存字节数

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	
extern u8  usart2_rx_flag; 
extern u8  usart2_rx_count; 

extern volatile uint8_t  	g_USART1_recv_buf[64];
extern volatile uint32_t 	g_USART1_recv_cnt;

//串口1初始化
//baud: 波特率
void USART1_Init(uint32_t baud);
//串口2初始化
//baud: 波特率
void USART2_Init(uint32_t baud);

void USART2_DisInit(void);
//串口3初始化
//baud: 波特率
void USART3_Init( uint32_t baud);



//串口发送函数
void USART_Send(USART_TypeDef* USARTx, char *str);

void usart_send_bytes(USART_TypeDef* USARTx,uint8_t *buf,uint32_t len);
//串口数据包发送
void USART_SendByte( USART_TypeDef *USARTx, uint8_t *src, uint32_t len);

void Uart2_Clearbuf(void);

#endif


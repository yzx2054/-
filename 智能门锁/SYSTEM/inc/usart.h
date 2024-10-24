#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"                  // Device header

#define USART2_MAX_RECV_LEN		200				//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		200				//����ͻ����ֽ���

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	
extern u8  usart2_rx_flag; 
extern u8  usart2_rx_count; 

extern volatile uint8_t  	g_USART1_recv_buf[64];
extern volatile uint32_t 	g_USART1_recv_cnt;

//����1��ʼ��
//baud: ������
void USART1_Init(uint32_t baud);
//����2��ʼ��
//baud: ������
void USART2_Init(uint32_t baud);

void USART2_DisInit(void);
//����3��ʼ��
//baud: ������
void USART3_Init( uint32_t baud);



//���ڷ��ͺ���
void USART_Send(USART_TypeDef* USARTx, char *str);

void usart_send_bytes(USART_TypeDef* USARTx,uint8_t *buf,uint32_t len);
//�������ݰ�����
void USART_SendByte( USART_TypeDef *USARTx, uint8_t *src, uint32_t len);

void Uart2_Clearbuf(void);

#endif


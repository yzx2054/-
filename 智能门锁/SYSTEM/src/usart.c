#include "usart.h"
#include "fingerprint.h"
#include "myincludes.h"
#include "ESP8266.h"

volatile 				uint8_t 	g_USART1_recv_buf[64]={0};
static volatile uint32_t  g_USART1_recv_cnt = 0;

volatile 				uint8_t 	g_USART2_recv_buf[64]={0};
static volatile uint32_t  g_USART2_recv_cnt = 0;

u8  usart2_rx_count = 0; 
u8  usart2_rx_flag  = 0; 
u16 USART2_RX_STA   = 0;    //����״̬���
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 

//����1��ʼ��
//baud: ������
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	//ʹ�ܴ���1��ʱ��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
  
  //ʹ��GPIOʱ��( ʹ�ܴ���TXD��RXD����)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
  
  //�������ŵĸ��ù���
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource9,  GPIO_AF_USART1); //USART1_TX
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //USART1_RX

  //��ʼ�����ŵĹ���
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOA, &GPIO_InitStruct);
  
  //���ô��ڲ��� -- �������õĲ�������Ҫ��Զ��豸���ڲ���һ��
  USART_InitStruct.USART_BaudRate             = baud;   //������, ���õĲ�����:4800, 9600, 19200,115200
                                                        //���ô������ݰ����������,��: 9600 - 1���ӷ���9600bit������   1��bit��Ҫʹ��104.17us
                                                        //ʱ����Ҫ����ϵͳʱ�ӽ��м��㣬 ϵͳʱ��(PLL_M, HSE_VALUE)����, 1��bit����Ҫ��ʱ��ͻ᲻׼ȷ, �Է��豸�Ͳ���׼ȷ�Ľ��յ���Ϣ
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //Ӳ���������� ��Ҫʹ��CTS�� RTS���ſ��ƣ� ����USART�ӿ�û������������
                                                                                        //��ʹ��Ӳ��������
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //�������ڵķ�������յĹ���
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //У��ģʽ����Ҫ����
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //����ֹͣλ1λ
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8λ����λ
  USART_Init( USART1, &USART_InitStruct);


  //���ý����ж�
  USART_ITConfig( USART1, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;        //����1�ж�ͨ��
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //��Ӧ���ȼ�
  NVIC_Init( &NVIC_InitStruct);
  
  //�������ڹ���
  USART_Cmd( USART1, ENABLE);
}

//����2��ʼ��
//baud: ������
void USART2_Init( uint32_t baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef  NVIC_InitStruct;
  
  //ʹ�ܴ���2��ʱ��
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
  
  //ʹ��GPIOʱ��( ʹ�ܴ���TXD��RXD����)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
  
  //�������ŵĸ��ù���
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //USART2_TX
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //USART2_RX

  //��ʼ�����ŵĹ���
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOA, &GPIO_InitStruct);
  
  //���ô��ڲ��� -- �������õĲ�������Ҫ��Զ��豸���ڲ���һ��
  USART_InitStruct.USART_BaudRate             = baud;   //������, ���õĲ�����:4800, 9600, 19200,115200
                                                        //���ô������ݰ����������,��: 9600 - 1���ӷ���9600bit������   1��bit��Ҫʹ��104.17us
                                                        //ʱ����Ҫ����ϵͳʱ�ӽ��м��㣬 ϵͳʱ��(PLL_M, HSE_VALUE)����, 1��bit����Ҫ��ʱ��ͻ᲻׼ȷ, �Է��豸�Ͳ���׼ȷ�Ľ��յ���Ϣ
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //Ӳ���������� ��Ҫʹ��CTS�� RTS���ſ��ƣ� ����USART�ӿ�û������������
                                                                                        //��ʹ��Ӳ��������
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //�������ڵķ�������յĹ���
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //У��ģʽ����Ҫ����
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //����ֹͣλ1λ
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8λ����λ
  USART_Init( USART2, &USART_InitStruct);


  //���ý����ж�
  USART_ITConfig( USART2, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART2_IRQn;        //����1�ж�ͨ��
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //��Ӧ���ȼ�
  NVIC_Init( &NVIC_InitStruct);
  
  //�������ڹ���
  USART_Cmd( USART2, ENABLE);
}

void USART2_DisInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //�������
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    PAout(2)=0;
    PAout(3)=0;	
}

//����3��ʼ��
//baud: ������
void USART3_Init( uint32_t baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef  NVIC_InitStruct;
  
  //ʹ�ܴ���3��ʱ��
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
  
  //ʹ��GPIOʱ��( ʹ�ܴ���TXD��RXD����)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
  
  //�������ŵĸ��ù���
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //USART3_TX
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //USART3_RX

  //��ʼ�����ŵĹ���
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOB, &GPIO_InitStruct);
  
  //���ô��ڲ��� -- �������õĲ�������Ҫ��Զ��豸���ڲ���һ��
  USART_InitStruct.USART_BaudRate             = baud;   //������, ���õĲ�����:4800, 9600, 19200,115200
                                                        //���ô������ݰ����������,��: 9600 - 1���ӷ���9600bit������   1��bit��Ҫʹ��104.17us
                                                        //ʱ����Ҫ����ϵͳʱ�ӽ��м��㣬 ϵͳʱ��(PLL_M, HSE_VALUE)����, 1��bit����Ҫ��ʱ��ͻ᲻׼ȷ, �Է��豸�Ͳ���׼ȷ�Ľ��յ���Ϣ
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //Ӳ���������� ��Ҫʹ��CTS�� RTS���ſ��ƣ� ����USART�ӿ�û������������
                                                                                        //��ʹ��Ӳ��������
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //�������ڵķ�������յĹ���
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //У��ģʽ����Ҫ����
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //����ֹͣλ1λ
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8λ����λ
  USART_Init( USART3, &USART_InitStruct);


  //���ý����ж�
  USART_ITConfig( USART3, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART3_IRQn;        //����3�ж�ͨ��
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //��Ӧ���ȼ�
  NVIC_Init( &NVIC_InitStruct);
  
  //�������ڹ���
   USART_Cmd( USART3, ENABLE);
}



//���ô���1���жϷ�����
void USART1_IRQHandler(void)
{
  //���ڴ����豸�ж���ж�Դ���Դ����жϷ�����,������Ҫ�ж��Ƿ��ĸ��ж�Դ����
  
  //�жϴ���1�ж��Ƿ��� �����ж� ������
  if( USART_GetITStatus( USART1,USART_IT_RXNE) == SET)
  {
    /******TODO******/
//		uint8_t dat = USART_ReceiveData( USART1);
    g_USART1_recv_buf[g_USART1_recv_cnt++] = USART_ReceiveData( USART1);
//    USART_SendData( USART1, dat);
    if(g_USART1_recv_buf[g_USART1_recv_cnt-1] == '#' || g_USART1_recv_cnt >= (sizeof g_USART1_recv_buf))
		{
			g_USART1_recv_cnt = 0;
		}
    USART_ClearITPendingBit( USART1,USART_IT_RXNE);
  }

}


/**
	* @brief	����2�жϷ�����
	* @param 	None
	* @return None
	*/
//���ô���2���жϷ�����
//void USART2_IRQHandler(void)
//{
//	static uint8_t d=0;
//	
//	//����Ƿ���յ�����
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		d=USART_ReceiveData(USART2);
//		
//		if(g_USART2_recv_cnt< sizeof(g_USART2_recv_buf))
//			g_USART2_recv_buf[g_USART2_recv_cnt++]=d;
//		
//		//��ձ�־λ��������Ӧ�µ��ж�����
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//	}
//}




//void USART3_IRQHandler(void)
//{
//	uint8_t d=0;
//	
//	uint32_t ulReturn;
//	
//	
//	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();
//	
//	//����Ƿ���յ�����
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//	{
//		d=USART_ReceiveData(USART3);
//		
//		if(g_esp8266_rx_cnt<(sizeof(g_esp8266_rx_buf)-1))
//			g_esp8266_rx_buf[g_esp8266_rx_cnt++]=d;
//		
//	
//		//��ձ�־λ��������Ӧ�µ��ж�����
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//	
//	/* �˳��ٽ�� */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
//}



//���ڷ��ͺ���
void USART_Send(USART_TypeDef* USARTx, char *str)
{
//	static int i =0;
  char *p = str;
  
  while( *p != '\0')
  {
      
      USART_SendData( USARTx, *p++);  
      while( USART_GetFlagStatus( USARTx, USART_FLAG_TXE) !=  SET);   //�ȴ����ݷ������
      USART_ClearFlag( USARTx, USART_FLAG_TXE);
  }
}


void usart_send_bytes(USART_TypeDef* USARTx,uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(USARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}
}



//�������ݰ�����
/**************************************************************
 ����Э���: 0XA0 0xA2  0x32 0x20 0xEF
����:    0xA0 : Э����İ�ͷ
         0xA2 : LED2��
         0x32 : ��˸Ƶ��
         0x20 : ִ��ʱ��
         0xEF : Э����İ�β
***************************************************************/
void USART_SendByte( USART_TypeDef *USARTx, uint8_t *src, uint32_t len)
{
  for( int index = 0; index < len ; index++)
  {
    USART_SendData( USARTx, src[index]);  
    while( USART_GetFlagStatus( USARTx, USART_FLAG_TXE) !=  SET);   //�ȴ����ݷ������
    USART_ClearFlag( USARTx, USART_FLAG_TXE);
  }
}

void Uart2_Clearbuf(void)
{
	u8 t;
	USART2_RX_STA=0;
	for( t=0;t<USART2_MAX_RECV_LEN;t++ )
	{
			USART2_RX_BUF[t] = 0;
	}	
}

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
u16 USART2_RX_STA   = 0;    //接收状态标记
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 

//串口1初始化
//baud: 波特率
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	//使能串口1的时钟
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
  
  //使能GPIO时钟( 使能串口TXD，RXD引脚)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
  
  //设置引脚的复用功能
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource9,  GPIO_AF_USART1); //USART1_TX
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //USART1_RX

  //初始化引脚的功能
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOA, &GPIO_InitStruct);
  
  //配置串口参数 -- 这里设置的参数，需要与对端设备串口参数一致
  USART_InitStruct.USART_BaudRate             = baud;   //波特率, 常用的波特率:4800, 9600, 19200,115200
                                                        //设置串口数据包传输的速率,例: 9600 - 1秒钟发送9600bit的数据   1个bit需要使用104.17us
                                                        //时间需要借助系统时钟进行计算， 系统时钟(PLL_M, HSE_VALUE)出错, 1个bit所需要的时间就会不准确, 对方设备就不会准确的接收到信息
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //硬件控制流， 需要使用CTS， RTS引脚控制， 但是USART接口没有引出该引脚
                                                                                        //不使用硬件控制流
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //启动串口的发送与接收的功能
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //校验模式不需要设置
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //设置停止位1位
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8位数据位
  USART_Init( USART1, &USART_InitStruct);


  //设置接收中断
  USART_ITConfig( USART1, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;        //串口1中断通道
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //响应优先级
  NVIC_Init( &NVIC_InitStruct);
  
  //启动串口工作
  USART_Cmd( USART1, ENABLE);
}

//串口2初始化
//baud: 波特率
void USART2_Init( uint32_t baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef  NVIC_InitStruct;
  
  //使能串口2的时钟
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
  
  //使能GPIO时钟( 使能串口TXD，RXD引脚)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
  
  //设置引脚的复用功能
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //USART2_TX
  GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //USART2_RX

  //初始化引脚的功能
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOA, &GPIO_InitStruct);
  
  //配置串口参数 -- 这里设置的参数，需要与对端设备串口参数一致
  USART_InitStruct.USART_BaudRate             = baud;   //波特率, 常用的波特率:4800, 9600, 19200,115200
                                                        //设置串口数据包传输的速率,例: 9600 - 1秒钟发送9600bit的数据   1个bit需要使用104.17us
                                                        //时间需要借助系统时钟进行计算， 系统时钟(PLL_M, HSE_VALUE)出错, 1个bit所需要的时间就会不准确, 对方设备就不会准确的接收到信息
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //硬件控制流， 需要使用CTS， RTS引脚控制， 但是USART接口没有引出该引脚
                                                                                        //不使用硬件控制流
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //启动串口的发送与接收的功能
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //校验模式不需要设置
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //设置停止位1位
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8位数据位
  USART_Init( USART2, &USART_InitStruct);


  //设置接收中断
  USART_ITConfig( USART2, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART2_IRQn;        //串口1中断通道
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //响应优先级
  NVIC_Init( &NVIC_InitStruct);
  
  //启动串口工作
  USART_Cmd( USART2, ENABLE);
}

void USART2_DisInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
		GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    PAout(2)=0;
    PAout(3)=0;	
}

//串口3初始化
//baud: 波特率
void USART3_Init( uint32_t baud)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;
  NVIC_InitTypeDef  NVIC_InitStruct;
  
  //使能串口3的时钟
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
  
  //使能GPIO时钟( 使能串口TXD，RXD引脚)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
  
  //设置引脚的复用功能
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //USART3_TX
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //USART3_RX

  //初始化引脚的功能
  GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;       
  GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;            
  GPIO_Init( GPIOB, &GPIO_InitStruct);
  
  //配置串口参数 -- 这里设置的参数，需要与对端设备串口参数一致
  USART_InitStruct.USART_BaudRate             = baud;   //波特率, 常用的波特率:4800, 9600, 19200,115200
                                                        //设置串口数据包传输的速率,例: 9600 - 1秒钟发送9600bit的数据   1个bit需要使用104.17us
                                                        //时间需要借助系统时钟进行计算， 系统时钟(PLL_M, HSE_VALUE)出错, 1个bit所需要的时间就会不准确, 对方设备就不会准确的接收到信息
  USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;         //硬件控制流， 需要使用CTS， RTS引脚控制， 但是USART接口没有引出该引脚
                                                                                        //不使用硬件控制流
  USART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;          //启动串口的发送与接收的功能
  USART_InitStruct.USART_Parity               = USART_Parity_No;                        //校验模式不需要设置
  USART_InitStruct.USART_StopBits             = USART_StopBits_1;                       //设置停止位1位
  USART_InitStruct.USART_WordLength           = USART_WordLength_8b;                    //8位数据位
  USART_Init( USART3, &USART_InitStruct);


  //设置接收中断
  USART_ITConfig( USART3, USART_IT_RXNE, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel                     = USART3_IRQn;        //串口3中断通道
  NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0;                  //抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0;                  //响应优先级
  NVIC_Init( &NVIC_InitStruct);
  
  //启动串口工作
   USART_Cmd( USART3, ENABLE);
}



//配置串口1的中断服务函数
void USART1_IRQHandler(void)
{
  //由于串口设备有多个中断源可以触发中断服务函数,所以需要判断是否哪个中断源触发
  
  //判断串口1中断是否由 接收中断 触发的
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
	* @brief	串口2中断服务函数
	* @param 	None
	* @return None
	*/
//配置串口2的中断服务函数
//void USART2_IRQHandler(void)
//{
//	static uint8_t d=0;
//	
//	//检测是否接收到数据
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		d=USART_ReceiveData(USART2);
//		
//		if(g_USART2_recv_cnt< sizeof(g_USART2_recv_buf))
//			g_USART2_recv_buf[g_USART2_recv_cnt++]=d;
//		
//		//清空标志位，可以响应新的中断请求
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
//	/* 进入临界段，临界段可以嵌套 */
//	ulReturn = taskENTER_CRITICAL_FROM_ISR();
//	
//	//检测是否接收到数据
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//	{
//		d=USART_ReceiveData(USART3);
//		
//		if(g_esp8266_rx_cnt<(sizeof(g_esp8266_rx_buf)-1))
//			g_esp8266_rx_buf[g_esp8266_rx_cnt++]=d;
//		
//	
//		//清空标志位，可以响应新的中断请求
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//	
//	/* 退出临界段 */
//	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
//}



//串口发送函数
void USART_Send(USART_TypeDef* USARTx, char *str)
{
//	static int i =0;
  char *p = str;
  
  while( *p != '\0')
  {
      
      USART_SendData( USARTx, *p++);  
      while( USART_GetFlagStatus( USARTx, USART_FLAG_TXE) !=  SET);   //等待数据发送完毕
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
		
		//等待数据发送成功
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}
}



//串口数据包发送
/**************************************************************
 例如协议包: 0XA0 0xA2  0x32 0x20 0xEF
例如:    0xA0 : 协议包的包头
         0xA2 : LED2打开
         0x32 : 闪烁频率
         0x20 : 执行时间
         0xEF : 协议包的包尾
***************************************************************/
void USART_SendByte( USART_TypeDef *USARTx, uint8_t *src, uint32_t len)
{
  for( int index = 0; index < len ; index++)
  {
    USART_SendData( USARTx, src[index]);  
    while( USART_GetFlagStatus( USARTx, USART_FLAG_TXE) !=  SET);   //等待数据发送完毕
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

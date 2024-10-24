#include "myprintf.h"

FILE __stdout;
FILE __stdin;

//重载printf函数,可指定USART接口
int fputc(int ch, FILE *f) 
{
  USART_SendData( f->USARTx, ch);  
  while( USART_GetFlagStatus( f->USARTx, USART_FLAG_TXE) !=  SET);   //等待数据发送完毕
  USART_ClearFlag( f->USARTx, USART_FLAG_TXE);
  return(ch);
}
/*******************使用示例******************
		int main(void)
		{
				// 初始化 USART1 和 USART2
				USART_Init(USART1);
				USART_Init(USART2);

				// 设置 stdout 为 USART1
				__stdout.USARTx = USART1;
				printf("Hello, USART1!\n");

				// 动态修改 stdout 为 USART2
				__stdout.USARTx = USART2;
				printf("Hello, USART2!\n");

				while (1);
		}
*********************************************/




// 自定义带有可变参数的打印函数
void custom_print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // 使用vprintf格式化输出
    va_end(args);
}

/******************************vprintf 使用实例*************************************
		int main() 
		{
				custom_print("Hello %s, you have %d new messages!\n", "Alice", 5);
				return 0;
		}
************************************************************************************/

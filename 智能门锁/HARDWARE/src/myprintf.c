#include "myprintf.h"

FILE __stdout;
FILE __stdin;

//����printf����,��ָ��USART�ӿ�
int fputc(int ch, FILE *f) 
{
  USART_SendData( f->USARTx, ch);  
  while( USART_GetFlagStatus( f->USARTx, USART_FLAG_TXE) !=  SET);   //�ȴ����ݷ������
  USART_ClearFlag( f->USARTx, USART_FLAG_TXE);
  return(ch);
}
/*******************ʹ��ʾ��******************
		int main(void)
		{
				// ��ʼ�� USART1 �� USART2
				USART_Init(USART1);
				USART_Init(USART2);

				// ���� stdout Ϊ USART1
				__stdout.USARTx = USART1;
				printf("Hello, USART1!\n");

				// ��̬�޸� stdout Ϊ USART2
				__stdout.USARTx = USART2;
				printf("Hello, USART2!\n");

				while (1);
		}
*********************************************/




// �Զ�����пɱ�����Ĵ�ӡ����
void custom_print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // ʹ��vprintf��ʽ�����
    va_end(args);
}

/******************************vprintf ʹ��ʵ��*************************************
		int main() 
		{
				custom_print("Hello %s, you have %d new messages!\n", "Alice", 5);
				return 0;
		}
************************************************************************************/

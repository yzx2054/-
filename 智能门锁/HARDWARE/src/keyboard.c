#include "keyboard.h"
#include "sys.h"
#include "delay.h"

void key_board_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		//ʹ�ܶ˿�D��Ӳ��ʱ�ӣ����ǶԶ˿�D����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		//ʹ�ܶ˿�E��Ӳ��ʱ�ӣ����ǶԶ˿�E����
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);



		//PD14 PD0 PE7 PE9      //PD15 PD1 PE8 PE10
		//ʹ��GPIO_Init����������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;         //���ģʽ
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;        //�������ģʽ,Ĭ�ϵ�
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_14; //ָ����0 14������
		GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;      //���٣����ǹ��������
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //���������������ʹ���������裩
		GPIO_Init(GPIOD,&GPIO_InitStructure);               //D��

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_9; 	//ָ����7 9������
		GPIO_Init(GPIOE,&GPIO_InitStructure);              	//E��


		/* �����������ģʽ����ʹ���ڲ��������裬�������յ�ʱ��Ϊ�̶��ĸߵ�ƽ */
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	   					//����
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;         				//����ģʽ		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_1;     //ָ����15 1������
		GPIO_Init(GPIOD,&GPIO_InitStructure);              //D��
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_10;			//ָ����8 10������
		GPIO_Init(GPIOE,&GPIO_InitStructure);              //E��
}


char get_key_board(void)
{
    //PD14 PD0 PE7 PE9
    PDout(14) = 0;
    PDout(0) 	= 1;
    PEout(7) 	= 1;
    PEout(9) 	= 1;
    delay_ms(2);
	
    //PD15 PD1 PE8 PE10
    if( PDin(15) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(15) == 0);
				return '1';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(1) == 0);
				return '2';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(8) == 0);
				return '3';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(10) == 0);
				return 'A';
    }
    
    PDout(14) = 1;
    PDout(0) 	= 0;
    PEout(7) 	= 1;
    PEout(9) 	= 1;
    delay_ms(2);

    if( PDin(15) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(15) == 0);
				return '4';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(1) == 0);
				return '5';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(8) == 0);
				return '6';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(10) == 0);
				return 'B';
    }
    
    PDout(14) = 1;
    PDout(0) 	= 1;
    PEout(7) 	= 0;
    PEout(9) 	= 1;
    delay_ms(2);

    if( PDin(15) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(15) == 0);
				return '7';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(1) == 0);
				return '8';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(8) == 0);
				return '9';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(10) == 0);
				return 'C';
    }
    
    PDout(14) = 1;
    PDout(0) 	= 1;
    PEout(7) 	= 1;
    PEout(9) 	= 0;
    delay_ms(2);

    if( PDin(15) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(15) == 0);
				return '*';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PDin(1) == 0);
				return '0';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(8) == 0);
				return '#';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // ����
        while (PEin(10) == 0);
				return 'D';
    }

    return 'N';  // û�а�������
}




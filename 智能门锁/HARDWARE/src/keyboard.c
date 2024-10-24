#include "keyboard.h"
#include "sys.h"
#include "delay.h"

void key_board_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		//使能端口D的硬件时钟，就是对端口D供电
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		//使能端口E的硬件时钟，就是对端口E供电
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);



		//PD14 PD0 PE7 PE9      //PD15 PD1 PE8 PE10
		//使用GPIO_Init来配置引脚
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;         //输出模式
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;        //推挽输出模式,默认的
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_14; //指定第0 14根引脚
		GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;      //高速，但是功耗是最高
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   //无需上下拉（亦可使能下拉电阻）
		GPIO_Init(GPIOD,&GPIO_InitStructure);               //D口

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_9; 	//指定第7 9根引脚
		GPIO_Init(GPIOE,&GPIO_InitStructure);              	//E口


		/* 矩阵键盘输入模式必须使能内部上拉电阻，引脚悬空的时候为固定的高电平 */
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	   					//上拉
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;         				//输入模式		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_1;     //指定第15 1根引脚
		GPIO_Init(GPIOD,&GPIO_InitStructure);              //D口
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_10;			//指定第8 10根引脚
		GPIO_Init(GPIOE,&GPIO_InitStructure);              //E口
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
        delay_ms(20);  // 消抖
        while (PDin(15) == 0);
				return '1';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PDin(1) == 0);
				return '2';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PEin(8) == 0);
				return '3';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // 消抖
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
        delay_ms(20);  // 消抖
        while (PDin(15) == 0);
				return '4';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PDin(1) == 0);
				return '5';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PEin(8) == 0);
				return '6';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // 消抖
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
        delay_ms(20);  // 消抖
        while (PDin(15) == 0);
				return '7';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PDin(1) == 0);
				return '8';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PEin(8) == 0);
				return '9';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // 消抖
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
        delay_ms(20);  // 消抖
        while (PDin(15) == 0);
				return '*';
    }
    else if( PDin(1) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PDin(1) == 0);
				return '0';
    }
    else if( PEin(8) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PEin(8) == 0);
				return '#';
    }
    else if( PEin(10) == 0 ) 
		{
        delay_ms(20);  // 消抖
        while (PEin(10) == 0);
				return 'D';
    }

    return 'N';  // 没有按键按下
}




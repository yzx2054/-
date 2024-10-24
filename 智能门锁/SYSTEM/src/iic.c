#include "iic.h"

/*此为软件IIC*/

//IIC初始化
void software_iic_init()
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode      =    GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType     =    GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin       =    GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_PuPd      =    GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed     =    GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    SDA_OUT = 1;
    SCL_OUT = 1;

}

//IIC模式选择
void software_iic_sda_mode( GPIOMode_TypeDef GPIO_Mode)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode      =    GPIO_Mode;
    GPIO_InitStruct.GPIO_OType     =    GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin       =    GPIO_Pin_9;
    GPIO_InitStruct.GPIO_PuPd      =    GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed     =    GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//IIC启动
void software_iic_start()
{
    //设置SDA引脚为输出模式
    software_iic_sda_mode( GPIO_Mode_OUT);
    
    SDA_OUT = 1;
    SCL_OUT = 1;
    delay_us( 5);
    
    SDA_OUT = 0;    //产生下降沿， 开始信号
    delay_us( 5);

    SCL_OUT = 0;    //为发送数据做准备
}

//IIC停止
void software_iic_stop()
{
    //设置SDA引脚为输出模式
    software_iic_sda_mode( GPIO_Mode_OUT);

    SDA_OUT = 0;
    SCL_OUT = 1;
    delay_us( 5);
    
    SDA_OUT = 1;    //SDA产生上升沿， 结束信号
    delay_us(5);
}


//IIC数据发送
void software_iic_sendbyte( uint8_t val)
{
    //设置SDA引脚为输出模式
    software_iic_sda_mode( GPIO_Mode_OUT);
    
    for( int index = 0; index < 8; index++)
    {
        
        if( val & 0x80)     //判断最高位的数据
            SDA_OUT = 1;
        else
            SDA_OUT = 0;    //准备需要发送的数据
        val <<= 1;
        delay_us(5);
    
        SCL_OUT = 1;        //SCL产生上升沿, 通知从机准备接收数据
        delay_us( 5);
    
        SCL_OUT = 0;        //SCL产生下降沿， 通知从机结束接收
    }
}

//IIC数据接收
uint8_t software_iic_recvbyte( )
{
    //设置SDA引脚为输入模式
    software_iic_sda_mode( GPIO_Mode_IN);
    
    uint8_t d = 0;
    
    SCL_OUT = 0;        //SCL产生下降沿， 通知从机准备下一个数据
    delay_us(5);
    
    for( int index = 0; index < 8; index++)
    {
        
        SCL_OUT = 1;    //要求从机数据稳定，发送到主机上
        delay_us(5);

        if( SDA_IN )
            d |= (1<< ( 7- index));  //保存接收到的信息

        SCL_OUT = 0;        //SCL产生下降沿， 通知从机准备下一个数据
        delay_us(5);
    }
    
    return d;
}

//IIC格式发送
//ack: 
//      1：有效应答
//      0：无效应答
void software_iic_sendAck( uint8_t ack)
{
    //设置SDA引脚为输出模式
    software_iic_sda_mode( GPIO_Mode_OUT);

        
    if( ack)            //判断最高位的数据
        SDA_OUT = 0;
    else
        SDA_OUT = 1;    //准备需要发送的数据
    
    delay_us(5);

    SCL_OUT = 1;        //SCL产生上升沿, 通知从机准备接收数据
    delay_us( 5);

    SCL_OUT = 0;        //SCL产生下降沿， 通知从机结束接收

}


//IIC格式接收
//返回值ack: 
//      1：有效应答
//      0：无效应答
uint8_t software_iic_recvAck()
{
    //设置SDA引脚为输入模式
    software_iic_sda_mode( GPIO_Mode_IN);
    
    uint8_t ack = 0;
      
    SCL_OUT = 1;    //要求从机数据稳定，发送到主机上
    delay_us(5);

    if( SDA_IN )
       ack = 0;
    else
       ack = 1;

    SCL_OUT = 0;        //SCL产生下降沿， 通知从机准备下一个数据
    delay_us(5);

    return ack;
}





#include "iic.h"

/*��Ϊ���IIC*/

//IIC��ʼ��
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

//IICģʽѡ��
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

//IIC����
void software_iic_start()
{
    //����SDA����Ϊ���ģʽ
    software_iic_sda_mode( GPIO_Mode_OUT);
    
    SDA_OUT = 1;
    SCL_OUT = 1;
    delay_us( 5);
    
    SDA_OUT = 0;    //�����½��أ� ��ʼ�ź�
    delay_us( 5);

    SCL_OUT = 0;    //Ϊ����������׼��
}

//IICֹͣ
void software_iic_stop()
{
    //����SDA����Ϊ���ģʽ
    software_iic_sda_mode( GPIO_Mode_OUT);

    SDA_OUT = 0;
    SCL_OUT = 1;
    delay_us( 5);
    
    SDA_OUT = 1;    //SDA���������أ� �����ź�
    delay_us(5);
}


//IIC���ݷ���
void software_iic_sendbyte( uint8_t val)
{
    //����SDA����Ϊ���ģʽ
    software_iic_sda_mode( GPIO_Mode_OUT);
    
    for( int index = 0; index < 8; index++)
    {
        
        if( val & 0x80)     //�ж����λ������
            SDA_OUT = 1;
        else
            SDA_OUT = 0;    //׼����Ҫ���͵�����
        val <<= 1;
        delay_us(5);
    
        SCL_OUT = 1;        //SCL����������, ֪ͨ�ӻ�׼����������
        delay_us( 5);
    
        SCL_OUT = 0;        //SCL�����½��أ� ֪ͨ�ӻ���������
    }
}

//IIC���ݽ���
uint8_t software_iic_recvbyte( )
{
    //����SDA����Ϊ����ģʽ
    software_iic_sda_mode( GPIO_Mode_IN);
    
    uint8_t d = 0;
    
    SCL_OUT = 0;        //SCL�����½��أ� ֪ͨ�ӻ�׼����һ������
    delay_us(5);
    
    for( int index = 0; index < 8; index++)
    {
        
        SCL_OUT = 1;    //Ҫ��ӻ������ȶ������͵�������
        delay_us(5);

        if( SDA_IN )
            d |= (1<< ( 7- index));  //������յ�����Ϣ

        SCL_OUT = 0;        //SCL�����½��أ� ֪ͨ�ӻ�׼����һ������
        delay_us(5);
    }
    
    return d;
}

//IIC��ʽ����
//ack: 
//      1����ЧӦ��
//      0����ЧӦ��
void software_iic_sendAck( uint8_t ack)
{
    //����SDA����Ϊ���ģʽ
    software_iic_sda_mode( GPIO_Mode_OUT);

        
    if( ack)            //�ж����λ������
        SDA_OUT = 0;
    else
        SDA_OUT = 1;    //׼����Ҫ���͵�����
    
    delay_us(5);

    SCL_OUT = 1;        //SCL����������, ֪ͨ�ӻ�׼����������
    delay_us( 5);

    SCL_OUT = 0;        //SCL�����½��أ� ֪ͨ�ӻ���������

}


//IIC��ʽ����
//����ֵack: 
//      1����ЧӦ��
//      0����ЧӦ��
uint8_t software_iic_recvAck()
{
    //����SDA����Ϊ����ģʽ
    software_iic_sda_mode( GPIO_Mode_IN);
    
    uint8_t ack = 0;
      
    SCL_OUT = 1;    //Ҫ��ӻ������ȶ������͵�������
    delay_us(5);

    if( SDA_IN )
       ack = 0;
    else
       ack = 1;

    SCL_OUT = 0;        //SCL�����½��أ� ֪ͨ�ӻ�׼����һ������
    delay_us(5);

    return ack;
}





#define AT24C02_ADDR 0xA0
#define WRITE        0x00
#define READ         0x01

enum AT24C02_ERROR
{
    DEVICE_ADDRESS_ERROR,
    MEMORY_ADDRESS_ERROR,
    DATA_ERROR

};

uint8_t at24c02_writeByte( uint8_t address,uint8_t byte)
{
    uint8_t ack = 0;
    
    //开始信号
    software_iic_start();

    //发送设备地址+写权限
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02寻址出错\n");
        software_iic_stop();
        return 1;
    }
    
    //发送存储器的内存地址，用于指定保存数据的位置
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02存储器地址出错\n");
        software_iic_stop();
        return 2;
    } 
    
    //发送需要存储的数据
    software_iic_sendbyte( byte );
    
    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02写入存储数据出错\n");
        software_iic_stop();
        return 3;
    } 
    
    //发送停止信号，结束通信
    software_iic_stop();
    
    return 0;
}

uint8_t at24c02_write( uint8_t address,uint8_t *pdata, uint8_t len)
{
    uint8_t ack = 0;
    uint8_t *p = pdata;
    
    //开始信号
    software_iic_start();

    //发送设备地址+写权限
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02寻址出错\n");
        software_iic_stop();
        return 1;
    }
    
    //发送存储器的内存地址，用于指定保存数据的位置
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02存储器地址出错\n");
        software_iic_stop();
        return 2;
    } 
    
    //发送需要存储的数据
    for( int i = 0; i < len; i++)
    {
        software_iic_sendbyte( *p++ );
        
        ack = software_iic_recvAck();
        if( ack == 0)                   //无效应答
        {
            printf("AT24C02写入存储数据出错\n");
            software_iic_stop();
            return 3;
        } 
    }
    //发送停止信号，结束通信
    software_iic_stop();
    
    return 0;
}

void at24c02_writeData( uint8_t address,uint8_t *pdata, uint8_t len)
{
    for( int i =0 ; i< len;i++)
    {
        at24c02_writeByte( address + i, pdata[i]);
        delay_ms(2);
    }
}



uint8_t at24c02_read( uint8_t address, uint8_t *pdata, uint32_t len)
{

    uint8_t ack = 0;
    
    //开始信号
    software_iic_start();

    //发送设备地址+写权限
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02写操作寻址出错\n");
        software_iic_stop();
        return 1;
    }
    
    //发送存储器的内存地址，用于指定读取数据的位置
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02存储器地址出错\n");
        software_iic_stop();
        return 2;
    } 
    
    //开始信号
    software_iic_start();

    //发送设备地址+写权限
    software_iic_sendbyte( AT24C02_ADDR | READ);

    ack = software_iic_recvAck();
    if( ack == 0)                   //无效应答
    {
        printf("AT24C02读操作寻址出错\n");
        software_iic_stop();
        return 1;
    }
    
    for( int i =0; i < len-1; i++)
    {
        *pdata++ = software_iic_recvbyte();
        
        software_iic_sendAck(1);    //发送有效应答 
    }
    
    *pdata = software_iic_recvbyte();
        
    software_iic_sendAck(0);    //发送无效应答，停止AT24C02的输出    
    
    software_iic_stop();
    
    return 0;
}

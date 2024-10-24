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
    
    //��ʼ�ź�
    software_iic_start();

    //�����豸��ַ+дȨ��
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02Ѱַ����\n");
        software_iic_stop();
        return 1;
    }
    
    //���ʹ洢�����ڴ��ַ������ָ���������ݵ�λ��
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02�洢����ַ����\n");
        software_iic_stop();
        return 2;
    } 
    
    //������Ҫ�洢������
    software_iic_sendbyte( byte );
    
    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02д��洢���ݳ���\n");
        software_iic_stop();
        return 3;
    } 
    
    //����ֹͣ�źţ�����ͨ��
    software_iic_stop();
    
    return 0;
}

uint8_t at24c02_write( uint8_t address,uint8_t *pdata, uint8_t len)
{
    uint8_t ack = 0;
    uint8_t *p = pdata;
    
    //��ʼ�ź�
    software_iic_start();

    //�����豸��ַ+дȨ��
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02Ѱַ����\n");
        software_iic_stop();
        return 1;
    }
    
    //���ʹ洢�����ڴ��ַ������ָ���������ݵ�λ��
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02�洢����ַ����\n");
        software_iic_stop();
        return 2;
    } 
    
    //������Ҫ�洢������
    for( int i = 0; i < len; i++)
    {
        software_iic_sendbyte( *p++ );
        
        ack = software_iic_recvAck();
        if( ack == 0)                   //��ЧӦ��
        {
            printf("AT24C02д��洢���ݳ���\n");
            software_iic_stop();
            return 3;
        } 
    }
    //����ֹͣ�źţ�����ͨ��
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
    
    //��ʼ�ź�
    software_iic_start();

    //�����豸��ַ+дȨ��
    software_iic_sendbyte( AT24C02_ADDR | WRITE);

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02д����Ѱַ����\n");
        software_iic_stop();
        return 1;
    }
    
    //���ʹ洢�����ڴ��ַ������ָ����ȡ���ݵ�λ��
    software_iic_sendbyte( address );

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02�洢����ַ����\n");
        software_iic_stop();
        return 2;
    } 
    
    //��ʼ�ź�
    software_iic_start();

    //�����豸��ַ+дȨ��
    software_iic_sendbyte( AT24C02_ADDR | READ);

    ack = software_iic_recvAck();
    if( ack == 0)                   //��ЧӦ��
    {
        printf("AT24C02������Ѱַ����\n");
        software_iic_stop();
        return 1;
    }
    
    for( int i =0; i < len-1; i++)
    {
        *pdata++ = software_iic_recvbyte();
        
        software_iic_sendAck(1);    //������ЧӦ�� 
    }
    
    *pdata = software_iic_recvbyte();
        
    software_iic_sendAck(0);    //������ЧӦ��ֹͣAT24C02�����    
    
    software_iic_stop();
    
    return 0;
}

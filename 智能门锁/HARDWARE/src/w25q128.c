#include "w25q128.h"
#include "myincludes.h"

//SPI1_Flash


//90H		��ID
//03H		������
//20H		��������
//04H		дʧ��
//06H		дʹ��
//02H		ҳ���
//C7H/60H	оƬ����
//05H/35H	��״̬�Ĵ���1/2




void software_spi_init(void)
{
    //��ʼ��SCLK( PB3) MISO( PB4) MOSI( PB5)
    
    static GPIO_InitTypeDef GPIO_InitStruct;
    
    
    //ʹ������Ӳ��ʱ��
    RCC_AHB1PeriphClockCmd( SCLK_CLOCK | MOSI_CLOCK | MISO_CLOCK, ENABLE);
    

    GPIO_InitStruct.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed  =   GPIO_Speed_100MHz;
    
    //SCLK
    GPIO_InitStruct.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin    =   SCLK_PIN;
    GPIO_Init( SCLK_PORT, &GPIO_InitStruct);
    
    //MISO
    GPIO_InitStruct.GPIO_Mode   =   GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin    =   MISO_PIN;
    GPIO_Init( MISO_PORT, &GPIO_InitStruct);

    //MOSI
    GPIO_InitStruct.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin    =   MOSI_PIN;
    GPIO_Init( MOSI_PORT, &GPIO_InitStruct);

#if 1
    //ģʽ0 -- CPOL = 0�� SPI���߿���״̬ʱ��ʱ���ߴ��ڵ͵�ƽ״̬
    SCLK_OUT = 0;   
#else
    //ģʽ3 -- CPOL = 1�� SPI���߿���״̬ʱ��ʱ���ߴ��ڸߵ�ƽ״̬
    SCLK_OUT = 0;  
#endif    
    
    
    MOSI_OUT = 0;
}

/*
    SPIģʽ0
    byte: ��Ҫ���͵��ֽ�����
    ����ֵ�� ���յ��ֽ�����
*/
uint8_t software_spi_send_read_byte( uint8_t byte)
{
    //ģʽ0�� �������źŵĵ�һ��������ؽ�������
   
    uint8_t d = 0;      //����ӻ����͵�����
    
    int i = 0;
    for( i =0; i < 8; i++)
    {
        //׼��������Ҫ���͵�����, �г�ֵ�ʱ�䷢�͵��ӻ�
        //��λ���ȷ���
        if( byte & 0x80)
            MOSI_OUT = 1;
        else
            MOSI_OUT = 0;
        byte <<= 1;     //��ʣ���bitλ�ƶ�����λ������λ�ж�
        
        
        //����ʱ�������ߣ� ������ʼ�ɼ�MISO���ŵ������ƽ
        SCLK_OUT = 1;   
        delay_us(2);

        if( MISO_IN)
            d |= (1<<( 7 - i ));
        
        /****************************
        //׼��������Ҫ���͵�����,�����źŴ�������Ҫʱ�䣬
        //�����λ��׼���� ���̰�ʱ�������ͣ� ����MOSI������źţ���û�д��䵽�ӻ��ϣ�
        //�ӻ������̰����ݲɼ��ˣ� �ֿ��ܲɼ�����һ�η��͵�����
        if( byte & 0x80)
            MOSI_OUT = 1;
        else
            MOSI_OUT = 0;
        ****************************/
        
        //����ʱ�������ͣ� ֪ͨ�ӻ��ɼ�MOSI����ĵ�ƽ
        SCLK_OUT = 0;
        delay_us(2);
    }
    return d;
}



//W25Q128��Ƭѡ���ų�ʼ��
void W25Q128_Init(void)
{
     //��ʼ��SCLK( PB3) MISO( PB4) MOSI( PB5)
    
    static GPIO_InitTypeDef GPIO_InitStruct;
    
    //ʹ������Ӳ��ʱ��
    RCC_AHB1PeriphClockCmd( W25Q128_CLOCK, ENABLE);
    

    GPIO_InitStruct.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin    =   W25Q128_PIN;
    GPIO_Init( W25Q128_PORT, &GPIO_InitStruct);
    
    //����Ƭѡ����Ϊ�ߵ�ƽ, оƬ���ڿ���״̬
    W25Q128_OUT = 1;

    //��ʼ�����SPI�Ľӿ�
    software_spi_init();

}

//��ȡ�豸ID
uint16_t W25Q128_ReadID(void)
{
    uint8_t m_id , d_id;
    //����Ƭѡ���ţ�ѡ����W25Q128оƬ����ͨ��
    W25Q128_OUT = 0;
    
    //���Ͷ�ȡ�豸ID��ָ��
    software_spi_send_read_byte( 0x90);

    //����24bit����
    software_spi_send_read_byte( 0x00);
    software_spi_send_read_byte( 0x00);
    software_spi_send_read_byte( 0x00);


    //��ȡ����ID
    m_id = software_spi_send_read_byte( 0x00);

    
    //��ȡ�豸ID
    d_id = software_spi_send_read_byte( 0x00);
    
    //����Ƭѡ���ţ�ȡ����W25Q128оƬ��ͨ��
    W25Q128_OUT = 1;
    
    return ((m_id<< 8) | d_id );
}

//��ȡ״̬�Ĵ���
uint8_t  W25Q128_ReadRegistr(void)
{
    uint8_t sta =0;
    
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x05);
    
    //��ȡ״̬�Ĵ�����ֵ
    sta = software_spi_send_read_byte( 0x00);
    
    W25Q128_OUT = 1;
    
    return sta;
}

//��W25Q128��д����
void W25Q128_WriteEnable(void)
{
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x06);
    
    W25Q128_OUT = 1;
}

//�ر�W25Q128��д����
void W25Q128_WriteDisable(void)
{
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x04);
    
    W25Q128_OUT = 1;
}

//ҳ���:д��1~256���ֽڵ��ڴ�ҳ��
//      address��  �ڴ��ַ
//      data:      ��Ҫд�������
//      len :      ��Ҫд������ݳ���
void W25Q128_PageProgram(uint32_t address, uint8_t *data, uint32_t len)
{
    uint8_t *p = data;
    
    //дʹ��
    W25Q128_WriteEnable();    
    
    //Ƭѡ����
    W25Q128_OUT = 0;

    //����ҳ���ָ�� -- 02h
    software_spi_send_read_byte( 0x02);
    
    //���� �ڴ�ҳ��24bit��ַ 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);

    while( len-- )
        software_spi_send_read_byte( *p++);
    
    //Ƭѡ���� -- ������ǰд��
    W25Q128_OUT = 1;
    
    
    //�ȴ�ҳ���д�����������д�뵽�ڴ����
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //дʧ��
    W25Q128_WriteDisable();
}

//��������--����ڴ��е�����
//      address: ��Ҫ������������ַ
void W25Q128_SectorErase(uint32_t address)
{
    //��д����
    W25Q128_WriteEnable( );
    
    //Ƭѡ��������ѡ��оƬ
    W25Q128_OUT = 0;
    
    //������������ָ��
    software_spi_send_read_byte( 0x20);
    
    //���� �ڴ�ҳ��24bit��ַ 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);
    
    //Ƭѡ��������ȡ��ѡ��оƬ
    W25Q128_OUT = 1;
    
    //�ȴ������������
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //дʧ��
    W25Q128_WriteDisable();
}

//оƬ����--����ڴ��е�����
void W25Q128_ChipErase(void)
{
    //��д����
    W25Q128_WriteEnable( );
    
    //Ƭѡ��������ѡ��оƬ
    W25Q128_OUT = 0;
    
    //����оƬ����ָ��
    software_spi_send_read_byte( 0xC7);
   
    //Ƭѡ��������ȡ��ѡ��оƬ
    W25Q128_OUT = 1;
   

    //�ȴ�оƬ�������
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //дʧ��
    W25Q128_WriteDisable();
    
}


//������
void W25Q128_ReadData( uint32_t address, uint8_t *pdata, uint32_t len)
{
    //Ƭѡ��������ѡ��оƬ
    W25Q128_OUT = 0;
    
    //���Ͷ�����ָ��
    software_spi_send_read_byte( 0x03);
    
    //���� �ڴ�ҳ��24bit��ַ 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);
   
    while( len--)
        *pdata++ = software_spi_send_read_byte(0xFF);
  
    //Ƭѡ��������ȡ��ѡ��оƬ
    W25Q128_OUT = 1;
}


#include "w25q128.h"
#include "myincludes.h"

//SPI1_Flash


//90H		读ID
//03H		读数据
//20H		擦除扇区
//04H		写失能
//06H		写使能
//02H		页编程
//C7H/60H	芯片擦除
//05H/35H	读状态寄存器1/2




void software_spi_init(void)
{
    //初始化SCLK( PB3) MISO( PB4) MOSI( PB5)
    
    static GPIO_InitTypeDef GPIO_InitStruct;
    
    
    //使能引脚硬件时钟
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
    //模式0 -- CPOL = 0， SPI总线空闲状态时，时钟线处于低电平状态
    SCLK_OUT = 0;   
#else
    //模式3 -- CPOL = 1， SPI总线空闲状态时，时钟线处于高电平状态
    SCLK_OUT = 0;  
#endif    
    
    
    MOSI_OUT = 0;
}

/*
    SPI模式0
    byte: 需要发送的字节数据
    返回值： 接收的字节数据
*/
uint8_t software_spi_send_read_byte( uint8_t byte)
{
    //模式0， 在脉冲信号的第一个跳变边沿接收数据
   
    uint8_t d = 0;      //保存从机发送的数据
    
    int i = 0;
    for( i =0; i < 8; i++)
    {
        //准备主机需要发送的数据, 有充分的时间发送到从机
        //高位优先发送
        if( byte & 0x80)
            MOSI_OUT = 1;
        else
            MOSI_OUT = 0;
        byte <<= 1;     //把剩余的bit位移动到高位进行逐位判断
        
        
        //设置时钟线拉高， 主机开始采集MISO引脚的输入电平
        SCLK_OUT = 1;   
        delay_us(2);

        if( MISO_IN)
            d |= (1<<( 7 - i ));
        
        /****************************
        //准备主机需要发送的数据,但是信号传输是需要时间，
        //在这个位置准备， 立刻把时钟线拉低， 可以MOSI输出的信号，还没有传输到从机上，
        //从机就立刻把数据采集了， 又可能采集到上一次发送的数据
        if( byte & 0x80)
            MOSI_OUT = 1;
        else
            MOSI_OUT = 0;
        ****************************/
        
        //设置时钟线拉低， 通知从机采集MOSI输出的电平
        SCLK_OUT = 0;
        delay_us(2);
    }
    return d;
}



//W25Q128的片选引脚初始化
void W25Q128_Init(void)
{
     //初始化SCLK( PB3) MISO( PB4) MOSI( PB5)
    
    static GPIO_InitTypeDef GPIO_InitStruct;
    
    //使能引脚硬件时钟
    RCC_AHB1PeriphClockCmd( W25Q128_CLOCK, ENABLE);
    

    GPIO_InitStruct.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin    =   W25Q128_PIN;
    GPIO_Init( W25Q128_PORT, &GPIO_InitStruct);
    
    //设置片选引脚为高电平, 芯片处于空闲状态
    W25Q128_OUT = 1;

    //初始化软件SPI的接口
    software_spi_init();

}

//读取设备ID
uint16_t W25Q128_ReadID(void)
{
    uint8_t m_id , d_id;
    //拉低片选引脚，选择与W25Q128芯片进行通信
    W25Q128_OUT = 0;
    
    //发送读取设备ID的指令
    software_spi_send_read_byte( 0x90);

    //发送24bit数据
    software_spi_send_read_byte( 0x00);
    software_spi_send_read_byte( 0x00);
    software_spi_send_read_byte( 0x00);


    //读取厂商ID
    m_id = software_spi_send_read_byte( 0x00);

    
    //读取设备ID
    d_id = software_spi_send_read_byte( 0x00);
    
    //拉高片选引脚，取消与W25Q128芯片的通信
    W25Q128_OUT = 1;
    
    return ((m_id<< 8) | d_id );
}

//读取状态寄存器
uint8_t  W25Q128_ReadRegistr(void)
{
    uint8_t sta =0;
    
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x05);
    
    //读取状态寄存器的值
    sta = software_spi_send_read_byte( 0x00);
    
    W25Q128_OUT = 1;
    
    return sta;
}

//打开W25Q128的写功能
void W25Q128_WriteEnable(void)
{
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x06);
    
    W25Q128_OUT = 1;
}

//关闭W25Q128的写功能
void W25Q128_WriteDisable(void)
{
    W25Q128_OUT = 0;

    software_spi_send_read_byte( 0x04);
    
    W25Q128_OUT = 1;
}

//页编程:写入1~256个字节到内存页中
//      address：  内存地址
//      data:      需要写入的数据
//      len :      需要写入的数据长度
void W25Q128_PageProgram(uint32_t address, uint8_t *data, uint32_t len)
{
    uint8_t *p = data;
    
    //写使能
    W25Q128_WriteEnable();    
    
    //片选拉低
    W25Q128_OUT = 0;

    //发送页编程指令 -- 02h
    software_spi_send_read_byte( 0x02);
    
    //发送 内存页的24bit地址 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);

    while( len-- )
        software_spi_send_read_byte( *p++);
    
    //片选拉高 -- 结束当前写入
    W25Q128_OUT = 1;
    
    
    //等待页编程写入的数据真正写入到内存块中
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //写失能
    W25Q128_WriteDisable();
}

//扇区擦除--清空内存中的数据
//      address: 需要擦除的扇区地址
void W25Q128_SectorErase(uint32_t address)
{
    //打开写功能
    W25Q128_WriteEnable( );
    
    //片选引脚拉低选择芯片
    W25Q128_OUT = 0;
    
    //发送扇区擦除指令
    software_spi_send_read_byte( 0x20);
    
    //发送 内存页的24bit地址 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);
    
    //片选引脚拉高取消选择芯片
    W25Q128_OUT = 1;
    
    //等待扇区擦除完毕
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //写失能
    W25Q128_WriteDisable();
}

//芯片擦除--清空内存中的数据
void W25Q128_ChipErase(void)
{
    //打开写功能
    W25Q128_WriteEnable( );
    
    //片选引脚拉低选择芯片
    W25Q128_OUT = 0;
    
    //发送芯片擦除指令
    software_spi_send_read_byte( 0xC7);
   
    //片选引脚拉高取消选择芯片
    W25Q128_OUT = 1;
   

    //等待芯片擦除完毕
    uint8_t sta = 0;
    while(1)
    {
        sta = W25Q128_ReadRegistr();
        if( (sta & 0x01) == 0)
            break; 
    }
    
    //写失能
    W25Q128_WriteDisable();
    
}


//读数据
void W25Q128_ReadData( uint32_t address, uint8_t *pdata, uint32_t len)
{
    //片选引脚拉低选择芯片
    W25Q128_OUT = 0;
    
    //发送读数据指令
    software_spi_send_read_byte( 0x03);
    
    //发送 内存页的24bit地址 
    software_spi_send_read_byte( (address >> 16) & 0xFF);
    software_spi_send_read_byte( (address >> 8)  & 0xFF);
    software_spi_send_read_byte(  address        & 0xFF);
   
    while( len--)
        *pdata++ = software_spi_send_read_byte(0xFF);
  
    //片选引脚拉高取消选择芯片
    W25Q128_OUT = 1;
}


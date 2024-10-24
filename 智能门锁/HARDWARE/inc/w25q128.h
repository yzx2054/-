#ifndef _W25Q128_H
#define _W25Q128_H

#include "stm32f4xx.h"                  // Device header

#define SCLK_CLOCK  RCC_AHB1Periph_GPIOB
#define SCLK_PORT   GPIOB
#define SCLK_PIN    GPIO_Pin_3

#define MISO_CLOCK  RCC_AHB1Periph_GPIOB
#define MISO_PORT   GPIOB
#define MISO_PIN    GPIO_Pin_4

#define MOSI_CLOCK  RCC_AHB1Periph_GPIOB
#define MOSI_PORT   GPIOB
#define MOSI_PIN    GPIO_Pin_5

#define SCLK_OUT    PBout(3)
#define MISO_IN     PBin(4)
#define MOSI_OUT    PBout(5)

#define W25Q128_CLOCK  RCC_AHB1Periph_GPIOB
#define W25Q128_PORT   GPIOB
#define W25Q128_PIN    GPIO_Pin_14
#define W25Q128_OUT    PBout(14)

#define Page_Address( block_num, sector_num, page_num) ( 0x10000 * block_num + 0x1000 * sector_num + 0x100 * page_num)
#define Sector_Address( block_num, sector_num)         ( 0x10000 * block_num + 0x1000 * sector_num )
 
 
void software_spi_init(void);
/*
    SPI模式0
    byte: 需要发送的字节数据
    返回值： 接收的字节数据
*/
uint8_t software_spi_send_read_byte( uint8_t byte);
//W25Q128的片选引脚初始化
void W25Q128_Init(void);
//读取设备ID
uint16_t W25Q128_ReadID(void);
//读取状态寄存器
uint8_t  W25Q128_ReadRegistr(void);
//打开W25Q128的写功能
void W25Q128_WriteEnable(void);
//关闭W25Q128的写功能
void W25Q128_WriteDisable(void);
//页编程:写入1~256个字节到内存页中
//      address：  内存地址
//      data:      需要写入的数据
//      len :      需要写入的数据长度
void W25Q128_PageProgram(uint32_t address, uint8_t *data, uint32_t len);
//扇区擦除--清空内存中的数据
//      address: 需要擦除的扇区地址
void W25Q128_SectorErase(uint32_t address);
//芯片擦除--清空内存中的数据
void W25Q128_ChipErase(void);
//读数据
void W25Q128_ReadData( uint32_t address, uint8_t *pdata, uint32_t len);


#endif

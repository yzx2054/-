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
    SPIģʽ0
    byte: ��Ҫ���͵��ֽ�����
    ����ֵ�� ���յ��ֽ�����
*/
uint8_t software_spi_send_read_byte( uint8_t byte);
//W25Q128��Ƭѡ���ų�ʼ��
void W25Q128_Init(void);
//��ȡ�豸ID
uint16_t W25Q128_ReadID(void);
//��ȡ״̬�Ĵ���
uint8_t  W25Q128_ReadRegistr(void);
//��W25Q128��д����
void W25Q128_WriteEnable(void);
//�ر�W25Q128��д����
void W25Q128_WriteDisable(void);
//ҳ���:д��1~256���ֽڵ��ڴ�ҳ��
//      address��  �ڴ��ַ
//      data:      ��Ҫд�������
//      len :      ��Ҫд������ݳ���
void W25Q128_PageProgram(uint32_t address, uint8_t *data, uint32_t len);
//��������--����ڴ��е�����
//      address: ��Ҫ������������ַ
void W25Q128_SectorErase(uint32_t address);
//оƬ����--����ڴ��е�����
void W25Q128_ChipErase(void);
//������
void W25Q128_ReadData( uint32_t address, uint8_t *pdata, uint32_t len);


#endif

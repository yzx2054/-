#ifndef __RFID_H__
#define __RFID_H__

#include "stm32f4xx.h"                  // Device header

//MFRC522Êý¾ÝÇø

extern 	uint8_t  card_enrollnum[3][5];	//×¢²á¿¨ÐòÁÐ´æ´¢

extern	u8  mfrc552pidbuf[18];
extern	u8  card_pydebuf[2];
extern	u8  card_numberbuf[5];
extern	u8  card_key0Abuf[6];
extern	u8  card_writebuf[16];
extern	u8  card_readbuf[18];

//MFRC522²âÊÔº¯Êý
void MFRC522Test(USART_TypeDef* USARTx);

uint8_t RFID_Enroll(void);
uint8_t RFID_Verify(void);
uint8_t RFID_Get_num(void);
void RFID_Delete(void);

#endif



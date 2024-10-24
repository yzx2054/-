#ifndef __SSYY_H__
#define __SSYY_H__

#include "stm32f4xx.h"                  // Device header
//extern uint8_t  card_enrollnum[3][5];	//×¢²á¿¨ĞòÁĞ´æ´¢
extern uint8_t 	passwd[12];						//³õÊ¼ÃÜÂë
extern uint8_t 	directives[12];					//ĞŞ¸ÄÃÜÂëÖ¸Áî
extern uint8_t 	bluetooth_passwd[64];	//À¶ÑÀ¿ªËøÖ¸Áî
extern uint8_t 	bluetooth_logshow[64];			//À¶ÑÀÖ¸Áî
//extern uint8_t  card_enrollnum[3][5];

uint8_t key_passwd_verify(char * keystring);
uint8_t key_passwd_revise(void);

uint8_t menu_show(uint8_t num);
uint8_t Fingerprint_menu_show(uint8_t num);
uint8_t RFID_menu_show(uint8_t num);

uint8_t Fion1_t(void);
uint8_t Fion2_t(void);
uint8_t Fion3_t(void);
uint8_t Fion4_t(void);
uint8_t Fion5_t(void);

uint8_t Fingerprint_Unlock(void);
uint8_t sy(void);

void Logs_Flash(char *date, char *time, char *string);
void Passward_Init(void);
void user_passward_revise(uint8_t * uspasswd);

#endif


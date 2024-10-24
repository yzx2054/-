#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f4xx.h"                  // Device header


#define EN_DEBUG_ESP8266	0

//添加WIFI热点宏定义，此处根据自己的wifi作调整
#define WIFI_SSID 			"TWon"
#define WIFI_PASSWORD		"88888888"

extern uint8_t  g_esp8266_tx_buf[512];
extern volatile uint8_t  g_esp8266_rx_buf[512];
extern volatile uint32_t g_esp8266_rx_cnt;


extern volatile uint32_t g_esp8266_transparent_transmission_sta;

void 	esp8266_init(void);
int32_t  esp8266_self_test(void);
int32_t 	esp8266_exit_transparent_transmission (void);
int32_t 	esp8266_entry_transparent_transmission(void);
int32_t 	esp8266_connect_ap(char* ssid,char* pswd);
int32_t 	esp8266_connect_server(char* mode,char* ip,uint16_t port);
int32_t 	esp8266_disconnect_server(void);
void 	esp8266_send_bytes(uint8_t *buf,uint32_t len);
void 	esp8266_send_str(char *buf);
void 	esp8266_send_at(char *str);
int32_t  esp8266_enable_multiple_id(uint32_t b);
int32_t 	esp8266_create_server(uint16_t port);
int32_t 	esp8266_close_server(uint16_t port);
int32_t 	esp8266_enable_echo(uint32_t b);
int32_t 	esp8266_reset(void);

#endif





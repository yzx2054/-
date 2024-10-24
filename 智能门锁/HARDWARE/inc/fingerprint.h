#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__

#include "stm32f4xx.h"                  // Device header


#define FPM_LED_RED		1
#define FPM_LED_GREEN	2
#define FPM_LED_BLUE	3


extern volatile uint8_t  g_usart2_buf[64];
extern volatile uint32_t g_usart2_cnt;
extern volatile uint32_t g_usart2_event;

extern volatile uint32_t g_fpm_touch_event;

extern void usart2_printf_recv_buf(void);

int32_t fpm_touch_check(void);

extern void Fingerprint_Init(void);
extern void fpm_send_data(int length,uint8_t FPM383C_Databuffer[]);
extern void fpm_sleep(void);
extern int32_t fpm_empty(void);
extern int32_t fpm_id_total(uint16_t *total);
extern uint8_t fpm_ctrl_led(uint8_t color);
extern const char *fpm_error_code_auto_enroll(uint8_t error_code);
extern int32_t fpm_idenify_auto(uint16_t *id);
extern int32_t fpm_enroll_auto(uint16_t id);




#endif


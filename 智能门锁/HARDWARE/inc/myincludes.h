#ifndef __MYINCLUDES_H__
#define __MYINCLUDES_H__

/* ��׼C��*/
#include <stdio.h>	
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"       //ʹ��FreeRTOS��API����Ҫ������ͷ�ļ�
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "delay.h"
#include "sys.h"


#define USE_BEEP 1

/*--------------------------------------------------*/
extern EventGroupHandle_t 	g_event_group1_handle;
extern EventGroupHandle_t 	g_event_group2_handle;


extern SemaphoreHandle_t 		g_sem_beep;
extern SemaphoreHandle_t 		g_sem_led;
extern SemaphoreHandle_t  	g_sem_mutex_oled;
extern SemaphoreHandle_t  	g_sem_mutex_printf;

extern QueueHandle_t 		g_queue_blue;
extern QueueHandle_t 		g_queue_oled;
extern QueueHandle_t 		g_queue_beep;
extern QueueHandle_t 		g_queue_led;

/*--------------------------END------------------------*/

/* OLED��ʾλ�ýṹ�� */
typedef struct __oled_t
{

//OLED���Զ���ָ���
#define OLED_CTRL_DISPLAY_ON        0x01	//����
#define OLED_CTRL_DISPLAY_OFF       0x02	//����
#define OLED_CTRL_INIT              0x03	//��ʼ��
#define OLED_CTRL_CLEAR             0x04	//����
#define OLED_CTRL_SHOW_STRING       0x05	//��ʾ�ַ���
#define OLED_CTRL_SHOW_PICTURE      0x06	//��ʾBMPͼƬ
	
	uint8_t ctrl;		//��Ҫ��OLED���еĲ���
	uint8_t x;			//x������ʼλ
	uint8_t y;			//y������ʼλ

	uint8_t *str;		//��Ҫ��ʾ���ַ���
	uint8_t font_size;	//�����С
  uint8_t chinese;	//��Ҫ��ʾ������
	
	const uint8_t *pic;	//��Ҫ��ʾ��ͼƬ
	uint8_t pic_width;	//ͼƬ�Ŀ��
	uint8_t pic_height;	//ͼƬ�ĸ߶�
}oled_t;

typedef struct __beep_t
{
	uint32_t sta;				//1-���� 0-ֹͣ
	uint32_t duration;			//����ʱ�䣬��λ����
}beep_t;

typedef struct __led_t
{
	uint32_t sta;				//0-���� 1-ֹͣ
	uint32_t time;				//��������
	uint32_t duration;			//����ʱ�䣬��λ����
}led_t;


/* ���� */
extern float g_temp;
extern float g_humi;


#endif

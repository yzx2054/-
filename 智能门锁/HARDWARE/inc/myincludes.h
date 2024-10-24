#ifndef __MYINCLUDES_H__
#define __MYINCLUDES_H__

/* 标准C库*/
#include <stdio.h>	
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"       //使用FreeRTOS的API必须要包含该头文件
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

/* OLED显示位置结构体 */
typedef struct __oled_t
{

//OLED屏自定义指令宏
#define OLED_CTRL_DISPLAY_ON        0x01	//亮屏
#define OLED_CTRL_DISPLAY_OFF       0x02	//灭屏
#define OLED_CTRL_INIT              0x03	//初始化
#define OLED_CTRL_CLEAR             0x04	//清屏
#define OLED_CTRL_SHOW_STRING       0x05	//显示字符串
#define OLED_CTRL_SHOW_PICTURE      0x06	//显示BMP图片
	
	uint8_t ctrl;		//想要对OLED进行的操作
	uint8_t x;			//x坐标起始位
	uint8_t y;			//y坐标起始位

	uint8_t *str;		//需要显示的字符串
	uint8_t font_size;	//字体大小
  uint8_t chinese;	//需要显示的中文
	
	const uint8_t *pic;	//需要显示的图片
	uint8_t pic_width;	//图片的宽度
	uint8_t pic_height;	//图片的高度
}oled_t;

typedef struct __beep_t
{
	uint32_t sta;				//1-工作 0-停止
	uint32_t duration;			//持续时间，单位毫秒
}beep_t;

typedef struct __led_t
{
	uint32_t sta;				//0-工作 1-停止
	uint32_t time;				//工作次数
	uint32_t duration;			//持续时间，单位毫秒
}led_t;


/* 变量 */
extern float g_temp;
extern float g_humi;


#endif

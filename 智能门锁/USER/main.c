#include "stm32f4xx.h"                  // Device header

 
#include "myincludes.h"
#include "usart.h"
#include "delay.h"
#include "myprintf.h"
#include "buzzer.h"
#include "sys.h"
#include "oled.h"
#include "KEY.h"
#include "keyboard.h"
#include "RFID.h"
#include "MFRC522.h"
#include "fingerprint.h"
#include "bluetooth.h"
#include "RTC.h"
#include "w25q128.h"
#include "exti.h"
#include "PWM_Servos.h"

#include "ssyy.h"

/**************************任务句柄与函数**************************/
static TaskHandle_t app_Sys_Init_TaskHandler;							//软硬件统一初始化的任务句柄
void app_Sys_Init( void * arg);														//任务函数


static TaskHandle_t app_OLED_TaskHandler;            			//任务oled的句柄
static void app_OLED_task( void * arg);                   //任务函数

static TaskHandle_t app_Buzzer_TaskHandler;            		//任务buzzer的句柄<蜂鸣器>
static void app_Buzzer_task( void * arg);                 //任务函数

static TaskHandle_t app_Keyboard_TaskHandler;            	//任务keyboard的句柄<矩阵键盘>
static void app_Keyboard_task( void * arg);               //任务函数

static TaskHandle_t app_RFID_TaskHandler;            			//任务RFID的句柄
static void app_RFID_task( void * arg);                   //任务函数

static TaskHandle_t app_Fingerprint_TaskHandler;          //任务fingerprint的句柄<指纹>
static void app_Fingerprint_task( void * arg);            //任务函数

static TaskHandle_t app_Bluetooth_TaskHandler;          	//任务Bluetooth的句柄<蓝牙>
static void app_Bluetooth_task( void * arg);            	//任务函数

static TaskHandle_t app_Rtc_TaskHandler;          				//任务Rtc的句柄<时间>
static void app_Rtc_task( void * arg);            				//任务函数

/*************************END********************************/
/*************************************************************/

///* 事件标志组句柄 */
//EventGroupHandle_t 	g_event_group1_handle;
//EventGroupHandle_t 	g_event_group2_handle;


///* 信号量句柄 */
//SemaphoreHandle_t  	g_sem_mutex_printf;
//SemaphoreHandle_t 	g_sem_beep;
//SemaphoreHandle_t 	g_sem_led;
//SemaphoreHandle_t  	g_sem_mutex_oled;

///* 消息队列句柄 */
//QueueHandle_t 		g_queue_blue 	= NULL;
//QueueHandle_t 		g_queue_oled  	= NULL;
//QueueHandle_t 		g_queue_beep	= NULL;
//QueueHandle_t 		g_queue_led		= NULL;
//QueueHandle_t 		g_queue_flash	= NULL;

/*************************************************************/

/*********************事件标志组*******************************/
EventGroupHandle_t Event;

/************************END*********************************/

/**************************FreeRTOS任务管理******************/
struct TaskHandle
{
    TaskFunction_t pxTaskCode;		//任务函数
    char  * pcName;								//任务名称
    uint16_t usStackDepth;				//任务堆栈大小
    void *pvParameters;						//任务函数的参数
    UBaseType_t uxPriority;				//任务的优先级
    TaskHandle_t *pxCreatedTask;  //任务句柄
};

struct TaskHandle task_handler[] = 
{		//任务函数														 //任务名称							//任务堆栈大小  //任务函数的参数 //任务的优先级										 //任务句柄
//    {(TaskFunction_t )app_Sys_Init,  			 (char  *)"app_Sys_Init",  				128, (void *)NULL, (UBaseType_t )configMAX_PRIORITIES-1, (TaskHandle_t *)&app_Sys_Init_TaskHandler}, 
    {(TaskFunction_t )app_OLED_task, 			 (char  *)"app_OLED_task", 				128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_OLED_TaskHandler}, 
    {(TaskFunction_t )app_Buzzer_task,		 (char  *)"app_Buzzer_task",    	128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_Buzzer_TaskHandler}, 
    {(TaskFunction_t )app_Keyboard_task,	 (char  *)"app_Keyboard_task",    128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_Keyboard_TaskHandler}, 
    {(TaskFunction_t )app_RFID_task,			 (char  *)"app_RFID_task",    		128, (void *)NULL, (UBaseType_t )4,  										 (TaskHandle_t *)&app_RFID_TaskHandler}, 
    {(TaskFunction_t )app_Fingerprint_task,(char  *)"app_Fingerprint_task", 128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_Fingerprint_TaskHandler}, 
    {(TaskFunction_t )app_Bluetooth_task,	 (char  *)"app_Bluetooth_task",   128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_Bluetooth_TaskHandler}, 
    {(TaskFunction_t )app_Rtc_task, 			 (char  *)"app_Rtc_task",  				128, (void *)NULL, (UBaseType_t )4, 										 (TaskHandle_t *)&app_Rtc_TaskHandler}, 
};
/******************************END***************************/


/*-----------------------------变量--------------------------*/

volatile uint32_t   flash_offset = 0;   //存储在内部flash的结构体偏移量

float g_temp=0.0;
float g_humi=0.0;

/*------------------------------END--------------------------*/



/*	
//	//设置优先级分组4, 只支持抢占优先级， 抢占优先级大小( 0~15)
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
//	delay_init(168);
//	USART1_Init(115200);
//	USART2_Init(9600);
//	// 设置 stdout 为 USART1
//	__stdout.USARTx = USART1;
//	
//	BUZZER_Init();
//	BUZZER_OFF();
//	
//	OLED_Init();
//	OLED_Clear();
//	
//	key_board_init();
//	
//	MFRC522_Initializtion();        //初始化RFID
//	
//	USART_Send( USART2, "Usart_SendStr\n");
//	
//	char string_buff[6] = {0};
//	uint8_t i = 0;
//	while(1)
//	{
//		MFRC522_Initializtion();        //初始化RFID
//    MFRC522Test(USART1);
////		delay_ms(500);
//		char buff = get_key_board();
//		if(buff != 'N')
//		{
//			if(i < 6)
//			{
//				string_buff[i++] = buff;			
//			}
//			OLED_ShowString ( 0, 2,(u8*)string_buff,16);
//			OLED_ShowChar   ( 0, 4, buff, 16);//显示ASCII字符
//			printf("%c\n", buff);
//		}
//	}
*/
/*----------------------------------------------------*/
//	/* 定义返回值 */
//	BaseType_t xReturn;
//	
//	/* 设置系统中断优先级分组4，默认FreeRTOS的配置 */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
////	/* 系统定时器中断频率为configTICK_RATE_HZ */
////	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);								
//	
//	/* 初始化蓝牙 */
//	Bluetooth_Init();  
//	//重定向printf函数输出为串口1 <蓝牙>
//	__stdout.USARTx = USART1;

//	// printf("111\n");   							
//	/* 创建app_Sys_Init任务 */
//	xReturn = xTaskCreate((TaskFunction_t )app_Sys_Init,  		    			/* 任务入口函数 */
//												(const char*    )"app_Sys_Init",							/* 任务名字 */
//												(uint16_t       )512,  				    						/* 任务栈大小 */
//												(void*          )NULL,				        				/* 任务入口函数参数 */
//												(UBaseType_t    )7, 					    						/* 任务的优先级 */
//												(TaskHandle_t*  )&app_Sys_Init_TaskHandler);	/* 任务控制块指针 */ 

//	 if(xReturn != pdPASS)
//	{
//		printf("[main] xTaskCreate app_task_init fail\r\n");
//		while(1);
//	}
//						 
//	/* 开启任务调度 */
//	vTaskStartScheduler(); 
//			  
//	while(1)
//	{
//			/*****/
//	}

/*******************************/

/*******************************/

int main(void)
{
	uint8_t 	fine = 0;
	uint16_t 	user_total;
	
	//设置优先级分组4, 只支持抢占优先级， 抢占优先级大小( 0~15)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	delay_init(168);
	Bluetooth_Init();
	__stdout.USARTx = USART1;
	//初始化硬件
	OLED_Init();				 //OLED初始化
	OLED_startUI();			 //OLED开始界面
	
	RTC_Config();				 //RTC时钟初始化
	
	BUZZER_Init();			 //蜂鸣器初始化
	PWM_Servos_Init();	 //PWM舵机初始化
	Servo_OFF();
	
	KEY_Init();					 //四个按键按键初始化
	key_board_init();		 //矩阵键盘初始化
	
	Fingerprint_Init();	 //指纹模块初始化
	
	MFRC522_Initializtion();			//初始化MFRC522
	
	W25Q128_Init();         //初始化W25Q128芯片 以及 SPI接口
//	W25Q128_SectorErase( Sector_Address( 0, 0));
	Passward_Init();
	delay_s(2);
	
	while(1)
	{	
		switch(fine)
		{
			case 0:	//主界面
				fine = Fion1_t();
			break;
			
			case 1:	//键盘
				fine = Fion2_t();
			break;
			
			case 2:	//指纹
				fine = Fion3_t();
			break;
			
			case 3:	//RFID
				fine = Fion4_t();
			break;
			
			case 4:	//主菜单
				fine = Fion5_t();
			break;	
		}
	}
}



/*------------------------------任务函数------------------------*/

void app_Sys_Init( void * arg)
{
	
	
}

void app_OLED_task( void * arg)
{
	
}

void app_Buzzer_task( void * arg)
{

}	

void app_Keyboard_task( void * arg)
{

}

void app_RFID_task( void * arg)
{

}
	
void app_Fingerprint_task( void * arg)
{

}

void app_Bluetooth_task( void * arg)
{

}

//时钟任务
void app_Rtc_task( void * arg)
{
	
}

/*---------------------------------END--------------------------*/

/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	
}
/*-----------------------------------------------------------*/

/*------------------应用程序堆空间申请失败钩子------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


/*----------------------------应用程序空闲钩子-------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}


/*-----------------------应用程序堆栈溢出钩子--------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/



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

/**************************�������뺯��**************************/
static TaskHandle_t app_Sys_Init_TaskHandler;							//��Ӳ��ͳһ��ʼ����������
void app_Sys_Init( void * arg);														//������


static TaskHandle_t app_OLED_TaskHandler;            			//����oled�ľ��
static void app_OLED_task( void * arg);                   //������

static TaskHandle_t app_Buzzer_TaskHandler;            		//����buzzer�ľ��<������>
static void app_Buzzer_task( void * arg);                 //������

static TaskHandle_t app_Keyboard_TaskHandler;            	//����keyboard�ľ��<�������>
static void app_Keyboard_task( void * arg);               //������

static TaskHandle_t app_RFID_TaskHandler;            			//����RFID�ľ��
static void app_RFID_task( void * arg);                   //������

static TaskHandle_t app_Fingerprint_TaskHandler;          //����fingerprint�ľ��<ָ��>
static void app_Fingerprint_task( void * arg);            //������

static TaskHandle_t app_Bluetooth_TaskHandler;          	//����Bluetooth�ľ��<����>
static void app_Bluetooth_task( void * arg);            	//������

static TaskHandle_t app_Rtc_TaskHandler;          				//����Rtc�ľ��<ʱ��>
static void app_Rtc_task( void * arg);            				//������

/*************************END********************************/
/*************************************************************/

///* �¼���־���� */
//EventGroupHandle_t 	g_event_group1_handle;
//EventGroupHandle_t 	g_event_group2_handle;


///* �ź������ */
//SemaphoreHandle_t  	g_sem_mutex_printf;
//SemaphoreHandle_t 	g_sem_beep;
//SemaphoreHandle_t 	g_sem_led;
//SemaphoreHandle_t  	g_sem_mutex_oled;

///* ��Ϣ���о�� */
//QueueHandle_t 		g_queue_blue 	= NULL;
//QueueHandle_t 		g_queue_oled  	= NULL;
//QueueHandle_t 		g_queue_beep	= NULL;
//QueueHandle_t 		g_queue_led		= NULL;
//QueueHandle_t 		g_queue_flash	= NULL;

/*************************************************************/

/*********************�¼���־��*******************************/
EventGroupHandle_t Event;

/************************END*********************************/

/**************************FreeRTOS�������******************/
struct TaskHandle
{
    TaskFunction_t pxTaskCode;		//������
    char  * pcName;								//��������
    uint16_t usStackDepth;				//�����ջ��С
    void *pvParameters;						//�������Ĳ���
    UBaseType_t uxPriority;				//��������ȼ�
    TaskHandle_t *pxCreatedTask;  //������
};

struct TaskHandle task_handler[] = 
{		//������														 //��������							//�����ջ��С  //�������Ĳ��� //��������ȼ�										 //������
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


/*-----------------------------����--------------------------*/

volatile uint32_t   flash_offset = 0;   //�洢���ڲ�flash�Ľṹ��ƫ����

float g_temp=0.0;
float g_humi=0.0;

/*------------------------------END--------------------------*/



/*	
//	//�������ȼ�����4, ֻ֧����ռ���ȼ��� ��ռ���ȼ���С( 0~15)
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
//	delay_init(168);
//	USART1_Init(115200);
//	USART2_Init(9600);
//	// ���� stdout Ϊ USART1
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
//	MFRC522_Initializtion();        //��ʼ��RFID
//	
//	USART_Send( USART2, "Usart_SendStr\n");
//	
//	char string_buff[6] = {0};
//	uint8_t i = 0;
//	while(1)
//	{
//		MFRC522_Initializtion();        //��ʼ��RFID
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
//			OLED_ShowChar   ( 0, 4, buff, 16);//��ʾASCII�ַ�
//			printf("%c\n", buff);
//		}
//	}
*/
/*----------------------------------------------------*/
//	/* ���巵��ֵ */
//	BaseType_t xReturn;
//	
//	/* ����ϵͳ�ж����ȼ�����4��Ĭ��FreeRTOS������ */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
////	/* ϵͳ��ʱ���ж�Ƶ��ΪconfigTICK_RATE_HZ */
////	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);								
//	
//	/* ��ʼ������ */
//	Bluetooth_Init();  
//	//�ض���printf�������Ϊ����1 <����>
//	__stdout.USARTx = USART1;

//	// printf("111\n");   							
//	/* ����app_Sys_Init���� */
//	xReturn = xTaskCreate((TaskFunction_t )app_Sys_Init,  		    			/* ������ں��� */
//												(const char*    )"app_Sys_Init",							/* �������� */
//												(uint16_t       )512,  				    						/* ����ջ��С */
//												(void*          )NULL,				        				/* ������ں������� */
//												(UBaseType_t    )7, 					    						/* ��������ȼ� */
//												(TaskHandle_t*  )&app_Sys_Init_TaskHandler);	/* ������ƿ�ָ�� */ 

//	 if(xReturn != pdPASS)
//	{
//		printf("[main] xTaskCreate app_task_init fail\r\n");
//		while(1);
//	}
//						 
//	/* ����������� */
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
	
	//�������ȼ�����4, ֻ֧����ռ���ȼ��� ��ռ���ȼ���С( 0~15)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	delay_init(168);
	Bluetooth_Init();
	__stdout.USARTx = USART1;
	//��ʼ��Ӳ��
	OLED_Init();				 //OLED��ʼ��
	OLED_startUI();			 //OLED��ʼ����
	
	RTC_Config();				 //RTCʱ�ӳ�ʼ��
	
	BUZZER_Init();			 //��������ʼ��
	PWM_Servos_Init();	 //PWM�����ʼ��
	Servo_OFF();
	
	KEY_Init();					 //�ĸ�����������ʼ��
	key_board_init();		 //������̳�ʼ��
	
	Fingerprint_Init();	 //ָ��ģ���ʼ��
	
	MFRC522_Initializtion();			//��ʼ��MFRC522
	
	W25Q128_Init();         //��ʼ��W25Q128оƬ �Լ� SPI�ӿ�
//	W25Q128_SectorErase( Sector_Address( 0, 0));
	Passward_Init();
	delay_s(2);
	
	while(1)
	{	
		switch(fine)
		{
			case 0:	//������
				fine = Fion1_t();
			break;
			
			case 1:	//����
				fine = Fion2_t();
			break;
			
			case 2:	//ָ��
				fine = Fion3_t();
			break;
			
			case 3:	//RFID
				fine = Fion4_t();
			break;
			
			case 4:	//���˵�
				fine = Fion5_t();
			break;	
		}
	}
}



/*------------------------------������------------------------*/

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

//ʱ������
void app_Rtc_task( void * arg)
{
	
}

/*---------------------------------END--------------------------*/

/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	
}
/*-----------------------------------------------------------*/

/*------------------Ӧ�ó���ѿռ�����ʧ�ܹ���------------------------------*/

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


/*----------------------------Ӧ�ó�����й���-------------------------------*/

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


/*-----------------------Ӧ�ó����ջ�������--------------------------*/

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



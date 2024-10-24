#include "ssyy.h"
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
#include "PWM_Servos.h"


const  char 	 pass[12] = "123456";


static uint8_t  passwd[12] 	= {0};						//密码
static uint8_t 	directives[12] = {0};					//修改密码指令
static uint8_t 	bluetooth_passwd[64] = {0};			//蓝牙开锁指令
static uint8_t 	bluetooth_logshow[64] = {0};			//蓝牙开锁指令


//

//键盘密码验证
uint8_t key_passwd_verify(char * keystring)
{
	return memcmp((char *)passwd, keystring, sizeof(passwd));
}
//键盘指令验证
uint8_t key_directives_verify(char * keystring)
{
	return memcmp((char *)directives, keystring, sizeof(directives));
}
//键盘密码修改
uint8_t key_passwd_revise(void)
{
	uint8_t i = 0, j = 0;
	char pbuff[12] = {0};
	char keystring[12] = {0};
	memset(pbuff, 0, sizeof(pbuff));
	
	OLED_Clear();
	//请输入原密码
	OLED_ShowCHinese(	0, 0, 10);
	OLED_ShowCHinese(18, 0, 11);
	OLED_ShowCHinese(36, 0, 12);
	OLED_ShowCHinese(54, 0, 31);
	OLED_ShowCHinese(72, 0, 13);
	OLED_ShowCHinese(90, 0, 14);
	
	//密码输入
	while(1)
	{
		char buff = get_key_board();
		if(buff != 'N')
		{
			if(buff == '#')
			{
				i = 0;
				break;
			}
			
			if(i < 12)
				keystring[i++] = buff;
			
			OLED_ShowString(0 , 4, (u8 *)keystring, 16);
		}
	}
	if(key_passwd_verify(keystring) == 0)
	{
		OLED_Clear();
		OLED_ShowCHinese(14, 3, 13);	//密
		OLED_ShowCHinese(32, 3, 14);	//码
		OLED_ShowCHinese(50, 3, 47);	//正
		OLED_ShowCHinese(68, 3, 48);	//确
		
		delay_s(1);
		OLED_Clear();
		OLED_ShowCHinese(0, 0, 10);	//请
		OLED_ShowCHinese(18, 0, 11);	//输
		OLED_ShowCHinese(36, 0, 12);	//入
		
		while(1)
		{
			char buff = get_key_board();
			if(buff != 'N')
			{
				if(buff == '#')
				{
					if(j >= 5)
					{
//						memset(passwd, 0, sizeof(passwd));
//						memcpy(passwd, pbuff, sizeof(pbuff));
						
						user_passward_revise((uint8_t *)pbuff);
						
						return 0;
					}
					else
						continue;
				}
					
				
				if(j < 12)
					pbuff[j++] = buff;
				
				OLED_ShowString(0 , 4, (u8 *)pbuff, 16);
			}
		}
	}
	else 
	{
		OLED_Clear();
		OLED_ShowCHinese(14, 3, 13);	//密
		OLED_ShowCHinese(32, 3, 14);	//码
		OLED_ShowCHinese(50, 3, 20);	//错
		OLED_ShowCHinese(68, 3, 21);	//误
		
		delay_s(1);
		
		return 1;
	}
}
//修改主菜单显示
uint8_t menu_show(uint8_t num)
{
	//键盘密码
	OLED_ShowCHinese(18, 0, 39);
	OLED_ShowCHinese(36, 0, 40);
	OLED_ShowCHinese(54, 0, 13);
	OLED_ShowCHinese(72, 0, 14);
	//指纹选项
	OLED_ShowCHinese(18, 2, 15);
	OLED_ShowCHinese(36, 2, 16);
	OLED_ShowCHinese(54, 2, 41);
	OLED_ShowCHinese(72, 2, 42);
	//RFID选项
	OLED_ShowString (18, 4,(u8 *)"RFID", 16);
	OLED_ShowCHinese(54, 4, 41);
	OLED_ShowCHinese(72, 4, 42);
	
	switch(num)
	{
		case 1:	//修改键盘密码
		{
			OLED_ShowString ( 0, 0,(u8 *)"->", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 1;
		}
		break;	

		case 2:		//修改指纹
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"->", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 2;
		}
		break;

		case 3:		//修改RFID
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"->", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 3;
		}
		break;

//		case 4:
//		{
//			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 6,(u8 *)"->", 16);
//			
//			return 4;
//		}
//		break;	
	}
}

//修改指纹菜单显示
uint8_t Fingerprint_menu_show(uint8_t num)
{
	//指纹添加
	OLED_ShowCHinese(18, 0, 15);
	OLED_ShowCHinese(36, 0, 16);
	OLED_ShowCHinese(54, 0, 25);
	OLED_ShowCHinese(72, 0, 26);
	//指纹查看
	OLED_ShowCHinese(18, 2, 15);
	OLED_ShowCHinese(36, 2, 16);
	OLED_ShowCHinese(54, 2, 43);
	OLED_ShowCHinese(72, 2, 44);
	//指纹清空
	OLED_ShowCHinese(18, 4, 15);
	OLED_ShowCHinese(36, 4, 16);
	OLED_ShowCHinese(54, 4, 51);
	OLED_ShowCHinese(72, 4, 52);
	
	switch(num)
	{
		case 1:	//指纹添加
		{
			OLED_ShowString ( 0, 0,(u8 *)"->", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 1;
		}
		break;	

		case 2:		//指纹清空
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"->", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 2;
		}
		break;

		case 3:		//修改RFID
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"->", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 3;
		}
		break;
	
	}
}

//RFID菜单
uint8_t RFID_menu_show(uint8_t num)
{
	//RFID添加
	OLED_ShowString (18, 0,(u8 *)"RFID", 16);
	OLED_ShowCHinese(54, 0, 25);
	OLED_ShowCHinese(72, 0, 26);
	//RFID删除
	OLED_ShowString (18, 2,(u8 *)"RFID", 16);
	OLED_ShowCHinese(54, 2, 29);
	OLED_ShowCHinese(72, 2, 30);
	//RFID查看
	OLED_ShowString (18, 4,(u8 *)"RFID", 16);
	OLED_ShowCHinese(54, 4, 43);
	OLED_ShowCHinese(72, 4, 44);
	
	switch(num)
	{
		case 1:	//RFID添加
		{
			OLED_ShowString ( 0, 0,(u8 *)"->", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 1;
		}
		break;	

		case 2:	//RFID删除
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"->", 16);
			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 2;
		}
		break;

		case 3:	//RFID查看
		{
			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
			OLED_ShowString ( 0, 4,(u8 *)"->", 16);
			OLED_ShowString ( 0, 6,(u8 *)"  ", 16);
			
			return 3;
		}
		break;

//		case 4:
//		{
//			OLED_ShowString ( 0, 0,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 2,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 4,(u8 *)"  ", 16);
//			OLED_ShowString ( 0, 6,(u8 *)"->", 16);
//			
//			return 4;
//		}
//		break;	
	}
}



//初始界面
uint8_t Fion1_t(void)
{
	static char string1[16] = {0};
	static char string2[16] = {0};
	char keyboard_buff;
	uint8_t key_buff;
	OLED_Clear();
	
	g_fpm_touch_event = 0;
	
	while(1)
	{
		MFRC522_Initializtion();			//初始化MFRC522
		RTC_GetDate( RTC_Format_BIN, &RTC_DateStruct);
		RTC_GetTime( RTC_Format_BIN, &RTC_TimeStruct);

		sprintf(string1, "20%d-%02d-%02d", 
										RTC_DateStruct.RTC_Year, 
										RTC_DateStruct.RTC_Month, 
										RTC_DateStruct.RTC_Date);
		sprintf(string2, "%02d:%02d:%02d", 
										RTC_TimeStruct.RTC_Hours, 
										RTC_TimeStruct.RTC_Minutes, 
										RTC_TimeStruct.RTC_Seconds);
		OLED_ShowString ( 0, 2,(u8*)string1,16);
		OLED_ShowString ( 0, 4,(u8*)string2,16);

		
		//键盘唤醒
		if((keyboard_buff = get_key_board()) != 'N')	
			return 1;
		
		//蓝牙解锁
		if(strcmp((char *)g_USART1_recv_buf, (char *)bluetooth_passwd) == 0)	
		{

			OLED_Success_show();
			
			memset((char *)g_USART1_recv_buf, 0, sizeof(g_USART1_recv_buf));
			
			Servo_ON();
			Logs_Flash(string1, string2, "BluetoothOpen");
			delay_s(5);
			Servo_OFF();
			
			break;
		}
		//发送开锁记录
		if(strcmp((char *)g_USART1_recv_buf, (char *)bluetooth_logshow) == 0)
		{
			OLED_DataSend_show();
			uint8_t r_buff[128] = {0};
			for(uint8_t i = 3; i < 34; i++)
			{
				W25Q128_ReadData( Page_Address( 0, i, 0), r_buff, sizeof(r_buff));
				
				if(r_buff[0] == 0xFF)
					break;
				
				printf("%s\r\n", r_buff);
				memset(r_buff, 0, sizeof(r_buff));
			}
			memset((char *)g_USART1_recv_buf, 0, sizeof(g_USART1_recv_buf));
			
			break;
		}
		//指纹解锁
		if( g_fpm_touch_event)																
		{
			uint8_t i = 0;
			
			while(i < 4)
			{
				OLED_Fingerprint_run();
				OLED_ShowNum(92, 2, (4 - i), 2, 16);
				if(Fingerprint_Unlock() == 0)
				{
					OLED_Success_show();
					
					Servo_ON();
					Logs_Flash(string1, string2, "FingerprintOpen");
					delay_s(5);
					Servo_OFF();
					break;
				}
				else 
				{
					i++;
					OLED_Fail_show();
					delay_s(1);
				}
			}
			g_fpm_touch_event = 0;
		}
		
		//RFID解锁
		if(RFID_Verify() == 0)																
		{
			OLED_Success_show();
			
			Servo_ON();
			Logs_Flash(string1, string2, "RfidOpen");
			delay_s(5);
			Servo_OFF();
			break;
		}
		memset((char *)g_USART1_recv_buf, 0, sizeof(g_USART1_recv_buf));
		memset(string1, 0, sizeof(string1));
		memset(string2, 0, sizeof(string2));
		
		fpm_sleep();	
	}
	
	return 0;
}

//键盘唤醒
uint8_t Fion2_t(void)
{
	uint8_t i = 0;
	
	
	char passwdbuff[12] = {0};		//
	char keystring[12] = {0};
	
	while(1)
	{
		OLED_Clear();
		//请输入密码
		OLED_ShowCHinese(0, 0, 10);
		OLED_ShowCHinese(18, 0, 11);
		OLED_ShowCHinese(36, 0, 12);
		OLED_ShowCHinese(54, 0, 13);
		OLED_ShowCHinese(72, 0, 14);
		//密码输入
		while(1)
		{
			char buff = get_key_board();
			if(buff != 'N')
			{
				if(buff == '#')
				{
					i = 0;
					break;
				}
				
				if(i < 12)
					keystring[i++] = buff;
				
				OLED_ShowString(0 , 4, (u8 *)keystring, 16);
			}
		}
		//密码开锁判断识别密码
		if(key_passwd_verify(keystring) == 0)
		{
			OLED_Clear();
			
			OLED_ShowCHinese(14, 3, 4);	//解
			OLED_ShowCHinese(32, 3, 5);	//锁
			OLED_ShowCHinese(50, 3, 6);	//成
			OLED_ShowCHinese(68, 3, 7);	//功
			
			Servo_ON();
			delay_s(5);
			Servo_OFF();
			
			return 0;
		}

		//修改菜单界面
		else if(key_directives_verify(keystring) == 0)
		{			
			return 4;		
		}
		else 
		{
			OLED_Clear();
			
			OLED_ShowCHinese( 2, 2, 4);	//解
			OLED_ShowCHinese(20, 2, 5);	//锁
			OLED_ShowCHinese(38, 2, 8);	//失
			OLED_ShowCHinese(56, 2, 9);	//败
			delay_s(2);
		}
		memset(keystring, 0, sizeof(keystring));
	}
}


//指纹菜单操作
uint8_t Fion3_t(void)
{
	uint32_t key_sta;
	uint8_t fmp_error_code;
	uint16_t id;
	uint16_t id_total;
	
	uint8_t cont = 1;
	OLED_Clear();
	uint8_t mune_num = Fingerprint_menu_show(1);
	
	while(1)
	{
		/* 按键实现添加指纹、验证指纹、获取指纹总数、清空指纹 */	
		key_sta = get_KEY_Num();
		
		if(key_sta == 0)
		{
			cont = (cont % 3) + 1;
			mune_num = Fingerprint_menu_show(cont);
		}
		
		if(key_sta == 1)
		{
			/* 添加指纹 */
			if(mune_num == 1)
			{
				fpm_ctrl_led(FPM_LED_BLUE);
				
				OLED_Fingerprint_addrun();
				
				fmp_error_code =fpm_id_total(&id_total);

				if(fmp_error_code == 0)
				{
					/* 添加指纹*/
					fmp_error_code=fpm_enroll_auto(id_total+1);

					if(fmp_error_code == 0)
					{
						fpm_ctrl_led(FPM_LED_GREEN);					

						OLED_Add_show();
						
						BUZZER_ON();
						delay_ms(50);
						BUZZER_OFF();	
					}
					else
					{
						fpm_ctrl_led(FPM_LED_RED);
					}
//					delay_ms(100);	
					fpm_sleep();
					delay_ms(1000);	
					
					OLED_Clear();
					mune_num = Fingerprint_menu_show(mune_num);		
				}
			}
			
			//获取用户总数 
			if(mune_num == 2)
			{
				fpm_ctrl_led(FPM_LED_BLUE);
				fmp_error_code =fpm_id_total(&id_total);

				if(fmp_error_code == 0)
				{
					fpm_ctrl_led(FPM_LED_GREEN);
					
					OLED_View_show(id_total);
				
					BUZZER_ON();
					delay_ms(50);
					BUZZER_OFF();
					while(1)
					{
						uint8_t num = get_KEY_Num();
						if(num == 2)
							break;
					}
				}
				else
				{
					fpm_ctrl_led(FPM_LED_RED);
				}			
				
				delay_ms(100);		
				fpm_sleep();	
				delay_ms(1000);		

				OLED_Clear();
				mune_num = Fingerprint_menu_show(mune_num);
			}	

			
			/* 清空指纹 */
			if(mune_num == 3)
			{
				fpm_ctrl_led(FPM_LED_BLUE);
				fmp_error_code=fpm_empty();
				
				if(fmp_error_code == 0)
				{
					fpm_ctrl_led(FPM_LED_GREEN);	
					OLED_Delete_show();
					
					BUZZER_ON();
					delay_ms(50);
					BUZZER_OFF();
				}
				else
				{
					fpm_ctrl_led(FPM_LED_RED);
				}			
				delay_ms(100);		
				fpm_sleep();	
				delay_ms(1000);		
				
				OLED_Clear();
				mune_num = Fingerprint_menu_show(mune_num);
			}
		}
		if(key_sta == 2)
		{
			return 4;
		}
	}
}

//RFID操作
uint8_t Fion4_t(void)
{
	uint32_t key_sta;
	
	uint8_t cont = 1;
	OLED_Clear();
	uint8_t mune_num = RFID_menu_show(1);
	
	while(1)
	{
	
		/* 按键实现添加指纹、验证指纹、获取指纹总数、清空指纹 */	
		key_sta = get_KEY_Num();
		
		//选择键
		if(key_sta == 0)
		{
			cont = (cont % 3) + 1;
			mune_num = RFID_menu_show(cont);
		}
		//确认键
		if(key_sta == 1)
		{
			/* 添加RFID */
			if(mune_num == 1)
			{
				OLED_RFID_addrun();
				while(1)
				{
					uint8_t bu = get_KEY_Num();
					uint8_t bc = RFID_Enroll();
					if(bc == 0)
					{
						OLED_Add_show();
						delay_s(1);
						break;
					}
					else if(bc == 2)
					{
						void OLED_Full_show(void);
						delay_s(1);
						break;
					}
					if(bu == 2)
						break;
				}
			}
			
			/*清空RFID*/
			else if(mune_num == 2)
			{
				RFID_Delete();
				OLED_Delete_show();
			}	

			//获取用户总数
			else if(mune_num == 3)
			{
				uint8_t num = RFID_Get_num();
				OLED_View_show(num);
				while(1)
				{
					uint8_t bu = get_KEY_Num();
					
					if(bu == 2)
						break;
				}
			}
			OLED_Clear();
			mune_num = RFID_menu_show(cont);
		}
		
		//返回键
		if(key_sta == 2)
		{
			return 4;
		}
	}
}

//
uint8_t Fion5_t(void)
{
	uint8_t j  = 1;
	uint8_t mune_num = 0;
	OLED_Clear();
	mune_num = menu_show(1);
	
	while(1)
	{
		uint8_t KEY_num = get_KEY_Num();
		if(KEY_num == 0)	//选择键
		{
			j = (j % 3) + 1;
			uint8_t n = j;
			mune_num = menu_show(n);
		}
		if(KEY_num == 1)	//确认键
		{
			if(mune_num == 1)	
			{
				if(key_passwd_revise() == 0)
				{
					OLED_Clear();
					OLED_ShowCHinese(14, 3, 27);	//修
					OLED_ShowCHinese(32, 3, 28);	//改
					OLED_ShowCHinese(50, 3, 6);		//成
					OLED_ShowCHinese(68, 3, 7);		//功
					
					delay_s(2);
					return 0;
				}
				else 
				{
					OLED_Clear();
					mune_num = menu_show(1);
					continue;
				}
			}
			else if(mune_num == 2)
			{
				return 2;
			}
			else if(mune_num == 3)
			{
				return 3;
			}
		}
		if(KEY_num == 2)	//返回键
		{
			return 0;
		}
	}
}

//指纹解锁
uint8_t Fingerprint_Unlock(void)
{
	uint8_t fmp_error_code;

	uint16_t id;

	uint16_t id_total;

	fpm_ctrl_led(FPM_LED_BLUE);
			

	/* 参数为0xFFFF进行1:N匹配 */
	id = 0xFFFF;

	fmp_error_code = fpm_idenify_auto(&id);
				
	if(fmp_error_code == 0)
	{
		fpm_ctrl_led(FPM_LED_GREEN);
		
		BUZZER_ON();
		delay_ms(50);
		BUZZER_OFF();
		
		delay_ms(100);
//		g_fpm_touch_event = 0;
		fpm_sleep();	
		delay_ms(1000);
		
		return 0;
	}
	else
	{
		fpm_ctrl_led(FPM_LED_RED);
		
		delay_ms(100);
//		g_fpm_touch_event = 0;
		fpm_sleep();	
		delay_ms(1000);
		
		return 1;
	}
}



//记录存储
void Logs_Flash(char *date, char *time, char *string)
{
	uint8_t cont = 0;
	uint8_t r_buff[256] = {0};
	uint8_t w_buff[256] = {0};
	uint8_t buff[256] = {0};
	
	sprintf((char *)w_buff, "%s:%s:%s", date, time, string);
	printf("%s\r\n", w_buff);
	for(uint8_t i = 3; i < 34; i++)
	{
		W25Q128_ReadData( Page_Address( 0, i, 0), r_buff, sizeof(r_buff));
		if(r_buff[0] == 0xff)
		{
			cont = 1;
			W25Q128_PageProgram( Page_Address( 0, i, 0), w_buff, sizeof(w_buff));
			
			break;
		}
		memset(r_buff, 0, sizeof(r_buff));
	}
	
	if(cont == 0)
	{
		for(uint8_t i = 3; i < 33; i++)
		{
			W25Q128_SectorErase( Sector_Address( 0, i));
			W25Q128_ReadData( Page_Address( 0, (i + 1), 0), buff, sizeof(buff));
			W25Q128_PageProgram( Page_Address( 0, i, 0), buff, sizeof(buff));
		}
		W25Q128_SectorErase( Sector_Address( 0, 33));
		W25Q128_PageProgram( Page_Address( 0, 33, 0), w_buff, sizeof(w_buff));
	}
}


//初始化密码,指令
void Passward_Init(void)
{
	uint8_t r_data[256] = {0};
	uint8_t w_data1[64] = "123456";
	uint8_t w_data2[64] = "*A*A";
	uint8_t w_data3[64] = "blueon#";
	uint8_t w_data4[256] = {186,145,140,2,165};
	uint8_t w_data5[64] = "logshow#";
	
	W25Q128_ReadData( Page_Address( 0, 0, 0), r_data, sizeof(pass));
	if(strcmp((char*)r_data, pass) != 0)
	{
		for(uint8_t i = 0; i < 34; i++)
		{
			W25Q128_SectorErase( Sector_Address( 0, i));
		}

		
		W25Q128_PageProgram( Page_Address( 0, 0, 0), w_data1, sizeof(w_data1));
		W25Q128_PageProgram( Page_Address( 0, 0, 1), w_data2, sizeof(w_data2));
		W25Q128_PageProgram( Page_Address( 0, 0, 2), w_data3, sizeof(w_data3));
		W25Q128_PageProgram( Page_Address( 0, 0, 3), w_data5, sizeof(w_data5));
		W25Q128_PageProgram( Page_Address( 0, 2, 0), w_data4, sizeof(w_data4));
		
		W25Q128_ReadData( Page_Address( 0, 0, 0), passwd, 					sizeof(passwd));
		W25Q128_ReadData( Page_Address( 0, 0, 1), directives, 			sizeof(directives));
		W25Q128_ReadData( Page_Address( 0, 0, 2), bluetooth_passwd, sizeof(bluetooth_passwd));
		W25Q128_ReadData( Page_Address( 0, 0, 3), bluetooth_logshow, sizeof(bluetooth_logshow));
		W25Q128_ReadData( Page_Address( 0, 2, 0), card_enrollnum[0], sizeof(card_enrollnum[0]));
	}
	else
	{
		memset(r_data, 0, sizeof(r_data));
		
		W25Q128_ReadData( Page_Address( 0, 0, 1), directives, 			sizeof(directives));
		W25Q128_ReadData( Page_Address( 0, 0, 2), bluetooth_passwd, sizeof(bluetooth_passwd));
		W25Q128_ReadData( Page_Address( 0, 0, 3), bluetooth_logshow, sizeof(bluetooth_logshow));
		for(u8 i=0; i < 5;i++)
			printf("%d-",card_enrollnum[0][i]);
		printf("\r\n");
		
		W25Q128_ReadData( Page_Address( 0, 1, 0), r_data, sizeof(passwd));
		
		if(r_data[0] != 0xff)
		{
			W25Q128_ReadData( Page_Address( 0, 1, 0), passwd, 					sizeof(passwd));
		}
		else
		{
			W25Q128_ReadData( Page_Address( 0, 0, 0), passwd, 					sizeof(passwd));
		}
		for(uint8_t i = 0; i < 3; i++)
		{
			uint8_t abc[5] = {0};
			W25Q128_ReadData( Page_Address( 0, 2, i), abc, sizeof(abc));
			if(abc[0] != 0xFF)
			{
				W25Q128_ReadData( Page_Address( 0, 2, i), card_enrollnum[i], sizeof(card_enrollnum[i]));
			}
		}
	}
}

void user_passward_revise(uint8_t * uspasswd)
{
	memset(passwd, 0, sizeof(passwd));
	W25Q128_SectorErase( Sector_Address( 0, 1));
	W25Q128_PageProgram( Page_Address( 0, 1, 0), uspasswd, sizeof(passwd));
	W25Q128_ReadData	 ( Page_Address( 0, 1, 0), passwd, 	 sizeof(passwd));
}

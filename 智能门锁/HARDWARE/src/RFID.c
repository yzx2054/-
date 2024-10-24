#include "RFID.h"
#include "MFRC522.h"
#include "myincludes.h"
#include "myprintf.h"
#include "w25q128.h"

//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
uint8_t  card_enrollnum[3][5];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

//MFRC522测试函数
void MFRC522Test(USART_TypeDef* USARTx)
{
	// 设置 stdout 为 USARTx -> 重定向printf
	__stdout.USARTx = USARTx;
	
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	//
	if(status==0)		//如果读到卡
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Write(4, card_writebuf);				            //写卡（写卡要小心，特别是各区的块3）
		status=MFRC522_Read(4, card_readbuf);					        //读卡
		//MFRC522_Halt();												//使卡进入休眠状态
		printf("卡类型:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //卡类型显示

		//卡序列号显，最后一字节为卡的校验码
        printf("卡序列:");
		for(i=0;i<5;i++)
            printf("%02x",card_numberbuf[i]);
        printf("\r\n");

		
		printf("卡容量:%dkbits\n",card_size);              //卡容量显示，单位为Kbits

		printf("卡状态:%d\n",status);		               //读卡状态显示，正常为0
        
		//读一个块的数据显示
		printf("读卡内容:");
		for(i=0;i<2;i++)		//分两行显示
		for(j=0;j<9;j++)	//每行显示8个
		printf("%d ",card_readbuf[j+i*9]);	//16进显示
		printf("\r\n");
	}	
}

uint8_t RFID_Enroll(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	//
	if(status==0)		//如果读到卡
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Read(4, card_readbuf);					        //读卡
		//MFRC522_Halt();												//使卡进入休眠状态
		printf("卡类型:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //卡类型显示

		//卡序列号显，最后一字节为卡的校验码
		for(uint8_t i = 0; i < 3; i++)
		{
			uint8_t abc[5] = {0};
			W25Q128_ReadData( Page_Address( 0, 2, i), abc, sizeof(abc));
			if(abc[0] == 0xFF)
			{
				W25Q128_PageProgram( Page_Address( 0, 2, i), card_numberbuf, sizeof(card_numberbuf));
				
				memset(card_enrollnum, 0, sizeof(card_enrollnum));
				for(uint8_t k = 0; k < 3; k++)
				{
					uint8_t abc[5] = {0};
					W25Q128_ReadData( Page_Address( 0, 2, k), abc, sizeof(abc));
					if(abc[0] != 0xFF)
					{
						W25Q128_ReadData( Page_Address( 0, 2, k), card_enrollnum[k], sizeof(card_enrollnum[k]));
					}
				}
				return 0;
			}
			
		}return 2;
	}	
	return 1;
}


uint8_t RFID_Verify(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	//
	if(status==0)		//如果读到卡
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Read(4, card_readbuf);					        //读卡
		//MFRC522_Halt();												//使卡进入休眠状态
		printf("卡类型:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //卡类型显示

		//卡序列号显，最后一字节为卡的校验码
//		printf("卡序列:");
//		for(i=0;i<5;i++)
//            printf("%02x",card_numberbuf[i]);
//        printf("\r\n");
//		printf("mima卡序列:");
//		for(i=0;i<5;i++)
//            printf("%02x",card_enrollnum[0][i]);
//        printf("\r\n");
		
		uint8_t num = 0;
		for(uint8_t i = 0; i < 3; i++)
		{
			for(u8 j = 0; j < 5; j++)
			{
				if(card_enrollnum[i][j] != card_numberbuf[j])
				{
					num = 1;
					break;
				}
			}
			if(num == 0)
			{
				return 0;
			}
				
			
			num = 0;
		}
	}
	return 1;
}

//
uint8_t RFID_Get_num(void)
{
	uint8_t num = 0;
	for(uint8_t i = 0; i < 3; i++)
	{
		uint8_t abc[5] = {0};
		W25Q128_ReadData( Page_Address( 0, 2, i), abc, sizeof(abc));
		if(abc[0] != 0xFF)
		{
			num++;
		}
	}
	return num;
}

void RFID_Delete(void)
{
	W25Q128_SectorErase( Sector_Address( 0, 2));
}

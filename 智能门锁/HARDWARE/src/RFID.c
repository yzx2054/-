#include "RFID.h"
#include "MFRC522.h"
#include "myincludes.h"
#include "myprintf.h"
#include "w25q128.h"

//MFRC522������
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
uint8_t  card_enrollnum[3][5];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

//MFRC522���Ժ���
void MFRC522Test(USART_TypeDef* USARTx)
{
	// ���� stdout Ϊ USARTx -> �ض���printf
	__stdout.USARTx = USARTx;
	
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//Ѱ��
	//
	if(status==0)		//���������
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //ѡ��
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
		status=MFRC522_Write(4, card_writebuf);				            //д����д��ҪС�ģ��ر��Ǹ����Ŀ�3��
		status=MFRC522_Read(4, card_readbuf);					        //����
		//MFRC522_Halt();												//ʹ����������״̬
		printf("������:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //��������ʾ

		//�����к��ԣ����һ�ֽ�Ϊ����У����
        printf("������:");
		for(i=0;i<5;i++)
            printf("%02x",card_numberbuf[i]);
        printf("\r\n");

		
		printf("������:%dkbits\n",card_size);              //��������ʾ����λΪKbits

		printf("��״̬:%d\n",status);		               //����״̬��ʾ������Ϊ0
        
		//��һ�����������ʾ
		printf("��������:");
		for(i=0;i<2;i++)		//��������ʾ
		for(j=0;j<9;j++)	//ÿ����ʾ8��
		printf("%d ",card_readbuf[j+i*9]);	//16����ʾ
		printf("\r\n");
	}	
}

uint8_t RFID_Enroll(void)
{
	u8 i,j,status,card_size;
	//
	status=MFRC522_Request(0x52, card_pydebuf);			//Ѱ��
	//
	if(status==0)		//���������
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //ѡ��
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
		status=MFRC522_Read(4, card_readbuf);					        //����
		//MFRC522_Halt();												//ʹ����������״̬
		printf("������:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //��������ʾ

		//�����к��ԣ����һ�ֽ�Ϊ����У����
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
	status=MFRC522_Request(0x52, card_pydebuf);			//Ѱ��
	//
	if(status==0)		//���������
	{
		status=MFRC522_Anticoll(card_numberbuf);			            //��ײ����			
		card_size=MFRC522_SelectTag(card_numberbuf);	                //ѡ��
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//�鿨
		status=MFRC522_Read(4, card_readbuf);					        //����
		//MFRC522_Halt();												//ʹ����������״̬
		printf("������:%d  %d\r\n",card_pydebuf[0],card_pydebuf[1]);        //��������ʾ

		//�����к��ԣ����һ�ֽ�Ϊ����У����
//		printf("������:");
//		for(i=0;i<5;i++)
//            printf("%02x",card_numberbuf[i]);
//        printf("\r\n");
//		printf("mima������:");
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

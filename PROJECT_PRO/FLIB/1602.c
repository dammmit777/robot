#include "1602.h"
#include "DS1302.h"


//��ʼ��
void LCD_Init()
{
	LCD1602_Write_CMD(0x38);								//����16*2��ʾ��5*7����8λ���ݽӿ�
	LCD1602_Write_CMD(0x0c);								//����ʾ������ʾ���
	LCD1602_Write_CMD(0x06);								//������дһ���ַ����ַָ���һ���ҹ���һ
	LCD1602_Write_CMD(0x01);								//��ʾ����
	
}


//æ���
void Read_Busy()
{
	u8 busy;
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;													//LCD1602����Ϊ��״̬
	
	do
	{
		LCD1602_EN = 1;												//�ж�BIT7�Ƿ�Ϊ1
		busy = LCD1602_DB;										//��Ϊ1����busy��־����һ
		LCD1602_EN = 0;												//һֱ�ڴ�ѭ��������
	}while(busy & 0x80);										
	
}


//дָ��
void LCD1602_Write_CMD(u8 cmd)
{
	Read_Busy();														//æ���
	LCD1602_RS = 0;
	LCD1602_RW = 0;													//д����״̬
	LCD1602_DB = cmd;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//д����
void LCD1602_Write_DAT(u8 dat)
{
	Read_Busy();														//æ���
	LCD1602_RS = 1;
	LCD1602_RW = 0;													//д����״̬
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//������ʾ
void LCD_Set_Display(u8 x, u8 y)
{
	u8 addr;
	
	if(y == 0)
		addr = 0x00 + x;											//�ڵ�һ����ʾ
	else
		addr = 0x40 + x;											//�ڵڶ�����ʾ
	LCD1602_Write_CMD(addr | 0x80);					//�ڵڼ��еڼ�����ʾ
	
}


//��ʾһ���ֽ�����
void LCD_Display_OneChar(u8 X, u8 Y, u8 Data)
{
	if(Y)
		X |= 0x40;
	X |= 0x80;															//Y=1����ʾ�ڶ��У�Y=0����ʾ��һ��
	
	LCD1602_Write_CMD(X);										//X����ѡ����һλ
	LCD1602_Write_DAT(Data);								//Data����д����
	
}


//��ʾһ���ֽ��ַ�
void LCD_Display_OneStr(u8 X, u8 Y, u8 Data)
{
	LCD_Display_OneChar(X++, Y, Data/16 + '0');
	LCD_Display_OneChar(X, Y, Data%16 + '0');
	
}


//��ʾ�ַ���
void LCD_ShowStr(u8 x, u8 y, u8 *str)
{
	LCD_Set_Display(x, y);									//��ǰ�ַ�������
	
	while(*str != '\0')											//�ж��Ƿ���ʾ��
	{
		LCD1602_Write_DAT(*str++);
	}
}


//1602��ʾʱ��
void LCD_Display_Time()
{
		LCD_Display_OneStr(1, 0, TimeDat[2]);						//ʱ
		LCD_Display_OneChar(3, 0, ':');									//:
		LCD_Display_OneStr(4, 0, TimeDat[1]);						//��
		LCD_Display_OneChar(6, 0, ':');									//:
		LCD_Display_OneStr(7, 0, TimeDat[0] & 0x7f);		//��
}

//1602��ʾ����
void LCD_Display_Date()
{
		LCD_ShowStr(0, 1, "20");															//1602��ʾ�ַ���"20"
		LCD_Display_OneChar(2, 1, TimeDat[6]/16 + '0');				//��
		LCD_Display_OneChar(3, 1, TimeDat[6]%16 + '0');
		LCD_Display_OneChar(4, 1, '-');												//-
		LCD_Display_OneChar(5, 1, TimeDat[4]/16 + '0');				//��
		LCD_Display_OneChar(6, 1, TimeDat[4]%16 + '0');				
		LCD_Display_OneChar(7, 1, '-');												//-
		LCD_Display_OneChar(8, 1, TimeDat[3]/16 + '0');			//��
		LCD_Display_OneChar(9, 1, TimeDat[3]%16 + '0');
}

//1602��ʾ��
void LCD_Display_Week()
{
	LCD_ShowStr(9, 0, "Week:");									//��
	LCD_Display_OneStr(14, 0, TimeDat[5]);
}


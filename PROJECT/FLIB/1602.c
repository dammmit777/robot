#include "1602.h"


//��ʼ��
void LCD_Init()
{
	LCD1602_Write_CMD(0x38);		//����16*2��ʾ��5*7����8λ���ݽӿ�
	LCD1602_Write_CMD(0x0c);		//����ʾ������ʾ���
	LCD1602_Write_CMD(0x06);		//������дһ���ַ����ַָ���һ���ҹ���һ
	LCD1602_Write_CMD(0x01);		//��ʾ����
	
}


//æ���
void Read_Busy()
{
	u8 busy;
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	
	do
	{
		LCD1602_EN = 1;
		busy = LCD1602_DB;
		LCD1602_EN = 0;
	}while(busy & 0x80);
	
}


//дָ��
void LCD1602_Write_CMD(u8 cmd)
{
	Read_Busy();
	LCD1602_RS = 0;
	LCD1602_RW = 0;		//д����״̬
	LCD1602_DB = cmd;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//д����
void LCD1602_Write_DAT(u8 dat)
{
	Read_Busy();
	LCD1602_RS = 1;
	LCD1602_RW = 0;		//д����״̬
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//������ʾ
void LCD_Set_Display(u8 x, u8 y)
{
	u8 addr;
	
	if(y == 0)
		addr = 0x00 + x;		//�ڵ�һ����ʾ
	else
		addr = 0x40 + x;		//�ڵڶ�����ʾ
	
	LCD1602_Write_CMD(addr | 0x80);			//�ڵڼ��еڼ�����ʾ
	
}


//��ʾһ���ֽ�����
void LCD_Display_OneChar(u8 X, u8 Y, u8 Data)
{
	if(Y)
		X |= 0x40;
	X |= 0x80;		//Y=1����ʾ�ڶ��У�Y=0����ʾ��һ��
	
	LCD1602_Write_CMD(X);		//X����ѡ����һλ
	LCD1602_Write_DAT(Data);		//Data����д����
	
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
	LCD_Set_Display(x, y);			//��ǰ�ַ�������
	
	while(*str != '\0')
	{
		LCD1602_Write_DAT(*str++);
	}
}


#ifndef _LCD_H_
#define _LCD_H_

#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;


/***************
LCD1602���Ŷ���
***************/

#define LCD1602_DB P0		//DATA BUS
sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^4;



/***************
��������
*****************/

void LCD_Init(); 			//1602��ʼ��
void Read_Busy();			//æ��⺯�����ж�bit7�Ƿ�Ϊ0
											//0����1��ֹ
void LCD1602_Write_CMD(u8 cmd);			//д����
void LCD1602_Write_DAT(u8 dat);			//д����
void LCD_Set_Display(u8 x, u8 y);				//������ʾ
void LCD_Display_OneChar(u8 X, u8 Y, u8 Data); 			//��ʾһ���ֽ�����
void LCD_Display_OneStr(u8 X, u8 Y, u8 Data);				//��ʾһ���ֽ��ַ�
void LCD_ShowStr(u8 x, u8 y, u8 *str); 				//��ʾ�ַ���



#endif


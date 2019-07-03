#ifndef _LCD_H_
#define _LCD_H_

#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;


/***************
LCD1602引脚定义
***************/

#define LCD1602_DB P0		//DATA BUS
sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^4;



/***************
函数声明
*****************/

void LCD_Init(); 			//1602初始化
void Read_Busy();			//忙检测函数，判断bit7是否为0
											//0允许，1禁止
void LCD1602_Write_CMD(u8 cmd);			//写命令
void LCD1602_Write_DAT(u8 dat);			//写数据
void LCD_Set_Display(u8 x, u8 y);				//坐标显示
void LCD_Display_OneChar(u8 X, u8 Y, u8 Data); 			//显示一个字节数据
void LCD_Display_OneStr(u8 X, u8 Y, u8 Data);				//显示一个字节字符
void LCD_ShowStr(u8 x, u8 y, u8 *str); 				//显示字符串



#endif


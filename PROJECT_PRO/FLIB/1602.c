#include "1602.h"
#include "DS1302.h"


//初始化
void LCD_Init()
{
	LCD1602_Write_CMD(0x38);								//设置16*2显示，5*7点阵，8位数据接口
	LCD1602_Write_CMD(0x0c);								//开显示，不显示光标
	LCD1602_Write_CMD(0x06);								//当读或写一个字符后地址指针加一，且光标加一
	LCD1602_Write_CMD(0x01);								//显示清屏
	
}


//忙检测
void Read_Busy()
{
	u8 busy;
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;													//LCD1602设置为读状态
	
	do
	{
		LCD1602_EN = 1;												//判断BIT7是否为1
		busy = LCD1602_DB;										//若为1，则busy标志被置一
		LCD1602_EN = 0;												//一直在此循环中运行
	}while(busy & 0x80);										
	
}


//写指令
void LCD1602_Write_CMD(u8 cmd)
{
	Read_Busy();														//忙检测
	LCD1602_RS = 0;
	LCD1602_RW = 0;													//写命令状态
	LCD1602_DB = cmd;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//写数据
void LCD1602_Write_DAT(u8 dat)
{
	Read_Busy();														//忙检测
	LCD1602_RS = 1;
	LCD1602_RW = 0;													//写数据状态
	LCD1602_DB = dat;
	LCD1602_EN = 1;
	LCD1602_EN = 0;
	
}


//坐标显示
void LCD_Set_Display(u8 x, u8 y)
{
	u8 addr;
	
	if(y == 0)
		addr = 0x00 + x;											//在第一行显示
	else
		addr = 0x40 + x;											//在第二行显示
	LCD1602_Write_CMD(addr | 0x80);					//在第几行第几个显示
	
}


//显示一个字节数据
void LCD_Display_OneChar(u8 X, u8 Y, u8 Data)
{
	if(Y)
		X |= 0x40;
	X |= 0x80;															//Y=1，显示第二行，Y=0，显示第一行
	
	LCD1602_Write_CMD(X);										//X用来选择哪一位
	LCD1602_Write_DAT(Data);								//Data用来写数据
	
}


//显示一个字节字符
void LCD_Display_OneStr(u8 X, u8 Y, u8 Data)
{
	LCD_Display_OneChar(X++, Y, Data/16 + '0');
	LCD_Display_OneChar(X, Y, Data%16 + '0');
	
}


//显示字符串
void LCD_ShowStr(u8 x, u8 y, u8 *str)
{
	LCD_Set_Display(x, y);									//当前字符的坐标
	
	while(*str != '\0')											//判断是否显示完
	{
		LCD1602_Write_DAT(*str++);
	}
}


//1602显示时间
void LCD_Display_Time()
{
		LCD_Display_OneStr(1, 0, TimeDat[2]);						//时
		LCD_Display_OneChar(3, 0, ':');									//:
		LCD_Display_OneStr(4, 0, TimeDat[1]);						//分
		LCD_Display_OneChar(6, 0, ':');									//:
		LCD_Display_OneStr(7, 0, TimeDat[0] & 0x7f);		//秒
}

//1602显示日期
void LCD_Display_Date()
{
		LCD_ShowStr(0, 1, "20");															//1602显示字符串"20"
		LCD_Display_OneChar(2, 1, TimeDat[6]/16 + '0');				//年
		LCD_Display_OneChar(3, 1, TimeDat[6]%16 + '0');
		LCD_Display_OneChar(4, 1, '-');												//-
		LCD_Display_OneChar(5, 1, TimeDat[4]/16 + '0');				//月
		LCD_Display_OneChar(6, 1, TimeDat[4]%16 + '0');				
		LCD_Display_OneChar(7, 1, '-');												//-
		LCD_Display_OneChar(8, 1, TimeDat[3]/16 + '0');			//日
		LCD_Display_OneChar(9, 1, TimeDat[3]%16 + '0');
}

//1602显示周
void LCD_Display_Week()
{
	LCD_ShowStr(9, 0, "Week:");									//周
	LCD_Display_OneStr(14, 0, TimeDat[5]);
}


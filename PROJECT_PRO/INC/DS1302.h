#ifndef _DS1302_H_
#define _DS1302_H_

#include "1602.h"


#define Fosc  12000000UL								//宏定义主时钟HZ
#define DS1302_W_ADDR  0x80							//写时钟日历寄存器起始地址
#define DS1302_R_ADDR  0x81							//读时钟日历寄存器起始地址

extern u8 TimeDat[7];


//I/O口定义
sbit TSCLK = P1^0;											//时钟
sbit TIO = P1^1;												//数据
sbit TRST = P1^2;												//使能


/************************************/
					//函数声明
/************************************/
void DS1302_W_Byte(u8 dat);							//写一字节数据
void DS1302_W_DAT(u8 cmd, u8 dat);			//先写命令后写数据
void DS1302_Clear_WP();									//清除写保护
void DS1302_Set_WP();										//设置写保护
void Set_DS1302_Time(u8 addr);					//设置DS1302时钟日历数据
void Read_DS1302_Time(u8 addr);					//读取DS1302时钟数据
void Delay_Ms(u16 ms);									//延时函数
u8 DS1302_R_Byte();											//读一字节数据
u8 DS1302_R_DAT(u8 cmd);								//先写入命令后读出对应数据


#endif


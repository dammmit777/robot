#include "DS1302.h"
#include "reg52.h"


/*****************************************************************
时钟日历暂存数组。秒、分、时、日、月、周、年
初值为--19年7月2日周2  12：00：00
******************************************************************/
u8 TimeDat[7] = {0, 0, 12, 2, 7, 2, 19};



//延时函数
void Delay_Ms(u16 ms)
{
	u16 i;
		do
		{
			i = Fosc / 96000;
			while(--i);						//96T per loop
		}while(--ms);
		
}


//写一字节数据
void DS1302_W_Byte(u8 dat)
{
	u8 i;
		for(i = 0; i < 8; i++)		//每次写1 bit,写8次
		{
			TSCLK = 0;						//拉低时钟总线
			TIO = dat & 0x01; 		//从最低未开始写
			TSCLK = 1;						//拉高时钟总线，DS1302把数据读走
			dat >>= 1;						//数据右移一位
		}
		
}


//读一字节数据
u8 DS1302_R_Byte()
{
	u8 i, dat;
		for(i = 0; i < 8; i++)			//每次写1 bit ，写8次
		{
			TSCLK = 0;
			dat >>= 1;								//数据右移一位，从最低位开始读
				if(TIO)
					dat |= 0x80;					//读取数据
			TSCLK = 1;
		}
	return dat;
		
}


//先写命令再写数据
void DS1302_W_DAT(u8 cmd, u8 dat)
{
	TRST = 0;				//拉低使能端
	TSCLK = 0;			
	TRST = 1;
	DS1302_W_Byte(cmd);			//写命令
	DS1302_W_Byte(dat);			//写数据
	
}


//先写命令再读数据
u8 DS1302_R_DAT(u8 cmd)
{
	u8 dat;
	TRST = 0;
	TSCLK = 0;
	TRST = 1;
	DS1302_W_Byte(cmd);			//写命令
	dat = DS1302_R_Byte();	//读数据
	
	return dat;
	
}


//清除写保护
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e, 0x00);		//把控制寄存器WP位置清零
	
}


//设置写保护
void DS1302_Set_WP()
{
	DS1302_W_DAT(0x8e, 0x80);		//把控制寄存器WP位置置1
	TRST = 0;
	TSCLK = 0;
	
}


/*****************************************************************
设置DS1302时钟日历数据
说明： 把时钟日历暂存数组TimeData数据转换为BCD码
			并写入到DS1302时钟日历寄存器中
*****************************************************************/
void Set_DS1302_Time(u8 addr)
{
	u8 a, b;
	DS1302_Clear_WP();			//清除写保护
	
	for(a = 0; a < 7; a++)		//写入七个字节的时钟初始值
	{
		b = TimeDat[a] / 10;
		TimeDat[a] %= 10;
		TimeDat[a] += b*16;		//BCD码转换
		
		DS1302_W_DAT(addr, TimeDat[a]);		//先写时钟日历起始地址，再写数据
		addr += 2;				//转向下一个寄存器
		
	}
	DS1302_Set_WP();		//开启写保护
	
}


/*******************************************************************
读取DS1302时钟数据
说明：读取DS1302时钟数据，返回数据存入时钟日历暂存数组
			TimeData（数据格式BCD码）
***********************************************************************/
void Read_DS1302_Time(u8 addr)
{
	u8 i;
	DS1302_Clear_WP();			//清除写保护
	
	for(i = 0; i < 7; i++)
	{
		TimeDat[i] = DS1302_R_DAT(addr);				//先写入要读取数据的寄存器起始地址
		addr += 2;
		
	}
	DS1302_Set_WP();			//开启写保护
	
}


#include "key.h"


//中断初始化函数
void Inter_Init()
{
	EX0 = 1;											//外部中断0开启
	IT0 = 0;											//外部中断0设置为低电平触发
	EA = 1;												//CPU总中断开启
	
}


//矩阵键盘扫描函数
u8 Key_Scan()
{
	u8 row, line;									//定义行列值
	P2 = 0x0f;										//定义列线P2.7~P2.4为 0000；
																//定义行线P2.3~P2.0为	1111
	
	if((P2 & 0x0f) != 0x0f)				//判断有无按键按下
	{
		Delay_Ms(5);								//延时消抖
		if((P2 & 0x0f) != 0x0f)			//判断有无按键按下
		{
			row = P2 & 0x0f;					//判断哪一行电平被拉低
			P2 = row | 0xf0;					//列线电平反转
			line = P2 & 0xf0;					//判断哪一列电平被拉低
			while((P2 & 0xf0) != 0xf0)
			{;}												//松手检测
		}
	}
	
	return (row + line);
		
}


/***********************************************************************
																Key_Value
   K0 = 0xee;			K1 = 0xde;			K2 = 0xbe;			K3 = 0x7e;
   K4 = 0xed;			K5 = 0xdd;			K6 = 0xbd;			K7 = 0x7d;
   K8 = 0xeb;	 		K9 = 0xdb;	 	LEFT = 0xbb;	 RIGHT = 0x7b;
 STOP = 0xe7;	   RUN = 0xd7;    DATE = 0xb7;    WEEK = 0x77;

**************************************************************************/



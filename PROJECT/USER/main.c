/***********************************************************************
										2019年暑期智能仪器课程设计
组别：3组
课题：基于Proteus利用中断实现时间切换的HD44780液晶显示控制设计
小组成员：宋世淼、韩忠利、樊玲、何瑞彬、房晨、孙久瑞

工程实现：矩阵式按键采用中断方式处理。无按键按下时，主程序实现
					显示当前时间；按下按键1后，中断子程序实现显示当前日期（年月日）；
					按下按键2后，中断子程序实现显示当前是周几。
工程完成日期：
*********************************************************************/

#include "DS1302.h"
#include "key.h"


void main()				//主函数
{
	Inter_Init();			//中断初始化
	Key_Init();				//按键初始化
	LCD_Init();				//1602初始化
	Set_DS1302_Time(DS1302_W_ADDR);			//先写入时钟日历寄存器起始地址再设置
																			//时钟日历初值
	
	while(1)
	{
		Read_DS1302_Time(DS1302_R_ADDR);	//先写入时钟日历寄存器起始地址再读出
																			//时钟日历写入到TimeData数组中
		
		LCD_ShowStr(0, 0, "Time:");					//1602显示字符串"Time:"
		LCD_Display_OneStr(6, 0, TimeDat[2]);		//时
		LCD_Display_OneChar(8, 0, ':');						//:
		LCD_Display_OneStr(9, 0, TimeDat[1]);		//分
		LCD_Display_OneChar(11, 0, ':');						//:
		LCD_Display_OneStr(12, 0, TimeDat[0]);		//秒
		Delay_Ms(1000);
	}
	
}



//EX0外部中断函数
void Inter_K1()		interrupt 0
{
	LCD1602_Write_CMD(0x01);										//清屏
	
	LCD_ShowStr(0, 1, "Date:");									//1602显示字符串"Date:"
	LCD_Display_OneStr(6, 1, TimeDat[6]);				//年
	LCD_Display_OneChar(8, 1, '-');							//-
	LCD_Display_OneStr(9, 1, TimeDat[4]);				//月
	LCD_Display_OneChar(11, 1, '-');						//-
	LCD_Display_OneStr(12, 1, TimeDat[3]);			//日
	Delay_Ms(1000);
	
	if(K1 == 1)
		LCD1602_Write_CMD(0x01);									//清屏
	
}



//EX1外部中断函数
void Inter_K2()		interrupt	2
{
	LCD1602_Write_CMD(0x01);										//清屏
	LCD_ShowStr(0, 1, "week:");									//1602显示字符串"week:"
	LCD_Display_OneStr(6, 1, TimeDat[5]);				//周
	Delay_Ms(1000);
	
	if(K2 == 1)
		LCD1602_Write_CMD(0x01);									//清屏
	
}


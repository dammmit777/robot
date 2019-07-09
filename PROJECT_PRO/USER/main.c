/***********************************************************************
										2019年暑期智能仪器课程设计
组别：3组
课题：基于Proteus利用中断实现时间切换的HD44780液晶显示控制设计
小组成员：宋世淼、韩忠利、樊玲、何瑞彬、房晨、孙久瑞

工程实现：1、上电显示当前时间，矩阵键盘控制时间、日期、周数的显示及修改
					2、按下DATE键显示当前日期，按下WEEK键显示当前是周几
					3、按下STOP键停止走时，1602显示光标，此时可以设置时间；其中
						 LEFT、RIGHT键控制光标的左右移动；K0~K9键实现对时间的修改
					4、按下RUN键继续走时
工程完成日期：2019年7月6日
*********************************************************************/

#include "DS1302.h"
#include "key.h"


u8 Key_Flag[16] = {0};								//定义按键按下标志，按下为1；未按下为0
u8 DATE_Flag = 9;											//定义更改日期时光标的位置
u8 TIME_Flag = 8;											//定义更改时间时光标的位置
u8 KEY_NUM;														//定义K0~K9的按下标志
u8 var;																//定义中间变量，用作BCD码转换

void SET_DATE_GE(u8 addr, u8 x);
void SET_DATE_SHI(u8 addr, u8 x);			//设置时间

//主函数
void main()				
{
	u8 i;
	Inter_Init();												//中断初始化
	LCD_Init();													//1602初始化
	Set_DS1302_Time(DS1302_W_ADDR);			//先写入时钟日历寄存器起始地址再设置
																			//时钟日历初值
	
	Key_Scan();													//矩阵键盘扫描
	
	while(1)
	{
		Read_DS1302_Time(DS1302_R_ADDR);								//先写入时钟日历寄存器起始地址再
																										//读出时钟日历写入到TimeData数组中
		LCD_Display_Time();															//1602显示时间
		Delay_Ms(1000);
		
		
		//DATE键被按下时执行此部分
		while(Key_Flag[14] == 1)											
		{
			LCD1602_Write_CMD(0x01);										//清屏			
			LCD_Display_Date();														//1602显示日期
			Delay_Ms(2000);															//延时
			LCD1602_Write_CMD(0x01);										//清屏
			Key_Flag[14] = 0;														//标志位清零	
			
		}
		
		
		//WEEK键被按下时执行此部分
		while(Key_Flag[15] == 1)
		{
			LCD1602_Write_CMD(0x01);										//清屏
			LCD_Display_Week();													//显示周
			Delay_Ms(2000);															//延时
			LCD1602_Write_CMD(0x01);										//清屏
			Key_Flag[15] = 0;														//标志位清零
			
		}
	
		
		/******************************************/		
		//STOP键被按下时执行此部分,此部分为设置时间
		/******************************************/
		while(Key_Flag[12] == 1)											//STOP键被按下
		{

			DS1302_Clear_WP();													//清除写保护
			DS1302_W_DAT(0x80, 0x80 | TimeDat[0]);			//停止走时
			DS1302_Set_WP();														//开启写保护
			LCD_Display_Time();													//显示时间
			LCD1602_Write_CMD(0x0f);										//显示光标
			
			if(Key_Flag[14] == 0 && Key_Flag[15] == 0)	//若DATE和WEEK都未被按下
			{
				LCD_Set_Display(TIME_Flag, 0);
				Delay_Ms(1000);
			}
			
			switch(TIME_Flag)
			{
				case 1:SET_DATE_SHI(0x84, 2);break;						//光标在时的十位上
				case 2:SET_DATE_GE(0x84, 2);break;						//光标在时的个位上
				case 4:SET_DATE_SHI(0x82, 1);break;						//光标在分的十位上
				case 5:SET_DATE_GE(0x82, 1);break;						//光标在分的个位上
				case 7:SET_DATE_SHI(0x80, 0);break;						//光标在秒的十位上
				case 8:SET_DATE_GE(0x80, 0);break;						//光标在秒的个位上
				default:break;
			}
							
			if(Key_Flag[10] == 1)												//LEFT键被按下
			{
				TIME_Flag--;
				if(TIME_Flag == 0)
					TIME_Flag = 8;
				Key_Flag[10] = 0;
			}
			
			if(Key_Flag[11] == 1)												//RIGHT键被按下
			{
				TIME_Flag++;
				if(TIME_Flag == 9)
					TIME_Flag = 1;
				Key_Flag[11] = 0;
			}
			
			/***********************/
			//DATE键被按下，更改日期
			/***********************/			
			if(Key_Flag[14] == 1)													//DATE键被按下
			{
				Key_Flag[15] = 0;														//WEEK键标志清零
				LCD1602_Write_CMD(0x01);										//清屏
				LCD_Display_Date();													//显示日期
				LCD_Set_Display(DATE_Flag, 1);
				Delay_Ms(1000);
				
				switch(DATE_Flag)
				{
					case 2:SET_DATE_SHI(0x8c, 6);break;						//光标在年的十位上					
					case 3:SET_DATE_GE(0x8c, 6);break;						//光标在年的个位上
					case 5:SET_DATE_SHI(0x88, 4);break;						//光标在月的十位上
					case 6:SET_DATE_GE(0x88, 4);break;						//光标在月的个位上
					case 8:SET_DATE_SHI(0x86, 3);break;						//光标在日的十位上
					case 9:SET_DATE_GE(0x86, 3);break;						//光标在日的个位上
					default:break;
				}
			
								
				if(Key_Flag[10] == 1)												//LEFT键被按下
				{
					DATE_Flag--;
					if(DATE_Flag == 1)
						DATE_Flag = 9;
					Key_Flag[10] = 0;
				}
				
				if(Key_Flag[11] == 1)												//RIGHT键被按下
				{
					DATE_Flag++;
					if(DATE_Flag == 10)
						DATE_Flag = 2;
					Key_Flag[11] = 0;
				}
			}				
					

			/*******************/
			//WEEK键被按下,更改周
			/*******************/			
			if(Key_Flag[15] == 1)													//WEEK键被按下											
			{
				Key_Flag[14] = 0;														//DATE键标志清零
				LCD1602_Write_CMD(0x01);										//清屏
				LCD_Display_Week();													//显示周	
				LCD_Set_Display(15, 0);
				Delay_Ms(1000);
				
				if(Key_Flag[KEY_NUM] == 1)
				{
					DS1302_Clear_WP();													//清除写保护
					TimeDat[5] = KEY_NUM ;
					DS1302_W_DAT(0x8a, TimeDat[5]);							//周数写为周KEY_NUM
					DS1302_Set_WP();														//开启写保护
					Key_Flag[KEY_NUM] = 0;											//标志位清零
				}
			}	

			
			/************/
			//RUN键被按下
			/************/
			if(Key_Flag[13] == 1)													//RUN键被按下													
			{
				LCD1602_Write_CMD(0x01);										//清屏
				DS1302_Clear_WP();													//清除写保护
				DS1302_W_DAT(0x80, 0x7f & TimeDat[0]);			//开始走时
				DS1302_Set_WP();														//开启写保护
				LCD1602_Write_CMD(0x0c);										//不显示光标
				
				for(i = 0; i < 16; i++)											//所有标志位清零
				{
					Key_Flag[i] = 0;
				}
			}
/**************************STOP_OVER******************************************************/
		}	
	}

}


//EX0外部中断函数
void Display_Control()		interrupt 0
{
	switch(Key_Scan())
	{
		//K0被按下
		case 0xee:KEY_NUM = 0;Key_Flag[0] = 1;break;
		
		//K1被按下
		case 0xde:KEY_NUM = 1;Key_Flag[1] = 1;break;
		
		//K2被按下
		case 0xbe:KEY_NUM = 2;Key_Flag[2] = 1;break;
		
		//K3被按下
		case 0x7e:KEY_NUM = 3;Key_Flag[3] = 1;break;
		
		//K4被按下	
		case 0xed:KEY_NUM = 4;Key_Flag[4] = 1;break;
		
		//K5被按下
		case 0xdd:KEY_NUM = 5;Key_Flag[5] = 1;break;
		
		//K6被按下
		case 0xbd:KEY_NUM = 6;Key_Flag[6] = 1;break;
		
		//K7被按下
		case 0x7d:KEY_NUM = 7;Key_Flag[7] = 1;break;
			
		//K8被按下
		case 0xeb:KEY_NUM = 8;Key_Flag[8] = 1;break;
		
		//K9被按下
		case 0xdb:KEY_NUM = 9;Key_Flag[9] = 1;break;
		
		//LEFT被按下
		case 0xbb:Key_Flag[10] = 1;break;
		
		//RIGNT被按下
		case 0x7b:Key_Flag[11] = 1;break;
			
		//STOP被按下
		case 0xe7:Key_Flag[12] = 1;break;
		
		//RUN被按下
		case 0xd7:Key_Flag[13] = 1;break;
		
		//DATE被按下
		case 0xb7:Key_Flag[14] = 1;break;
		
		//WEEK被按下
		case 0x77:Key_Flag[15] = 1;break;
		default:	break;
		
	}
	
}


//设置时间
void SET_DATE_SHI(u8 addr, u8 x)								
{
	if(Key_Flag[KEY_NUM] == 1)
	{						
		TimeDat[x] =KEY_NUM*10 + TimeDat[x]%10;
		var = TimeDat[x]/10;
		TimeDat[x] %= 10;
		TimeDat[x] += var*16;													//BCD码转换
		DS1302_Clear_WP();													//清除写保护
		DS1302_W_DAT(addr, TimeDat[x]);
		DS1302_Set_WP();														//开启写保护
		Key_Flag[KEY_NUM] = 0;
	}
	
}


void SET_DATE_GE(u8 addr, u8 x)
{
	if(Key_Flag[KEY_NUM] == 1)
	{						
		TimeDat[x] =KEY_NUM + TimeDat[x]/10;
		var = TimeDat[x]/10;
		TimeDat[x] %= 10;
		TimeDat[x] += var*16;													//BCD码转换
		DS1302_Clear_WP();													//清除写保护
		DS1302_W_DAT(addr, TimeDat[x]);
		DS1302_Set_WP();														//开启写保护
		Key_Flag[KEY_NUM] = 0;
		
	}					
}


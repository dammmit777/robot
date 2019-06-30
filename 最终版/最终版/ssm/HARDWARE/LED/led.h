#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED0 PEout(8)	// DS0
#define LED1 PEout(9)	// DS1	
#define LED2 PEout(10)	// DS2
#define LED3 PEout(11)	// DS3
#define LED4 PEout(12)	// DS4
#define LED5 PEout(13)	// DS5


#define GS1 PDin(3)
#define GS2 PDin(4)
#define GS3 PDin(5)
#define GS4 PDin(6)
#define GS5 PDin(7)
#define KEY1 PDin(8)


void GS_Init(void);//灰度初始化
void LED_Init(void);//初始化		 				    
#endif

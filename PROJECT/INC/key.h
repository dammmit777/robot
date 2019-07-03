#ifndef _KEY_H_
#define _KEY_H_


#include "reg52.h"


//IO定义
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit COM = P3^7;


//函数定义
void Key_Init();			//按键识别
void Inter_Init();		//中断初始化



#endif


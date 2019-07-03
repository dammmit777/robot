#include "key.h"


void Key_Init()			//按键初始化
{
	K1 = 1;
	K2 = 1;
	COM = 0;
	
}

void Inter_Init()		//中断初始化
{
	EX0 = 1;
	EX1 = 1;
	EA = 1;
	
}
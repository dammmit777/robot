#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
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


void GS_Init(void);//�Ҷȳ�ʼ��
void LED_Init(void);//��ʼ��		 				    
#endif

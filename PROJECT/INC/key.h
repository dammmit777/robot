#ifndef _KEY_H_
#define _KEY_H_


#include "reg52.h"


//IO����
sbit K1 = P3^2;
sbit K2 = P3^3;
sbit COM = P3^7;


//��������
void Key_Init();			//����ʶ��
void Inter_Init();		//�жϳ�ʼ��



#endif


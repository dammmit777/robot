#ifndef _DS1302_H_
#define _DS1302_H_

#include "1602.h"


#define Fosc  12000000UL								//�궨����ʱ��HZ
#define DS1302_W_ADDR  0x80							//дʱ�������Ĵ�����ʼ��ַ
#define DS1302_R_ADDR  0x81							//��ʱ�������Ĵ�����ʼ��ַ

extern u8 TimeDat[7];


//I/O�ڶ���
sbit TSCLK = P1^0;											//ʱ��
sbit TIO = P1^1;												//����
sbit TRST = P1^2;												//ʹ��


/************************************/
					//��������
/************************************/
void DS1302_W_Byte(u8 dat);							//дһ�ֽ�����
void DS1302_W_DAT(u8 cmd, u8 dat);			//��д�����д����
void DS1302_Clear_WP();									//���д����
void DS1302_Set_WP();										//����д����
void Set_DS1302_Time(u8 addr);					//����DS1302ʱ����������
void Read_DS1302_Time(u8 addr);					//��ȡDS1302ʱ������
void Delay_Ms(u16 ms);									//��ʱ����
u8 DS1302_R_Byte();											//��һ�ֽ�����
u8 DS1302_R_DAT(u8 cmd);								//��д������������Ӧ����


#endif


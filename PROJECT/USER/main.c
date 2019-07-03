/***********************************************************************
										2019���������������γ����
���3��
���⣺����Proteus�����ж�ʵ��ʱ���л���HD44780Һ����ʾ�������
С���Ա�������������������ᡢ����򡢷����������

����ʵ�֣�����ʽ���������жϷ�ʽ�����ް�������ʱ��������ʵ��
					��ʾ��ǰʱ�䣻���°���1���ж��ӳ���ʵ����ʾ��ǰ���ڣ������գ���
					���°���2���ж��ӳ���ʵ����ʾ��ǰ���ܼ���
����������ڣ�
*********************************************************************/

#include "DS1302.h"
#include "key.h"


void main()				//������
{
	Inter_Init();			//�жϳ�ʼ��
	Key_Init();				//������ʼ��
	LCD_Init();				//1602��ʼ��
	Set_DS1302_Time(DS1302_W_ADDR);			//��д��ʱ�������Ĵ�����ʼ��ַ������
																			//ʱ��������ֵ
	
	while(1)
	{
		Read_DS1302_Time(DS1302_R_ADDR);	//��д��ʱ�������Ĵ�����ʼ��ַ�ٶ���
																			//ʱ������д�뵽TimeData������
		
		LCD_ShowStr(0, 0, "Time:");					//1602��ʾ�ַ���"Time:"
		LCD_Display_OneStr(6, 0, TimeDat[2]);		//ʱ
		LCD_Display_OneChar(8, 0, ':');						//:
		LCD_Display_OneStr(9, 0, TimeDat[1]);		//��
		LCD_Display_OneChar(11, 0, ':');						//:
		LCD_Display_OneStr(12, 0, TimeDat[0]);		//��
		Delay_Ms(1000);
	}
	
}



//EX0�ⲿ�жϺ���
void Inter_K1()		interrupt 0
{
	LCD1602_Write_CMD(0x01);										//����
	
	LCD_ShowStr(0, 1, "Date:");									//1602��ʾ�ַ���"Date:"
	LCD_Display_OneStr(6, 1, TimeDat[6]);				//��
	LCD_Display_OneChar(8, 1, '-');							//-
	LCD_Display_OneStr(9, 1, TimeDat[4]);				//��
	LCD_Display_OneChar(11, 1, '-');						//-
	LCD_Display_OneStr(12, 1, TimeDat[3]);			//��
	Delay_Ms(1000);
	
	if(K1 == 1)
		LCD1602_Write_CMD(0x01);									//����
	
}



//EX1�ⲿ�жϺ���
void Inter_K2()		interrupt	2
{
	LCD1602_Write_CMD(0x01);										//����
	LCD_ShowStr(0, 1, "week:");									//1602��ʾ�ַ���"week:"
	LCD_Display_OneStr(6, 1, TimeDat[5]);				//��
	Delay_Ms(1000);
	
	if(K2 == 1)
		LCD1602_Write_CMD(0x01);									//����
	
}


/***********************************************************************
										2019���������������γ����
���3��
���⣺����Proteus�����ж�ʵ��ʱ���л���HD44780Һ����ʾ�������
С���Ա�������������������ᡢ����򡢷����������

����ʵ�֣�1���ϵ���ʾ��ǰʱ�䣬������̿���ʱ�䡢���ڡ���������ʾ���޸�
					2������DATE����ʾ��ǰ���ڣ�����WEEK����ʾ��ǰ���ܼ�
					3������STOP��ֹͣ��ʱ��1602��ʾ��꣬��ʱ��������ʱ�䣻����
						 LEFT��RIGHT�����ƹ��������ƶ���K0~K9��ʵ�ֶ�ʱ����޸�
					4������RUN��������ʱ
����������ڣ�2019��7��6��
*********************************************************************/

#include "DS1302.h"
#include "key.h"


u8 Key_Flag[16] = {0};								//���尴�����±�־������Ϊ1��δ����Ϊ0
u8 DATE_Flag = 9;											//�����������ʱ����λ��
u8 TIME_Flag = 8;											//�������ʱ��ʱ����λ��
u8 KEY_NUM;														//����K0~K9�İ��±�־
u8 var;																//�����м����������BCD��ת��

void SET_DATE_GE(u8 addr, u8 x);
void SET_DATE_SHI(u8 addr, u8 x);			//����ʱ��

//������
void main()				
{
	u8 i;
	Inter_Init();												//�жϳ�ʼ��
	LCD_Init();													//1602��ʼ��
	Set_DS1302_Time(DS1302_W_ADDR);			//��д��ʱ�������Ĵ�����ʼ��ַ������
																			//ʱ��������ֵ
	
	Key_Scan();													//�������ɨ��
	
	while(1)
	{
		Read_DS1302_Time(DS1302_R_ADDR);								//��д��ʱ�������Ĵ�����ʼ��ַ��
																										//����ʱ������д�뵽TimeData������
		LCD_Display_Time();															//1602��ʾʱ��
		Delay_Ms(1000);
		
		
		//DATE��������ʱִ�д˲���
		while(Key_Flag[14] == 1)											
		{
			LCD1602_Write_CMD(0x01);										//����			
			LCD_Display_Date();														//1602��ʾ����
			Delay_Ms(2000);															//��ʱ
			LCD1602_Write_CMD(0x01);										//����
			Key_Flag[14] = 0;														//��־λ����	
			
		}
		
		
		//WEEK��������ʱִ�д˲���
		while(Key_Flag[15] == 1)
		{
			LCD1602_Write_CMD(0x01);										//����
			LCD_Display_Week();													//��ʾ��
			Delay_Ms(2000);															//��ʱ
			LCD1602_Write_CMD(0x01);										//����
			Key_Flag[15] = 0;														//��־λ����
			
		}
	
		
		/******************************************/		
		//STOP��������ʱִ�д˲���,�˲���Ϊ����ʱ��
		/******************************************/
		while(Key_Flag[12] == 1)											//STOP��������
		{

			DS1302_Clear_WP();													//���д����
			DS1302_W_DAT(0x80, 0x80 | TimeDat[0]);			//ֹͣ��ʱ
			DS1302_Set_WP();														//����д����
			LCD_Display_Time();													//��ʾʱ��
			LCD1602_Write_CMD(0x0f);										//��ʾ���
			
			if(Key_Flag[14] == 0 && Key_Flag[15] == 0)	//��DATE��WEEK��δ������
			{
				LCD_Set_Display(TIME_Flag, 0);
				Delay_Ms(1000);
			}
			
			switch(TIME_Flag)
			{
				case 1:SET_DATE_SHI(0x84, 2);break;						//�����ʱ��ʮλ��
				case 2:SET_DATE_GE(0x84, 2);break;						//�����ʱ�ĸ�λ��
				case 4:SET_DATE_SHI(0x82, 1);break;						//����ڷֵ�ʮλ��
				case 5:SET_DATE_GE(0x82, 1);break;						//����ڷֵĸ�λ��
				case 7:SET_DATE_SHI(0x80, 0);break;						//��������ʮλ��
				case 8:SET_DATE_GE(0x80, 0);break;						//�������ĸ�λ��
				default:break;
			}
							
			if(Key_Flag[10] == 1)												//LEFT��������
			{
				TIME_Flag--;
				if(TIME_Flag == 0)
					TIME_Flag = 8;
				Key_Flag[10] = 0;
			}
			
			if(Key_Flag[11] == 1)												//RIGHT��������
			{
				TIME_Flag++;
				if(TIME_Flag == 9)
					TIME_Flag = 1;
				Key_Flag[11] = 0;
			}
			
			/***********************/
			//DATE�������£���������
			/***********************/			
			if(Key_Flag[14] == 1)													//DATE��������
			{
				Key_Flag[15] = 0;														//WEEK����־����
				LCD1602_Write_CMD(0x01);										//����
				LCD_Display_Date();													//��ʾ����
				LCD_Set_Display(DATE_Flag, 1);
				Delay_Ms(1000);
				
				switch(DATE_Flag)
				{
					case 2:SET_DATE_SHI(0x8c, 6);break;						//��������ʮλ��					
					case 3:SET_DATE_GE(0x8c, 6);break;						//�������ĸ�λ��
					case 5:SET_DATE_SHI(0x88, 4);break;						//������µ�ʮλ��
					case 6:SET_DATE_GE(0x88, 4);break;						//������µĸ�λ��
					case 8:SET_DATE_SHI(0x86, 3);break;						//������յ�ʮλ��
					case 9:SET_DATE_GE(0x86, 3);break;						//������յĸ�λ��
					default:break;
				}
			
								
				if(Key_Flag[10] == 1)												//LEFT��������
				{
					DATE_Flag--;
					if(DATE_Flag == 1)
						DATE_Flag = 9;
					Key_Flag[10] = 0;
				}
				
				if(Key_Flag[11] == 1)												//RIGHT��������
				{
					DATE_Flag++;
					if(DATE_Flag == 10)
						DATE_Flag = 2;
					Key_Flag[11] = 0;
				}
			}				
					

			/*******************/
			//WEEK��������,������
			/*******************/			
			if(Key_Flag[15] == 1)													//WEEK��������											
			{
				Key_Flag[14] = 0;														//DATE����־����
				LCD1602_Write_CMD(0x01);										//����
				LCD_Display_Week();													//��ʾ��	
				LCD_Set_Display(15, 0);
				Delay_Ms(1000);
				
				if(Key_Flag[KEY_NUM] == 1)
				{
					DS1302_Clear_WP();													//���д����
					TimeDat[5] = KEY_NUM ;
					DS1302_W_DAT(0x8a, TimeDat[5]);							//����дΪ��KEY_NUM
					DS1302_Set_WP();														//����д����
					Key_Flag[KEY_NUM] = 0;											//��־λ����
				}
			}	

			
			/************/
			//RUN��������
			/************/
			if(Key_Flag[13] == 1)													//RUN��������													
			{
				LCD1602_Write_CMD(0x01);										//����
				DS1302_Clear_WP();													//���д����
				DS1302_W_DAT(0x80, 0x7f & TimeDat[0]);			//��ʼ��ʱ
				DS1302_Set_WP();														//����д����
				LCD1602_Write_CMD(0x0c);										//����ʾ���
				
				for(i = 0; i < 16; i++)											//���б�־λ����
				{
					Key_Flag[i] = 0;
				}
			}
/**************************STOP_OVER******************************************************/
		}	
	}

}


//EX0�ⲿ�жϺ���
void Display_Control()		interrupt 0
{
	switch(Key_Scan())
	{
		//K0������
		case 0xee:KEY_NUM = 0;Key_Flag[0] = 1;break;
		
		//K1������
		case 0xde:KEY_NUM = 1;Key_Flag[1] = 1;break;
		
		//K2������
		case 0xbe:KEY_NUM = 2;Key_Flag[2] = 1;break;
		
		//K3������
		case 0x7e:KEY_NUM = 3;Key_Flag[3] = 1;break;
		
		//K4������	
		case 0xed:KEY_NUM = 4;Key_Flag[4] = 1;break;
		
		//K5������
		case 0xdd:KEY_NUM = 5;Key_Flag[5] = 1;break;
		
		//K6������
		case 0xbd:KEY_NUM = 6;Key_Flag[6] = 1;break;
		
		//K7������
		case 0x7d:KEY_NUM = 7;Key_Flag[7] = 1;break;
			
		//K8������
		case 0xeb:KEY_NUM = 8;Key_Flag[8] = 1;break;
		
		//K9������
		case 0xdb:KEY_NUM = 9;Key_Flag[9] = 1;break;
		
		//LEFT������
		case 0xbb:Key_Flag[10] = 1;break;
		
		//RIGNT������
		case 0x7b:Key_Flag[11] = 1;break;
			
		//STOP������
		case 0xe7:Key_Flag[12] = 1;break;
		
		//RUN������
		case 0xd7:Key_Flag[13] = 1;break;
		
		//DATE������
		case 0xb7:Key_Flag[14] = 1;break;
		
		//WEEK������
		case 0x77:Key_Flag[15] = 1;break;
		default:	break;
		
	}
	
}


//����ʱ��
void SET_DATE_SHI(u8 addr, u8 x)								
{
	if(Key_Flag[KEY_NUM] == 1)
	{						
		TimeDat[x] =KEY_NUM*10 + TimeDat[x]%10;
		var = TimeDat[x]/10;
		TimeDat[x] %= 10;
		TimeDat[x] += var*16;													//BCD��ת��
		DS1302_Clear_WP();													//���д����
		DS1302_W_DAT(addr, TimeDat[x]);
		DS1302_Set_WP();														//����д����
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
		TimeDat[x] += var*16;													//BCD��ת��
		DS1302_Clear_WP();													//���д����
		DS1302_W_DAT(addr, TimeDat[x]);
		DS1302_Set_WP();														//����д����
		Key_Flag[KEY_NUM] = 0;
		
	}					
}


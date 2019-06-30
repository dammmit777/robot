#include "timer.h"
#include "led.h"
#include "usart.h"
#include "oled.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʱ��8 ���벶������   4��ͨ��
// arr���Զ���װֵ(TIM8��16λ��!!)
//psc��ʱ��Ԥ��Ƶ��	
extern short Distance[4];
void TIM8_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef           GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	TIM_ICInitTypeDef          TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8); 
    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	TIM_ITConfig(TIM8,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM8,ENABLE ); 	//ʹ�ܶ�ʱ��5
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}


void TIM8_CC_IRQHandler(void)
{
//		static unsigned char ROLL = 1;
		static short Temp[4];
	
		//extern short TIM8_CCR[4];
		static unsigned char Edage[4] = {1,1,1,1};
//	LCD_ShowNum(30,200,TIM8->SR,10,24);
		if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
		{
				PFout(10) = !PFout(10);
		}
		if(TIM_GetITStatus(TIM8, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4) != RESET)
		{ 
				//PFout(9) = PCin(6);
				if(TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)
				{
					if(Edage[0] ==1 ) //֤������ǲ����˸ߵ�ƽ
					{ 
						Edage[0] =0;
						TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling); 
						Temp[0] = TIM8->CCR1;
					}else 
					{	
						Edage[0] =1;
						TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising); 
						if(Temp[0] > TIM8->CCR1)
							Distance[0] = 0xffff- Temp[0] + TIM8->CCR1;
						else 
							Distance[0] = TIM8->CCR1 - Temp[0];
						
						Distance[0] = Distance[0]/50;
					
						
					}					
					
				}
				if(TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET)
				{
					if(Edage[1] ==1 ) //֤������ǲ����˸ߵ�ƽ
					{ 
						Edage[1] =0;
						TIM_OC2PolarityConfig(TIM8,TIM_ICPolarity_Falling); 
						Temp[1] = TIM8->CCR2;
					}else 
					{	
						Edage[1] =1;
						TIM_OC2PolarityConfig(TIM8,TIM_ICPolarity_Rising); 
						if(Temp[1] > TIM8->CCR2)
							Distance[1] = 0xffff- Temp[1] + TIM8->CCR2;
						else 
							Distance[1] = TIM8->CCR2 - Temp[1];
						
						  Distance[1] = Distance[1]/50;
					}					
					
				}
				if(TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET)
				{
					if(Edage[2] ==1 ) //֤������ǲ����˸ߵ�ƽ
					{ 
						Edage[2] =0;
						TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Falling); 
						Temp[2] = TIM8->CCR3;
					}else 
					{	
						Edage[2] =1;
						TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Rising); 
						if(Temp[2] > TIM8->CCR3)
							Distance[2] = 0xffff- Temp[2] + TIM8->CCR3;
						else 
							Distance[2] = TIM8->CCR3 - Temp[2];
						
						  Distance[2] = Distance[2]/50;
					}					
					
				}
				if(TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET)
				{
					if(Edage[3] ==1 ) //֤������ǲ����˸ߵ�ƽ
					{ 
						Edage[3] =0;
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Falling); 
						Temp[3] = TIM8->CCR4;
					}else 
					{	
						Edage[3] =1;
						TIM_OC4PolarityConfig(TIM8,TIM_ICPolarity_Rising); 
						if(Temp[3] > TIM8->CCR4)
							Distance[3] = 0xffff- Temp[3] + TIM8->CCR4;
						else 
							Distance[3] = TIM8->CCR4 - Temp[3];
						
						  Distance[3] = Distance[3]/50;
					}					
					
				}

			
			
		}
		
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4);
}





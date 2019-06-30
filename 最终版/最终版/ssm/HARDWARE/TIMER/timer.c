#include "timer.h"
#include "led.h"
#include "usart.h"
#include "oled.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//定时器8 输入捕获配置   4个通道
// arr：自动重装值(TIM8是16位的!!)
//psc：时钟预分频数	
extern short Distance[4];
void TIM8_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef           GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	TIM_ICInitTypeDef          TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORT时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8); 
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8); 
    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	//初始化TIM输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	TIM_ITConfig(TIM8,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM8,ENABLE ); 	//使能定时器5
 
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
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
					if(Edage[0] ==1 ) //证明这次是捕获到了高电平
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
					if(Edage[1] ==1 ) //证明这次是捕获到了高电平
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
					if(Edage[2] ==1 ) //证明这次是捕获到了高电平
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
					if(Edage[3] ==1 ) //证明这次是捕获到了高电平
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





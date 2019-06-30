#include "pwm.h"
#include "delay.h"   
#include "usart.h"
#include "oled.h"
#include "led.h"
#include "sr04.h"

extern	short Distance[4];


//TIM3 PWM, TIM5 PWM, TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM_PWM3_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	//GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOB4复用为定时器3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //GPIOB5复用为定时器3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB6复用为定时器3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB7复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_0|GPIO_Pin_1;           //GPIOB4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
	
	//初始化TIM3 Channe234 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	//TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器
	
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPB使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3										  
}  




void TIM_PWM5_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA01复用为定时器5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA01复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           //GPIOA01
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 Channel PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC2
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
 
										  
}  


void TIM_PWM14_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM5 Channel PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM14 OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
 
										  
}  

















void PWM_change1(u32 anger)//1舵机角度转换
	{
 
	TIM_SetCompare2(TIM5,19490-11*anger);
	
	}
	

void PWM_change2(u32 anger)//2舵机角度转换
	{
 
	TIM_SetCompare2(TIM3,19490-11*anger);
	
	}

	

void PWM_change3(u32 anger)//3舵机角度转换
	{
 
	TIM_SetCompare3(TIM3,19490-11*anger);
	
	}

void PWM_change4(u32 anger)//4舵机角度转换
	{
 
	TIM_SetCompare4(TIM3,19490-11*anger);
	
	}
	

void PWM_change5(u32 anger)//5舵机角度转换
	{
 
	TIM_SetCompare1(TIM5,19490-11*anger);
	
	}
	

void PWM_change6(u32 anger)//6舵机角度转换
	{
 
	TIM_SetCompare1(TIM14,19490-11*anger);
	
	}



	
	
	
	

	
	

	
	
	u32 a, b, c, d, e, f;
	
int Low_speed_change1_2(u32 angle, u32 x)//1_2舵机降速运动
{
	
	if(x == 0)
		{
			for(a = 99; a > 99 - angle; a--)
				{
					PWM_change1(a);
					PWM_change2(a-9);
					delay_ms(7);
				}
		}
	if(x == 1)
		{
			for(a = 99; a < 99 + angle; a++ )
				{
					PWM_change1(a);
					PWM_change2(a - 9);
					delay_ms(7);
				}
		}
		return a;
}


void Low_angle_reset1_2(u32 x)  //1_2舵机降速复位
{
	u32 g;
	if(x == 0)
	{
		for(g = a; g < 99; g++)
		{
			PWM_change1(g);
			PWM_change2(g - 9);
			delay_ms(7);
		}
		
	}
	if(x == 1)
	{
		for(g = a; g > 99; g--)
		{
			PWM_change1(g);
			PWM_change2(g - 9);
			delay_ms(7);
		}
		
	}
	

}



int Low_speed_change3(u32 angle, u32 x)//3舵机降速运动
{
		
		if(x == 0)
			{
				for(c = 95; c > 95 - angle; c--)
					{
						PWM_change3(c);
						delay_ms(7);
					}
			}
		if(x == 1)
			{
				for(c = 95; c < 95 + angle; c++)
				{
					PWM_change3(c);
					delay_ms(7);
				}
			}
			return c;
}

void Low_angle_reset3(u32 x)  //3舵机降速复位
{
	u32 i;		
	if(x == 0)
	{
		for(i = c; i < 95; i++)
		{
			PWM_change3(i);
			delay_ms(7);
		}
	}
	if(x == 1)
	{
		for(i = c; i > 95; i--)
		{
			PWM_change3(i);
			delay_ms(7);
		}
	}


}

int Low_speed_change4(u32 angle, u32 x)//4舵机降速运动
{
		
		if(x == 0)
		{
			for(d = 94; d > 94 - angle; d--)
			{
				PWM_change4(d);
				delay_ms(7);
			}
		}
		if(x == 1)
		{
			for(d = 94; d < 94 + angle; d++)
			{
				PWM_change4(d);
				delay_ms(7);
			}
		}
		return d;
}

void Low_angle_reset4(u32 x)  //4舵机降速复位
{
	u32 j;
	if(x == 0)
	{
		for(j = d; j < 94; j++)
		{
			PWM_change4(j);
			delay_ms(7);
		}
	}
	if(x == 1)
	{
		for(j = d; j > 94; j--)
		{
			PWM_change4(j);
			delay_ms(7);
		}
	}
	
	
}

int Low_speed_change5_6(u32 angle, u32 x)//5_6舵机降速运动
{	
		if(x == 0)
		{
			for(e = 85; e > 85 - angle; e--)
			{
				PWM_change5(e);
				PWM_change6(e);
				delay_ms(7);
			}
		}
		
		if(x == 1)
		{
			for(e = 85; e < 85 + angle; e++)
			{
				PWM_change5(e);
				PWM_change6(e);
				delay_ms(7);
			}
		}
		return e;
}


void Low_angle_reset5_6(u32 x)  //5_6舵机降速复位
{
	u32 k;	
	
	if(x == 0)
	{
		for(k = e; k < 85; k++)
		{
			PWM_change5(k);
			PWM_change6(k);
			delay_ms(7);
		}
	}
	
	if(x == 1)
	{
		for(k = e; k > 85; k--)
		{
			PWM_change5(k);
			PWM_change6(k);
			delay_ms(7);
		}
	}
}


void Low_speed(u32 angle, u32 x) //1，2，5，6舵机降速运动，延时20ms
{
	u32 p;
	
	if(x == 1)
	{
		for(p = 85; p < 85 + angle; p++)
		{
			PWM_change1(p + 14);
			PWM_change2(p + 5);
			PWM_change5(p);
			PWM_change6(p);
			delay_ms(7);
		}
	}
	
	if(x == 0)
	{
		for(p = 85; p > 85 - angle; p--)
		{

			PWM_change1(p + 14);
			PWM_change2(p + 5);
			PWM_change5(p);
			PWM_change6(p);
			delay_ms(7);
		}
	}
}


void low_speed(u32 angle, u32 x) // 1，2,5,6舵机降速运动，延时100ms
{
	u32 ii;
	
	if(x == 1)
	{
		for(ii = 85; ii < 85 + angle; ii++)
		{
			PWM_change1(ii + 14);
			PWM_change2(ii + 5);
			PWM_change5(ii);
			PWM_change6(ii);
			delay_ms(100);
		}
	}
	
	if(x == 0)
	{
		for(ii = 85; ii > 85 - angle; ii--)
		{
			PWM_change1(ii + 14);
			PWM_change2(ii + 5);
			PWM_change5(ii);
			PWM_change6(ii);
			delay_ms(100);
		}
	}
}


void low_speed2(u32 angle, u32 x) // 3，4舵机降速运动，延时100ms
{
	u32 jj;
	
	if(x == 1)
	{
		for(jj = 94; jj < 94 + angle; jj++)
		{
			PWM_change3(jj + 4);
			PWM_change4(jj);
			delay_ms(100);
		}
	}
	
	if(x == 0)
	{
		for(jj = 94; jj > 94 - angle; jj--)
		{
			PWM_change3(jj + 4);
			PWM_change4(jj);
			delay_ms(100);
		}
	}
}


void Low_speed2(u32 angle, u32 x) // 3，4舵机降速运动，延时100ms
{
	u32 q;
	
	if(x == 1)
	{
		for(q = 94; q < 94 + angle; q++)
		{
			PWM_change3(q + 4);
			PWM_change4(q);
			delay_ms(7);
		}
	}
	
	if(x == 0)
	{
		for(q = 94; q > 94 - angle; q--)
		{
			PWM_change3(q + 4);
			PWM_change4(q);
			delay_ms(7);
		}
	}
}



void Start_Init()//启动初始化函数
{
		u32 anger;
		if(anger<170)anger+=10;
		else anger = 10;
		PWM_change1(99);	//修改参数改1号舵机变角度值（10-170）PA1
		PWM_change2(90);	//修改参数改2号舵机变角度值（10-170）PB5
		PWM_change3(98);	//修改参数改3号舵机变角度值（10-170）PB0
		PWM_change4(94);	//修改参数改4号舵机变角度值（10-170）PB1
		PWM_change5(85);	//修改参数改5号舵机变角度值（10-170）PA0
		PWM_change6(85);	//修改参数改6号舵机变角度值（10-170）PF9
}


void Go()	//直走
{

		Low_speed2(17, 1);
		Low_speed(25, 1);
		Low_speed2(17, 0);
		Low_speed(25, 0);
		
}


void Low_Go()	//慢速直走,角度有待调整
{
	Low_speed_change4(22, 0);
		delay_ms(1000);
		Low_speed_change3(22, 0);
		delay_ms(1000);
		Low_angle_reset4(0);
		delay_ms(1000);
		Low_speed_change4(10, 0);
		delay_ms(1000);
		low_speed(30, 0);
		delay_ms(1000);
		Low_angle_reset3(0);
		Low_angle_reset4(0);
		delay_ms(1000);
		
		Low_speed_change3(22, 1);
		delay_ms(1000);
		Low_speed_change4(22, 1);
		delay_ms(1000);
		low_speed(30, 1);
		delay_ms(1000);
		Low_angle_reset4(1);
		Low_angle_reset3(1);
		delay_ms(1000);
}


void Turn_right()	//右转
{		
		Low_speed2(16, 1);
		Low_speed_change4(16, 1);
		Low_speed_change1_2(25, 1);	
		Low_angle_reset4(1);
		Low_angle_reset1_2(1);
		delay_ms(50);
}


void Turn_left()	// 左转
{

		Low_speed_change3(16, 0);
		Low_speed_change5_6(25, 0);
		Low_angle_reset3(0);
		Low_angle_reset5_6(0);
		delay_ms(50);
	
		
}









//通用定时器4中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{		
	u8 x=0;

	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		
		Trig1_Pin = 1;
		delay_us(50);
		Trig1_Pin = 0;
	
			for(x= 0;x<2;x++)
		{
				OLED_ShowNum(45,x*2,Distance[x],6,24);
			
		}	
			LED0 = !LED0;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}










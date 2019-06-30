#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "led.h"
#include "pwm.h"
#include "myiic.h"
#include "stm32f4xx.h"
#include "timer.h"
#include "sr04.h"

u32 ii;
u32 jj;
u32 aaaa = 0;
u32 bbbb;
u32 cccc;
short Distance[4];
int main(void)
{    


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		uart_init(115200);//初始化串口波特率为115200
		delay_init(168); //初始化延时函数
  
		LED_Init();
		IIC_Init();
		OLED_Init();
		Read_Yaw();
		GS_Init();
	  Trig_Init();
		
		TIM_PWM5_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz. 
		TIM_PWM3_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz. 
		TIM_PWM14_Init(20000-1,84-1);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz. 
		TIM8_Cap_Init(0xffff  ,167); 
		
		Start_Init();

    Trig1_Pin = 0;
		delay_ms(3000);
		
		OLED_ShowString(0,0,"sro4-1",6);
		OLED_ShowString(0,2,"sr04-2",5);
		OLED_ShowString(0,4,"step",5);
		
	for(ii = 0; ii < 4; ii++ )
		{
				Go();
		}
	
	TIM4_Int_Init(800-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数800次为80ms    
	
	
	while(1)
	{
			switch(aaaa)
			{
					case 0:  //1阶段靠右走
					{
								if(GS1 == 0 && GS3 == 0)
									Turn_left();
								
								else if(GS1 == 1 && GS3 == 1)
									Turn_right();
								
								else
									Go();
				
								if(Distance[0] > 300&& Distance[0] < 340)
								{
									if(Distance[0] > 300 && Distance[0] < 340)
									{
										if(Distance[0] > 300 && Distance[0] < 340)
										{
												aaaa = 1;
												LED1 = 1;				// 第二阶段开始
												bbbb = 1;
										}
									}
								}
					}break;

			
					case 1: // 2阶段左边超声波检测到柱子，左转
					{
								if(bbbb == 1)
								{
									if(GS1 == 0 && GS3 == 0)
										Turn_left();
									
									else if(GS1 == 1 && GS3 == 1)
										Turn_right();
				
									delay_ms(200);
				
									for(ii = 0; ii < 4; ii++)
										Go();
										

									delay_ms(200);
				
									for(jj = 0; jj <= 10; jj++)
										Turn_left();

									delay_ms(200);
									bbbb = 0;
								}
				
				
								if(GS4 == 0)
								{
										aaaa = 2;
										delay_ms(200);
										LED2 = 1;			//第三阶段开始
								}
								
								else
								{
									Go();
								}
								
						}break;
			
			
						case 2: //3阶段靠左走
						{
//								do
//								{
//									Turn_right();
//								}while(GS2 == 1 && GS4 == 1);			//修正，防止走出场地
								
									if(GS2 == 0 && GS4 == 0)
									{
											Turn_right();			
									}
									
									else if(GS2 == 1 && GS4 == 1)
									{
									 Turn_left();
									}
									
									else
									{
									 Go();
									}
								

							if(Distance[1] > 300&& Distance[1] < 340)
									{
										if(Distance[1] > 300&& Distance[1] < 340)
										{
											if(Distance[1] > 300&& Distance[1] < 340)
											{
														aaaa = 3;
														LED3 = 1;		//第四阶段开始
											}												
										}
									}
							}break;
			
						
						case 3: // 4阶段走弯路，，中途跨越障碍
						{
								if(GS2 == 0 && GS4 == 0)
									{
											Turn_right();			
									}
									
									else if(GS2 == 1 && GS4 == 1)
									{
									 Turn_left();
									}
									
									else if(GS3 == 0 || GS1 == 0)
									{
											Start_Init();
											
											delay_ms(1000);
											for(ii = 0; ii < 2; ii++)
											{
													Low_Go();
											}
											LED1 = 0;
									}
									
									
									else
									{
									 Go();
									}
									
									if(LED1 == 0)
									{
										if(Distance[1] > 300&& Distance[1] < 340)
										{
											if(Distance[1] > 300&& Distance[1] < 340)
											{
												if(Distance[1] > 300&& Distance[1] < 340)
												{
														aaaa = 4;
														LED2 = 0;
														LED3 = 0;
														cccc = 1;
												}
											}
										}
									}
								
						}break;
						
						case 4: //5阶段右转 
						{
							u32 a;
							
								if(cccc == 1)
								{
									if(GS2 == 0 && GS4 == 0)
										Turn_right();
					
									else if(GS2 == 1 && GS4 == 1)
										Turn_left();
				
									delay_ms(200);
					
									for(ii = 0; ii < 4; ii++)
										Go();
					
									delay_ms(200);
				
									for(jj = 0; jj < 4; jj++)
										Turn_right();
					
									delay_ms(200);
									cccc = 0;
								}
				
								if(GS3 == 0)
								{
									aaaa = 5;
									LED1 = 1;
									delay_ms(200);
								}
					
								else
								{
									Go();
									a++;
									if(a == 2)
									{
										Turn_left();
										a = 0;
									}
								}
									
						}break;
						
						case 5: //5阶段靠右走
						{
							u32 kk;
							
//							do
//							{
//								Turn_left();
//							}while(GS1 == 1 && GS3 == 1);
							
								if(GS1 == 0 && GS3 == 0)
									Turn_left();
								
								else if(GS1 == 1 && GS3 == 1)
									Turn_right();
								
								else if(GS2 == 0 && GS4 == 0)
									for(kk = 0; kk < 4; kk ++)
										Go();
								else
									Go();
						}break;
			
						default:break;
				} 

		

		}
}




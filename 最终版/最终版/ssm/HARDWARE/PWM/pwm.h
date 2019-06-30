#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"



void TIM_PWM3_Init(u32 arr,u32 psc);
void TIM_PWM5_Init(u32 arr,u32 psc);
void TIM_PWM14_Init(u32 arr,u32 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void);

void PWM_change1(u32 anger1);
void PWM_change2(u32 anger2);
void PWM_change3(u32 anger3);
void PWM_change4(u32 anger4);
void PWM_change5(u32 anger5);
void PWM_change6(u32 anger6);



int Low_speed_change1_2(u32 angle1, u32 y);
int Low_speed_change3(u32 angle3, u32 y);
int Low_speed_change4(u32 angle4, u32 y);
int Low_speed_change5_6(u32 angle5, u32 y);



void Start_Init(void);
void Go(void);
void Low_Go(void);
void Turn_left(void);
void Turn_right(void);



void Low_angle_reset1_2(u32 y);
void Low_angle_reset3(u32 y);
void Low_angle_reset4(u32 y);
void Low_angle_reset5_6(u32 y);


void Low_speed(u32 angle, u32 y);
void Low_speed2(u32 angle, u32 y);
void low_speed(u32 angle, u32 x);
void low_speed2(u32 angle, u32 x);


#endif

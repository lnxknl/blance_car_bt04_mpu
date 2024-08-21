#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_8;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);       //内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInitStructure.TIM_Period =  7200- 1;    //ARR(0~65535)       1KHz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;  //PSC(0~65535)      定时频率=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //重复计数器（高级计数器特有）	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);   //结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;    //CCR     占空比=CCR/(ARR+1)，分辨率=1/(ARR+1)
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);   //初始化通道
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);   //初始化通道
	
	//控制波形是立即生效还是定时器发生下一次更新事件时被更新的
	//Enable:下一次更新事件时被更新
	//Disable:立即生效
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_Cmd(TIM1,ENABLE);     //使能TIM1的外设	

	TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE 主输出使能,高级定时器必须开启这个
	
}

//void PWM_SetCompare3(uint16_t Compare)
//{
//	TIM_SetCompare3(TIM2,Compare);      //运行过程更改CCR
//}

//void PWM_SetCompare4(uint16_t Compare)
//{
//	TIM_SetCompare4(TIM2,Compare);      //运行过程更改CCR
//}

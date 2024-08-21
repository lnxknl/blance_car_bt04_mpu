#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "sys.h"

void SR04_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Trig
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Echo
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void SR04_TIM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;    
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2, DISABLE);
}

int Distance_SR04(void)
{
	uint16_t count = 0;
	uint16_t Distance = 0;
	
	PAout(0)=0;//预先拉低Trig引脚
	PAout(0)=1;
	Delay_us(13);
	PAout(0)=0;//发出13us的脉冲
	
	TIM2->CNT = 0;
	
	while(PAin(1) == 0)
	{
		if(TIM2->CNT >= 100)
			break;
	}
	
	TIM_Cmd(TIM2, ENABLE);
	
	while(PAin(1) == 1)
	{
		if(TIM2->CNT >= 1500)
			break;
	}
	
	TIM_Cmd(TIM2, DISABLE);
	
	count = TIM2->CNT;//us
	
	//单位cm
	//v = 340m/s = 34000cm/s = 34000cm/10^6us = 0.034cm/us
	//s = vt/2 = t*0.034/2 = t*0.017 ≈ t/58

	
	Distance = (int)count/58;
	
	return Distance;
}

//#include "stm32f10x.h"                  // Device header

//void Timer_Init(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//	
//	TIM_InternalClockConfig(TIM2);       //�ڲ�ʱ��
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
//	TIM_TimeBaseInitStructure.TIM_Period = 2000 - 1;    //ARR(0~65535)      1s
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;  //PSC(0~65535)      ��ʱƵ��=72M/(PSC+1)/(ARR+1)
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //�ظ����������߼����������У�	
//	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
//	
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);     //�����ж�
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
//	NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_Cmd(TIM2,ENABLE);
//}

/*
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
*/

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
	
	TIM_InternalClockConfig(TIM1);       //�ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period =  7200- 1;    //ARR(0~65535)       1KHz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;  //PSC(0~65535)      ��ʱƵ��=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //�ظ����������߼����������У�	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);   //�ṹ�帳��ʼֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;    //CCR     ռ�ձ�=CCR/(ARR+1)���ֱ���=1/(ARR+1)
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);   //��ʼ��ͨ��
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);   //��ʼ��ͨ��
	
	//���Ʋ�����������Ч���Ƕ�ʱ��������һ�θ����¼�ʱ�����µ�
	//Enable:��һ�θ����¼�ʱ������
	//Disable:������Ч
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_Cmd(TIM1,ENABLE);     //ʹ��TIM1������	

	TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE �����ʹ��,�߼���ʱ�����뿪�����
	
}

//void PWM_SetCompare3(uint16_t Compare)
//{
//	TIM_SetCompare3(TIM2,Compare);      //���й��̸���CCR
//}

//void PWM_SetCompare4(uint16_t Compare)
//{
//	TIM_SetCompare4(TIM2,Compare);      //���й��̸���CCR
//}

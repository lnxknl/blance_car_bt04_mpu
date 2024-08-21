#include "stm32f10x.h"                  // Device header
#include "PWM.h"  

/*电机初始化函数*/
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	PWM_Init();
}

/*赋值函数*/
void Load(int16_t moto1,int16_t moto2)
{
	if(moto1 >= 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		TIM_SetCompare1(TIM1,moto1);      //运行过程更改CCR
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		TIM_SetCompare1(TIM1,-moto1);//moto1是负数，PWM_SetCompare3函数需要传入正数，所以加负号。
	}
	
	if(moto2 >= 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		TIM_SetCompare4(TIM1,moto2);      //运行过程更改CCR
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		TIM_SetCompare4(TIM1,-moto2);
	}

}

/*限幅函数*/
extern int PWM_MAX,PWM_MIN;
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}





//void Motor_SetSpeed(int16_t Speed)
//{
//	if(Speed >= 0)
//	{
//		GPIO_SetBits(GPIOB,GPIO_Pin_14);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
//		PWM_SetCompare3(Speed);
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
//		GPIO_SetBits(GPIOB,GPIO_Pin_15);
//		PWM_SetCompare3(-Speed);       //Speed是负数，PWM_SetCompare3函数需要传入正数，所以加负号。
//	}
//}

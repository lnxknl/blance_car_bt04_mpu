#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Usart.h"
#include "OLED.h"

#include "Motor.h"
#include "Encoder.h"

#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "Exti.h"

#include "BT04.h"

#include "AD.h"

#include "SR04.h"

extern float 
	Pitch,Roll,Yaw;						//角度
extern short 
	gyrox,gyroy,gyroz;				//陀螺仪--角速度
extern short 
	aacx,aacy,aacz;						//加速度
extern float 
	Vertical_Kp,//直立环KP、KD
	Vertical_Kd;
extern float 
	Velocity_Kp,//速度环KP、KI
	Velocity_Ki;
extern int
	Encoder_Left,
	Encoder_Right;	//编码器数据（速度）
extern int
	MOTO1,
	MOTO2;				//电机装载变量


uint16_t ADValue;
float Voltage;

extern uint16_t Distance;
extern u8 Distance_Flag;

int main(void)
{
	uart_init(115200);
	
	OLED_Init();
	
	/*蓝牙初始化*/
	BT04_Init();
	
	AD_Init();
	
	SR04_GPIO_Init();
	SR04_TIM2_Init();
	
	Motor_Init();
	Encoder_TIM3_Init();
	Encoder_TIM4_Init();
	
	/*MPU6050初始化	*/
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();	
	
	OLED_ShowString(1,1,"Roll:");
	OLED_ShowString(1,10,"U:  .");
	OLED_ShowString(2,1,"Dis:   cm");
	
	while(1)
	{
		BT04_Control();
						
		ADValue = AD_GetValue();
		Voltage = ((float)ADValue)/4096*3.3*5;
		
		
		
		OLED_ShowSignedNum(1,6,Roll,2);		
		OLED_ShowNum(1,12,Voltage,2);
		OLED_ShowNum(1,15,(int)(Voltage*100)%100,2);
		
		
		
		OLED_ShowString(3,1,"Hello World");
				
		OLED_ShowSignedNum(4,1,Encoder_Left,3);
	  OLED_ShowSignedNum(4,10,Encoder_Right,3);
		OLED_ShowNum(2,5,Distance,3);

	}
}	


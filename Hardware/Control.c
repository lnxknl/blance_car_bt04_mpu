#include "stm32f10x.h"                  // Device header
#include "sys.h"  
#include "OLED.h"
#include "Control.h"

#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "MPU6050.h"

#include "Motor.h"
#include "Encoder.h"

#include "SR04.h"

#define SPEED_Y 18 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 70//ƫ��(����)����趨�ٶ� 

float Med_Angle=2;	//��е��ֵ��---�������޸���Ļ�е��ֵ���ɡ�
float Target_Speed=0;	//�����ٶȣ���������---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�
float Turn_Speed=0;		//�����ٶȣ�ƫ����
extern u8 car;
uint16_t Distance=0;
u8 Distance_Flag=0;

/*********    10ms    *********/
float 
	Vertical_Kp=540,//ֱ����KP��KD
	Vertical_Kd=1.8;
float 
	Velocity_Kp=0.3,//�ٶȻ�KP��KI
	Velocity_Ki=0.0015;

/*********    20ms    *********/
//float 
//	Vertical_Kp=550,//ֱ����KP��KD
//	Vertical_Kd=2.5;
//float 
//	Velocity_Kp=0.8,//�ٶȻ�KP��KI
//	Velocity_Ki=0.004;

float 
	Turn_Kd=0,//ת��KP��KD
	Turn_Kp=40;

float Pitch,Roll,Yaw;						//�Ƕ�
short gyrox,gyroy,gyroz;				//������--���ٶ�
short aacx,aacy,aacz;						//���ٶ�
int Encoder_Left,Encoder_Right;	//���������ݣ��ٶȣ�

u8 stop=0;
int PWM_MAX=5000,PWM_MIN=-5000;	//PWM�޷�����
int MOTO1,MOTO2;								//���װ�ر���

int Vertical_out,Velocity_out,Turn_out;//ֱ����&�ٶȻ�&ת�� ���������

void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//һ���ж�
	{
		if(PBin(5)==0)//�����ж�
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
			
			//1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
			Encoder_Left=Read_Speed(3);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
//			Encoder_Right=-Read_Speed(4);
			/*�ұ������Ӵ��������óɺ����һ����������ǶԵ�*/
			Encoder_Right=Encoder_Left;
						
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//�Ƕ�
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
	
			
			/****************					����ң��     		*************************************/
			/*ǰ��*/
			if((car!=1)&&(car!=2)&&(car==0))Target_Speed=0;//δ���ܵ�ǰ������ָ��-->�ٶ����㣬����ԭ��
			if(car==1)Target_Speed++;//ǰ��1��־λ����-->��Ҫǰ��
			if(car==2)Target_Speed--;//
			Target_Speed = Target_Speed>SPEED_Y?SPEED_Y : (Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�
			
			/*����*/
			if((car!=3)&&(car!=4)&&(car==0))Turn_Speed=0;
			if(car==3)Turn_Speed += -10;	//��ת
			if(car==4)Turn_Speed += 10;	//��ת
			Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);//�޷�( (20*100) * 100   )
		
			/*ת��Լ��*/
			if((car!=3)&&(car!=4)&&(car==0))Turn_Kd=0;//��������ת��ָ�����ת��Լ��
			else if((car==3)||(car==4))Turn_Kd=0;//������ת��ָ����յ�����ȥ��ת��Լ��
			/*********************************************************************************************/

			
			
			//2.������ѹ��ջ������У�����������������	
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
			Vertical_out=Vertical(Med_Angle,Roll,gyrox);			//ֱ����
			Turn_out=Turn(gyroz,Turn_Speed);																//ת��
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//�������
			
			//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out-Turn_out;//����
			MOTO2=PWM_out+Turn_out;//�ҵ��
			Limit(&MOTO1,&MOTO2);	 //PWM�޷�		

			/*ƽ�⳵ˤ������*/
			if(Roll > 40 || Roll < -40)
				Load(0,0),stop=1;
			else
				Load(MOTO1,MOTO2);
			
//			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
			Distance = Distance_SR04();
			
		}
	}
}

/*********************
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*Angle+Vertical_Kd*(gyro_Y-0);//��1��
	return PWM_out;
}



/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	//1.�����ٶ�ƫ��
	Encoder_Err=((encoder_left+encoder_right)-Target);//��ȥ���--�ҵ���⣺�ܹ����ٶ�Ϊ"0"�ĽǶȣ����ǻ�е��ֵ��
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;
	//4.�����޷�
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
	if(stop==1)Encoder_S=0,stop=0;//���������
	
	//5.�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*********************
ת�򻷣�ϵ��*Z����ٶ�+ϵ��*ң������
*********************/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	//�ⲻ��һ���ϸ��PD��������Kd��Ե���ת���Լ������Kp��Ե���ң�ص�ת��
	PWM_out=Turn_Kd*gyro_Z + Turn_Kp*RC;
	return PWM_out;
}


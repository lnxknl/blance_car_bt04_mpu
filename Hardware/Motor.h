#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);

void Load(int16_t moto1,int16_t moto2);/*��ֵ����*/
void Limit(int *motoA,int *motoB);/*�޷�����*/
void Motor_SetSpeed(int16_t Speed);

#endif

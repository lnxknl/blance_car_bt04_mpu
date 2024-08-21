#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);

void Load(int16_t moto1,int16_t moto2);/*赋值函数*/
void Limit(int *motoA,int *motoB);/*限幅函数*/
void Motor_SetSpeed(int16_t Speed);

#endif

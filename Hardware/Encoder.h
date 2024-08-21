#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_TIM3_Init(void);
void Encoder_TIM4_Init(void);
int Read_Speed(int TIMx);

#endif

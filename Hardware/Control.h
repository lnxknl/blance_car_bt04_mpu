#ifndef __CONTROL_H
#define __CONTROL_H	

/*��������*/
int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);

#endif

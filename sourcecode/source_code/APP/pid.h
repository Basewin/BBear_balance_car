#ifndef PID_H
#define PID_H
#include<stdint.h>

typedef struct PID
{
	float target;
	float integral;
	float Kp;	// ����ϵ��
	float Ki;	// ����ϵ��
	float Kd;	// ΢��ϵ��/Derivative
}pid_s, *pid_t;

void PID_Init(pid_t pid, float Kp, float Ki, float Kd);
int32_t PID_Cal(pid_s *p, float current, float differential);

#endif

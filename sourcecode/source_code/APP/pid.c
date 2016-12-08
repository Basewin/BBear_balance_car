#include"pid.h"

/************************************************************************************************ 
Name��PID_Init 
Function:	
		  	initialize the PID value
Parameters��
		   	[in]	-	pid:pid structure
						Kp:P
						Ki:I
						Kd:D
Returns��
			void 
Description:
			null
************************************************************************************************/
void PID_Init(pid_t pid, float Kp, float Ki, float Kd)
{
	pid->target = 0;
	pid->integral = 0;
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
}

/************************************************************************************************ 
Name��PID_Cal 
Function:	
		  	calculate the PID result value
Parameters��
		   	[in]	-	*p:the pointer point to the PID structure
						current:current stage(angle)
						differential:the angle which is calculated every time
Returns��
			[out]	-	int32_t:the PID value output 
Description:
			null
************************************************************************************************/
int32_t PID_Cal(pid_s *p, float current, float differential)
{
	float offset;

	offset = p->target - current;
	p->integral += offset;

	return (int32_t)(p->Kp*offset + p->Ki*p->integral - p->Kd*differential);
}

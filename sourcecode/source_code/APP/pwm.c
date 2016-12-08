#include"pwm.h"

/************************************************************************************************ 
Name��PWM_left_Configuration 
Function:	
		  	configuration the motor's pwm
Parameters��
		   	void
Returns��
			void 
Description:
			null
************************************************************************************************/
void PWM_Motor_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	  
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//3
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ//3
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx//3
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���	  //3
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���	  //3
	
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����//3
}

/************************************************************************************************ 
Name��Cal_angle 
Function:	
		  	TIM2 interrupt to calculate the angle of the car
Parameters��
		   	void
Returns��
			void 
Description:
			null
************************************************************************************************/
void Cal_angle(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_3);

  	TIM_DeInit(TIM2);//��λTIM2��ʱ��

	TIM_TimeBaseStructure.TIM_Period = 10000-1;//20 - 1; //������ֵ0xffff  0.01s      
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;//36000 - 1;//��Ƶ0x36       
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; // ʱ�ӷָ�  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // ѡ������� IC4ӳ�䵽TI4��
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC4, ENABLE);  
	
	TIM_Cmd(TIM2, ENABLE);
}

/************************************************************************************************ 
Name��Heart_TIM 
Function:	
		  	configuration the heart timer(TIM3 interrupt)
Parameters��
		   	void
Returns��
			void 
Description:
			null
************************************************************************************************/
void Heart_TIM()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//open the clock of tim3

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 20 - 1; //0.5s(1000)  0.1s(200)
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	//TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);   

	//TIM_ICInitStruct.TIM_ICMode = TIM_ICMode_ICAP;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;  
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;  
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;  
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;  
	TIM_ICInitStruct.TIM_ICFilter = 0x0;   
	TIM_ICInit(TIM3,&TIM_ICInitStruct);      
	
	TIM_Cmd(TIM3, ENABLE);
}

/************************************************************************************************ 
Name��PWM_Control 
Function:	
		  	pwm control of motor
Parameters��
		   	[in]	-	motorL:pwm value of left motor
						motorR:pwm value of right motor
Returns��
			void 
Description:
			GPIO defination:
			PB6			left motor forward
			PB7			left motor back
			PB8			right motor forward
			PB9			right motor back
************************************************************************************************/
void PWM_Control(int32_t motorL, int32_t motorR)
{
//	if(motorL >= 32760)motorL = 32760;
//	if(motorL < -32760)motorL = -32760;
//	if(motorR >= 32760)motorR = 32760;
//	if(motorR < -32760)motorR = -32760;

	if(motorL >= 1000)motorL = 1000;
	if(motorL < -1000)motorL = -1000;
	if(motorR >= 1000)motorR = 1000;
	if(motorR < -1000)motorR = -1000;

	if(motorL >= 0){TIM_SetCompare1(TIM4,motorL);TIM_SetCompare2(TIM4,0);}
	if(motorL < 0){TIM_SetCompare1(TIM4,0);TIM_SetCompare2(TIM4,-motorL);}

	if(motorR >= 0){TIM_SetCompare3(TIM4,motorR);TIM_SetCompare4(TIM4,0);}
	if(motorR < 0){TIM_SetCompare3(TIM4,0);TIM_SetCompare4(TIM4,-motorR);}

	//TIM_SetCompare1(TIM4,motorL);
}
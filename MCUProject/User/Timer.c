				 
#include "global.h"	

void SetIuA(INT16U uA)//  ��ڵ�����λΪuA
{
	//100%PWMΪ24.53386mA
	uA += 180;//������ز���
	if(uA > 24449)
	{
		uA = 24449;
	}
	TIM_SetCompare3(TIM3,uA); 
}

void Out4_20mA(void)
{
	float ftmp;
	if(Cfg.MeterType==0) //������Ϊ 0ˮ��  1�ȱ�
	{
			ftmp = FlowQ;
	}
	else
	{
			ftmp = HeatQ;
	}
	
	if(ftmp <= Cfg.mA4)
	{
		IuA = 4000;
	}
	else if(ftmp >= Cfg.mA20)
	{
		IuA = 20000;
	}
	else
	{
		//��(y1-y0)/(x1-x0)=(y-y0)/(x-x0); �ó�
		//y = (x-x0)*(y1-y0)/(x1-x0)+y0 �� Iout=(ftmp-Cfg.mA4)*(20000-4000)/(Cfg.mA20-Cfg.mA4)+4000;
		IuA = (INT16U)((ftmp-Cfg.mA4)*16000.0/(Cfg.mA20-Cfg.mA4))+4000;
	}
	SetIuA(IuA);
}

void TIM3Init(void)	  //���195.9Hz��PWM,����0~24.5mA
{	  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	   
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	TIM_DeInit(TIM3);
		
	TIM_TimeBaseStructure.TIM_Period=24500-1;//65536-1	// �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
																	// �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� 
	TIM_TimeBaseStructure.TIM_Prescaler= (5 - 1);				    // ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=24MHZ/(ʱ��Ԥ��Ƶ+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			// ������Ƶ 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		// ���ϼ���ģʽ 

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//Time3CCR3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//TIM_OCInitStructure.TIM_Pulse = (uint16_t)(72000000/MOTORPWMFREQ*motor.DutyRatio/10000); //PWM��ʼռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	SetIuA(IuA);

	TIM_Cmd(TIM3, ENABLE);		
}



void TIM4Init(void)
{	  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	TIM_DeInit(TIM4);
		
	TIM_TimeBaseStructure.TIM_Period=0xffff;		 				// �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
																	// �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� 
	TIM_TimeBaseStructure.TIM_Prescaler= (24000000/TIM4CLK - 1);				    // ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=8MHZ/(ʱ��Ԥ��Ƶ+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			// ������Ƶ 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		// ���ϼ���ģʽ 

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
				  
	TIM_ClearFlag(TIM4, TIM_IT_CC1);							    // ���CC1�жϱ�־
	//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);

	TIM_Cmd(TIM4, ENABLE);		
}



void TIM4_IRQHandler(void)
{
	static char Flag;
    //--CC1�ж����ڶ�ʱ����ADC�ɼ�--
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) 
	{			  
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		
		if(Flag == 0)
		{
			Flag = 1;
			TIM_SetCompare1(TIM4, TIM_GetCapture1(TIM4) + PHCnt);//�����ж�ʱ��
			if(PoutN)
			{
				OCT_OUTH;
			}
		}
		else
		{
			Flag = 0;
			TIM_SetCompare1(TIM4, TIM_GetCapture1(TIM4) + PLCnt);//�����ж�ʱ��
			OCT_OUTL;
			if(PoutN)
			{
				PoutN--;
				if(PoutN == 0)//û�п��������������
				{
					TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);
				}
			}
		}
    }	
}





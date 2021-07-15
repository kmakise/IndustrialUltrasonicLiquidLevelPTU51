				 
#include "global.h"	

void SetIuA(INT16U uA)//  入口电流单位为uA
{
	//100%PWM为24.53386mA
	uA += 180;//光耦边沿补偿
	if(uA > 24449)
	{
		uA = 24449;
	}
	TIM_SetCompare3(TIM3,uA); 
}

void Out4_20mA(void)
{
	float ftmp;
	if(Cfg.MeterType==0) //表类型为 0水表  1热表
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
		//由(y1-y0)/(x1-x0)=(y-y0)/(x-x0); 得出
		//y = (x-x0)*(y1-y0)/(x1-x0)+y0 即 Iout=(ftmp-Cfg.mA4)*(20000-4000)/(Cfg.mA20-Cfg.mA4)+4000;
		IuA = (INT16U)((ftmp-Cfg.mA4)*16000.0/(Cfg.mA20-Cfg.mA4))+4000;
	}
	SetIuA(IuA);
}

void TIM3Init(void)	  //输出195.9Hz的PWM,生成0~24.5mA
{	  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	   
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	TIM_DeInit(TIM3);
		
	TIM_TimeBaseStructure.TIM_Period=24500-1;//65536-1	// 自动重装载寄存器周期的值(计数值) 
																	// 累计 TIM_Period个频率后产生一个更新或者中断 
	TIM_TimeBaseStructure.TIM_Prescaler= (5 - 1);				    // 时钟预分频数   例如：时钟频率=24MHZ/(时钟预分频+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			// 采样分频 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		// 向上计数模式 

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//Time3CCR3
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//TIM_OCInitStructure.TIM_Pulse = (uint16_t)(72000000/MOTORPWMFREQ*motor.DutyRatio/10000); //PWM初始占空比
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
		
	TIM_TimeBaseStructure.TIM_Period=0xffff;		 				// 自动重装载寄存器周期的值(计数值) 
																	// 累计 TIM_Period个频率后产生一个更新或者中断 
	TIM_TimeBaseStructure.TIM_Prescaler= (24000000/TIM4CLK - 1);				    // 时钟预分频数   例如：时钟频率=8MHZ/(时钟预分频+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			// 采样分频 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		// 向上计数模式 

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
				  
	TIM_ClearFlag(TIM4, TIM_IT_CC1);							    // 清除CC1中断标志
	//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);

	TIM_Cmd(TIM4, ENABLE);		
}



void TIM4_IRQHandler(void)
{
	static char Flag;
    //--CC1中断用于定时进行ADC采集--
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) 
	{			  
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		
		if(Flag == 0)
		{
			Flag = 1;
			TIM_SetCompare1(TIM4, TIM_GetCapture1(TIM4) + PHCnt);//调整中断时间
			if(PoutN)
			{
				OCT_OUTH;
			}
		}
		else
		{
			Flag = 0;
			TIM_SetCompare1(TIM4, TIM_GetCapture1(TIM4) + PLCnt);//调整中断时间
			OCT_OUTL;
			if(PoutN)
			{
				PoutN--;
				if(PoutN == 0)//没有可以输出的脉冲了
				{
					TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);
				}
			}
		}
    }	
}





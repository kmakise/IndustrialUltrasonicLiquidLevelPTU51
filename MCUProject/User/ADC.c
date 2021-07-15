				
#include "global.h"	

void ADC1Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);	//ADC时钟为8M/2=4M
	// ADC1 configuration ------------------------------------------------------
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                // 独立模式 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			            // 禁止连续多通道模式 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	                // 禁止连续转换 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 转换不受外界决定 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		        // 右对齐 
	ADC_InitStructure.ADC_NbrOfChannel = 1;					            // 扫描通道数 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ADC1 regular channel5 configuration 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_71Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_71Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_71Cycles5);
	ADC_Cmd(ADC1, ENABLE);                          // Enable ADC1   
	Delayus(20); //GD32的Bug，需要加20us延时
	// Enable ADC1 reset calibaration register    
	ADC_ResetCalibration(ADC1);
	// Check the end of ADC1 reset calibration register 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	// Start ADC1 calibaration 
	ADC_StartCalibration(ADC1);
	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));
	                    
	//ADC_SoftwareStartConvCmd(ADC1,ENABLE);          // 使能转换开始
}
 
	   
INT16S GetVin(void)//获取Vin的电压 放大10倍
{
	//总的转换时间为“采样周期数 + 12.5”个ADC_CLK 周期 时钟为4M故转换时间为6.5us								  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);    // 使能转换开始 
	Delayus(7);	//实际9us  	
    //Vin*10.2/157.2/3.3 = num/4096	
	//Vin = num/4096*3.3*15.39215686 = num*0.0124;放大10倍即num*0.124 即num*15/121;
	return (ADC_GetConversionValue(ADC1)*15/121+3);//补偿0.3V
}
 


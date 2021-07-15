				
#include "global.h"	

void ADC1Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);	//ADCʱ��Ϊ8M/2=4M
	// ADC1 configuration ------------------------------------------------------
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                // ����ģʽ 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			            // ��ֹ������ͨ��ģʽ 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	                // ��ֹ����ת�� 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ת������������ 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		        // �Ҷ��� 
	ADC_InitStructure.ADC_NbrOfChannel = 1;					            // ɨ��ͨ���� 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ADC1 regular channel5 configuration 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_71Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_71Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_71Cycles5);
	ADC_Cmd(ADC1, ENABLE);                          // Enable ADC1   
	Delayus(20); //GD32��Bug����Ҫ��20us��ʱ
	// Enable ADC1 reset calibaration register    
	ADC_ResetCalibration(ADC1);
	// Check the end of ADC1 reset calibration register 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	// Start ADC1 calibaration 
	ADC_StartCalibration(ADC1);
	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));
	                    
	//ADC_SoftwareStartConvCmd(ADC1,ENABLE);          // ʹ��ת����ʼ
}
 
	   
INT16S GetVin(void)//��ȡVin�ĵ�ѹ �Ŵ�10��
{
	//�ܵ�ת��ʱ��Ϊ������������ + 12.5����ADC_CLK ���� ʱ��Ϊ4M��ת��ʱ��Ϊ6.5us								  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);    // ʹ��ת����ʼ 
	Delayus(7);	//ʵ��9us  	
    //Vin*10.2/157.2/3.3 = num/4096	
	//Vin = num/4096*3.3*15.39215686 = num*0.0124;�Ŵ�10����num*0.124 ��num*15/121;
	return (ADC_GetConversionValue(ADC1)*15/121+3);//����0.3V
}
 


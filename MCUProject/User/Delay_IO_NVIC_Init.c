	
#include "global.h"			 

//24M�����µ��øú���ʵ����ʱʱ��Ϊ ����(us+1)us
void Delayus(INT16U us)
{	
	for(;us>0;us--)
	{
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();
	} 
}
   
//8M�����µ��øú���
void Delayms(INT16U ms)
{	
	for(;ms>0;ms--)
	{
		Delayus(1000);
	} 
}
  
void IWDG_Reset(void)//ʹ�ÿ��Ź���λ
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //д��0x5555,���������Ź��Ĵ���д�빦�� 
	IWDG_SetPrescaler(IWDG_Prescaler_4);          //�ڲ�����ʱ��256��Ƶ 40k/4=10kHz
	IWDG_SetReload(100);						    //ι��ʱ�� 10ms/0.1ms=100 .ע�ⲻ�ܴ���0xfff
	IWDG_ReloadCounter();							//ι��
	IWDG_Enable(); 									//ʹ�ܿ��Ź�
	while(1);//�ȴ����Ź���λ
}
	 

void IO_Init(void) //IO��ʼ��
{				 
    GPIO_InitTypeDef  GPIO_InitStructure;  				   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //ʹ�ܶ˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //ʹ�ܶ˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //ʹ�ܶ˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   //ʹ�ܶ˿�ʱ��	
				   
	//---------PA�������������-----------
	//�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	RX1EN;    
	RX2EN;    
	FMQOFF;
	RL1OFF;
    //����������� TXD
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	 
    //�������� RXD ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_12;				
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  
    GPIO_Init(GPIOA, &GPIO_InitStructure);			   
					 
	//---------PB�������������-----------		
	//�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
    //�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//�������� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	//��©��� I2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	  
	//---------PC�������������-----------	
	//ģ������ ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	 	
	
    //�������     	 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;     			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	//�������� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    //�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//---------PD�������������-----------		
	//�������� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);	 										 
}
 
	
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuration the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_Configuration(void) //NVIC����
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //NVIC���� һ������ֻ����һ�ַ��鷽ʽ	
	//��1   1λ��ռ���ȼ���3λ��Ӧ���ȼ�
	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
}



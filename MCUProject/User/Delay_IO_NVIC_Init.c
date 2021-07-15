	
#include "global.h"			 

//24M晶振下调用该函数实际延时时间为 常量(us+1)us
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
   
//8M晶振下调用该函数
void Delayms(INT16U ms)
{	
	for(;ms>0;ms--)
	{
		Delayus(1000);
	} 
}
  
void IWDG_Reset(void)//使用看门狗复位
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //写入0x5555,用于允许看门狗寄存器写入功能 
	IWDG_SetPrescaler(IWDG_Prescaler_4);          //内部低速时钟256分频 40k/4=10kHz
	IWDG_SetReload(100);						    //喂狗时间 10ms/0.1ms=100 .注意不能大于0xfff
	IWDG_ReloadCounter();							//喂狗
	IWDG_Enable(); 									//使能看门狗
	while(1);//等待看门狗复位
}
	 

void IO_Init(void) //IO初始化
{				 
    GPIO_InitTypeDef  GPIO_InitStructure;  				   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //使能端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   //使能端口时钟	
				   
	//---------PA口相关引脚设置-----------
	//推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	
	RX1EN;    
	RX2EN;    
	FMQOFF;
	RL1OFF;
    //复用推挽输出 TXD
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	 
    //浮空输入 RXD 等
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_10|GPIO_Pin_12;				
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  
    GPIO_Init(GPIOA, &GPIO_InitStructure);			   
					 
	//---------PB口相关引脚设置-----------		
	//推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
    //复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//浮空输入 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_14;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	//开漏输出 I2C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	  
	//---------PC口相关引脚设置-----------	
	//模拟输入 ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	 	
	
    //推挽输出     	 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;     			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	//浮空输入 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    //复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12;			
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//---------PD口相关引脚设置-----------		
	//浮空输入 
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
void NVIC_Configuration(void) //NVIC配置
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //NVIC分组 一个工程只能有一种分组方式	
	//组1   1位抢占优先级，3位响应优先级
	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;	  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
		 
}



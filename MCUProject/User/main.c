
#define GLOBAL//ʹ��ȫ�ֱ�����Ϊȫ�ֱ�������ռ�
#include "global.h"
				

uint16_t Get_JTAG_ID(void)
{
	if( *( uint8_t *)( 0xE00FFFE8 ) & 0x08 )
	{
		return  ( ( *( uint8_t *)( 0xE00FFFD0 ) & 0x0F ) << 8 ) |
		( ( *( uint8_t *)( 0xE00FFFE4 ) & 0xFF ) >> 3  )  | 
		( ( *( uint8_t *)( 0xE00FFFE8 ) & 0x07 ) << 5 ) + 1 ;
	}
	return  0;
//���� 0x041 �� ST ��ST�ֲ� Doc ID 13902 Rev 14 �� 1055ҳ    31.6.2 �� 31.6.4 ����
//���� 0x7A3 �� GD ��
}


void PowerDetection(void)//��Դ���
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();  //��ֹ�����жϷ�ֹ�붨ʱ�ɼ�ADC������ͻ
	Vin = GetVin();//��ȡVin�ĵ�ѹ 
	OS_EXIT_CRITICAL() 
    if(Vin < UNDER_VOL)//Ƿ��ʱ���ݴ���
    {
        //WrCmd |= FLAG_TIME;
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        PowerDownEvent();//�����¼���¼
        WritePar();//���ݴ洢��غ���
        Delayms(100);
	    IWDG_Reset();//ʹ�ÿ��Ź���λ
    }       
}

void Task0(void * p_arg)  //-------����洢-------
{
	(void) p_arg;			  

	while(1)
	{			
	    static INT16U Scnt;
        OSTimeDly(1); 
		PowerDetection();//��Դ���
		Scnt++;
		if((Scnt == (OS_TICKS_PER_SEC/2))||(Scnt == OS_TICKS_PER_SEC))//����
		{
			HalfSecTrigger = 1;
		}	
		if(Scnt == OS_TICKS_PER_SEC)
		{
			Scnt = 0;  
			SysTime++;  
			if(PassWord.CountDown)//PassWord.CountDownΪ0ʱ����ʧЧ
			{
				PassWord.CountDown--;
			}
			if(PassWord.ForbidKeytDown)//PassWord.ForbidKeytDown��Ϊ0ʱ��ֹ�����ٴ���֤
			{
				PassWord.ForbidKeytDown--;
			}    
			RunTime.All++;//������ʱ�� s
			if(Fault)//���ڹ���
			{
				RunTime.Fault++;//��������ʱ�� s
			}
		} 	
	}
}


INT8U KeyData;

void Task1(void * p_arg)
{
	(void) p_arg;	 

	K_CONON;
	while(1)
	{		  
        OSTimeDly(500);//
		//KeyData = (~GPIO_ReadInputData(GPIOC)>>6)&0x0f;
	}
}

void Task2(void * p_arg)
{
	(void) p_arg;
	while(1)
	{	
		OSTimeDly(mS(500));
		LED1CPL;
		OSTimeDly(mS(500));
		LED1CPL;
		LED2CPL;
	}
}

	  
void Task3(void * p_arg)//������PCͨѶ
{
	(void) p_arg; 
    ReadPar();//�������ʼ��ʱ��һ��  		
    OSTimeDly(Sec(1));//��ʱ�ȴ�������ʼ�����
	
#ifdef FALSE_WATER_METER //����ͨѶ��ģ��ˮ��û�иö���ʱ������ͨѶ
    W_M_Init();
#endif
	
    while(1)
    {	
		static INT8U Old_ComCfg = 0xff;
        OSTimeDly(1);//
		if(Old_ComCfg != Cfg.ComCfg)
		{
			INT16U bandrate;
			INT8U ch;
			Old_ComCfg = Cfg.ComCfg;
			
            bandrate = (1<<(Cfg.ComCfg/3))*1200;
            ch = Cfg.ComCfg%3;
            if(ch == 0)      
            {
                ch = 'E';
                COM3Init(bandrate,ch,8);                    
            } 
            else if(ch == 1) 
            {
                ch = 'O';
                COM3Init(bandrate,ch,8); 
            }
            else              
            {
                ch = 'N';
                COM3Init(bandrate,ch,8);
            }
		}
		
		
		if(Rx3OverTime < 0xff)
		{
				Rx3OverTime++;
		}

		if((Rx3Num>0)&&(Rx3OverTime > (32>>(Cfg.ComCfg/3))))
		{
				//ModBusProtocol();
			//GetUstcToPC();
		} 

		if(WrCmd)
		{
			WritePar();//д������������ÿ��ִ��һ��,������Ҫд��
		}
		 OSTimeDly(mS(20));
   }
}

void AnalogRTC(void)//ģ��RTC  ÿСʱ��RTC����һ��
{
	if(Time.Sec < 59)
	{
		Time.Sec++;
	}
	else
	{
		Time.Sec = 0;		
		if(Time.Min < 59)
		{
			Time.Min++;
		}
		else
		{		
			ReadTime();
		}
	}
}


void Task4(void * p_arg)//��ȡ̽ͷ���ݲ����м���
{		
	(void) p_arg;	
	
	ReadTime();
	Old_Year = Time.Year;//��ֹ�ս������ͽ������ڱ����¼
	Old_Mon = Time.Mon;//��ֹ�ս������ͽ������ڱ����¼
	Old_Day = Time.Day;//��ֹ�ս������ͽ������ڱ����¼
	CalRatio();
	
	while(1)
	{	
    OSTimeDly(mS(5));
		
		if(HalfSecTrigger)
		{
			static INT8U SecFlag;
			if(SecFlag)
			{
				SecFlag = 0;
			}
			else
			{
				SecFlag = 1;
			}
			HalfSecTrigger = 0;
			

			if(SecFlag)
			{
				
				DateChange();//���ڱ������
				EventRecord();//�¼���¼
				
				AnalogRTC();//ģ��RTC  ÿСʱ��RTC����һ��
				Out4_20mA();
			}
		}
	}
}	



void Task5(void * p_arg)//�̵���״̬����
{
	(void) p_arg; 	
    OSTimeDly(mS(500));	
		
	while(1)//�ó�����һ����ѭ�������ٷ���
	{	
		RelayTask();
			//OSTimeDly(mS(500));	
	}
}


	
void Task6(void * p_arg)
{
	(void) p_arg; 	
  OSTimeDly(mS(500));	
	while(1)
	{	
		Disp();//�ó�����һ����ѭ�������ٷ���
			//OSTimeDly(mS(500));	
	}
}

void Task7(void * p_arg)
{
	(void) p_arg; 	
    OSTimeDly(mS(500));	
		
	COM4Init(); //--��ʾ�ӿ� �ô���Ϊ 115200 N 8 1---
	while(1)
	{	
		//SD_Mem();//�ó�����һ����ѭ�������ٷ���
    OSTimeDly(mS(500));	
	}
}


void SysTick_init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);		
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency/OS_TICKS_PER_SEC);
}

__align(8) static OS_STK Task0Stk[TASK0_STK_SIZE];	 
__align(8) static OS_STK Task1Stk[TASK1_STK_SIZE];
__align(8) static OS_STK Task2Stk[TASK2_STK_SIZE];	 
__align(8) static OS_STK Task3Stk[TASK3_STK_SIZE];
__align(8) static OS_STK Task4Stk[TASK4_STK_SIZE];	 
__align(8) static OS_STK Task5Stk[TASK5_STK_SIZE];
__align(8) static OS_STK Task6Stk[TASK6_STK_SIZE];
__align(8) static OS_STK Task7Stk[TASK7_STK_SIZE];

void AppTaskCreate(void)
{			   
	OSTaskCreate(Task0,(void * )0,&Task0Stk[TASK0_STK_SIZE-1],TASK0_PRIO);
	OSTaskCreate(Task1,(void * )0,&Task1Stk[TASK1_STK_SIZE-1],TASK1_PRIO);
	OSTaskCreate(Task2,(void * )0,&Task2Stk[TASK2_STK_SIZE-1],TASK2_PRIO);  
	OSTaskCreate(Task3,(void * )0,&Task3Stk[TASK3_STK_SIZE-1],TASK3_PRIO);  
	OSTaskCreate(Task4,(void * )0,&Task4Stk[TASK4_STK_SIZE-1],TASK4_PRIO);  
	OSTaskCreate(Task5,(void * )0,&Task5Stk[TASK5_STK_SIZE-1],TASK5_PRIO);  
	OSTaskCreate(Task6,(void * )0,&Task6Stk[TASK6_STK_SIZE-1],TASK6_PRIO);  
	OSTaskCreate(Task7,(void * )0,&Task7Stk[TASK7_STK_SIZE-1],TASK7_PRIO);  
}


int main(void)
{  		  			   
	CPU_Type = Get_JTAG_ID();	
	IO_Init();
	
//SystemInit();	
	ADC1Init();
	Delayms(50); 	
	Vin = GetVin();//��ȡVin�ĵ�ѹ 
	while(Vin < UNDER_VOL)//�ⲿ�����ѹ��
	{		
			Delayms(50);
	Vin = GetVin();//��ȡVin�ĵ�ѹ 
	}

	Delayms(100); 
	
	IuA = 4000;
	TIM3Init();	  //���195.65Hz��PWM,����0~24.53386mA
	
	TIM4Init();//OCT���ʹ��

	COM1Init(); 
	COM2Init(); 	
    	  
	NVIC_Configuration();
	SysTick_init();	
	OSInit();
	AppTaskCreate();
	OSStart();	 		
	return 0;
}




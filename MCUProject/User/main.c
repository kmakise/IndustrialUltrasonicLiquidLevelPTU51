
#define GLOBAL//使能全局变量，为全局变量分配空间
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
//返回 0x041 是 ST （ST手册 Doc ID 13902 Rev 14 的 1055页    31.6.2 和 31.6.4 ），
//返回 0x7A3 是 GD 。
}


void PowerDetection(void)//电源检查
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();  //禁止所有中断防止与定时采集ADC产生冲突
	Vin = GetVin();//获取Vin的电压 
	OS_EXIT_CRITICAL() 
    if(Vin < UNDER_VOL)//欠电时数据处理
    {
        //WrCmd |= FLAG_TIME;
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        PowerDownEvent();//掉电事件记录
        WritePar();//数据存储相关函数
        Delayms(100);
	    IWDG_Reset();//使用看门狗复位
    }       
}

void Task0(void * p_arg)  //-------铁电存储-------
{
	(void) p_arg;			  

	while(1)
	{			
	    static INT16U Scnt;
        OSTimeDly(1); 
		PowerDetection();//电源检查
		Scnt++;
		if((Scnt == (OS_TICKS_PER_SEC/2))||(Scnt == OS_TICKS_PER_SEC))//半秒
		{
			HalfSecTrigger = 1;
		}	
		if(Scnt == OS_TICKS_PER_SEC)
		{
			Scnt = 0;  
			SysTime++;  
			if(PassWord.CountDown)//PassWord.CountDown为0时密码失效
			{
				PassWord.CountDown--;
			}
			if(PassWord.ForbidKeytDown)//PassWord.ForbidKeytDown不为0时禁止密码再次认证
			{
				PassWord.ForbidKeytDown--;
			}    
			RunTime.All++;//总运行时间 s
			if(Fault)//存在故障
			{
				RunTime.Fault++;//故障运行时间 s
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

	  
void Task3(void * p_arg)//用于与PC通讯
{
	(void) p_arg; 
    ReadPar();//主程序初始化时读一次  		
    OSTimeDly(Sec(1));//延时等待变量初始化完毕
	
#ifdef FALSE_WATER_METER //定义通讯中模拟水表。没有该定义时正常的通讯
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
			WritePar();//写参数由主程序每秒执行一次,根据需要写入
		}
		 OSTimeDly(mS(20));
   }
}

void AnalogRTC(void)//模拟RTC  每小时用RTC更新一次
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


void Task4(void * p_arg)//读取探头数据并进行计算
{		
	(void) p_arg;	
	
	ReadTime();
	Old_Year = Time.Year;//防止刚进入程序就进行日期变更记录
	Old_Mon = Time.Mon;//防止刚进入程序就进行日期变更记录
	Old_Day = Time.Day;//防止刚进入程序就进行日期变更记录
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
				
				DateChange();//日期变更处理
				EventRecord();//事件记录
				
				AnalogRTC();//模拟RTC  每小时用RTC更新一次
				Out4_20mA();
			}
		}
	}
}	



void Task5(void * p_arg)//继电器状态更新
{
	(void) p_arg; 	
    OSTimeDly(mS(500));	
		
	while(1)//该程序是一个死循环，不再返回
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
		Disp();//该程序是一个死循环，不再返回
			//OSTimeDly(mS(500));	
	}
}

void Task7(void * p_arg)
{
	(void) p_arg; 	
    OSTimeDly(mS(500));	
		
	COM4Init(); //--显示接口 该串口为 115200 N 8 1---
	while(1)
	{	
		//SD_Mem();//该程序是一个死循环，不再返回
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
	Vin = GetVin();//获取Vin的电压 
	while(Vin < UNDER_VOL)//外部供电电压低
	{		
			Delayms(50);
	Vin = GetVin();//获取Vin的电压 
	}

	Delayms(100); 
	
	IuA = 4000;
	TIM3Init();	  //输出195.65Hz的PWM,生成0~24.53386mA
	
	TIM4Init();//OCT输出使用

	COM1Init(); 
	COM2Init(); 	
    	  
	NVIC_Configuration();
	SysTick_init();	
	OSInit();
	AppTaskCreate();
	OSStart();	 		
	return 0;
}




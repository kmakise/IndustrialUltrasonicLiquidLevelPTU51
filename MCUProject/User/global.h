#ifndef  GLOBAL_H
#define  GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "math.h"
#include "DataType.h"

#include "Delay_IO_NVIC_Init.h"	 
#include "COM.h"	
#include "crc.h"	
#include "ADC.h"	
#include "Timer.h"	
#include "pcf8563.h"	
#include "FM25CL64.h"	
#include "memory.h"	
#include "ModBusProtocol.h"	
#include "FATFS.h"	
#include "FlowMeterHandle.h"	
#include "Disp.h"	
#include "Calc.h"	


#include "Relay.h"
#include "PTU5X.h"
			  	
//--定义全局变量-----
#ifdef GLOBAL
  #define EXT
#else
  #define EXT extern
#endif

  
//#define FALSE_WATER_METER 1 //定义通讯中模拟水表。没有该定义时正常的通讯
  
#define VERSION 01.05        //版本号必须小于99.99

//V1.0
//200130 增加RTC驱动，按键驱动，铁电驱动，4~20mA驱动

//V1.1 200313
//通讯驱动

//V1.2 
//加SD卡驱动 已完成 已加入文件时间
  
//V1.3
//显示驱动，初步已通
  
//V1.4
//脉冲输出驱动，已写好，待测。
//写计算及存储程序  
//加入ModBus协议
//4~20mA的输出实现程序

//V1.5 200622
//读水表的频率由1次/秒 该为2次/秒，这样流速更新速度更快。 
//启用阻尼参数  



//以后考虑显示屏将变量存储空间从0x5000开始，这样预留出来曲线空间备用
  
  
  
#define FALSE 0
#define TRUE 1

#define GD32  0x07A3
#define STM32 0x0041
EXT INT16U CPU_Type;


#define Sec(x) ((INT16U)((float)(x)*OS_TICKS_PER_SEC+0.5))
#define mS(x) ((INT16U)((float)(x)/1000*OS_TICKS_PER_SEC+0.5))
			
#define ABS(x) (((x)>=0)?(x):-(x))

#define UNDER_VOL 150  //欠压值 *0.1V



EXT TIME Time;

EXT CFG Cfg;
EXT RUNTIME RunTime;//运行时间  需要保存 掉电保存
EXT SUMFLOWQ SumFlowQ;//流量累积器 掉电保存 每日零点保存
EXT SUMHEATQ SumHeatQ;//热量累积器  需要保存 掉电保存
EXT HISP HisP;

//存储区容量为8K=8192字节
#define ADDR_HISEVENT 6000  //历史记录 各种事件各16条

#define ADDR_HISYMD 4000  //历史记录 年月日 8年 32月 64天

#define ADDR_BACKUP 2000  //从存储区该位置开始做备份

//#define ADDR_TIME      0  //内部数据起始地址
//#define ADDR_COMPAR    (ADDR_TIME+sizeof(Time))  
#define ADDR_CFG       0
#define ADDR_RUNTIME   (ADDR_CFG+sizeof(CFG))
#define ADDR_SUMFLOWQ  (ADDR_RUNTIME+sizeof(RUNTIME))
#define ADDR_SUMHEATQ  (ADDR_SUMFLOWQ+sizeof(SUMFLOWQ))
#define ADDR_HISP      (ADDR_SUMHEATQ+sizeof(SUMHEATQ))

//#define ADDR_COMPAR    0  

//#define ADDR_CALPAR    (ADDR_COMPAR+sizeof(ComPar))
//#define ADDR_TEMPCORPAR (ADDR_CALPAR+sizeof(CalPar))
//#define ADDR_RUNTIME   (ADDR_TEMPCORPAR+sizeof(TempCorPar))
//#define ADDR_SUMFLOWQ  (ADDR_RUNTIME+sizeof(RunTime))
//#define ADDR_SUMHEATQ  (ADDR_SUMFLOWQ+sizeof(SumFlowQ))
//#define ADDR_SERNUM    (ADDR_SUMHEATQ+sizeof(SumHeatQ))
	
#define WR_TIME      (1<<0)
#define WR_CFG       (1<<1)
#define WR_RUNTIME (1<<4)
#define WR_SUMFLOWQ  (1<<5)
#define WR_SUMHEATQ  (1<<6)
#define WR_HISP    (1<<9)

EXT INT16U WrCmd;//写命令

	

#define LED1ON   GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED1OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define LED1CPL  GPIOC->ODR ^= (1<<2)   

#define LED2ON   GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define LED2OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define LED2CPL  GPIOC->ODR ^= (1<<3)   
					
					
#define FMQON   GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define FMQOFF  GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define K_CONON   GPIO_SetBits(GPIOC,GPIO_Pin_5) //按键开关
#define K_CONOFF  GPIO_ResetBits(GPIOC,GPIO_Pin_5)


//#define MOTOR_DIRSTATUS  (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))

#define POWERFALL (Vin < 150)//外部小于15V电压低 

#define TIM4CLK 50000 //TIM4CLK = 50K

EXT INT16S Vin;//板子输入的电压*0.1V

EXT INT16U SysTime;//系统运行时间 单位S

EXT INT16U IuA;//4~20mA输出电流

//---------------串口相关---------------
#define TX1EN GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define RX1EN GPIO_ResetBits(GPIOA,GPIO_Pin_11)


EXT INT8U Tx1Buf[100];		
EXT INT8U Tx1Num; //发送个数
EXT INT8U Tx1Cnt;//发送个数计数

EXT INT8U Rx1Buf[100];		
EXT INT8U Rx1Num; //接收个数
EXT INT8U Rx1OverTime;//接收超时计数器


#define TX2EN GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define RX2EN GPIO_ResetBits(GPIOA,GPIO_Pin_1)

EXT INT8U Tx2Buf[100];		
EXT INT8U Tx2Num; //发送个数
EXT INT8U Tx2Cnt;//发送个数计数

EXT INT8U Rx2Buf[200];		
EXT INT8U Rx2Num; //接收个数
EXT INT8U Rx2OverTime;//接收超时计数器


#define TX3EN GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define RX3EN GPIO_ResetBits(GPIOB,GPIO_Pin_2)

EXT INT8U Tx3Buf[256];		
EXT INT8U Tx3Num; //发送个数
EXT INT8U Tx3Cnt;//发送个数计数

EXT INT8U Rx3Buf[100];		
EXT INT8U Rx3Num; //接收个数
EXT INT8U Rx3OverTime;//接收超时计数器


EXT INT8U Rx4Buf[100];		
EXT INT8U Rx4Num; //接收个数
EXT INT8U Rx4OverTime;//接收超时计数器



EXT char Str[100];		


#define FAULT_COMM1       (1<<0)//存在小水表通讯故障1
#define FAULT_COMM2       (1<<1)//存在小水表通讯故障2
#define FAULT_TIMEDIFF    (1<<2)//存在换能器故障
#define FAULT_FM          (1<<4)//铁电不能正常读写
#define FAULT_PT          (1<<5)//存在温度故障
EXT INT16U Fault;//总故障（故障概述）

EXT FLOW_METER FlowMeter1,FlowMeter2;//串口读取的水表变量
EXT INT8U HalfSecTrigger; //半秒触发，用于每半秒获取一次数据并计算一次


EXT float AveTimeDiff;//加权的平均时差ns
EXT float AveFlyTime;//平均飞行时间us
EXT INT8U AveSA;//平均信号强度
EXT INT16U AveSQ;//平均信号质量

EXT INT16S HeatTemp;  //热端温度 放大100倍
EXT INT16S ColdTemp; //冷端温度 放大100倍

EXT float FlowRatio;//时差转换为流速的系数  
EXT float Flow;//切除后的流速  //单位m/s
EXT float FlowQRatio; //流速转换为流量的系数
EXT float FlowQ;//单位时间流量 (瞬时流量)m3/h
EXT float HeatQRatio; //流量转换为热量的系数
EXT float HeatQ;//单位时间热量 (瞬时热量)kW

#define OCT_OUTH  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define OCT_OUTL  GPIO_ResetBits(GPIOB,GPIO_Pin_1)
EXT INT16U PoutN;//还需要输出的脉冲个数。
EXT INT16U PHCnt;//输高电平时计数个数
EXT INT16U PLCnt;//输低电平时计数个数


EXT PASSWORD PassWord;//通信密码相关认证

EXT INT8U Old_Year;//用于检测日期变更
EXT INT8U Old_Mon;//用于检测日期变更
EXT INT8U Old_Day;//用于检测日期变更

			   
//模拟小水表的变量	

EXT FLOW_METER W_M;//这个最终要删除

//用于测试程序执行时间 us
EXT INT16U T0;
EXT INT16U T1; 
EXT INT16U T2;
EXT INT16U T3; 

//超声测距相关数据
EXT INT16U UsDistance;	//00 2
EXT INT16U UsLevelmm;		//01 2
EXT float  UsLevelpst;  //02 4
EXT float  UsTemp;      //04 4
EXT float  UsOutput;		//06 4
EXT INT16U Us4mADis;		//0a 2
EXT INT16U Us20mADis;   //0b 2
EXT INT16U UsBlindDis;	//0e 2
EXT INT16U UsCoeffcient;//16 2
EXT INT64U UsUID;				//18 8
EXT INT16U UsMedium;		//2B 2
EXT INT16U UsRLMin;			//48 2
EXT INT16U UsRLMax;			//47 2
EXT INT16U UsRLVal;			//25 2
EXT INT16U UsRLstatus;  //3b 2

#define RL1ON   GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define RL1OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_4)


#endif


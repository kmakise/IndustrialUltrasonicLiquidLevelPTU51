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
			  	
//--����ȫ�ֱ���-----
#ifdef GLOBAL
  #define EXT
#else
  #define EXT extern
#endif

  
//#define FALSE_WATER_METER 1 //����ͨѶ��ģ��ˮ��û�иö���ʱ������ͨѶ
  
#define VERSION 01.05        //�汾�ű���С��99.99

//V1.0
//200130 ����RTC��������������������������4~20mA����

//V1.1 200313
//ͨѶ����

//V1.2 
//��SD������ ����� �Ѽ����ļ�ʱ��
  
//V1.3
//��ʾ������������ͨ
  
//V1.4
//���������������д�ã����⡣
//д���㼰�洢����  
//����ModBusЭ��
//4~20mA�����ʵ�ֳ���

//V1.5 200622
//��ˮ���Ƶ����1��/�� ��Ϊ2��/�룬�������ٸ����ٶȸ��졣 
//�����������  



//�Ժ�����ʾ���������洢�ռ��0x5000��ʼ������Ԥ���������߿ռ䱸��
  
  
  
#define FALSE 0
#define TRUE 1

#define GD32  0x07A3
#define STM32 0x0041
EXT INT16U CPU_Type;


#define Sec(x) ((INT16U)((float)(x)*OS_TICKS_PER_SEC+0.5))
#define mS(x) ((INT16U)((float)(x)/1000*OS_TICKS_PER_SEC+0.5))
			
#define ABS(x) (((x)>=0)?(x):-(x))

#define UNDER_VOL 150  //Ƿѹֵ *0.1V



EXT TIME Time;

EXT CFG Cfg;
EXT RUNTIME RunTime;//����ʱ��  ��Ҫ���� ���籣��
EXT SUMFLOWQ SumFlowQ;//�����ۻ��� ���籣�� ÿ����㱣��
EXT SUMHEATQ SumHeatQ;//�����ۻ���  ��Ҫ���� ���籣��
EXT HISP HisP;

//�洢������Ϊ8K=8192�ֽ�
#define ADDR_HISEVENT 6000  //��ʷ��¼ �����¼���16��

#define ADDR_HISYMD 4000  //��ʷ��¼ ������ 8�� 32�� 64��

#define ADDR_BACKUP 2000  //�Ӵ洢����λ�ÿ�ʼ������

//#define ADDR_TIME      0  //�ڲ�������ʼ��ַ
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

EXT INT16U WrCmd;//д����

	

#define LED1ON   GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED1OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define LED1CPL  GPIOC->ODR ^= (1<<2)   

#define LED2ON   GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define LED2OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define LED2CPL  GPIOC->ODR ^= (1<<3)   
					
					
#define FMQON   GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define FMQOFF  GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define K_CONON   GPIO_SetBits(GPIOC,GPIO_Pin_5) //��������
#define K_CONOFF  GPIO_ResetBits(GPIOC,GPIO_Pin_5)


//#define MOTOR_DIRSTATUS  (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))

#define POWERFALL (Vin < 150)//�ⲿС��15V��ѹ�� 

#define TIM4CLK 50000 //TIM4CLK = 50K

EXT INT16S Vin;//��������ĵ�ѹ*0.1V

EXT INT16U SysTime;//ϵͳ����ʱ�� ��λS

EXT INT16U IuA;//4~20mA�������

//---------------�������---------------
#define TX1EN GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define RX1EN GPIO_ResetBits(GPIOA,GPIO_Pin_11)


EXT INT8U Tx1Buf[100];		
EXT INT8U Tx1Num; //���͸���
EXT INT8U Tx1Cnt;//���͸�������

EXT INT8U Rx1Buf[100];		
EXT INT8U Rx1Num; //���ո���
EXT INT8U Rx1OverTime;//���ճ�ʱ������


#define TX2EN GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define RX2EN GPIO_ResetBits(GPIOA,GPIO_Pin_1)

EXT INT8U Tx2Buf[100];		
EXT INT8U Tx2Num; //���͸���
EXT INT8U Tx2Cnt;//���͸�������

EXT INT8U Rx2Buf[200];		
EXT INT8U Rx2Num; //���ո���
EXT INT8U Rx2OverTime;//���ճ�ʱ������


#define TX3EN GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define RX3EN GPIO_ResetBits(GPIOB,GPIO_Pin_2)

EXT INT8U Tx3Buf[256];		
EXT INT8U Tx3Num; //���͸���
EXT INT8U Tx3Cnt;//���͸�������

EXT INT8U Rx3Buf[100];		
EXT INT8U Rx3Num; //���ո���
EXT INT8U Rx3OverTime;//���ճ�ʱ������


EXT INT8U Rx4Buf[100];		
EXT INT8U Rx4Num; //���ո���
EXT INT8U Rx4OverTime;//���ճ�ʱ������



EXT char Str[100];		


#define FAULT_COMM1       (1<<0)//����Сˮ��ͨѶ����1
#define FAULT_COMM2       (1<<1)//����Сˮ��ͨѶ����2
#define FAULT_TIMEDIFF    (1<<2)//���ڻ���������
#define FAULT_FM          (1<<4)//���粻��������д
#define FAULT_PT          (1<<5)//�����¶ȹ���
EXT INT16U Fault;//�ܹ��ϣ����ϸ�����

EXT FLOW_METER FlowMeter1,FlowMeter2;//���ڶ�ȡ��ˮ�����
EXT INT8U HalfSecTrigger; //���봥��������ÿ�����ȡһ�����ݲ�����һ��


EXT float AveTimeDiff;//��Ȩ��ƽ��ʱ��ns
EXT float AveFlyTime;//ƽ������ʱ��us
EXT INT8U AveSA;//ƽ���ź�ǿ��
EXT INT16U AveSQ;//ƽ���ź�����

EXT INT16S HeatTemp;  //�ȶ��¶� �Ŵ�100��
EXT INT16S ColdTemp; //����¶� �Ŵ�100��

EXT float FlowRatio;//ʱ��ת��Ϊ���ٵ�ϵ��  
EXT float Flow;//�г��������  //��λm/s
EXT float FlowQRatio; //����ת��Ϊ������ϵ��
EXT float FlowQ;//��λʱ������ (˲ʱ����)m3/h
EXT float HeatQRatio; //����ת��Ϊ������ϵ��
EXT float HeatQ;//��λʱ������ (˲ʱ����)kW

#define OCT_OUTH  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define OCT_OUTL  GPIO_ResetBits(GPIOB,GPIO_Pin_1)
EXT INT16U PoutN;//����Ҫ��������������
EXT INT16U PHCnt;//��ߵ�ƽʱ��������
EXT INT16U PLCnt;//��͵�ƽʱ��������


EXT PASSWORD PassWord;//ͨ�����������֤

EXT INT8U Old_Year;//���ڼ�����ڱ��
EXT INT8U Old_Mon;//���ڼ�����ڱ��
EXT INT8U Old_Day;//���ڼ�����ڱ��

			   
//ģ��Сˮ��ı���	

EXT FLOW_METER W_M;//�������Ҫɾ��

//���ڲ��Գ���ִ��ʱ�� us
EXT INT16U T0;
EXT INT16U T1; 
EXT INT16U T2;
EXT INT16U T3; 

//��������������
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


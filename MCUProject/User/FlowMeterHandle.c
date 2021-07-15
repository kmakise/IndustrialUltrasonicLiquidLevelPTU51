
#include "global.h"

void DataAnalysis(INT8U *p,FLOW_METER *w_m)
{
	w_m->SA[0] = ChToINT16(p); p += 2;
	w_m->SQ[0] = ChToINT16(p); p += 2;
	w_m->FlyTime[0] = ChToFloat(p); p += 4;
	w_m->TimeDiff[0] = ChToFloat(p); p += 4;
	
	w_m->SA[1] = ChToINT16(p); p += 2;
	w_m->SQ[1] = ChToINT16(p); p += 2;
	w_m->FlyTime[1] = ChToFloat(p); p += 4;
	w_m->TimeDiff[1] = ChToFloat(p); p += 4;
	
	w_m->SA[2] = ChToINT16(p); p += 2;
	w_m->SQ[2] = ChToINT16(p); p += 2;
	w_m->FlyTime[2] = ChToFloat(p); p += 4;
	w_m->TimeDiff[2] = ChToFloat(p); p += 4;
	
	w_m->SA[3] = ChToINT16(p); p += 2;
	w_m->SQ[3] = ChToINT16(p); p += 2;
	w_m->FlyTime[3] = ChToFloat(p); p += 4;
	w_m->TimeDiff[3] = ChToFloat(p); p += 4;
		
	w_m->Ver = ChToINT16(p); p += 2;
	w_m->Status = ChToINT16(p); p += 2;
	w_m->Temp1 = ChToFloat(p); p += 4;
	w_m->Temp2 = ChToFloat(p); p += 4;
}



INT8U COM1_SendRecProcess(void)//发送、接收处理 返回 0接收错误 1接收成功 
{
	INT8U i;	 
	Rx1Num = 0;
	for(i=50;i>0;i--) //等待数据发送完成
	{		   
		OSTimeDly(mS(1));
		if(Tx1Cnt >= Tx1Num)
		{
			break;
		}	
	}		 
	for(i=200;i>0;i--) //等待开始回传数据
	{		   
		OSTimeDly(mS(1));
		if(Rx1Num)
		{	   
			break;		
		}	
	}
	
	//等待外联板接收超时
	i=0;
	while(i != Rx1Num)
	{
		i = Rx1Num; 
		OSTimeDly(mS(4));
	}
	
    if(Rx1Num > 7)//至少需要的数据个数数据
	{			  
		if(CalCRC16(Rx1Buf,Rx1Num)==0)//接收数据CRC正确
		{
			return 1;	//接收到了正确的数据
		}
	}
	return 0;	
}



void GetFlowMeter1(void)
{
	static INT8U ErrCnt;
	FLOW_METER f_m;
	//01 03 00 00 00 1e c5 c2 
    INT16U SendCrc;
	Tx1Num = 0;
	Tx1Buf[Tx1Num++] = 0x01;
	Tx1Buf[Tx1Num++] = 0x03;
  INT16ToCh(&Tx1Buf[Tx1Num],0x0000);
	Tx1Num += 2;
  INT16ToCh(&Tx1Buf[Tx1Num],0x001e);
	Tx1Num += 2;
  SendCrc= CalCRC16(Tx1Buf,Tx1Num);
  INT16ToCh(&Tx1Buf[Tx1Num],SendCrc);
	Tx1Num += 2;   
	Tx1Send();
	if(COM1_SendRecProcess())//发送、接收处理 返回 0接收错误 1接收成功 
	{
		ErrCnt = 0;
		Fault &= ~FAULT_COMM1;//清除小水表通讯故障1
		//DataAnalysis(&Rx1Buf[3],&FlowMeter1);	
		DataAnalysis(&Rx1Buf[3],&f_m);	
		f_m.TimeDiff[0] += Cfg.TimeDiffOffset[0]/10.0;
		f_m.TimeDiff[1] += Cfg.TimeDiffOffset[1]/10.0;
		f_m.TimeDiff[2] += Cfg.TimeDiffOffset[2]/10.0;
		f_m.TimeDiff[3] += Cfg.TimeDiffOffset[3]/10.0;	
		
		f_m.Temp1 += Cfg.HeatCor/100.0;
		f_m.Temp2 += Cfg.ColdCor/100.0;
		
		HeatTemp = (INT16S)(f_m.Temp1*100.0);
		ColdTemp = (INT16S)(f_m.Temp2*100.0);
		
		FlowMeter1 = f_m;
		if(FlowMeter1.Status & (FAULT_PT1|FAULT_PT2))
		{
			Fault |= FAULT_PT; //存在温度故障
		}
		else
		{
			Fault &= ~FAULT_PT; //清除温度故障
		}
	}
	else
	{
		ErrCnt++;
		if(ErrCnt > 5)
		{
			Fault |= FAULT_COMM1;//存在小水表通讯故障1
			FlowMeter1.Status = FAULT_ALLERR;//无通讯故障
		}
	}
}


INT8U COM2_SendRecProcess(void)//发送、接收处理 返回 0接收错误 1接收成功 
{
	INT8U i;	 
	Rx2Num = 0;
	for(i=50;i>0;i--) //等待数据发送完成
	{		   
		OSTimeDly(mS(1));
		if(Tx2Cnt >= Tx2Num)
		{
			break;
		}	
	}		 
	for(i=100;i>0;i--) //等待开始回传数据
	{		   
		OSTimeDly(mS(1));
		if(Rx2Num)
		{	   
			break;		
		}	
	}
	
	//等待外联板接收超时
	i=0;
	while(i != Rx2Num)
	{
		i = Rx2Num; 
		OSTimeDly(mS(4));
	}
	
    if(Rx2Num > 7)//至少需要的数据个数数据
	{			  
		if(CalCRC16(Rx2Buf,Rx2Num)==0)//接收数据CRC正确
		{
			return 1;	//接收到了正确的数据
		}
	}
	return 0;	
}


void GetFlowMeter2(void)
{
	static INT8U ErrCnt;
	FLOW_METER f_m;
	//01 03 00 00 00 1e c5 c2 
    INT16U SendCrc;
	Tx2Num = 0;
	Tx2Buf[Tx2Num++] = 0x01;
	Tx2Buf[Tx2Num++] = 0x03;
    INT16ToCh(&Tx2Buf[Tx2Num],0x0000);
	Tx2Num += 2;
    INT16ToCh(&Tx2Buf[Tx2Num],0x001e);
	Tx2Num += 2;
    SendCrc= CalCRC16(Tx2Buf,Tx2Num);
    INT16ToCh(&Tx2Buf[Tx2Num],SendCrc);
	Tx2Num += 2;   
	Tx2Send();
	if(COM2_SendRecProcess())//发送、接收处理 返回 0接收错误 1接收成功 
	{
	    ErrCnt = 0;
		Fault &= ~FAULT_COMM2;//清除小水表通讯故障2
		//DataAnalysis(&Rx2Buf[3],&FlowMeter2);	
		DataAnalysis(&Rx2Buf[3],&f_m);	
		f_m.TimeDiff[0] += Cfg.TimeDiffOffset[4]/10.0;
		f_m.TimeDiff[1] += Cfg.TimeDiffOffset[5]/10.0;
		f_m.TimeDiff[2] += Cfg.TimeDiffOffset[6]/10.0;
		f_m.TimeDiff[3] += Cfg.TimeDiffOffset[7]/10.0;		
		FlowMeter2 = f_m;
	}
	else
	{
		ErrCnt++;
		if(ErrCnt > 5)
		{
			Fault |= FAULT_COMM2;//存在小水表通讯故障2
			FlowMeter2.Status = FAULT_ALLERR;//无通讯故障
		}
	}
}


		
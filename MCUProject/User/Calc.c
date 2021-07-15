
#include "global.h"

float F_FlowThreshold;
float F_TimeDiffThreshold;
//----V=CalPar.Factor*C*C/(2*d*tan(a))*TimeDiff/type  //其中C为声速；d为管径；a为夹角；TimeDiff为时差；type为安装类型
//----令FlowRatio = CalPar.Factor*C*C/(2*d*tan(a))/type //其中FlowRatio为时差转换为流速的系数 设备一旦安装，该系数即固定
void CalFlowRatio(void)//计算时差转换流速的系数
{  
    // 1) FlowRatio = CalPar.Factor*0.0001*CalPar.SoundV*0.1 * CalPar.SoundV*0.1 / (2*CalPar.Diameter*0.0001*tan(CalPar.Angle*0.01*PI/180)*CalPar.Type);
    // 1)式为按国际制单位计算的公式 
    //tmp = 0.01f*CalPar.Factor*CalPar.SoundV*CalPar.SoundV*/(2*CalPar.Diameter*tan(CalPar.Angle*0.01*PI/180)*CalPar.Type);//单位m/s2
    if(Cfg.Type)
    {
        FlowRatio = 0.00000000001f*Cfg.FlowFactor*Cfg.SoundV*Cfg.SoundV/(2*Cfg.Diameter*tan(Cfg.Angle*(0.031415926/180))*Cfg.Type);//单位m/s2
    }
    else//安装类型为Pi型
    {
        //FlowRatio = CalPar.Factor*0.0001*CalPar.SoundV*0.1 * CalPar.SoundV*0.1 /(2*CalPar.Angle*0.00001)//(CalPar.Angle为安装的直线距离)*0.01mm 即0.1ns为10^-10s
        FlowRatio = 0.00000000001f*Cfg.FlowFactor*Cfg.SoundV*Cfg.SoundV /(2*Cfg.Angle);
    }
    F_FlowThreshold = Cfg.FlowThreshold*0.0001f;
	F_TimeDiffThreshold = Cfg.TimeDiffThreshold*0.1f;
}

void CalFlowQRatio(void)//计算流速转换为流量的系数
{
    //Area = PI * CalPar.Diameter*0.0001 * CalPar.Diameter*0.0001/4;    单位为平方米
    //令FlowQRatio = 3600*Area，则计算出的结果为单位为 m2*s/h
    FlowQRatio = (0.0001*3600*3.1415926f*0.0001/4) * Cfg.Diameter * Cfg.Diameter;
}

void CalHeatQRatio(void)//计算流量转换为热量的系数
{
    //密度按1000kg/m3  1kWh=3600000J
    //令HeatQ =CalPar.HeatQFactor*0.0001* FlowQ*1000kg/m3*CalPar.SpecificHeat*(HeatTemp-ColdTemp)*0.01*/3600000;则计算出的结果为单位为 m3/h*kg/m3*J/KG/℃*℃*kWh/3600000J=J/h=kw
    HeatQRatio = Cfg.HeatQFactor*0.0001f*Cfg.SpecificHeat/360000.0f;
    //注 HeatQ = HeatQRatio * FlowQ*(HeatTemp-ColdTemp);//瞬时热量 单位kW
}


void CalRatio(void)//计算各转换的系数
{
    CalFlowRatio();//计算时差转换流速的系数
    CalFlowQRatio();//计算流速转换为流量的系数
    CalHeatQRatio();//计算流量转换为热量的系数
}



const float JQ[8] = {1.0,1.0,1.0,1.0,  1.0,1.0,1.0,1.0,};//通道的加权值
void CalcAveTimeDiff(void)//计算加权的平均时差
{
	INT8U i;	
    float zjq = 0;//累计权值
    float sum = 0;
    INT8U sumcnt = 0;//累计次数
    float sum_fly_time = 0;
    INT16U sumSA = 0;
    INT16U sumSQ = 0;
	
    for(i=0;i<Cfg.ChNum;i++)//获取各通道的流速及平均
    {
		INT8U ch;
		ch = i%4;
		if(i < 4)
		{			
			if((FlowMeter1.Status&(1<<ch))== 0)//标记对应通道时差正常
			{
				float ftmp;
				ftmp = FlowMeter1.TimeDiff[ch];
				if(ABS(ftmp)<F_TimeDiffThreshold)//切除时差
				{
					ftmp = 0;
				}
				sumcnt++;
				sum += ftmp * JQ[i];
				zjq += JQ[i];
				sum_fly_time += FlowMeter1.FlyTime[ch];
				sumSA += FlowMeter1.SA[ch];
				sumSQ += FlowMeter1.SQ[ch];
			}
		}
		else
		{	
			if((FlowMeter2.Status&(1<<ch))== 0)//标记对应通道时差正常
			{
				float ftmp;
				ftmp = FlowMeter2.TimeDiff[ch];
				if(ABS(ftmp)<F_TimeDiffThreshold)//切除时差
				{
					ftmp = 0;
				}
				sumcnt++;
				sum += ftmp * JQ[i];
				zjq += JQ[i];
				sum_fly_time += FlowMeter2.FlyTime[ch];
				sumSA += FlowMeter2.SA[ch];
				sumSQ += FlowMeter2.SQ[ch];
			}
		}		
    }
	if(Cfg.ChNum > sumcnt)//存在时差故障
	{
		Fault |= FAULT_TIMEDIFF;
	}
	else
	{
		Fault &= ~FAULT_TIMEDIFF;
	}
    if(sumcnt > 0)
    {
		float ftmpNew;
		float ftmpOld;
		INT16U damp;
		damp = (INT16U)Cfg.Damp * 2;//2倍采样率
		
		//AveTimeDiff = sum/zjq;//加权的平均时差us
		ftmpNew = sum/zjq;//加权的平均时差us
		ftmpOld = AveTimeDiff;
		ftmpOld = (ftmpNew-ftmpOld)/damp + ftmpOld;// Cfg.Damp*2为2倍采样率
		AveTimeDiff = ftmpOld;
		
		//AveFlyTime = sum_fly_time/sumcnt;//平均飞行时间us		
		ftmpNew = sum_fly_time/sumcnt;//平均飞行时间us	
		ftmpOld = AveFlyTime;
		ftmpOld = (ftmpNew-ftmpOld)/damp + ftmpOld;// Cfg.Damp*2为2倍采样率
		AveFlyTime = ftmpOld;
		
		AveSA = sumSA/sumcnt;//平均信号强度
		AveSQ = sumSQ/sumcnt;//平均信号质量
    }
    else
    {           
		AveTimeDiff = 0;//加权的平均时差us
		AveFlyTime = 0;//平均飞行时间us
		AveSA = 0;//平均信号强度
		AveSQ = 0;//平均信号质量
    }  
}


void CalFlow(void)//计算流速
{
    float ftmp;
    ftmp = FlowRatio * AveTimeDiff;
	if(ABS(ftmp)<F_FlowThreshold)//切除流速
	{
		ftmp = 0;
	}
	
    //---此处可以对f_flow(Flow)进行分段调整,以获得更高的精度。
    Flow = ftmp;//单位 m/s  
}


void CalFlowQ(void)//计算瞬时流量
{
    FlowQ = FlowQRatio * Flow;//瞬时流量 单位m3/h
}

void CalHeatQ(void)//计算瞬时热量
{
    HeatQ = HeatQRatio * FlowQ*(HeatTemp-ColdTemp);//瞬时热量 单位kW
}

void CalConvertData(void)//计算流速 瞬时流量 瞬时热量
{ 
    CalFlow();//计算流速
    CalFlowQ();//计算瞬时流量
    CalHeatQ();//计算瞬时热量
}

//--Oct输出//入口为本秒产生的流量mL
void OutOCT(long ltmpQ)
{
    static float DecimalPout;//脉冲输出剩余的小数部分
             
    if(Cfg.OutFlowQ>0)//允许输出 
    {
        INT16U utmp;
        DecimalPout += ltmpQ/1000.0/Cfg.OutFlowQ;//需要输出的脉冲个数（含小数）
        utmp = (INT16U)DecimalPout;//取出整数部分的脉冲数。
        DecimalPout -= utmp;
        PoutN += utmp;//还需要输出的脉冲个数。
        //为使输出波形均匀，令每秒输出脉冲个数的4/5，故脉冲输出频率为PoutN*4/5;
        //计算出定时器高低电平应输出的定时值
        if(PoutN > 0)
        {
            utmp = TIM4CLK/PoutN/4*5;//脉冲输出整周期的计数值
            if(utmp > 32768) utmp = 32768;
            if(utmp > (INT16U)(TIM4CLK/500*Cfg.PulseWidth))//周期大于2倍指定脉冲宽度
            {
                PHCnt = (INT16U)(TIM4CLK/1000*Cfg.PulseWidth);
            }
            else
            {
                PHCnt = utmp/2;
            } 
            PLCnt = utmp - PHCnt;
			if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == RESET)//定时器的脉冲输出已经停止
			{
				//使其马上就产生脉冲输出				
				TIM_SetCompare1(TIM4, TIM_GetCounter(TIM4) + TIM4CLK/1000);//1ms后产生中断
				TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);			
			}
        }
    }
}




//FlowQ/3600
void Sum_FlowQ(void)//累计流量
{   
    long ltmp;
    long long FullData;//满屏显示时的数值
    long long FullData_Neg;//满屏显示时的数值
    //ltmp = (long)(FlowQ/3600*1000000);//单位mL
    //ltmp = (long)(FlowQ*277.777778);//单位mL
    ltmp = (long)(FlowQ*138.8888888);//单位mL 半秒一次
    
    if(ltmp >= 0)//正累积
    {   
        SumFlowQ.Plus += ltmp;
        OutOCT(ltmp);
    }
    else//负累积
    {
        SumFlowQ.Neg += ltmp;
    }
    SumFlowQ.Year += ltmp;
    SumFlowQ.Mon += ltmp;
    SumFlowQ.Day += ltmp;
    
    //溢出清零处理
    //累积量/1000000 大于等于 10^(8-Cfg.Dot)时清零//Cfg.Dot累积量小数点位数
    //即 累积量 大于等于 10^(14-Cfg.Dot)时清零//Cfg.Dot累积量小数点位数
    //为避免long long类型的数据进行乘法运算，此处用switch语句分情况处理
    switch(Cfg.Dot)
    {
    case 0: FullData = 100000000000000; FullData_Neg = -10000000000000; break;
    case 1: FullData = 10000000000000;  FullData_Neg = -1000000000000;  break;
    case 2: FullData = 1000000000000;   FullData_Neg = -100000000000;   break;
    case 3: FullData = 100000000000;    FullData_Neg = -10000000000;    break;
    case 4: FullData = 10000000000;     FullData_Neg = -1000000000;     break;
    default: FullData =1000000000;      FullData_Neg = -100000000;     break;
    }
    
    if(SumFlowQ.Plus >= FullData)
    {
        SumFlowQ.Plus = 0;
    }
    if(SumFlowQ.Neg <= FullData_Neg)
    {
        SumFlowQ.Neg = 0;
    }
    
    if((SumFlowQ.Year >= FullData)||(SumFlowQ.Year <= FullData_Neg))
    {
        SumFlowQ.Year = 0;
    }
    if((SumFlowQ.Mon >= FullData)||(SumFlowQ.Mon <= FullData_Neg))
    {
        SumFlowQ.Mon = 0;
    }
    if((SumFlowQ.Day >= FullData)||(SumFlowQ.Day <= FullData_Neg))
    {
        SumFlowQ.Day = 0;
    }    
}

//累积热量若按最小 2.38mL/s 温差为1度。则每秒产生42焦的热量 取热量的单位为焦即可。
//则longlong可表示-9223372036854775808J到+9223372036854775807J

void Sum_HeatQ(void)//累计热量
{   
    long ltmp;
    long long FullData;//满屏显示时的数值
    long long FullData_Neg;//满屏显示时的数值
    
    //ltmp = (long)HeatQ*1000;//单位J
    ltmp = (long)HeatQ*500;//单位J 半秒
    
    if(ltmp >= 0)//正累积
    {
        SumHeatQ.Plus += ltmp;
    }
    else//负累积
    {
        SumHeatQ.Neg += ltmp;
    }
    
    SumHeatQ.Year += ltmp;
    SumHeatQ.Mon += ltmp;
    SumHeatQ.Day += ltmp;
    
   // if((Cfg.Unit & CFG_SUMHEATQ_UNIT)==0)//单位为kWh
 //   {
        FullData =      360000000000000;//100000000*3600000
        FullData_Neg = -36000000000000;
 //   }
   // else//单位为GJ
 //   {
 //       FullData =      100000000000000000;
 //       FullData_Neg = -10000000000000000;
 //   }
    
    if(SumHeatQ.Plus >= FullData)
    {
        SumHeatQ.Plus = 0;
    }
    if(SumHeatQ.Neg <= FullData_Neg)
    {
        SumHeatQ.Neg = 0;
    }
    
    if((SumHeatQ.Year >= FullData)||(SumHeatQ.Year <= FullData_Neg))
    {
        SumHeatQ.Year = 0;
    }
    if((SumHeatQ.Mon >= FullData)||(SumHeatQ.Mon <= FullData_Neg))
    {
        SumHeatQ.Mon = 0;
    }
    if((SumHeatQ.Day >= FullData)||(SumHeatQ.Day <= FullData_Neg))
    {
        SumHeatQ.Day = 0;
    }    
}


void WriteYearHis(void)//写年历史记录
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//复制年月日
    FloatToCh(&ch[6],SumFlowQ.Year/1000000.0);//将longlong单位mL转换为float单位m3进行存储
    WriteFM(ADDR_HISYMD+0+HisP.Year*10,&ch[0],sizeof(ch));//写入铁电存储区
    HisP.Year = (HisP.Year+1)&0x07;
    WrCmd |= WR_HISP;
}

void WriteMonHis(void)//写月历史记录
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//复制年月日
    FloatToCh(&ch[6],SumFlowQ.Mon/1000000.0);//将longlong单位mL转换为float单位m3进行存储
    WriteFM(ADDR_HISYMD+80+HisP.Mon*10,&ch[0],sizeof(ch));//写入铁电存储区
    HisP.Mon = (HisP.Mon+1)&0x1f;
    WrCmd |= WR_HISP;
}
void WriteDayHis(void)//写日历史记录
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//复制年月日
    FloatToCh(&ch[6],SumFlowQ.Day/1000000.0);//将longlong单位mL转换为float单位m3进行存储
    WriteFM(ADDR_HISYMD+400+HisP.Day*10,&ch[0],sizeof(ch));//写入铁电存储区
    HisP.Day = (HisP.Day+1)&0x3f;
    WrCmd |= WR_HISP;
}

void PowerDownEvent(void)//掉电事件
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//复制年月日
    FloatToCh(&ch[6],SumFlowQ.Plus/1000000.0);//将longlong单位mL转换为float单位m3进行存储
    WriteFM(ADDR_HISYMD+1040+HisP.PowerDown*10,&ch[0],sizeof(ch));//写入铁电存储区
    HisP.PowerDown = (HisP.PowerDown+1)&0x0f;
    WrCmd |= WR_HISP;
}

void DateChange(void)//日期变更处理
{
    if(Old_Year != Time.Year)
    {
        Old_Year = Time.Year;
        Old_Mon = Time.Mon;
        Old_Day = Time.Day;

        WriteYearHis();//写年历史记录
        WriteMonHis();//写月历史记录
        WriteDayHis();//写日历史记录
        
        SumFlowQ.Year = 0;
        SumFlowQ.Mon = 0;
        SumFlowQ.Day = 0;
        
        SumHeatQ.Year = 0;
        SumHeatQ.Mon = 0;
        SumHeatQ.Day = 0;
        
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        WrCmd |= WR_SUMHEATQ;
    }
    else if(Old_Mon != Time.Mon)
    {
        Old_Mon = Time.Mon;
        Old_Day = Time.Day;
        
        WriteMonHis();//写月历史记录
        WriteDayHis();//写日历史记录
        
        SumFlowQ.Mon = 0;
        SumFlowQ.Day = 0;
        
        SumHeatQ.Mon = 0;
        SumHeatQ.Day = 0;
        
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        WrCmd |= WR_SUMHEATQ;
    }    
    else if(Old_Day != Time.Day)
    {
        Old_Day = Time.Day;
        
        WriteDayHis();//写日历史记录
        
        SumFlowQ.Day = 0;
        
        SumHeatQ.Day = 0;
        
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        WrCmd |= WR_SUMHEATQ;
    }   
    else
    {
        //日期未发生变化
    }
}


void EventRecord(void)//事件记录
{
    char ch[7];
    static INT8U UnderVoltageEvent;
    static INT8U SensorFaultEvent;
    static INT8U ReverseFlowEvent;
    static INT8U FlowQmaxEvent;
    static INT8U FlowQminEvent;
    
    static INT8U TempErrEvent;//温度超范围事件
    char record;
    //PowerDownEvent();//掉电事件记录由掉电检测程序完成
    //以下程序中的事件 = 0:事件已恢复，1:发生事件，
    //record == 2时表示未产生事件
    
//欠压事件
    record = 2;
    if((Vin < UNDER_VOL)&&(UnderVoltageEvent == 0))//检测到电压低
    {
        UnderVoltageEvent = 1;//事件发生
        record = 1;
    }
    if((Vin > UNDER_VOL)&&(UnderVoltageEvent != 0))//检测到已恢复
    {
        UnderVoltageEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(0*7*16)+HisP.UnderVoltage*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.UnderVoltage = (HisP.UnderVoltage+1)&0x0f;
        WrCmd |= WR_HISP;
    }

//换能器故障事件
    record = 2;
    if(((Fault & FAULT_TIMEDIFF) == FAULT_TIMEDIFF)&&(SensorFaultEvent == 0))//检测到传感器故障
    {
        SensorFaultEvent = 1;//事件发生
        record = 1;
    }
    if(((Fault & FAULT_TIMEDIFF) == 0)&&(SensorFaultEvent != 0))//检测到已恢复
    {
        SensorFaultEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(1*7*16)+HisP.SensorFault*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.SensorFault = (HisP.SensorFault+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//水流反向事件    
    record = 2;
    if((Flow < 0)&&(ReverseFlowEvent == 0))//检测到传感器故障
    {
        ReverseFlowEvent = 1;//事件发生
        record = 1;
    }
    if((Flow > 0)&&(ReverseFlowEvent != 0))//检测到已恢复
    {
        ReverseFlowEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(2*7*16)+HisP.ReverseFlow*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.ReverseFlow = (HisP.ReverseFlow+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
	
//流量大于Qmax事件    
    record = 2;
    if((FlowQ > Cfg.Qmax)&&(FlowQmaxEvent == 0))//检测到传感器故障
    {
        FlowQmaxEvent = 1;//事件发生
        record = 1;
    }
    if((FlowQ < Cfg.Qmax)&&(FlowQmaxEvent != 0))//检测到已恢复
    {
        FlowQmaxEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(3*7*16)+HisP.FlowQmax*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.FlowQmax = (HisP.FlowQmax+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//流量小于Qmin事件    
    record = 2;
    if((FlowQ < Cfg.Qmin)&&(FlowQminEvent == 0))//检测到传感器故障
    {
        FlowQminEvent = 1;//事件发生
        record = 1;
    }
    if((FlowQ > Cfg.Qmin)&&(FlowQminEvent != 0))//检测到已恢复
    {
        FlowQminEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(4*7*16)+HisP.FlowQmin*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.FlowQmin = (HisP.FlowQmin+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//水温故障事件
    record = 2;
    if((Fault & FAULT_PT)&&(TempErrEvent == 0))//检测到温度故障
    {
        TempErrEvent = 1;//事件发生
        record = 1;
    }
    if(((Fault & FAULT_PT) == 0)&&(TempErrEvent != 0))//检测到已恢复
    {
        TempErrEvent = 0;//事件恢复
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//复制年月日
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(5*7*16)+HisP.TempErr*7,&ch[0],sizeof(ch));//写入铁电存储区
        HisP.TempErr = (HisP.TempErr+1)&0x0f;
        WrCmd |= WR_HISP;
    }
}

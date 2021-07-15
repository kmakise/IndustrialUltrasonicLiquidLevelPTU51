
#include "global.h"

float F_FlowThreshold;
float F_TimeDiffThreshold;
//----V=CalPar.Factor*C*C/(2*d*tan(a))*TimeDiff/type  //����CΪ���٣�dΪ�ܾ���aΪ�нǣ�TimeDiffΪʱ�typeΪ��װ����
//----��FlowRatio = CalPar.Factor*C*C/(2*d*tan(a))/type //����FlowRatioΪʱ��ת��Ϊ���ٵ�ϵ�� �豸һ����װ����ϵ�����̶�
void CalFlowRatio(void)//����ʱ��ת�����ٵ�ϵ��
{  
    // 1) FlowRatio = CalPar.Factor*0.0001*CalPar.SoundV*0.1 * CalPar.SoundV*0.1 / (2*CalPar.Diameter*0.0001*tan(CalPar.Angle*0.01*PI/180)*CalPar.Type);
    // 1)ʽΪ�������Ƶ�λ����Ĺ�ʽ 
    //tmp = 0.01f*CalPar.Factor*CalPar.SoundV*CalPar.SoundV*/(2*CalPar.Diameter*tan(CalPar.Angle*0.01*PI/180)*CalPar.Type);//��λm/s2
    if(Cfg.Type)
    {
        FlowRatio = 0.00000000001f*Cfg.FlowFactor*Cfg.SoundV*Cfg.SoundV/(2*Cfg.Diameter*tan(Cfg.Angle*(0.031415926/180))*Cfg.Type);//��λm/s2
    }
    else//��װ����ΪPi��
    {
        //FlowRatio = CalPar.Factor*0.0001*CalPar.SoundV*0.1 * CalPar.SoundV*0.1 /(2*CalPar.Angle*0.00001)//(CalPar.AngleΪ��װ��ֱ�߾���)*0.01mm ��0.1nsΪ10^-10s
        FlowRatio = 0.00000000001f*Cfg.FlowFactor*Cfg.SoundV*Cfg.SoundV /(2*Cfg.Angle);
    }
    F_FlowThreshold = Cfg.FlowThreshold*0.0001f;
	F_TimeDiffThreshold = Cfg.TimeDiffThreshold*0.1f;
}

void CalFlowQRatio(void)//��������ת��Ϊ������ϵ��
{
    //Area = PI * CalPar.Diameter*0.0001 * CalPar.Diameter*0.0001/4;    ��λΪƽ����
    //��FlowQRatio = 3600*Area���������Ľ��Ϊ��λΪ m2*s/h
    FlowQRatio = (0.0001*3600*3.1415926f*0.0001/4) * Cfg.Diameter * Cfg.Diameter;
}

void CalHeatQRatio(void)//��������ת��Ϊ������ϵ��
{
    //�ܶȰ�1000kg/m3  1kWh=3600000J
    //��HeatQ =CalPar.HeatQFactor*0.0001* FlowQ*1000kg/m3*CalPar.SpecificHeat*(HeatTemp-ColdTemp)*0.01*/3600000;�������Ľ��Ϊ��λΪ m3/h*kg/m3*J/KG/��*��*kWh/3600000J=J/h=kw
    HeatQRatio = Cfg.HeatQFactor*0.0001f*Cfg.SpecificHeat/360000.0f;
    //ע HeatQ = HeatQRatio * FlowQ*(HeatTemp-ColdTemp);//˲ʱ���� ��λkW
}


void CalRatio(void)//�����ת����ϵ��
{
    CalFlowRatio();//����ʱ��ת�����ٵ�ϵ��
    CalFlowQRatio();//��������ת��Ϊ������ϵ��
    CalHeatQRatio();//��������ת��Ϊ������ϵ��
}



const float JQ[8] = {1.0,1.0,1.0,1.0,  1.0,1.0,1.0,1.0,};//ͨ���ļ�Ȩֵ
void CalcAveTimeDiff(void)//�����Ȩ��ƽ��ʱ��
{
	INT8U i;	
    float zjq = 0;//�ۼ�Ȩֵ
    float sum = 0;
    INT8U sumcnt = 0;//�ۼƴ���
    float sum_fly_time = 0;
    INT16U sumSA = 0;
    INT16U sumSQ = 0;
	
    for(i=0;i<Cfg.ChNum;i++)//��ȡ��ͨ�������ټ�ƽ��
    {
		INT8U ch;
		ch = i%4;
		if(i < 4)
		{			
			if((FlowMeter1.Status&(1<<ch))== 0)//��Ƕ�Ӧͨ��ʱ������
			{
				float ftmp;
				ftmp = FlowMeter1.TimeDiff[ch];
				if(ABS(ftmp)<F_TimeDiffThreshold)//�г�ʱ��
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
			if((FlowMeter2.Status&(1<<ch))== 0)//��Ƕ�Ӧͨ��ʱ������
			{
				float ftmp;
				ftmp = FlowMeter2.TimeDiff[ch];
				if(ABS(ftmp)<F_TimeDiffThreshold)//�г�ʱ��
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
	if(Cfg.ChNum > sumcnt)//����ʱ�����
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
		damp = (INT16U)Cfg.Damp * 2;//2��������
		
		//AveTimeDiff = sum/zjq;//��Ȩ��ƽ��ʱ��us
		ftmpNew = sum/zjq;//��Ȩ��ƽ��ʱ��us
		ftmpOld = AveTimeDiff;
		ftmpOld = (ftmpNew-ftmpOld)/damp + ftmpOld;// Cfg.Damp*2Ϊ2��������
		AveTimeDiff = ftmpOld;
		
		//AveFlyTime = sum_fly_time/sumcnt;//ƽ������ʱ��us		
		ftmpNew = sum_fly_time/sumcnt;//ƽ������ʱ��us	
		ftmpOld = AveFlyTime;
		ftmpOld = (ftmpNew-ftmpOld)/damp + ftmpOld;// Cfg.Damp*2Ϊ2��������
		AveFlyTime = ftmpOld;
		
		AveSA = sumSA/sumcnt;//ƽ���ź�ǿ��
		AveSQ = sumSQ/sumcnt;//ƽ���ź�����
    }
    else
    {           
		AveTimeDiff = 0;//��Ȩ��ƽ��ʱ��us
		AveFlyTime = 0;//ƽ������ʱ��us
		AveSA = 0;//ƽ���ź�ǿ��
		AveSQ = 0;//ƽ���ź�����
    }  
}


void CalFlow(void)//��������
{
    float ftmp;
    ftmp = FlowRatio * AveTimeDiff;
	if(ABS(ftmp)<F_FlowThreshold)//�г�����
	{
		ftmp = 0;
	}
	
    //---�˴����Զ�f_flow(Flow)���зֶε���,�Ի�ø��ߵľ��ȡ�
    Flow = ftmp;//��λ m/s  
}


void CalFlowQ(void)//����˲ʱ����
{
    FlowQ = FlowQRatio * Flow;//˲ʱ���� ��λm3/h
}

void CalHeatQ(void)//����˲ʱ����
{
    HeatQ = HeatQRatio * FlowQ*(HeatTemp-ColdTemp);//˲ʱ���� ��λkW
}

void CalConvertData(void)//�������� ˲ʱ���� ˲ʱ����
{ 
    CalFlow();//��������
    CalFlowQ();//����˲ʱ����
    CalHeatQ();//����˲ʱ����
}

//--Oct���//���Ϊ�������������mL
void OutOCT(long ltmpQ)
{
    static float DecimalPout;//�������ʣ���С������
             
    if(Cfg.OutFlowQ>0)//������� 
    {
        INT16U utmp;
        DecimalPout += ltmpQ/1000.0/Cfg.OutFlowQ;//��Ҫ����������������С����
        utmp = (INT16U)DecimalPout;//ȡ���������ֵ���������
        DecimalPout -= utmp;
        PoutN += utmp;//����Ҫ��������������
        //Ϊʹ������ξ��ȣ���ÿ��������������4/5�����������Ƶ��ΪPoutN*4/5;
        //�������ʱ���ߵ͵�ƽӦ����Ķ�ʱֵ
        if(PoutN > 0)
        {
            utmp = TIM4CLK/PoutN/4*5;//������������ڵļ���ֵ
            if(utmp > 32768) utmp = 32768;
            if(utmp > (INT16U)(TIM4CLK/500*Cfg.PulseWidth))//���ڴ���2��ָ��������
            {
                PHCnt = (INT16U)(TIM4CLK/1000*Cfg.PulseWidth);
            }
            else
            {
                PHCnt = utmp/2;
            } 
            PLCnt = utmp - PHCnt;
			if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == RESET)//��ʱ������������Ѿ�ֹͣ
			{
				//ʹ�����ϾͲ����������				
				TIM_SetCompare1(TIM4, TIM_GetCounter(TIM4) + TIM4CLK/1000);//1ms������ж�
				TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);			
			}
        }
    }
}




//FlowQ/3600
void Sum_FlowQ(void)//�ۼ�����
{   
    long ltmp;
    long long FullData;//������ʾʱ����ֵ
    long long FullData_Neg;//������ʾʱ����ֵ
    //ltmp = (long)(FlowQ/3600*1000000);//��λmL
    //ltmp = (long)(FlowQ*277.777778);//��λmL
    ltmp = (long)(FlowQ*138.8888888);//��λmL ����һ��
    
    if(ltmp >= 0)//���ۻ�
    {   
        SumFlowQ.Plus += ltmp;
        OutOCT(ltmp);
    }
    else//���ۻ�
    {
        SumFlowQ.Neg += ltmp;
    }
    SumFlowQ.Year += ltmp;
    SumFlowQ.Mon += ltmp;
    SumFlowQ.Day += ltmp;
    
    //������㴦��
    //�ۻ���/1000000 ���ڵ��� 10^(8-Cfg.Dot)ʱ����//Cfg.Dot�ۻ���С����λ��
    //�� �ۻ��� ���ڵ��� 10^(14-Cfg.Dot)ʱ����//Cfg.Dot�ۻ���С����λ��
    //Ϊ����long long���͵����ݽ��г˷����㣬�˴���switch�����������
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

//�ۻ�����������С 2.38mL/s �²�Ϊ1�ȡ���ÿ�����42�������� ȡ�����ĵ�λΪ�����ɡ�
//��longlong�ɱ�ʾ-9223372036854775808J��+9223372036854775807J

void Sum_HeatQ(void)//�ۼ�����
{   
    long ltmp;
    long long FullData;//������ʾʱ����ֵ
    long long FullData_Neg;//������ʾʱ����ֵ
    
    //ltmp = (long)HeatQ*1000;//��λJ
    ltmp = (long)HeatQ*500;//��λJ ����
    
    if(ltmp >= 0)//���ۻ�
    {
        SumHeatQ.Plus += ltmp;
    }
    else//���ۻ�
    {
        SumHeatQ.Neg += ltmp;
    }
    
    SumHeatQ.Year += ltmp;
    SumHeatQ.Mon += ltmp;
    SumHeatQ.Day += ltmp;
    
   // if((Cfg.Unit & CFG_SUMHEATQ_UNIT)==0)//��λΪkWh
 //   {
        FullData =      360000000000000;//100000000*3600000
        FullData_Neg = -36000000000000;
 //   }
   // else//��λΪGJ
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


void WriteYearHis(void)//д����ʷ��¼
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//����������
    FloatToCh(&ch[6],SumFlowQ.Year/1000000.0);//��longlong��λmLת��Ϊfloat��λm3���д洢
    WriteFM(ADDR_HISYMD+0+HisP.Year*10,&ch[0],sizeof(ch));//д������洢��
    HisP.Year = (HisP.Year+1)&0x07;
    WrCmd |= WR_HISP;
}

void WriteMonHis(void)//д����ʷ��¼
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//����������
    FloatToCh(&ch[6],SumFlowQ.Mon/1000000.0);//��longlong��λmLת��Ϊfloat��λm3���д洢
    WriteFM(ADDR_HISYMD+80+HisP.Mon*10,&ch[0],sizeof(ch));//д������洢��
    HisP.Mon = (HisP.Mon+1)&0x1f;
    WrCmd |= WR_HISP;
}
void WriteDayHis(void)//д����ʷ��¼
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//����������
    FloatToCh(&ch[6],SumFlowQ.Day/1000000.0);//��longlong��λmLת��Ϊfloat��λm3���д洢
    WriteFM(ADDR_HISYMD+400+HisP.Day*10,&ch[0],sizeof(ch));//д������洢��
    HisP.Day = (HisP.Day+1)&0x3f;
    WrCmd |= WR_HISP;
}

void PowerDownEvent(void)//�����¼�
{
    char ch[10];
    memcpy(&ch[0],&Time.Year,6);//����������
    FloatToCh(&ch[6],SumFlowQ.Plus/1000000.0);//��longlong��λmLת��Ϊfloat��λm3���д洢
    WriteFM(ADDR_HISYMD+1040+HisP.PowerDown*10,&ch[0],sizeof(ch));//д������洢��
    HisP.PowerDown = (HisP.PowerDown+1)&0x0f;
    WrCmd |= WR_HISP;
}

void DateChange(void)//���ڱ������
{
    if(Old_Year != Time.Year)
    {
        Old_Year = Time.Year;
        Old_Mon = Time.Mon;
        Old_Day = Time.Day;

        WriteYearHis();//д����ʷ��¼
        WriteMonHis();//д����ʷ��¼
        WriteDayHis();//д����ʷ��¼
        
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
        
        WriteMonHis();//д����ʷ��¼
        WriteDayHis();//д����ʷ��¼
        
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
        
        WriteDayHis();//д����ʷ��¼
        
        SumFlowQ.Day = 0;
        
        SumHeatQ.Day = 0;
        
        WrCmd |= WR_RUNTIME;
        WrCmd |= WR_SUMFLOWQ;
        WrCmd |= WR_SUMHEATQ;
    }   
    else
    {
        //����δ�����仯
    }
}


void EventRecord(void)//�¼���¼
{
    char ch[7];
    static INT8U UnderVoltageEvent;
    static INT8U SensorFaultEvent;
    static INT8U ReverseFlowEvent;
    static INT8U FlowQmaxEvent;
    static INT8U FlowQminEvent;
    
    static INT8U TempErrEvent;//�¶ȳ���Χ�¼�
    char record;
    //PowerDownEvent();//�����¼���¼�ɵ�����������
    //���³����е��¼� = 0:�¼��ѻָ���1:�����¼���
    //record == 2ʱ��ʾδ�����¼�
    
//Ƿѹ�¼�
    record = 2;
    if((Vin < UNDER_VOL)&&(UnderVoltageEvent == 0))//��⵽��ѹ��
    {
        UnderVoltageEvent = 1;//�¼�����
        record = 1;
    }
    if((Vin > UNDER_VOL)&&(UnderVoltageEvent != 0))//��⵽�ѻָ�
    {
        UnderVoltageEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(0*7*16)+HisP.UnderVoltage*7,&ch[0],sizeof(ch));//д������洢��
        HisP.UnderVoltage = (HisP.UnderVoltage+1)&0x0f;
        WrCmd |= WR_HISP;
    }

//�����������¼�
    record = 2;
    if(((Fault & FAULT_TIMEDIFF) == FAULT_TIMEDIFF)&&(SensorFaultEvent == 0))//��⵽����������
    {
        SensorFaultEvent = 1;//�¼�����
        record = 1;
    }
    if(((Fault & FAULT_TIMEDIFF) == 0)&&(SensorFaultEvent != 0))//��⵽�ѻָ�
    {
        SensorFaultEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(1*7*16)+HisP.SensorFault*7,&ch[0],sizeof(ch));//д������洢��
        HisP.SensorFault = (HisP.SensorFault+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//ˮ�������¼�    
    record = 2;
    if((Flow < 0)&&(ReverseFlowEvent == 0))//��⵽����������
    {
        ReverseFlowEvent = 1;//�¼�����
        record = 1;
    }
    if((Flow > 0)&&(ReverseFlowEvent != 0))//��⵽�ѻָ�
    {
        ReverseFlowEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(2*7*16)+HisP.ReverseFlow*7,&ch[0],sizeof(ch));//д������洢��
        HisP.ReverseFlow = (HisP.ReverseFlow+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
	
//��������Qmax�¼�    
    record = 2;
    if((FlowQ > Cfg.Qmax)&&(FlowQmaxEvent == 0))//��⵽����������
    {
        FlowQmaxEvent = 1;//�¼�����
        record = 1;
    }
    if((FlowQ < Cfg.Qmax)&&(FlowQmaxEvent != 0))//��⵽�ѻָ�
    {
        FlowQmaxEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(3*7*16)+HisP.FlowQmax*7,&ch[0],sizeof(ch));//д������洢��
        HisP.FlowQmax = (HisP.FlowQmax+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//����С��Qmin�¼�    
    record = 2;
    if((FlowQ < Cfg.Qmin)&&(FlowQminEvent == 0))//��⵽����������
    {
        FlowQminEvent = 1;//�¼�����
        record = 1;
    }
    if((FlowQ > Cfg.Qmin)&&(FlowQminEvent != 0))//��⵽�ѻָ�
    {
        FlowQminEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(4*7*16)+HisP.FlowQmin*7,&ch[0],sizeof(ch));//д������洢��
        HisP.FlowQmin = (HisP.FlowQmin+1)&0x0f;
        WrCmd |= WR_HISP;
    }
    
//ˮ�¹����¼�
    record = 2;
    if((Fault & FAULT_PT)&&(TempErrEvent == 0))//��⵽�¶ȹ���
    {
        TempErrEvent = 1;//�¼�����
        record = 1;
    }
    if(((Fault & FAULT_PT) == 0)&&(TempErrEvent != 0))//��⵽�ѻָ�
    {
        TempErrEvent = 0;//�¼��ָ�
        record = 0;
    }
    if(record != 2)
    {
        memcpy(&ch[0],&Time.Year,6);//����������
        ch[6] = record;
        WriteFM(ADDR_HISEVENT+(5*7*16)+HisP.TempErr*7,&ch[0],sizeof(ch));//д������洢��
        HisP.TempErr = (HisP.TempErr+1)&0x0f;
        WrCmd |= WR_HISP;
    }
}

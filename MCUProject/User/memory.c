
#include "global.h"

char TestFM(void)
{
    Write_FM25CL64_Byte(0x1fff,0x55);//��ַ��ߴ�д��
    if(Read_FM25CL64_Byte(0x1fff)!=0x55)
    {
         return 1;
    }
    Write_FM25CL64_Byte(0x1fff,0xaa);//��ַ��ߴ�д��
    if(Read_FM25CL64_Byte(0x1fff)!=0xaa)
    {
         return 1;
    }
    return 0;
}


void WriteCfg(void)//д����
{
    Cfg.crc = CalCRC16(&Cfg,sizeof(Cfg)-2);
    WriteFM(ADDR_CFG,&Cfg,sizeof(Cfg));//д������洢��
    WriteFM(ADDR_CFG+ADDR_BACKUP,&Cfg,sizeof(Cfg));//д�����籸����
}

void ReadCfg(void)//��ȡ���к�
{
    INT8U flag=(1<<0)|(1<<1);
    CFG buf;
    
    ReadFM(ADDR_CFG,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&Cfg,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_CFG+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&Cfg,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//ȫ������
    {      
		memset(&Cfg,0,sizeof(Cfg));
		Cfg.Addr = 1;//�豸��ַ     ͨ��ʱ��ʶ���ַ
		Cfg.ComCfg = 11;//ͨѶ����������		
		Cfg.Damp = 1;//����ϵ��1��32
        Cfg.Dot = 4;//�ۻ���С����λ��
        Cfg.MeterType = 0; //������Ϊ 0ˮ��  1�ȱ�
        Cfg.Type = 1;// ����Ϊ 1��V�� 2��M��Ϊ 4  
		Cfg.ChNum = 8;//��Чͨ����  (1~8)
        Cfg.Angle = 6000;//̽ͷ�н� Ϊ60.00��
        Cfg.Diameter = 1000;//�ܵ�ֱ�� Ϊ 100.0mm   
        Cfg.SoundV = 14500; //�����ٶ� Ϊ 1450.0m/s
        Cfg.FlowFactor = 10000;  //����ϵ�� ϵ��*0.0001 �˴�ϵ��Ϊ0.0001
		Cfg.HeatQFactor = 10000;   //����ϵ�� ϵ��*10000 ���� 10200����1.02  ������������ת��ϵ����
        Cfg.FlowThreshold = 0;     //���ٵ���ֵ  ��λΪmm/s
		Cfg.SpecificHeat = 4200;   //������  ��λΪJ/kg/��   
        Cfg.TimeDiffThreshold = 0; //ʱ�����ֵ����ֵ�����ڸ�ֵ��ʱ�0�㡣    ��λΪ0.1ns        
	//TimeDiffOffset[8];//ʱ�������ֵ      ���ڵ�����ˮ�е�ʱ��ƫ�� ��λΪ0.1ns Ĭ��Ϊ0
        Cfg.HeatCor = 0;//�ȶ������¶� �Ŵ�100�� 
        Cfg.ColdCor = 0;//��������¶� �Ŵ�100�� 		
        Cfg.mA4  = 0;//   0��65535m3/h ��  0��65535kW 
        Cfg.mA20 = 1;//  0��65535m3/h ��  0��65535kW
		
        Cfg.PulseWidth  = 50;//Ĭ������ 50ms����
        Cfg.OutFlowQ = 0;//0Ϊ�ر�OCT���
        Cfg.Num = 2020070001;
		Cfg.Qmax = 10.0;//m3/h
		Cfg.Qmin = 0.1;//m3/h
        WriteCfg();//��д
    }  
    else//ȫ����ȷ�򲿷���ȷ
    {
        if(flag != 0)//���ֳ���
        {
            WriteCfg();//��д
        }
    }
}


void WriteRunTime(void)//д���豸����ʱ��
{
    RunTime.crc = CalCRC16(&RunTime,sizeof(RunTime)-2);
    WriteFM(ADDR_RUNTIME,&RunTime,sizeof(RunTime));//д������洢��
    WriteFM(ADDR_RUNTIME+ADDR_BACKUP,&RunTime,sizeof(RunTime));//д�����籸����
}


void ReadRunTime(void)
{
    INT8U flag=(1<<0)|(1<<1);
    RUNTIME buf;
    
    ReadFM(ADDR_RUNTIME,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&RunTime,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_RUNTIME+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&RunTime,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//ȫ������
    {      
        memset(&RunTime,0,sizeof(RunTime));//ȫ������
        WriteRunTime();//��д
    }
    else//ȫ����ȷ�򲿷���ȷ
    {
        if(flag != 0)//���ֳ���
        {
            WriteRunTime();//��д
        }
    }
}

 
void WriteSumFlowQ(void)//д�������ۻ����������ۻ���  ���紦��ÿ�մ���
{
    SumFlowQ.crc = CalCRC16(&SumFlowQ,sizeof(SumFlowQ)-2);
    WriteFM(ADDR_SUMFLOWQ,&SumFlowQ,sizeof(SumFlowQ));//д������洢��
    WriteFM(ADDR_SUMFLOWQ+ADDR_BACKUP,&SumFlowQ,sizeof(SumFlowQ));//д�����籸����
}


void ReadSumFlowQ(void)
{
    INT8U flag=(1<<0)|(1<<1);
    SUMFLOWQ buf;
    
    ReadFM(ADDR_SUMFLOWQ,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&SumFlowQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_SUMFLOWQ+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&SumFlowQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//ȫ������
    {      
        memset(&SumFlowQ,0,sizeof(SumFlowQ));//ȫ������
        WriteSumFlowQ();//��д
    }
    else//ȫ����ȷ�򲿷���ȷ
    {
        if(flag != 0)//���ֳ���
        {
            WriteSumFlowQ();//��д
        }
    }
}


void WriteSumHeatQ(void)//д�������ۻ����������ۻ���  ���紦��ÿ�մ���
{
    SumHeatQ.crc = CalCRC16(&SumHeatQ,sizeof(SumHeatQ)-2);
    WriteFM(ADDR_SUMHEATQ,&SumHeatQ,sizeof(SumHeatQ));//д������洢��
    WriteFM(ADDR_SUMHEATQ+ADDR_BACKUP,&SumHeatQ,sizeof(SumHeatQ));//д�����籸����
}

void ReadSumHeatQ(void)
{
    INT8U flag=(1<<0)|(1<<1);
    SUMHEATQ buf;
    
    ReadFM(ADDR_SUMHEATQ,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&SumHeatQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_SUMHEATQ+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&SumHeatQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//ȫ������
    {      
        memset(&SumHeatQ,0,sizeof(SumHeatQ));//ȫ������
        WriteSumHeatQ();//��д
    }
    else//ȫ����ȷ�򲿷���ȷ
    {
        if(flag != 0)//���ֳ���
        {
            WriteSumHeatQ();//��д
        }
    }
}

void WriteHisP(void)//д��ʷ��¼ָ��
{
    HisP.crc = CalCRC16(&HisP,sizeof(HisP)-2);
    WriteFM(ADDR_HISP,&HisP,sizeof(HisP));//д������洢��
    WriteFM(ADDR_HISP+ADDR_BACKUP,&HisP,sizeof(HisP));//д�����籸����
}

void ReadHisP(void)//����ʷ��¼��ָ��
{
    INT8U flag=(1<<0)|(1<<1);
    HISP buf;
    
    ReadFM(ADDR_HISP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&HisP,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_HISP+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//������ȷ
    {
        if(flag == 0x03)
        {
            memcpy(&HisP,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//ȫ������
    {      
        memset(&HisP,0,sizeof(HisP));
        WriteHisP();//��д
        WriteFMFill(ADDR_HISYMD,0,sizeof(HISYMD));//FM ��� �൱��������ʷ��¼
        WriteFMFill(ADDR_HISEVENT,0,sizeof(HISEVENT));//FM ��� �൱��������ʷ��¼
    }
    else//ȫ����ȷ�򲿷���ȷ
    {
        if(flag != 0)//���ֳ���
        {
            WriteHisP();//��д
        }
    }
}


void ReadPar(void)//�������ʼ��ʱ��һ��
{
    ReadCfg();
    ReadRunTime();
    ReadSumFlowQ();
    ReadSumHeatQ();
    ReadHisP();
}

void WritePar(void)//д������������ÿ��ִ��һ��,������Ҫд��
{
    if(WrCmd & WR_CFG)
    {
        WrCmd &= ~WR_CFG;//�����־
        WriteCfg();
        //CalRatio();
    }
    if(WrCmd & WR_RUNTIME)
    {
        WrCmd &= ~WR_RUNTIME;//�����־
        WriteRunTime();
    }
    if(WrCmd & WR_SUMFLOWQ)
    {
        WrCmd &= ~WR_SUMFLOWQ;//�����־
        WriteSumFlowQ();
    }
    if(WrCmd & WR_SUMHEATQ)
    {
        WrCmd &= ~WR_SUMHEATQ;//�����־
        WriteSumHeatQ();
    }
    if(WrCmd & WR_HISP)
    {
        WrCmd &= ~WR_HISP;//�����־
        WriteHisP();
    }
	
    if(WrCmd & WR_TIME)
    {
        WrCmd &= ~WR_TIME;//�����־
				WriteTime();		
		}
}

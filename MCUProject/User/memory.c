
#include "global.h"

char TestFM(void)
{
    Write_FM25CL64_Byte(0x1fff,0x55);//地址最高处写入
    if(Read_FM25CL64_Byte(0x1fff)!=0x55)
    {
         return 1;
    }
    Write_FM25CL64_Byte(0x1fff,0xaa);//地址最高处写入
    if(Read_FM25CL64_Byte(0x1fff)!=0xaa)
    {
         return 1;
    }
    return 0;
}


void WriteCfg(void)//写配置
{
    Cfg.crc = CalCRC16(&Cfg,sizeof(Cfg)-2);
    WriteFM(ADDR_CFG,&Cfg,sizeof(Cfg));//写入铁电存储区
    WriteFM(ADDR_CFG+ADDR_BACKUP,&Cfg,sizeof(Cfg));//写入铁电备份区
}

void ReadCfg(void)//读取序列号
{
    INT8U flag=(1<<0)|(1<<1);
    CFG buf;
    
    ReadFM(ADDR_CFG,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&Cfg,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_CFG+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&Cfg,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//全部错误
    {      
		memset(&Cfg,0,sizeof(Cfg));
		Cfg.Addr = 1;//设备地址     通信时的识别地址
		Cfg.ComCfg = 11;//通讯波特率配置		
		Cfg.Damp = 1;//阻尼系数1～32
        Cfg.Dot = 4;//累积量小数点位数
        Cfg.MeterType = 0; //表类型为 0水表  1热表
        Cfg.Type = 1;// 对射为 1，V型 2，M型为 4  
		Cfg.ChNum = 8;//有效通道数  (1~8)
        Cfg.Angle = 6000;//探头夹角 为60.00度
        Cfg.Diameter = 1000;//管道直径 为 100.0mm   
        Cfg.SoundV = 14500; //声波速度 为 1450.0m/s
        Cfg.FlowFactor = 10000;  //修正系数 系数*0.0001 此处系数为0.0001
		Cfg.HeatQFactor = 10000;   //修正系数 系数*10000 例如 10200代表1.02  （流量与热量转换系数）
        Cfg.FlowThreshold = 0;     //流速的阈值  单位为mm/s
		Cfg.SpecificHeat = 4200;   //热容量  单位为J/kg/℃   
        Cfg.TimeDiffThreshold = 0; //时差绝对值的阈值，低于该值的时差按0算。    单位为0.1ns        
	//TimeDiffOffset[8];//时差的修正值      用于抵消静水中的时差偏差 单位为0.1ns 默认为0
        Cfg.HeatCor = 0;//热端修正温度 放大100倍 
        Cfg.ColdCor = 0;//冷端修正温度 放大100倍 		
        Cfg.mA4  = 0;//   0～65535m3/h 或  0～65535kW 
        Cfg.mA20 = 1;//  0～65535m3/h 或  0～65535kW
		
        Cfg.PulseWidth  = 50;//默认配置 50ms脉宽
        Cfg.OutFlowQ = 0;//0为关闭OCT输出
        Cfg.Num = 2020070001;
		Cfg.Qmax = 10.0;//m3/h
		Cfg.Qmin = 0.1;//m3/h
        WriteCfg();//重写
    }  
    else//全部正确或部分正确
    {
        if(flag != 0)//部分出错
        {
            WriteCfg();//重写
        }
    }
}


void WriteRunTime(void)//写入设备运行时间
{
    RunTime.crc = CalCRC16(&RunTime,sizeof(RunTime)-2);
    WriteFM(ADDR_RUNTIME,&RunTime,sizeof(RunTime));//写入铁电存储区
    WriteFM(ADDR_RUNTIME+ADDR_BACKUP,&RunTime,sizeof(RunTime));//写入铁电备份区
}


void ReadRunTime(void)
{
    INT8U flag=(1<<0)|(1<<1);
    RUNTIME buf;
    
    ReadFM(ADDR_RUNTIME,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&RunTime,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_RUNTIME+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&RunTime,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//全部错误
    {      
        memset(&RunTime,0,sizeof(RunTime));//全部清零
        WriteRunTime();//重写
    }
    else//全部正确或部分正确
    {
        if(flag != 0)//部分出错
        {
            WriteRunTime();//重写
        }
    }
}

 
void WriteSumFlowQ(void)//写入正负累积及年月日累积器  掉电处理及每日处理
{
    SumFlowQ.crc = CalCRC16(&SumFlowQ,sizeof(SumFlowQ)-2);
    WriteFM(ADDR_SUMFLOWQ,&SumFlowQ,sizeof(SumFlowQ));//写入铁电存储区
    WriteFM(ADDR_SUMFLOWQ+ADDR_BACKUP,&SumFlowQ,sizeof(SumFlowQ));//写入铁电备份区
}


void ReadSumFlowQ(void)
{
    INT8U flag=(1<<0)|(1<<1);
    SUMFLOWQ buf;
    
    ReadFM(ADDR_SUMFLOWQ,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&SumFlowQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_SUMFLOWQ+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&SumFlowQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//全部错误
    {      
        memset(&SumFlowQ,0,sizeof(SumFlowQ));//全部清零
        WriteSumFlowQ();//重写
    }
    else//全部正确或部分正确
    {
        if(flag != 0)//部分出错
        {
            WriteSumFlowQ();//重写
        }
    }
}


void WriteSumHeatQ(void)//写入正负累积及年月日累积器  掉电处理及每日处理
{
    SumHeatQ.crc = CalCRC16(&SumHeatQ,sizeof(SumHeatQ)-2);
    WriteFM(ADDR_SUMHEATQ,&SumHeatQ,sizeof(SumHeatQ));//写入铁电存储区
    WriteFM(ADDR_SUMHEATQ+ADDR_BACKUP,&SumHeatQ,sizeof(SumHeatQ));//写入铁电备份区
}

void ReadSumHeatQ(void)
{
    INT8U flag=(1<<0)|(1<<1);
    SUMHEATQ buf;
    
    ReadFM(ADDR_SUMHEATQ,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&SumHeatQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_SUMHEATQ+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&SumHeatQ,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//全部错误
    {      
        memset(&SumHeatQ,0,sizeof(SumHeatQ));//全部清零
        WriteSumHeatQ();//重写
    }
    else//全部正确或部分正确
    {
        if(flag != 0)//部分出错
        {
            WriteSumHeatQ();//重写
        }
    }
}

void WriteHisP(void)//写历史记录指针
{
    HisP.crc = CalCRC16(&HisP,sizeof(HisP)-2);
    WriteFM(ADDR_HISP,&HisP,sizeof(HisP));//写入铁电存储区
    WriteFM(ADDR_HISP+ADDR_BACKUP,&HisP,sizeof(HisP));//写入铁电备份区
}

void ReadHisP(void)//读历史记录的指针
{
    INT8U flag=(1<<0)|(1<<1);
    HISP buf;
    
    ReadFM(ADDR_HISP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&HisP,&buf,sizeof(buf));
        }
        flag &= ~(1<<0);    
    }
    ReadFM(ADDR_HISP+ADDR_BACKUP,&buf,sizeof(buf));
    if(buf.crc == CalCRC16(&buf,sizeof(buf)-2))//数据正确
    {
        if(flag == 0x03)
        {
            memcpy(&HisP,&buf,sizeof(buf));
        }
        flag &= ~(1<<1);    
    }
    
    if(flag == 0x03)//全部错误
    {      
        memset(&HisP,0,sizeof(HisP));
        WriteHisP();//重写
        WriteFMFill(ADDR_HISYMD,0,sizeof(HISYMD));//FM 填充 相当于清零历史记录
        WriteFMFill(ADDR_HISEVENT,0,sizeof(HISEVENT));//FM 填充 相当于清零历史记录
    }
    else//全部正确或部分正确
    {
        if(flag != 0)//部分出错
        {
            WriteHisP();//重写
        }
    }
}


void ReadPar(void)//主程序初始化时读一次
{
    ReadCfg();
    ReadRunTime();
    ReadSumFlowQ();
    ReadSumHeatQ();
    ReadHisP();
}

void WritePar(void)//写参数由主程序每秒执行一次,根据需要写入
{
    if(WrCmd & WR_CFG)
    {
        WrCmd &= ~WR_CFG;//清除标志
        WriteCfg();
        //CalRatio();
    }
    if(WrCmd & WR_RUNTIME)
    {
        WrCmd &= ~WR_RUNTIME;//清除标志
        WriteRunTime();
    }
    if(WrCmd & WR_SUMFLOWQ)
    {
        WrCmd &= ~WR_SUMFLOWQ;//清除标志
        WriteSumFlowQ();
    }
    if(WrCmd & WR_SUMHEATQ)
    {
        WrCmd &= ~WR_SUMHEATQ;//清除标志
        WriteSumHeatQ();
    }
    if(WrCmd & WR_HISP)
    {
        WrCmd &= ~WR_HISP;//清除标志
        WriteHisP();
    }
	
    if(WrCmd & WR_TIME)
    {
        WrCmd &= ~WR_TIME;//清除标志
				WriteTime();		
		}
}

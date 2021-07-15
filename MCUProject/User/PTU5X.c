
#include "global.h"

//写入全局变量
void UstcWriteGolbal(INT8U * uxarr2p)
{

	
	UsDistance      = ChToINT16((uxarr2p + 0x00 * 2));
	UsLevelmm 			= ChToINT16((uxarr2p + 0x01 * 2));
	UsLevelpst 			= ChToFloat((uxarr2p + 0x02 * 2));		//02 4
	UsTemp 					= ChToFloat((uxarr2p + 0x04 * 2));		//04 4
	UsOutput 				= ChToFloat((uxarr2p + 0x06 * 2));		//06 4
	
	Us4mADis 				= ChToINT16((uxarr2p + 0x0a * 2));		//0a 2
	Us20mADis 			= ChToINT16((uxarr2p + 0x0b * 2));    //0b 2
	UsBlindDis 			= ChToINT16((uxarr2p + 0x0e * 2));		//0e 2
	UsCoeffcient 		= ChToINT16((uxarr2p + 0x16 * 2));		//16 2
	//UsUID 					= ChToINT16((uxarr2p + 0x18 * 2));		//16 2

	UsMedium 				= ChToINT16((uxarr2p + 0x2b * 2));		//2B 2
	
	UsRLMin					= ChToINT16((uxarr2p + 0x48 * 2));			//48 2
	UsRLMax					= ChToINT16((uxarr2p + 0x47 * 2));			//47 2
	UsRLVal					= ChToINT16((uxarr2p + 0x25 * 2));			//25 2
	UsRLstatus			= ChToINT16((uxarr2p + 0x3b * 2));      //3b 2
}

//寄存器更新
void GetUstcDis2(void)
{
	//64 03 00 00 00 1e c5 c2 
    INT16U SendCrc;
	Tx2Num = 0;
	Tx2Buf[Tx2Num++] = 0x01;
	Tx2Buf[Tx2Num++] = 0x03;
    INT16ToCh(&Tx2Buf[Tx2Num],0x0000);
	Tx2Num += 2;
    INT16ToCh(&Tx2Buf[Tx2Num],0x004E);
	Tx2Num += 2;
    SendCrc= CalCRC16(Tx2Buf,Tx2Num);
    INT16ToCh(&Tx2Buf[Tx2Num],SendCrc);
	Tx2Num += 2;   
	Tx2Send();
	if(COM2_SendRecProcess())//发送、接收处理 返回 0接收错误 1接收成功 
	{
		UstcWriteGolbal(&Rx2Buf[3]);
	}
}

//写入数据到探头
void UsDataSend(INT16U addr,INT16U data)
{
	
	int16_t num = 0;
	int8_t state = 0;
	
		INT16U SendCrc;
	Tx2Num = 0;
	Tx2Buf[Tx2Num++] = 0x01;
	Tx2Buf[Tx2Num++] = 0x06;
    INT16ToCh(&Tx2Buf[Tx2Num],addr);
	Tx2Num += 2;
    INT16ToCh(&Tx2Buf[Tx2Num],data);
	Tx2Num += 2;
    SendCrc= CalCRC16(Tx2Buf,Tx2Num);
    INT16ToCh(&Tx2Buf[Tx2Num],SendCrc);
	Tx2Num += 2;  
	
	for(num = 0;num < 5;num++)
	{
		Tx2Send();
		state = COM2_SendRecProcess();
		if(state != 0)
		{
			num = 5;
		}
	}

}


void CRCReplace(uint8_t * buf,uint16_t len)
{
	INT16U SendCrc;
	//re place
	len = len -2;
	SendCrc= CalCRC16(buf,len);
	INT16ToCh(&buf[len],SendCrc);
}

//直接通信映射
void UstcComMirror(uint16_t Rx3Num_history)
{
	int16_t num = 0;
	int8_t state = 0;
	
	for(num = 0;num < Rx3Num_history;num++)
	{
		Tx2Buf[num] = Rx3Buf[num];
	}
	Tx2Buf[0]=0x01;
	CRCReplace(Tx2Buf,Tx2Num);
	
	

	for(num = 0;num < 8;num++)
	{
		Tx2Num = Rx3Num_history;
		Tx2Send();
		state = COM2_SendRecProcess();
		if(state != 0)
		{
			num = 10;
		}
	}
	//01 30 00 00 00 4e c5 fe
	if (Tx2Buf[1] == 0x01 && Tx2Buf[5] > 48)
	{
		UstcWriteGolbal(&Rx2Buf[3]);
	}

	if(state)//发送、接收处理 返回 0接收错误 1接收成功 
	{
		for(num = 0;num < Rx2Num;num++)
		{
			Tx3Buf[num] = Rx2Buf[num];
		}
		Tx3Num = Rx2Num;
		Tx3Buf[0] = Cfg.Addr;
		Tx3Buf[52] = Cfg.Addr;
		CRCReplace(Tx3Buf,Tx3Num);
		Tx3Send();
	}
	Rx3Num_history = 0;
	Rx3Num = 0;
	
}

//超声寄存器修改
void UstcDateUpdate(uint16_t Addr,uint8_t * p,uint16_t Rx3Num_history)
{
	int16_t temp = 0;
	switch(Addr)
	{
		case 0x18: Cfg.Addr = ChToINT16(p);WriteCfg(); break;		//本机地址修改
		
		case 0x48:		
			switch(ChToINT16(p))
			{
				case 1200: Cfg.ComCfg = 2;break;
				case 2400: Cfg.ComCfg = 5;break;
				case 4800: Cfg.ComCfg = 8;break;
				case 19200: Cfg.ComCfg = 14;break;
				default : Cfg.ComCfg = 11; //算做9600无校验
			}		 
			WrCmd |= WR_CFG;
			break;//本机波特率修改
			
		case 0x4A:temp = ChToINT16(p);break;//本机校验修改
			
		//其他寄存器修改
		default:UstcComMirror(Rx3Num_history); return;
	}
	Write_Ret();
}


//PC通信
int8_t GetUstcToPC(void)
{
	static uint8_t Rx3Num_history = 0;
	int16_t num = 0;
	int8_t state = 0;
	
	if(Rx3Num_history == Rx3Num && Rx3Num_history != 0)
	{
		
		if((Rx3Buf[0] == Cfg.Addr)&&(CalCRC16(Rx3Buf,Rx3Num)==0))//是本机的地址 且接收数据CRC正确
    {
        INT16U DataAddr;
        DataAddr = ChToINT16(&Rx3Buf[2]);
        switch(Rx3Buf[1])
        {
            case 3://读寄存器命令
            case 4://读寄存器命令
							UstcComMirror(Rx3Num_history);//直接通信映射
            break;
  
            case 6://写单个寄存器 备注，不支持连写
							{
								UstcDateUpdate(DataAddr,&Rx3Buf[4],Rx3Num_history);
							}
							break;
							
            default:;//不支持的命令
        }
    }
    else
    {
        //00 03/04 00 65 00 01 crcH crcL 
        if((Rx3Buf[0] == 0x00)&&((Rx3Buf[1] == 0x03)||(Rx3Buf[1] == 0x04))&&//判断是否是广播方式读地址
         (ChToINT16(&Rx3Buf[2])==101)&&(ChToINT16(&Rx3Buf[4])==1)&&(CalCRC16(Rx3Buf,Rx3Num)==0))
        {
             //回传本机地址
            INT8U *p= &Tx3Buf[3];
            Tx3Buf[2]=1*2;
            MbRegRead(101,p);
            Read_Ret();//串口读命令返回
        }
    }
		Rx3Num = 0;
		Rx3Num_history = 0;
		return 1;
	}
	else
	{
		Rx3Num_history = Rx3Num;

	}
	return 0;
}	
		
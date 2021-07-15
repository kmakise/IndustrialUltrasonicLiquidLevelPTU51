
#include "global.h"
		
INT8U Arr[100];		
INT8U ArrCnt; //发送个数

const INT8U DispRst[] = {0x82,0x00,0x04,0x55,0xAA,0x5A,0xA5,};

void Disp_SendData(INT8U ch) //发送字节
{
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
	
    USART_SendData(UART4, ch);
}


void Disp_SendArr(INT8U *p,INT8U n)//发送数组(自动添加帧头)
{
	INT8U i;
    Disp_SendData(0x5A);
    Disp_SendData(0xA5);
    Disp_SendData(n);	
	for(i=0;i<n;i++)
	{
		Disp_SendData(p[i]);
	}
}


void Disp_Rst(void) //复位显示屏
{	
	Disp_SendArr((INT8U*)DispRst,sizeof(DispRst));//发送数组(自动添加帧头)
}

void Disp_SetPic(INT8U pic) //设置要显示的图片
{
	//PIC_Set 1号图片 5A A5 07 82 00845A010001
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x84; 	
	Arr[ArrCnt++] = 0x5a; 	
	Arr[ArrCnt++] = 0x01; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = pic; 	
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)
}

INT16S Disp_ReadPic(void) //读取当前显示的图片
{
//读当前页面：5A A5 04 83 0014 01
//应答：5A A5 06 83 00 14 01 00 07(0007 为07 号页面)	
	INT16S pic = -1;
	Rx4Num = 0; //接收个数
	
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x83; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x14; 
	Arr[ArrCnt++] = 0x01; 
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)
	//延时一定时间
  OSTimeDly(mS(2));	
	if((Rx4Buf[0] == 0x5A)&&(Rx4Buf[1] == 0xA5)&&(Rx4Buf[2] == 0x06))
	{
		pic = Rx4Buf[8];
	}
	return pic;
}

void Disp_WrRtc(void)//写RTC
{
	//写：5A A5 0B 82 0010 14011d 00 0B0C0D 00
	//文本RTC 显示2020-01-29 11:12:13 SUN，
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x10; 	
	Arr[ArrCnt++] = Time.Year;//年
	Arr[ArrCnt++] = Time.Mon;//月 	
	Arr[ArrCnt++] = Time.Day;//日 		
	Arr[ArrCnt++] = 0;//星期
	Arr[ArrCnt++] = Time.Hour;//时	
	Arr[ArrCnt++] = Time.Min;//分 		
	Arr[ArrCnt++] = Time.Sec;//秒
	Arr[ArrCnt++] = 0;//预留0 
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)	
}

void Disp_WrInt(INT16U addr,INT16S data)//写16位整数
{
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = addr>>8; 	
	Arr[ArrCnt++] = addr&0xff; 	
	Arr[ArrCnt++] = data>>8; 	
	Arr[ArrCnt++] = data&0xff; 	
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)	
}


void Disp_WrFloat(INT16U addr,float fdata)//写浮点数
{
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = addr>>8; 	
	Arr[ArrCnt++] = addr&0xff; 	
	FloatToCh(&Arr[ArrCnt],fdata); ArrCnt+=4;
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)	
}

void Disp_WrStr(INT16U addr,char *p)//写数组
{
	//①显示“北12AB”
	//5A A509825020B1B1 3132 4142
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = addr>>8; 	
	Arr[ArrCnt++] = addr&0xff; 	
	while(*p)
	{
		Arr[ArrCnt++] = *p; 	
		p++;
	}	
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)	
}

//液面高度百分比显示
void HighPstDis(float pst)
{
	//5A A5 13 82 5440 0004 0001 00350060 0080 0075 f79e FF00
	INT8U high;
	
	pst = (pst > 100) ? 100: pst;
	pst = (pst < 0) ? 0: pst;
	high  = 0xF7 - 13 * pst / 10.0;
	if(pst == 100)high = 5;
	
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
 	Arr[ArrCnt++] = 0x54; 	
	Arr[ArrCnt++] = 0x40;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = 0x04;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = 0x01;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = 0x35;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = 0x60;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = 0x80;
	Arr[ArrCnt++] = 0x00;
	Arr[ArrCnt++] = high;
	Arr[ArrCnt++] = 0xf7;
	Arr[ArrCnt++] = 0x9e;
	Arr[ArrCnt++] = 0xFF;
	Arr[ArrCnt++] = 0x00;
	Disp_SendArr(Arr,ArrCnt);//发送数组(自动添加帧头)	
}

/*-----------------------------------------------------------------------------------*/



//传感器配置修改
void UsConfigSet(INT16U addr,INT16U data)
{
		UsDataSend(addr,data);
}



//返回数据处理
void RxDataAnalyze(INT8U * data)
{
	//	UsConfigSet(0x002B,0x0008);
	// 	5A A5 06 83 11 00 01 00 78 
	if(data[0] == 0x83)
	{
		INT16U addr 			= (data[1] << 8)|data[2];
		INT16U dataInt16 	= (data[4] << 8)|data[5];
		
		switch(addr)
		{
			//Set win 1
			case 0x1100:break; //Dist.Empty
			case 0x1102:break; //Dist.Full
			case 0x1104:UsConfigSet(0x000e,dataInt16);UsBlindDis 		= dataInt16;	break; //Bind Dist
			case 0x1106:UsConfigSet(0x0016,dataInt16);UsCoeffcient	= dataInt16;	break; //Filter Coefficient
			case 0x1108:UsConfigSet(0x002b,((dataInt16 % 3) * 2 + 4));UsMedium = ((dataInt16 % 3) * 2 + 4);break; //Medium
			case 0x1110:Time.Year = dataInt16; WrCmd |= WR_TIME;break; //Year
			case 0x1112:Time.Mon  = dataInt16; WrCmd |= WR_TIME;break; //Mon
			case 0x1114:Time.Day  = dataInt16; WrCmd |= WR_TIME;break; //Day
			case 0x1116:Time.Hour = dataInt16; WrCmd |= WR_TIME;break; //Hour
			case 0x1118:Time.Min  = dataInt16; WrCmd |= WR_TIME;break; //Min
			case 0x1120:Time.Sec  = dataInt16; WrCmd |= WR_TIME;break; //Sec
			
			//Set win 2
			case 0x1200:UsConfigSet(0x000a,dataInt16);Us4mADis  = dataInt16;break; //4ma
			case 0x1202:UsConfigSet(0x000b,dataInt16);Us20mADis = dataInt16;break; //20ma
			case 0x1204:UsConfigSet(0x0048,dataInt16);UsRLMin 	= dataInt16;break; //Minv
			case 0x1206:UsConfigSet(0x0047,dataInt16);UsRLMax 	= dataInt16;break; //Maxv
			case 0x1208:UsConfigSet(0x0025,dataInt16);UsRLVal 	= dataInt16;break; //Delay
			case 0x1210:Cfg.Addr = dataInt16;WriteCfg();break; //UID
		}
	}
}
//数据返回检查
int8_t DispBackCheck(void)
{
	//Rx4Num
	static INT16U numhist = 0;
	
	if(Rx4Num != 0)
	{
		if(numhist != Rx4Num)
		{
			numhist = Rx4Num;
		}
		else if(numhist == Rx4Num)
		{
			if(numhist > 7)
			{
				if((Rx4Buf[0] == 0x5A)&&(Rx4Buf[1] == 0xA5)&&(Rx4Buf[2] == 0x06))
				{
					RxDataAnalyze(&Rx4Buf[3]);
					numhist = 0;
					Rx4Num = 0;
					return 1;
				}
			}
			numhist = 0;
			Rx4Num = 0;
		}
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/


//含义：0x5A A5:帧头；
//0x 09:数据长度；
//0x 82:写指令；
//0x 5020:变量地址；
//0x B1B1:“北”字的ASCII码；
//0x 3132：“12”的ASCII码；
//0x 4142“AB”的ASCII码。ASCII码可由ASCII转换工具转换。

INT16S Pic;
void Disp(void)
{
	INT16U loopnum = 0;
	int8_t state = 0;
	//更新数据
	GetUstcDis2();
	
	while(1)
	{	
		OSTimeDly(mS(5));
		//接收数据处理
		state = DispBackCheck();
		//PC通信转发
		GetUstcToPC();
		
		//500ms更新显示数据
		loopnum++;
		if((loopnum > 100))//||(state != 0))
		{		
			loopnum = 0;
			
			
			//MainWin0
			Disp_WrInt(0x2000,UsLevelmm);	
			Disp_WrInt(0x1002,UsDistance);	
			Disp_WrFloat(0x1004,UsOutput);
			Disp_WrFloat(0x1008,UsLevelpst);
			Disp_WrFloat(0x1012,UsTemp);
			HighPstDis(UsLevelpst);
			
			//IntWin1
			Disp_WrInt(0x1100,Us4mADis);
			Disp_WrInt(0x1102,Us20mADis);
			Disp_WrInt(0x1104,UsBlindDis);
			Disp_WrInt(0x1108,(UsMedium / 2) - 2);
			Disp_WrInt(0x1106,UsCoeffcient);
			
			//IntWin2
			Disp_WrInt(0x1200,Us4mADis);
			Disp_WrInt(0x1202,Us20mADis);
			Disp_WrInt(0x1204,UsRLMin);
			Disp_WrInt(0x1206,UsRLMax);
			Disp_WrInt(0x1208,UsRLVal);
			Disp_WrInt(0x1210,Cfg.Addr);
			
			//写RTC
			Disp_WrRtc();
			
			//更新数据
			GetUstcDis2();
		}
	}
}









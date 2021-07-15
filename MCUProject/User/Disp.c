
#include "global.h"
		
INT8U Arr[100];		
INT8U ArrCnt; //���͸���

const INT8U DispRst[] = {0x82,0x00,0x04,0x55,0xAA,0x5A,0xA5,};

void Disp_SendData(INT8U ch) //�����ֽ�
{
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
	
    USART_SendData(UART4, ch);
}


void Disp_SendArr(INT8U *p,INT8U n)//��������(�Զ����֡ͷ)
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


void Disp_Rst(void) //��λ��ʾ��
{	
	Disp_SendArr((INT8U*)DispRst,sizeof(DispRst));//��������(�Զ����֡ͷ)
}

void Disp_SetPic(INT8U pic) //����Ҫ��ʾ��ͼƬ
{
	//PIC_Set 1��ͼƬ 5A A5 07 82 00845A010001
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x84; 	
	Arr[ArrCnt++] = 0x5a; 	
	Arr[ArrCnt++] = 0x01; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = pic; 	
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)
}

INT16S Disp_ReadPic(void) //��ȡ��ǰ��ʾ��ͼƬ
{
//����ǰҳ�棺5A A5 04 83 0014 01
//Ӧ��5A A5 06 83 00 14 01 00 07(0007 Ϊ07 ��ҳ��)	
	INT16S pic = -1;
	Rx4Num = 0; //���ո���
	
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x83; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x14; 
	Arr[ArrCnt++] = 0x01; 
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)
	//��ʱһ��ʱ��
  OSTimeDly(mS(2));	
	if((Rx4Buf[0] == 0x5A)&&(Rx4Buf[1] == 0xA5)&&(Rx4Buf[2] == 0x06))
	{
		pic = Rx4Buf[8];
	}
	return pic;
}

void Disp_WrRtc(void)//дRTC
{
	//д��5A A5 0B 82 0010 14011d 00 0B0C0D 00
	//�ı�RTC ��ʾ2020-01-29 11:12:13 SUN��
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = 0x00; 	
	Arr[ArrCnt++] = 0x10; 	
	Arr[ArrCnt++] = Time.Year;//��
	Arr[ArrCnt++] = Time.Mon;//�� 	
	Arr[ArrCnt++] = Time.Day;//�� 		
	Arr[ArrCnt++] = 0;//����
	Arr[ArrCnt++] = Time.Hour;//ʱ	
	Arr[ArrCnt++] = Time.Min;//�� 		
	Arr[ArrCnt++] = Time.Sec;//��
	Arr[ArrCnt++] = 0;//Ԥ��0 
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)	
}

void Disp_WrInt(INT16U addr,INT16S data)//д16λ����
{
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = addr>>8; 	
	Arr[ArrCnt++] = addr&0xff; 	
	Arr[ArrCnt++] = data>>8; 	
	Arr[ArrCnt++] = data&0xff; 	
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)	
}


void Disp_WrFloat(INT16U addr,float fdata)//д������
{
	ArrCnt = 0;
	Arr[ArrCnt++] = 0x82; 	
	Arr[ArrCnt++] = addr>>8; 	
	Arr[ArrCnt++] = addr&0xff; 	
	FloatToCh(&Arr[ArrCnt],fdata); ArrCnt+=4;
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)	
}

void Disp_WrStr(INT16U addr,char *p)//д����
{
	//����ʾ����12AB��
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
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)	
}

//Һ��߶Ȱٷֱ���ʾ
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
	Disp_SendArr(Arr,ArrCnt);//��������(�Զ����֡ͷ)	
}

/*-----------------------------------------------------------------------------------*/



//�����������޸�
void UsConfigSet(INT16U addr,INT16U data)
{
		UsDataSend(addr,data);
}



//�������ݴ���
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
//���ݷ��ؼ��
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


//���壺0x5A A5:֡ͷ��
//0x 09:���ݳ��ȣ�
//0x 82:дָ�
//0x 5020:������ַ��
//0x B1B1:�������ֵ�ASCII�룻
//0x 3132����12����ASCII�룻
//0x 4142��AB����ASCII�롣ASCII�����ASCIIת������ת����

INT16S Pic;
void Disp(void)
{
	INT16U loopnum = 0;
	int8_t state = 0;
	//��������
	GetUstcDis2();
	
	while(1)
	{	
		OSTimeDly(mS(5));
		//�������ݴ���
		state = DispBackCheck();
		//PCͨ��ת��
		GetUstcToPC();
		
		//500ms������ʾ����
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
			
			//дRTC
			Disp_WrRtc();
			
			//��������
			GetUstcDis2();
		}
	}
}









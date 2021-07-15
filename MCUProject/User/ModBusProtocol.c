
#include "global.h"

void Read_Ret(void)//���ڶ������
{
    INT16U SendCrc;
    Tx3Buf[0]=Rx3Buf[0];
    Tx3Buf[1]=Rx3Buf[1];
    SendCrc= CalCRC16(Tx3Buf,Tx3Buf[2]+3);
    INT16ToCh(&Tx3Buf[Tx3Buf[2]+3],SendCrc);
    Tx3Num = Tx3Buf[2]+5;

	Tx3Send(); //ʹ����1��ʼ��������
}

void Write_Ret(void)//����д����أ����ؽ��յ������
{
    INT16U SendCrc;
    memcpy(Tx3Buf,Rx3Buf,6);//���ƽ��յ�������
    SendCrc = CalCRC16(Tx3Buf,6);
    INT16ToCh(&Tx3Buf[6],SendCrc); 
    Tx3Num = 8;//���ع̶���8���ֽ�
    Tx3Send(); //ʹ����1��ʼ��������
}



void FloatToCh(void *p,float f)//--��������ת��ΪModBus��ʽ���ַ�
{
    char *pch = (char *)p;
    char *pf = (char *)&f;
    pch[0] = pf[3];
    pch[1] = pf[2];
    pch[2] = pf[1];
    pch[3] = pf[0];
}          

float ChToFloat(void *p)//--��ModBus��ʽ���ַ�ת��Ϊ������
{
    float f;
    char *pch = (char *)p;
    char *pf = (char *)&f;
    pf[3] = pch[2];
    pf[2] = pch[3];
    pf[1] = pch[0];
    pf[0] = pch[1];
    return f;
}           

void INT16ToCh(void *p,INT16U Int)//--������ת��ΪModBus��ʽ���ַ�
{
    char *pch = p;
    pch[0] = Int>>8;
    pch[1] = Int&0xff;
}           

INT16S ChToINT16(void *p)//--������ת��ΪModBus��ʽ���ַ�
{
    char *pch = p;
    return ((INT16S)pch[0]<<8)+pch[1];
}           

INT32S ChToLong(void *p)//--��ModBus��ʽ���ַ�ת��Ϊ������
{
    INT32S l;
    char *pch = (char *)p;
    char *pl = (char *)&l;
    pl[0] = pch[3];
    pl[1] = pch[2];
    pl[2] = pch[1];
    pl[3] = pch[0];
    return l;
}           
void LongToCh(void *p,INT32S l)//--��long������ת��ΪModBus��ʽ���ַ�
{
    char *pch = (char *)p;
    char *pl = (char *)&l;
    pch[0] = pl[3];
    pch[1] = pl[2];
    pch[2] = pl[1];
    pch[3] = pl[0];
}      

#ifdef FALSE_WATER_METER //����ͨѶ��ģ��ˮ��û�иö���ʱ������ͨѶ

void W_M_Init(void)
{
	
	W_M.Ver = 11;//ˮ��汾��
    W_M.Status = 0;//����״̬
	W_M.SA[0] = 6;    //��ͨ�����ź�ǿ��
	W_M.SA[1] = 5;    //��ͨ�����ź�ǿ��
	W_M.SA[2] = 4;    //��ͨ�����ź�ǿ��
	W_M.SA[3] = 3;    //��ͨ�����ź�ǿ��
    W_M.SQ[0] = 2;//�ź�����
    W_M.SQ[1] = 3;//�ź�����
    W_M.SQ[2] = 4;//�ź�����
    W_M.SQ[3] = 5;//�ź�����
    W_M.FlyTime[0] = 101.1;//��������ʱ��us
    W_M.FlyTime[1] = 102.2;//��������ʱ��us
    W_M.FlyTime[2] = 103.3;//��������ʱ��us
    W_M.FlyTime[3] = 104.4;//��������ʱ��us	
    W_M.TimeDiff[0] = 0.00;//ʱ��ns
    W_M.TimeDiff[1] = 1.11;//ʱ��ns
    W_M.TimeDiff[2] = -2.22;//ʱ��ns
    W_M.TimeDiff[3] = 3.33;//ʱ��ns
	
    W_M.Temp1 = 12.34;//�¶�1
    W_M.Temp2 = -12.34;//�¶�2
}
  
//--ModBus���Ĵ�������������Ӧ�Ĵ���������ת�����ַ�����ָ����ָ��ĵ�ַ
void MbRegRead(INT16U addr,void *pp)
{
    char *p=pp;
    switch(addr)
    {
    case  0: INT16ToCh(p,W_M.SA[0]); break;
    case  1: INT16ToCh(p,W_M.SQ[0]); break;
    case  2: FloatToCh(p,W_M.FlyTime[0]); break;
    case  3: break;
    case  4: FloatToCh(p,W_M.TimeDiff[0]); break;
    case  5: break;
		
    case  6: INT16ToCh(p,W_M.SA[1]); break;
    case  7: INT16ToCh(p,W_M.SQ[1]); break;
    case  8: FloatToCh(p,W_M.FlyTime[1]); break;
    case  9: break;
    case  10: FloatToCh(p,W_M.TimeDiff[1]); break;
    case  11: break;
		
    case  12: INT16ToCh(p,W_M.SA[2]); break;
    case  13: INT16ToCh(p,W_M.SQ[2]); break;
    case  14: FloatToCh(p,W_M.FlyTime[2]); break;
    case  15: break;
    case  16: FloatToCh(p,W_M.TimeDiff[2]); break;
    case  17: break;
		
    case  18: INT16ToCh(p,W_M.SA[3]); break;
    case  19: INT16ToCh(p,W_M.SQ[3]); break;
    case  20: FloatToCh(p,W_M.FlyTime[3]); break;
    case  21: break;
    case  22: 
		FloatToCh(p,W_M.TimeDiff[3]); 
	
		W_M.TimeDiff[3] += 0.01;
		if(W_M.TimeDiff[3] > 4.00)
		{
			W_M.TimeDiff[3] = -4.00;
		}
	  break;
    case  23: break;
	
    case 24: INT16ToCh(p,W_M.Ver); break; //����汾��
    case 25: INT16ToCh(p,W_M.Status); break; //����״̬
    case 26: FloatToCh(p,W_M.Temp1); break;
    case 27: break;
    case 28: FloatToCh(p,W_M.Temp2); break;
    case 29: break;
		
    default:INT16ToCh(p,0);
    }	   
}    

#else
  

//--ModBus���Ĵ�������������Ӧ�Ĵ���������ת�����ַ�����ָ����ָ��ĵ�ַ
void MbRegRead(INT16U addr,void *pp)
{
    char *p=pp;
    switch(addr)
    {
    case 0: INT16ToCh(p,0); break;
    case 1: INT16ToCh(p,AveSA); break;;//ƽ���ź�ǿ��
    case 2: INT16ToCh(p,HeatTemp); break;
    case 3: INT16ToCh(p,ColdTemp); break;
    case 4: INT16ToCh(p,(INT16U)(AveFlyTime*100.0)); break;//ƽ������ʱ��*0.01us
    case 5: INT16ToCh(p,(int)(AveTimeDiff*10.0+0.5)); break;//��Ȩ��ƽ��ʱ��*0.1ns
    case 6: FloatToCh(p,Flow); break;
    case 7: break;
    case 8: FloatToCh(p,FlowQ); break;
    case 9: break;
    case 10: FloatToCh(p,HeatQ); break;
    case 11: break;
    case 12: INT16ToCh(p,0); break;
    case 13: INT16ToCh(p,0); break;
    case 14: INT16ToCh(p,IuA); break;
    case 15: INT16ToCh(p,(INT16U)(VERSION*100)); break;
    case 16: INT16ToCh(p,Vin*10.0); break;   //*0.01V
    case 17: INT16ToCh(p,Fault); break;
    
    case 22: FloatToCh(p,(float)RunTime.Fault/3600.0); break;
    case 23: break;
    case 24: FloatToCh(p,(float)RunTime.All/3600.0); break;
    case 25: break;
    case 26: FloatToCh(p,(float)SumFlowQ.Plus/1000000.0); break;
    case 27: break;
    case 28: FloatToCh(p,(float)SumFlowQ.Neg/1000000.0); break;
    case 29: break;
    case 30: FloatToCh(p,(float)SumFlowQ.Day/1000000.0); break;
    case 31: break;
    case 32: FloatToCh(p,(float)SumFlowQ.Mon/1000000.0); break;
    case 33: break;
    case 34: FloatToCh(p,(float)SumFlowQ.Year/1000000.0); break;
    case 35: break;
    case 36: INT16ToCh(p,0); break;
    case 37: INT16ToCh(p,0); break;
    case 38: FloatToCh(p,(float)SumHeatQ.Plus/3600000.0); break;
    case 39: break;
    case 40: FloatToCh(p,(float)SumHeatQ.Neg/3600000.0); break;
    case 41: break;
    case 42: FloatToCh(p,(float)SumHeatQ.Day/3600000.0); break;
    case 43: break;
    case 44: FloatToCh(p,(float)SumHeatQ.Mon/3600000.0); break;
    case 45: break;
    case 46: FloatToCh(p,(float)SumHeatQ.Year/3600000.0); break;
    case 47: break;
    case 48: INT16ToCh(p,Cfg.ChNum); break;
    case 49: INT16ToCh(p,Cfg.MeterType); break;
    case 50: INT16ToCh(p,Cfg.Type); break;
    case 51: INT16ToCh(p,Cfg.Angle); break;
    case 52: INT16ToCh(p,Cfg.Diameter); break;
    case 53: INT16ToCh(p,Cfg.SoundV); break;
    case 54: INT16ToCh(p,Cfg.FlowFactor); break;
    
    case 56: INT16ToCh(p,Cfg.TimeDiffThreshold); break;
    case 57: INT16ToCh(p,Cfg.FlowThreshold); break;
    case 58: INT16ToCh(p,Cfg.HeatQFactor); break;
    case 59: INT16ToCh(p,Cfg.SpecificHeat); break;
    case 60: INT16ToCh(p,0); break;
//    case 61: INT16ToCh(p,Cfg.SampleRate); break;
    case 62: INT16ToCh(p,Cfg.Damp); break;
    case 63: INT16ToCh(p,Cfg.Dot); break;
//    case 64: INT16ToCh(p,Cfg.Unit); break;
    case 65: FloatToCh(p,Cfg.Qmax); break;
    case 66: break;
    case 67: FloatToCh(p,Cfg.Qmin); break;
    case 68: break;
    
//    case 70: INT16ToCh(p,CalPar.TimeDiffOffset[0]); break;
//    case 71: INT16ToCh(p,CalPar.TimeDiffOffset[1]); break;
//    case 72: INT16ToCh(p,CalPar.TimeDiffOffset[2]); break;
 //   case 73: INT16ToCh(p,CalPar.TimeDiffOffset[3]); break;
 //   case 65: INT16ToCh(p,0); break;
//    case 66: INT16ToCh(p,0); break;
 //   case 67: memcpy(p,&MenuPar.Loop[0],2); break;
 //   case 68: memcpy(p,&MenuPar.Loop[2],2); break;
 //   case 69: memcpy(p,&MenuPar.Loop[4],2); break;
 //   case 70: memcpy(p,&MenuPar.Loop[6],2); break;
 //   case 71: p[0] = MenuPar.SwapTime;p[1] = MenuPar.NoOpsTime; break;
  //  case 72: INT16ToCh(p,0); break;
  //  case 73: INT16ToCh(p,0); break;
 //   case 78: INT16ToCh(p,0); break;
 //   case 79: INT16ToCh(p,0); break;
 //   case 80: INT16ToCh(p,0); break;
 //   case 81: INT16ToCh(p,0); break;
    //case 82: INT16ToCh(p,0); break;
    //case 83: INT16ToCh(p,0); break;
    //case 84: INT16ToCh(p,0); break;
    //case 85: INT16ToCh(p,0); break;
    //case 86: INT16ToCh(p,0); break;
    //case 87: INT16ToCh(p,0); break;
    case 88: INT16ToCh(p,0); break;
    case 89: INT16ToCh(p,0); break;
    case 90: p[0] = Time.Year;p[1] = Time.Mon; break;
    case 91: p[0] = Time.Day; p[1] = Time.Hour; break;
    case 92: p[0] = Time.Min; p[1] = Time.Sec; break;
    case 93: INT16ToCh(p,0); break;
    case 94: INT16ToCh(p,0); break;
    case 95: INT16ToCh(p,0); break;
    case 96: INT16ToCh(p,(INT16U)(Cfg.Num>>16)); INT16ToCh(&p[2],(INT16U)(Cfg.Num&0xffff)); break;
    case 97: break;
    case 98: INT16ToCh(p,0); break;		  	
    case 99: p[0] = Cfg.HeatCor/10;p[1] = Cfg.ColdCor/10; break;	
    case 100: INT16ToCh(p,0); break;				  
    case 101: INT16ToCh(p,Cfg.Addr); break;				  
    case 102: INT16ToCh(p, (1<<(Cfg.ComCfg/3))*1200); break;				  
    case 103: PassWord.Keyt = (PassWord.Keyt+13)*TIM_GetCounter(TIM4);INT16ToCh(p,PassWord.Keyt); break;	//���η�����λ������Կ��(TIM4->CNT�䵱�����)
     

//    case 120: INT16ToCh(p,Fly[0].SA); break;
//    case 121: INT16ToCh(p,Fly[1].SA); break;
//    case 122: INT16ToCh(p,Fly[2].SA); break;
//    case 123: INT16ToCh(p,Fly[3].SA); break;
//    case 124: INT16ToCh(p,Fly[4].SA); break;
//    case 125: INT16ToCh(p,(INT16U)(Fly[0].delval*100.0)); break;
//    case 126: INT16ToCh(p,(INT16U)(Fly[1].delval*100.0)); break;
//    case 127: INT16ToCh(p,(INT16U)(Fly[2].delval*100.0)); break;
//    case 128: INT16ToCh(p,(INT16U)(Fly[3].delval*100.0)); break;
//    case 129: INT16ToCh(p,(INT16U)(Fly[4].delval*100.0)); break;
//    case 130: INT16ToCh(p,(int)TimeDiff[0]); break;
//    case 131: INT16ToCh(p,(int)TimeDiff[1]); break;
//    case 132: INT16ToCh(p,(int)TimeDiff[2]); break;
//    case 133: INT16ToCh(p,(int)TimeDiff[3]); break;
//    case 134: INT16ToCh(p,(int)TimeDiff[4]); break;
    
    case 150: INT16ToCh(p,Cfg.PulseWidth); break;
    case 151: INT16ToCh(p,Cfg.OutFlowQ); break;
    case 152: INT16ToCh(p,Cfg.mA4); break;
    case 153: INT16ToCh(p,Cfg.mA20); break;
	
    case 170: INT16ToCh(p,Cfg.TimeDiffOffset[0]); break;
    case 171: INT16ToCh(p,Cfg.TimeDiffOffset[1]); break;
    case 172: INT16ToCh(p,Cfg.TimeDiffOffset[2]); break;
    case 173: INT16ToCh(p,Cfg.TimeDiffOffset[3]); break;
    case 174: INT16ToCh(p,Cfg.TimeDiffOffset[4]); break;
    case 175: INT16ToCh(p,Cfg.TimeDiffOffset[5]); break;
    case 176: INT16ToCh(p,Cfg.TimeDiffOffset[6]); break;
    case 177: INT16ToCh(p,Cfg.TimeDiffOffset[7]); break;

    case 200: INT16ToCh(p,FlowMeter1.SA[0]); break;
    case 201: INT16ToCh(p,FlowMeter1.SQ[0]); break;
    case 202: FloatToCh(p,FlowMeter1.FlyTime[0]); break;
    case 203: break;
    case 204: FloatToCh(p,FlowMeter1.TimeDiff[0]); break;
    case 205: break;
    case 206: INT16ToCh(p,FlowMeter1.SA[1]); break;
    case 207: INT16ToCh(p,FlowMeter1.SQ[1]); break;
    case 208: FloatToCh(p,FlowMeter1.FlyTime[1]); break;
    case 209: break;
    case 210: FloatToCh(p,FlowMeter1.TimeDiff[1]); break;
    case 211: break;
    
    case 212: INT16ToCh(p,FlowMeter1.SA[2]); break;
    case 213: INT16ToCh(p,FlowMeter1.SQ[2]); break;
    case 214: FloatToCh(p,FlowMeter1.FlyTime[2]); break;
    case 215: break;
    case 216: FloatToCh(p,FlowMeter1.TimeDiff[2]); break;
    case 217: break;
    
    case 218: INT16ToCh(p,FlowMeter1.SA[3]); break;
    case 219: INT16ToCh(p,FlowMeter1.SQ[3]); break;
    case 220: FloatToCh(p,FlowMeter1.FlyTime[3]); break;
    case 221: break;
    case 222: FloatToCh(p,FlowMeter1.TimeDiff[3]); break;
    case 223: break;
    
    case 224: INT16ToCh(p,FlowMeter1.Ver); break; //����汾��
    case 225: INT16ToCh(p,FlowMeter1.Status); break; //����״̬
    case 226: FloatToCh(p,FlowMeter1.Temp1); break;
    case 227: break;
    case 228: FloatToCh(p,FlowMeter1.Temp2); break;
    case 229: break;
	
    case 230: INT16ToCh(p,FlowMeter2.SA[0]); break;
    case 231: INT16ToCh(p,FlowMeter2.SQ[0]); break;
    case 232: FloatToCh(p,FlowMeter2.FlyTime[0]); break;
    case 233: break;
    case 234: FloatToCh(p,FlowMeter2.TimeDiff[0]); break;
    case 235: break;
    case 236: INT16ToCh(p,FlowMeter2.SA[1]); break;
    case 237: INT16ToCh(p,FlowMeter2.SQ[1]); break;
    case 238: FloatToCh(p,FlowMeter2.FlyTime[1]); break;
    case 239: break;
    case 240: FloatToCh(p,FlowMeter2.TimeDiff[1]); break;
    case 241: break;
   
    case 242: INT16ToCh(p,FlowMeter2.SA[2]); break;
    case 243: INT16ToCh(p,FlowMeter2.SQ[2]); break;
    case 244: FloatToCh(p,FlowMeter2.FlyTime[2]); break;
    case 245: break;
    case 246: FloatToCh(p,FlowMeter2.TimeDiff[2]); break;
    case 247: break;
    
    case 248: INT16ToCh(p,FlowMeter2.SA[3]); break;
    case 249: INT16ToCh(p,FlowMeter2.SQ[3]); break;
    case 250: FloatToCh(p,FlowMeter2.FlyTime[3]); break;
    case 251: break;
    case 252: FloatToCh(p,FlowMeter2.TimeDiff[3]); break;
    case 253: break;
    
    case 254: INT16ToCh(p,FlowMeter2.Ver); break; //����汾��
    case 255: INT16ToCh(p,FlowMeter2.Status); break; //����״̬
    case 256: FloatToCh(p,FlowMeter2.Temp1); break;
    case 257: break;
    case 258: FloatToCh(p,FlowMeter2.Temp2); break;
    case 259: break;	
	
    default: INT16ToCh(p,0);	
    }	
}

#endif

//--ModBusд�Ĵ���������������ָ����ָ��ĵ�ַ���ַ�ת����д���Ӧ�Ĵ��� ��Ҫ������֤��ſɵ��øú���
void MbRegWrite(INT16U addr,void *pp)
{
    char *p=pp;
    switch(addr)
    {
    case 20: ChToINT16(p); break;
    case 21: ChToINT16(p); break;
    case 22: RunTime.Fault = (long)(ChToFloat(p)*3600.0); break;
    case 23: break;
    case 24: RunTime.All = (long)(ChToFloat(p)*3600.0); break;
    case 25: break;
    case 26: if(ChToFloat(p)==0){SumFlowQ.Plus = 0; WrCmd |= WR_SUMFLOWQ;} break;
    case 27: break;
    case 28: if(ChToFloat(p)==0){SumFlowQ.Neg = 0;  WrCmd |= WR_SUMFLOWQ;} break;
    case 29: break;
    case 30: if(ChToFloat(p)==0){SumFlowQ.Day = 0;  WrCmd |= WR_SUMFLOWQ;} break;
    case 31: break;
    case 32: if(ChToFloat(p)==0){SumFlowQ.Mon = 0;  WrCmd |= WR_SUMFLOWQ;} break;
    case 33: break;
    case 34: if(ChToFloat(p)==0){SumFlowQ.Year = 0; WrCmd |= WR_SUMFLOWQ;} break;
    case 35: break;   
    case 36: ChToINT16(p); break;
    case 37: ChToINT16(p); break;
    case 38: if(ChToFloat(p)==0){SumHeatQ.Plus = 0; WrCmd |= WR_SUMHEATQ;}break;   
    case 39: break;   
    case 40: if(ChToFloat(p)==0){SumHeatQ.Neg = 0;  WrCmd |= WR_SUMHEATQ;}break;   
    case 41: break;   
    case 42: if(ChToFloat(p)==0){SumHeatQ.Day = 0;  WrCmd |= WR_SUMHEATQ;}break;   
    case 43: break;   
    case 44: if(ChToFloat(p)==0){SumHeatQ.Mon = 0;  WrCmd |= WR_SUMHEATQ;}break;   
    case 45: break;   
    case 46: if(ChToFloat(p)==0){SumHeatQ.Year = 0; WrCmd |= WR_SUMHEATQ;}break;   
    case 47: break;   
    case 48: Cfg.ChNum = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 49: Cfg.MeterType = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 50: Cfg.Type = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 51: Cfg.Angle = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 52: Cfg.Diameter = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 53: Cfg.SoundV = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 54: Cfg.FlowFactor = ChToINT16(p); WrCmd |= WR_CFG; break;
    
    case 56: Cfg.TimeDiffThreshold = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 57: Cfg.FlowThreshold = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 58: Cfg.HeatQFactor = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 59: Cfg.SpecificHeat = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 60: ChToINT16(p); break;
    //case 61: Cfg.SampleRate = ChToINT16(p); WrCmd |= FLAG_CFG; break;
    case 62: Cfg.Damp = ChToINT16(p); if(Cfg.Damp < 1) {Cfg.Damp = 1;} if(Cfg.Damp > 64) {Cfg.Damp = 64;} WrCmd |= WR_CFG; break;
    case 63: Cfg.Dot = ChToINT16(p); if(Cfg.Dot > 5) {Cfg.Dot = 5;} WrCmd |= WR_CFG; break;
    //case 64: Cfg.Unit = ChToINT16(p); WrCmd |= FLAG_CFG; break;
    case 65: Cfg.Qmax = ChToFloat(p); WrCmd |= WR_CFG; break;   
    case 66: break;   
    case 67: Cfg.Qmin = ChToFloat(p); WrCmd |= WR_CFG; break;   
    case 68: break;    
    
//    case 70: CalPar.TimeDiffOffset[0] = ChToINT16(p); WrCmd |= FLAG_CALPAR; break;
//    case 71: CalPar.TimeDiffOffset[1] = ChToINT16(p); WrCmd |= FLAG_CALPAR; break;
//    case 72: CalPar.TimeDiffOffset[2] = ChToINT16(p); WrCmd |= FLAG_CALPAR; break;
//    case 73: CalPar.TimeDiffOffset[3] = ChToINT16(p); WrCmd |= FLAG_CALPAR; break;
//    case 65: ChToINT16(p); break;
//   case 66: ChToINT16(p); break;
//  case 67: memcpy(&MenuPar.Loop[0],p,2); WrCmd |= FLAG_MENUPAR; break;
//  case 68: memcpy(&MenuPar.Loop[2],p,2); WrCmd |= FLAG_MENUPAR; break;
//  case 69: memcpy(&MenuPar.Loop[4],p,2); WrCmd |= FLAG_MENUPAR; break;
//  case 70: memcpy(&MenuPar.Loop[6],p,2); WrCmd |= FLAG_MENUPAR; break;
//  case 71: MenuPar.SwapTime = p[0];MenuPar.NoOpsTime = p[1]; WrCmd |= FLAG_MENUPAR; break;
 //   case 72: ChToINT16(p); break;
//    case 73: ChToINT16(p); break;
 //   case 78: ChToINT16(p); break;
 //   case 79: ChToINT16(p); break;
//    case 80: ChToINT16(p); break;
//    case 81: ChToINT16(p); break;
    case 75: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+0*7*16,0,16*7);} break;//16�����õ��Ƿѹ��־
    case 76: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+1*7*16,0,16*7);} break;//16�����������ϻ����ܱ�־
    case 77: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+2*7*16,0,16*7);} break;//16��ˮ�������־
    case 78: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+3*7*16,0,16*7);} break;//16����������Q4��־
    case 79: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+4*7*16,0,16*7);} break;//16������С��Q1��־
    case 80: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISEVENT+5*7*16,0,16*7);} break;//16��ˮ�³���Χ��־
    
    case 82: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+0,0,80);} break;//8��������ʷ����
    case 83: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+80,0,320);} break;//32��������ʷ����
    case 84: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+400,0,640);} break;//64��������ʷ����
    case 85: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+1040,0,160);} break;//16���ϵ�ʱ�ۻ�����

    //case 85: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+(16+64+64)*4,0,16*4);} break;//16��������ʷ����
   // case 86: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+(16+64+64+16)*4,0,64*4);} break;//64��������ʷ����
   // case 87: if(ChToINT16(p)==0x5a3c){WriteFMFill(ADDR_HISYMD+(16+64+64+16+64)*4,0,64*4);} break;//64��������ʷ����
    case 88: ChToINT16(p); break;
    case 89: ChToINT16(p); break;
    case 90: Time.Year= p[0]; Time.Mon  = p[1]; WrCmd |= WR_TIME; break;
    case 91: Time.Day = p[0]; Time.Hour = p[1]; WrCmd |= WR_TIME; break;
    case 92: Time.Min = p[0]; Time.Sec  = p[1]; WrCmd |= WR_TIME; break;
    case 93: ChToINT16(p); break;
    case 94: ChToINT16(p); break;
    case 95: ChToINT16(p); break;
    case 96: Cfg.Num = ((INT16U)p[0]<<8)+p[1]; Cfg.Num = (Cfg.Num<<16)+((INT16U)p[2]<<8)+p[3]; WrCmd |= WR_CFG;break;
    case 97: break;
    case 98: ChToINT16(p); break;
    case 99: //�¶�У׼ 
	  {
		INT16S stmp;
		stmp = ChToINT16(p);
		Cfg.HeatCor = stmp - (HeatTemp-Cfg.HeatCor);
		Cfg.ColdCor = stmp - (ColdTemp-Cfg.ColdCor);
        if(Cfg.HeatCor>1270) Cfg.HeatCor = 1270;
        if(Cfg.HeatCor<-1270) Cfg.HeatCor = -1270;
        if(Cfg.ColdCor>1270) Cfg.ColdCor = 1270;
        if(Cfg.ColdCor<-1270) Cfg.ColdCor = -1270;    
        WrCmd |= WR_CFG;
	  }
        break;
    case 100: ChToINT16(p); break;
    case 101: Cfg.Addr = ChToINT16(p); WrCmd |= WR_CFG; break;
	case 102: 
		switch(ChToINT16(p))
		{
			case 1200: Cfg.ComCfg = 2;break;
			case 2400: Cfg.ComCfg = 5;break;
			case 4800: Cfg.ComCfg = 8;break;
			case 19200: Cfg.ComCfg = 14;break;
			default : Cfg.ComCfg = 11; //����9600��У��
		}		 
	    break;
    case 150: Cfg.PulseWidth = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 151: Cfg.OutFlowQ = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 152: Cfg.mA4 = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 153: Cfg.mA20 = ChToINT16(p); WrCmd |= WR_CFG; break;
	  
    case 170: Cfg.TimeDiffOffset[0] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 171: Cfg.TimeDiffOffset[1] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 172: Cfg.TimeDiffOffset[2] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 173: Cfg.TimeDiffOffset[3] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 174: Cfg.TimeDiffOffset[4] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 175: Cfg.TimeDiffOffset[5] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 176: Cfg.TimeDiffOffset[6] = ChToINT16(p); WrCmd |= WR_CFG; break;
    case 177: Cfg.TimeDiffOffset[7] = ChToINT16(p); WrCmd |= WR_CFG; break;
    default:;
    }
}



void ModBusProtocol(void)//ModBusЭ����մ���
{
	
    if((Rx3Buf[0] == Cfg.Addr)&&(CalCRC16(Rx3Buf,Rx3Num)==0))//�Ǳ����ĵ�ַ �ҽ�������CRC��ȷ
    {
        INT16U DataAddr;
        DataAddr = ChToINT16(&Rx3Buf[2]);
        switch(Rx3Buf[1])
        {
            case 3://���Ĵ�������
            case 4://���Ĵ�������
            {
                INT16U Reg_n;//��ȡ�Ĵ����ĸ���
                Reg_n = ChToINT16(&Rx3Buf[4]);//��ȡ�ļĴ�������
                
                if((DataAddr >= 75)&&(DataAddr <= 80))//�����������ʷ��¼,ָ�����ModBusЭ�� ������ȡ��ʷ��¼�еĵ�Reg_n����¼
                {    
                    char ch[7];
                    switch(DataAddr)
                    {
                    case 75: ReadFM(ADDR_HISEVENT+0*7*16+((HisP.UnderVoltage-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16�����õ��Ƿѹ��־
                    case 76: ReadFM(ADDR_HISEVENT+1*7*16+((HisP.SensorFault-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16�����������ϻ����ܱ�־
                    case 77: ReadFM(ADDR_HISEVENT+2*7*16+((HisP.ReverseFlow-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16��ˮ�������־
                    case 78: ReadFM(ADDR_HISEVENT+3*7*16+((HisP.FlowQmax-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16����������Q4��־
                    case 79: ReadFM(ADDR_HISEVENT+4*7*16+((HisP.FlowQmin-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16������С��Q1��־
                    case 80: ReadFM(ADDR_HISEVENT+5*7*16+((HisP.TempErr-Reg_n)&0x0f)*7,&ch,sizeof(ch)); break;//16��ˮ�³���Χ��־
                    default:;
                    }  
                    Tx3Buf[2]=7;              
                    memcpy(&Tx3Buf[3],ch,sizeof(ch));
                    Read_Ret();//���ڶ������
                }
                else if((DataAddr >= 82)&&(DataAddr <= 85))//�����������ʷ��¼,ָ�����ModBusЭ�� ������ȡ��ʷ��¼�еĵ�Reg_n����¼
                {
                    char ch[10];
                    switch(DataAddr)
                    {
                    case 82: ReadFM(ADDR_HISYMD+0+((HisP.Year-Reg_n)&0x07)*10,&ch,sizeof(ch)); break;//��ȡ��ʷ�������¼
                    case 83: ReadFM(ADDR_HISYMD+80+((HisP.Mon-Reg_n)&0x1f)*10,&ch,sizeof(ch)); break;//��ȡ��ʷ�����¼�¼
                    case 84: ReadFM(ADDR_HISYMD+400+((HisP.Day-Reg_n)&0x3f)*10,&ch,sizeof(ch)); break;//��ȡ��ʷ�����ռ�¼
                    case 85: ReadFM(ADDR_HISYMD+1040+((HisP.PowerDown-Reg_n)&0x0f)*10,&ch,sizeof(ch)); break;//��ȡ16���ϵ�ʱ�ۻ�����
                    //case 85: ReadFM(ADDR_HISYMD+(16+64+64)*4+((HisP.Year-Reg_n)&0x0f)*4,&ftmp,sizeof(ftmp)); break;//��ȡ��ʷ�������¼
                    //case 86: ReadFM(ADDR_HISYMD+(16+64+64+16)*4+((HisP.Mon-Reg_n)&0x3f)*4,&ftmp,sizeof(ftmp)); break;//��ȡ��ʷ�����¼�¼ 
                    //case 87: ReadFM(ADDR_HISYMD+(16+64+64+16+64)*4+((HisP.Day-Reg_n)&0x3f)*4,&ftmp,sizeof(ftmp)); break;//��ȡ��ʷ�����ռ�¼
                    default:;
                    }      
                    Tx3Buf[2]=10;              
                    memcpy(&Tx3Buf[3],ch,sizeof(ch));
                    Read_Ret();//���ڶ������
                }
                else if((DataAddr+Reg_n) < 300)//��ཫ�Ĵ�����ַ����300
                {
                    INT8U *p = &Tx3Buf[3];
                    Tx3Buf[2]=Reg_n*2;
                    while(Reg_n>0)
                    {
                         MbRegRead(DataAddr,p);
                         DataAddr++;
                         p += 2;
                         Reg_n--;
                    }
                    Read_Ret();//���ڶ������
                }
                else//����
                {
                
                }
            }
            break;
  
            case 6:    //д�����Ĵ��� ��ע����֧����д
            {
                if((DataAddr == 101)||(DataAddr == 102))//������ַ���豸������
                {
                    MbRegWrite(DataAddr,&Rx3Buf[4]);
                    Write_Ret();
                }
                else if(DataAddr == 103)//��Կ��������֤
                {
                    if(PassWord.ForbidKeytDown==0)//��ֹ��Կʱ��ĵ���ʱ(�룩 ������д�����������ֹ������֤��ʱ�䣩
                    {
                        INT16U RecPassWord;
                        INT8U CrcBuf[2];
                        RecPassWord = ChToINT16(&Rx3Buf[4]);  
                        //PassWord.KeytΪ���η�����λ������Կ��
                        INT16ToCh(CrcBuf,PassWord.Keyt); 
                        PassWord.Keyt = CalCRC16(CrcBuf,2);
                        if(PassWord.Keyt==RecPassWord)//������֤ͨ��
                        {
                            PassWord.CountDown = 60;//������Ч��60s��������Ϊ255s��
                            Write_Ret();
                        }
                        else//������֤����
                        {
                            PassWord.CountDown = 0;//����ʧЧ
                            PassWord.ForbidKeytDown = 60;//60���ڽ�ֹ�����ٴ���֤��������Ϊ255s��
                        }
                    }
                }
                else if((PassWord.CountDown > 0)&&(DataAddr<300))//������Ч,�ҼĴ�����ַ��ȷ �ĵ���д����
                {
                    MbRegWrite(DataAddr,&Rx3Buf[4]);
                    Write_Ret();
                }
                else//������Чʱ��д����
                {
                    
                }
            }
            break;
            case 0x10://����д�Ĵ���
            {
                INT16U Reg_n;//��ȡ�Ĵ����ĸ���
                Reg_n = ChToINT16(&Rx3Buf[4]);//д�Ĵ�������	
				
                if((PassWord.CountDown > 0)&&//������Ч
                   (Rx3Buf[6] == Reg_n*2)&&//������ȷ
                   ((DataAddr+Reg_n)<=300))//��ཫ�Ĵ�������д��300
                {
                    INT8U *p = &Rx3Buf[7];
					while(Reg_n>0)
					{			  
						MbRegWrite(DataAddr,p);
						DataAddr++;
						p += 2;
						Reg_n--;
					}
					Write_Ret();
                }
                else//����
                {
                
                }
            }
            default:;//��֧�ֵ�����
        }
    }
    else
    {
        //00 03/04 00 65 00 01 crcH crcL 
        if((Rx3Buf[0] == 0x00)&&((Rx3Buf[1] == 0x03)||(Rx3Buf[1] == 0x04))&&//�ж��Ƿ��ǹ㲥��ʽ����ַ
         (ChToINT16(&Rx3Buf[2])==101)&&(ChToINT16(&Rx3Buf[4])==1)&&(CalCRC16(Rx3Buf,Rx3Num)==0))
        {
             //�ش�������ַ
            INT8U *p= &Tx3Buf[3];
            Tx3Buf[2]=1*2;
            MbRegRead(101,p);
            Read_Ret();//���ڶ������
        }
    }    
    
    Rx3Num=0; //���ո���
}






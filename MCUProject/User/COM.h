		   
void COM1Init(void); //--ˮ��ͨѶ�� �ô���Ϊ 9600 N 8 1
	  	
void Tx1Send(void); //ʹ����1��ʼ��������

void COM2Init(void); //--ˮ��ͨѶ�� �ô���Ϊ 9600 N 8 1
	  	
void Tx2Send(void); //ʹ����2��ʼ��������


//--����λ��ͨ�ſ�---
void COM3Init(INT16U BandRate, char parity, char DataN);//���� COM3Init(9600,'N',8);//Ϊ9600 N	 8
		
void Tx3Send(void); //ʹ����3��ʼ��������
	

void COM4Init(void); //--��ʾ�ӿ� �ô���Ϊ 115200 N 8 1---

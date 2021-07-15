		   
void COM1Init(void); //--水表通讯口 该串口为 9600 N 8 1
	  	
void Tx1Send(void); //使串口1开始发送数据

void COM2Init(void); //--水表通讯口 该串口为 9600 N 8 1
	  	
void Tx2Send(void); //使串口2开始发送数据


//--与上位机通信口---
void COM3Init(INT16U BandRate, char parity, char DataN);//例如 COM3Init(9600,'N',8);//为9600 N	 8
		
void Tx3Send(void); //使串口3开始发送数据
	

void COM4Init(void); //--显示接口 该串口为 115200 N 8 1---

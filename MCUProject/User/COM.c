
#include "global.h"
		
	  
//--水表通讯口 该串口为 9600 N 8 1---
void COM1Init(void)
{				 	  
    USART_InitTypeDef       USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitStructure.USART_BaudRate            = 9600;
    USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity          = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);			 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	RX1EN;
}

	  	
void Tx1Send(void) //使串口1开始发送数据
{		   			
    TX1EN;	
	OSTimeDly(mS(8)); 
    Tx1Cnt = 0;								  
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}	  

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
	    Rx1OverTime = 0;
        Rx1Buf[Rx1Num]=USART_ReceiveData(USART1) & 0xFF;
        if(Rx1Num < sizeof(Rx1Buf)-1)
        {
            Rx1Num++;
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
    }			  
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{			  
		if(Tx1Cnt < Tx1Num)
        {
        	USART_SendData(USART1, Tx1Buf[Tx1Cnt++]);
        }
        else//已发送完毕，使能发送完成中断
        {
		    USART_ITConfig(USART1, USART_IT_TXE, DISABLE); 
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
        }			 
        USART_ClearITPendingBit(USART1, USART_IT_TXE);  
    }  
    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET) 	//发送完成中断
	{													  
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);			   
		RX1EN;		
	}
}



		  
//--水表通讯口 该串口为 9600 N 8 1---
void COM2Init(void)
{				 	  
    USART_InitTypeDef       USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_InitStructure.USART_BaudRate            = 9600;
    USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity          = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);			 
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	RX2EN;
}

	  	
void Tx2Send(void) //使串口2开始发送数据
{		   			
    TX2EN;	 
	OSTimeDly(mS(8));
    Tx2Cnt = 0;								  
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}	  

void USART2_IRQHandler(void)
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
	    Rx2OverTime = 0;
        Rx2Buf[Rx2Num]=USART_ReceiveData(USART2) & 0xFF;
        if(Rx2Num < sizeof(Rx2Buf)-1)
        {
            Rx2Num++;
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
  }		
	
  if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
	{			  
		if(Tx2Cnt < Tx2Num)
        {
        	USART_SendData(USART2, Tx2Buf[Tx2Cnt++]);
        }
        else//已发送完毕，使能发送完成中断
        {
		    USART_ITConfig(USART2, USART_IT_TXE, DISABLE); 
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
        }			 
        USART_ClearITPendingBit(USART2, USART_IT_TXE);  
  }  
	
  if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) 	//发送完成中断
	{													  
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);			   
		RX2EN;		
	}
}


//--与上位机通信口---
void COM3Init(INT16U BandRate, char parity, char DataN)//例如 COM3Init(9600,'N',8);//为9600 N	 8
{
    USART_InitTypeDef       USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_InitStructure.USART_BaudRate            = BandRate;
	if(DataN == 7)
	{
    	USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
	}
	else
	{
		if((parity == 'o')||(parity == 'O')||(parity == 'e')||(parity == 'E'))
		{
    		USART_InitStructure.USART_WordLength      = USART_WordLength_9b;
		}
		else
		{ 			
    		USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
		}
	}
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;												   
    if((parity == 'o')||(parity == 'O'))
    {
    	USART_InitStructure.USART_Parity          = USART_Parity_Odd ;
    }
    else if((parity == 'e')||(parity == 'E'))
    {
    	USART_InitStructure.USART_Parity          = USART_Parity_Even ;
    }
	else
	{
    	USART_InitStructure.USART_Parity          = USART_Parity_No ;
	}
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE); 
	
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE);	
	RX3EN;
}			
					
void Tx3Send(void) //使串口3开始发送数据
{		   			
    TX3EN;	 
	OSTimeDly(mS(10));
	if((USART3->CR1 & 0x0400) == 0) //判断通讯如果无校验		
	{
		//发送两个停止位 接收一个停止位
		USART3->CR2 = (USART3->CR2 & (~0x3000))|USART_StopBits_2;
	}
    Tx3Cnt = 0;								  
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}	  

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
	    Rx3OverTime = 0;
        Rx3Buf[Rx3Num]=USART_ReceiveData(USART3) & 0xFF;
        if(Rx3Num < sizeof(Rx3Buf)-1)
        {
            Rx3Num++;
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
    }			  
    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) 
	{			  
		if(Tx3Cnt < Tx3Num)
        {
        	USART_SendData(USART3, Tx3Buf[Tx3Cnt++]);
        }
        else//已发送完毕，使能发送完成中断
        {
		    USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
        }			 
        USART_ClearITPendingBit(USART3, USART_IT_TXE);  
    }  
    if (USART_GetITStatus(USART3, USART_IT_TC) != RESET) 	//发送完成中断
	{													  
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);			   
		RX3EN;							  	
		
		if((USART3->CR1 & 0x0400) == 0) //判断通讯如果无校验	
		{
			//发送两个停止位 接收一个停止位
			USART3->CR2 = (USART3->CR2 & (~0x3000))|USART_StopBits_1;
		}
	}
}
 
		  

void COM4Init(void) //--显示接口 该串口为 115200 N 8 1---
{				 	  
    USART_InitTypeDef       USART_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity          = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);
    USART_Cmd(UART4, ENABLE);			 
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
		
}

void UART4_IRQHandler(void)
{
	//接收中断
  if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		Rx4OverTime = 0;
		Rx4Buf[Rx4Num]=USART_ReceiveData(UART4) & 0xFF;
		
		//5A A5 03 82 4F 4B      
		if(Rx4Num < sizeof(Rx4Buf)-1)
		{
				Rx4Num++;
		}
		//应答消息清除
		if((Rx4Num > 4) && 
				(Rx4Buf[Rx4Num - 1] == 0x4b) && 
				(Rx4Buf[Rx4Num - 2] == 0x4f) && 
				(Rx4Buf[Rx4Num - 4] == 0x03) &&
				(Rx4Buf[Rx4Num - 5] == 0xA5) &&
				(Rx4Buf[Rx4Num - 6] == 0x5A) )
		{
			Rx4Num -= 6;
		}
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
  }			  
	//空闲中断 
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
		int clear = UART4->DR;
		clear = UART4->SR;

		USART_ITConfig(UART4, USART_IT_IDLE, DISABLE);
	}
	 
	 
	 
//    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET) 
//	{			  
//		if(Tx4Cnt < Tx4Num)
//        {
//        	USART_SendData(UART4, Tx4Buf[Tx4Cnt++]);
//        }
//        else//已发送完毕，使能发送完成中断
//        {
//		    USART_ITConfig(UART4, USART_IT_TXE, DISABLE); 
//        }			 
//        USART_ClearITPendingBit(UART4, USART_IT_TXE);  
//    }  
}


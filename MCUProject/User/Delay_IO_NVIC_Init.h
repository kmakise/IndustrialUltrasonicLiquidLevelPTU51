
//8M晶振下调用该函数实际延时时间为 常量(us+2)us
void Delayus(INT16U us);

//8M晶振下调用该函数
void Delayms(INT16U ms);

void IWDG_Reset(void);//使用看门狗复位
		
void IO_Init(void); //IO初始化

void NVIC_Configuration(void);//NVIC配置
 

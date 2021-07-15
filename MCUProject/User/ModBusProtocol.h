
void ModBusProtocol(void);//ModBus协议接收处理

void FloatToCh(void *p,float f);//--将浮点数转换为ModBus格式的字符

float ChToFloat(void *p);//--将ModBus格式的字符转换为浮点数

void INT16ToCh(void *p,INT16U Int);//--整型数转换为ModBus格式的字符

INT16S ChToINT16(void *p);//--ModBus格式的字符转换为整型数

INT32S ChToLong(void *p);//--将ModBus格式的字符转换为浮点数

void LongToCh(void *p,INT32S l);//--将long型数据转换为ModBus格式的字符

void W_M_Init(void);

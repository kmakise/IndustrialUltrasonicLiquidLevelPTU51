
void ModBusProtocol(void);//ModBusЭ����մ���

void FloatToCh(void *p,float f);//--��������ת��ΪModBus��ʽ���ַ�

float ChToFloat(void *p);//--��ModBus��ʽ���ַ�ת��Ϊ������

void INT16ToCh(void *p,INT16U Int);//--������ת��ΪModBus��ʽ���ַ�

INT16S ChToINT16(void *p);//--ModBus��ʽ���ַ�ת��Ϊ������

INT32S ChToLong(void *p);//--��ModBus��ʽ���ַ�ת��Ϊ������

void LongToCh(void *p,INT32S l);//--��long������ת��ΪModBus��ʽ���ַ�

void W_M_Init(void);

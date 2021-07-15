


typedef struct
{
    INT8U Year;
    INT8U Mon;        
    INT8U Day;
    INT8U Hour;        
    INT8U Min;
    INT8U Sec;
} TIME;//ʱ�� 

//ע1������״̬
#define FAULT_TIMEDIFF1    (1<<0)//ʱ��1����
#define FAULT_TIMEDIFF2    (1<<1)//ʱ��2����
#define FAULT_TIMEDIFF3    (1<<2)//ʱ��3����
#define FAULT_TIMEDIFF4    (1<<3)//ʱ��4����

#define FAULT_PT1       (1<<8)//�¶�1����
#define FAULT_PT2       (1<<9)//�¶�2����
#define FAULT_ALLERR     0xffff//��ͨѶ

typedef struct  
{
	INT8U Ver;//ˮ��汾��*0.1
    INT16U Status;//����״̬
	INT8U SA[4];    //��ͨ�����ź�ǿ��
    INT16U SQ[4];//�ź�����
    float FlyTime[4];//��������ʱ��us
    float TimeDiff[4];//ʱ��ns
    float Temp1;//�¶�1
    float Temp2;//�¶�2
}FLOW_METER;



typedef struct  
{
    INT8U Addr;//�豸��ַ     ͨ��ʱ��ʶ���ַ
	INT8U ComCfg;//ͨѶ����������
    INT8U Damp;//����ϵ��1��32
    INT8U Dot;//�ۻ���С����λ��
    INT8U MeterType;//0Ϊˮ�� 1Ϊ�ȱ�
    INT8U Type;//��װ����     Pi��Ϊ0 ����Ϊ 1��V�� 2��M��Ϊ 4  
	INT16U ChNum;//��Чͨ����  (1~8)
    INT16U Angle;//̽ͷ�н�      ��*0.01   (Pi��ʱΪ��װ��ֱ�߾���)*0.01mm            
    INT16U Diameter;//�ܵ�ֱ��   mm*0.1                       
    INT16U SoundV;  //�����ٶ�   m/s*0.1                   
    INT16U FlowFactor;  //����ϵ�� ϵ��*10000 ���� 10200����1.02  ��ʱ��������ת��ϵ������������������������ת��ϵ������Ϊ���ٱ�����ǹܵ���ƽ�����٣�            
    INT16U HeatQFactor;   //����ϵ�� ϵ��*10000 ���� 10200����1.02  ������������ת��ϵ����
	
    INT16U FlowThreshold;     //���پ���ֵ����ֵ�����ڸ�ֵ��0�㡣    ��λΪ0.1mm/s
	
    INT16U SpecificHeat;   //������    ��λΪJ/kg/��
    INT16U TimeDiffThreshold;     //ʱ�����ֵ����ֵ�����ڸ�ֵ��0�㡣    ��λΪ0.1ns
	INT16S TimeDiffOffset[8];//ʱ�������ֵ      ���ڵ�����ˮ�е�ʱ��ƫ�� ��λΪ0.1ns
    INT16S HeatCor;//�ȶ������¶� �Ŵ�100��   
    INT16S ColdCor;//��������¶� �Ŵ�100��  
	
    INT16U mA4;//   0��65535m3/h ��  0��65535kW 
    INT16U mA20;//  0��65535m3/h ��  0��65535kW
    INT16U PulseWidth;//OCT���������� ms
    INT16U OutFlowQ;//������� ÿ������������� 1��65535L 0Ϊ�ر�OCT���
		
    long Num;    //�豸���к�
    float Qmax;//m3/h
    float Qmin;//m3/h   
	INT16U yuliu1;
	INT16U yuliu2;
	//float yuliu3;
    INT16U yuliu5;
    INT16U crc;//
} CFG;//�豸����(�漰��λ��4~20mA��Ӧ�������������ã�



typedef struct
{
    INT16U yuliu1;
    INT16U yuliu2;
    INT16U yuliu3;
    INT16U yuliu4;
    INT16U yuliu5;
    INT16U crc;     //CRCУ�飬����У���������ݵ���ȷ��
} CORPAR;//��������  ��Ҫ����


typedef struct  // ��λmL
{
    long long Plus;// ���ۻ���        
    long long Neg; // ���ۻ���
    long long Day; // �����ۻ���
    long long Mon; // �����ۻ���
    long long Year;// �����ۻ���
	float yuliu1;
    INT16U yuliu2;//
    INT16U crc;//
} SUMFLOWQ;//�����ۻ��� ���籣�� ÿ����㱣��


typedef struct  // ��λJ
{
    long long Plus;// ���ۻ���        
    long long Neg; // ���ۻ���
    long long Day; // �����ۻ���
    long long Mon; // �����ۻ���
    long long Year;// �����ۻ���
	float yuliu1;
    INT16U yuliu2;//
    INT16U crc;//
} SUMHEATQ;//�����ۻ��� ���籣�� ÿ����㱣��


typedef struct  
{   
    long Fault; //��������ʱ�� s
    long All;// ������ʱ�� s   
    INT16U yuliu;	
    INT16U crc;//
} RUNTIME;//����ʱ��  ���籣�� ÿ����㱣��


typedef struct
{
    //INT16U Revise;//�����޸Ĳ���������
    INT16U Keyt;//���η�����λ������Կ��
    INT8U ForbidKeytDown;//��ֹ��Կʱ��ĵ���ʱ(�룩 ������д�����������ֹ������֤��ʱ�䣩��
    INT8U CountDown;//������֤ͨ����Чʱ��ĵ���ʱ���룩   
} PASSWORD;//


typedef struct
{
    INT8U SumFlowQYear[8][10];
    INT8U SumFlowQMon[32][10];
    INT8U SumFlowQDay[64][10];
    INT8U PowerDown[16][10];//�����¼ 
} HISYMD;//��ʷ��¼�ڴ洢���Ĵ洢�ṹ�����������ϴ󣬲���ָ��ָ��ķ�ʽѭ�����ã��˴�ֻ����ṹ����ռ��Ram�ռ�  

typedef struct
{
    INT8U UnderVoltage[16][7];//Ƿѹ
    INT8U SensorFault[16][7]; //���������ϻ�����
    INT8U ReverseFlow[16][7];//ˮ������
    INT8U FlowQmax[16][7];//��������Qmax
    INT8U FlowQmin[16][7];//����С��Qmin  
    INT8U TempErr[16][7];//ˮ�³���Χ
} HISEVENT;//��ʷ��¼�ڴ洢���Ĵ洢�ṹ�����������ϴ󣬲���ָ��ָ��ķ�ʽѭ�����ã��˴�ֻ����ṹ����ռ��Ram�ռ�  

typedef struct
{
    INT8U Year;
    INT8U Mon;
    INT8U Day;
    INT8U PowerDown;//�����¼ 
    
    INT8U UnderVoltage;//Ƿѹ
    INT8U SensorFault;//���������ϻ�����    
    INT8U ReverseFlow;//ˮ������
    INT8U FlowQmax;//��������Qmax
    INT8U FlowQmin;//����С��Qmin  
    INT8U TempErr;//ˮ�³���Χ
    
    INT16U crc;//
} HISP;//��ʷ��¼ ��ָ��  Year������0x07 Moon������0x1f Day������0x3f  ����0x0f


typedef enum //ö�ٰ����ļ�ֵ
{
    KEY_NO,KEY_UP,KEY_DOWN,KEY_ESC,KEY_OK,
} ENUMKEY;


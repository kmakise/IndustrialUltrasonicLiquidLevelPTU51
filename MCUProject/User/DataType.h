


typedef struct
{
    INT8U Year;
    INT8U Mon;        
    INT8U Day;
    INT8U Hour;        
    INT8U Min;
    INT8U Sec;
} TIME;//时间 

//注1：故障状态
#define FAULT_TIMEDIFF1    (1<<0)//时差1故障
#define FAULT_TIMEDIFF2    (1<<1)//时差2故障
#define FAULT_TIMEDIFF3    (1<<2)//时差3故障
#define FAULT_TIMEDIFF4    (1<<3)//时差4故障

#define FAULT_PT1       (1<<8)//温度1故障
#define FAULT_PT2       (1<<9)//温度2故障
#define FAULT_ALLERR     0xffff//无通讯

typedef struct  
{
	INT8U Ver;//水表版本号*0.1
    INT16U Status;//工作状态
	INT8U SA[4];    //各通道的信号强度
    INT16U SQ[4];//信号质量
    float FlyTime[4];//声音传播时间us
    float TimeDiff[4];//时差ns
    float Temp1;//温度1
    float Temp2;//温度2
}FLOW_METER;



typedef struct  
{
    INT8U Addr;//设备地址     通信时的识别地址
	INT8U ComCfg;//通讯波特率配置
    INT8U Damp;//阻尼系数1～32
    INT8U Dot;//累积量小数点位数
    INT8U MeterType;//0为水表 1为热表
    INT8U Type;//安装类型     Pi型为0 对射为 1，V型 2，M型为 4  
	INT16U ChNum;//有效通道数  (1~8)
    INT16U Angle;//探头夹角      度*0.01   (Pi型时为安装的直线距离)*0.01mm            
    INT16U Diameter;//管道直径   mm*0.1                       
    INT16U SoundV;  //声波速度   m/s*0.1                   
    INT16U FlowFactor;  //修正系数 系数*10000 例如 10200代表1.02  （时差与流速转换系数）（不再设立流速与流量转换系数，因为流速本身就是管道的平均流速）            
    INT16U HeatQFactor;   //修正系数 系数*10000 例如 10200代表1.02  （流量与热量转换系数）
	
    INT16U FlowThreshold;     //流速绝对值的阈值，低于该值按0算。    单位为0.1mm/s
	
    INT16U SpecificHeat;   //热容量    单位为J/kg/℃
    INT16U TimeDiffThreshold;     //时差绝对值的阈值，低于该值按0算。    单位为0.1ns
	INT16S TimeDiffOffset[8];//时差的修正值      用于抵消静水中的时差偏差 单位为0.1ns
    INT16S HeatCor;//热端修正温度 放大100倍   
    INT16S ColdCor;//冷端修正温度 放大100倍  
	
    INT16U mA4;//   0～65535m3/h 或  0～65535kW 
    INT16U mA20;//  0～65535m3/h 或  0～65535kW
    INT16U PulseWidth;//OCT脉冲输出宽度 ms
    INT16U OutFlowQ;//输出流量 每个脉冲代表流量 1～65535L 0为关闭OCT输出
		
    long Num;    //设备序列号
    float Qmax;//m3/h
    float Qmin;//m3/h   
	INT16U yuliu1;
	INT16U yuliu2;
	//float yuliu3;
    INT16U yuliu5;
    INT16U crc;//
} CFG;//设备配置(涉及单位、4~20mA对应物理量、等配置）



typedef struct
{
    INT16U yuliu1;
    INT16U yuliu2;
    INT16U yuliu3;
    INT16U yuliu4;
    INT16U yuliu5;
    INT16U crc;     //CRC校验，用于校验以上数据的正确性
} CORPAR;//修正参数  需要保存


typedef struct  // 单位mL
{
    long long Plus;// 正累积器        
    long long Neg; // 负累积器
    long long Day; // 本日累积器
    long long Mon; // 本月累积器
    long long Year;// 本年累积器
	float yuliu1;
    INT16U yuliu2;//
    INT16U crc;//
} SUMFLOWQ;//流量累积器 掉电保存 每日零点保存


typedef struct  // 单位J
{
    long long Plus;// 正累积器        
    long long Neg; // 负累积器
    long long Day; // 本日累积器
    long long Mon; // 本月累积器
    long long Year;// 本年累积器
	float yuliu1;
    INT16U yuliu2;//
    INT16U crc;//
} SUMHEATQ;//热量累积器 掉电保存 每日零点保存


typedef struct  
{   
    long Fault; //故障运行时间 s
    long All;// 总运行时间 s   
    INT16U yuliu;	
    INT16U crc;//
} RUNTIME;//运行时间  掉电保存 每日零点保存


typedef struct
{
    //INT16U Revise;//按键修改参数的密码
    INT16U Keyt;//本次发给上位机的密钥，
    INT8U ForbidKeytDown;//禁止密钥时间的倒计时(秒） （密码写入错误后产生禁止密码认证的时间），
    INT8U CountDown;//密码认证通过有效时间的倒计时（秒）   
} PASSWORD;//


typedef struct
{
    INT8U SumFlowQYear[8][10];
    INT8U SumFlowQMon[32][10];
    INT8U SumFlowQDay[64][10];
    INT8U PowerDown[16][10];//掉电记录 
} HISYMD;//历史记录在存储区的存储结构，因数据量较大，采用指针指向的方式循环利用，此处只定义结构，不占用Ram空间  

typedef struct
{
    INT8U UnderVoltage[16][7];//欠压
    INT8U SensorFault[16][7]; //传感器故障或不满管
    INT8U ReverseFlow[16][7];//水流反向
    INT8U FlowQmax[16][7];//流量大于Qmax
    INT8U FlowQmin[16][7];//流量小于Qmin  
    INT8U TempErr[16][7];//水温超范围
} HISEVENT;//历史记录在存储区的存储结构，因数据量较大，采用指针指向的方式循环利用，此处只定义结构，不占用Ram空间  

typedef struct
{
    INT8U Year;
    INT8U Mon;
    INT8U Day;
    INT8U PowerDown;//掉电记录 
    
    INT8U UnderVoltage;//欠压
    INT8U SensorFault;//传感器故障或不满管    
    INT8U ReverseFlow;//水流反向
    INT8U FlowQmax;//流量大于Qmax
    INT8U FlowQmin;//流量小于Qmin  
    INT8U TempErr;//水温超范围
    
    INT16U crc;//
} HISP;//历史记录 的指针  Year屏蔽字0x07 Moon屏蔽字0x1f Day屏蔽字0x3f  其余0x0f


typedef enum //枚举按键的键值
{
    KEY_NO,KEY_UP,KEY_DOWN,KEY_ESC,KEY_OK,
} ENUMKEY;


#define	EXTERN  

DWORD   Tout1, Ticks1;
#define SETTIMEOUT(x)	Tout1=x; Ticks1=GetTickCount()		
#define TIMEOUT			((GetTickCount() - Ticks1) > Tout1)		

int   Mjxt_author=0;
int   Sfxt_author=0;
int   Jsxt_author=0;

union _lic_data
{
	short int i[512];
	unsigned char ch[1024];
}LIC_DATA;

struct _sys_info
{	
	unsigned char ch[2];	
	int mjxt;
	int mjxt_sec;
	unsigned char mjxt_cardtype;
	int xfxt;	
	int xfxt_sec;    
	unsigned char syscard_no[5];
	unsigned char sys_keyb[8];
	unsigned char user_password[8];
	unsigned char oprater_password[8];
	unsigned char comm_password[8];
	unsigned char unuser[8];
}SysInfo, SI;

struct _sys_infonew
{	
	unsigned char ch[2];	
	int mjxt;
	int mjxt_sec;
	unsigned char mjxt_cardtype;
	int xfxt;	
	int xfxt_sec;
	int jsxt;   
    int jsxt_sec;
	unsigned char syscard_no_xf[5];
	unsigned char syscard_no_mj[5];
	unsigned char syscard_no_js[5];
	unsigned char sys_keyb[8];
	unsigned char user_password[8];
	unsigned char oprater_password[8];
	unsigned char comm_password[8];
	unsigned char unuser[6];
	unsigned char User_KeyAB[16];
	unsigned char User_Return_KeyAB[16];
}SysInfonew,SInew;

typedef	struct								//用户卡信息缓存
{
	unsigned long	Card_Number;			//发卡时的流水号
	unsigned long 	Rest_Money;				//卡余额
	unsigned long 	Used_Money;				//消费额
	unsigned long   Used_Recharge_Times;
	unsigned long   Used_All_Times;			
	unsigned long   Used_All_Money;		
	unsigned long   Used_Address;
	unsigned long   Card_Type;
	unsigned char   User_Password[3];
	unsigned char   User_EndTime[3];
	unsigned char   User_Name[6];
	unsigned char   Used_Time[6];
	unsigned char   User_CardData[48];
}_UserCardInfo;

typedef	struct								//用户卡高级信息结构体
{
	unsigned long	Card_Number;			//用户卡流水号
	unsigned long   Card_Type;				//用户卡类型
	unsigned long 	Rest_Money;				//用户卡余额
	unsigned long   Recharge_Times;			//充值次数
	unsigned long   Used_All_Times;			//用户卡累计使用次数
	unsigned long   Used_All_Money;			//用户卡累计消费金额	
	unsigned long 	Used_Money;				//最后一次消费额

	unsigned char   User_Password[4];		//用户卡超额密码(BCD)
	unsigned char   Used_Time[6];			//最后一次交易时间
	
	unsigned char   User_Name[20];			//用户卡编号(ASCII)
	unsigned char   User_ValidityPeriod[4];	//用户卡有效期

	char   User_KQ_Name[48];				//考勤专用信息-用户姓名
	char   User_KQ_Department[48];			//考勤专用信息-用户部门
	char   User_KQ_Number[48];				//考勤专用信息-用户编号
}_advanced_message;

typedef struct 
{
	long  MachineNo;
	unsigned long  TotalBalance;
}RecordData;

typedef struct
{
	unsigned long  Card_Number;
	unsigned long  Card_RestMoney;	
	unsigned long  Card_UsedMoney;
	unsigned long  Card_UsedTimes; 
	unsigned long  Card_Address;	
	unsigned char  Card_Times[12];
}RecordDetailedData;

typedef struct
{
	unsigned long  Materiel_All_Money;
	unsigned long  Materiel_All_Times;	
	unsigned long  Materiel_All_Amount;
	unsigned long  Card_Address;	
}RecordAllData;

typedef struct 
{
 	unsigned char ConsumeMode;		//消费方式
  	unsigned char MinFeeUnit;		//最小计费单位
	unsigned long Rate1;			//费率1
	unsigned long Rate2;			//费率2
	unsigned long Rate3;			//费率3
	unsigned long Rate4;			//费率4
	unsigned long Rate5;			//费率5
	unsigned long Rate6;			//费率6
	unsigned long Rate7;			//费率7
	unsigned long Rate8;			//费率8
}JS_FEE_RATE;
      
typedef struct 
{
	unsigned char   Grade_Rate1;						//为三种费率的阶梯比率          默认为0
 	unsigned char   Grade_Rate2;						//9-25分成3个值							
	unsigned char   Grade_Rate3;
	unsigned int    One_Grade;							//为一阶限制值                   两个字节   默认为0
	unsigned int    Two_Grade;							//为二阶限制值                   两个字节   默认为0
	unsigned int    Max_Consume_Times;					//当天最大消费次数				 两个字节   默认为0
	unsigned long   Max_Consume_Money;					//当天最大消费额（元）			 两个字节   默认为0
	unsigned long   Free_Time;							//当天免费时间（时，分，秒）     三个字节   默认为0
	unsigned int    Consume_Interval;					//消费的间隔时间2字节<秒>					默认为0
	unsigned int    Once_Max_Consume_Money;				//为单次的最大消费额             两个字节	默认为0
	unsigned char   En_Card_Type;						//允许使用的卡类默认0XFF  					
	unsigned char   Used_OneCard_Mode;					//保留使用默认为0
	unsigned char   En_Auto_Close;						//保留使用默认为0	
	unsigned char   User_Card_Flag;						//保留使用默认为0x20
	unsigned char 	Auto_Write_Record_Flag;				//保留使用默认为0
	unsigned char 	Auto_Lock_Used_Card;				//保留使用默认为0
	unsigned char 	Used_Card_Overdraft_Flag;			//保留使用默认为0  										
	unsigned char 	Used_Card_Overdraft_Flag2;			//保留使用默认为0	
	unsigned char 	GK_Cala_Key;						//保留使用默认为1
}JS_AdvanPara;

typedef	struct							
{
	unsigned long	RecordSnr;						//记录流水号
	unsigned long	CardNum;						//发卡时的流水号
	unsigned long 	Rest_Money;						//卡余额
	unsigned long 	Used_Money;						//消费额
	unsigned long	Used_All_Times;					//卡片使用次数
	unsigned long 	Terminal_ID;					//终端编号
	unsigned long 	Operater;						//操作员号 
	char UsedTime[12];								//使用时间
	unsigned char 	Status;							//交易状态

}XFREC;

typedef	struct							
{
	unsigned long	RecordSnr;						//记录流水号
	unsigned long	CardNum;						//发卡时的流水号	
	unsigned long	Rest_Money;						//卡余额
	unsigned long	Used_Money;						//消费额
	unsigned long	Used_All_Times;					//卡片使用次数
	unsigned long	Terminal_ID;					//终端编号
	unsigned long	Old_Rest_Money;	
	unsigned long	Get_Key_Money;	
	unsigned short	Operater;						//操作员号 
	unsigned char 	UsedTime[6];					//使用时间
	unsigned char 	CardUID[4];						//卡片物理ID
	unsigned char 	Step;							//记录进度
	unsigned char 	Status;							//交易状态
	unsigned char 	CardType;						//卡类
	unsigned char 	Crc;						    //效验	
	
}YCXFG30_UserRecordInfo;

char  Author_FileName[] = "./licence.das"; 
char  Author_FileNameNew[] = "./licencecard.dat";
//char  Author_FileNameNew[] = "C:\\WINDOWS\\system32\\licencecard.dat"; 

#define		CRC_PRESET					0xFFFF
#define		CRC_POLYNOM					0x1021
#define		USB_VID						(0x2011)
#define		USB_PID						(0x0318)

#define		COMMAND_START				0xAA5A
#define		COMMAND_END					0xBB6B

#define		DEVICE_ADDRESS 				0x0000

#define		COMM_ERR					"COMM_ERR"
#define		COMM_ERR_SIZE				sizeof(COMM_ERR)-1

#define		CONTROL_SET_TIME			"SET_TIME"
#define		CONTROL_SET_TIME_WORD		 0x13

#define		CONTROL_GET_TIME			"GET_TIME"
#define		CONTROL_GET_TIME_WORD		 0x14

#define		CONTROL_SET_BELL			"SET_BELL"
#define		CONTROL_SET_BELL_WORD		 0x15

#define		CONTROL_SET_DISPLAY			"SET_DISPLAY"
#define		CONTROL_SET_DISPLAY_WORD	 0x16

#define		CONTROL_REQUEST				"RF_REQUEST"
#define		CONTROL_REQUEST_WORD		 0x21

#define		CONTROL_ANTICOLL			"RF_ANTICOLL"
#define		CONTROL_ANTICOLL_WORD		 0x22

#define		CONTROL_SELECT				"RF_SELECT"
#define		CONTROL_SELECT_WORD			 0x23

#define		CONTROL_AUTHENTICATION		"RF_AUTHENTICATION"
#define		CONTROL_AUTHENTICATION_WORD	 0x24

#define		CONTROL_READ				"RF_READ"
#define		CONTROL_READ_WORD			 0x25

#define		CONTROL_WRITE				"RF_WRITE"
#define		CONTROL_WRITE_WORD			 0x26

#define		CONTROL_HALT				"RF_HALT"
#define		CONTROL_HALT_WORD			 0x27

#define		CONTROL_LOAD_KEY			"RF_LOAD_KEY"
#define		CONTROL_LOAD_KEY_WORD		 0x28

#define		CONTROL_OPERATION_COM		"OPERATION_COM"
#define		CONTROL_OPERATION_COM_WORD	 0x31

#define		CONTROL_BIG_DATA			"BIG_DATA"

#define		Files1						"OPERATION_EEP"

#define		CONTROL_OPEN_RF				"OPEN_RF"
#define		CONTROL_OPEN_RF_WORD		 0x20


#define		MAX_USB_BUF					50
#define		Time_Number					30
#define		Time_Number2				15
#define		Time_Number3				5
#define		Wait_Card_Time				500
#define		Wait_Card_Number			8
#define		Wait_MK_Times				15
#define		Wait_Card_Times				300

#define		KEYA						0x00
#define		KEYB						0x04
#define		KEYSET0						0x00
#define		KEYSET1						0x01
#define		KEYSET2						0x02

#define		CRC_PRESET					0xFFFF
#define		CRC_POLYNOM					0x1021

#define		Snr_Type					0x10
#define		Snr_Len						0x04

#define		Send_KeyWord				0x80
#define		Receive_KeyWord				0x90

//===========================================错误代码表===============================================

#define	ALL					0x52
#define MJXT				1
#define SFXT				2
#define MJDT				4	//电梯系统
#define JSXT				3	//节水系统2006-4-23

#define MJ_Sec				2
#define SF_Sec				1
#define DT_Sec				4
#define JS_Sec				3	//节水系统系统卡使用3区

#define	MI_OK				 0
#define DAS_OK				 0
#define Comm_Err			-1
#define Reader_Err			-2
#define No_SF_Author		-3
#define No_MJ_Author		-4
#define Para_Err			-5
#define TimesOut_Err		-6
#define No_Card				-7
#define No_MJ_SysCard		-8
#define No_XF_SysCard		-9
#define SysCard_Err			-10
#define UserCard_Err		-11
#define ReadCard_Err		-12
#define WriteCard_Err		-13
#define CreateLicence_Err	-14
#define Not_Identify		-15

#define Not_Identify0		-41
#define Not_Identify1		-42
#define Not_Identify2		-43
#define Not_Identify3		-44
#define Not_Identify4		-45

#define Not_Identify11		-51
#define Not_Identify12		-52
#define Not_Identify13		-53
#define Not_Identify14		-54

#define No_Lic_Err			-16
#define No_Key_Err			-17		//找不到加密键文件
#define No_JS_SysCard       -18		//非节水系统卡
#define No_JS_Author		-19		//无节水系统权限
#define CardType_Err		-20		//初始化系统卡时，卡类型不是M1卡
#define NO_CpuCard			-21		
#define Select_File_Err		-22		
#define GetChallenge_Err	-23		
#define External_Authen_Err	-24		
#define UpDate_Err			-25		
#define Read_Binary_Err		-26		
#define User_Card			-27		
#define Blank_Card			-28		
#define Make_UserCard_Err	-29	
#define PassWord_Err		-30	

#define		Rev_Data_Err2				-38					
#define		Write_Err					-61

#define		COMMAND_START_ERR			-70
#define		DEVICE_ADDRESS_ERR			-71
#define		CONTROL_WORD_ERR			-72
#define		CONTROL_FAIL				-73
#define		RT_COMM_ERR					-74
#define		CRC_ERR						-75
#define		COMMAND_END_ERR				-76
#define		REV_LEN_ERR					-77

#define		USB_DATA_ERR1				-81
#define		USB_DATA_ERR2				-82
#define		USB_DATA_ERR3				-83
#define		Read_USB_File_ERR			-84
#define		Write_USB_File_ERR			-85

#define		YC_XF1						0x01
#define		YC_JS12						0x02
#define		YC_JS10						0x03
#define		DTC_XF						0x04

#define		YCXF_G3						0x11
#define		YCXF_G7						0x12

#define		YCXF_G3X					0x21
#define		YCXF_G7X					0x22

#define		YCXF_V1X					0x31
#define		YCXF_V3X					0x32
#define		YCXF_V7X					0x33

#define		YCJS_A2X					0x41

#define		YC_XF1_ERR					0x1000
#define		YC_JS12_ERR					0x2000
#define		YC_JS10_ERR					0x3000
#define		DTC_XF_ERR					0x4000
#define		YC_READ_ERR					0x5000
#define		YC_WRITE_ERR				0x6000
#define		YC_ANALYSIS_ERR				0x7000
#define		AUTOPAY_ERR					0x8000
#define		RESOLVEDATA_ERR				0x9000

unsigned char RF_Rev_Buff[250];
unsigned char RF_Send_Buff[250];

unsigned char access1[3] = {0xff,0x07,0x80};	
unsigned char access2[3] = {0x7f,0x07,0x88};

BYTE  ControlWord[] = {0x7f,0x07,0x88,0x69};
BYTE  ControlWord12[] = {0x7F,0x07,0x88,0xDA};

unsigned char def_keya1[6] = {0xa0,0xa1,0xa2,0xa3,0xa4,0xa5};
unsigned char def_keya2[6] = {0xff,0xff,0xff,0xff,0xff,0xff};	
unsigned char def_keyb1[6] = {0xb0,0xb1,0xb2,0xb3,0xb4,0xb5};
unsigned char def_keyb2[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

unsigned char def_keyA0[6] = {0xa0,0xa1,0xa2,0xa3,0xa4,0xa5};
unsigned char def_keyA1[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char def_keyA3[6] = {0x3C,0x11,0x22,0xcc,0x44,0x3c};

unsigned char def_keyB0[6] = {0xb0,0xb1,0xb2,0xb3,0xb4,0xb5};
unsigned char def_keyB3[6] = {0xAC,0x09,0x87,0x65,0x43,0x25};

unsigned char LockCard_KeyB[6] = {0xdd, 0xea, 0xfc, 0xa0, 0xbc, 0xd1};		//ddeafca0bcd1
unsigned char UserCard1_KeyA[6] = {0xf1, 0xcf, 0xd3, 0xed, 0xf9, 0x58};		//f1cfd3edf958

unsigned char LockCard_KeyB_new[6] = {0x78, 0x28, 0x1f, 0x88, 0x5f, 0x78};
unsigned char SystemCard_KeyA12[6] = {0xCC, 0x11, 0x22, 0x33, 0x44, 0x3C}; 
unsigned char SystemCard_KeyB12[6] = {0xAC, 0x09, 0x87, 0x65, 0x43, 0x21};

unsigned char UP_Warrant_Key1[16] = {0x3C,0x11,0x22,0xcc,0x44,0x3C,0xFF,0x07,0x80,0x69,0xAC,0x09,0x87,0x65,0x43,0x25}; 
unsigned char UP_Warrant_Key2[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	

extern "C"
{
	EXTERN	HANDLE __stdcall OpenComm(int CommPort);
	EXTERN	HANDLE __stdcall rf_init(__int16 port,long baud);

    EXTERN	int __stdcall CloseComm(HANDLE icdev);

    EXTERN	int __stdcall rf_settime(HANDLE icdev,unsigned char *time);
    EXTERN	int __stdcall rf_gettime(HANDLE icdev,unsigned char *time);

    EXTERN	int __stdcall Set_Display(HANDLE icdev, unsigned char Mod, unsigned char *display1);

    EXTERN	int __stdcall rf_halt(HANDLE icdev);
    EXTERN	int __stdcall rf_exit(HANDLE icdev);
    EXTERN	int __stdcall rf_card(HANDLE icdev,unsigned char _Mode,unsigned long *_Snr);
    EXTERN	int __stdcall rf_read(HANDLE icdev,unsigned char _Adr,unsigned char *_Data);
    EXTERN	int __stdcall rf_write(HANDLE icdev,unsigned char _Adr,unsigned char *_Data);
    EXTERN	int __stdcall rf_select(HANDLE icdev,unsigned long _Snr, unsigned char *_Size);
    EXTERN	int __stdcall rf_anticoll(HANDLE icdev, unsigned char _Bcnt, unsigned long *_Snr);
    EXTERN	int __stdcall rf_request(HANDLE icdev, unsigned char _Mode, unsigned __int16 *TagType);
    EXTERN	int __stdcall rf_authentication(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr);
    EXTERN	int __stdcall rf_load_key(HANDLE icdev,unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey);
    EXTERN	int __stdcall rf_load_key_hex(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr,char *_NKey);

    EXTERN	int __stdcall rf_s_t_hex(HANDLE icdev,unsigned char *time);
    EXTERN	int __stdcall rf_g_t_hex(HANDLE icdev,unsigned char *receive_data);	
    EXTERN	int __stdcall Set_Display(HANDLE icdev, unsigned char Mod, unsigned char *display1);

    EXTERN	int __stdcall rf_beep(HANDLE icdev,unsigned short _Msec);

    EXTERN	int __stdcall Init_SysCard12_NewReWrite(HANDLE icdev, int SysType, int UseSector);
    EXTERN	int __stdcall Init_SysCard12_NewCreat(HANDLE icdev,LPCSTR UserPassword,int SysType,int UseSector,LPSTR CommPassword);
    EXTERN	int __stdcall Init_Mobile_SysCard(HANDLE icdev,LPCSTR UserPassword, LPCSTR User_KEYAB, int SysType, int UseSector, unsigned char *User_KEY, LPSTR CommPassword);

    EXTERN	int __stdcall ReadCard_ID(HANDLE icdev, unsigned char *TagType, unsigned long *Snr);
    EXTERN	int __stdcall ReadCard_ID_NEW(HANDLE icdev,unsigned char *TagType,unsigned long *Snr);
    EXTERN	int __stdcall Get_SysUse_Sec(unsigned int *Mjxt_Sec,unsigned int *Sfxt_Sec,unsigned int *Jsxt_Sec,LPSTR CommPassword);

    EXTERN	int __stdcall Query_Card_Type(HANDLE icdev,LPINT SysType,LPINT CardType,unsigned long * CardSerno,unsigned int *OPT_Num ,int WaitTime,unsigned char *user_code_new);
    EXTERN	int __stdcall Query_Pos_UserCard12(HANDLE icdev,LPINT CardType,LPINT OPT_Num,LPINT Serno,LPSTR Cardno,LPINT UserType,unsigned long * CardSerno,LPINT ChkSum1,LPINT Value1,LPINT LastPay1, LPINT Count1,LPINT Consume_Add1,LPINT ChkSum2,LPINT Value2,LPINT LastPay2, LPINT Count2,LPINT Consume_Add2,unsigned long *use_term,LPINT AddCount,int WaitTime,unsigned char * user_code_new);

    EXTERN	int __stdcall RST_Pos_OPTCard12(HANDLE icdev,unsigned int CardSerno,int WaitTime ,unsigned char *user_code_new);
    EXTERN	int __stdcall RST_Pos_UserCard12(HANDLE icdev, unsigned int CardSerno, int WaitTime, unsigned char *user_code_new);

    EXTERN	int __stdcall WRT_UserCard_Term(HANDLE icdev,int Systype,unsigned long useterm,unsigned int CardSerno,int WaitTime);
    EXTERN	int __stdcall WRT_Pos_UserCard12(HANDLE icdev,int Value,unsigned int CardSerno,int WaitTime);
    EXTERN	int __stdcall WRT_Pos_UserCard_AddCount12(HANDLE icdev, int Value, unsigned int CardSerno, int WaitTime);

    EXTERN	int __stdcall Init_Pos_UserCard12(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int WaitTime,unsigned long * CardSerno,unsigned long use_term ,unsigned char * user_code_new);
    EXTERN	int __stdcall Init_Pos_UserCard_N12(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int Value,int UseCount,int WaitTime,unsigned long * CardSerno,unsigned long use_term,unsigned char * user_code_new);
    EXTERN	int __stdcall Init_Pos_OPTCard12(HANDLE icdev, int OPT_Num, int WaitTime, unsigned long * CardSerno, unsigned char *user_code_new);

    EXTERN	int __stdcall Change_Pos_UserType12(HANDLE icdev, int UserType);
    EXTERN	int __stdcall Change_Pos_UserType12_NEW(HANDLE icdev,int * Old_UserType,int UserType ,int Systype);
    EXTERN	int __stdcall Update_UserCard_Period(HANDLE icdev, unsigned long use_term, int Systype, int Number);

    EXTERN	int __stdcall Init_JS_OPTCard12(HANDLE icdev,unsigned long OPT_Num,int WaitTime,unsigned long * CardSerno ,unsigned char *user_code_new);
    EXTERN	int __stdcall Init_Js_UserCard(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,unsigned long * CardSerno ,unsigned char * user_code_new);
	EXTERN	int __stdcall Init_Js_UserCard_New(HANDLE icdev,int newcard_Serno,int oldcard_Serno,LPCSTR Cardno,int UserType,int CardBlance,int ChargeTimes,unsigned long * CardSerno ,unsigned char * user_code_new);
	EXTERN	int __stdcall Init_Js_UserCard_N(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int CardBlance,int ChargeTimes,unsigned long * CardSerno ,unsigned char * user_code_new);
    EXTERN	int __stdcall WRT_Js_UserCard(HANDLE icdev,int Blance,unsigned long CardSerno);
    EXTERN	int __stdcall WRT_Js_UserCard_AddCount(HANDLE icdev,int Blance,char *ChargeDateTime,unsigned long CardSerno);
    EXTERN	int __stdcall InitCollectCard(HANDLE icdev,int OperatorNo, int Mode, unsigned long * CardSerno ,unsigned char * user_code_new);
    EXTERN	int __stdcall InitBlackCard(HANDLE icdev, unsigned char Block, unsigned long * CardSerno, unsigned char * user_code_new);
    EXTERN	int __stdcall WriteBlackCard(HANDLE icdev, unsigned long CardSerno, unsigned char *Black_Data);
    EXTERN	int __stdcall ReadRecNum(HANDLE icdev,unsigned int &RecNum, unsigned long &CardSerno);
    EXTERN	int __stdcall ReadRecNum_NEW(HANDLE icdev, unsigned int *MODE, unsigned long *CardSerno, unsigned int *RecNum);
    EXTERN	int __stdcall ReadAllRec_NEW(HANDLE icdev, RecordAllData *RecAllData, unsigned long CardSerno);
    EXTERN	int __stdcall ReadDetailedRec_NEW(HANDLE icdev, RecordDetailedData *RecDetailedData, unsigned long CardSerno, unsigned long *All_Money , unsigned long *All_Times, unsigned long *All_Amount, unsigned long *Address);
    EXTERN	int __stdcall ReadAllRec(HANDLE icdev,RecordData* RecData,long& RecNum);
    EXTERN	int __stdcall ReadRecbySec(HANDLE icdev,int SecNo,RecordData* RecData,long& RecNum);
    EXTERN	int __stdcall QueryJsCard(HANDLE icdev,LPINT CardType,LPINT OPT_Num,LPINT Serno,LPSTR Cardno,unsigned long * CardSerno,LPINT Value,LPINT Count, LPINT UserType,int WaitTime ,unsigned char * user_code_new);
    EXTERN	int __stdcall RSTJsUserCard(HANDLE icdev,unsigned int CardSerno ,unsigned char *user_code_new);
    EXTERN	int __stdcall ClearRecNum(HANDLE icdev);
    EXTERN	int __stdcall RSTCollectCard(HANDLE icdev,unsigned int CardSerno ,unsigned char *user_code_new);
    EXTERN	int __stdcall RSTJSSubSysCard(HANDLE icdev ,unsigned char *user_code_new);
	EXTERN	int __stdcall MakeJSSubSysCard12_New(HANDLE icdev, JS_FEE_RATE& fee_rate, JS_AdvanPara&  AdvanPara ,int Addr, unsigned char temperature,unsigned char clearrateflag,unsigned char *user_code_new);
	EXTERN	int __stdcall MakeJSSubSysCard12(HANDLE icdev, JS_FEE_RATE& fee_rate, JS_AdvanPara&  AdvanPara ,int Addr, unsigned char *user_code_new);
    EXTERN	int __stdcall MakeJSSubSysCard12_Integrative(HANDLE icdev, JS_FEE_RATE & fee_rate, JS_AdvanPara &  AdvanPara ,unsigned char *user_code_new);
    EXTERN	int __stdcall Set_Js_AddreCard(HANDLE icdev, int Addr ,unsigned char *user_code_new);
    EXTERN	int __stdcall Set_Js_Overdraft_Card(HANDLE icdev, unsigned long Overdraft_Money ,unsigned char *user_code_new);
    EXTERN	int __stdcall Set_Js_DateTimeCard(HANDLE icdev, char *js_date, char *js_time ,unsigned char *user_code_new);
    EXTERN	int __stdcall Set_Js_QueryCard(HANDLE icdev ,unsigned char *user_code_new);
    EXTERN	int __stdcall WRT_UserCard_Subsidize_Time(HANDLE icdev, int Systype, unsigned long useterm, unsigned int CardSerno, int WaitTime);
    EXTERN	int __stdcall Write_UP_Warrant(HANDLE icdev, unsigned char *Warrant_Data, unsigned long CardSerno);
    EXTERN	int __stdcall Read_UP_Warrant(HANDLE icdev, unsigned char *Warrant_Data, unsigned long CardSerno);
    EXTERN	int __stdcall RST_UP_Warrant(HANDLE icdev, unsigned long CardSerno);
    EXTERN	int __stdcall WRT_Pos_Virement(HANDLE icdev, unsigned long Value, unsigned int CardSerno, unsigned long flag, int WaitTime);
    EXTERN	int __stdcall Read_Pos_Balance(HANDLE icdev, unsigned long *Balance_XF,  unsigned long *Balance_JS, unsigned long CardSerno, int WaitTime);
    EXTERN	int __stdcall Make_UserCard_New(HANDLE icdev, unsigned long Value, unsigned long Systype, unsigned long CardSerno, unsigned long WaitTime );
    EXTERN	int __stdcall MAKE_UserCard12_CardName(HANDLE icdev, LPCSTR Cardno , unsigned int CardSerno, int WaitTime);
    EXTERN	int __stdcall Make_UserCard_New091102(HANDLE icdev, unsigned long Value, unsigned long Systype, unsigned long CardSerno, unsigned long *Card_Value1, unsigned long *Card_Value2, unsigned long *Card_Value, unsigned long WaitTime );
    EXTERN	int __stdcall RST_UserCard(HANDLE icdev,unsigned char sector);
}
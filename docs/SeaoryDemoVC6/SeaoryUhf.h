#ifndef _UHFREADER_API_H__
#define _UHFREADER_API_H__

#ifdef UHFREADER_API_EXPORTS
#define UHFREADER_API extern "C" __declspec(dllexport)
#else
#define UHFREADER_API extern "C" __declspec(dllimport)
#endif

//0表示标签RESERVED存储区，1表示EPC数据区，2表示TID数据区，3表示USER数据区
#define UHF_DATA_SECTION_RESERVED 0
#define UHF_DATA_SECTION_EPC 1
#define UHF_DATA_SECTION_TID 2
#define UHF_DATA_SECTION_USER 3


/**********************************************************************************************************
 *                                          UhfReader_API header file
 **********************************************************************************************************/


void WINAPI UhfSetLogWindow(void* wnd);
/**********************************************************************************************************
 *
 *                                         Open port and connect
 *flagCrc取值0，2，4，6，分别对应不同波特率9600/19200/57600/115200
 *********************************************************************************************************/
int WINAPI UhfReaderConnect (HANDLE &hCom, const char* cPort/*示例：COM3，字符串*/, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *										   Disconnect and close port
 *flagCrc这个送0，后边的函数如无特殊说明，此参数均送0
 *********************************************************************************************************/
int WINAPI UhfReaderDisconnect (HANDLE &hCom, UCHAR flagCrc);


/**********************************************************************************************************
 *
 *                                         UHF module Get Version
 *
 *********************************************************************************************************/
//int WINAPI UhfGetVersion (HANDLE hCom, char* uVersion/*返回的版本号，字符串，调用前需要预分配空间*/, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         Get UHF module power setting
 *
 *********************************************************************************************************/
int WINAPI UhfGetPower (HANDLE hCom, char* uPower/*返回功率，字符串，调用前需要预分配空间*/, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         Set UHF module power
 *
 *********************************************************************************************************/
int WINAPI UhfSetPower (HANDLE hCom, char uOption, int uPower, UCHAR flagCrc);


/**********************************************************************************************************
 *
 *                                         Get UHF module frequency setting
 *
 *********************************************************************************************************/
int WINAPI UhfGetFrequency(HANDLE hCom, UCHAR* uFreMode, UCHAR* uFreBase, UCHAR* uBaseFre, UCHAR* uChannNum, UCHAR* uChannSpc, UCHAR* uFreHop, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         Set UHF module frequency
 *
 *********************************************************************************************************/
int WINAPI UhfSetFrequency(HANDLE hCom, UCHAR uFreMode, UCHAR uFreBase, UCHAR* uBaseFre, UCHAR uChannNum, UCHAR uChannSpc, UCHAR uFreHop, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         Read UHF module Uid
 *
 *********************************************************************************************************/
int WINAPI UhfGetReaderUID (HANDLE hCom, UCHAR* uUid, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module inventory
 *
 *********************************************************************************************************/
int WINAPI UhfStartInventory (HANDLE hCom, UCHAR flagAnti, UCHAR initQ, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         Get received data
 *
 *********************************************************************************************************/
//此指令只是启动硬件连续读数据，然后调用者需要自己启动线程，使用UhfRecvData循环读取标签数据。读完后，应该停止读取
int WINAPI UhfReadInventory (HANDLE hCom, UCHAR* uLenUii, UCHAR* uUii);

//UhfRecvData用于用户直接循环读取数据
int/*返回的数据长度*/ WINAPI UhfRecvData(HANDLE hCom, UCHAR* uUii/*返回的数据缓冲*/);
/**********************************************************************************************************
 *
 *                                         UHF module stop get
 *
 *********************************************************************************************************/
int WINAPI UhfStopOperation (HANDLE hCom, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module read data
 *
 *********************************************************************************************************/
//int WINAPI UhfReadDataByEPC (HANDLE hCom, UCHAR* uAccessPwd, UCHAR uBank, UCHAR* uPtr, UCHAR uCnt, UCHAR* uUii, UCHAR* uReadData, UCHAR* uErrorCode, UCHAR flagCrc);
int WINAPI UhfReadDataByEPC (HANDLE hCom, const char* accessPwd/*000000,4字节默认密码，送入字符串*/, 
							 int memBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/, int sa, int dl,UCHAR* uDataReturn/*返回的数据*/, UCHAR flagCrc);

//仅用于读取TID区域的数据，内部会自动选择标签，此函数会返回uDataReturn有效数据长度
int WINAPI UhfReadDataByTID (HANDLE hCom, int sa, int dl,UCHAR* uDataReturn/*硬件返回的原始数据*/,UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);

//功能与UhfReadDataByEPC，唯一区别是读取前，会自动选择指定的标签
int WINAPI UhfReadDataByXZEPC(HANDLE hCom, const char* epcLabelStr/*需要自动选中的标签*/,const char* accessPwd/*000000,4字节默认密码，送入字符串*/, 
							 int memBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/, int sa, int dl,UCHAR* uDataReturn/*返回的数据*/, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module single inventory
 *
 *********************************************************************************************************/
int WINAPI UhfInventorySingleTag (HANDLE hCom, UCHAR* uLenUii, UCHAR* uUii , UCHAR flagCrc);


/**********************************************************************************************************
 *
 *                                         UHF module Add select,?竚?select
 *
 *********************************************************************************************************/
int WINAPI UhfAddFilter (HANDLE hCom, int intSelTarget, int intAction, int intSelMemBank,
						 int intSelPointer, int intMaskLen, int intTruncate, const char* txtSelMask, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module Delete select
 *
 *********************************************************************************************************/
int WINAPI UhfDeleteFilterByIndex (HANDLE hCom, UCHAR SINDEX, UCHAR* STATUS, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module Get select
 *
 *********************************************************************************************************/
int WINAPI UhfStartGetFilterByIndex (HANDLE hCom, UCHAR SINDEX, UCHAR SNUM, UCHAR* STATUS, UCHAR flagCrc);


/**********************************************************************************************************
 *
 *                                         UHF module write data
 *
 *********************************************************************************************************/
//此函数写之前，需要先手工选择标签
int WINAPI UhfWriteDataByEPC(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/, UCHAR uBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/, const char* uPtr/*写内存的起始地址00 00*/, 
			UCHAR uCnt/*写的个数00 03*/, UCHAR* uWriteData/*写入的数据*/, UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);


//这个函数与UhfWriteDataByEPC函数的区别是写数据前，内部会自动做选择设备附近标签操作
int WINAPI UhfWriteDataByEPCEx(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,UCHAR uBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/, const char* uPtr/*写内存的起始地址00 00*/, 
			UCHAR uCnt/*写的个数00 03*/, UCHAR* uWriteData/*写入的数据*/, UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);

//这个函数与UhfWriteDataByEPC函数的区别是写数据前，内部会自动做选择用户指定标签操作
int WINAPI UhfWriteDataByXZEPC(HANDLE hCom, const char* EPCXZ, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/, UCHAR uBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/, const char* uPtr/*写内存的起始地址00 00*/, 
							   UCHAR uCnt/*写的个数00 03*/, UCHAR* uWriteData/*写入的数据*/, UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);

//这个函数与UhfWriteDataByEPC函数的区别是写数据前，内部会自动做选择标签操作，同时固定写入USER区
int WINAPI UhfWriteDataByUSER(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/, const char* uPtr/*写内存的起始地址00 00*/, 
							   UCHAR uCnt/*写的个数00 03*/, UCHAR* uWriteData/*写入的数据*/, UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);


int WINAPI UhfEraseDataByEPC(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,UCHAR uBank/*详见文件开头UHF_DATA_SECTION_XX的定义*/,
			UCHAR* uErrorCode/*返回的错误代码*/, UCHAR flagCrc);


//使用前需要先选中标签
int WINAPI UhfChangeConfig(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,
						   int Config/*只使用了双字节数据*/,UCHAR* uErrorCode/*返回的错误码*/, UCHAR flagCrc);

//4.25.NXP ReadProtect/Reset ReadProtect 指令
int WINAPI UhfSetReadProtect(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,
						   int Config/*1表示启用ReadProtect，0表示取消ReadProtect*/,UCHAR* uErrorCode/*返回的错误码*/, UCHAR flagCrc);

//ChangeEAS
int WINAPI UhfChangeEAS(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,
							 int ConfigPSF/*0x01 代表设置 PSF 位为1，0x00 代表设置 PSF 位为0*/,UCHAR* uErrorCode/*返回的错误码*/, UCHAR flagCrc);

//EAS_Alarm
int WINAPI UhfEASAlarm(HANDLE hCom, UCHAR flagCrc);

int WINAPI UhfChangeQTControl(HANDLE hCom, const char* uAccessPwd/*000000,4字节默认密码，送入字符串*/,
							  int ReadWrite/*0x00: Read，0x01: Write*/,int Persistence/*0x00: 写入标签挥发性存储区，0x01: 写入非挥发性存储区*/,
							  const char*Payload/*双字节的字符串*/, UCHAR* uErrorCode/*返回的错误码*/, UCHAR flagCrc);
/**********************************************************************************************************
 *
 *                                         UHF module lock memory
 *
 *********************************************************************************************************/
//锁定
int WINAPI UhfLockMemByEPC (HANDLE hCom, const char* EPCXZ, const char* uAccessPwd, const char* uLockData, UCHAR* uErrorCode, UCHAR flagCrc);

//解锁
int WINAPI UhfunLockMemByEPC (HANDLE hCom, const char* EPCXZ, const char* uAccessPwd, const char* uLockData, UCHAR* uErrorCode, UCHAR flagCrc);

/**********************************************************************************************************
 *
 *                                         UHF module kill tag
 *
 *********************************************************************************************************/
int WINAPI UhfKillTagByEPC (HANDLE hCom, const char* uKillPwd, UCHAR* uErrorCode, UCHAR flagCrc);

int WINAPI UhfSetMode (HANDLE hCom, byte mode/*0高速识别，1防碰撞群读识别*/, UCHAR flagCrc);//设置读卡模式
int WINAPI UhfSaveConfig (HANDLE hCom, UCHAR flagCrc);//保存配置
int WINAPI UhfSleep (HANDLE hCom,  UCHAR flagCrc);//休眠
int WINAPI UhfAutoFrequeC(HANDLE hCom, int mode/*0取消自动调频，1自动调频*/, UCHAR flagCrc);//设置发射连续载波
int WINAPI UhfSelectQ(HANDLE hCom, int mode/*取值为1到8*/, UCHAR flagCrc);//设置发射连续载波

//ioCtrlType类型
#define UHF_IO_CONTROL_IO_DIR		0
#define UHF_IO_CONTROL_IO_SET_EL	1
#define UHF_IO_CONTROL_IO_GET_EL	2

int WINAPI UhfControIO(HANDLE hCom, int ioCtrlType/*详见上方UHF_IO_CONTROL_的定义*/, int param1/*取值1~4*/,int param2,UCHAR flagCrc);//控制输入输出
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          End
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
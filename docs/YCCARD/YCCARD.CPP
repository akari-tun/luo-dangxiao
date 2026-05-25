#include "stdio.h"
//#include "windows.h"
#include "memory.h"
#include "stdlib.h"
#include "time.h"
#include "assert.h"
#include "iostream.h"
#include "stdafx.h"
#include "yccard.h"

#include "HID/hid.h"

unsigned char Wallet_Format[16] = {0x00, 0x00, 0x00, 0x00, 0xff,0xff, 0xff,0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff};	

HANDLE __stdcall OpenComm(int CommPort)
{
	return((HANDLE)0x500);
}

HANDLE __stdcall OpenCommAuto(int CommPort, int BaudVal)
{
	return((HANDLE)0x500);
}

int __stdcall CloseComm(HANDLE icdev)
{
	return DAS_OK ;
}

int __stdcall Check_Reader(HANDLE icdev)			//检验读写器状态；
{
	return(rf_beep(icdev, 0));
}

//功能: 把BCD码转换为32位的长整数
unsigned long BcdToBin(unsigned char *bcd_buf, unsigned long  bcd_len)
{
	unsigned char  i;
	unsigned char  val;
	unsigned long  rc=0;

	for(i=0; i<bcd_len; i++)
	{
		val = *bcd_buf;
		rc = rc*10 + (val >> 4);
		rc = rc*10 + (val & 0x0f);
		bcd_buf++;
	}
	return(rc);
}

//功能: 把32位的长整数转换为BCD码.
void BinToBcd(unsigned char *bcd_buf, long bin, int bcd_len)
{
	unsigned int  i;
	unsigned int  multiple, remainder;

	bcd_buf += bcd_len - 1;

	for(i=bcd_len; i>0; i--)
	{
		multiple  = bin / 10;
		remainder = bin - (multiple * 10);
		bin = multiple;
		*bcd_buf  = remainder;

		multiple  = bin / 10;
		remainder = bin - (multiple * 10);
		bin = multiple;
		*bcd_buf |= remainder << 4;

		bcd_buf--;
	}
}

void hex_a(unsigned char *hex, LPSTR a,unsigned char length)
{
    int i;
	unsigned char hLowbit,hHighbit;

	for(i=0; i<length*2; i+=2)
	{
		hLowbit  = hex[i/2]%10;
		hHighbit = hex[i/2]/10;

		if(hHighbit>=10)	a[i]=hHighbit+'7';
		else				a[i]=hHighbit+'0';

		if(hLowbit>=10)		a[i+1]=hLowbit+'7';
		else				a[i+1]=hLowbit+'0';
	}
	a[length*2]='\0';
}

__int16 a_hex(unsigned char *a,unsigned char *hex,unsigned char len)
{
	short i;
	unsigned char aLowbit,aHighbit;
	unsigned char hconval,lconval;

	for(i=0;i<len;i+=2)
	{
		aHighbit=toupper(a[i]);
		if((aHighbit>='A')&&(aHighbit<='F'))		hconval='7';
		else if((aHighbit>='0')&&(aHighbit<='9'))	hconval='0';
		else		return -1;

		aLowbit=toupper(a[i+1]);
		if((aLowbit>='A') && (aLowbit<='F'))		lconval='7';
		else if((aLowbit>='0')&&(aLowbit<='9'))		lconval='0';
		else		return -1;

		hex[(i/2)]=((aHighbit-hconval)*10+(aLowbit-lconval));
	}
	hex[len/2]=0x00;
	return 0;
}

void NumberStr2BCD(unsigned char *str,int len)
{
	int i,j = 0;
	int  temp;

	assert(len>0);

	for(i=0;i<len;i++)
	{
		assert(str[i]>='0' && str[i]<='9');
		temp = (str[i]-'0') * 16;
		++i;
		if(i<len) 
		{
			assert(str[i]>='0' && str[i]<='9');
			temp += str[i]-'0';
		}
		str[j++] = temp;
	}
	str[j] = 0; 
}

void hex_a2(unsigned char *hex,unsigned char *a,unsigned char length)
{

    int i;

	for(i=0;i<length;i++)
	{
		sprintf((char *)(a+2*i), "%02x", hex[i]);
	}
	
	return ;
}

int a_hex2(unsigned char *a,unsigned char *hex,unsigned char len)
{
	short i;
	unsigned char uszTmp[3] = {0};
	   
	for(i=0;i<len;i+=2)
	{
		uszTmp[0] = a[i];
		uszTmp[1] = a[i+1];
		hex[i/2] = atoi((char *)uszTmp);
	}

	hex[(len+len%2)/2]=0x00;
	return 0;
}

int a_hex3(char *a, unsigned char *hex, unsigned char len)
{
	unsigned char i, hconval, lconval;

	for(i=0; i<len; i+=2)
	{
		if((a[i] >= 'A')&&(a[i] <= 'F'))			hconval=(10+(a[i]-'A'));
		else if((a[i] >= 'a')&&(a[i] <= 'f'))			hconval=(10+(a[i]-'a'));
		else if((a[i] >= '0')&&(a[i] <= '9'))				hconval=(a[i]-'0');
		else return -1;

		if((a[i+1]>='A') && (a[i+1]<='F'))			lconval=(10+(a[i+1]-'A'));
		else if((a[i+1] >= 'a')&&(a[i+1] <= 'f'))		lconval=(10+(a[i+1]-'a'));
		else if((a[i+1]>='0')&&(a[i+1]<='9'))				lconval=(a[i+1]-'0');
		else return -1;

		hex[(i/2)]=((hconval<<4)|lconval);
	}

	return 0;
}

unsigned  char itoa_h(unsigned  char _byte)
{
	unsigned  char nibble = _byte / 10;

	return ((nibble > 9)? nibble + 'A' - 10 : nibble + '0');
}

unsigned  char itoa_l(unsigned  char _byte)
{
	unsigned  char nibble = _byte % 10;

	return ((nibble > 9)? nibble + 'A' - 10 : nibble + '0');
}

unsigned char ADD8(unsigned char *sbuf, unsigned char data_len)
{
	unsigned  char  sum=0;
	unsigned  char  i;

	for(i=0; i<data_len; i++)		   
	{
		sum = sum + sbuf[i];
	}
	return(~sum);
}

unsigned char CRC8(char  *sbuf, unsigned char data_len)
{
	unsigned  char  sum=0;
	unsigned  char  i;

	for(i=0; i<data_len; i++)		   
	{
		sum = sum ^ sbuf[i];
	}
	return(~sum);
}

unsigned char CRC8_NEW(char  *sbuf, unsigned char data_len)
{
	unsigned  char  sum=0;
	unsigned  char  i;

	for(i=0; i<data_len; i++)		   
	{
		sum = sum ^ sbuf[i];
	}
	return(sum);
}

unsigned char CRC8_ADD(unsigned char *sbuf, unsigned char data_len)
{
	unsigned  char  sum=0;
	unsigned  char  i;

	for(i=0; i<data_len; i++)		   
	{
		sum = sum + sbuf[i];
	}
	return(sum);
}

int  Package(unsigned int addr, unsigned char control, char *sbuf, int sbuf_len, unsigned char *sdata, int sdata_len, char *out_data)
{
	char buff[256];
	int rand1=0, rand2=0, i, LEN = 0, LEN1 = 0;

	union 
	{
		unsigned short Int;
		unsigned char  ch[2];
	}INT_CHAR;

	srand((unsigned)time(NULL)+2348534);
	rand1 = rand();
	rand1 = rand();

	INT_CHAR.Int = COMMAND_START ;
	memcpy(&out_data[LEN1], INT_CHAR.ch, 2);
	LEN1 += 2 ;

	INT_CHAR.Int = addr ;
	memcpy(&out_data[LEN1], INT_CHAR.ch, 2);
	LEN1 += 2 ;

	out_data[LEN1] = control ;
	LEN1 += 1 ;

	LEN1 += 1 ;					//数据长度

	if(sbuf_len)
	{
		memcpy(&out_data[LEN1], sbuf, sbuf_len);
		LEN += sbuf_len;
		LEN1 += sbuf_len ;
	}

	if(sdata_len)
	{
		if((sdata_len+sbuf_len) > MAX_USB_BUF)
		{
			memcpy(&out_data[LEN1], sdata, (MAX_USB_BUF-sbuf_len));
			LEN1 += (MAX_USB_BUF-sbuf_len) ;
		}
		else
		{
			memcpy(&out_data[LEN1], sdata, sdata_len);
			LEN1+= sdata_len;
		}
   		LEN += sdata_len;
	}

	out_data[5] = LEN;

	out_data[LEN1] = CRC8(&out_data[2], LEN1-2);
	LEN1 += 1 ;

	INT_CHAR.Int = COMMAND_END ;
	memcpy(&out_data[LEN1], INT_CHAR.ch, 2);
	LEN1 += 2 ;

	srand((unsigned)time(NULL)+6844390);
	rand2 = rand();
	rand2 = rand();
	rand2 = rand1^rand2;

	memcpy(&buff[2], out_data, LEN1);

	out_data[0] = (unsigned char)(rand1 + 0x05) ;
	out_data[1] = (unsigned char)(rand2 + 0x0a);

	for(i=0; i<LEN1; i++)
	{
		out_data[2+i] = (buff[2+i]^rand1);
		out_data[2+i] = (out_data[2+i]^rand2);
	}

	return(LEN1+2);
}

int  UNPackage(char *in_data, unsigned int addr, unsigned char control, char *out_data, unsigned int *out_data_len)
{
	char buff[256];
	int rand1, rand2, i, LEN = 0, LEN1 = 0;

	union 
	{
		unsigned short Int;
		unsigned char  ch[2];
	}INT_CHAR;

	rand1 = (unsigned char)(in_data[0]-0x05);
	rand2 = (unsigned char)(in_data[1]-0x0a); 

	memcpy(&buff[0], &in_data[2], 6);

	for(i=0; i<6; i++)
	{
		in_data[i] = (buff[i]^rand1);
		in_data[i] = (in_data[i]^rand2);
	}

	memcpy(&buff[6], &in_data[8], in_data[5]+3);	
    
	for(i=6; i<(in_data[5]+9); i++)
	{
		in_data[i] = (buff[i]^rand1);
		in_data[i] = (in_data[i]^rand2);
	}
	
	*out_data_len = 0 ;

	INT_CHAR.ch[0] = in_data[LEN1];
	LEN1 += 1 ;
	INT_CHAR.ch[1] = in_data[LEN1];

	if(INT_CHAR.Int != COMMAND_START)		return(COMMAND_START_ERR);
	LEN1 += 1 ;

	INT_CHAR.ch[0] = in_data[LEN1];
	LEN1 += 1 ;
	INT_CHAR.ch[1] = in_data[LEN1];

	if(INT_CHAR.Int != addr)				return(DEVICE_ADDRESS_ERR);
	LEN1 += 1 ;

	if(in_data[LEN1] != control)			return(CONTROL_WORD_ERR);
	LEN1 += 1 ;

	LEN = in_data[LEN1];
	LEN1 += 1 ;

	if(!LEN)								return(REV_LEN_ERR);
	if(in_data[LEN1])						return((int)in_data[LEN1]);								//判断状态位是否正常

	LEN1 += 1 ;

	if(LEN-1)
	{
		if((LEN-1)<=MAX_USB_BUF)
		{
			memcpy(out_data, &in_data[LEN1], LEN-1);
			*out_data_len = LEN - 1 ;
			LEN1 += (LEN-1) ;
		}
		else
		{
			memcpy(out_data, &in_data[LEN1], MAX_USB_BUF);
			*out_data_len = LEN - 1 ;
			LEN1 += MAX_USB_BUF ;			
		}
	}
	
	if(((unsigned char)in_data[LEN1]) != (CRC8(&in_data[2], LEN1-2)))							return(CRC_ERR);
	LEN1 += 1 ;

	INT_CHAR.ch[0] = in_data[LEN1];
	LEN1 += 1 ;

	INT_CHAR.ch[1] = in_data[LEN1];
	LEN1 += 1 ;

	if(INT_CHAR.Int != COMMAND_END)																return(COMMAND_END_ERR);

	return(DAS_OK);
}

//包头(2B-5AAA) + 地址(2B-FFFF) + 命令字(1B-11) + 数据长度(2B-000F) + 控制字(XB-SET_TIME) + 数据(XB-YYMMDDHHMMSSWW) + 效验(1B-CRC) + 包尾(2B-6BBB)
int  __stdcall rf_settime(HANDLE icdev,unsigned char *time1)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	char  Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	LEN = Package(DEVICE_ADDRESS, CONTROL_SET_TIME_WORD, CONTROL_SET_TIME, strlen(CONTROL_SET_TIME), time1, 7, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_SET_TIME_WORD, NULL, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

//包头(2B-5AAA) + 地址(2B-FFFF) + 命令字(1B-12) + 数据长度(2B-000F) + 控制字(XB-SET_TIME) + 数据(XB-YYMMDDHHMMSSWW) + 效验(1B-CRC) + 包尾(2B-6BBB)
int  __stdcall rf_gettime(HANDLE icdev,unsigned char *time1)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	LEN = Package(DEVICE_ADDRESS, CONTROL_GET_TIME_WORD, CONTROL_GET_TIME, strlen(CONTROL_GET_TIME), NULL, 0, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_GET_TIME_WORD, buff, &REV_LEN);
			if(status)  return(status);

			memcpy(time1, buff, 7);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int  __stdcall Set_Display(HANDLE icdev, unsigned char Mod, unsigned char *display1)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = Mod ;
	memcpy(&buff2[1], display1, 7);

	LEN = Package(DEVICE_ADDRESS, CONTROL_SET_DISPLAY_WORD, CONTROL_SET_DISPLAY, strlen(CONTROL_SET_DISPLAY), buff2, 8, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_SET_DISPLAY_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int __stdcall rf_s_t_hex(HANDLE icdev,char *time)
{
	unsigned char buff[14], buff2[14];

	if(a_hex3(time, buff, 14)!=0)		return Para_Err;

	buff2[0] = buff[0];
	memcpy(&buff2[1], &buff[2], 5);
	buff2[6] = buff[1];

	return rf_settime(icdev,buff2);
}

int __stdcall rf_g_t_hex(HANDLE icdev, char *receive_data)
{	
	int st;
	unsigned char buff[7], buff2[7];

    if(st=rf_gettime(icdev,buff))	return st;

	buff2[0] = buff[0];
	memcpy(&buff2[2], &buff[1], 5);
	buff2[1] = buff[6];

	hex_a(buff2, receive_data, 7);

	return 0;
}

HANDLE __stdcall rf_init(__int16 port, long baud)
{
	return((HANDLE)0x000008);
}

int __stdcall rf_exit(HANDLE icdev)
{
	return(0);
}

int __stdcall rf_beep(HANDLE icdev,unsigned short _Msec)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	union 
	{
		unsigned short Int;
		unsigned char  ch[2];
	}INT_CHAR;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = 0x00 ;
	buff2[1] = 0x01 ;

	INT_CHAR.Int = _Msec;
	memcpy(&buff2[2], &INT_CHAR.ch[0], 2);

	LEN = Package(DEVICE_ADDRESS, CONTROL_SET_BELL_WORD, CONTROL_SET_BELL, strlen(CONTROL_SET_BELL), buff2, 4, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_SET_BELL_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}
int __stdcall rf_open(HANDLE icdev)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	LEN = Package(DEVICE_ADDRESS, CONTROL_OPEN_RF_WORD, CONTROL_OPEN_RF, strlen(CONTROL_OPEN_RF), NULL, 0, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 10);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 10);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_OPEN_RF_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}
int __stdcall rf_request(HANDLE icdev, unsigned char _Mode, unsigned __int16 *TagType)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	union 
	{
		unsigned short Int;
		unsigned char  ch[2];
	}INT_CHAR;

	rf_open(icdev);

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	if(_Mode)
	{
		buff2[0] = 0x52;
	}
	else
	{
		buff2[0] = 0x26;
	}

	LEN = Package(DEVICE_ADDRESS, CONTROL_REQUEST_WORD, CONTROL_REQUEST, strlen(CONTROL_REQUEST), buff2, 1, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_REQUEST_WORD, buff, &REV_LEN);
			if(status)  return(status);

			memcpy(&INT_CHAR.ch[0], buff, 2);

			*TagType = INT_CHAR.Int;

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int __stdcall  rf_anticoll(HANDLE icdev, unsigned char _Bcnt, unsigned long *_Snr)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	union 
	{
		unsigned long  Long;
		unsigned char  ch[4];
	}LONG_CHAR;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = _Bcnt;

	LEN = Package(DEVICE_ADDRESS, CONTROL_ANTICOLL_WORD, CONTROL_ANTICOLL, strlen(CONTROL_ANTICOLL), buff2, 1, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_ANTICOLL_WORD, buff, &REV_LEN);
			if(status)  return(status);

			memcpy(&LONG_CHAR.ch[0], buff, 4);

			*_Snr = LONG_CHAR.Long;

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

__int16 __stdcall rf_decrement_ml(HANDLE icdev,unsigned __int16 _Value)
{
	return(0x00);
}

__int16 __stdcall rf_readval(HANDLE icdev,unsigned char _Adr,unsigned long *_Value)
{
	return(0x00);
}

__int16 __stdcall rf_initval(HANDLE icdev,unsigned char _Adr,unsigned long _Value)
{
	return(0x00);
}

__int16 __stdcall rf_initval_ml(HANDLE icdev,unsigned __int16 _Value)
{
	return(0x00);
}

__int16 __stdcall set_host_485(unsigned char _Mode)
{
	return(0x00);
}

__int16 __stdcall rf_setbright(HANDLE icdev,unsigned char bright)
{
	return(0x00);
}

__int16 __stdcall rf_get_status(HANDLE icdev,unsigned char *_Status)
{
	return(0x00);
}

__int16 __stdcall rf_readval_ml(HANDLE icdev,unsigned __int16 *_Value)
{
	return(0x00);
}

__int16 __stdcall rf_config(HANDLE icdev,unsigned char _Mode,unsigned char _Baud)
{ 
	return 0;
}                                

__int16 __stdcall rf_read_hex(HANDLE icdev,unsigned char _Adr, char *_Data)
{
	int status ;
	unsigned char buff[16];

	status = rf_read(icdev, _Adr, buff);

	hex_a2(buff, (unsigned char *)_Data, 16);

	return 0;	
}                                   

__int16 __stdcall rf_write_hex(HANDLE icdev,unsigned char _Adr,char *_Data)
{
	unsigned char buff[32];

	if(a_hex3(_Data, buff, 32)!=0)	  return Para_Err;

	return(rf_write(icdev, _Adr, buff));	
}

__int16 __stdcall rf_changeb3(HANDLE icdev,unsigned char _SecNr,unsigned char *_KeyA,unsigned char _B0,unsigned char _B1,unsigned char _B2,unsigned char _B3,unsigned char _Bk,unsigned char *_KeyB)
{
	return 0;	
}

__int16 __stdcall rf_reset(HANDLE icdev,unsigned __int16 _Msec)
{
	return 0;	
}

__int16 __stdcall rf_set_control_bit(HANDLE icdev,unsigned char _b)
{
	return 0;	
}

__int16 __stdcall rf_clr_control_bit(HANDLE icdev,unsigned char _b)
{
	return 0;	
}

__int16 __stdcall rf_HL_decrement(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr,unsigned long _Value,unsigned long _Snr,unsigned long *_NValue,unsigned long *_NSnr)
{
	return 0;
}

__int16 __stdcall rf_HL_increment(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr,unsigned long _Value,unsigned long _Snr,unsigned long *_NValue,unsigned long *_NSnr)
{
	return 0;	
}

__int16 __stdcall rf_HL_write(HANDLE icdev,unsigned char _Mode,unsigned char _Adr,unsigned long *_Snr,unsigned char *_Data)
{
	return 0;	
}

__int16 __stdcall rf_HL_writehex(HANDLE icdev,unsigned char _Mode,unsigned char _Adr,unsigned long *_Snr,unsigned char *_Data)
{
	return 0;	
}

__int16 __stdcall rf_HL_read(HANDLE icdev,unsigned char _Mode, unsigned char _Adr,unsigned long _Snr, unsigned char *_Data,unsigned long *_NSnr)
{
	return 0;	
}

__int16 __stdcall rf_HL_readhex(HANDLE icdev,unsigned char _Mode, unsigned char _Adr,unsigned long _Snr, unsigned char *_Data,unsigned long *_NSnr)
{
	return 0;	
}

__int16 __stdcall rf_HL_initval(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr,unsigned long _Value,unsigned long *_Snr)
{
	return 0;	
}

unsigned long __stdcall DES_Encrypt( unsigned char *Key,__int16 KeyLen, unsigned char *Source, unsigned long SrcLen, unsigned char *result)
{
	return 0;	
}

__int16 __stdcall rf_encrypt( char *Key,char *Source, unsigned __int16 SrcLen, char *result)
{
	return 0;	
}

__int16 __stdcall rf_decrypt( char *Key,char *Source, unsigned __int16 SrcLen, char *result)
{
	return 0;	
}

__int16 __stdcall rf_disp0(HANDLE icdev,unsigned char pt_mode,unsigned short digit)
{
	return 0;
}

__int16 __stdcall rf_disp(HANDLE icdev,unsigned char pt_mode,unsigned short digit)
{
	return 0;
}

__int16 __stdcall rf_disp8(HANDLE icdev,__int16 disp_len,unsigned char *disp_str)
{
	return 0;	
}

__int16 __stdcall rf_disp08(HANDLE icdev,__int16 disp_len,unsigned char *disp_str)
{
	return 0;
}

__int16 __stdcall rf_HL_authentication(HANDLE icdev,unsigned char reqmode,unsigned long snr,unsigned char authmode,unsigned char secnr)
{
	return 0;	
}

__int16 __stdcall rf_check_write(HANDLE icdev,unsigned long Snr,unsigned char authmode,unsigned char Adr,unsigned char * _data)
{
	return 0;	
}

__int16 __stdcall rf_check_writehex(HANDLE icdev,unsigned long Snr,unsigned char authmode,unsigned char Adr,unsigned char * _data)
{
	return 0;	
}

__int16 __stdcall srd_alleeprom(HANDLE icdev,__int16 offset,__int16 len,unsigned char *receive_buffer)
{ 
	return 0;
}

__int16 __stdcall swr_alleeprom(HANDLE icdev,__int16 offset,__int16 len,unsigned char* send_buffer)
{ 
	return 0;
}

__int16 __stdcall rf_swr_eeprom(HANDLE icdev,__int16 offset,__int16 lenth,unsigned char* send_buffer)
{
	return 0;	
}

__int16 __stdcall rf_srd_eeprom(HANDLE icdev,__int16 offset,__int16 lenth,unsigned char* receive_buffer)
{
	return 0;	
}

__int16 __stdcall rf_swr_snr(HANDLE icdev,__int16 lenth,unsigned char* send_buffer)
{
	return 0;	
}

__int16 __stdcall rf_srd_snr(HANDLE icdev,__int16 lenth,unsigned char* receive_buffer)
{
	return 0;	
}

__int16 __stdcall rf_val_set(HANDLE icdev,unsigned long *p_Value)
{       
	return 0;
}

__int16 __stdcall rf_val_read(HANDLE icdev,unsigned long *p_Value)
{       
	return 0;
}

__int16 __stdcall rf_val_dec(HANDLE icdev,unsigned long _Value)
{
	return 0;
}

__int16 __stdcall rf_val_inc(HANDLE icdev,unsigned long _Value)
{
	return 0;
}

__int16 __stdcall rf_authentication_2(HANDLE icdev,unsigned char _Mode,unsigned char KeyNr,unsigned char Adr)
{
	return 0;	
}

__int16 __stdcall rf_decrement_transfer(HANDLE icdev,unsigned char Adr,unsigned long _Value)
{
	return 0;	
}

__int16 __stdcall rf_setport(HANDLE icdev,unsigned char _Byte)
{
	return 0;	
}

__int16 __stdcall rf_getport(HANDLE icdev,unsigned char *receive_data)
{
	return 0;	
}

__int16 __stdcall rf_gettimehex(HANDLE icdev,char *receive_data)
{
	return 0;	
}

__int16 __stdcall rf_settimehex(HANDLE icdev,char *time)
{
	return 0;	
}

__int16 __stdcall rf_ctl_mode(HANDLE icdev,unsigned char mode)
{
	return 0;	
}

__int16 __stdcall rf_disp_mode(HANDLE icdev,unsigned char mode)
{
	return 0;	
}

__int16 __stdcall lib_ver(unsigned char *str_ver)
{
	return 0;
}

__int16 __stdcall rf_comm_check(HANDLE icdev,unsigned char _Mode)
{ 
	return 0;
}

__int16 __stdcall set_host_check(unsigned char _Mode)
{
	return 0;
}

__int16 __stdcall set_host_NT(unsigned char _Mode)
{
	return 0;
}

__int16 __stdcall rf_set_BIN(HANDLE icdev,unsigned char _Mode)
{
	return 0;
}

__int16 __stdcall rf_sel_dev(HANDLE icdev,unsigned char reader)
{
	return 0;
}

__int16 __stdcall mf2_protocol(HANDLE icdev,unsigned __int16 timeout,unsigned char slen,char *dbuff)
{
	return 0;
}

__int16 __stdcall rf_sam_rst(HANDLE icdev, unsigned char baud, unsigned char *samack)
{
	return 0;
}

__int16 __stdcall rf_sam_trn(HANDLE icdev, unsigned char *samblock,unsigned char*recv)
{
	return 0;
}

__int16 __stdcall rf_sam_off(HANDLE icdev)
{
	return 0;
}

__int16 __stdcall rf_antenna(HANDLE icdev,unsigned char ant)
{
	return 0;
}

__int16 __stdcall  rf_authen(HANDLE icdev,unsigned char sector,unsigned char keyab,unsigned char defaltAdr,unsigned char *defaltkey)
{
	return 0;
}

int __stdcall rf_select(HANDLE icdev,unsigned long _Snr, unsigned char *_Size)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	union 
	{
		unsigned long  Long;
		unsigned char  ch[4];
	}LONG_CHAR;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	memset(buff2, 0, sizeof(buff2));
	LONG_CHAR.Long = _Snr ;
	memcpy(buff2, &LONG_CHAR.ch[0], 4);

	LEN = Package(DEVICE_ADDRESS, CONTROL_SELECT_WORD, CONTROL_SELECT, strlen(CONTROL_SELECT), buff2, 8, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_SELECT_WORD, buff, &REV_LEN);
			if(status)  return(status);

			memcpy(_Size, &buff[REV_LEN-1], 1);
			
			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int __stdcall rf_authentication(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = _Mode;
	buff2[1] = _SecNr;

	LEN = Package(DEVICE_ADDRESS, CONTROL_AUTHENTICATION_WORD, CONTROL_AUTHENTICATION, strlen(CONTROL_AUTHENTICATION), buff2, 2, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_AUTHENTICATION_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int __stdcall rf_halt(HANDLE icdev)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	LEN = Package(DEVICE_ADDRESS, CONTROL_HALT_WORD, CONTROL_HALT, strlen(CONTROL_HALT), NULL, 0, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_HALT_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}

int __stdcall rf_read(HANDLE icdev,unsigned char _Adr,unsigned char *_Data)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[256];
	char  buff[256], Send_Buff[256], Rev_Buff[256];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = _Adr;

	LEN = Package(DEVICE_ADDRESS, CONTROL_READ_WORD, CONTROL_READ, strlen(CONTROL_READ), buff2, 1, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_READ_WORD, buff, &REV_LEN);
			if(status)  return(status);

			memcpy(_Data, buff, 16);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);
}       

int __stdcall rf_i_block(HANDLE icdev, unsigned char *in_Data, unsigned char len1, unsigned char *out_data, unsigned char *rlen)
{
	return 0;	
}  

int __stdcall rf_increment(HANDLE icdev, unsigned char block, unsigned long  Value)
{
	return 0;
} 

int __stdcall rf_decrement(HANDLE icdev, unsigned char block, unsigned long  Value)
{
	return 0;
} 

int __stdcall rf_restore(HANDLE icdev, unsigned char block)
{
	return 0;
} 

int __stdcall rf_transfer(HANDLE icdev,  unsigned char block)
{
	return 0;
} 

int __stdcall rf_write(HANDLE icdev,unsigned char _Adr,unsigned char *_Data)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[100];
	char  buff[100], Send_Buff[100], Rev_Buff[100];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = _Adr;
	memcpy(&buff2[1], _Data, 16);

	LEN = Package(DEVICE_ADDRESS, CONTROL_WRITE_WORD, CONTROL_WRITE, strlen(CONTROL_WRITE), buff2, 17, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_WRITE_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);	
}

//加载密码
int __stdcall rf_load_key(HANDLE icdev,unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey)
{
	CHidCmd io;
	int  status ;
	unsigned int  LEN = 0, REV_LEN;
	unsigned char buff2[100];
	char  buff[100], Send_Buff[100], Rev_Buff[100];
	DWORD Send_Len, Rev_Len;

	if(!io.OpenDevice(USB_VID, USB_PID))  return Comm_Err;

	buff2[0] = _Mode;
	buff2[1] = _SecNr;

	memcpy(&buff2[2], _NKey, 6);

	LEN = Package(DEVICE_ADDRESS, CONTROL_LOAD_KEY_WORD, CONTROL_LOAD_KEY, strlen(CONTROL_LOAD_KEY), buff2, 8, Send_Buff);

	status = io.WriteFile(Send_Buff, LEN, &Send_Len, 500);
	if((status)&&(Send_Len))
	{
		status = io.ReadFile(Rev_Buff, sizeof(Rev_Buff), &Rev_Len, 500);
		if((status)&&(Rev_Len))
		{
			io.CloseDevice();

			status = UNPackage(Rev_Buff, DEVICE_ADDRESS, CONTROL_LOAD_KEY_WORD, buff, &REV_LEN);
			if(status)  return(status);

			return DAS_OK ;
		}
	}

	io.CloseDevice();

	return(Write_Err);	
}

int __stdcall rf_load_key_hex(HANDLE icdev,unsigned char _Mode,unsigned char _SecNr, char *_NKey)
{
	int status;
	unsigned char buff[50];

	if(a_hex3(_NKey, buff, 12) != 0)	  return Para_Err;

	status = rf_load_key(icdev,_Mode,_SecNr,buff);

	return(status);
}

int __stdcall rf_card(HANDLE icdev,unsigned char _Mode,unsigned long *_Snr)
{
	unsigned __int16 tt=0;
	unsigned long snr;
	unsigned char size;
	int st;

    st=rf_request(icdev,_Mode,&tt);
	if(st!=0) return st;

	st=rf_anticoll(icdev,0x00,&snr);
	if(st!=0) return st;

	st=rf_select(icdev,snr,&size);
	if(st!=0) return st;

	memcpy(_Snr,&snr,4);

	return 0;
}

int __stdcall rf_card_2(HANDLE icdev,unsigned char _Mode,unsigned long *_Snr,unsigned __int16 *tt)
{
	unsigned long snr;
	unsigned char i=3, size;
	int st=1;

	while(i--)
	{
		st=rf_request(icdev, _Mode, tt);
		if(st!=0) {rf_halt(icdev); continue;}

		st=rf_anticoll(icdev, 0x00, &snr);
		if(st!=0) {rf_halt(icdev); continue;}

		st=rf_select(icdev, snr, &size);
		if(st!=0) {rf_halt(icdev); continue;}

		memcpy(_Snr,&snr,4);

		return(DAS_OK);
	}

	rf_halt(icdev);

	return(st);
}

int __stdcall ReadCard_ID(HANDLE icdev, unsigned char *TagType, unsigned long *Snr)
{
	unsigned long use_snr;
	unsigned __int16 tt;

	if(rf_card_2(icdev, 0x52, &use_snr, &tt) == MI_OK) 
	{
        *TagType = (unsigned char)tt;
		*Snr = use_snr;
		return DAS_OK;
	} 

	if(rf_halt(icdev)) return Reader_Err;

	return (No_Card);
}

int __stdcall ReadCard_ID_NEW(HANDLE icdev, unsigned char *TagType, unsigned long *Snr)
{
	unsigned long use_snr;
	unsigned __int16 tt;

	if(rf_card_2(icdev, 0x52, &use_snr, &tt) == MI_OK) 
	{
        *TagType = (unsigned char)tt;
		*Snr = use_snr;
		return DAS_OK;
	} 

	if(rf_halt(icdev)) return Reader_Err;

	return (No_Card);
}

char  InitialTr[64] ={
                 58,50,42,34,26,18,10,02,
                 60,52,44,36,28,20,12,04,
                 62,54,46,38,30,22,14,06,
                 64,56,48,40,32,24,16,0x08,
                 57,49,41,33,25,17,0x09,01,
                 59,51,43,35,27,19,11,03,
                 61,53,45,37,29,21,13,05,
                 63,55,47,39,31,23,15,07 };

char FinalTr[64] ={ 
                 40,0x08,48,16,56,24,64,32,
                 39,07,47,15,55,23,63,31,
                 38,06,46,14,54,22,62,30,
                 37,05,45,13,53,21,61,29,
                 36,04,44,12,52,20,60,28,
                 35,03,43,11,51,19,59,27,
                 34,02,42,10,50,18,58,26,
                 33,01,41,0x09,49,17,57,25 };

char Swap[64] = {
                 33,34,35,36,37,38,39,40,
                 41,42,43,44,45,46,47,48,
                 49,50,51,52,53,54,55,56,
                 57,58,59,60,61,62,63,64,
                 01,02,03,04,05,06,07,0x08,
                 0x09,10,11,12,13,14,15,16,
                 17,18,19,20,21,22,23,24,
                 25,26,27,28,29,30,31,32 };

char KeyTr1[64] = {
                 57,49,41,33,25,17,0x09,
                 01,58,50,42,34,26,18,
                 10,02,59,51,43,35,27,
                 19,11,03,60,52,44,36,

                 63,55,47,39,31,23,15,
                 07,62,54,46,38,30,22,
                 14,06,61,53,45,37,29,
                 21,13,05,28,20,12,04,

                 00,00,00,00,00,00,00,00 };

char KeyTr2[64] = {
                 14,17,11,24,01,05,
                 03,28,15,06,21,10,
                 23,19,12,04,26,0x08,
                 16,07,27,20,13,02,
                 41,52,31,37,47,55,
                 30,40,51,45,33,48,
                 44,49,39,56,34,53,
                 46,42,50,36,29,32,

                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00 };

char etr[64]= {
                 32,01,02,03,04,05,
                 04,05,06,07,0x08,0x09,
                 0x08,0x09,10,11,12,13,
                 12,13,14,15,16,17,
                 16,17,18,19,20,21,
                 20,21,22,23,24,25,
                 24,25,26,27,28,29,
                 28,29,30,31,32,01,

                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
};

char ptr[64] = {
                 16,07,20,21,
                 29,12,28,17,
                 01,15,23,26,
                 05,18,31,10,
                 02,0x08,24,14,
                 32,27,03,0x09,
                 19,13,30,06,
                 22,11,04,25,

                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
                 00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00 };

char s[8][64] = 
{
					{14,04,13,01,02,15,11,0x08,03,10,06,12,05,0x09,00,07,
                     00,15,07,04,14,02,13,01,10,06,12,11,0x09,05,03,0x08,
                     01,01,14,0x08,13,06,02,11,15,12,0x09,07,03,10,05,00,
                     15,12,0x08,02,04,0x09,01,07,05,11,03,14,10,00,06,13
					},
					{15,01,0x08,14,06,11,03,04,0x09,07,02,13,12,00,05,10,
                     03,13,04,07,15,02,0x08,15,12,00,01,10,06,0x09,11,05,
                     00,14,17,11,10,04,13,01,05,0x08,12,06,0x09,03,02,15,
                     13,0x08,10,01,03,15,04,02,11,06,07,12,00,05,14,0x09
					},
					{10,00,0x09,14,06,03,15,05,01,13,12,07,11,04,02,0x08,
                     13,07,00,0x09,03,04,06,10,02,0x08,05,14,12,11,15,01,
                     13,06,04,0x09,0x08,15,03,00,11,01,02,12,05,10,14,07,
                     01,10,13,00,06,0x09,0x08,07,04,15,14,03,11,05,02,12
					},
					{07,13,14,03,00,06,0x09,10,01,02,0x08,05,11,12,04,15,
                     13,0x08,11,05,06,15,00,03,04,07,02,12,01,10,14,0x09,
                     10,06,0x09,00,12,11,07,13,15,01,03,14,05,02,0x08,04,
                     03,15,00,06,10,10,13,0x08,0x09,04,05,11,12,07,02,14
					},
					{02,12,04,01,07,10,11,06,0x08,05,03,15,13,00,14,0x09,
                     14,11,02,12,04,07,13,01,05,00,15,10,03,0x09,0x08,06,
                     04,02,01,11,10,13,07,0x08,15,0x09,12,05,06,03,00,14,
                     11,0x08,12,07,01,14,02,13,06,15,00,0x09,10,04,05,03
					},
					{12,01,10,15,0x09,02,06,0x08,00,13,03,04,14,07,05,11,
                     10,15,04,02,07,12,0x09,05,06,01,13,14,00,11,03,0x08,
                     0x09,14,15,05,02,0x08,12,03,07,00,04,10,01,13,11,06,
                     04,03,02,12,0x09,05,15,10,11,14,01,07,06,00,0x08,13
					},
					{04,11,02,14,15,00,0x08,13,03,12,0x09,07,05,10,06,01,
                     13,00,11,07,04,0x09,01,10,14,03,05,12,02,15,0x08,06,
                     01,04,11,13,12,03,07,14,10,15,06,0x08,00,05,0x09,02,
                     06,11,13,0x08,01,04,10,07,0x09,05,00,15,14,02,03,12
					},
					{13,02,0x08,04,06,15,11,01,10,0x09,03,14,05,00,12,07,
                     01,15,13,0x08,10,03,07,04,12,05,06,11,00,14,0x09,02,
                     07,11,04,01,0x09,12,14,02,00,06,10,13,15,03,05,0x08,
                     02,01,14,07,04,10,0x08,13,15,12,0x09,00,03,05,06,11
					}
};

char  rots[16] ={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,2};

char  CKey[16][64];

unsigned char xtoa_h(unsigned char _byte)
{
  unsigned char nibble = _byte >> 4;

  return ((nibble > 9)? nibble + 'A' - 10 : nibble + '0');
}

unsigned char xtoa_l(unsigned char _byte)
{
  unsigned char nibble = _byte & 0x0F;

  return ((nibble > 9)? nibble + 'A' - 10 : nibble + '0');
}

unsigned char atox(char * _char)
{	
	unsigned char byte_h =toupper(_char[0]);
	unsigned char byte_l =toupper(_char[1]);

	if(byte_h > '9') byte_h -= 0x07;	
	byte_h = (byte_h - '0') << 4;	
	
	if(byte_l > '9') byte_l -= 0x07;	
	byte_l -= '0';	
	
	return (byte_h + byte_l);
}

void TransPose(char * data,char * t,int n)
{
	char x[64];
    int i;

	for(i=0;i<64;i++)
	    x[i]=data[i];

    for(i=0;i<n;i++)
         data[i]=x[t[i]-1];
};

void Rotate(char * key)
{
	int i;
    char x[64];

	for(i=0;i<56;i++)
		x[i]=key[i];

    for(i=0;i<55;i++)    
		x[i]=x[i+1];

	x[27]=key[1];		
	x[55]=key[28];
    
	for(i=0;i<56;i++)
		key[i]=x[i];
};

void SetKey(char *key)
{
	char ikey[64];
	int i,j;

	for(i=0;i<64;i++)
		ikey[i]=key[i];
	
    for(i=0;i<16;i++)
    {
         for(j=0;j<rots[i];j++) 
             Rotate(ikey);

         memcpy(&CKey[i][0],ikey,64);
         TransPose(&CKey[i][0],KeyTr2,48);
    }

}

void f(int i,char *a,char * x)
{
    char ch,e[64],ikey[64],y[64];
    int r, k, j;

    memcpy(e,a,64);
    TransPose(e,etr,48);
    memcpy(ikey,&CKey[i][0],64);
    for(j=0;j<48;j++)
        if(e[j]+ikey[j]==1) y[j]=1;
        else y[j]=0;

    for(k=0;k<8;k++)
	{ 
		r=32*y[6*k+5]+16*y[6*k]  + 8*y[6*k+4]+4*y[6*k+3]+2*y[6*k+2]+y[6*k+1];
		
		ch =s[k][r];
        if(ch & 8) x[4*k]=1;
        else x[4*k]=0;
        if(ch & 4) x[4*k+1]=1;
        else x[4*k+1]=0;
        if(ch & 2) x[4*k+2]=1;
        else x[4*k+2]=0;
        if(ch & 1) x[4*k+3]=1;
        else x[4*k+3]=0;
	}
    TransPose(x,ptr,32);

}

void _des(char *plaintext,char *key,char *ciphertext)
{
	int i,j;
	char a[64],b[64],x[64],_key[64];

	memcpy(_key,key,64);
    memcpy(a,plaintext,64);
    TransPose(a,InitialTr,64);
    TransPose(_key,KeyTr1,56);
    SetKey(_key);
    for(i=0;i<16;i++)
	{
		memcpy(b,a,64);
        for(j=0;j<32;j++) 
             a[j]=b[j+32];

        f(i,a,x);
        for(j=0;j<32;j++)
            if(b[j]+x[j]==1) a[j+32]=1;
            else a[j+32]=0;
	}
    TransPose(a,Swap,64);
    TransPose(a,FinalTr,64);
    memcpy(ciphertext,a,64);
}

void _undes(char *ciphertext,char *key,char *plaintext)
{
	int i,j;
	char a[64],b[64],x[64];

    memcpy(a,ciphertext,64);
    TransPose(a,InitialTr,64);
    TransPose(key,KeyTr1,56);
    SetKey(key);
    for(i=0;i<16;i++)
	{
		memcpy(b,a,64);
        for(j=0;j<32;j++) 
             a[j]=b[j+32];

        f(15-i,a,x);
        for(j=0;j<32;j++)
            if(b[j]+x[j]==1) a[j+32]=1;
            else a[j+32]=0;
	}
    TransPose(a,Swap,64);
    TransPose(a,FinalTr,64);
    memcpy(plaintext,a,64);
}

void PackingT(unsigned char *s,unsigned char *d)
{
	int i,j;
	unsigned char ch,k;

	for(i=0;i<8;i++)
	{
		k = 0x80;ch=0;
		for(j=0;j<8;j++)
		{
			if(s[i*8+j]) ch |= (k>>j);
			else ch &=~(k>>j);
		}
		d[i] = ch;
	}
}

void ExpandT(unsigned char *s,unsigned char *d)
{

	int i,j;
	unsigned char ch,k;

	for(i=0;i<8;i++)
	{
		ch=s[i];
		k = 0x80;
		for(j=0;j<8;j++)
		{
			if 	(ch & (k>>j)) d[i*8+j] = 1;
			else d[i*8+j] = 0;
		}
	}
}

int __stdcall Des(unsigned char *plaintext,unsigned char *key, unsigned char *ciphertext)
{
	unsigned char _pt[64],_ct[64],_key[64];	

	ExpandT(plaintext,_pt);
	ExpandT(key,_key);
	_des((char*)_pt,(char*)_key,(char*)_ct);
	PackingT(_ct,ciphertext);
	return DAS_OK;
}

int __stdcall UnDes(unsigned char *ciphertext,unsigned char *key,unsigned char *plaintext)
{
	unsigned char _pt[64],_ct[64],_key[64];	

	ExpandT(ciphertext,_ct);
	ExpandT(key,_key);
	_undes((char*)_ct,(char*)_key,(char*)_pt);
	PackingT(_pt,plaintext);
	return DAS_OK;
}

int hex_ascii(unsigned char *pStr, unsigned char *p, unsigned char pLen)
{
	int   i=0;  

	if(pLen > 0)   
	{
		for(i=0; i < pLen; i++)   
		{ 
			if(((pStr[i]>>4)&0x0f)<0x0A)
			{
				p[2*i] = ((pStr[i]>>4)&0x0f)+'0';  
			}
			else
			{
				p[2*i] = ((pStr[i]>>4)&0x0f)+'A'-10;  
			}

			if(((pStr[i])&0x0f)<0x0A)
			{
				p[2*i+1] = ((pStr[i])&0x0f)+'0';  
			}
			else
			{
				p[2*i+1] = ((pStr[i])&0x0f)+'A'-10;  
			}
		} 
	}  
	p[2*pLen] = '\0';
	return   i*2;   
}

void Set_Crc(unsigned char * data,int len)
{
	unsigned short crc;
	int i,j;

	crc = CRC_PRESET;
	for(i = 0; i < len; i++)
	{
		crc ^= (unsigned short) data[i] << 8;
		for(j=0; j<8; j++)
		{
			if(crc & 0x8000)
				crc = (crc << 1) ^ CRC_POLYNOM;
			else
				crc = (crc << 1);
		}
	}
	data[len]   = (unsigned char)(crc & 0xff);
	data[len+1] = (unsigned char)(crc >> 8);
}

int Check_Crc(unsigned char *data,int len)
{
	unsigned short crc,ii;

	int i,j;

	crc = CRC_PRESET;
	for(i = 0; i < len; i++)
	{
		crc ^= (unsigned short) data[i] << 8;
		for(j=0; j<8; j++)
		{
			if(crc & 0x8000)
				crc = (crc << 1) ^ CRC_POLYNOM;
			else
				crc = (crc << 1);
		}
	}
	ii = ((unsigned short )data[len+1] << 8) +data[len];
	if(ii != crc) return (-1);
	return (0);
}

int Check_Bcc(unsigned char * data)
{
	int i;
	unsigned char ch = 0;

	for(i=0;i<15;i++)
	{
		ch += data[i];
	}
	if(ch != data[15]) return (-1);
	return (0);
}

int FileExists(const char *filename)
{
	FILE *fp;
	if((fp=fopen(filename,"r")) == NULL) return 0;
	fclose(fp);
	return 1;
}

//读一卡通LICENCECARD文件
int Get_Lic_New(unsigned char  * lic)
{
	FILE *fp;	
	int i;

	unsigned char key1[]="D199902A";
	unsigned char key12[]="20060110";

	if(FileExists(Author_FileNameNew)) 
	{
		if((fp=fopen(Author_FileNameNew,"r+b")) == NULL) return (-1);
		i = fread(&LIC_DATA,1024,1,fp);
		fclose(fp);
		if(i != 1) return (-3); 
		if(Check_Crc(&LIC_DATA.ch[0],1022) !=0) return (-4);		
		for(i=0;i<15;i++) 
		{
			if(UnDes(&LIC_DATA.ch[i*8+0x308],key12,&lic[i*8]))return-1;
		}
		return (0);
	}
	return (-2);
}

//新建一卡通LICENCECARD文件
int Create_Lic_New(unsigned char * Lic)
{
	FILE *fp;	
	int i;
	unsigned char ch[120]; 
	unsigned char key1[]="D199902A";
	unsigned char key12[]="20060110";

	for(i=0;i<15;i++) 
	{
	   if(Des(&Lic[i*8],key12,&ch[i*8]))return CreateLicence_Err;
	}

	srand((unsigned)time(NULL));

	for(i=0;i<512;i++)
	{	  
		LIC_DATA.i[i] = rand();
	}

	for(i=0;i<sizeof(_sys_infonew);i++)
	{
		LIC_DATA.ch[i+0x308] = ch[i];
	}

	Set_Crc(&LIC_DATA.ch[0],1022);
	
	if((fp=fopen(Author_FileNameNew,"w+b")) == NULL) return CreateLicence_Err;

	i = fwrite(&LIC_DATA,sizeof(LIC_DATA),1,fp);

	fclose(fp);

	if(i != 1) return CreateLicence_Err;

	return 0;
}

//修改一卡通LICENCECARD文件
int ReCreate_Lic_New(unsigned char * Lic)
{
	FILE *fp;	
	int i;
	unsigned char ch[120]; 
	unsigned char key1[]="D199902A";
	unsigned char key12[]="20060110";

	for(i=0;i<15;i++) 
	{
	   if(Des(&Lic[i*8],key12,&ch[i*8]))return CreateLicence_Err;
	}

	srand((unsigned)time(NULL));

	for(i=0;i<512;i++)
	{	  
		LIC_DATA.i[i] = rand();
	}

	for(i=0;i<sizeof(_sys_infonew);i++)
	{
		LIC_DATA.ch[i+0x308] = ch[i];
	}
	Set_Crc(&LIC_DATA.ch[0],1022);
	
	if((fp=fopen(Author_FileNameNew,"w+b")) == NULL) return CreateLicence_Err;
	i = fwrite(&LIC_DATA,sizeof(LIC_DATA),1,fp);

	fclose(fp);

	if(i != 1) return CreateLicence_Err;

	return 0;
}

int Create_Lic(unsigned char * Lic)
{
	FILE *fp;	
	int i;
	unsigned char ch[65];
	unsigned char key1[]="D199902A";

	for (i=0;i<8;i++)
	{
	   if(Des(&Lic[i*8],key1,&ch[i*8]))return CreateLicence_Err;
	}

	srand((unsigned)time(NULL));
	for (i=0;i<512;i++)
	{	  
		LIC_DATA.i[i] = rand();
	}
	for (i=0;i<sizeof(_sys_info);i++)
	{
		LIC_DATA.ch[i+0x308] = ch[i];
	}
	Set_Crc(&LIC_DATA.ch[0],1022);
	
	if ((fp=fopen(Author_FileName,"w+b")) == NULL) return CreateLicence_Err;
	i = fwrite(&LIC_DATA,sizeof(LIC_DATA),1,fp);
	fclose(fp);
	if (i != 1) return CreateLicence_Err;
	return 0;
}

int Get_Lic(unsigned char  * lic)
{
	FILE *fp;	
	int i;

	unsigned char key1[]="D199902A";		

	if (FileExists(Author_FileName)) 
	{
		if ((fp=fopen(Author_FileName,"r+b")) == NULL) return (-1);
		i = fread(&LIC_DATA,1024,1,fp);
		fclose(fp);
		if (i != 1) return (-3); 
		if (Check_Crc(&LIC_DATA.ch[0],1022) !=0) return (-4);		
		for (i=0;i<8;i++)
		{
			if(UnDes(&LIC_DATA.ch[i*8+0x308],key1,&lic[i*8]))return-1;
		}
		return (0);
	}
	return (-2);
}

void Cala_Key12(unsigned char *snr,unsigned char *password,unsigned char * KeyA)
{
	KeyA[0]	= password[2] + snr[0] + 0x33;
	KeyA[1]	= password[1] - snr[1] + 0x24;
	KeyA[2]	= password[0] | snr[2] + 0x35;
	KeyA[3]	= password[3] & snr[3] + 0x46;
	KeyA[4]	= password[4] ^ (snr[0] + snr[1] + snr[2] + snr[3]) + 0x57;
	KeyA[5]	= password[5] ^  snr[0] ^ snr[1] ^ snr[2] ^ snr[3] + 0x68;
}

void Cala_JS_Key20(unsigned char *snr,unsigned char *password,unsigned char * KeyA)
{
	KeyA[0]	= password[0] + snr[0] ;
	KeyA[1]	= password[1] + snr[1] ;
	KeyA[2]	= password[2] | snr[2] ;
	KeyA[3]	= password[3] ^ snr[3] ;
	KeyA[4]	= password[4] ^ snr[0] + 0x18;
	KeyA[5]	= password[5] & snr[3] + 0x40;
}
	
void  cala_key12_new(unsigned char *snr_new ,unsigned char *pass_code_new ,unsigned char *pass_code_old )
{
	pass_code_new[0]	= pass_code_old[2] + snr_new[0] + 0x11;                        	//加密过程：先判断是否是空白卡，效验KEYA，然后启用加密，KEYB固定
	pass_code_new[1]	= pass_code_old[1] - snr_new[1] + 0x22;
	pass_code_new[2]	= pass_code_old[0] | snr_new[2] + 0x33;
	pass_code_new[3]	= pass_code_old[3] & snr_new[3] + 0x44;
	pass_code_new[4]	= pass_code_old[4] ^ snr_new[2] + pass_code_old[6] + 0x55;
	pass_code_new[5]	= pass_code_old[5] ^ snr_new[1] + pass_code_old[7] + 0x66;

	pass_code_new[6]	= 0xff;     													//密码区只有KEYB才可以改写块三
	pass_code_new[7]	= 0x07;     
	pass_code_new[8]	= 0x80;
	pass_code_new[9]	= 0x69;

	pass_code_new[10]	= 0x78;															//78281f885f78  
	pass_code_new[11]	= 0x28;
	pass_code_new[12]	= 0x1f;
	pass_code_new[13]	= 0x88;
	pass_code_new[14]	= 0x5f;
	pass_code_new[15]	= 0x78;
}

int __stdcall  Query_Card_Type(HANDLE icdev,LPINT SysType,LPINT CardType,unsigned long * CardSerno,unsigned int *OPT_Num ,int WaitTime,unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16],data2[16] ;
	unsigned char init_type[4] = {0x55,0x55,0x55,0x55};
	unsigned char jiami_type[4] = {0x88,0x88,0x88,0x88};
	unsigned char chaxun_type[4] = {0x88,0x88,0x88,0x88};
	unsigned char time_type[4] = {0x88,0x88,0x88,0x88};
	unsigned char jihao_type[4] = {0x77,0x77,0x77,0x77};
	unsigned int  usercard_sec;
	unsigned __int16 tt;

	unsigned char code_word[16];

	union ic1
	{
		unsigned long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch))	return No_Lic_Err;

	if(SInew.mjxt) Mjxt_author = 1; 
	if(SInew.xfxt) Sfxt_author = 1; 
	if(SInew.jsxt) Jsxt_author = 1; 

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;
	cala_key12_new(IC1.ch,code_word,user_code_new);
	
	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{

//=====================================用户卡=========================================

		if(Sfxt_author==1)
		{
			usercard_sec = SInew.xfxt_sec;	
			
			if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

			if(rf_halt(icdev)) return Reader_Err;

			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
					
			if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) == MI_OK)
			{			
				if(rf_read(icdev,usercard_sec*4+0,data1))return ReadCard_Err;
				if(rf_read(icdev,usercard_sec*4+1,data2))return ReadCard_Err;
				if(rf_halt(icdev)!=0)return Reader_Err;
				*CardType = 0;		
                *SysType  = 2;
				*CardSerno = use_snr;  
				return (DAS_OK);
			}
		}
		
		if(Jsxt_author==1)
		{
			usercard_sec = SInew.jsxt_sec;
			if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
			if(rf_halt(icdev)) return Reader_Err;
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
					
			if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) == MI_OK)
			{
			
				if(rf_read(icdev,usercard_sec*4+0,data1))return ReadCard_Err;
				if(rf_read(icdev,usercard_sec*4+1,data2))return ReadCard_Err;
				if(rf_halt(icdev)!=0)return Reader_Err;
				*CardType = 0;		
                *SysType  = 3;
				*CardSerno = use_snr; 
				return (DAS_OK);
			}
		}

		if(Mjxt_author==1)
		{
			usercard_sec = SInew.mjxt_sec;
			if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
			if(rf_halt(icdev)) return Reader_Err;
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
					
			if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) == MI_OK)
			{
			
				if(rf_read(icdev,usercard_sec*4+0,data1))return ReadCard_Err;
				if(rf_read(icdev,usercard_sec*4+1,data2))return ReadCard_Err;
				if(rf_halt(icdev)!=0)return Reader_Err;
				*CardType = 0;			// user card
                *SysType  = 1;
				*CardSerno = use_snr;  // card id 
				return (DAS_OK);
			}
		} 

//===================================系统卡==================================================

		if(Sfxt_author==1)
		{
			if(rf_load_key(icdev,KEYB|KEYSET2,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
			if(rf_halt(icdev))return Reader_Err;
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
			if(rf_authentication(icdev,KEYB|KEYSET2,1) == MI_OK)
			{
				if(rf_read(icdev,4+0,data1) != MI_OK) return ReadCard_Err;
				if(rf_read(icdev,4+1,data2) != MI_OK) return ReadCard_Err;
				if(rf_halt(icdev))return Reader_Err;

				if(data1[0] == 3)
				{
					*CardType = 2 ;	
					if(memcmp(&data1[1],SInew.syscard_no_xf,5) != 0) return (UserCard_Err);
					*CardSerno = use_snr;
					*SysType  = 2; 
					return (DAS_OK);
				}
			}
		}

		if(Mjxt_author==1)
		{
			if(rf_load_key(icdev,KEYB|KEYSET2,2,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
			if(rf_halt(icdev))return Reader_Err;
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
			if(rf_authentication(icdev,KEYB|KEYSET2,2) == MI_OK)
			{
				if(rf_read(icdev,2*4+0,data1) != MI_OK) return ReadCard_Err;
				if(rf_read(icdev,2*4+1,data2) != MI_OK) return ReadCard_Err;
				if(rf_halt(icdev))return Reader_Err;
				if(data1[0] == 3)
				{
					*CardType = 2 ;			
					if(memcmp(&data1[1],SInew.syscard_no_mj,5) != 0) return (UserCard_Err);
					*CardSerno = use_snr;	
					if(*SysType  == 2)  *SysType  = 12;
					else 	*SysType  = 1;  
					return (DAS_OK);
				}
			}
		}

		if(Jsxt_author==1)
		{
			if(rf_load_key(icdev,KEYB|KEYSET2,3,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
			if(rf_halt(icdev))return Reader_Err;
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
			if(rf_authentication(icdev,KEYB|KEYSET2,3) == MI_OK)
			{
				if(rf_read(icdev,3*4+0,data1) != MI_OK) return ReadCard_Err;
				if(rf_read(icdev,3*4+1,data2) != MI_OK) return ReadCard_Err;
				if(rf_halt(icdev))return Reader_Err;
				if(data1[0] == 3)
				{
					*CardType = 2 ;	
					if(memcmp(&data1[1],SInew.syscard_no_js,5) != 0) return (UserCard_Err);
					*CardSerno = use_snr;	
					if(*SysType == 1)	*SysType  = 13;
					else if(*SysType  ==2 )	*SysType  = 23;
					else  if(*SysType == 12)  *SysType  = 123;
					else	*SysType  = 3;  
					return (DAS_OK);
				}
			}
		}

//===================================功能卡==================================================

		if(rf_load_key(icdev,KEYB|KEYSET2, 1, SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_halt(icdev))return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
		if(rf_authentication(icdev,KEYB|KEYSET2,1) == MI_OK)
		{
			if(rf_read(icdev,4+0,data1) != MI_OK) return ReadCard_Err;
			if(rf_read(icdev,4+1,data2) != MI_OK) return ReadCard_Err;
			if(rf_halt(icdev))return Reader_Err;

			if(data1[0] == 4)
			{
				*CardType = 1 ;				
				*OPT_Num = (int)data2[0];	
				*CardSerno = use_snr;	
				if(memcmp(&data1[1],SInew.syscard_no_xf,5) == 0)
				{
					*SysType  = 1;
					return (DAS_OK);
				}
				else
				{
					if(memcmp(&data1[1],SInew.syscard_no_js,5) == 0)
					{
						*SysType  = 3;
						return (DAS_OK);
					}
					else  return (UserCard_Err);
				}
			}

			if((data1[0] == 0x02)&&(!(memcmp(&data1[1],init_type,4))))
			{
				*CardType = 3 ;					// 初始化卡
				*CardSerno = use_snr;
				*SysType  = 0;
				return (DAS_OK);
			}

			if(data1[0] == 0x0a)
			{
				*CardType = 5 ;					// JS 设置卡 card
				if(memcmp(&data1[1],SInew.syscard_no_js,5) != 0) return (UserCard_Err);
				*CardSerno = use_snr;		
				*SysType  = 0;
				return (DAS_OK);
			}

			if((data1[0] == 0x1A)&&(!(memcmp(&data1[1],jiami_type,4))))
			{
				*CardType = 7 ;					// JS 加密卡 card
				*CardSerno = use_snr;	
				*SysType  = 0;
				return (DAS_OK);	
			}
			if((data1[0] == 0x2A)&&(!(memcmp(&data1[1],chaxun_type,4))))
			{
				*CardType = 8 ;					// JS 查询卡 card
				*CardSerno = use_snr;	
				*SysType  = 0;
				return (DAS_OK);
			}
			if((data1[0] == 0x3A)&&(!(memcmp(&data1[1],jihao_type,4))))
			{
				*CardType = 9 ;					// JS 机号设置卡 card
				*CardSerno = use_snr;		
				*SysType  = 0;
				return (DAS_OK);
			}

			if((data1[0] == 0x4A)&&(!(memcmp(&data1[1],time_type,4))))
			{
				*CardType = 10 ;				// JS 时间设置卡 card
				*CardSerno = use_snr;	
				*SysType  = 0;
				return (DAS_OK);
			}
		}
		
//===================================节能采集卡==================================================

		if(rf_load_key(icdev,KEYB|KEYSET2,0,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_halt(icdev)) return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
		if(rf_authentication(icdev,KEYB|KEYSET2,0) == MI_OK)
		{
			if(rf_read(icdev,1,data1) != MI_OK) return ReadCard_Err;
		    if(rf_halt(icdev))return Reader_Err;
			if(data1[0] == 0x0D)
			{
				*CardType = 6 ;		
				if(memcmp(&data1[1],SInew.syscard_no_js,5) != 0) return (UserCard_Err);
				*CardSerno = use_snr;	
				*SysType  = 0;
				return (DAS_OK);
			}
		}	 
			
		if(rf_halt(icdev))	return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)		return No_Card;	

		IC1.i = use_snr ;

		if(Sfxt_author==1)  
		{
			usercard_sec = SInew.xfxt_sec;
		}
		else if(Jsxt_author==1)
		{
			usercard_sec = SInew.jsxt_sec;
		}
		else if(Mjxt_author==1)
		{
			usercard_sec = SInew.mjxt_sec;
		}

		if(user_code_new[8] == 0x01)					//1为不加密方式		
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			cala_key12_new(IC1.ch,code_word,user_code_new);
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		}
		else
		{
			return(Para_Err);
		}
		
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev, ALL, &use_snr, &tt) != MI_OK) return (No_Card);					
				if(rf_authentication(icdev, KEYB|KEYSET0, usercard_sec) != MI_OK)	return (Not_Identify);
				else	*CardType  = 4;										
			}	
			else 	*CardType  = 4;		
		}
		else 	*CardType  = 4;	
			
		*CardSerno = use_snr;

		if(rf_halt(icdev))return Reader_Err;

		return (DAS_OK);
	}
    return Not_Identify;
}

int __stdcall Query_Pos_UserCard12(HANDLE icdev,LPINT CardType,LPINT OPT_Num,unsigned long * Serno,LPSTR Cardno,LPINT UserType,unsigned long * CardSerno,LPINT ChkSum1,LPINT Value1,LPINT LastPay1, LPINT Count1,LPINT Consume_Add1,LPINT ChkSum2,LPINT Value2,LPINT LastPay2, LPINT Count2,LPINT Consume_Add2,unsigned long *use_term,LPINT AddCount,int WaitTime,unsigned char * user_code_new)
{
	unsigned long temp1, use_snr;
	unsigned char chksum, data1[16], data2[16], data3[16], data4[16], data5[16], data6[16], code_word[16];
	unsigned char init_type[4] = {0x55,0x55,0x55,0x55};
	unsigned int  i,j, usercard_sec;
	unsigned __int16 tt;
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned long  i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
	}

	if(!Sfxt_author)  return (No_SF_Author);

	usercard_sec = SInew.xfxt_sec;	

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	//if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec+1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if((tt ==0x04)||(tt)||(tt == 0x02)) 
	{
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) == MI_OK)				
		{ 
			if(rf_read(icdev,usercard_sec*4+0,data1)) return ReadCard_Err;
			if(rf_read(icdev,usercard_sec*4+1,data2)) return ReadCard_Err;
			if(rf_read(icdev,usercard_sec*4+2,data3)) return ReadCard_Err;

			//if(rf_authentication(icdev,KEYB|KEYSET2,(usercard_sec+1)) == MI_OK)	
			if(1)
			{  
				//if(rf_read(icdev,(usercard_sec+1)*4+0,data4)) return ReadCard_Err;
				//if(rf_read(icdev,(usercard_sec+1)*4+1,data5)) return ReadCard_Err;
				//if(rf_read(icdev,(usercard_sec+1)*4+2,data6)) return ReadCard_Err;
				if(rf_halt(icdev)) return Reader_Err;
						
				IC.i = 0;
				IC.ch[0] = data1[1];
				IC.ch[1] = data1[2];
				IC.ch[2] = data1[0];
				*Serno = IC.i;	
				
				for(j=0;j<5;j++)
				{
					Cardno[j] = data1[j+10];
				}

				*UserType = data1[3];	
				*CardSerno = use_snr;	

				chksum = 0;
				for(i=0;i<15;i++)
				{
					chksum +=data2[i];
				}
				if(chksum == data2[15]) *ChkSum1 = 0;
				else *ChkSum1 = 1;

				chksum = 0;
				for(i=0;i<15;i++)
				{
					chksum +=data3[i];
				}
				if(chksum == data3[15]) *ChkSum2 = 0;
				else *ChkSum2 = 1;

				IC.i = 0 ;
				IC.ch[0] = data2[0];
				IC.ch[1] = data2[1];
				IC.ch[2] = data2[2];
				*Value1 = IC.i;	
				
				IC.i = 0 ;
				IC.ch[0] = data2[3];
				IC.ch[1] = data2[4];
				IC.ch[2] = data2[5];
				*LastPay1 = IC.i;
				
				IC.i = 0; 
				IC.ch[0] = data2[11];
				IC.ch[1] = data2[12];
				*Count1 = IC.i;		
				
				IC.i = 0; 
				IC.ch[0] = data2[13];
				IC.ch[1] = data2[14];
				*Consume_Add1 =IC.i;

				IC.i = 0 ;
				IC.ch[0] = data3[0];
				IC.ch[1] = data3[1];
				IC.ch[2] = data3[2];
				*Value2 = IC.i;		

				IC.i = 0 ;
				IC.ch[0] = data3[3];
				IC.ch[1] = data3[4];
				IC.ch[2] = data3[5];
				*LastPay2 = IC.i;	

				IC.i = 0; 
				IC.ch[0] = data3[11];
				IC.ch[1] = data3[12];
				*Count2 = IC.i;		
				
				IC.i = 0; 
				IC.ch[0] = data3[13];
				IC.ch[1] = data3[14];
				*Consume_Add2 =IC.i;

				temp1 =  ((data1[6]&0xf0)>>4) |((data1[7]&0xe0)>>1) ;
				temp1 =  temp1*10000+((data1[6]&0x0f)*100);

				*use_term = temp1+((data1[7]&0x1f)); 

				*AddCount = data1[9]; 

				//memcpy(&user_code_new[9], data4, 16);
				//memcpy(&user_code_new[25], data5, 16);
                  memset(&user_code_new[9],0,16);
				  memset(&user_code_new[25],0,16);
				return (DAS_OK);
			}
		}

		if(rf_load_key(icdev, KEYB|KEYSET2,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_halt(icdev))return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;

		if(rf_authentication(icdev,KEYB|KEYSET2,1) == MI_OK)
		{
			if(rf_read(icdev,4+0,data1) != MI_OK) return ReadCard_Err;
			if(rf_read(icdev,4+1,data2) != MI_OK) return ReadCard_Err;
			if(rf_halt(icdev))return Reader_Err;
			if((data1[0] == 3)&&(!(memcmp(&data1[1],SInew.syscard_no_xf,5))))
			{
				*CardType = 2 ;		
			}
			else if((data1[0] == 0x02)&&(!(memcmp(&data1[1],init_type,4))))
			{
				*CardType = 3 ;	
			}
			else if((data1[0] == 4)&&(!(memcmp(&data1[1],SInew.syscard_no_xf,5))))
			{
				*CardType = 1 ;				
				*OPT_Num = (int)data2[0];
			}
			else return(UserCard_Err);

			*CardSerno = use_snr;
			
			return (DAS_OK);
		}

		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)	return (No_Card);	

		IC1.i = use_snr ;

		if(user_code_new[8] == 0x01)					//1为不加密方式	
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			cala_key12_new(IC1.ch,code_word,user_code_new);
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		}
		else
		{
			return(Para_Err);
		}
		
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)	return (No_Card);						
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
				if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)	return (No_Card);					
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
				else	*CardType  = 4;										
			}	
			else 	*CardType  = 4;		
		}
		else 	*CardType  = 4;	
			
		*CardSerno = use_snr;

		if(rf_halt(icdev))return Reader_Err;

		return (DAS_OK);
	}
    return Not_Identify;
}

int __stdcall Get_SysType(unsigned int *Mjxt,unsigned int *Sfxt,unsigned int *Jsxt)
{
	 *Mjxt = 0; *Sfxt = 0; *Jsxt = 0;
	if (Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt)   *Mjxt = 1;
		if(SInew.xfxt)   *Sfxt = 1; 
		if(SInew.jsxt)   *Jsxt = 1; 
	}
	else 
		return No_Lic_Err;

	return(DAS_OK);
}

int __stdcall Init_SysCard12_NewCreat(HANDLE icdev,LPCSTR UserPassword,int SysType,int UseSector,LPSTR CommPassword)
{
	unsigned long snr;
	unsigned char password[8];
	unsigned char mjdata[16];
	unsigned char sfdata[16];
	unsigned char jsdata[16];
	unsigned char ch[10];
	unsigned char bcc = 0;
	char ch1[32];
	int i, i1;

	unsigned char key1[]="D199902A";
	unsigned char key12[]="20060110";

	if((SysType<1) | (SysType>3) | (UseSector>31) | (UseSector<1))	return (Para_Err);

	memcpy(ch,UserPassword,8);
	Des(ch,key12,password);	

	srand((unsigned)time(NULL));	
	i = rand();
	i1 = rand();
	i += i1*10000;
	
	_itoa(i,ch1,10);
	ch1[8] = 0;

	memcpy(CommPassword,ch1,8);

	if(rf_card(icdev,ALL,&snr) != MI_OK)
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) 	return (No_Card);
	}

	if(rf_load_key(icdev,KEYB|KEYSET0,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,2,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,3,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,4,SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	switch(SysType)
	{
//======================================门禁系统==========================================
	case MJXT:

		if(rf_authentication(icdev, KEYB|KEYSET0, MJ_Sec) !=MI_OK)  return(No_MJ_SysCard);
		if(rf_read(icdev,MJ_Sec*4,mjdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_mj,&mjdata[1],5);  

		if(rf_read(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.comm_password,ch1,8);
		memcpy(SysInfonew.oprater_password,password,8);
		memcpy(SysInfonew.user_password,UserPassword,8);
		memcpy(SysInfonew.sys_keyb,password,8);

		for(i=0;i<5;i++)
		{
			SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_mj[i];
		}

		for(i=0;i<5;i++)
		{
			SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_mj[i];
		}

		SysInfonew.xfxt = 0;
		SysInfonew.xfxt_sec = 0;
		SysInfonew.mjxt_cardtype = 1;
		SysInfonew.jsxt = 0;				
		SysInfonew.jsxt_sec = 0;
		SysInfonew.mjxt = 1;
		SysInfonew.mjxt_sec = (unsigned char)UseSector;

		memcpy(mjdata,SysInfonew.oprater_password,6);

		mjdata[6] = (unsigned char)UseSector;;
		mjdata[7] = 1; 

		for(i=0;i<15;i++)	bcc += mjdata[i];
		mjdata[15] = bcc;

		if(rf_write(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(WriteCard_Err);
		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

		if(Create_Lic_New(SysInfonew.ch) != 0)  return(CreateLicence_Err);
		
		break;

//======================================消费系统==========================================

	case SFXT:

		if(rf_authentication(icdev ,KEYB|KEYSET0,SF_Sec) !=MI_OK) return(No_XF_SysCard);				
		if(rf_read(icdev,SF_Sec*4,sfdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(sfdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_xf,&sfdata[1],5);

		if(rf_read(icdev,SF_Sec*4+1,sfdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(sfdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.comm_password,ch1,8);
		memcpy(SysInfonew.oprater_password,password,8);				
		memcpy(SysInfonew.user_password,UserPassword,8);
		memcpy(SysInfonew.sys_keyb,password,8);

		for(i=0;i<5;i++)
		{
			SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_xf[i];
		}

		for(i=0;i<5;i++)
		{
			SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_xf[i];
		}

		SysInfonew.mjxt = 0;				
		SysInfonew.mjxt_sec = 0;
		SysInfonew.mjxt_cardtype = 0;
		SysInfonew.jsxt = 0;				
		SysInfonew.jsxt_sec = 0;
		SysInfonew.xfxt =1;
		SysInfonew.xfxt_sec = (unsigned char)UseSector;	

		memcpy(sfdata,SysInfonew.oprater_password,6);

		sfdata[6] = unsigned char(UseSector); 
		sfdata[7] = 0;

		for(i=0;i<15;i++)	bcc += sfdata[i];
		sfdata[15] = bcc;

		if(rf_write(icdev,SF_Sec*4+1,sfdata) != MI_OK) return(WriteCard_Err);

		memset(sfdata, 0, sizeof(sfdata));

		bcc = 0 ;
		memcpy(sfdata,SysInfonew.sys_keyb, 8);

		for(i=0;i<15;i++)	bcc += sfdata[i];
		sfdata[15] = bcc;

		if(rf_write(icdev,SF_Sec*4+2,sfdata) != MI_OK) return(WriteCard_Err);

		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

		if(Create_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);	
		
		break;

//======================================节能系统==========================================

	case JSXT:

		if(rf_authentication(icdev ,KEYB|KEYSET0,JS_Sec) !=MI_OK) return(No_XF_SysCard);				
		if(rf_read(icdev,JS_Sec*4,jsdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(jsdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_js,&jsdata[1],5);

		if(rf_read(icdev,JS_Sec*4+1,jsdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(jsdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.comm_password,ch1,8);
		memcpy(SysInfonew.oprater_password,password,8);				
		memcpy(SysInfonew.user_password,UserPassword,8);
		memcpy(SysInfonew.sys_keyb,password,8);

		for(i=0;i<5;i++)
		{
			SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_js[i];
		}

		for(i=0;i<5;i++)
		{
			SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_js[i];
		}

		SysInfonew.mjxt = 0;				
		SysInfonew.mjxt_sec = 0;
		SysInfonew.mjxt_cardtype = 0;
		SysInfonew.xfxt = 0;
		SysInfonew.xfxt_sec = 0;
		SysInfonew.jsxt =1;  
		SysInfonew.jsxt_sec =(unsigned char)UseSector;

		memcpy(jsdata,SysInfonew.oprater_password,6);

		jsdata[6] = (unsigned char)UseSector;
		jsdata[7] = 0;

		for(i=0;i<15;i++)	bcc += jsdata[i];

		jsdata[15] = bcc;

		if(rf_write(icdev, JS_Sec*4+1, jsdata) != MI_OK) return(WriteCard_Err);

		memset(jsdata, 0, sizeof(sfdata));

		bcc = 0 ;
		memcpy(jsdata,SysInfonew.sys_keyb, 8);

		for(i=0;i<15;i++)	bcc += jsdata[i];
		jsdata[15] = bcc;

		if(rf_write(icdev,JS_Sec*4+2,jsdata) != MI_OK) return(WriteCard_Err);

		if(rf_halt(icdev)!=MI_OK) return Reader_Err; 

		if(Create_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);	
		
		break;
			
	default:
		return (Para_Err);
	}

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;  
	}
	else 
	{
		return CreateLicence_Err;
	}
	return(DAS_OK);
}

int __stdcall Init_SysCard12_NewReWrite(HANDLE icdev, int SysType, int UseSector)
{
	unsigned long snr;
	unsigned char mjdata[16];
	unsigned char sfdata[16];
	unsigned char jsdata[16];
	unsigned char bcc = 0;
	int i;

	unsigned char key1[]="D199902A";
	unsigned char key12[]="20060110";

	if((SysType<1) | (SysType>3) | (UseSector>31) | (UseSector<1)) return (Para_Err);

    if(Get_Lic_New(SysInfonew.ch)!=0)  return(No_Lic_Err); 

	if(rf_card(icdev,ALL,&snr) != MI_OK)
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) 	return (No_Card);
	}
		
	if(rf_load_key(icdev,KEYB|KEYSET0,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,2,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,3,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET0,4,SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	switch(SysType)
	{
//======================================门禁系统==========================================
	case MJXT:

		if(rf_authentication(icdev ,KEYB|KEYSET0,MJ_Sec) !=MI_OK) return(No_MJ_SysCard);
		if(rf_read(icdev,MJ_Sec*4,mjdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_mj,&mjdata[1],5); 

		if(rf_read(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

		SysInfonew.mjxt =1;
		SysInfonew.mjxt_sec = unsigned char(UseSector);

		memcpy(mjdata,SysInfonew.oprater_password,6);

		mjdata[6] = unsigned char(UseSector);
		mjdata[7] = SysInfonew.mjxt_cardtype; 

		for(i=0;i<15;i++)		bcc += mjdata[i];
		mjdata[15] = bcc;

		if(rf_write(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(WriteCard_Err);
		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

		if(ReCreate_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);	
		
		break;

//======================================消费系统==========================================

	case SFXT:
	
		if(rf_authentication(icdev ,KEYB|KEYSET0,SF_Sec) !=MI_OK) return(No_XF_SysCard);				
		if(rf_read(icdev,SF_Sec*4,sfdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(sfdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_xf,&sfdata[1],5);

		if(rf_read(icdev,SF_Sec*4+1,sfdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(sfdata) != 0) return (SysCard_Err);

		SysInfonew.xfxt =1;
		SysInfonew.xfxt_sec = unsigned char(UseSector);

		memcpy(sfdata,SysInfonew.oprater_password,6);

		sfdata[6] = unsigned char(SysInfonew.xfxt_sec); 
		sfdata[7] = 0;

		for(i=0;i<15;i++)		bcc += sfdata[i];
		sfdata[15] = bcc;

		if(rf_write(icdev,SF_Sec*4+1,sfdata) != MI_OK) return(WriteCard_Err);

		memset(sfdata, 0, 16);

		bcc = 0 ;
		memcpy(sfdata, SysInfonew.sys_keyb, 8);

		for(i=0;i<15;i++)	bcc += sfdata[i];
		sfdata[15] = bcc;

		if(rf_write(icdev, SF_Sec*4+2, sfdata) != MI_OK) return(WriteCard_Err);

		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

		if(ReCreate_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);

		break;

//======================================节能系统==========================================

	case JSXT:

		if(rf_authentication(icdev ,KEYB|KEYSET0,JS_Sec) !=MI_OK) return(No_XF_SysCard);				
		if(rf_read(icdev,JS_Sec*4,jsdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(jsdata) != 0) return (SysCard_Err);

		memcpy(SysInfonew.syscard_no_js,&jsdata[1],5);

		if(rf_read(icdev,JS_Sec*4+1,jsdata) != MI_OK) return(ReadCard_Err);				
		if(Check_Bcc(jsdata) != 0) return (SysCard_Err);

		SysInfonew.jsxt =1;  
		SysInfonew.jsxt_sec =(unsigned char)UseSector;

		memcpy(jsdata,SysInfonew.oprater_password,6);

		jsdata[6] = (unsigned char)UseSector;
		jsdata[7] = 0;

		for(i=0;i<15;i++)	bcc += jsdata[i];
		jsdata[15] = bcc;

		if(rf_write(icdev,JS_Sec*4+1,jsdata) != MI_OK) return(WriteCard_Err);

		bcc = 0 ;
		memcpy(sfdata, SysInfonew.User_KeyAB, 16);

		for(i=0;i<15;i++)	bcc += sfdata[i];
		sfdata[15] = bcc;

		if(rf_write(icdev, JS_Sec*4+2, sfdata) != MI_OK) return(WriteCard_Err);

		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

		if(ReCreate_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);
		
		break;
				
	default:
		return (Para_Err);
	}

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else
	{
		return CreateLicence_Err;
	}

	return(DAS_OK);
}

int __stdcall Init_Mobile_SysCard(HANDLE icdev, LPCSTR UserPassword, LPCSTR User_KEYAB, int SysType, int UseSector, unsigned char *User_KEY, LPSTR CommPassword)
{
	unsigned long  snr;
	unsigned char  password[8];
	unsigned char  passwordAB[16];
	unsigned char  mjdata[16];
	unsigned char  sfdata[16];
	unsigned char  jsdata[16];
	char  ch1[32];
	unsigned char  ch[10];
	unsigned char  bcc = 0;
	int i, i1;

	unsigned char key1[]="D199902A";
	unsigned char  key12[]="20060110";

	if((SysType<1) | (SysType>3) | (UseSector>31) | (UseSector<1)) return (Para_Err);

	memcpy(ch,UserPassword,8);
	Des(ch,key12,password);		

	memset(ch, 0, sizeof(ch));

	memcpy(ch,User_KEYAB,6);
	Des(ch,key12,passwordAB);
	memcpy(ch,&User_KEYAB[6],6);
	Des(ch,key12,&passwordAB[8]);

	srand((unsigned)time(NULL));
	i = rand();
	i1 = rand();
	i += i1*10000;
	
	_itoa(i,ch1,10);
	ch1[8] = 0;

	memcpy(CommPassword,ch1,8);

	if (rf_card(icdev,ALL,&snr) != MI_OK)
		if (rf_card(icdev,ALL,&snr) != MI_OK) 
			return (No_Card);	

	if (rf_load_key(icdev,KEYB|KEYSET0,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if (rf_load_key(icdev,KEYB|KEYSET0,2,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if (rf_load_key(icdev,KEYB|KEYSET0,3,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	if (rf_load_key(icdev,KEYB|KEYSET0,4,SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	switch (SysType)
	{
		case MJXT://一卡通使用函数
			if(rf_authentication(icdev ,KEYB|KEYSET0,MJ_Sec) !=MI_OK) return(No_MJ_SysCard);
			if(rf_read(icdev,MJ_Sec*4,mjdata) != MI_OK) return(ReadCard_Err);				
			if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

			memcpy(SysInfonew.syscard_no_mj,&mjdata[1],5); 

			if(rf_read(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(ReadCard_Err);				
			if(Check_Bcc(mjdata) != 0) return (SysCard_Err);

			memcpy(SysInfonew.comm_password,ch1,8);
			memcpy(SysInfonew.oprater_password,password,8);
			memcpy(SysInfonew.user_password,UserPassword,8);
			memcpy(SysInfonew.sys_keyb,password,8);

			for(i=0;i<5;i++)
			{
				SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_mj[i];
			}

			for(i=0;i<5;i++)
			{
				SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_mj[i];
			}

			SysInfonew.xfxt = 0;
			SysInfonew.xfxt_sec = 0;
			SysInfonew.mjxt_cardtype = 1;
			SysInfonew.jsxt = 0;				
			SysInfonew.jsxt_sec = 0;
			SysInfonew.mjxt =1;
			SysInfonew.mjxt_sec =(unsigned char )UseSector;

			bcc = 0 ;
			memcpy(mjdata,SysInfonew.oprater_password,6);
			mjdata[6] = (unsigned char )UseSector;;
			mjdata[7] = 1; 
			for(i=0;i<15;i++)	bcc += mjdata[i];
			mjdata[15] = bcc;

			//把系统信息写入系统卡
			if(rf_write(icdev,MJ_Sec*4+1,mjdata) != MI_OK) return(WriteCard_Err);
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

			//生成系统文件 
			if(Create_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);										
			break;

		case SFXT:
			if(rf_authentication(icdev ,KEYB|KEYSET0,SF_Sec) !=MI_OK)   return(No_XF_SysCard);				
			if(rf_read(icdev,SF_Sec*4,sfdata) != MI_OK)   return(ReadCard_Err);				
			if(Check_Bcc(sfdata) != 0)   return (SysCard_Err);

			memcpy(SysInfonew.syscard_no_xf,&sfdata[1],5);

			if (rf_read(icdev,SF_Sec*4+1,sfdata) != MI_OK) return(ReadCard_Err);				
			if (Check_Bcc(sfdata) != 0) return (SysCard_Err);

			memcpy(User_KEY, passwordAB, 16);

			memcpy(SysInfonew.comm_password,ch1,8);
			memcpy(SysInfonew.oprater_password,password,8);	
			memcpy(SysInfonew.user_password,UserPassword,8);
			memcpy(SysInfonew.sys_keyb,password,8);
			memcpy(SysInfonew.User_KeyAB, User_KEYAB, 12);
			memcpy(SysInfonew.User_Return_KeyAB, passwordAB, 16);	

			for(i=0;i<5;i++)
			{
				SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_xf[i];
			}

			for(i=0;i<5;i++)
			{
				SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_xf[i];
			}

			SysInfonew.mjxt = 0;				
			SysInfonew.mjxt_sec = 0;
			SysInfonew.mjxt_cardtype = 0;
			SysInfonew.jsxt = 0;				
			SysInfonew.jsxt_sec = 0;
			SysInfonew.xfxt =1;
			SysInfonew.xfxt_sec = (unsigned char )UseSector;	

			bcc = 0 ;
			memcpy(sfdata, SysInfonew.oprater_password, 6);
			sfdata[6] = unsigned char (UseSector);
			sfdata[7] = 0;
			for(i=0;i<15;i++)	bcc += sfdata[i];
			sfdata[15] = bcc;

			//把系统信息写入系统卡
			if(rf_write(icdev, SF_Sec*4+1, sfdata) != MI_OK) return(WriteCard_Err);

			bcc = 0 ;
			memcpy(sfdata, SysInfonew.sys_keyb, 8);
			for(i=0;i<15;i++)	bcc += sfdata[i];
			sfdata[15] = bcc;

			//把系统信息写入系统卡
			if(rf_write(icdev, SF_Sec*4+2, sfdata) != MI_OK) return(WriteCard_Err);

			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

			//生成系统文件 
			if(Create_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);	    
			
			break;

		case JSXT:
			if (rf_authentication(icdev ,KEYB|KEYSET0,JS_Sec) !=MI_OK) return(No_XF_SysCard);				
			if (rf_read(icdev,JS_Sec*4,jsdata) != MI_OK) return(ReadCard_Err);				
			if (Check_Bcc(jsdata) != 0) return (SysCard_Err);
			memcpy(SysInfonew.syscard_no_js,&jsdata[1],5);
//			if (memcmp(SysInfo.syscard_no,SI.syscard_no,5) !=0) return (SysCard_Err);
			if (rf_read(icdev,JS_Sec*4+1,jsdata) != MI_OK) return(ReadCard_Err);				
			if (Check_Bcc(jsdata) != 0) return (SysCard_Err);

			memcpy(User_KEY, passwordAB, 16);

			memcpy(SysInfonew.comm_password,ch1,8);
			memcpy(SysInfonew.oprater_password,password,8);	
			memcpy(SysInfonew.user_password,UserPassword,8);
			memcpy(SysInfonew.sys_keyb,password,8);
			memcpy(SysInfonew.User_KeyAB, User_KEYAB, 12);
			memcpy(SysInfonew.User_Return_KeyAB, passwordAB, 16);	

			for (i=0;i<5;i++)
			{
				SysInfonew.oprater_password[i] ^= SysInfonew.syscard_no_js[i];
			}

			for (i=0;i<5;i++)
			{
				SysInfonew.sys_keyb[i] += SysInfonew.syscard_no_js[i];
			}

			SysInfonew.mjxt = 0;
			SysInfonew.mjxt_sec = 0;
			SysInfonew.mjxt_cardtype = 0;
			SysInfonew.xfxt = 0;
			SysInfonew.xfxt_sec = 0;				//(uchar)SfSector;
			
			SysInfonew.jsxt = 1;					//060701
			SysInfonew.jsxt_sec =(unsigned char)UseSector;

			bcc = 0 ;
			memcpy(jsdata,SysInfonew.oprater_password,6);
			jsdata[6] = (unsigned char)UseSector;//uchar(SfSector);
			jsdata[7] = 0;
			for (i=0;i<15;i++)	bcc += jsdata[i];
			jsdata[15] = bcc;
			//把系统信息写入系统卡
			if (rf_write(icdev,JS_Sec*4+1,jsdata) != MI_OK) return(WriteCard_Err);

			bcc = 0 ;
			memcpy(sfdata, SysInfonew.User_KeyAB, 12);

//			a_hex3((char*)SysInfonew.User_KeyAB, sfdata, 30);

			for(i=0;i<15;i++)	bcc += sfdata[i];
			sfdata[15] = bcc;

			//把系统信息写入系统卡
			if(rf_write(icdev, JS_Sec*4+2, sfdata) != MI_OK) return(WriteCard_Err);

			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			
			//生成系统文件 
			if (Create_Lic_New(SysInfonew.ch) != 0) return (CreateLicence_Err);								
			break;
			
		default:
			return (Para_Err);
	}

	if (Get_Lic_New(SInew.ch) == 0)
	{
		if (SInew.mjxt) Mjxt_author = 1; 
		if (SInew.xfxt) Sfxt_author = 1; 
		if (SInew.jsxt) Jsxt_author = 1; 
	}
	else 
		return CreateLicence_Err;

	return(DAS_OK);
}

int  __stdcall Init_Pos_UserCard12(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int WaitTime,unsigned long * CardSerno,unsigned long use_term ,unsigned char * user_code_new)//,int use_term
{
	unsigned long snr1,use_snr;
	unsigned char data1[16] ,data2[16],code_word[16];
	unsigned char access1[3] = {0xff,0x07,0x80};	
	unsigned char access2[3] = {0x7f,0x07,0x88};	
	unsigned int  i, j, usercard_sec;		
	unsigned char bcc;	
	unsigned char data3[16];
	unsigned __int16 tt;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
	  unsigned	long i;
		unsigned char ch[4];
	}IC1;

	unsigned char temp1;
	unsigned char temp2,temp3;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else
	{
		return No_Lic_Err;
	}

	if(Sfxt_author!=1) return (No_SF_Author);

    i = strlen(Cardno);

	if((Serno <= 0) | (WaitTime < 0) | (UserType==0) | (UserType>32)) return (Para_Err);

	usercard_sec = SInew.xfxt_sec;   

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec+1,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec+1,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec+1,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec+1,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec+1,LockCard_KeyB) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec+1,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch, code_word, user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,&code_word[10]) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec+1,&code_word[10]) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if((tt == 0x04)||(tt)||(tt == 0x02))		
	{	
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);			
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
				if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	
				{
					if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
					if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
					if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)	
					{
						return (Not_Identify);
					}
				}
			}
		}

		if(rf_read(icdev,usercard_sec*4+3,data3)==MI_OK)
		{
			if(memcmp(&data3[6],access1,3)!=0)	
			{
			   if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);

			   if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				
				if(rf_card_2(icdev,ALL,&snr1,&tt) == MI_OK) 
				{					
					if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)
					{
				       if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

						if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return (No_Card);					
						if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)
						return (Not_Identify);
					}		
				}
				else return (No_Card);					
			}
		}
		else return(ReadCard_Err);

		memset(data1,0,16);
		memset(data2,0,16);

		IC.i = Serno;	
		use_snr = snr1;
		data1[0] = IC.ch[2];
		data1[1] = IC.ch[0];
		data1[2] = IC.ch[1]; 

		data1[3] = (unsigned char)UserType;

		IC1.i = use_term/1000;

		temp1= (unsigned char)((use_term/10000)%1000);
		temp3= (unsigned char)((temp1&0x0f)<< 4);
		temp2= (unsigned char)((use_term%10000)/100); 
		data1[6]  = temp3|temp2;
		temp3= (unsigned char)((use_term/10000)%1000);
		data1[7] = (unsigned char)((use_term%10000)%100);				
		data1[7] =(unsigned char) (data1[7]&0x1f) | ((temp3&0xf0)<<1);	

		for(i=strlen(Cardno),j=0;i>0;i--,j++)
		{
			data1[14-j] = Cardno[i-1];
		}

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data1[i];
		}	
		data1[15] = bcc;

		memset(data2, 0, 16);
		bcc = 0;

		for(i=0;i<15;i++)
		{
			bcc +=data2[i];
		}	
		data2[15] = bcc;
     
		IC.i = use_snr;
		Cala_Key12(IC.ch,SInew.oprater_password,data3);
		data3[6] = 0x7F;
		data3[7] = 0x07;
		data3[8] = 0x88;
		data3[9] = 0xDA;
		memcpy(&data3[10],SInew.sys_keyb,6);

		if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+2,data2) == MI_OK)
					if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
					{
						if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
						if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);			
						if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec+1) != MI_OK)
						{
							if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
							if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);					
							if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec+1) != MI_OK)
							{
								if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
								if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
								if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec+1) != MI_OK)	
								{
									if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
									if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
									if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec+1) != MI_OK)	
									{
										return (Not_Identify);
									}
								}
							}
						}

						if(rf_read(icdev,(usercard_sec+1)*4+3,data3)==MI_OK)
						{
							if(memcmp(&data3[6],access1,3)!=0)	
							{
								if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);

								if(rf_halt(icdev)!=MI_OK) return Reader_Err;  								
								if(rf_card_2(icdev,ALL,&snr1,&tt) == MI_OK) 
								{					
									if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec+1) != MI_OK)
									{
										if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

										if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return (No_Card);					
										if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec+1) != MI_OK)
										return (Not_Identify);
									}		
								}
								else return (No_Card);					
							}
						}
						else return(ReadCard_Err);

						IC.i = use_snr;
						Cala_Key12(IC.ch,SInew.oprater_password,data3);
						data3[6] = 0x7F;
						data3[7] = 0x07;
						data3[8] = 0x88;
						data3[9] = 0xDA;
						memcpy(&data3[10],SInew.sys_keyb,6);

						memcpy(data2, &user_code_new[9], 16);
						if(rf_write(icdev,(usercard_sec+1)*4,data2) == MI_OK)
							memcpy(data2, &user_code_new[25], 16);
							if(rf_write(icdev,(usercard_sec+1)*4+1,data2) == MI_OK)
								memcpy(data2, &user_code_new[41], 16);
								if(rf_write(icdev,(usercard_sec+1)*4+2,data2) == MI_OK)
									if(rf_write(icdev,(usercard_sec+1)*4+3,data3) == MI_OK)
						{
							if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
							*CardSerno = use_snr;
							return (DAS_OK);
						}
		}				
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall WRT_Pos_UserCard12(HANDLE icdev,int Value,unsigned int CardSerno,int WaitTime)
{
	unsigned long use_snr;
	unsigned char data1[16], Password[16];
	unsigned char data2[16] ;
	int usercard_sec;
	unsigned char bcc;
	int i;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if(Sfxt_author!=1) return (No_SF_Author);

	if((WaitTime < 0) | (Value <0)) return (Para_Err);

	usercard_sec = SInew.xfxt_sec;	
	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	Cala_Key12((unsigned char *)&CardSerno, SInew.oprater_password, &Password[0]);

	if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,Password) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno) return (UserCard_Err);
	
//		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)		return (UserCard_Err);
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK) 	return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);
	
		IC.i = Value;		
		data1[0] = IC.ch[0];
		data1[1] = IC.ch[1];
		data1[2] = IC.ch[2];  
	
		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data1[i];
		}	
		data1[15] = bcc;
	
		if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
		{
			if(rf_read(icdev,usercard_sec*4+2,data2) != MI_OK) return (ReadCard_Err);

			data1[14]=data2[14];
			bcc = 0;
			for(i=0;i<15;i++)
			{
				bcc +=data1[i];
			}	
			data1[15] = bcc;
			
			if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)			
			{
				if(rf_halt(icdev)!=0)  return Reader_Err;			
				return (DAS_OK);
			}
		}		
		return(WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall WRT_Pos_UserCard_AddCount12(HANDLE icdev, int Value, unsigned int CardSerno, int WaitTime)
{
	unsigned long use_snr;
	unsigned char data0[16], Password[16];
	unsigned char data1[16] ;
	int i, usercard_sec;
	unsigned char bcc;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else 
	{
		return No_Lic_Err;
	}
	
	if(Sfxt_author!=1) return (No_SF_Author);

	if((WaitTime < 0) | (Value <0)) return (Para_Err);

	usercard_sec = SInew.xfxt_sec;			

//	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	Cala_Key12((unsigned char *)&CardSerno, SInew.oprater_password, &Password[0]);

	if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,Password) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno)	return (UserCard_Err);

//		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)			return (UserCard_Err);
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK) 		return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);

		IC.i = Value;		
		data1[0] = IC.ch[0];
		data1[1] = IC.ch[1];
		data1[2] = IC.ch[2];  

		data0[9]++;

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data0[i];
		}	
		data0[15] = bcc;

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data1[i];
		}	
		data1[15] = bcc;
	  
		if(rf_write(icdev,usercard_sec*4,data0) == MI_OK)
		{
			if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
			{			
				if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)			
				{			  		
				   if(rf_halt(icdev)!=0)	return Reader_Err;			
					return (DAS_OK);
				}
			}	
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall RST_Pos_UserCard12(HANDLE icdev, unsigned int CardSerno, int WaitTime, unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16], data3[16];
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned char data6[16] = {0xf1,0xcf,0xd3,0xed,0xf9,0x58,0xFF,0x07,0x80,0x69,0xdd,0xea,0xfc,0xa0,0xbc,0xd1};
	unsigned char userkeyb[6], code_word[16];
	int usercard_sec;
	unsigned __int16 tt;	

	memset(data1,0xff,16);   

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	memset(data1, 0, 16);

	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}

	if(Sfxt_author!=1)   return (No_SF_Author);

	usercard_sec = SInew.xfxt_sec;

	memcpy(userkeyb,SInew.sys_keyb, 6);	

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec, userkeyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec+1, userkeyb) != MI_OK) return (Reader_Err);

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{	
		if(use_snr != CardSerno)	return(UserCard_Err);
		
		IC1.i = use_snr;
		cala_key12_new(IC1.ch,code_word,user_code_new);

		if(user_code_new[8] == 0x03)
		{
			if(rf_load_key(icdev, KEYB|KEYSET1, usercard_sec, &code_word[10]) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev, KEYB|KEYSET1, usercard_sec+1, &code_word[10]) != MI_OK) return (Reader_Err);
		}

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	
		{
			if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)	
			{
				return (Not_Identify);
			}
		}

		if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)

		if(user_code_new[8] == 0x01)					//1为不加密方式		
		{
			if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)  return WriteCard_Err;	
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)		return WriteCard_Err;
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			if(rf_write(icdev,usercard_sec*4+3,code_word) != MI_OK)		return WriteCard_Err;
		}
		else
		{
			return(Para_Err);
		}

		if(rf_authentication(icdev,KEYB|KEYSET2,(usercard_sec+1)) != MI_OK)		
		{
			if(rf_authentication(icdev,KEYB|KEYSET1,(usercard_sec+1)) != MI_OK)		
			{
				return (UserCard_Err);
			}
		}

		if(rf_write(icdev,(usercard_sec+1)*4,data1) == MI_OK)
			if(rf_write(icdev,(usercard_sec+1)*4+1,data1) == MI_OK)
				if(rf_write(icdev,(usercard_sec+1)*4+2,data1) == MI_OK)

		if(user_code_new[8] == 0x01)					//1为不加密方式		
		{
			if(rf_write(icdev,(usercard_sec+1)*4+3,data3) == MI_OK)	
			{
				if(	rf_halt(icdev)!=0)return Reader_Err;						
				return (DAS_OK);
			}
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_write(icdev,(usercard_sec+1)*4+3,data3) == MI_OK)	 
			{
				if(	rf_halt(icdev)!=0)return Reader_Err;						
				return (DAS_OK);
			}
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			if(rf_write(icdev,(usercard_sec+1)*4+3,code_word) == MI_OK) 
			{
				if(	rf_halt(icdev)!=0)  return Reader_Err;						
				return (DAS_OK);
			}
		}

		return (WriteCard_Err);
	}
	return (UserCard_Err);	
}

int __stdcall RST_UserCard_All(HANDLE icdev)
{
	unsigned long use_snr;
	unsigned char i;
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned __int16 tt;	

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;

	for(i=0; i<16; i++)	
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) continue;
		}

		if(rf_load_key(icdev, KEYB|KEYSET0, i, SInew.sys_keyb) != MI_OK)continue ;
		
		if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)	
		{
			if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)	continue;
		}

		if(rf_write(icdev, (4*i+3), data5) != MI_OK)
		{
			rf_write(icdev, (4*i+3), data5) ;
		}
	}

	return (DAS_OK);	
}
int __stdcall RST_UserCard(HANDLE icdev,unsigned char sector)
{
	unsigned long use_snr;

	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned __int16 tt;	

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;


	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)return No_Card ;
	}

	if(rf_load_key(icdev, KEYB|KEYSET0, sector, SInew.sys_keyb) != MI_OK) return (Reader_Err);
	
	if(rf_authentication(icdev, KEYB|KEYSET0, sector) != MI_OK)	
	{
		if(rf_authentication(icdev, KEYB|KEYSET0, sector) != MI_OK)	return UserCard_Err;
	}

	if(rf_write(icdev, (4*sector+3), data5) != MI_OK)
	{
		if(rf_write(icdev, (4*sector+3), data5) != MI_OK) return (WriteCard_Err);
	}
	

	return (DAS_OK);	
}

int __stdcall WRT_UserCard_Term(HANDLE icdev,int Systype,unsigned long useterm,unsigned int CardSerno,int WaitTime)
{
	unsigned long use_snr;
	unsigned char data0[16];
	unsigned char data1[16] ;
	int i, usercard_sec;
	unsigned char bcc,year,mounth,day;
	unsigned __int16 tt;
	
	year = (unsigned char)((useterm/10000)%1000);
	mounth = (unsigned char)((useterm%10000)/100);
	day    =  (unsigned char)(useterm%100);

	if((mounth>12&&mounth<0)||(day>31&&day<0)||(year>127&&year<0)||(WaitTime < 0)) return Para_Err;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}
 	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))
	{
		if(use_snr != CardSerno) return (UserCard_Err);

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)  return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);

		data0[6] = ((year&0x0f)<<4)|mounth;data0[7]=day;
		data0[7] = (data0[7]&0x1f) | ((year&0xf0)<<1);

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data0[i];
		}	
		data0[15] = bcc;

		if(rf_write(icdev,usercard_sec*4,data0) == MI_OK)
		{
			if(rf_halt(icdev)!=0) return Reader_Err;			
			return (DAS_OK);
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall WRT_UserCard_PassWord(HANDLE icdev,int Systype,unsigned char *Password,unsigned int CardSerno,int WaitTime)
{
	unsigned long use_snr;
	unsigned char data0[16];
	int i, usercard_sec;
	unsigned char bcc;
	unsigned __int16 tt;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}
 	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if((tt ==0x04)||(tt)||(tt == 0x02))
	{
		if(use_snr != CardSerno) return (UserCard_Err);

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)  return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);

		memcpy(&data0[4], Password, 2);

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data0[i];
		}	
		data0[15] = bcc;

		if(rf_write(icdev,usercard_sec*4,data0) == MI_OK)
		{
			if(rf_halt(icdev)!=0) return Reader_Err;			
			return (DAS_OK);
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall Clear_UserCard_Lock(HANDLE icdev, int Systype, unsigned long Address, unsigned int CardSerno, int WaitTime)
{
	unsigned long use_snr;
	unsigned char data0[16];
	unsigned char data1[16] ;
	int i, usercard_sec;
	unsigned char bcc;
	unsigned __int16 tt;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}
 	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))
	{
		if(use_snr != CardSerno) return (UserCard_Err);

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)  return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);

		data0[4] = (unsigned char)(Address&0xff);
		data0[5] = (unsigned char)((Address&0xff00)>>8);

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data0[i];
		}
		
		data0[15] = bcc;

		if(rf_write(icdev,usercard_sec*4,data0) == MI_OK)
		{
			if(rf_halt(icdev)!=0) return Reader_Err;			
			return (DAS_OK);
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall Init_Pos_OPTCard12(HANDLE icdev, int OPT_Num, int WaitTime, unsigned long * CardSerno, unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char usercard_sec,data1[16] ,data2[16],data3[16],data4[16],code_word[16];
	unsigned char bcc;
	unsigned __int16 tt;
	int i;	
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;

    if(Sfxt_author!=1) return (No_SF_Author);

	if((OPT_Num < 0) | (WaitTime < 0)) return (Para_Err);

	usercard_sec = 1; 

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}
	
	if(rf_read(icdev,1*4+3,data3)==MI_OK)
	{
		if(memcmp(&data3[6],access1,3)!=0)
		{
			if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);
		    if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

				if(rf_card_2(icdev,ALL,&use_snr,&tt) == MI_OK) 
				{
					if(!(tt & 0x06)) return (UserCard_Err);
					if(rf_authentication(icdev,KEYB|KEYSET0,1) != MI_OK)
					{
		              if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

						if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return (No_Card);					
						if(rf_authentication(icdev,KEYB|KEYSET1,1) != MI_OK)
						return (Not_Identify);
					}		
				}
				else return (No_Card);					
		}		
	}
	else return(ReadCard_Err);

	memset(data1,0,16);
	memset(data2,0,16);
	memset(data4,0,16);

	data1[0] = 0x04;
	memcpy(&data1[1],SInew.syscard_no_xf,5);

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}	
	data1[15] = bcc;

	IC.i = OPT_Num;
	data2[0]=IC.ch[0];	
	data2[1]=IC.ch[1];	
	data2[2]=IC.ch[2];	
	data2[3]=IC.ch[3];	

	memcpy(&data2[4], &user_code_new[10], 7);
	
	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data2[i];
	}	
	data2[15] = bcc;	

	memcpy(data3,SystemCard_KeyA12,6);			
	data3[6] = 0x7F;
	data3[7] = 0x07;
	data3[8] = 0x88;
	data3[9] = 0xDA;
	memcpy(&data3[10],SystemCard_KeyB12,6);		
	
	if(rf_write(icdev,4,data1) == MI_OK)
		if(rf_write(icdev,1*4+1,data2) == MI_OK)
			if(rf_write(icdev,1*4+2,data4) == MI_OK)
				if(rf_write(icdev,1*4+3,data3) == MI_OK)
				{
		           if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

					*CardSerno = use_snr;
					return (DAS_OK);
				}
				
 	return (WriteCard_Err);
}

int __stdcall Get_SysUse_Sec(unsigned int *Mjxt_Sec,unsigned int *Sfxt_Sec,unsigned int *Jsxt_Sec,LPSTR CommPassword)
{
	*Mjxt_Sec = 0; 
	*Sfxt_Sec = 0; 
	*Jsxt_Sec = 0;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		*Mjxt_Sec  = SInew.mjxt_sec;
		*Sfxt_Sec  = SInew.xfxt_sec;
		*Jsxt_Sec  = SInew.jsxt_sec;
		memcpy(CommPassword,SInew.comm_password,8);
	}
	else 
	{
		return No_Lic_Err;
	}
	return(DAS_OK);
}

int __stdcall Change_Pos_UserType12(HANDLE icdev, int UserType)
{
	unsigned long use_snr;
	unsigned char data1[16];
	unsigned char usertype, bcc=0;
	int usercard_sec;
	unsigned __int16 tt;
	int i;	

	if((UserType>32)||(UserType<1)) return Para_Err;

	if (Get_Lic_New(SInew.ch) == 0)
	{
		if (SInew.mjxt) Mjxt_author = 1; 
		if (SInew.xfxt) Sfxt_author = 1; 
		if (SInew.jsxt) Jsxt_author = 1; 
	}

	if (Sfxt_author!=1) return (No_SF_Author);
        		
	usercard_sec = SInew.xfxt_sec;			
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_halt(icdev)) return Reader_Err; 
	if (rf_card_2(icdev,ALL,&use_snr,&tt)) return No_Card; 
	
	if((tt ==0x04)||(tt)||(tt == 0x02)||(tt)) 
	{	
		if (rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)return UserCard_Err;		
		if (rf_read(icdev,usercard_sec*4+0,data1) != MI_OK) return ReadCard_Err;
		usertype= data1[3];		
        if((UserType>32)||(usertype<1)) return UserCard_Err;
		data1[3]=(unsigned char)UserType;
		for(i=0;i<15;i++)bcc+=data1[i];  
		data1[15]=bcc;
		if (rf_write(icdev,usercard_sec*4+0,data1) != MI_OK) return WriteCard_Err;
		if(rf_halt(icdev)) return Reader_Err;
        return DAS_OK;
	}						
	return UserCard_Err;
}

int __stdcall Change_Pos_UserType12_NEW(HANDLE icdev,int * Old_UserType,int UserType ,int Systype)
{
	unsigned long use_snr;
	unsigned char  data1[16];
	unsigned char  bcc=0;
	int usercard_sec;
	unsigned __int16 tt;
	int i;	

	if((UserType>32)||(UserType<1)) return Para_Err;

	if (Get_Lic_New(SInew.ch) == 0)
	{
		if (SInew.mjxt) Mjxt_author = 1; 
		if (SInew.xfxt) Sfxt_author = 1; 
		if (SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;

    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;

	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;

	default:
		break;
	}
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_halt(icdev)) return Reader_Err; 
	if(rf_card_2(icdev,ALL,&use_snr,&tt)) return No_Card; 
	
	if ((tt ==0x04)||(tt)||(tt == 0x02)||(tt)) 
	{	
		if (rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)return UserCard_Err;		
		if (rf_read(icdev,usercard_sec*4+0,data1) != MI_OK) return ReadCard_Err;
		*Old_UserType = data1[3];		
		data1[3]=(unsigned char )UserType;
		for(i=0;i<15;i++) bcc+=data1[i];
		data1[15]=bcc;
		if(rf_write(icdev,usercard_sec*4+0,data1) != MI_OK) return WriteCard_Err;
		if(rf_halt(icdev)) return Reader_Err;
        return DAS_OK;
	}						
	return UserCard_Err;
}

int __stdcall Update_UserCard_Period(HANDLE icdev, unsigned long use_term, int Systype, int Number)
{
	unsigned long use_snr;
	unsigned char temp1, temp2, temp3, data1[16];
	unsigned char bcc=0;
	int usercard_sec;
	unsigned __int16 tt;
	int i;	

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if (SInew.mjxt) Mjxt_author = 1; 
		if (SInew.xfxt) Sfxt_author = 1; 
		if (SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if ((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if ((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if ((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;

    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;

	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;

	default:
		break;
	}

	usercard_sec = usercard_sec + Number;
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_halt(icdev)) return Reader_Err; 
	if(rf_card_2(icdev,ALL,&use_snr,&tt)) return No_Card; 
	
	if(tt == 0x04) 
	{	
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)return UserCard_Err;		
		if(rf_read(icdev,usercard_sec*4+0,data1) != MI_OK) return ReadCard_Err;
		
		temp1= (unsigned char)((use_term/10000)%1000);
		temp3=(unsigned char)((temp1 & 0x0f)<< 4);
		temp2= (unsigned char)((use_term % 10000)/100); 

		data1[6]  = temp3|temp2;
		temp3= (unsigned char)((use_term/10000)%1000);
		data1[7] = (unsigned char)((use_term%10000)%100);					//使用期限2006-4-22表示为0X64 0x16
		data1[7] =(unsigned char) (data1[7]&0x1f) | ((temp3&0xf0)<<1);		//年的高位放在7字节的高三位 10-11

		for(i=0;i<15;i++) bcc+=data1[i];
		data1[15]=bcc;
		if(rf_write(icdev,usercard_sec*4+0,data1) != MI_OK) return WriteCard_Err;
		if(rf_halt(icdev)) return Reader_Err;
        return DAS_OK;
	}						
	return UserCard_Err;
}

int __stdcall RST_Pos_OPTCard12(HANDLE icdev,unsigned int CardSerno,int WaitTime ,unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16] ,data3[16];
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned char data6[16] = {0xf1,0xcf,0xd3,0xed,0xf9,0x58,0xFF,0x07,0x80,0x69,0xdd,0xea,0xfc,0xa0,0xbc,0xd1};
	unsigned char code_word[16];
	int usercard_sec;
	unsigned __int16 tt;
	
	memset(data1,0xff,16);   

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;

	if((Sfxt_author!=1)&&(Jsxt_author!=1))		return (No_SF_Author);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if(rf_load_key(icdev,KEYB|KEYSET2,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	usercard_sec = 0x01;

	if(!((tt ==0x04)||(tt)||(tt == 0x02))) return (UserCard_Err);
	if(use_snr != CardSerno) return (UserCard_Err);
	
	IC1.i = use_snr;
	cala_key12_new(IC1.ch,code_word,user_code_new);

	if(rf_authentication(icdev,KEYB|KEYSET2,1) != MI_OK)	return (UserCard_Err);

	memset(data1,0xff,16);		

	if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)	
		{
			if(	rf_halt(icdev)!=0)return Reader_Err;						
			return (DAS_OK);
		}
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)	 
		{
			if(	rf_halt(icdev)!=0)return Reader_Err;						
			return (DAS_OK);
		}
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,code_word) == MI_OK) 
		{
			if(	rf_halt(icdev)!=0)return Reader_Err;						
			return (DAS_OK);
		}
	}	
	else
	{
		return(Para_Err);
	}	
	return (WriteCard_Err);
}

int __stdcall Init_Pos_UserCard_N12(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int Value,int UseCount,int WaitTime,unsigned long * CardSerno,unsigned long use_term,unsigned char * user_code_new)
{
	unsigned long snr1,use_snr;
	unsigned char data1[16] ,data2[16],code_word[16];
	int usercard_sec, i, j;
	unsigned char bcc,temp1,temp2,temp3;	
	unsigned char data3[16];
	unsigned __int16 tt;
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1;
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Sfxt_author!=1) return (No_SF_Author);

	if((strlen(Cardno)>5) | (Serno <= 0) | (WaitTime < 0) | (UserType==0) | (UserType>32)) return (Para_Err);

	usercard_sec = SInew.xfxt_sec;

	if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return No_Card;
	}

	IC1.i = snr1;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec+1,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec+1,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec+1,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec+1,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec+1,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec+1,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec+1,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if((tt == 0x04)||(tt)||(tt == 0x02))		
	{	
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);			
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
				if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	
				{
					if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
					if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
					if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)	
					{
						return (Not_Identify);
					}
				}
			}
		}

		if(rf_read(icdev,usercard_sec*4+3,data3)==MI_OK)
		{
			if(memcmp(&data3[6],access1,3)!=0)	
			{
			   if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);

			   if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				
				if(rf_card_2(icdev,ALL,&snr1,&tt) == MI_OK) 
				{					
					if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)
					{
				       if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

						if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return (No_Card);					
						if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)
						return (Not_Identify);
					}		
				}
				else return (No_Card);					
			}
		}
		else return(ReadCard_Err);

		memset(data1,0,16);
		memset(data2,0,16);
		IC.i = Serno;
		use_snr = snr1;
		data1[0]=IC.ch[2];
		data1[1] = IC.ch[0];data1[2] = IC.ch[1]; 
		data1[3] = (unsigned char)UserType;

		temp1=(unsigned char) ((use_term/10000)%1000);
		temp3=(unsigned char)((temp1 & 0x0f)<< 4);
		temp2= (unsigned char)((use_term % 10000)/100);
		data1[6]  = temp3|temp2;
		temp3= (unsigned char)((use_term/10000)%1000);
		data1[7] = (unsigned char)((use_term%10000)%100); 
		data1[7] = (data1[7]&0x1f) | ((temp3&0xf0)<<1);

		IC.i = UseCount;
		data1[8]=IC.ch[0];data1[9]=IC.ch[1];

		for(i=strlen(Cardno),j=0;i>0;i--,j++)
		{
			data1[14-j] = Cardno[i-1];
		}
		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data1[i];
		}	
		data1[15] = bcc;

		IC.i = Value;
		data2[0]=IC.ch[0];data2[1]=IC.ch[1];data2[2]=IC.ch[2];

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data2[i];
		}	
		data2[15] = bcc;
	
		IC.i = use_snr;
		Cala_Key12(IC.ch,SInew.oprater_password,data3);
		data3[6] = 0x7F;
		data3[7] = 0x07;
		data3[8] = 0x88;
		data3[9] = 0xDA;
		memcpy(&data3[10],SInew.sys_keyb,6);

		if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+2,data2) == MI_OK)
					if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
					{
						if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
						if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);			
						if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec+1) != MI_OK)
						{
							if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
							if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);					
							if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec+1) != MI_OK)
							{
								if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
								if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
								if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec+1) != MI_OK)	
								{
									if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
									if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);				
									if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec+1) != MI_OK)	
									{
										return (Not_Identify);
									}
								}
							}
						}

						if(rf_read(icdev,(usercard_sec+1)*4+3,data3)==MI_OK)
						{
							if(memcmp(&data3[6],access1,3)!=0)	
							{
								if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);

								if(rf_halt(icdev)!=MI_OK) return Reader_Err;  								
								if(rf_card_2(icdev,ALL,&snr1,&tt) == MI_OK) 
								{					
									if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec+1) != MI_OK)
									{
										if(rf_halt(icdev)!=MI_OK) return Reader_Err;  

										if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return (No_Card);					
										if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec+1) != MI_OK)
										return (Not_Identify);
									}		
								}
								else return (No_Card);					
							}
						}
						else return(ReadCard_Err);

						IC.i = use_snr;
						Cala_Key12(IC.ch,SInew.oprater_password,data3);
						data3[6] = 0x7F;
						data3[7] = 0x07;
						data3[8] = 0x88;
						data3[9] = 0xDA;
						memcpy(&data3[10],SInew.sys_keyb,6);

						memcpy(data2, &user_code_new[9], 16);
						if(rf_write(icdev,(usercard_sec+1)*4,data2) == MI_OK)
							memcpy(data2, &user_code_new[25], 16);
							if(rf_write(icdev,(usercard_sec+1)*4+1,data2) == MI_OK)
								memcpy(data2, &user_code_new[41], 16);
								if(rf_write(icdev,(usercard_sec+1)*4+2,data2) == MI_OK)
									if(rf_write(icdev,(usercard_sec+1)*4+3,data3) == MI_OK)
						{
							if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
							*CardSerno = use_snr;
							return (DAS_OK);
						}
		}				
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

//========================================================================节能控制器函数部分===================================================

int __stdcall Init_JS_OPTCard12(HANDLE icdev,unsigned long OPT_Num,int WaitTime,unsigned long * CardSerno ,unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char usercard_sec,data1[16] ,data2[16],data3[16],data4[16],code_word[16];
	unsigned char bcc;
	unsigned __int16 tt;
	int i;	
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;

	if((Jsxt_author!=1)) return (No_JS_Author);

	if((OPT_Num<0)|(WaitTime < 0))    return (Para_Err);

	usercard_sec = 1;  

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}
	
	if(rf_read(icdev,usercard_sec*4+3,data3)==MI_OK)
	{
		if(memcmp(&data3[6],access1,3)!=0)
		{
			if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);
		    if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

			if(rf_card_2(icdev,ALL,&use_snr,&tt) == MI_OK) 
			{
				if(!(tt & 0x06)) return (UserCard_Err);
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)
				{
					if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

					if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return (No_Card);					
					if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)
					return (Not_Identify);
				}		
			}
			else return(No_Card);					
		}		
	}
	else return(ReadCard_Err);

	memset(data1,0,16);
	memset(data2,0,16);
	memset(data4,0,16);

	data1[0] = 0x04;

	memcpy(&data1[1],SInew.syscard_no_js,5);

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}	
	data1[15] = bcc;

	IC.i = OPT_Num;
	data2[0]=IC.ch[0];	
	data2[1]=IC.ch[1];	
	data2[2]=IC.ch[2];	
	data2[3]=IC.ch[3];	

	memcpy(&data2[4], &user_code_new[10], 7);
	
	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data2[i];
	}	
	data2[15] = bcc;	

	memcpy(data3,SystemCard_KeyA12,6);			
	data3[6] = 0x7F;
	data3[7] = 0x07;
	data3[8] = 0x88;
	data3[9] = 0xDA;
	memcpy(&data3[10],SystemCard_KeyB12,6);		
	
	if(rf_write(icdev,usercard_sec*4+0,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data4) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
				{
		           if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

					*CardSerno = use_snr;
					return (DAS_OK);
				}
				
 	return (WriteCard_Err);
}

int __stdcall Init_Js_UserCard(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,unsigned long * CardSerno ,unsigned char * user_code_new)
{
	unsigned long snr1,use_snr;
	unsigned char data1[16], data2[16], code_word[16];
	int usercard_sec;		
	unsigned char bcc;
	int i,j;	
	unsigned char data3[16];
	unsigned __int16 tt;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if((Jsxt_author!=1)&&(Sfxt_author != 1)) return (No_JS_Author);
	
	if((UserType<1) || (UserType>32))		return (Para_Err);

	if(Jsxt_author==1) 
	{
		usercard_sec = (SInew.jsxt_sec) ;
	}
	else
	{
		if(Sfxt_author==1)
		{
			usercard_sec = (SInew.xfxt_sec) ;
		}
	}

	if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr1,&tt) != MI_OK) return No_Card;
	}

	IC1.i  = 0 ;
	IC1.i = snr1;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,&code_word[10]) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);		
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);						
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr1) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	if(rf_read(icdev,usercard_sec*4+3,data3)==MI_OK)
	{
		if(memcmp(&data3[6],access1,3)!=0)
		{
			if(memcmp(&data3[6],access2,3)!=0) return (Not_Identify);

		    if(rf_halt(icdev)!=MI_OK)return Reader_Err;	
			if(rf_card_2(icdev,ALL,&use_snr,&tt) == MI_OK) 
			{
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)
				{
					if(rf_halt(icdev)!=MI_OK)return Reader_Err;	
					if(rf_card_2(icdev,ALL,&use_snr,&tt) == MI_OK) 
					{
						if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)
						{
							if(rf_halt(icdev)!=MI_OK)return Reader_Err;	

							if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return (No_Card);					
							if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (Not_Identify);
						}
					}
					else return(No_Card);
				}		
			}
			else return(No_Card);					
		}		
	}
	else return(ReadCard_Err);

	memset(data1,0,16);
	memset(data2,0,16);

	IC.i  = 0;
	IC.i  = Serno;					

	data1[0] = IC.ch[2];
	data1[1] = IC.ch[0];
	data1[2] = IC.ch[1]; 
	
	data1[3] = (unsigned char)UserType;	

	i=strlen(Cardno); 
	if(i>5) i = 5;
	for(i,j=0; i>0; i--,j++)
	{
		data1[14-j] = Cardno[i-1];
	}

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}	
	data1[15] = bcc;

	data2[10] = 1;
	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data2[i];
	}	
	data2[15] = bcc;
	IC.i  = 0;

	use_snr =  snr1;

	IC.i = snr1;

	Cala_Key12(IC.ch,SInew.oprater_password,data3);

	data3[6] = 0x7F;data3[7]=0x07;data3[8]=0x88;data3[9]=0xDA;
	memcpy(&data3[10],SInew.sys_keyb,6);
	
	if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data2) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
				{
					if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
					
					*CardSerno = use_snr;
					return (DAS_OK);
				}				
	return (WriteCard_Err);
}
int __stdcall Init_Js_UserCard_N(HANDLE icdev,int Serno,LPCSTR Cardno,int UserType,int CardBlance,int ChargeTimes,unsigned long * CardSerno ,unsigned char * user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16] ,data2[16],code_word[16];
	int usercard_sec;
	unsigned char bcc;
	int i,j;	
	unsigned char data3[16];
	unsigned __int16 tt;
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if(Jsxt_author!=1) return (No_JS_Author);
	if((UserType<1) || (UserType>32))		return (Para_Err);
	if(Jsxt_author==1)         
	{
		usercard_sec = SInew.jsxt_sec;
	} 
	else
	{
		if(Sfxt_author == 1)
		{
			usercard_sec = SInew.xfxt_sec;
		}
	}

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;
	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);		
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);						
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(data1,0,16);
	memset(data2,0,16);
	IC.i = Serno;
	data1[0] = IC.ch[2];
	data1[1] = IC.ch[0];
	data1[2] = IC.ch[1]; 
	data1[3] = (unsigned char)UserType;
	i=strlen(Cardno);  

	if(i>5) i = 5;
	for(i,j=0;i>0;i--,j++)
	{
		data1[14-j] = Cardno[i-1];
	}

	IC.i = ChargeTimes;
	data1[8]=IC.ch[0];
	data1[9]=IC.ch[1];
	bcc = 0;

	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}
	
	data1[15] = bcc;
	
	IC.i = CardBlance;
	data2[0]=IC.ch[0];
	data2[1]=IC.ch[1];
	data2[2]=IC.ch[2];

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data2[i];
	}	
	data2[15] = bcc;
	
	IC.i = use_snr;
	Cala_Key12(IC.ch,SInew.oprater_password,data3);
	data3[6] = 0x7F;data3[7]=0x07;data3[8]=0x88;data3[9]=0x69;
	memcpy(&data3[10],SInew.sys_keyb,6);
	if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data2) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
				{
					
					if(	rf_halt(icdev)!=MI_OK)return Reader_Err;	
					*CardSerno = use_snr;
					return (DAS_OK);
				}

				
	return (WriteCard_Err);
}

int __stdcall Init_Js_UserCard_New(HANDLE icdev,int newcard_Serno,int oldcard_Serno,LPCSTR Cardno,int UserType,int CardBlance,int ChargeTimes,unsigned long * CardSerno ,unsigned char * user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16] ,data2[16],code_word[16];
	int usercard_sec;
	unsigned char bcc;
	int i,j;	
	unsigned char data3[16];
	unsigned __int16 tt;
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if(Jsxt_author!=1) return (No_JS_Author);
	
	//if(((UserType<1) || (UserType>32))&&(UserType != 0xFF))		return (Para_Err);

	if(Jsxt_author==1)         
	{
		usercard_sec = SInew.jsxt_sec;
	} 
	else
	{
		if(Sfxt_author == 1)
		{
			usercard_sec = SInew.xfxt_sec;
		}
	}

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = use_snr;
	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);		
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);						
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&use_snr) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(data1,0,16);
	memset(data2,0,16);
	IC.i = newcard_Serno;
	data1[0] = IC.ch[2];
	data1[1] = IC.ch[0];
	data1[2] = IC.ch[1]; 
	data1[3] = (unsigned char)UserType;
	i=strlen(Cardno);  

	if(i>5) i = 5;
	for(i,j=0;i>0;i--,j++)
	{
		data1[14-j] = Cardno[i-1];
	}

	IC.i = ChargeTimes;
	data1[8]=IC.ch[0];
	data1[9]=IC.ch[1];
	bcc = 0;

	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}
	
	data1[15] = bcc;
	
	IC.i = CardBlance;
	data2[0]=IC.ch[0];
	data2[1]=IC.ch[1];
	data2[2]=IC.ch[2];
	
	IC.i = 0;
	IC.i = oldcard_Serno;
	data2[3] = IC.ch[2];
	data2[4] = IC.ch[0];
	data2[5] = IC.ch[1]; 

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data2[i];
	}	
	data2[15] = bcc;
	
	IC.i = use_snr;
	Cala_Key12(IC.ch,SInew.oprater_password,data3);
	data3[6] = 0x7F;data3[7]=0x07;data3[8]=0x88;data3[9]=0xDA;
	memcpy(&data3[10],SInew.sys_keyb,6);
	if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data2) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data2) == MI_OK)
				if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)
				{
					
					if(	rf_halt(icdev)!=MI_OK)return Reader_Err;	
					*CardSerno = use_snr;
					return (DAS_OK);
				}

				
	return (WriteCard_Err);
}

int __stdcall WRT_Js_UserCard(HANDLE icdev,int Blance,unsigned long CardSerno)
{
	unsigned long use_snr;
	unsigned char data1[16], Password[16];
	int i, usercard_sec;
	unsigned char bcc;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if(Jsxt_author!=1) return (No_JS_Author);

	if(Blance <0) return (Para_Err);
		
	if(Jsxt_author==1) {usercard_sec = SInew.jsxt_sec;}

//	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
	if(use_snr != CardSerno) return (UserCard_Err);

	Cala_Key12((unsigned char *)&CardSerno, SInew.oprater_password, &Password[0]);
	if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,Password) != MI_OK) return (Reader_Err);
	
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK) 	return (UserCard_Err);
		
	if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);
	
	IC.i = Blance;		
	data1[0] = IC.ch[0];
	data1[1] = IC.ch[1];
	data1[2] = IC.ch[2];  
	
	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}	
	data1[15] = bcc;
	
	if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
	if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)			
	{
		if(	rf_halt(icdev)!=0)return Reader_Err;			
		return (DAS_OK);
	}
	return (WriteCard_Err);		
}

int __stdcall WRT_Js_UserCard_AddCount(HANDLE icdev, int Blance, char *ChargeDateTime, unsigned long CardSerno)
{
	unsigned long use_snr;
	unsigned char data0[16], Password[16];
	unsigned char data1[16] ;
	int usercard_sec;
	unsigned char bcc;
	int i;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if((Jsxt_author!=1)&&(Sfxt_author != 1)) return (No_JS_Author);
	
	if(strlen(ChargeDateTime)!=12)  return Para_Err;
		
	if(Jsxt_author==1)
	{
		usercard_sec = SInew.jsxt_sec;
	}
    else
	{
		if(Sfxt_author==1)
		{
			usercard_sec = SInew.xfxt_sec;
		}
	}

//	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if(use_snr != CardSerno) return (UserCard_Err);

	Cala_Key12((unsigned char *)&CardSerno, SInew.oprater_password, &Password[0]);
	if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,Password) != MI_OK) return (Reader_Err);
	
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK) 	return (UserCard_Err);
	
//	if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)		return (UserCard_Err);
	
	if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);
	if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);
	
//	NumberStr2BCD(TempStr,12);

	IC.i = Blance; 
	data1[0] = IC.ch[0];
	data1[1] = IC.ch[1];
	data1[2] = IC.ch[2];  
	
	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data1[i];
	}	
	data1[15] = bcc;


	IC.i=0;
	IC.ch[0]=data0[8];IC.ch[1]=data0[9];
	IC.i=IC.i+1;
	data0[8]=IC.ch[0];data0[9]=IC.ch[1];

	bcc = 0;
	for(i=0;i<15;i++)
	{
		bcc +=data0[i];
	}	
	data0[15] = bcc;

	if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
	{
		if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)			
		{
			if(	rf_halt(icdev)!=0)return Reader_Err;			
			return (DAS_OK);
		}
	}
	return (WriteCard_Err);
}

int __stdcall InitCollectCard(HANDLE icdev,int OperatorNo, int Mode, unsigned long * CardSerno ,unsigned char * user_code_new)
{
	unsigned long snr;
	unsigned __int16 tt;
	BYTE bcc = 0;
	int i;	
	unsigned char  code_word[16];
	BYTE  BlockData[16];

	int usercard_sec;	
	
	union ic1
	{
	  unsigned	long i;
		unsigned char ch[4];
	}IC1;
	
	if(OperatorNo < 0 || OperatorNo>255)  	return (Para_Err);
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else		return No_Lic_Err;

    if(Jsxt_author!=1) return (No_JS_Author);

	usercard_sec = 0;

	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
	}

	if(rf_halt(icdev))	return Reader_Err;
	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;	

	IC1.i = snr ;

	if(user_code_new[8] == 0x01)					//1为不加密方式	
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec, SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
				if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (Not_Identify);	
			}
		}
	}
	
	*CardSerno = snr;
	memset(BlockData,0,16);
	BlockData[0] = 0x0D;						
	memcpy(BlockData+1, SInew.syscard_no_js, 5);		
	if(Mode)  BlockData[10] = 0xAA;
	else BlockData[10] = 0x00;

	for(i=0;i<15;i++)		bcc += BlockData[i];

	BlockData[15] = bcc;
	if(rf_write(icdev,1,BlockData) != MI_OK) 		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) 						return WriteCard_Err;  
	
	for(i=1; i<16; i++)
	{
		usercard_sec = i;

		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
		}

		IC1.i = snr ;

		if(user_code_new[8] == 0x01)					//1为不加密方式	
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			cala_key12_new(IC1.ch,code_word,user_code_new);
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		}
		else
		{
			return(Para_Err);
		}

		if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec, SystemCard_KeyB12) != MI_OK) return (Reader_Err);

		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	
				{
					if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
					if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
					if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (Not_Identify);	
				}
			}
		}
			
		if(rf_write(icdev, (usercard_sec*4)+3, BlockData) != MI_OK) 			return(WriteCard_Err);
		if(rf_halt(icdev)!=MI_OK) 												return(WriteCard_Err);  
	}
	return(DAS_OK);
}

int __stdcall InitBlackCard(HANDLE icdev, unsigned char Block, unsigned long * CardSerno, unsigned char * user_code_new)
{
	unsigned long snr;
	unsigned __int16 tt;
	BYTE bcc = 0;
	int i;
	unsigned char BlockData1[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char  code_word[16];

	BYTE  BlockData[16];

	int usercard_sec;	
	
	union ic1
	{
		unsigned long i;
		unsigned char ch[4];
	}IC1;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else  return No_Lic_Err;

    if(Jsxt_author!=1) return (No_JS_Author);

	usercard_sec = 0;

	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
	}

	IC1.i = snr ;

	if(user_code_new[8] == 0x01)					//1为不加密方式	
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}

	if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec, SystemCard_KeyB12) != MI_OK) return (Reader_Err);

	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
				if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (Not_Identify);	
			}
		}
	}
	
	*CardSerno = snr;
	memset(BlockData,0,16);
	BlockData[0] = 0xAC;								
	memcpy(BlockData+1, SInew.syscard_no_js, 5);
	BlockData[10] = Block;

	for(i=0;i<15;i++)		bcc += BlockData[i];

	BlockData[15] = bcc;
	if(rf_write(icdev,1,BlockData) != MI_OK) 		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) 						return WriteCard_Err;  
	
	for(i=1; i<16; i++)
	{
		usercard_sec = i;

		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
		}

		if(rf_halt(icdev))	return Reader_Err;
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;	
		IC1.i = snr ;

		if(rf_load_key(icdev, KEYB|KEYSET2, usercard_sec, SystemCard_KeyB12) != MI_OK) return (Reader_Err);

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)//KEYA|KEYSET0
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);		   			
				if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)
				{
					if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
					if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return (No_Card);					
					if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);	
				}
			}
		}
		if(rf_write(icdev, (usercard_sec*4)+0, BlockData1) != MI_OK) 			return(WriteCard_Err);
		if(rf_write(icdev, (usercard_sec*4)+1, BlockData1) != MI_OK) 			return(WriteCard_Err);
		if(rf_write(icdev, (usercard_sec*4)+2, BlockData1) != MI_OK) 			return(WriteCard_Err);
		if(rf_write(icdev, (usercard_sec*4)+3, BlockData) != MI_OK) 			return(WriteCard_Err);
		if(rf_halt(icdev)!=MI_OK) 			return WriteCard_Err;  
	}
	return(DAS_OK);
}

int __stdcall WriteBlackCard(HANDLE icdev, unsigned long CardSerno, unsigned char *Black_Data)
{
	unsigned long snr;
	unsigned char i, j ;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author); 

	for(i=1; i<16; i++)
	{
		if(rf_load_key(icdev,KEYB|KEYSET0,i,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}


	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(snr != CardSerno)   return(UserCard_Err);

	for(i=1; i<16; i++)
	{
		if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)			return (Not_Identify);
		for(j=0; j<3; j++)
		{
			if(rf_write(icdev, (i*4)+j, &Black_Data[(((i-1)*3)+j)*16]) != MI_OK) 			return(WriteCard_Err);
		}
	}	
	if(	rf_halt(icdev)!=0)		return Reader_Err;			
	return (DAS_OK);
}

int __stdcall ReadRecNum(HANDLE icdev,unsigned int &RecNum, unsigned long &CardSerno)
{
	unsigned long snr;
	unsigned char data1[16] ;	

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author); 
	if(rf_load_key(icdev,KEYB|KEYSET0,0,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	CardSerno = snr;

	if(rf_authentication(icdev,KEYB|KEYSET0,0) != MI_OK)			return (UserCard_Err);

	if(rf_read(icdev,1,data1) != MI_OK) 		return (ReadCard_Err);

    RecNum = data1[7];

	if(	rf_halt(icdev)!=0)		return Reader_Err;			
	return (DAS_OK);
}

int __stdcall ReadRecNum_NEW(HANDLE icdev, unsigned int *MODE, unsigned long *CardSerno, unsigned int *RecNum)
{
	unsigned long snr;
	unsigned char data1[16] ;	

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author); 
	if(rf_load_key(icdev,KEYB|KEYSET0,0,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	*CardSerno = snr;

	if(rf_authentication(icdev,KEYB|KEYSET0,0) != MI_OK)			return (UserCard_Err);
	if(rf_read(icdev,1,data1) != MI_OK) 							return (ReadCard_Err);
	
	if(data1[10] == 0xAA)
	{
		*MODE = 1 ;
		*RecNum = 0;
	}
	else
	{
		*MODE = 0 ;
		*RecNum = data1[7];
	}
	if(	rf_halt(icdev)!=0)		return Reader_Err;			
	return (DAS_OK);
}

int __stdcall ReadAllRec_NEW(HANDLE icdev, RecordAllData *RecAllData, unsigned long CardSerno)
{
	unsigned long snr;
	unsigned char i, j, data1[16] ;
	
	union ic1
	{
	  unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author); 

	for(i=1; i<16; i++)
	{
		if(rf_load_key(icdev,KEYB|KEYSET0,i,SystemCard_KeyB12) != MI_OK) return (PassWord_Err);
	}

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(snr != CardSerno)   return(UserCard_Err);

	for(i=1; i<16; i++)
	{
		if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)			return (UserCard_Err);
		for(j=0; j<3; j++)
		{
			if(rf_read(icdev, (i*4)+j, data1) != MI_OK) 	return (ReadCard_Err);
			IC1.i = 0 ;
			IC1.ch[0] = data1[3] ;
			IC1.ch[1] = data1[2] ;
			IC1.ch[2] = data1[1] ;
			IC1.ch[3] = data1[0] ;
			RecAllData[((i-1)*3)+j].Materiel_All_Money = IC1.i ;
			IC1.i = 0 ;
			IC1.ch[0] = data1[7] ;
			IC1.ch[1] = data1[6] ;
			IC1.ch[2] = data1[5] ;
			IC1.ch[3] = data1[4] ;
			RecAllData[((i-1)*3)+j].Materiel_All_Times = IC1.i ;
			IC1.i = 0 ;
			IC1.ch[0] = data1[11] ;
			IC1.ch[1] = data1[10] ;
			IC1.ch[2] = data1[9] ;
			IC1.ch[3] = data1[8] ;
			RecAllData[((i-1)*3)+j].Materiel_All_Amount = IC1.i ;
			IC1.i = 0 ;
			IC1.ch[0] = data1[13] ;
			IC1.ch[1] = data1[12] ;
			IC1.ch[2] = 0x00 ;
			IC1.ch[3] = 0x00 ;
			RecAllData[((i-1)*3)+j].Card_Address = IC1.i ;				
		}
	}	
	if(	rf_halt(icdev)!=0)		return Reader_Err;			
	return (DAS_OK);
}

int __stdcall ReadDetailedRec_NEW(HANDLE icdev, RecordDetailedData *RecDetailedData, unsigned long CardSerno, unsigned long *All_Money , unsigned long *All_Times, unsigned long *All_Amount, unsigned long *Address)
{
	unsigned long snr;
	unsigned char i, j, data1[16] ,buff[6] ;
	
	union ic1
	{
		unsigned   long i;
		unsigned   char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author); 

	for(i=0; i<16; i++)
	{
		if(rf_load_key(icdev,KEYB|KEYSET0,i,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}	

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(snr != CardSerno)   return(UserCard_Err);

	if(rf_authentication(icdev, KEYB|KEYSET0, 0) != MI_OK)			return (UserCard_Err);
	if(rf_read(icdev, 2, data1) != MI_OK) 							return (ReadCard_Err);

	IC1.i = 0 ;
	IC1.ch[0] = data1[3] ;
	IC1.ch[1] = data1[2] ;
	IC1.ch[2] = data1[1] ;
	IC1.ch[3] = data1[0] ;
	*All_Money = IC1.i ;

	IC1.i = 0 ;
	IC1.ch[0] = data1[7] ;
	IC1.ch[1] = data1[6] ;
	IC1.ch[2] = data1[5] ;
	IC1.ch[3] = data1[4] ;
	*All_Amount = IC1.i ;

	IC1.i = 0 ;
	IC1.ch[0] = data1[11] ;
	IC1.ch[1] = data1[10] ;
	IC1.ch[2] = data1[9] ;
	IC1.ch[3] = data1[8] ;
	*All_Times = IC1.i ;

	IC1.i = 0 ;
	IC1.ch[0] = data1[13] ;
	IC1.ch[1] = data1[12] ;
	IC1.ch[2] = 0x00 ;
	IC1.ch[3] = 0x00 ;
	*Address = IC1.i ;	

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(snr != CardSerno)   return(UserCard_Err);

	for(i=1; i<16; i++)
	{
		if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)			return (UserCard_Err);

		for(j=0; j<3; j++)
		{
			if(rf_read(icdev, (i*4)+j, data1) != MI_OK) 	return (ReadCard_Err);

			IC1.i = 0 ;
			IC1.ch[0] = data1[2] ;
			IC1.ch[1] = data1[1] ;
			IC1.ch[2] = data1[0] ;
			IC1.ch[3] = 0x00 ;
			RecDetailedData[((i-1)*3)+j].Card_Number = IC1.i ;

			IC1.i = 0 ;
			IC1.ch[0] = data1[5] ;
			IC1.ch[1] = data1[4] ;
			IC1.ch[2] = data1[3] ;
			IC1.ch[3] = 0x00 ;
			RecDetailedData[((i-1)*3)+j].Card_RestMoney = IC1.i ;

			IC1.i = 0 ;
			IC1.ch[0] = data1[7] ;
			IC1.ch[1] = data1[6] ;
			IC1.ch[2] = 0x00 ;
			IC1.ch[3] = 0x00 ;
			RecDetailedData[((i-1)*3)+j].Card_UsedMoney = IC1.i ;

			IC1.i = 0 ;
			IC1.ch[0] = data1[9] ;
			IC1.ch[1] = data1[8] ;
			IC1.ch[2] = 0x00 ;
			IC1.ch[3] = 0x00 ;
			RecDetailedData[((i-1)*3)+j].Card_UsedTimes = IC1.i ;
			
			IC1.i = 0 ;
			IC1.ch[0] = data1[11] ;
			IC1.ch[1] = data1[10] ;
			IC1.ch[2] = 0x00 ;
			IC1.ch[3] = 0x00 ;
			RecDetailedData[((i-1)*3)+j].Card_Address = IC1.i ;

			IC1.i = 0 ;
			IC1.ch[0] = data1[15] ;
			IC1.ch[1] = data1[14] ;
			IC1.ch[2] = data1[13] ;
			IC1.ch[3] = data1[12] ;

			buff[0] = (unsigned char)((IC1.i >> 0 ) & 0x0000003f) ;
			buff[1] = (unsigned char)((IC1.i >> 6 ) & 0x0000003f) ;
			buff[2] = (unsigned char)((IC1.i >> 12 ) & 0x0000001f) ;
			buff[3] = (unsigned char)((IC1.i >> 17 ) & 0x0000001f) ;
			buff[4] = (unsigned char)((IC1.i >> 22 ) & 0x0000000f) ;
			buff[5] = (unsigned char)((IC1.i >> 26 ) & 0x0000003f) ;

			RecDetailedData[((i-1)*3)+j].Card_Times[0] = buff[5]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[1] = buff[5]%10 + 0x30 ;	
			RecDetailedData[((i-1)*3)+j].Card_Times[2] = buff[4]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[3] = buff[4]%10 + 0x30 ;				
			RecDetailedData[((i-1)*3)+j].Card_Times[4] = buff[3]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[5] = buff[3]%10 + 0x30 ;	
			RecDetailedData[((i-1)*3)+j].Card_Times[6] = buff[2]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[7] = buff[2]%10 + 0x30 ;	
			RecDetailedData[((i-1)*3)+j].Card_Times[8] = buff[1]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[9] = buff[1]%10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[10] = buff[0]/10 + 0x30 ;
			RecDetailedData[((i-1)*3)+j].Card_Times[11] = buff[0]%10 + 0x30 ;				
		}
	}			
	if(	rf_halt(icdev)!=0)		return Reader_Err;			
	return (DAS_OK);
}

int __stdcall ReadAllRec(HANDLE icdev,RecordData* RecData,long& RecNum)
{
	unsigned long  snr, CardSerno;
	BYTE   BlockData[16] ;
	BYTE   count=0;
	int	   i, j;
	union ic
	{
		unsigned long data;
		BYTE     ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author);

	if(rf_load_key(icdev, KEYB|KEYSET0, 0, SystemCard_KeyB12) != MI_OK)	return (Reader_Err);
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	CardSerno = snr;

	if(rf_authentication(icdev,KEYB|KEYSET0,0) != MI_OK)	return (UserCard_Err);

	if(rf_read(icdev,1,BlockData) != MI_OK) 	return (ReadCard_Err);

    RecNum = BlockData[7];

	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	for(i=1;i<=(RecNum+2)/3;i++)
	{
		if(rf_load_key(icdev,KEYB|KEYSET0,i,SystemCard_KeyB12) != MI_OK)	return (Reader_Err);
	}

	for(i=1;i<=(RecNum+2)/3;i++)
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) 
		{
			if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
		}
		
		if(CardSerno != snr)  return UserCard_Err;

		if(rf_authentication(icdev,KEYB|KEYSET0,i)!=MI_OK)		return (UserCard_Err);

        if((RecNum+2)/3 > i )
		{
			for(j=0;j<3;j++)
			{
				if(rf_read(icdev,i*4+j,BlockData)!=MI_OK)		return(ReadCard_Err);

				if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);

				IC.data = 0;
				IC.ch[0] = BlockData[0];
				IC.ch[1] = BlockData[1];
				IC.ch[2] = BlockData[2];
				IC.ch[3] = BlockData[3];
				RecData[count].MachineNo = IC.data;
				IC.data = 0;
				IC.ch[0] = BlockData[4];
				IC.ch[1] = BlockData[5];
				IC.ch[2] = BlockData[6];
				IC.ch[3] = BlockData[7];
				RecData[count++].TotalBalance = IC.data;
			}
		}
		else
		{
			switch( RecNum % 3 )
			{
			case 1:
				if(rf_read(icdev,i*4,BlockData)!=MI_OK)
					return(ReadCard_Err);
				if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
				IC.data = 0;
				IC.ch[0] = BlockData[0];
				IC.ch[1] = BlockData[1];
				IC.ch[2] = BlockData[2];
				IC.ch[3] = BlockData[3];
				RecData[count].MachineNo = IC.data;
				IC.data = 0;
				IC.ch[0] = BlockData[4];
				IC.ch[1] = BlockData[5];
				IC.ch[2] = BlockData[6];
				IC.ch[3] = BlockData[7];
				RecData[count++].TotalBalance = IC.data;
				break;

			case 2:
				for(j=0;j<2;j++) 
				{
					if(rf_read(icdev,i*4+j,BlockData)!=MI_OK)
						return(ReadCard_Err);
					if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
					IC.data = 0;
					IC.ch[0] = BlockData[0];
					IC.ch[1] = BlockData[1];
					IC.ch[2] = BlockData[2];
					IC.ch[3] = BlockData[3];
					RecData[count].MachineNo = IC.data;
					IC.data = 0;
					IC.ch[0] = BlockData[4];
					IC.ch[1] = BlockData[5];
					IC.ch[2] = BlockData[6];
					IC.ch[3] = BlockData[7];
					RecData[count++].TotalBalance = IC.data;
				}
				break;

			case 0:
				for(j=0;j<3;j++)
				{
					if(rf_read(icdev,i*4+j,BlockData)!=MI_OK) 	 return(ReadCard_Err);
					if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
					IC.data = 0;
					IC.ch[0] = BlockData[0];
					IC.ch[1] = BlockData[1];
					IC.ch[2] = BlockData[2];
					IC.ch[3] = BlockData[3];
					RecData[count].MachineNo = IC.data;
					IC.data = 0;
					IC.ch[0] = BlockData[4];
					IC.ch[1] = BlockData[5];
					IC.ch[2] = BlockData[6];
					IC.ch[3] = BlockData[7];
					RecData[count++].TotalBalance = IC.data;
				}
				break;

			default: 
				break;
			}
		}
		if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
	}
	return (DAS_OK);
}

int __stdcall ReadRecbySec(HANDLE icdev,int SecNo,RecordData* RecData,long& RecNum)
{
	unsigned long snr;
	BYTE   BlockData[16] ;
	BYTE   count=0;
	union ic
	{
		unsigned long data;
		BYTE     ch[4];
	}IC;
	int j;
	int TotalRecNum;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;
	
	if(Jsxt_author!=1) return (No_JS_Author);

	if(rf_load_key(icdev,KEYB|KEYSET0,0,SystemCard_KeyB12) != MI_OK)	return (Reader_Err);

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}	
	
	if(rf_authentication(icdev,KEYB|KEYSET0,0) != MI_OK)		return (UserCard_Err);
	
	if(rf_read(icdev,1,BlockData) != MI_OK) 	return (ReadCard_Err);
	
    TotalRecNum = BlockData[7];

	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	if(rf_load_key(icdev,KEYB|KEYSET0,SecNo,SystemCard_KeyB12) != MI_OK)	return (Reader_Err);
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(rf_authentication(icdev,KEYB|KEYSET0,SecNo) != MI_OK)	return (UserCard_Err);
	
	if((TotalRecNum+2)/3 > SecNo )
	{
		for(j=0;j<3;j++)
		{
			if(rf_read(icdev,SecNo*4+j,BlockData)!=MI_OK)	return(ReadCard_Err);
			if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);

			IC.data = 0;
			IC.ch[0] = BlockData[0];
			IC.ch[1] = BlockData[1];
			IC.ch[2] = BlockData[2];
			IC.ch[3] = BlockData[3];
			RecData[count].MachineNo = IC.data;
			IC.data = 0;
			IC.ch[0] = BlockData[4];
			IC.ch[1] = BlockData[5];
			IC.ch[2] = BlockData[6];
			IC.ch[3] = BlockData[7];
			RecData[count++].TotalBalance = IC.data;
		}
		RecNum = 3;
	}
	else
	{
		switch( TotalRecNum % 3 )
		{
		case 1:
			if(rf_read(icdev,SecNo*4,BlockData)!=MI_OK) 	return(ReadCard_Err);
			if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
			IC.data = 0;
			IC.ch[0] = BlockData[0];
			IC.ch[1] = BlockData[1];
			IC.ch[2] = BlockData[2];
			IC.ch[3] = BlockData[3];
			RecData[count].MachineNo = IC.data;
			IC.data = 0;
			IC.ch[0] = BlockData[4];
			IC.ch[1] = BlockData[5];
			IC.ch[2] = BlockData[6];
			IC.ch[3] = BlockData[7];
			RecData[count++].TotalBalance = IC.data;
			RecNum = 1;
			break;

		case 2:
			for(j=0;j<2;j++)
			{
				if(rf_read(icdev,SecNo*4+j,BlockData)!=MI_OK)	return(ReadCard_Err);
				if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
				IC.data = 0;
				IC.ch[0] = BlockData[0];
				IC.ch[1] = BlockData[1];
				IC.ch[2] = BlockData[2];
				IC.ch[3] = BlockData[3];
				RecData[count].MachineNo = IC.data;
				IC.data = 0;
				IC.ch[0] = BlockData[4];
				IC.ch[1] = BlockData[5];
				IC.ch[2] = BlockData[6];
				IC.ch[3] = BlockData[7];
				RecData[count++].TotalBalance = IC.data;
			}
			RecNum = 2;
			break;

		case 0:
			for(j=0;j<3;j++)
			{
				if(rf_read(icdev,SecNo*4+j,BlockData)!=MI_OK)	return(ReadCard_Err);
				if(Check_Bcc(BlockData) != 0) return (ReadCard_Err);
				IC.data = 0;
				IC.ch[0] = BlockData[0];
				IC.ch[1] = BlockData[1];
				IC.ch[2] = BlockData[2];
				IC.ch[3] = BlockData[3];
				RecData[count].MachineNo = IC.data;
				IC.data = 0;
				IC.ch[0] = BlockData[4];
				IC.ch[1] = BlockData[5];
				IC.ch[2] = BlockData[6];
				IC.ch[3] = BlockData[7];
				RecData[count++].TotalBalance = IC.data;
			}
			RecNum = 3;
			break;
		default: break;
		}
	}
	if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
	return (DAS_OK);
}

int  __stdcall QueryJsCard(HANDLE icdev,LPINT CardType,LPINT OPT_Num,LPINT Serno,LPSTR Cardno,unsigned long * CardSerno,LPINT Value,LPINT Count, LPINT UserType,int WaitTime ,unsigned char * user_code_new)  //LPINT AddCount,
{
	unsigned long use_snr;
	unsigned char data1[16],data2[16], code_word[16] ;
	int usercard_sec;
	unsigned __int16 tt;
	int i,j;	

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(Jsxt_author!=1) return (No_SF_Author);

	if(WaitTime < 800) 		WaitTime = 800;
	
	if(Jsxt_author==1)  {usercard_sec = SInew.jsxt_sec;}
	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

 		if(rf_halt(icdev)) return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) == MI_OK)
		{
			if(rf_read(icdev,usercard_sec*4+0,data1))return ReadCard_Err;
			if(rf_read(icdev,usercard_sec*4+1,data2))return ReadCard_Err;
			if(rf_halt(icdev)!=0)	return Reader_Err;
			*CardType = 0;	
			*OPT_Num = 0;
			IC.i = 0;
			IC.ch[0] = data1[1];IC.ch[1] = data1[2];IC.ch[2] = data1[0];
			*Serno = IC.i;		
			for(i=0;i<6;i++)	
			{
				if(data1[i+10] !=0) break;
			}
			for(j=0;j<5-i;j++)
			{
				Cardno[j] = data1[14+j-(4-i)];
			}
			Cardno[j] = 0;
			*UserType = data1[3];
			*CardSerno = use_snr;

			IC.i = 0 ;IC.ch[0] = data2[0];IC.ch[1] = data2[1];IC.ch[2] = data2[2];
			*Value = IC.i;		
			IC.i = 0; IC.ch[0] = data2[11];IC.ch[1] = data2[12];
			*Count = IC.i;		
			return (DAS_OK);
		}

		if(rf_load_key(icdev,KEYB|KEYSET2,3,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_halt(icdev))return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;	
		if(rf_authentication(icdev,KEYB|KEYSET2,3) == MI_OK)
		{
			if(rf_read(icdev,3*4+0,data1) != MI_OK) return ReadCard_Err;
			if(rf_read(icdev,3*4+1,data2) != MI_OK) return ReadCard_Err;
		    if(rf_halt(icdev))return Reader_Err;
			if(memcmp(&data1[1],SInew.syscard_no_js,5) != 0) return (UserCard_Err);
			if(data1[0] == 3)
			{
				*CardType = 2 ;	
			}
			else return (UserCard_Err);

			*CardSerno = use_snr;
			return (DAS_OK);
		}	
		
		if(rf_load_key(icdev,KEYB|KEYSET2,1,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_halt(icdev))return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
		if(rf_authentication(icdev,KEYB|KEYSET2,1) == MI_OK)
		{
			if(rf_read(icdev,1*4+0,data1) != MI_OK) return ReadCard_Err;
			if(rf_read(icdev,1*4+1,data2) != MI_OK) return ReadCard_Err;
		    if(rf_halt(icdev))  return Reader_Err;

			if(memcmp(&data1[1],SInew.syscard_no_js,5) != 0) return (UserCard_Err);
			
			if(data1[0] == 4)
			{
				*CardType = 1 ;			
				*OPT_Num = (int)data2[0];
			}
			else if(data1[0] == 0x0D)
			{
				*CardType = 6 ;			
			}
			else if(data1[0] == 0x0A)
			{
				*CardType = 7 ;		
			}
			else return (UserCard_Err);

			*CardSerno = use_snr;	
			return (DAS_OK);
		}

		if(rf_halt(icdev))return Reader_Err;
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;

		IC1.i = use_snr ;
		
		if(user_code_new[8] == 0x01)					//1为不加密方式		
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,def_keyb1) != MI_OK) return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,def_keyb2) != MI_OK) return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,LockCard_KeyB) != MI_OK)   return (Reader_Err);//装载第0套密码
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,LockCard_KeyB) != MI_OK)  return (Reader_Err);
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			cala_key12_new(IC1.ch,code_word,user_code_new);
			if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
			if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		}
		else
		{
			return(Para_Err);
		}
		
		if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
			if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
			{
				if(rf_halt(icdev)!=MI_OK) return Reader_Err;  
				if(rf_card_2(icdev, ALL, &use_snr, &tt) != MI_OK) return (No_Card);					
				if(rf_authentication(icdev, KEYB|KEYSET0, usercard_sec) != MI_OK)	return (Not_Identify);
				else	*CardType  = 4;										
			}	
			else 	*CardType  = 4;		
		}
		else 	*CardType  = 4;	
			  
		*CardSerno = use_snr;
		if(rf_halt(icdev))return Reader_Err;
		return (DAS_OK);
	}
    return Not_Identify;
}

int __stdcall RSTJsUserCard(HANDLE icdev,unsigned int CardSerno ,unsigned char *user_code_new)
{
	unsigned long use_snr;
	unsigned char data1[16] ,data3[16];
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned char data6[16] = {0xf1,0xcf,0xd3,0xed,0xf9,0x58,0xFF,0x07,0x80,0x69,0xdd,0xea,0xfc,0xa0,0xbc,0xd1};
	unsigned char userkeyb[6] ,code_word[16];
	int usercard_sec;
	unsigned __int16 tt;	

	memset(data1,0xff,16); 
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}
		
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else
		return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author);	

	if(Jsxt_author == 1){usercard_sec = SInew.jsxt_sec;}

	memcpy(userkeyb,SInew.sys_keyb,6);	
	
	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if(use_snr != CardSerno) return (UserCard_Err);
	
	IC1.i = use_snr;

	cala_key12_new(IC1.ch,code_word,user_code_new);

	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,userkeyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,&code_word[10]) != MI_OK) return (Reader_Err);

	if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	
	{
		if(rf_authentication(icdev,KEYB|KEYSET1,usercard_sec) != MI_OK)
		{
			return (UserCard_Err);
		}
	}

	if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		if(rf_write(icdev,usercard_sec*4+1,data1) == MI_OK)
			if(rf_write(icdev,usercard_sec*4+2,data1) == MI_OK)
				if(user_code_new[8] == 0x01)					//1为不加密方式		
				{
					if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)	
					{
						if(	rf_halt(icdev)!=0)return Reader_Err;						
						return (DAS_OK);
					}
				}
				else  if(user_code_new[8] == 0x02)				//2为统一加密方式
				{
					if(rf_write(icdev,usercard_sec*4+3,data3) == MI_OK)	 
					{
						if(	rf_halt(icdev)!=0)return Reader_Err;						
						return (DAS_OK);
					}
				}
				else  if(user_code_new[8] == 0x03)				//3为动态加密方式
				{
					if(rf_write(icdev,usercard_sec*4+3,code_word) == MI_OK) 
					{
						if(	rf_halt(icdev)!=0)return Reader_Err;						
						return (DAS_OK);
					}
				}
				else
				{
					return(Para_Err);
				}
	return (WriteCard_Err);
}

int __stdcall ClearRecNum(HANDLE icdev)
{
	unsigned long snr;
	BYTE   BlockData[16] ;
	BYTE   count=0;
	int    i;
	BYTE bcc = 0;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if((Jsxt_author!=1)&&(Sfxt_author!=1) )return (No_JS_Author);

	if(rf_load_key(icdev,KEYB|KEYSET0,0,SystemCard_KeyB12) != MI_OK) 		return (Reader_Err);
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	if(rf_authentication(icdev,KEYB|KEYSET0,0) != MI_OK)		return (UserCard_Err);

	if(rf_read(icdev, 1, BlockData) != MI_OK) 		return (ReadCard_Err);

    BlockData[7] = 0;

	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev, 1, BlockData) != MI_OK) 		return(WriteCard_Err);

	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return (DAS_OK);
}

int __stdcall RSTCollectCard(HANDLE icdev,unsigned int CardSerno ,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char i, data3[16], BlockData[16];
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned char data6[16] = {0xf1,0xcf,0xd3,0xed,0xf9,0x58,0xFF,0x07,0x80,0x69,0xdd,0xea,0xfc,0xa0,0xbc,0xd1};
	unsigned char code_word[16];
	int   usercard_sec;

	memset(BlockData,0x00,16);   

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
	
	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else	return No_Lic_Err;

	if(Jsxt_author!=1) return (No_JS_Author);

	for(i=0;i<16;i++)
	{
		if(rf_load_key(icdev,KEYB|KEYSET0, i, SystemCard_KeyB12) != MI_OK) 
		{
			rf_halt(icdev);
			return (Reader_Err);
		}
	}

	for(i=0;i<16;i++)
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) 
		{
			if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
		}

		if(snr != CardSerno) return (UserCard_Err);	

		IC1.i = snr;
		cala_key12_new(IC1.ch,code_word,user_code_new);

		if(rf_authentication(icdev,KEYB|KEYSET0,i)!=MI_OK)
		{		
			rf_halt(icdev);
			return(UserCard_Err);
		}
		if(i!=0)
		{
			if(rf_write(icdev,i*4,BlockData) != 0)	
			{
				rf_halt(icdev);
				return(WriteCard_Err);
			}
		}
		if(rf_write(icdev,i*4+1,BlockData) != 0)
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
		if(rf_write(icdev,i*4+2,BlockData) != 0)
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}

		usercard_sec = i ;

		if(user_code_new[8] == 0x01)
		{
			if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	
			{
				rf_halt(icdev);
				return(WriteCard_Err);
			}
		}
		else  if(user_code_new[8] == 0x02)				//2为统一加密方式
		{
			if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	 
			{
				rf_halt(icdev);
				return(WriteCard_Err);
			}
		}
		else  if(user_code_new[8] == 0x03)				//3为动态加密方式
		{
			if(rf_write(icdev,usercard_sec*4+3,code_word) != MI_OK) 
			{
				rf_halt(icdev);
				return(WriteCard_Err);
			}
		}
		else
		{
			return(Para_Err);
		}

		if(rf_halt(icdev)!=0) 
		{
			return WriteCard_Err;  
		}
	}
	return(DAS_OK);
}

int __stdcall RSTJSSubSysCard(HANDLE icdev ,unsigned char *user_code_new)
{
	unsigned long  snr=0;
	unsigned char  usercard_sec, BlockData[16],code_word[16];
	unsigned char data3[16];
	unsigned char data5[16] = {0xff,0xff,0xff,0xff,0xff,0xff,0xFF,0x07,0x80,0x69,0xff,0xff,0xff,0xff,0xff,0xff};	
	unsigned char data6[16] = {0xf1,0xcf,0xd3,0xed,0xf9,0x58,0xFF,0x07,0x80,0x69,0xdd,0xea,0xfc,0xa0,0xbc,0xd1};
	BYTE def_ControlWord[4] = {0xff,0x07,0x80,0x69};
	BYTE def_ControlWord12[4] = {0xff,0x07,0x80,0xDA};
	unsigned char def_keyA1[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	memset(BlockData,0,16);
	
	if(rf_load_key(icdev,KEYB|KEYSET0,1,SystemCard_KeyB12) != MI_OK) 
	{
		rf_halt(icdev);
		return (Reader_Err);
	}
	if(rf_load_key(icdev,KEYB|KEYSET0,2,SystemCard_KeyB12) != MI_OK) 
	{
		rf_halt(icdev);
		return (Reader_Err);
	}

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}
	
	if(rf_authentication(icdev,KEYB|KEYSET0,1)!=MI_OK)
	{			
		rf_halt(icdev);
		return(UserCard_Err);
	}
	
	if(rf_write(icdev,1*4,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}
	
	if(rf_write(icdev,1*4+1,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}
	
	if(rf_write(icdev,1*4+2,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}

	usercard_sec = 1 ;
	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}
	else if(user_code_new[8] == 0x03)
	{
		IC1.i = snr;
		cala_key12_new(IC1.ch,code_word,user_code_new);
	}
	else
	{
		return(Para_Err);
	}

	if(user_code_new[8] == 0x01)			
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	 
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,code_word) != MI_OK) 
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else
	{
		return(Para_Err);
	}

	if(rf_halt(icdev)!=0) 
	{
		return WriteCard_Err;  
	}

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}
	
	if(rf_authentication(icdev,KEYB|KEYSET0,2)!=MI_OK)
	{		
		rf_halt(icdev);
		return(UserCard_Err);
	}
	
	if(rf_write(icdev,2*4,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}
	
	if(rf_write(icdev,2*4+1,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}
	
	if(rf_write(icdev,2*4+2,BlockData) != 0) 
	{
		rf_halt(icdev);
		return(WriteCard_Err);
	}

	usercard_sec = 2 ;
	if(user_code_new[8] == 0x01)						//1为不加密方式		
	{
		memcpy(data3, data5, 16);
	}
	else  if(user_code_new[8] == 0x02)					//2为统一加密方式		
	{
		memcpy(data3, data6, 16);
	}
	else if(user_code_new[8] == 0x03)
	{
		IC1.i = snr;
		cala_key12_new(IC1.ch,code_word,user_code_new);
	}
	else
	{
		return(Para_Err);
	}

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,data3) != MI_OK)	 
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		if(rf_write(icdev,usercard_sec*4+3,code_word) != MI_OK) 
		{
			rf_halt(icdev);
			return(WriteCard_Err);
		}
	}
	else
	{
		return(Para_Err);
	}

	if(rf_halt(icdev)!=0) 
	{
		return WriteCard_Err;  
	}

	return 0;
}

int __stdcall MakeJSSubSysCard(HANDLE icdev, JS_FEE_RATE& fee_rate, JS_AdvanPara&  AdvanPara ,int Addr, unsigned char *user_code_new)
{
	return 0;
}
int __stdcall MakeJSSubSysCard12(HANDLE icdev, JS_FEE_RATE& fee_rate, JS_AdvanPara&  AdvanPara ,int Addr, unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char bcc = 0;
	int i;	
	unsigned char  code_word[16];
	BYTE  BlockData[16];

	int usercard_sec;

	union
	{
		long   val;
		BYTE  ch[4];
	}data;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
		
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else
	{
		return No_Lic_Err;
	}

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return No_Card;
	}

	usercard_sec = 1;
	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x0A;
	memcpy(BlockData+1,SInew.syscard_no_js,5);
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4,BlockData) != MI_OK) 		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
 	BlockData[1] = fee_rate.ConsumeMode;
 	BlockData[2] = fee_rate.MinFeeUnit;
	data.val = fee_rate.Rate1;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate2;
	memcpy(BlockData+6,data.ch,3);
 	data.val = fee_rate.Rate3;
 	memcpy(BlockData+9,data.ch,3);
 	data.val = fee_rate.Rate4;
 	memcpy(BlockData+12,data.ch,3);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+1,BlockData) != MI_OK)		return(WriteCard_Err);	

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
    memset(&BlockData[1],0x0,4);
	data.val = fee_rate.Rate5;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate6;
	memcpy(BlockData+6,data.ch,3);
	data.val = fee_rate.Rate7;
	memcpy(BlockData+9,data.ch,3);
	data.val = fee_rate.Rate8;
	memcpy(BlockData+12,data.ch,3);
	
	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+2,BlockData) != MI_OK)		return(WriteCard_Err);	

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,1*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 2;

	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
    memset(&BlockData[1],0x77,4);
	IC.i = Addr;
    BlockData[5] = IC.ch[3];
	BlockData[6] = IC.ch[2];
	BlockData[7] = IC.ch[1];
	BlockData[8] = IC.ch[0];

    BlockData[9] = IC.ch[3];
	BlockData[10] = IC.ch[2];
	BlockData[11] = IC.ch[1];
	BlockData[12] = IC.ch[0];

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+0,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE;							//卡类型
	data.val = AdvanPara.Max_Consume_Times;			//为当天最大消费次数。 两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Max_Consume_Money;			//为当天最大消费额（元）。两个字节
	memcpy(BlockData+3,data.ch,2);
	data.val = AdvanPara.Free_Time;					//当天免费时间（时，分，秒） 三个字节
	memcpy(BlockData+5,data.ch,3);

    BlockData[8] = AdvanPara.Grade_Rate1;
	BlockData[9] = AdvanPara.Grade_Rate2;
	BlockData[10] = AdvanPara.Grade_Rate3;

	data.val = AdvanPara.One_Grade;					//为一阶限制值
	memcpy(BlockData+11,data.ch,2);
	data.val = AdvanPara.Two_Grade;					//为二阶限制值（大于二阶全部为三阶）
	memcpy(BlockData+13,data.ch,2);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+1,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE;										//卡类型
	data.val = AdvanPara.Consume_Interval;						//消费时间间隔 （秒）  两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Once_Max_Consume_Money;				//为单次的最大消费额。 两个字节
	memcpy(BlockData+3,data.ch,2);
	BlockData[5] = AdvanPara.En_Card_Type ;
	BlockData[6] = AdvanPara.Used_OneCard_Mode ;
	BlockData[7] = AdvanPara.En_Auto_Close ;
	BlockData[8] = AdvanPara.User_Card_Flag ;
	BlockData[9]  = AdvanPara.Auto_Write_Record_Flag ;
	BlockData[10] = AdvanPara.Auto_Lock_Used_Card ;
	BlockData[11] = AdvanPara.Used_Card_Overdraft_Flag ;
	BlockData[12] = AdvanPara.Used_Card_Overdraft_Flag2 ;
	BlockData[13] = AdvanPara.GK_Cala_Key ;
	bcc = 0;

	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+2,BlockData) != MI_OK)		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,2*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return(DAS_OK);
}

int __stdcall MakeJSSubSysCard12_New(HANDLE icdev, JS_FEE_RATE& fee_rate, JS_AdvanPara&  AdvanPara ,int Addr, unsigned char temperature,unsigned char clearrateflag,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char bcc = 0;
	int i;	
	unsigned char  code_word[16];
	BYTE  BlockData[16];

	int usercard_sec;

	union
	{
		long   val;
		BYTE  ch[4];
	}data;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
		
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else
	{
		return No_Lic_Err;
	}

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return No_Card;
	}

	usercard_sec = 1;
	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x0A;
	memcpy(BlockData+1,SInew.syscard_no_js,5);
	BlockData[6] = temperature;//保存温控限制值
	BlockData[7] = clearrateflag;//保存计量模式转计时模式费率控制位，为零，则费率为零，不为零，则费率按相应的算法计算
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4,BlockData) != MI_OK) 		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
 	BlockData[1] = fee_rate.ConsumeMode;
 	BlockData[2] = fee_rate.MinFeeUnit;
	data.val = fee_rate.Rate1;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate2;
	memcpy(BlockData+6,data.ch,3);
 	data.val = fee_rate.Rate3;
 	memcpy(BlockData+9,data.ch,3);
 	data.val = fee_rate.Rate4;
 	memcpy(BlockData+12,data.ch,3);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+1,BlockData) != MI_OK)		return(WriteCard_Err);	

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
    memset(&BlockData[1],0x0,4);
	data.val = fee_rate.Rate5;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate6;
	memcpy(BlockData+6,data.ch,3);
	data.val = fee_rate.Rate7;
	memcpy(BlockData+9,data.ch,3);
	data.val = fee_rate.Rate8;
	memcpy(BlockData+12,data.ch,3);
	
	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+2,BlockData) != MI_OK)		return(WriteCard_Err);	

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,1*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 2;

	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
    memset(&BlockData[1],0x77,4);
	IC.i = Addr;
    BlockData[5] = IC.ch[3];
	BlockData[6] = IC.ch[2];
	BlockData[7] = IC.ch[1];
	BlockData[8] = IC.ch[0];

    BlockData[9] = IC.ch[3];
	BlockData[10] = IC.ch[2];
	BlockData[11] = IC.ch[1];
	BlockData[12] = IC.ch[0];

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+0,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE;							//卡类型
	data.val = AdvanPara.Max_Consume_Times;			//为当天最大消费次数。 两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Max_Consume_Money;			//为当天最大消费额（元）。两个字节
	memcpy(BlockData+3,data.ch,2);
	data.val = AdvanPara.Free_Time;					//当天免费时间（时，分，秒） 三个字节
	memcpy(BlockData+5,data.ch,3);

    BlockData[8] = AdvanPara.Grade_Rate1;
	BlockData[9] = AdvanPara.Grade_Rate2;
	BlockData[10] = AdvanPara.Grade_Rate3;

	data.val = AdvanPara.One_Grade;					//为一阶限制值
	memcpy(BlockData+11,data.ch,2);
	data.val = AdvanPara.Two_Grade;					//为二阶限制值（大于二阶全部为三阶）
	memcpy(BlockData+13,data.ch,2);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+1,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE;										//卡类型
	data.val = AdvanPara.Consume_Interval;						//消费时间间隔 （秒）  两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Once_Max_Consume_Money;				//为单次的最大消费额。 两个字节
	memcpy(BlockData+3,data.ch,2);
	BlockData[5] = AdvanPara.En_Card_Type ;
	BlockData[6] = AdvanPara.Used_OneCard_Mode ;
	BlockData[7] = AdvanPara.En_Auto_Close ;
	BlockData[8] = AdvanPara.User_Card_Flag ;
	BlockData[9]  = AdvanPara.Auto_Write_Record_Flag ;
	BlockData[10] = AdvanPara.Auto_Lock_Used_Card ;
	BlockData[11] = AdvanPara.Used_Card_Overdraft_Flag ;
	BlockData[12] = AdvanPara.Used_Card_Overdraft_Flag2 ;
	BlockData[13] = AdvanPara.GK_Cala_Key ;
	bcc = 0;

	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+2,BlockData) != MI_OK)		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,2*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return(DAS_OK);
}

int __stdcall MakeJSSubSysCard12_Integrative(HANDLE icdev, JS_FEE_RATE & fee_rate, JS_AdvanPara &  AdvanPara ,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char code_word[16];
	unsigned char bcc = 0;
	int i;	
	BYTE  BlockData[16];
	int usercard_sec;

	union
	{
		long   val;
		BYTE  ch[4];
	}data;
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;
		
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else		return No_Lic_Err;
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 1;

	IC1.i = snr;
	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x0A;
	memcpy(BlockData+1,SInew.syscard_no_js,5);
	for(i=0;i<15;i++)		bcc += BlockData[i];

	BlockData[15] = bcc;
	if(rf_write(icdev,1*4,BlockData) != MI_OK) 		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
	BlockData[1] = (BYTE)fee_rate.ConsumeMode;
	BlockData[2] = (BYTE)fee_rate.MinFeeUnit;
	data.val = fee_rate.Rate1;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate2;
	memcpy(BlockData+6,data.ch,3);
	data.val = fee_rate.Rate3;
	memcpy(BlockData+9,data.ch,3);
	data.val = fee_rate.Rate4;
	memcpy(BlockData+12,data.ch,3);
	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+1,BlockData) != MI_OK)		return(WriteCard_Err);	

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;
    memset(&BlockData[1],0x0,4);
	data.val = fee_rate.Rate5;
	memcpy(BlockData+3,data.ch,3);
	data.val = fee_rate.Rate6;
	memcpy(BlockData+6,data.ch,3);
	data.val = fee_rate.Rate7;
	memcpy(BlockData+9,data.ch,3);
	data.val = fee_rate.Rate8;
	memcpy(BlockData+12,data.ch,3);
	bcc = 0;

	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;

	if(rf_write(icdev,1*4+2,BlockData) != MI_OK)	return(WriteCard_Err);	

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,1*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 2;

	IC1.i = snr;
	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0xAA;

	bcc = 0;
	for(i=0;i<15;i++)			bcc += BlockData[i];
	BlockData[15] = bcc;

	if(rf_write(icdev,2*4+0,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE; 
	data.val = AdvanPara.Max_Consume_Times;			//为当天最大消费次数。 两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Max_Consume_Money;			//为当天最大消费额（元）。两个字节
	memcpy(BlockData+3,data.ch,2);
	data.val = AdvanPara.Free_Time;					//当天免费时间（时，分，秒） 三个字节
	memcpy(BlockData+5,data.ch,3);

    BlockData[8] = AdvanPara.Grade_Rate1;
	BlockData[9] = AdvanPara.Grade_Rate2;
	BlockData[10] = AdvanPara.Grade_Rate3;

	data.val = AdvanPara.One_Grade;					//为一阶限制值
	memcpy(BlockData+11,data.ch,2);
	data.val = AdvanPara.Two_Grade;					//为二阶限制值（大于二阶全部为三阶）
	memcpy(BlockData+13,data.ch,2);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+1,BlockData) != MI_OK)		return(WriteCard_Err);

	memset(BlockData,0,16);
	BlockData[0] = 0xAE; 
	data.val = AdvanPara.Consume_Interval;				//消费时间间隔 （秒）  两个字节
	memcpy(BlockData+1,data.ch,2);
	data.val = AdvanPara.Once_Max_Consume_Money;		//为单次的最大消费额。 两个字节
	memcpy(BlockData+3,data.ch,2);

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,2*4+2,BlockData) != MI_OK)		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);
	if(rf_write(icdev,2*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return(DAS_OK);		
}

int __stdcall Set_Js_AddreCard(HANDLE icdev, int Addr ,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char bcc = 0;
	int i;	
	BYTE  BlockData[16];

	unsigned char code_word[16];
	int usercard_sec;
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;


	
	if(Addr<=0 )  	return (Para_Err);
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else		return No_Lic_Err;
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 1;

	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x3A;
    memset(&BlockData[1],0x77,4);

	IC.i = Addr;
    BlockData[5] = IC.ch[3];
	BlockData[6] = IC.ch[2];
	BlockData[7] = IC.ch[1];
	BlockData[8] = IC.ch[0];

    BlockData[9] = IC.ch[3];
	BlockData[10] = IC.ch[2];
	BlockData[11] = IC.ch[1];
	BlockData[12] = IC.ch[0];

	bcc = 0;
	for(i=0;i<15;i++)	bcc += BlockData[i];

	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+0,BlockData) != MI_OK)	return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,1*4+3,BlockData) != MI_OK)		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return(DAS_OK);
}

int __stdcall Set_Js_Overdraft_Card(HANDLE icdev, unsigned long Overdraft_Money ,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char bcc = 0;
	int i;	
	BYTE  BlockData[16];

	unsigned char code_word[16];
	int usercard_sec;
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else		return No_Lic_Err;
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 1;

	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x5A;

    memset(&BlockData[1],0x66,4);

	IC.i = Overdraft_Money;
    BlockData[10] = IC.ch[3];
	BlockData[11] = IC.ch[2];
	BlockData[12] = IC.ch[1];
	BlockData[13] = IC.ch[0];

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;

	if(rf_write(icdev,1*4+0,BlockData) != MI_OK)		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,1*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  
	return(DAS_OK);
}

int __stdcall Set_Js_DateTimeCard(HANDLE icdev, char *js_date, char *js_time ,unsigned char *user_code_new)
{
	unsigned long snr;
	unsigned char bcc = 0;
	int  i,usercard_sec;
	BYTE  BlockData[16];
	unsigned char TempStr[12];
	unsigned char code_word[16];
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(strlen(js_date)>12 && strlen(js_time)>12 )  return Para_Err;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else	return No_Lic_Err;

	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 1;

	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memcpy(TempStr,js_date,6);
	NumberStr2BCD(TempStr,6);
	memset(BlockData,0,16);
	BlockData[0] = 0x4A;
    memset(&BlockData[1],0x88,4);
	memcpy(&BlockData[5],TempStr,3); 

	memcpy(TempStr,js_time,6);
	NumberStr2BCD(TempStr,6);
	memcpy(&BlockData[8],TempStr,3); 

	bcc = 0;
	for(i=0;i<15;i++)		bcc += BlockData[i];
	BlockData[15] = bcc;

	if(rf_write(icdev,1*4+0,BlockData) != MI_OK)		return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,1*4+3,BlockData) != MI_OK)		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK) return WriteCard_Err;  

	return(DAS_OK);
}

int __stdcall Set_Js_QueryCard(HANDLE icdev ,unsigned char *user_code_new)
{

	unsigned long snr;
	unsigned char bcc = 0;
	int i;	
	BYTE  BlockData[16];

	unsigned char code_word[16];
	int usercard_sec;
	
	union ic1
	{
		unsigned	long i;
		unsigned char ch[4];
	}IC1;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else	return No_Lic_Err;
	
	if(rf_card(icdev,ALL,&snr) != MI_OK) 
	{
		if(rf_card(icdev,ALL,&snr) != MI_OK) return No_Card;
	}

	usercard_sec = 1;
	IC1.i = snr;

	if(user_code_new[8] == 0x01)					//1为不加密方式		
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK) return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x02)				//2为统一加密方式
	{
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,UserCard1_KeyA) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)   return (Reader_Err);//装载第0套密码
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else  if(user_code_new[8] == 0x03)				//3为动态加密方式
	{
		cala_key12_new(IC1.ch,code_word,user_code_new);
		if(rf_load_key(icdev,KEYA|KEYSET0,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,usercard_sec,code_word) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET0,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
		if(rf_load_key(icdev,KEYB|KEYSET1,usercard_sec,SystemCard_KeyB12) != MI_OK)  return (Reader_Err);
	}
	else
	{
		return(Para_Err);
	}
	
	if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
	if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);			
	if(rf_authentication(icdev,KEYA|KEYSET0,usercard_sec) != MI_OK)
	{
		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
		if(rf_authentication(icdev,KEYA|KEYSET1,usercard_sec) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);				
			if(rf_authentication(icdev,KEYB|KEYSET0,usercard_sec) != MI_OK)	return (Not_Identify);
		}
	}

	memset(BlockData,0,16);
	BlockData[0] = 0x2A;  
    memset(&BlockData[1],0x88,4);

	bcc = 0;
	for(i=0;i<15;i++)	bcc += BlockData[i];
	BlockData[15] = bcc;
	if(rf_write(icdev,1*4+0,BlockData) != MI_OK)	return(WriteCard_Err);

	memcpy(BlockData,SystemCard_KeyA12,6);
	memcpy(BlockData+6,ControlWord12,4);
    memcpy(BlockData+10,SystemCard_KeyB12,6);

	if(rf_write(icdev,1*4+3,BlockData) != MI_OK) 		return(WriteCard_Err);
	if(rf_halt(icdev)!=MI_OK)	return WriteCard_Err;  

	return(DAS_OK);

}

int __stdcall WRT_UserCard_Subsidize_Time(HANDLE icdev, int Systype, unsigned long useterm, unsigned int CardSerno, int WaitTime)
{
	unsigned long use_snr;
	unsigned char data0[16];
	int usercard_sec;
	unsigned char bcc,year,mounth,day;
	int i;
	unsigned __int16 tt;
	
	year   = (unsigned char)((useterm/10000)%1000);
	mounth = (unsigned char)((useterm%10000)/100);
	day    = (unsigned char)(useterm%100);

	if((mounth>12&&mounth<0)||(day>31&&day<0)||(year>127&&year<0)||(WaitTime < 0)) return Para_Err;
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno) return (UserCard_Err);

		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data0) != MI_OK) return (ReadCard_Err);

		data0[10] =	year%2000;
		data0[11] = mounth;

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data0[i];
		}	
		data0[15] = bcc;

		if(rf_write(icdev,usercard_sec*4,data0) == MI_OK)
		{
			if(rf_halt(icdev)!=0)return Reader_Err;			
			return (DAS_OK);
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall Write_UP_Warrant(HANDLE icdev, unsigned char *Warrant_Data, unsigned long CardSerno)
{
	unsigned long snr;
	unsigned __int16 tt;
	int i;	

	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
	}

	if(CardSerno != snr)   return Reader_Err; 

	for(i=1; i<16; i++)
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
		}

		if(CardSerno != snr)   return Reader_Err; 

		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);	

		if(rf_load_key(icdev,KEYA|KEYSET0,i,def_keya1) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET1,i,def_keya2) != MI_OK) return (Reader_Err);
		if(rf_load_key(icdev,KEYA|KEYSET2,i,def_keyA3) != MI_OK) return (Reader_Err);

		if(rf_authentication(icdev,KEYA|KEYSET0,i) != MI_OK)
		{
			if(rf_halt(icdev)!=MI_OK)	return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);					
			if(rf_authentication(icdev,KEYA|KEYSET1,i) != MI_OK)  
			{
				if(rf_halt(icdev)!=MI_OK)	return Reader_Err;  
				if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);	
				if(rf_authentication(icdev,KEYA|KEYSET2,i) != MI_OK)  return (Not_Identify);
			}
		}	

		if(rf_write(icdev,i*4+0,&Warrant_Data[((i-1)*3+0)*16]) != MI_OK) 	return(WriteCard_Err);
		if(rf_write(icdev,i*4+1,&Warrant_Data[((i-1)*3+1)*16]) != MI_OK) 	return(WriteCard_Err);		
		if(rf_write(icdev,i*4+2,&Warrant_Data[((i-1)*3+2)*16]) != MI_OK) 	return(WriteCard_Err);

		if(rf_write(icdev,i*4+3,UP_Warrant_Key1) != MI_OK) 	return(WriteCard_Err);

		if(rf_halt(icdev)!=MI_OK) 		return WriteCard_Err;  
	}
	return(DAS_OK);
}

int __stdcall Read_UP_Warrant(HANDLE icdev, unsigned char *Warrant_Data, unsigned long CardSerno)
{
	unsigned long snr;
	unsigned __int16 tt;
	int i;		

	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
	}

	if(CardSerno != snr)   return Reader_Err; 

	for(i=1; i<16; i++)
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
		}

		if(CardSerno != snr) return Reader_Err; 

		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);	
		if(rf_load_key(icdev,KEYA|KEYSET0, i, def_keyA3) != MI_OK)  return (Reader_Err);
		if(rf_authentication(icdev,KEYA|KEYSET0,i) != MI_OK)	return (Not_Identify);

		if(rf_read(icdev,((i*4)+0),&Warrant_Data[((i-1)*3+0)*16]) != MI_OK) 	return(WriteCard_Err);
		if(rf_read(icdev,((i*4)+1),&Warrant_Data[((i-1)*3+1)*16]) != MI_OK) 	 return(WriteCard_Err);	
		if(rf_read(icdev,((i*4)+2),&Warrant_Data[((i-1)*3+2)*16]) != MI_OK) 	  return(WriteCard_Err);
	}
	return(DAS_OK);	
}

int __stdcall RST_UP_Warrant(HANDLE icdev, unsigned long CardSerno)
{
	unsigned long snr;
	unsigned __int16 tt;
	int i;		

	if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
	}

	if(CardSerno != snr)   return Reader_Err; 

	for(i=1; i<16; i++)
	{
		if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) 
		{
			if(rf_card_2(icdev,ALL,&snr,&tt) != MI_OK) return No_Card;
		}

		if(CardSerno != snr) return Reader_Err; 

		if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
		if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);	
		if(rf_load_key(icdev,KEYA|KEYSET0, i, def_keyA3) != MI_OK)  return (Reader_Err);
		if(rf_authentication(icdev, KEYA|KEYSET0, i) != MI_OK)	return (Not_Identify);

		if(rf_write(icdev,((i*4)+3), UP_Warrant_Key2) != MI_OK) 	
		{
			if(rf_halt(icdev)!=MI_OK)  return Reader_Err;  
			if(rf_card(icdev,ALL,&snr) != MI_OK)	return (No_Card);	
			if(rf_load_key(icdev, KEYB|KEYSET0, i, def_keyB3) != MI_OK)  return (Reader_Err);
			if(rf_authentication(icdev, KEYB|KEYSET0, i) != MI_OK)	return (Not_Identify);
			if(rf_write(icdev,((i*4)+3), UP_Warrant_Key2) != MI_OK) 	 return(WriteCard_Err);
		}
	}
	return(DAS_OK);	
}

int __stdcall WRT_Pos_Virement(HANDLE icdev, unsigned long Value, unsigned int CardSerno, unsigned long flag, int WaitTime)
{
	unsigned long Blance1, Blance2, i, use_snr;
	unsigned char numb, usercard_sec_xf, usercard_sec_js, data0[16], data1[16] ;

	unsigned char bcc;
	unsigned __int16 tt;

	union ic
	{
		unsigned long i;
		unsigned char ch[4];
	}IC;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if(Sfxt_author!=1)  return (No_SF_Author);
    if(Jsxt_author!=1)  return (No_JS_Author);

	usercard_sec_xf = SInew.xfxt_sec;
	usercard_sec_js = SInew.jsxt_sec;

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
 	if(use_snr != CardSerno) return (UserCard_Err);

	numb = 10 ;

//	while(numb--)		
	{
		if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_xf,SInew.sys_keyb) != MI_OK) return (Reader_Err);		
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_xf) != MI_OK)	return (UserCard_Err);
        if(rf_read(icdev,usercard_sec_xf*4 +1,data0) != MI_OK) return (ReadCard_Err);

 		if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_js,SInew.sys_keyb) != MI_OK) return (Reader_Err);
 		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_js) != MI_OK)	return (UserCard_Err);
        if(rf_read(icdev,usercard_sec_js*4 +1,data1) != MI_OK) return (ReadCard_Err);

		IC.i = 0 ; 
		IC.ch[0] = data0[0] ;
		IC.ch[1] = data0[1] ;
		IC.ch[2] = data0[2] ;
		Blance1 = IC.i  ;

		IC.i = 0 ; 
		IC.ch[0] = data1[0] ;
		IC.ch[1] = data1[1] ;
		IC.ch[2] = data1[2] ;
		Blance2 = IC.i  ;

		if(flag)   
		{
			if(Blance1 < Value)  return(Para_Err);
			Blance1 = Blance1 - Value ;       
			Blance2 = Blance2 + Value ;
		}
		else  
		{
			if(Blance2 < Value)  return(Para_Err);
			Blance1 = Blance1 + Value ;       
			Blance2 = Blance2 - Value ;
		}
				
		IC.i = 0 ; 
		IC.i = Blance1; 
		data0[0] = IC.ch[0];
		data0[1] = IC.ch[1];
		data0[2] = IC.ch[2];  

		IC.i = 0 ; 
		IC.i = Blance2; 
		data1[0] = IC.ch[0];
		data1[1] = IC.ch[1];
		data1[2] = IC.ch[2];  

		bcc = 0;
		for(i=0;i<15;i++)	 bcc +=data0[i];
		data0[15] = bcc;

		bcc = 0;
		for(i=0;i<15;i++)	 bcc +=data1[i];
		data1[15] = bcc;

		while(numb--)
		{
			if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_xf,SInew.sys_keyb) != MI_OK)  return (WriteCard_Err);		
			if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_xf) != MI_OK)	return (WriteCard_Err);
			if(rf_write(icdev,usercard_sec_xf*4 +1,data0) != MI_OK)  continue;
			
 			if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_js,SInew.sys_keyb) != MI_OK)  return (WriteCard_Err);
 			if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_js) != MI_OK)	return (WriteCard_Err);
			if(rf_write(icdev,usercard_sec_js*4 +1,data1) != MI_OK)  continue;

			if(rf_halt(icdev)!=0)   return Reader_Err;
			return(DAS_OK);
		}
		return (WriteCard_Err);
	}
	return (UserCard_Err);
}

int __stdcall Read_Pos_Balance(HANDLE icdev, unsigned long *Balance_XF,  unsigned long *Balance_JS, unsigned long CardSerno, int WaitTime)
{
	unsigned long use_snr;
	unsigned char usercard_sec_xf, usercard_sec_js, data0[16], data1[16] ;

	unsigned __int16 tt;

	union ic
	{
		unsigned long i;
		unsigned char ch[4];
	}IC;
	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if(Sfxt_author!=1)  return (No_SF_Author);
    if(Jsxt_author!=1)  return (No_JS_Author);

	usercard_sec_xf = SInew.xfxt_sec;
	usercard_sec_js = SInew.jsxt_sec;

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}
	
	if(use_snr != CardSerno) return (UserCard_Err);

	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_xf,SInew.sys_keyb) != MI_OK) return (Reader_Err);		
	if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_xf) != MI_OK)	return (UserCard_Err);
    if(rf_read(icdev,usercard_sec_xf*4 +1,data0) != MI_OK) return (ReadCard_Err);

	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec_js,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec_js) != MI_OK)	return (UserCard_Err);
    if(rf_read(icdev,usercard_sec_js*4 +1,data1) != MI_OK) return (ReadCard_Err);

	IC.i = 0 ; 
	IC.ch[0] = data0[0] ;
	IC.ch[1] = data0[1] ;
	IC.ch[2] = data0[2] ;
	*Balance_XF = IC.i  ;

	IC.i = 0 ; 
	IC.ch[0] = data1[0] ;
	IC.ch[1] = data1[1] ;
	IC.ch[2] = data1[2] ;
	*Balance_JS = IC.i  ;

	if(rf_halt(icdev)!=0)   return Reader_Err;
	return(DAS_OK);
}

int __stdcall Make_UserCard_New(HANDLE icdev, unsigned long Value, unsigned long Systype, unsigned long CardSerno, unsigned long WaitTime )
{
	unsigned long use_snr, card_numb, card_money1, card_money2;
	unsigned char data0[16];
	unsigned char data1[16] ;
	unsigned char data2[16] ;
	int usercard_sec, i, numb;
	unsigned char bcc, bcc1, bcc2, bcc3 ;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype!=2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno) return (UserCard_Err);
		
		if(rf_authentication(icdev, KEYB|KEYSET2, usercard_sec) != MI_OK)	return (UserCard_Err);

        if(rf_read(icdev,usercard_sec*4+0,data0) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+2,data2) != MI_OK) return (ReadCard_Err);

		IC.i = 0 ;		
		IC.ch[0] = data0[1];
		IC.ch[1] = data0[2];
		IC.ch[2] = data0[0];
		card_numb = IC.i ;

		bcc1 = 0;
		for(i=0; i<15; i++)	 bcc1 += data0[i] ;

		bcc2 = 0;
		for(i=0; i<15; i++)	 bcc2 += data1[i] ;

		bcc3 = 0;
		for(i=0; i<15; i++)	 bcc3 += data2[i] ;

		if((bcc1 != data0[15])||(!card_numb)||(card_numb > 1000000))  return(88);  

		IC.i = 0 ;		
		IC.ch[0] = data1[0];
		IC.ch[1] = data1[1];
		IC.ch[2] = data1[2];
		card_money1 = IC.i ;		
		
		IC.i = 0 ;		
		IC.ch[0] = data2[0];
		IC.ch[1] = data2[1];
		IC.ch[2] = data2[2];
		card_money2 = IC.i ;
		
		if((bcc2 == bcc3)&&(bcc2 == data1[15])&&(bcc3 == data2[15])&&(card_money1 == card_money2))
		{
			if(rf_halt(icdev)!=0)  return Reader_Err;			
			return (DAS_OK);
		}

		if((card_money1 != card_money2)||(bcc2 != bcc3))
		{
			if(card_money1 != card_money2)
			{
				if((card_money1 != 0)&&(card_money2 != 0)&&(bcc3 != data2[15])&&(bcc2 != data1[15]))
				{
					return(Make_UserCard_Err); 
				}
				if((card_money1 == 0)&&((card_money2 != 0)&&(bcc3 == data2[15])))
				{
					for(i=0; i<16; i++)	 data1[i] = data2[i] ;
				}
				else if((card_money2 == 0)&&((card_money1 != 0)&&(bcc2 == data1[15])))
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
				else if((card_money1 > 16000000)&&((card_money2 < 16000000)&&(bcc3 == data2[15])))
				{
					for(i=0; i<16; i++)	 data1[i] = data2[i] ;
				}
				else if((card_money2 > 16000000)&&((card_money1 < 16000000)&&(bcc2 == data1[15])))
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
				else
				{
					if(Value) 
					{
						if(card_money1 == Value)
						{
							for(i=0; i<16; i++)	 data2[i] = data1[i] ;
						}
						else if(card_money2 == Value)
						{
							for(i=0; i<16; i++)	 data1[i] = data2[i] ;
						}
						else
						{
							IC.i = 0 ;	
							IC.i = Value ;	
							data1[0] = IC.ch[0] ;
							data1[1] = IC.ch[1] ;
							data1[2] = IC.ch[2] ;						
						}
					}
					else
					{
						for(i=0; i<16; i++)	 data2[i] = data1[i] ;
					}
				}
			}
			else
			{
				if((card_money2 == 0)&&((card_money1 != 0)&&(bcc2 == data1[15])))
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
				else if((card_money1 == 0)&&((card_money2 != 0)&&(bcc3 == data2[15])))
				{
					for(i=0; i<16; i++)	 data1[i] = data2[i] ;
				}
				else
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
			}
		}
		else
		{
			if((bcc2 == data1[15])&&(bcc3 != data2[15]))
			{
				for(i=0; i<16; i++)	 data2[i] = data1[i] ;
			}
			else if((bcc2 != data1[15])&&(bcc3 == data2[15]))
			{
				for(i=0; i<16; i++)	 data1[i] = data2[i] ;
			}
			else
			{
				for(i=0; i<16; i++)	 data2[i] = data1[i] ;
			}
		}

		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[i] ;
		data1[15]  = bcc ;

		numb = 5 ;
		while(numb--)
		{
			if(rf_write(icdev,usercard_sec*4+1, data1) == MI_OK)
			{ 
				if(rf_write(icdev,usercard_sec*4+2, data1) == MI_OK)			
				{			  		
					if(rf_halt(icdev)!=0)  return Reader_Err;			
					return (DAS_OK);
				}
			}
		}
	}
	return(UserCard_Err); 
}

int __stdcall MAKE_UserCard12_CardName(HANDLE icdev, LPCSTR Cardno , unsigned int CardSerno, int WaitTime)
{
	unsigned long use_snr;
	unsigned char data1[16] ;
	int usercard_sec;
	unsigned char bcc;
	int i , j;
	unsigned __int16 tt;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if(Sfxt_author!=1) return (No_SF_Author);

	if(WaitTime < 0) return (Para_Err);

	usercard_sec = SInew.xfxt_sec;	
	
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);
	if(rf_load_key(icdev,KEYB|KEYSET2,1,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(WaitTime < 800) WaitTime = 800;

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno) return (UserCard_Err);
		
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)		return (UserCard_Err);

		if(rf_read(icdev,usercard_sec*4,data1) != MI_OK) return (ReadCard_Err);
	

		for(i=strlen(Cardno),j=0;i>0;i--,j++)
		{
			data1[14-j] = Cardno[i-1];
		}

		bcc = 0;
		for(i=0;i<15;i++)
		{
			bcc +=data1[i];
		}	
		data1[15] = bcc;
	
		if(rf_write(icdev,usercard_sec*4,data1) == MI_OK)
		{
			if(	rf_halt(icdev)!=0)return Reader_Err;		return (DAS_OK);
		}		
		return (WriteCard_Err);
	}
	 return (UserCard_Err);
}

int __stdcall Make_UserCard_New091102(HANDLE icdev, unsigned long Value, unsigned long Systype, unsigned long CardSerno, unsigned long *Card_Value1, unsigned long *Card_Value2, unsigned long *Card_Value, unsigned long WaitTime )
{
	unsigned long use_snr, card_numb, card_money1, card_money2;
	unsigned char data0[16];
	unsigned char data1[16] ;
	unsigned char data2[16] ;
	int usercard_sec, i, numb;
	unsigned char bcc, bcc1, bcc2, bcc3 ;
	unsigned int  bcc11, bcc22, bcc33 ;
	unsigned __int16 tt;
	
	union ic
	{
		int i;
		unsigned char ch[4];
	}IC;

	
	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1; 
	}
	else return No_Lic_Err;
	
	if((Sfxt_author!=1)&&(Systype==2)) return (No_SF_Author);
    if((Jsxt_author!=1)&&(Systype==3)) return (No_JS_Author);
    if((Mjxt_author!=1)&&(Systype==1)) return (No_MJ_Author);

	switch(Systype)
	{
	case 1:
		usercard_sec = SInew.mjxt_sec;
		break;
    case 2:
		usercard_sec = SInew.xfxt_sec;
		break;
	case 3:
		usercard_sec = SInew.jsxt_sec;
		break;
	default:
		break;
	}

//	usercard_sec = usercard_sec + Number ;
		
	if(rf_load_key(icdev,KEYB|KEYSET2,usercard_sec,SInew.sys_keyb) != MI_OK) return (Reader_Err);

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) return No_Card;
	}

	if((tt ==0x04)||(tt)||(tt == 0x02))	
	{
		if(use_snr != CardSerno) return (UserCard_Err);
		
		if(rf_authentication(icdev,KEYB|KEYSET2,usercard_sec) != MI_OK)	return (UserCard_Err);

        if(rf_read(icdev,usercard_sec*4+0,data0) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+1,data1) != MI_OK) return (ReadCard_Err);
		if(rf_read(icdev,usercard_sec*4+2,data2) != MI_OK) return (ReadCard_Err);

		IC.i = 0 ;		
		IC.ch[0] = data0[1];
		IC.ch[1] = data0[2];
		IC.ch[2] = data0[0];
		card_numb = IC.i ;

		bcc11 = 0;
		for(i=0; i<15; i++)	 bcc11 += data0[i] ;
		bcc1 = (unsigned char)bcc11 ;

		bcc22 = 0;
		for(i=0; i<15; i++)	 bcc22 += data1[i] ;
		bcc2 = (unsigned char)bcc22 ;

		bcc33 = 0;
		for(i=0; i<15; i++)	 bcc33 += data2[i] ;
		bcc3 = (unsigned char)bcc33 ;

		if((bcc1 != data0[15])||(!card_numb)||(card_numb > 1000000))  return(-88);  

		if(memcmp(data1, data2, 16))
		{
			IC.i = 0 ;		
			IC.ch[0] = data1[0];
			IC.ch[1] = data1[1];
			IC.ch[2] = data1[2];
			card_money1  = IC.i ;
			*Card_Value1 = card_money1;
			
			IC.i = 0 ;		
			IC.ch[0] = data2[0];
			IC.ch[1] = data2[1];
			IC.ch[2] = data2[2];
			card_money2  = IC.i ;
			*Card_Value1 = card_money2;
			
			if((bcc22 == bcc33)&&(bcc2 == bcc3)&&(bcc2 == data1[15])&&(bcc3 == data2[15])&&(card_money1 == card_money2)&&(bcc22)&&(bcc33))
			{
				if(rf_halt(icdev)!=0)  return Reader_Err;
				*Card_Value = card_money1;
				return (DAS_OK);
			}
			
			if((card_money1 != card_money2)||(bcc2 != bcc3)||(bcc22 != bcc33))
			{
				if(card_money1 != card_money2)
				{
					if((card_money1 != 0)&&(card_money2 != 0)&&(bcc3 != data2[15])&&(bcc2 != data1[15])&&(!Value))
					{
						return(Make_UserCard_Err); 
					}
					if((card_money1 == 0)&&((card_money2 != 0)&&(bcc3 == data2[15])&&(bcc33)))
					{
						for(i=0; i<16; i++)	 data1[i] = data2[i] ;
					}
					else if((card_money2 == 0)&&((card_money1 != 0)&&(bcc2 == data1[15])&&(bcc22)))
					{
						for(i=0; i<16; i++)	 data2[i] = data1[i] ;
					}
					else if((card_money1 > 16000000)&&((card_money2 < 16000000)&&(bcc3 == data2[15])&&(bcc33)))
					{
						for(i=0; i<16; i++)	 data1[i] = data2[i] ;
					}
					else if((card_money2 > 16000000)&&((card_money1 < 16000000)&&(bcc2 == data1[15])&&(bcc22)))
					{
						for(i=0; i<16; i++)	 data2[i] = data1[i] ;
					}
					else
					{
						if(Value) 
						{
							if(card_money1 == Value)
							{
								for(i=0; i<16; i++)	 data2[i] = data1[i] ;
							}
							else if(card_money2 == Value)
							{
								for(i=0; i<16; i++)	 data1[i] = data2[i] ;
							}
							else
							{
								IC.i = 0 ;	
								IC.i = Value ;	
								data1[0] = IC.ch[0] ;
								data1[1] = IC.ch[1] ;
								data1[2] = IC.ch[2] ;						
							}
						}
						else
						{
							if(card_money1 <= card_money2)
							{
								for(i=0; i<16; i++)	 data2[i] = data1[i] ;
							}
							else
							{
								for(i=0; i<16; i++)	 data1[i] = data2[i] ;
							}
						}
					}
				}
				else
				{
					if((card_money2 == 0)&&((card_money1 != 0)&&(bcc2 == data1[15])))
					{
						for(i=0; i<16; i++)	 data2[i] = data1[i] ;
					}
					else if((card_money1 == 0)&&((card_money2 != 0)&&(bcc3 == data2[15])))
					{
						for(i=0; i<16; i++)	 data1[i] = data2[i] ;
					}
					else
					{
						for(i=0; i<16; i++)	 data2[i] = data1[i] ;
					}
				}
			}
			else
			{
				if((bcc2 == data1[15])&&(bcc22))
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
				else if((bcc33)&&(bcc3 == data2[15]))
				{
					for(i=0; i<16; i++)	 data1[i] = data2[i] ;
				}
				else
				{
					for(i=0; i<16; i++)	 data2[i] = data1[i] ;
				}
			}
		}

		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[i] ;
		data1[15]  = bcc ;

		IC.i = 0 ;		
		IC.ch[0] = data1[0];
		IC.ch[1] = data1[1];
		IC.ch[2] = data1[2];
		*Card_Value = IC.i;

		numb = 5 ;
		while(numb--)
		{
			if(rf_write(icdev,usercard_sec*4+1, data1) == MI_OK)
			{ 
				if(rf_write(icdev,usercard_sec*4+2, data1) == MI_OK)			
				{			  		
					if(rf_halt(icdev)!=0)  return Reader_Err;			
					return (DAS_OK);
				}
			}
		}
	}
	return(UserCard_Err); 
}

int __stdcall Read_SysMainKey(unsigned char *SysMainKey)
{
	if(Get_Lic_New(SysInfonew.ch)!=0)  return(No_Lic_Err);

	memcpy(SysMainKey,SysInfonew.oprater_password,6);

	SysMainKey[6] = (unsigned char )SysInfonew.jsxt_sec ;

	return(DAS_OK);
}

void __stdcall GK_Calc_Key(unsigned char KeyType, unsigned  char *SNR, unsigned char *SysMainKey, unsigned char *ucKey)
{
	if(KeyType)
	{
		ucKey[0]	= SysMainKey[2] + SNR[3] - 0x11; 
		ucKey[1]	= SysMainKey[1] - SNR[2] - 0x22;
		ucKey[2]	= SysMainKey[0] | SNR[0] - 0x33;
		ucKey[3]	= SysMainKey[3] & SNR[1] - 0x44;
		ucKey[4]	= SysMainKey[4] ^ SNR[0] - 0x55;
		ucKey[5]	= SysMainKey[5] | SNR[2] - 0x66;
	}
	else
	{
		ucKey[0]	= SysMainKey[2] + SNR[0] + 0x11; 
		ucKey[1]	= SysMainKey[1] - SNR[3] + 0x22;
		ucKey[2]	= SysMainKey[0] | SNR[1] + 0x33;
		ucKey[3]	= SysMainKey[3] & SNR[2] + 0x44;
		ucKey[4]	= SysMainKey[4] ^ SNR[3] + 0x55;
		ucKey[5]	= SysMainKey[5] ^ SNR[0] + 0x66;
	}
}

//FLASH提取工具 之  数据解析
int __stdcall FlashTool_ResolveData(unsigned char Mode, unsigned char *Record_Data,  unsigned char *Data_Flag, XFREC& Rec)
{
	int  i;
	YCXFG30_UserRecordInfo RecordInfo;

	union _ic
	{
		unsigned long i;
		unsigned char ch[4];
	}IC;
	
	switch(Mode)
	{
	case YCXF_G3:
	case YCXF_G7:
		IC.ch[3] = Record_Data[0];
		IC.ch[2] = Record_Data[1];
		IC.ch[1] = Record_Data[2];
		IC.ch[0] = Record_Data[3];
		Rec.RecordSnr = IC.i;		

		IC.ch[0] = Record_Data[5];		
		IC.ch[1] = Record_Data[6];
		IC.ch[2] = Record_Data[4];
		IC.ch[3] = 0;
		Rec.CardNum = IC.i;		

		IC.ch[0] = Record_Data[7];	
		IC.ch[1] = Record_Data[8];
		IC.ch[2] = Record_Data[9];
		IC.ch[3] = 0;
		Rec.Rest_Money = IC.i;

		IC.ch[0] = Record_Data[10];	
		IC.ch[1] = Record_Data[11];
		IC.ch[2] = Record_Data[12];
		IC.ch[3] = 0;
		Rec.Used_Money = IC.i;

		Rec.UsedTime[0] = itoa_h(Record_Data[13]);
		Rec.UsedTime[1] = itoa_l(Record_Data[13]);
		Rec.UsedTime[2] = itoa_h(Record_Data[14]);
		Rec.UsedTime[3] = itoa_l(Record_Data[14]);		
		Rec.UsedTime[4] = itoa_h(Record_Data[15]);
		Rec.UsedTime[5] = itoa_l(Record_Data[15]);
		Rec.UsedTime[6] = itoa_h(Record_Data[16]);
		Rec.UsedTime[7] = itoa_l(Record_Data[16]);
		Rec.UsedTime[8] = itoa_h(Record_Data[17]);
		Rec.UsedTime[9] = itoa_l(Record_Data[17]);
		Rec.UsedTime[10] = itoa_h(Record_Data[18]);
		Rec.UsedTime[11] = itoa_l(Record_Data[18]);

		IC.ch[0] = Record_Data[19];	
		IC.ch[1] = Record_Data[20];
		IC.ch[2] = 0;
		IC.ch[3] = 0;
		Rec.Used_All_Times = (unsigned __int16)IC.i;	

		Rec.Operater = (unsigned long)Record_Data[21];
		Rec.Status = Record_Data[22];

		IC.ch[0] = Record_Data[29];	
		IC.ch[1] = Record_Data[28];
		IC.ch[2] = 0;
		IC.ch[3] = 0;
		Rec.Terminal_ID =(unsigned __int16)IC.i;

		*Data_Flag = (Record_Data[31] == CRC8_ADD(Record_Data, 31))?0:-1;

		break;

	case YCXF_G3X:
	case YCXF_G7X:
		memcpy(&RecordInfo, &Record_Data[0], sizeof(YCXFG30_UserRecordInfo));

		Rec.RecordSnr = RecordInfo.RecordSnr;
		Rec.CardNum = RecordInfo.CardNum;
		Rec.Rest_Money = RecordInfo.Rest_Money;
		Rec.Operater = RecordInfo.Operater;
		Rec.Status = RecordInfo.Status;
		Rec.Terminal_ID = RecordInfo.Terminal_ID;
		Rec.Used_All_Times = RecordInfo.Used_All_Times;
		Rec.Used_Money = RecordInfo.Used_Money;

		for(i=0;i<6;i++)
		{
			Rec.UsedTime[i*2+0] = itoa_h(RecordInfo.UsedTime[i]);
			Rec.UsedTime[i*2+1] = itoa_l(RecordInfo.UsedTime[i]);
		}

		*Data_Flag = (Record_Data[47] == ((unsigned char)~CRC8_ADD(Record_Data, 47)))?0:-1;

		break;

	case YCXF_V1X:
	case YCXF_V3X:
	case YCXF_V7X:
		IC.ch[0] = Record_Data[0];
		IC.ch[1] = Record_Data[1];
		IC.ch[2] = Record_Data[2];
		IC.ch[3] = Record_Data[3];
		Rec.RecordSnr = IC.i;		

		IC.ch[0] = Record_Data[5];		
		IC.ch[1] = Record_Data[6];
		IC.ch[2] = Record_Data[4];
		IC.ch[3] = 0;
		Rec.CardNum = IC.i;		

		IC.ch[0] = Record_Data[7];	
		IC.ch[1] = Record_Data[8];
		IC.ch[2] = Record_Data[9];
		IC.ch[3] = 0;
		Rec.Rest_Money = IC.i;

		IC.ch[0] = Record_Data[10];	
		IC.ch[1] = Record_Data[11];
		IC.ch[2] = Record_Data[12];
		IC.ch[3] = 0;
		Rec.Used_Money = IC.i;

		Rec.UsedTime[0] = itoa_h(Record_Data[13]);
		Rec.UsedTime[1] = itoa_l(Record_Data[13]);
		Rec.UsedTime[2] = itoa_h(Record_Data[14]);
		Rec.UsedTime[3] = itoa_l(Record_Data[14]);		
		Rec.UsedTime[4] = itoa_h(Record_Data[15]);
		Rec.UsedTime[5] = itoa_l(Record_Data[15]);
		Rec.UsedTime[6] = itoa_h(Record_Data[16]);
		Rec.UsedTime[7] = itoa_l(Record_Data[16]);
		Rec.UsedTime[8] = itoa_h(Record_Data[17]);
		Rec.UsedTime[9] = itoa_l(Record_Data[17]);
		Rec.UsedTime[10] = itoa_h(Record_Data[18]);
		Rec.UsedTime[11] = itoa_l(Record_Data[18]);

		IC.ch[0] = Record_Data[19];	
		IC.ch[1] = Record_Data[20];
		IC.ch[2] = 0;
		IC.ch[3] = 0;
		Rec.Used_All_Times = (unsigned __int16)IC.i;	

		Rec.Operater = (unsigned long)Record_Data[21];
		Rec.Status = Record_Data[22];

		IC.ch[0] = Record_Data[28];	
		IC.ch[1] = Record_Data[29];
		IC.ch[2] = 0;
		IC.ch[3] = 0;
		Rec.Terminal_ID =(unsigned __int16)IC.i;

		*Data_Flag = (Record_Data[31] == ((unsigned char)~CRC8_ADD(Record_Data, 31)))?0:-1;

		break;

	case YCJS_A2X:
		IC.ch[0] = Record_Data[0];
		IC.ch[1] = Record_Data[1];
		IC.ch[2] = Record_Data[2];
		IC.ch[3] = Record_Data[3];
		Rec.RecordSnr = IC.i;		

		IC.ch[0] = Record_Data[4];		
		IC.ch[1] = Record_Data[5];
		IC.ch[2] = Record_Data[6];
		IC.ch[3] = Record_Data[7];
		Rec.CardNum = IC.i;		

		IC.ch[0] = Record_Data[8];	
		IC.ch[1] = Record_Data[9];
		IC.ch[2] = Record_Data[10];
		IC.ch[3] = Record_Data[11];
		Rec.Rest_Money = IC.i;

		IC.ch[0] = Record_Data[12];	
		IC.ch[1] = Record_Data[13];
		IC.ch[2] = Record_Data[14];
		IC.ch[3] = Record_Data[15];
		Rec.Used_Money = IC.i;

		IC.ch[0] = Record_Data[16];	
		IC.ch[1] = Record_Data[17];
		IC.ch[2] = Record_Data[18];
		IC.ch[3] = Record_Data[19];
		Rec.Used_All_Times = IC.i;
		
		IC.ch[0] = Record_Data[20];	
		IC.ch[1] = Record_Data[21];
		IC.ch[2] = Record_Data[22];
		IC.ch[3] = Record_Data[23];
		Rec.Terminal_ID = IC.i;

		Rec.UsedTime[0] = itoa_h(Record_Data[24]);
		Rec.UsedTime[1] = itoa_l(Record_Data[24]);
		Rec.UsedTime[2] = itoa_h(Record_Data[25]);
		Rec.UsedTime[3] = itoa_l(Record_Data[25]);		
		Rec.UsedTime[4] = itoa_h(Record_Data[26]);
		Rec.UsedTime[5] = itoa_l(Record_Data[26]);
		Rec.UsedTime[6] = itoa_h(Record_Data[27]);
		Rec.UsedTime[7] = itoa_l(Record_Data[27]);
		Rec.UsedTime[8] = itoa_h(Record_Data[28]);
		Rec.UsedTime[9] = itoa_l(Record_Data[28]);
		Rec.UsedTime[10] = itoa_h(Record_Data[29]);
		Rec.UsedTime[11] = itoa_l(Record_Data[29]);

		Rec.Status = Record_Data[30];

		*Data_Flag = (Record_Data[31] == ((unsigned char)~CRC8_ADD(Record_Data, 31)))?0:-1;

		break;

	default:
		return(RESOLVEDATA_ERR|0x02);
	}
	return(DAS_OK);
}

int  __stdcall  Advanced_Read_UserCard_Message(HANDLE icdev, int SysType, _advanced_message *Advanced_Message)
{
	unsigned long use_snr;
	unsigned char data1[100];
	int usercard_sec;
	unsigned __int16 tt;

	union ic
	{
		unsigned long i;
		unsigned char ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)	return No_Card;
	}

	if(SysType == SFXT)
	{
		usercard_sec = SInew.xfxt_sec;
		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_read(icdev, usercard_sec*4+0, &data1[0])  != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+1, &data1[16]) != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+2, &data1[32]) != MI_OK)		return ReadCard_Err;

		usercard_sec += 1;
		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_read(icdev, usercard_sec*4+0, &data1[48]) != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+1, &data1[64]) != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+2, &data1[80]) != MI_OK)		return ReadCard_Err;

		IC.i = 0;
		IC.ch[0] = data1[1];
		IC.ch[1] = data1[2];
		IC.ch[2] = data1[0];
		Advanced_Message->Card_Number = IC.i;

		IC.i = 0;
		IC.ch[0] = data1[8];
		IC.ch[1] = data1[9];
		Advanced_Message->Recharge_Times = IC.i;

		Advanced_Message->Card_Type = data1[3];
		Advanced_Message->User_ValidityPeriod[0] = (((data1[7]&0xE0)>>1)|((data1[6]&0xf0)>>4));
		Advanced_Message->User_ValidityPeriod[1] = data1[6]&0x0f;
		Advanced_Message->User_ValidityPeriod[2] = data1[7]&0x1f;
		
		if(data1[10]) memcpy(&Advanced_Message->User_Name[0], &data1[10], 5);
		else if(data1[11]) memcpy(&Advanced_Message->User_Name[0], &data1[11], 4);
		else if(data1[12]) memcpy(&Advanced_Message->User_Name[0], &data1[12], 3);
		else if(data1[13]) memcpy(&Advanced_Message->User_Name[0], &data1[13], 2);
		else if(data1[14]) memcpy(&Advanced_Message->User_Name[0], &data1[14], 1);

		memcpy(&Advanced_Message->User_Password[0], &data1[4], 2);	
		
		IC.i = 0 ;
		IC.ch[0] = data1[16];
		IC.ch[1] = data1[17];
		IC.ch[2] = data1[18];
		Advanced_Message->Rest_Money = IC.i;	
		
		IC.i = 0 ;
		IC.ch[0] = data1[19];
		IC.ch[1] = data1[20];
		IC.ch[2] = data1[21];
		Advanced_Message->Used_Money = IC.i;

		memcpy(&Advanced_Message->Used_Time[0], &data1[22], 5);	
		
		IC.i = 0; 
		IC.ch[0] = data1[27];
		IC.ch[1] = data1[28];
		Advanced_Message->Used_All_Times = IC.i;		
		
		IC.i = 0; 
		IC.ch[0] = data1[29];
		IC.ch[1] = data1[30];
		Advanced_Message->Used_All_Money =IC.i;	
		
		memcpy(&Advanced_Message->User_KQ_Number[0], &data1[49], data1[48]);
		memcpy(&Advanced_Message->User_KQ_Name[0], &data1[50 + data1[48]],  data1[49 + data1[48]]);		
		memcpy(&Advanced_Message->User_KQ_Department[0], &data1[51 + data1[48] + data1[49 + data1[48]]],  data1[50 + data1[48] + data1[49 + data1[48]]]);	
	}
	else if(SysType == JSXT)
	{
		usercard_sec = SInew.jsxt_sec;

		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_read(icdev, usercard_sec*4+0, &data1[0])  != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+1, &data1[16]) != MI_OK)		return ReadCard_Err;
		if(rf_read(icdev, usercard_sec*4+2, &data1[32]) != MI_OK)		return ReadCard_Err;

		IC.i = 0;
		IC.ch[0] = data1[1];
		IC.ch[1] = data1[2];
		IC.ch[2] = data1[0];
		Advanced_Message->Card_Number = IC.i;

		IC.i = 0;
		IC.ch[0] = data1[8];
		IC.ch[1] = data1[9];
		Advanced_Message->Recharge_Times = IC.i;

		Advanced_Message->Card_Type = data1[3];
		Advanced_Message->User_ValidityPeriod[0] = (((data1[7]&0xE0)>>1)|((data1[6]&0xf0)>>4));
		Advanced_Message->User_ValidityPeriod[1] = data1[6]&0x0f;
		Advanced_Message->User_ValidityPeriod[2] = data1[7]&0x1f;
		
		if(data1[10]) memcpy(&Advanced_Message->User_Name[0], &data1[10], 5);
		else if(data1[11]) memcpy(&Advanced_Message->User_Name[0], &data1[11], 4);
		else if(data1[12]) memcpy(&Advanced_Message->User_Name[0], &data1[12], 3);
		else if(data1[13]) memcpy(&Advanced_Message->User_Name[0], &data1[13], 2);
		else if(data1[14]) memcpy(&Advanced_Message->User_Name[0], &data1[14], 1);

		memcpy(&Advanced_Message->User_Password[0], &data1[4], 2);	
		
		IC.i = 0 ;
		IC.ch[0] = data1[16];
		IC.ch[1] = data1[17];
		IC.ch[2] = data1[18];
		Advanced_Message->Rest_Money = IC.i;	
		
		IC.i = 0 ;
		IC.ch[0] = data1[19];
		IC.ch[1] = data1[20];
		IC.ch[2] = data1[21];
		Advanced_Message->Used_Money = IC.i;

		memcpy(&Advanced_Message->Used_Time[0], &data1[22], 5);	
		
		IC.i = 0; 
		IC.ch[0] = data1[27];
		IC.ch[1] = data1[28];
		Advanced_Message->Used_All_Times = IC.i;		
		
		IC.i = 0; 
		IC.ch[0] = data1[29];
		IC.ch[1] = data1[30];
		Advanced_Message->Used_All_Money =IC.i;	
	}
	else
	{
		return(Para_Err);
	}

	return(DAS_OK);
}

int  __stdcall  Advanced_Write_UserCard_Message(HANDLE icdev, int  SysType, _advanced_message  &Advanced_Message)
{
	unsigned long use_snr;
	unsigned char bcc, i, data1[96];
	int usercard_sec;
	unsigned __int16 tt;

	union ic
	{
		unsigned long i;
		unsigned char ch[4];
	}IC;

	if(Get_Lic_New(SInew.ch) == 0)
	{
		if(SInew.mjxt) Mjxt_author = 1; 
		if(SInew.xfxt) Sfxt_author = 1; 
		if(SInew.jsxt) Jsxt_author = 1;
	}
	else return No_Lic_Err;

	if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK) 
	{
		if(rf_card_2(icdev,ALL,&use_snr,&tt) != MI_OK)	return No_Card;
	}

	if(SysType == SFXT)
	{
		IC.i = Advanced_Message.Card_Number;
		data1[1] = IC.ch[0];
		data1[2] = IC.ch[1];
		data1[0] = IC.ch[2];

		IC.i = Advanced_Message.Recharge_Times;
		data1[8] = IC.ch[0];
		data1[9] = IC.ch[1];

		data1[3] = (unsigned char)Advanced_Message.Card_Type;

		data1[6] = (((Advanced_Message.User_ValidityPeriod[0]&0x0f)<<4)|(Advanced_Message.User_ValidityPeriod[1]&0x0f));
		data1[7] = (((Advanced_Message.User_ValidityPeriod[0]&0xF0)<<1)|(Advanced_Message.User_ValidityPeriod[2]&0x1f));
	
		memcpy(&data1[10], &Advanced_Message.User_Name[0], 5);
		memcpy(&data1[4], &Advanced_Message.User_Password[0], 2);	
		
		IC.i = Advanced_Message.Rest_Money;
		data1[16] = IC.ch[0];
		data1[17] = IC.ch[1];
		data1[18] = IC.ch[2];

		IC.i = Advanced_Message.Used_Money;
		data1[19] = IC.ch[0];
		data1[20] = IC.ch[1];
		data1[21] = IC.ch[2];

		memcpy(&data1[22], &Advanced_Message.Used_Time[0], 5);
		
		IC.i = Advanced_Message.Used_All_Times;
		data1[27] = IC.ch[0];
		data1[28] = IC.ch[1];
		
		IC.i = Advanced_Message.Used_All_Money;
		data1[29] = IC.ch[0];
		data1[30] = IC.ch[1];	
		
		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[i] ;
		data1[15]  = bcc ;
		
		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[16+i] ;
		data1[31]  = bcc ;
	
		memset(&data1[48], 0x00, 48);
		data1[48] = (unsigned char)strlen(Advanced_Message.User_KQ_Number);
		memcpy(&data1[49], &Advanced_Message.User_KQ_Number[0], data1[48]);
		data1[49 + data1[48]] = (unsigned char)strlen(Advanced_Message.User_KQ_Name);
		memcpy(&data1[50 + data1[48]],  &Advanced_Message.User_KQ_Name[0], data1[49 + data1[48]]);	
		data1[50 + data1[48] + data1[49 + data1[48]]] = (unsigned char)strlen(Advanced_Message.User_KQ_Department);
		memcpy(&data1[51 + data1[48] + data1[49 + data1[48]]],  &Advanced_Message.User_KQ_Department[0], data1[50 + data1[48] + data1[49 + data1[48]]]);	
		
		usercard_sec = SInew.xfxt_sec;
		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_write(icdev, usercard_sec*4+0, &data1[0])  != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+1, &data1[16]) != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+2, &data1[16]) != MI_OK)		return ReadCard_Err;

		usercard_sec += 1;
		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_write(icdev, usercard_sec*4+0, &data1[48]) != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+1, &data1[64]) != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+2, &data1[80]) != MI_OK)		return ReadCard_Err;
	}
	else if(SysType == JSXT)
	{
		IC.i = Advanced_Message.Card_Number;
		data1[1] = IC.ch[0];
		data1[2] = IC.ch[1];
		data1[0] = IC.ch[2];

		IC.i = Advanced_Message.Recharge_Times;
		data1[8] = IC.ch[0];
		data1[9] = IC.ch[1];

		data1[3] = (unsigned char)Advanced_Message.Card_Type;

		data1[6] = (((Advanced_Message.User_ValidityPeriod[0]&0x0f)<<4)|(Advanced_Message.User_ValidityPeriod[1]&0x0f));
		data1[7] = (((Advanced_Message.User_ValidityPeriod[0]&0xF0)<<1)|(Advanced_Message.User_ValidityPeriod[2]&0x1f));
		
		memcpy(&data1[10], &Advanced_Message.User_Name[0], 5);
		memcpy(&data1[4], &Advanced_Message.User_Password[0], 2);	
		
		IC.i = Advanced_Message.Rest_Money;
		data1[16] = IC.ch[0];
		data1[17] = IC.ch[1];
		data1[18] = IC.ch[2];

		IC.i = Advanced_Message.Used_Money;
		data1[19] = IC.ch[0];
		data1[20] = IC.ch[1];
		data1[21] = IC.ch[2];

		memcpy(&data1[22], &Advanced_Message.Used_Time[0], 5);
		
		IC.i = Advanced_Message.Used_All_Times;
		data1[27] = IC.ch[0];
		data1[28] = IC.ch[1];
		
		IC.i = Advanced_Message.Used_All_Money;
		data1[29] = IC.ch[0];
		data1[30] = IC.ch[1];	
		
		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[i] ;
		data1[15]  = bcc ;
		
		bcc = 0;
		for(i=0; i<15; i++)	 bcc += data1[16+i] ;
		data1[31]  = bcc ;
	
		usercard_sec = SInew.jsxt_sec;
		if(rf_load_key(icdev, KEYB, usercard_sec, SInew.sys_keyb) != MI_OK)		return (Reader_Err);

		if(rf_authentication(icdev, KEYB, usercard_sec) != MI_OK)		return Not_Identify;
		if(rf_write(icdev, usercard_sec*4+0, &data1[0])  != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+1, &data1[16]) != MI_OK)		return ReadCard_Err;
		if(rf_write(icdev, usercard_sec*4+2, &data1[16]) != MI_OK)		return ReadCard_Err;
	}
	else
	{
		return(Para_Err);
	}

	return(DAS_OK);
}


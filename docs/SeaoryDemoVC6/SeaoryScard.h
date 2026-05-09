// -*- mode:c++ -*-

/**
 * @file      SeaoryScard.h
 * @author    Shenzhen Seaory Technology Co., Ltd.
 * @date      2020/06/11
 * @brief     读写器功能接口声明文件。
 * @revision  0.9.19.0
 */

////////////////////////////////////////////////////////////////////////////////
#ifndef __SOYSCAPI_H__
#define __SOYSCAPI_H__

#ifdef __cplusplus
extern "C" {
#endif


  /**
   * @brief  打开设备。
   * @par    说明：
   * 建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 SOY_SC_Exit() 去关闭设备的通讯和释放资源。
   * @param[in] port 端口号。
   * @n 1~100 - 编号 1 表示第一个USB合法设备，编号 2 表示第二个USB合法设备，以此类推。
   * @return <0表示失败，否则为设备标识符。
   */
  HANDLE WINAPI SOY_SC_Init(short port);

  /**
   * @brief  关闭设备。
   * @par    说明：
   * 关闭设备的通讯和释放资源。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Exit(HANDLE hDev);

  /**
   * @brief  获取设备版本。
   * @par    说明：
   * 获取设备内部固件代码的版本。
   * @param[in] icdev 设备标识符。
   * @param[out] sver 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_GetVer(HANDLE hDev, unsigned char *sver);

  /**
   * @brief  复位设备。
   * @par    说明：
   * 使设备进入上电初始状态。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ResetDevice(HANDLE hDev);

  /**
   * @brief  设备蜂鸣。
   * @par    说明：
   * 设备蜂鸣器发出指定时间的蜂鸣声。
   * @param[in] hDev 设备标识符。
   * @param[in] _Msec 蜂鸣时间，单位为10毫秒。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Beep(HANDLE hDev, unsigned short _Msec);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * 读取设备内部EEPROM中的数据，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] rec_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_EEPROM(HANDLE hDev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * 写入数据到设备内部EEPROM中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] send_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_EEPROM(HANDLE hDev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * ::SOY_SC_Read_EEPROM 的HEX形式接口，参数 @a rec_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Read_EEPROM_hex(HANDLE hDev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * ::SOY_SC_Write_EEPROM 的HEX形式接口，参数 @a send_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Write_EEPROM_hex(HANDLE hDev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  装载设备密码。
   * @par    说明：
   * 装载M1卡密码到设备内部，装载后可以在特定时候用 SOY_SC_Authentication 来验证M1卡密码。注意：由于多次装载设备密码可能会受硬件存储寿命限制，此接口只用于密码相对固定，装载次数少的场合。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 SOY_SC_Authentication 的 @a _Mode 。
   * @param[in] _SecNr 对应装载的扇区号。
   * @param[in] _NKey 写入设备中的卡密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Load_Key(HANDLE hDev, unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey);

  /**
   * @brief  装载设备密码。
   * @par    说明：
   * SOY_SC_Load_Key 的HEX形式接口，参数 @a _NKey 为HEX格式。
   */
  short WINAPI SOY_SC_Load_Key_hex(HANDLE hDev, unsigned char _Mode, unsigned char _SecNr, char *_NKey);

  /**
   * @brief  数据转换。
   * @par    说明：
   * 普通数据换成十六进制字符串（短转长）。
   * @param[in] hex 要转换的数据。
   * @param[out] a 转换后的字符串。
   * @param[in] length 要转换数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Hex_A(unsigned char *hex, unsigned char *a, short len);

  /**
   * @brief  数据转换。
   * @par    说明：
   * 十六进制字符数据转换为普通数据（长转短）。
   * @param[in] a 要转换的数据。
   * @param[out] hex 转换后的数据。
   * @param[in] len 要转换数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_A_Hex(unsigned char *a, unsigned char *hex, short len);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * 使用单DES算法对数据进行加密/解密。
   * @param[in] key 8个字节密钥。
   * @param[in] sour 8个字节要做加密/解密的数据。
   * @param[out] dest 返回8个字节运算后的数据。
   * @param[in] m 模式，0-解密，1-加密。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Des(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * SOY_SC_Des 的HEX形式接口，参数 @a key @a sour @a dest 为HEX格式。
   */
  short WINAPI SOY_SC_Des_hex(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  三DES加解密。
   * @par    说明：
   * 使用三DES算法对数据进行加密/解密。
   * @param[in] key 16个字节密钥。
   * @param[in] src 8个字节要做加密/解密的数据。
   * @param[out] dest 返回8个字节运算后的数据。
   * @param[in] m 模式，0-解密，1-加密。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_TripleDes(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  三DES加解密。
   * @par    说明：
   * SOY_SC_TripleDes 的HEX形式接口，参数 @a key @a src @a dest 为HEX格式。
   */
  short WINAPI SOY_SC_TripleDes_hex(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  设置当前接触式卡座。
   * @par    说明：
   * 设置当前接触式卡座为指定卡座，用于多卡座切换卡操作。
   * @param[in] hDev 设备标识符。
   * @param[in] _Byte 卡座编号。
   * @n 0x0C - 附卡座/接触式CPU1卡座。
   * @n 0x0B - 接触式CPU2卡座。
   * @n 0x0D - SAM1卡座。
   * @n 0x0E - SAM2卡座。
   * @n 0x0F - SAM3卡座。
   * @n 0x11 - SAM4卡座。
   * @n 0x12 - SAM5卡座。
   * @n 0x13 - SAM6卡座/ESAM芯片。
   * @n 0x14 - SAM7卡座。
   * @n 0x15 - SAM8卡座。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Set_CPU(HANDLE hDev, unsigned char _Byte);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * 对当前卡座CPU卡进行复位操作，此复位为冷复位。
   * @param[in] hDev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuReset(HANDLE hDev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * SOY_SC_CpuReset 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuReset_hex(HANDLE hDev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * 对当前卡座CPU卡进行复位操作，此复位为热复位。
   * @param[in] hDev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuHotReset(HANDLE hDev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * SOY_SC_CpuHotReset 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuHotReset_hex(HANDLE hDev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApdu(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapdu 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApdu_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduInt(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_CpuApduInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduInt_hex(HANDLE hDev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduEXT(HANDLE hDev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_CpuApduEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduEXT_hex(HANDLE hDev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduSource(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_CpuApduSource 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduSource_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduSourceEXT(HANDLE hDev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_CpuApduSourceEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduSourceEXT_hex(HANDLE hDev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分，内部处理了SW1为0x61和0x6C的情况。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduRespon(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_CpuApduRespon 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduRespon_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分，内部处理了SW1为0x61和0x6C的情况。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuApduRespon_Int(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapduresponInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_CpuApduRespon_Int_hex(HANDLE hDev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡下电。
   * @par    说明：
   * 对当前卡座CPU卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CpuDown(HANDLE hDev);


  /**
   * @brief  复位射频。
   * @par    说明：
   * 复位设备的射频，可以关闭，关闭并且启动。
   * @param[in] hDev 设备标识符。
   * @param[in] _Msec 为0表示关闭射频，否则为复位时间，单位为10毫秒，一般调用建议值为10。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Reset		(HANDLE hDev, unsigned short _Msec);

  /**
   * @brief  配置非接触卡类型。
   * @par    说明：
   * 配置需要操作什么类型的非接触式卡，设备上电后默认操作Type A卡，可以使用此接口来改变类型，一般在寻卡前调用此接口。
   * @param[in] hDev 设备标识符。
   * @param[in] cardtype 类型，'A'表示Type A卡，'B'表示Type B卡。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ConfigCard	(HANDLE hDev, unsigned char cardtype);

  /**
   * @brief  检测多卡状态。
   * @par    说明：
   * 检测设备中存在什么类型的卡片，支持磁卡、非接、接触、身份证。
   * @param[in] hDev 设备标识符。
   * @param[out] flag 状态标记，固定为2个字节，可以解析为一个无符号整数（Big-Endian），值为下面列出的一种或多种之和。
   * @n 0x0001 - 表示已刷磁卡。
   * @n 0x0002 - 表示卡座存在接触式CPU卡。
   * @n 0x0004 - 表示卡座存在接触式未知卡。
   * @n 0x0008 - 表示感应区存在身份证。
   * @n 0x0010 - 表示感应区存在激活前的Type A CPU卡或Type B CPU卡。
   * @n 0x0020 - 表示感应区存在激活后的Type A CPU卡或Type B CPU卡。
   * @n 0x0040 - 表示感应区存在激活前的M1卡。
   * @n 0x0100 - 表示感应区存在激活前的多张Type A CPU卡。
   * @n 0x0200 - 表示感应区存在激活前的多张M1卡。
   * @n 0x0400 - 表示感应区同时存在激活前的Type A CPU卡和M1卡。
   * @n 0x0800 - 表示刷磁卡失败。
   * @n 0x1000 - 表示启动刷磁卡模式处于关闭状态。
   * @n 0x2000 - 表示启动刷磁卡模式处于开启状态。
   * @n 0x4000 - 表示感应区身份证激活前后状态一致。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_Exist(HANDLE hDev, unsigned char *flag);

  /**
   * @brief  检测接触式卡存在。
   * @par    说明：
   * 检测接触式卡片是否存在于当前卡座中。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示存在，==1表示不存在。
   */
  short WINAPI SOY_SC_Card_Status(HANDLE hDev);

  /**
   * @brief  身份证、Type A CPU卡、Type B CPU卡检测。
   * @par    说明：
   * 检测感应区是否存在身份证、Type A CPU卡、Type B CPU卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示无卡，==1表示存在身份证，==2表示存在Type A CPU卡，==3表示存在Type B CPU卡。
   */
  short WINAPI SOY_SC_TypeAB_Card_Status(HANDLE hDev);

  /**
   * @brief  寻卡请求。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的寻卡请求。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，0x00表示对空闲卡进行操作，0x01表示对所有卡操作。
   * @param[out] TagType 返回的ATQA值。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Request(HANDLE hDev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的防卡冲突。
   * @param[in] hDev 设备标识符。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Anticoll(HANDLE hDev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] hDev 设备标识符。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Select(HANDLE hDev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  第二级防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的第二级防卡冲突。
   * @param[in] hDev 设备标识符。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Anticoll2(HANDLE hDev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  第二级选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] hDev 设备标识符。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Select2(HANDLE hDev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  终止卡操作。
   * @par    说明：
   * 使卡片进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Halt(HANDLE hDev);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select 的功能。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card(HANDLE hDev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * ::dc_card 的HEX形式接口，参数 @a snrstr 为HEX格式。
   */
  short WINAPI SOY_SC_Card_hex(HANDLE hDev, unsigned char _Mode, unsigned char *snrstr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 SOY_SC_Request SOY_SC_Anticoll SOY_SC_Select SOY_SC_Anticoll2 SOY_SC_Select2 的功能。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 SOY_SC_Request 的 @a _Mode 。
   * @param[out] SnrLen 返回卡序列号的长度。
   * @param[out] _Snr 返回的卡序列号，请至少分配8个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_n		(HANDLE hDev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * SOY_SC_Card_n 的HEX形式接口，参数 @a _Snr 为HEX格式。
   */
  short WINAPI SOY_SC_Card_n_hex (HANDLE hDev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 SOY_SC_Request SOY_SC_Anticoll SOY_SC_Select SOY_SC_Anticoll2 SOY_SC_Select2 的功能，此接口有设计缺陷，不能返回卡序列号实际长度，建议使用 ::dc_card_n 替代。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号，请至少分配8个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_double(HANDLE hDev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * SOY_SC_Card_double 的HEX形式接口，参数 @a _Snr 为HEX格式。
   */
  short WINAPI SOY_SC_Card_double_hex(HANDLE hDev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 SOY_SC_Request SOY_SC_Anticoll SOY_SC_Select 的功能。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 SOY_SC_Request 的 @a _Mode 。
   * @param[out] Strsnr 返回的卡序列号，格式为数字字符串。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_CardStr(HANDLE hDev, unsigned char _Mode, char *Strsnr);

  /**
   * @brief  寻Type A或Type B卡并激活。
   * @par    说明：
   * 对Type A或Type B卡进行寻卡和激活。
   * @param[in] hDev 设备标识符。
   * @param[out] rlen 返回激活信息的长度。
   * @param[out] rbuf 返回的激活信息，请至少分配128个字节。
   * @param[out] type 类型，'A'表示Type A卡，'B'表示Type B卡。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_AB(HANDLE hDev, unsigned char *rlen, unsigned char *rbuf, unsigned char *type);

  /**
   * @brief  寻Type B卡并激活。
   * @par    说明：
   * 对Type B卡进行寻卡和激活。
   * @param[in] hDev 设备标识符。
   * @param[out] rbuf 返回的激活信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_B(HANDLE hDev, unsigned char *rbuf);

  /**
   * @brief  寻Type B卡并激活。
   * @par    说明：
   * ::dc_card_b 的HEX形式接口，参数 @a rbuf 为HEX格式。
   */
  short WINAPI SOY_SC_Card_B_hex(HANDLE hDev, char *rbuf);

  /**
   * @brief  寻卡请求。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的寻卡请求。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 保留，固定为0x00。
   * @param[in] AFI 应用族标识符。
   * @param[in] N 时间槽编号。
   * @param[out] ATQB 返回的ATQB值，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Request_B(HANDLE hDev, unsigned char _Mode, unsigned char AFI, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  管道标记。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的管道标记。
   * @param[in] hDev 设备标识符。
   * @param[in] N 时间槽编号。
   * @param[out] ATQB 返回的ATQB值，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_SlotMarker(HANDLE hDev, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  激活卡片。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的激活。
   * @param[in] hDev 设备标识符。
   * @param[in] PUPI 伪唯一PICC标识符，固定为4个字节。
   * @param[in] CID 信道号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Attri_B(HANDLE hDev, unsigned char *PUPI, unsigned char CID);


  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用设备内部装载的密码来验证M1卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式。
   * @n 0x00 - 表示用设备内部装载的第0套A密码来验证当前选取卡片的A密码。
   * @n 0x01 - 表示用设备内部装载的第1套A密码来验证当前选取卡片的A密码。
   * @n 0x02 - 表示用设备内部装载的第2套A密码来验证当前选取卡片的A密码。
   * @n 0x04 - 表示用设备内部装载的第0套B密码来验证当前选取卡片的B密码。
   * @n 0x05 - 表示用设备内部装载的第1套B密码来验证当前选取卡片的B密码。
   * @n 0x06 - 表示用设备内部装载的第2套B密码来验证当前选取卡片的B密码。
   * @param[in] _SecNr 要验证密码的扇区号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Authentication(HANDLE hDev, unsigned char _Mode, unsigned char _SecNr);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用设备内部装载的密码来验证M1卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] KeyNr 要验证密码的扇区号。
   * @param[in] Adr 要验证密码扇区中的块号，此块号非卡片绝对块号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Authentication_2(HANDLE hDev, unsigned char _Mode, unsigned char KeyNr, unsigned char Adr);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用传入的密码来验证M1卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，0x00表示验证A密码，0x04表示验证B密码。
   * @param[in] _Addr 要验证密码的块号。
   * @param[in] passbuff 密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_AuthenticationPassAddr(HANDLE hDev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * SOY_SC_AuthenticationPassAddr 的HEX形式接口，参数 @a passbuff 为HEX格式。
   */
  short WINAPI SOY_SC_AuthenticationPassAddr_hex(HANDLE hDev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用传入的密码来验证M1卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，0x00表示验证A密码，0x04表示验证B密码。
   * @param[in] _Addr 要验证密码的扇区号。
   * @param[in] passbuff 密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Authentication_pass(HANDLE hDev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * SOY_SC_Authentication_pass 的HEX形式接口，参数 @a passbuff 为HEX格式。
   */
  short WINAPI SOY_SC_Authentication_pass_hex(HANDLE hDev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  高层验证M1卡密码。
   * @par    说明：
   * 内部包含了 SOY_SC_Card SOY_SC_Authentication 的功能。
   * @param[in] hDev 设备标识符。
   * @param[in] reqmode 寻卡请求模式，同 ::dc_request 的 @a _Mode 。
   * @param[in] snr 保留，固定为0。
   * @param[in] authmode 验证模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] secnr 要验证密码的扇区号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_HL_Authentication(HANDLE hDev, unsigned char reqmode, unsigned int snr, unsigned char authmode, unsigned char secnr);

  /**
   * @brief  读卡数据。
   * @par    说明：
   * 读取卡内数据，对于M1卡，一次读取一个块的数据，为16个字节；对于ML卡，一次读取相同属性的两页，为8个字节。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 地址。
   * @n M1卡 - S50块地址（0~63），S70块地址（0~255）。
   * @n ML卡 - 页地址（0~11）。
   * @param[out] _Data 固定返回16个字节数据，真实数据可能小于16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读卡数据。
   * @par    说明：
   * SOY_SC_Read 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  short WINAPI SOY_SC_Read_hex(HANDLE hDev, unsigned char _Adr, char *_Data);

  /**
   * @brief  写卡数据。
   * @par    说明：
   * 写入数据到卡片内，对于M1卡，一次必须写入一个块的数据，为16个字节；对于ML卡，一次必须写入一个页的数据，为4个字节。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 地址。
   * @n M1卡 - S50块地址（1~63），S70块地址（1~255）。
   * @n ML卡 - 页地址（2~11）。
   * @param[out] _Data 固定传入16个字节数据，真实数据可能小于16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  写卡数据。
   * @par    说明：
   * SOY_SC_Write 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  short WINAPI SOY_SC_Write_hex(HANDLE hDev, unsigned char _Adr, char *_Data);

  /**
   * @brief  M1卡密码配置块操作。
   * @par    说明：
   * 修改M1卡密码配置块数据，M1卡密码配置块也就是每个扇区的最后一块，包含密码A、控制字节、密码B数据。
   * @param[in] hDev 设备标识符。
   * @param[in] _SecNr 扇区号。
   * @param[in] _KeyA 密码A，固定为6个字节。
   * @param[in] _B0 块0控制字（当一扇区有16块时，对应为块0~4的控制字），低3位（D2D1D0）对应C10、C20、C30。
   * @param[in] _B1 块1控制字（当一扇区有16块时，对应为块5~9的控制字），低3位（D2D1D0）对应C11、C21、C31。
   * @param[in] _B2 块2控制字（当一扇区有16块时，对应为块10~14的控制字），低3位（D2D1D0）对应C12、C22、C32。
   * @param[in] _B3 块3控制字（当一扇区有16块时，对应为块15的控制字），低3位（D2D1D0）对应C13、C23、C33。
   * @param[in] _Bk 保留，固定为0x00。
   * @param[in] _KeyB 密码B，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ChangeB3(HANDLE hDev, unsigned char _SecNr, unsigned char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, unsigned char *_KeyB);

  /**
   * @brief  M1卡密码配置块操作。
   * @par    说明：
   * SOY_SC_ChangeB3 的HEX形式接口，参数 @a _KeyA @a _KeyB 为HEX格式。
   */
  short WINAPI SOY_SC_ChangeB3_hex(HANDLE hDev, unsigned char _SecNr, const char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, const char *_KeyB);

  /**
   * @brief  M1卡值块回传。
   * @par    说明：
   * 用于把指定的值块内容暂存，后续可以使用 ::dc_transfer 将暂存值块内容传递到另一块中，实现块与块之间数值传送。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 要回传的块地址。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Restore(HANDLE hDev, unsigned char _Adr);

  /**
   * @brief  M1卡值块传递。
   * @par    说明：
   * 用于把 ::dc_restore 暂存的值块内容传递到指定块中，实现块与块之间数值传送。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 要传递的块地址。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Transfer(HANDLE hDev, unsigned char _Adr);

  /**
   * @brief  M1卡值块初始化。
   * @par    说明：
   * 用于操作M1卡，使得指定块为值块，并且初始化此值块为指定数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 初始化数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_InitVal(HANDLE hDev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1卡值块读值。
   * @par    说明：
   * 用于读取M1卡值块数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 要读取的块地址。
   * @param[out] _Value 返回的数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadVal(HANDLE hDev, unsigned char _Adr, unsigned int *_Value);

  /**
   * @brief  M1卡值块加值。
   * @par    说明：
   * 用于操作M1卡值块，使块值增加指定数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 要增加的数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Increment(HANDLE hDev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1卡值块减值。
   * @par    说明：
   * 用于操作M1卡值块，使块值减少指定数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 要减少的数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Decrement(HANDLE hDev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  ML卡值初始化。
   * @par    说明：
   * 用于操作ML卡，初始化卡内容为指定数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Value 初始化数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_InitVal_ml(HANDLE hDev, unsigned short _Value);

  /**
   * @brief  ML卡读值。
   * @par    说明：
   * 用于读取ML卡数值。
   * @param[in] hDev 设备标识符。
   * @param[out] _Value 返回的数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadVal_ml(HANDLE hDev, unsigned short *_Value);

  /**
   * @brief  ML卡减值。
   * @par    说明：
   * 用于操作ML卡值，使值减少指定数值。
   * @param[in] hDev 设备标识符。
   * @param[in] _Value 要减少的数值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Decrement_ml(HANDLE hDev, unsigned short _Value);

  /**
   * @brief  检查之前写入的数据。
   * @par    说明：
   * 内部包含了 SOY_SC_Card SOY_SC_Authentication SOY_SC_Read 的功能，并且验证传入和读取的数据是否一致。
   * @param[in] hDev 设备标识符。
   * @param[in] Snr 卡序列号，用于内部核对。
   * @param[in] authmode 验证模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] Adr 块地址。
   * @param[in] _data 块数据，固定为16个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Check_Write(HANDLE hDev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  检查之前写入的数据。
   * @par    说明：
   * SOY_SC_Check_Write 的HEX形式接口，参数 @a _data 为HEX格式。
   */
  short WINAPI SOY_SC_Check_Write_hex(HANDLE hDev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  Mifare Desfire卡认证。
   * @par    说明：
   * 对Mifare Desfire卡进行认证。
   * @param[in] hDev 设备标识符。
   * @param[in] keyno 密钥号。
   * @param[in] keylen 密钥数据的长度。
   * @param[in] authkey 密钥数据。
   * @param[in] randAdata 传入随机数A，8个字节。
   * @param[out] randBdata 返回的随机数B，8个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Mfdes_Auth(HANDLE hDev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  Mifare Desfire卡认证。
   * @par    说明：
   * SOY_SC_Mfdes_Auth 的HEX形式接口，参数 @a authkey @a randAdata @a randBdata 为HEX格式。
   */
  short WINAPI SOY_SC_Mfdes_Auth_hex(HANDLE hDev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * 对感应区CPU卡进行复位操作。
   * @param[in] hDev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] receive_data 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_Reset(HANDLE hDev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * SOY_SC_Pro_Reset 的HEX形式接口，参数 @a receive_data 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_Reset_hex(HANDLE hDev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * 对感应区CPU卡进行复位操作。
   * @param[in] hDev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] receive_data 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_ResetInt(HANDLE hDev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * ::dc_pro_resetInt 的HEX形式接口，参数 @a receive_data 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_ResetInt_hex(HANDLE hDev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_Command(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_command 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_Command_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_Command_Int(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_Command_Int 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_Command_Int_hex(HANDLE hDev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);;

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandSource(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_CommandSource 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_CommandSource_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandSource_int(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分，带CRC。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] CRCSTU CRC值。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandSource_CRC(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char CRCSTU);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_CommandSource_CRC 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_CommandSource_CRC_hex(HANDLE hDev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout, unsigned char CRCSTU);
  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] FG 分割值，此值只在部分设备的底层使用，单位为字节，一般调用建议值为64。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandLink(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_CommandLink 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_CommandLink_hex(HANDLE hDev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandLinkInt(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_CommandLinkInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_CommandLinkInt_hex(HANDLE hDev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] hDev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] FG 分割值，此值只在部分设备的底层使用，单位为字节，一般调用建议值为64。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_CommandLinkEXT(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * SOY_SC_Pro_CommandLinkEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Pro_CommandLinkEXT_hex(HANDLE hDev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  终止非接触式CPU卡操作。
   * @par    说明：
   * 使非接触式CPU卡进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Pro_Halt(HANDLE hDev);

  /**
   * @brief  SHC1102卡寻卡请求。
   * @par    说明：
   * 支持SHC1102卡的寻卡请求。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，0x00表示对空闲卡进行操作，0x01表示对所有卡操作。
   * @param[out] TagType 返回的ATQA值。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Request_Shc1102(HANDLE hDev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  验证SHC1102卡密码。
   * @par    说明：
   * 使用传入的密码来验证SHC1102卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] _Data 密码，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Auth_Shc1102(HANDLE hDev, unsigned char *_Data);

  /**
   * @brief  读SHC1102卡。
   * @par    说明：
   * 读取SHC1102卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址（0~15）。
   * @param[out] _Data 返回的数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_Shc1102(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  写SHC1102卡。
   * @par    说明：
   * 写入数据到SHC1102卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址（2~15）。
   * @param[in] _Data 传入数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_Shc1102(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  终止SHC1102卡操作。
   * @par    说明：
   * 使SHC1102卡进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Halt_Shc1102(HANDLE hDev);

  /**
   * @brief  Mifare Plus卡设置个人化数据（0级）。
   * @par    说明：
   * 设置Mifare Plus卡的个人化数据。
   * @param[in] hDev 设备标识符。
   * @param[in] BNr 要写入的个人化数据块号。
   * @param[in] dataperso 要写入的数据，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L0_WritePerson(HANDLE hDev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus卡设置个人化数据（0级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L0_WritePerson 的HEX形式接口，参数 @a dataperso 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L0_WritePerson_hex(HANDLE hDev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus卡提交个人化数据（0级）。
   * @par    说明：
   * 提交Mifare Plus卡的个人化数据，提交成功后卡片进入1级状态。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L0_CommitPerson(HANDLE hDev);

  /**
   * @brief  验证Mifare Plus卡状态密码（1级）。
   * @par    说明：
   * 验证Mifare Plus卡状态密码，用于在1级状态下实现严格的认证。
   * @param[in] hDev 设备标识符。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L1_Auth_L1key(HANDLE hDev, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（1级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L1_Auth_L1key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L1_Auth_L1key_hex(HANDLE hDev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到2级（1级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，1级状态的卡片转换到2级。
   * @param[in] hDev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L1_Switch_toL2(HANDLE hDev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到3级（1级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，1级状态的卡片转换到3级。
   * @param[in] hDev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L1_Switch_toL3(HANDLE hDev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到3级（2级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，2级状态的卡片转换到3级。
   * @param[in] hDev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L2_Switch_toL3(HANDLE hDev, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * 执行3级状态卡片认证，根据密码块号的不同，验证不同的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] keyBNr 密码块号。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_Auth_L3key(HANDLE hDev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * ::dc_MFPL3_authl3key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_Auth_L3_hex(HANDLE hDev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * 执行3级状态卡片认证，根据密码扇区号的不同，验证不同的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] mode 模式，0x00表示验证A密码，内部用0x4000+sectorBNr*2计算密码块，0x04表示验证B密码，内部用0x4000+sectorBNr*2+1计算密码块。
   * @param[in] sectorBNr 密码扇区号。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_Auth_L3_SectorKey(HANDLE hDev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * ::dc_MFPL3_authl3key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_Auth_L3_SectorKey_hex(HANDLE hDev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  读Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下读取Mifare Plus卡数据，可以连续读多块，每块16字节。
   * @param[in] hDev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数目，一般不大于6块。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_ReadInPlain(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  读Mifare Plus卡数据（3级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L3_ReadInPlain 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_ReadInPlain_hex(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  加密模式读Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下用加密模式读取Mifare Plus卡数据，可以连续读多块，每块16字节。
   * @param[in] hDev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数目，一般不大于6块。
   * @param[out] readdata 返回的数据。
   * @param[in] flag 标记，0x00表示加密数据内部解密后再返回，0x01表示加密数据直接返回。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_ReadEncrypted(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  加密模式读Mifare Plus卡数据（3级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L3_ReadEncrypted 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_ReadEncrypted_hex(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  写Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下写入数据到Mifare Plus卡中，可以连续写多块，每块16字节。
   * @param[in] hDev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数目，一般不大于6块。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_WriteInPlain(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  写Mifare Plus卡数据（3级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L3_WriteInPlain 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_WriteInPlain_hex(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  加密模式写Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下用加密模式写入数据到Mifare Plus卡中，可以连续写多块，每块16字节。注意：写密码时，必须使用此函数并且块数目只能为1。
   * @param[in] hDev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数目，一般不大于6块。
   * @param[in] writedata 传入数据。
   * @param[in] flag 标记，0x00表示传入数据需内部加密后再使用，0x01表示传入数据直接使用。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_WriteEncrypted(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  加密模式写Mifare Plus卡数据（3级）。
   * @par    说明：
   * SOY_SC_MF_PLUS_L3_WriteEncrypted 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_MF_PLUS_L3_WriteEncrypted_hex(HANDLE hDev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  验证Mifare Ultralight C卡密码。
   * @par    说明：
   * 使用传入的密码来验证Mifare Ultralight C卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] key 密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Auth_Ulc(HANDLE hDev, unsigned char *key);

  /**
   * @brief  验证Mifare Ultralight C卡密码。
   * @par    说明：
   * SOY_SC_Auth_Ulc 的HEX形式接口，参数 @a key 为HEX格式。
   */
  short WINAPI SOY_SC_Auth_Ulc_hex(HANDLE hDev, unsigned char *key);

  /**
   * @brief  修改Mifare Ultralight C卡密码。
   * @par    说明：
   * 修改Mifare Ultralight C卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] newkey 密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Changekey_Ulc(HANDLE hDev, unsigned char *newkey);

  /**
   * @brief  修改Mifare Ultralight C卡密码。
   * @par    说明：
   * SOY_SC_Changekey_Ulc 的HEX形式接口，参数 @a newkey 为HEX格式。
   */
  short WINAPI SOY_SC_Changekey_Ulc_hex(HANDLE hDev, unsigned char *newkey);

  /**
   * @brief  检测4442卡。
   * @par    说明：
   * 检测当前卡座中是否存在4442卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_4442(HANDLE hDev);

  /**
   * @brief  4442卡下电。
   * @par    说明：
   * 对4442卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Down_4442(HANDLE hDev);

  /**
   * @brief  验证4442卡密码。
   * @par    说明：
   * 使用传入的密码来验证4442卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_VerifyPin_4442(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  验证4442卡密码。
   * @par    说明：
   * SOY_SC_VerifyPin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_VerifyPin_4442_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码。
   * @par    说明：
   * 读取4442卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[out] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadPin_4442(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码。
   * @par    说明：
   * SOY_SC_ReadPin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_ReadPin_4442_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码计数。
   * @par    说明：
   * 读取4442卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，否则为密码计数值。
   */
  short WINAPI SOY_SC_ReadPinCount_4442(HANDLE hDev);

  /**
   * @brief  修改4442卡密码。
   * @par    说明：
   * 修改4442卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ChangePin_4442(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  修改4442卡密码。
   * @par    说明：
   * SOY_SC_ChangePin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_ChangePin_4442_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * 读取4442卡的保护区中哪些位置已经被置保护。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据，数据中含有0x00字节的位置表示已经被置保护。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadProtect_4442(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * SOY_SC_ReadProtect_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_ReadProtect_4442_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡保护位。
   * @par    说明：
   * 对4442卡的保护区中指定位置进行置保护。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据，数据中和卡内原有数据相同的字节位置将被置保护。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_WriteProtect_4442(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡保护位。
   * @par    说明：
   * SOY_SC_WriteProtect_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_WriteProtect_4442_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡。
   * @par    说明：
   * 读取4442卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_4442(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡。
   * @par    说明：
   * SOY_SC_Read_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Read_4442_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡。
   * @par    说明：
   * 写入数据到4442卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_4442(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡。
   * @par    说明：
   * SOY_SC_Write_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Write_4442_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  检测4428卡。
   * @par    说明：
   * 检测当前卡座中是否存在4428卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_4428(HANDLE hDev);

  /**
   * @brief  4428卡下电。
   * @par    说明：
   * 对4428卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Down_4428(HANDLE hDev);

  /**
   * @brief  验证4428卡密码。
   * @par    说明：
   * 使用传入的密码来验证4428卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_VerifyPin_4428(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  验证4428卡密码。
   * @par    说明：
   * SOY_SC_VerifyPin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_VerifyPin_4428_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码。
   * @par    说明：
   * 读取4428卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[out] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadPin_4428(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码。
   * @par    说明：
   * ::dc_readpin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_ReadPin_4428_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码计数。
   * @par    说明：
   * 读取4428卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，否则为密码计数值。
   */
  short WINAPI SOY_SC_ReadPinCount_4428(HANDLE hDev);

  /**
   * @brief  修改4428卡密码。
   * @par    说明：
   * 修改4428卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ChangePin_4428(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  修改4428卡密码。
   * @par    说明：
   * SOY_SC_ChangePin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  short WINAPI SOY_SC_ChangePin_4428_hex(HANDLE hDev, unsigned char *passwd);

  /**
   * @brief  读4428卡保护位。
   * @par    说明：
   * 读取4428卡的保护区中哪些位置已经被置保护。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据，数据中含有0x00字节的位置表示已经被置保护。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ReadProtect_4428(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4428卡保护位。
   * @par    说明：
   * SOY_SC_ReadProtect_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_ReadProtect_4428_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡保护位。
   * @par    说明：
   * 对4428卡的保护区中指定位置进行置保护。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据，数据中和卡内原有数据相同的字节位置将被置保护。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_WriteProtect_4428(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡保护位。
   * @par    说明：
   * SOY_SC_WriteProtect_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_WriteProtect_4428_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4428卡。
   * @par    说明：
   * 读取4428卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_4428(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4428卡。
   * @par    说明：
   * SOY_SC_Read_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Read_4428_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡。
   * @par    说明：
   * 写入数据到4428卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_4428(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡。
   * @par    说明：
   * SOY_SC_Write_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Write_4428_hex(HANDLE hDev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  检测24C01卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C01卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c01(HANDLE hDev);

  /**
   * @brief  检测24C02卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C02卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c02(HANDLE hDev);

  /**
   * @brief  检测24C04卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C04卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c04(HANDLE hDev);

  /**
   * @brief  检测24C08卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C08卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c08(HANDLE hDev);

  /**
   * @brief  检测24C16卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C16卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c16(HANDLE hDev);

  /**
   * @brief  检测24C64卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C64卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_24c64(HANDLE hDev);

  /**
   * @brief  检测CPU卡。
   * @par    说明：
   * 检测当前卡座中是否存在CPU卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在T=0的CPU卡，==1表示存在T=1的CPU卡。
   */
  short WINAPI SOY_SC_Check_CPU(HANDLE hDev);

  /**
   * @brief  检测卡。
   * @par    说明：
   * 检测当前卡座中存在的卡类型。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在。
   * @n 8 - 表示存在4442卡。
   * @n 9 - 表示存在4428卡。
   * @n 30 - 表示存在T=0的CPU卡。
   * @n 31 - 表示存在T=1的CPU卡。
   * @n 21 - 表示存在24C01卡。
   * @n 22 - 表示存在24C02卡。
   * @n 23 - 表示存在24C04卡。
   * @n 24 - 表示存在24C08卡。
   * @n 25 - 表示存在24C16卡。
   * @n 26 - 表示存在24C64卡。
   */
  short WINAPI SOY_SC_Check_Card(HANDLE hDev);

  /**
   * @brief  读24C系列卡。
   * @par    说明：
   * 读取24C系列卡的数据，支持24C01、24C02、24C04、24C08、24C16卡。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] receive_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_24c(HANDLE hDev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读24C系列卡。
   * @par    说明：
   * SOY_SC_Read_24c 的HEX形式接口，参数 @a receive_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Read_24c_hex(HANDLE hDev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  写24C系列卡。
   * @par    说明：
   * 写入数据到24C系列卡中，支持24C01、24C02、24C04、24C08、24C16卡。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] snd_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_24c(HANDLE hDev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  写24C系列卡。
   * @par    说明：
   * SOY_SC_Write_24c 的HEX形式接口，参数 @a snd_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Write_24c_hex(HANDLE hDev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  读24C64系列卡。
   * @par    说明：
   * 读取24C64系列卡的数据，支持24C64、24C512、24C1024卡。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] receive_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_24c64(HANDLE hDev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读24C64系列卡。
   * @par    说明：
   * SOY_SC_Read_24c64 的HEX形式接口，参数 @a receive_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Read_24c64_hex(HANDLE hDev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  写24C64系列卡。
   * @par    说明：
   * 写入数据到24C64系列卡中，支持24C64、24C512、24C1024卡。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] snd_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_24c64(HANDLE hDev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  写24C64系列卡。
   * @par    说明：
   * SOY_SC_Write_24c64 的HEX形式接口，参数 @a snd_buffer 为HEX格式。
   */
  short WINAPI SOY_SC_Write_24c64_hex(HANDLE hDev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  检测102卡。
   * @par    说明：
   * 检测是否存在102卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_102(HANDLE hDev);

  /**
   * @brief  102卡下电。
   * @par    说明：
   * 对102卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Down_102(HANDLE hDev);

  /**
   * @brief  读102卡。
   * @par    说明：
   * 读取102卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_102(HANDLE hDev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  读102卡。
   * @par    说明：
   * SOY_SC_Read_102 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_Read_102_hex(HANDLE hDev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  写102卡。
   * @par    说明：
   * 写入数据到102卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_102(HANDLE hDev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  写102卡。
   * @par    说明：
   * SOY_SC_Write_102 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_Write_102_hex(HANDLE hDev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  验证102卡密码。
   * @par    说明：
   * 使用传入的密码来验证102卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码，密码长度为2个字节。
   * @n 11 - 表示一区擦除密码，密码长度为6个字节。
   * @n 12 - 表示二区擦除密码，密码长度为4个字节。
   * @param[in] password 密码。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CheckPass_102(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  验证102卡密码。
   * @par    说明：
   * SOY_SC_CheckPass_102 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_CheckPass_102_hex(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  修改102卡密码。
   * @par    说明：
   * 修改102卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码，密码长度为2个字节。
   * @n 11 - 表示一区擦除密码，密码长度为6个字节。
   * @n 12 - 表示二区擦除密码，密码长度为4个字节。
   * @param[in] password 密码。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ChangePass_102(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  修改102卡密码。
   * @par    说明：
   * SOY_SC_ChangePass_102 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_ChangePass_102_hex(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  读102卡密码计数。
   * @par    说明：
   * 读取102卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @return <0表示失败，否则为密码计数值。
   */
  short WINAPI SOY_SC_ReadCount_102(HANDLE hDev, short zone);

  /**
   * @brief  102卡熔丝。
   * @par    说明：
   * 对102卡进行熔丝操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Fuse_102(HANDLE hDev);

  /**
   * @brief  检测1604卡。
   * @par    说明：
   * 检测是否存在1604卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_1604(HANDLE hDev);

  /**
   * @brief  1604卡下电。
   * @par    说明：
   * 对1604卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Down_1604(HANDLE hDev);

  /**
   * @brief  读1604卡。
   * @par    说明：
   * 读取1604卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_1604(HANDLE hDev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读1604卡。
   * @par    说明：
   * SOY_SC_Read_1604 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_Read_1604_hex(HANDLE hDev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写1604卡。
   * @par    说明：
   * 写入数据到1604卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_1604(HANDLE hDev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写1604卡。
   * @par    说明：
   * SOY_SC_Write_1604 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_Write_1604_hex(HANDLE hDev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证1604卡密码。
   * @par    说明：
   * 使用传入的密码来验证1604卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @param[in] password 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CheckPass_1604(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  验证1604卡密码。
   * @par    说明：
   * SOY_SC_CheckPass_1604 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_CheckPass_1604_hex(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  修改1604卡密码。
   * @par    说明：
   * 修改1604卡的密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @param[in] password 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_ChangePass_1604(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  修改1604卡密码。
   * @par    说明：
   * SOY_SC_ChangePass_1604 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_ChangePass_160_hex(HANDLE hDev, short zone, unsigned char *password);

  /**
   * @brief  读1604卡密码计数。
   * @par    说明：
   * 读取1604卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @return <0表示失败，否则为密码计数值。
   */
  short WINAPI SOY_SC_ReadCount_1604(HANDLE hDev, short zone);

  /**
   * @brief  1604卡熔丝。
   * @par    说明：
   * 对1604卡进行熔丝操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Fuse_1604(HANDLE hDev);

  /**
   * @brief  读1608卡。
   * @par    说明：
   * 读取1608卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_1608(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读1608卡。
   * @par    说明：
   * SOY_SC_Read_1608 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_Read_1608_hex(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写1608卡。
   * @par    说明：
   * 写入数据到1608卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_1608(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写1608卡。
   * @par    说明：
   * SOY_SC_Write_1608 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_Write_1608_hex(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证1608卡密码。
   * @par    说明：
   * 使用传入的密码来验证1608卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] type 类型，0表示验证写密钥，1表示验证读密钥。
   * @param[in] password 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CheckPass_1608(HANDLE hDev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  验证1608卡密码。
   * @par    说明：
   * SOY_SC_CheckPass_1608 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_CheckPass_1608_hex(HANDLE hDev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  初始化1608卡认证。
   * @par    说明：
   * 使用传入的数据来初始化1608卡认证。
   * @param[in] hDev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Init_Auth_1608(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  初始化1608卡认证。
   * @par    说明：
   * SOY_SC_Init_Auth_1608 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Init_Auth_1608_hex(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  核对1608卡认证。
   * @par    说明：
   * 使用传入的数据来核对1608卡认证。
   * @param[in] hDev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Check_Auth_1608(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  核对1608卡认证。
   * @par    说明：
   * SOY_SC_Check_Auth_1608 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Check_Auth_1608_hex(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  写1608卡熔丝。
   * @par    说明：
   * 写1608卡熔丝。
   * @param[in] hDev 设备标识符。
   * @param[in] value 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_Fuse_1608(HANDLE hDev, unsigned char value);

  /**
   * @brief  读1608卡熔丝。
   * @par    说明：
   * 读1608卡熔丝。
   * @param[in] hDev 设备标识符。
   * @param[out] value 状态值。
   * @n bit0 - FAB，Atmel公司发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit1 - CMA，卡发行商发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit2 - PER，个人化发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit3~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_Fuse_1608(HANDLE hDev, unsigned char *value);

  /**
   * @brief  检测153卡。
   * @par    说明：
   * 检测是否存在153卡。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  short WINAPI SOY_SC_Check_153(HANDLE hDev);

  /**
   * @brief  153卡下电。
   * @par    说明：
   * 对153卡进行下电操作。
   * @param[in] hDev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Down_153(HANDLE hDev);

  /**
   * @brief  读153卡。
   * @par    说明：
   * 读取153卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_153(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读153卡。
   * @par    说明：
   * SOY_SC_Read_153 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  short WINAPI SOY_SC_Read_153_hex(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写153卡。
   * @par    说明：
   * 写入数据到153卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_153(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写153卡。
   * @par    说明：
   * SOY_SC_Write_153 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  short WINAPI SOY_SC_Write_153_hex(HANDLE hDev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证153卡密码。
   * @par    说明：
   * 使用传入的密码来验证153卡密码。
   * @param[in] hDev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] type 类型，0表示验证写密钥，1表示验证读密钥。
   * @param[in] password 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_CheckPass_153(HANDLE hDev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  验证153卡密码。
   * @par    说明：
   * SOY_SC_CheckPass_153 的HEX形式接口，参数 @a password 为HEX格式。
   */
  short WINAPI SOY_SC_CheckPass_153_hex(HANDLE hDev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  初始化153卡认证。
   * @par    说明：
   * 使用传入的数据来初始化153卡认证。
   * @param[in] hDev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Init_Auth_153(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  初始化153卡认证。
   * @par    说明：
   * SOY_SC_Init_Auth_153 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Init_Auth_153_hex(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  核对153卡认证。
   * @par    说明：
   * 使用传入的数据来核对153卡认证。
   * @param[in] hDev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Check_Auth_153(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  核对153卡认证。
   * @par    说明：
   * SOY_SC_Check_Auth_153 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  short WINAPI SOY_SC_Check_Auth_153_hex(HANDLE hDev, unsigned char *databuffer);

  /**
   * @brief  写153卡熔丝。
   * @par    说明：
   * 写153卡熔丝。
   * @param[in] hDev 设备标识符。
   * @param[in] value 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_Fuse_153(HANDLE hDev, unsigned char value);;

  /**
   * @brief  读153卡熔丝。
   * @par    说明：
   * 读153卡熔丝。
   * @param[in] hDev 设备标识符。
   * @param[out] value 状态值。
   * @n bit0 - FAB，Atmel公司发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit1 - CMA，卡发行商发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit2 - PER，个人化发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit3~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_Fuse_153(HANDLE hDev, unsigned char *value);;

  /**
   * @brief  寻FM11RF005卡。
   * @par    说明：
   * 对FM11RF005卡进行寻卡操作。
   * @param[in] hDev 设备标识符。
   * @param[in] _Mode 模式，模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  short WINAPI SOY_SC_Card_fm11rf005(HANDLE hDev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  获取FM11RF005卡序列号。
   * @par    说明：
   * 获取FM11RF005卡序列号。
   * @param[in] hDev 设备标识符。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_GetSnr_fm11rf005(HANDLE hDev, unsigned int *_Snr);

  /**
   * @brief  获取FM11RF005卡序列号。
   * @par    说明：
   * SOY_SC_GetSnr_fm11rf005 的HEX形式接口，参数 @a snrstr 为HEX格式。
   */
  short WINAPI SOY_SC_GetSnr_fm11rf005_hex(HANDLE hDev, unsigned char *snrstr);

  /**
   * @brief  读FM11RF005卡。
   * @par    说明：
   * 读取FM11RF005卡的数据。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址（0~15）。
   * @param[out] _Data 返回的数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Read_fm11rf005(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读FM11RF005卡。
   * @par    说明：
   * SOY_SC_Read_fm11rf005 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  short WINAPI SOY_SC_Read_fm11rf005_hex(HANDLE hDev, unsigned char _Adr, char *_Data);

  /**
   * @brief  写FM11RF005卡。
   * @par    说明：
   * 写入数据到FM11RF005卡中。
   * @param[in] hDev 设备标识符。
   * @param[in] _Adr 块地址（2~15）。
   * @param[in] _Data 传入数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  short WINAPI SOY_SC_Write_fm11rf005(HANDLE hDev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  写FM11RF005卡。
   * @par    说明：
   * SOY_SC_Write_fm11rf005 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  short WINAPI SOY_SC_Write_fm11rf005_hex(HANDLE hDev, unsigned char _Adr, char *_Data);


#ifdef __cplusplus
}
#endif

#endif//__SOYSCAPI_H__

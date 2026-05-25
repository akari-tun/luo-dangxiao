/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-08-02
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* NameSpace:       YC.SelfCardSys.CardOperate
* Description:     卡片操作类
* ***********************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace YC.SelfCardSys.CardOperate
{
    class YCCard
    {
        const string CardDLL = "YCCard.DLL";

        static YCCard()
        {
            System.IO.Directory.SetCurrentDirectory( System.AppDomain.CurrentDomain.BaseDirectory );
        }

        #region 通用串口API
        /// <summary>
        /// 打开串口
        /// </summary>
        /// <param name="CommPort">
        /// 串口编号
        /// （com1为0，com2为1……最大编号为20）
        /// </param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "OpenComm")]
        public static extern IntPtr OpenComm(byte CommPort);
        /// <summary>
        /// 关闭串口
        /// </summary>
        /// <param name="hwnd">打开串口后返回的设备句柄</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "CloseComm")]
        public static extern int CloseComm(IntPtr hwnd);
        #endregion

        #region 读写器的API
        /// <summary>
        /// 设置显示管的控制方式
        /// 计算机控制或者读写器自动控制
        /// 仅适用于新型控制器
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="mode">工作模式（0计算机1读写器）</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "rf_ctl_m")]
        public static extern int rf_ctl_m(IntPtr hwnd, int mode);

        /// <summary>
        /// 设置显示管时间
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="datetime">时间(7个字节的首字节，分别表示年、周、月、日、时、分、秒)</param>
        /// <returns></returns>
        /// <example>
        /// 时间的表示方法（用十六进制表示，比如显示30，则内存数据应当为十六进制的30，即十进制的48
        /// 时间       2009-6-10 16:42:05
        /// 十六进制   09 00 06 10 16 42 05 （第二字节表示星期，未启用）
        /// 十进制     09 00 09 16 22 66 05
        /// </example>
        [DllImport(CardDLL, EntryPoint = "rf_settime")]
        public static extern int rf_settime(IntPtr hwnd, ref  byte datetime);

        /// <summary>
        /// 设置读写器显示模式
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="mode">
        /// 显示模式（0显示日期1显示时间）
        /// 读写器控制，仅适用于新版读卡器
        /// </param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "rf_disp_m")]
        public static extern int rf_disp_m(IntPtr hwnd, int mode);

        /// <summary>
        /// 在读写器上显示数字
        /// 最多显示8位
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="lenNum">截取的有效数字</param>
        /// <param name="num">
        /// 数字序列的第一个字节
        /// 数字用数组表示，需要在数字后显示小数点，则该数字加128进行掩码
        /// </param>
        /// <returns></returns>
        /// <example>
        /// 为显示1234567.8
        /// 传入的字节数组应该为1，2，3，4，5，6，128+7，8
        /// 截取的有效数字总长为8
        /// </example>
        [DllImport(CardDLL, EntryPoint = "rf_disp8")]
        public static extern int rf_disp8(IntPtr hwnd, int lenNum, ref  byte num);

        /// <summary>
        /// 测试发卡器
        /// </summary>
        /// <param name="hwnd">打开串口后返回的设备句柄</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "Check_Reader")]
        public static extern int Check_Reader(IntPtr hwnd);
        /// <summary>
        /// 蜂鸣
        /// </summary>
        /// <param name="hwnd">打开串口后返回的设备句柄</param>
        /// <param name="MSecond">持续的毫秒数</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "rf_beep")]
        public static extern int rf_beep(IntPtr hwnd, int MSecond);
        #endregion

        #region 通用卡API
        /// <summary>
        /// 直接读取卡物理号
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="tagType">卡类型，04 为MF1卡</param>
        /// <param name="cardID">卡物理号</param>
        [DllImport(CardDLL, EntryPoint = "ReadCard_ID")]
        public static extern int ReadCard_ID
           (
           IntPtr hwnd,
           ref int tagType,
           ref uint cardID
           );

        /// <summary>
        /// 直接读取卡物理号
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="tagType">卡类型，04 为MF1卡</param>
        /// <param name="cardID">卡物理号</param>
        [DllImport(CardDLL, EntryPoint = "ReadCard_ID_NEW")]
        public static extern int ReadCard_ID_NEW
           (
           IntPtr hwnd,
           ref int tagType,
           ref int cardID
           );

        /// <summary>
        /// 读当前系统
        /// </summary>
        /// <param name="jsxt">节水系统</param>
        /// <param name="mjxt">考勤（门禁）系统</param>
        /// <param name="sfxt">消费系统</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "Get_SysType")]
        public static extern Int64 Get_SysType
            (
            ref string mjxt,
            ref string sfxt,
            ref string jsxt
            );

        /// <summary>
        /// 获取授权文件中的各系统扇区
        /// </summary>
        /// <param name="Mjxt_Sec">考勤（门禁）扇区</param>
        /// <param name="Sfxt_Sec">消费扇区</param>
        /// <param name="Jsxt_Sec">节水扇区</param>
        /// <param name="CommPassword">通讯密码</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "Get_SysUse_Sec")]
        public static extern int Get_SysUse_Sec
            (
            ref int Mjxt_Sec,
            ref int Sfxt_Sec,
            ref int Jsxt_Sec,
            StringBuilder CommPassword
            );

        /// <summary>
        /// 新建系统卡
        /// </summary>
        /// <param name="hwnd">串口句柄</param>
        /// <param name="UserPassword">用户密码</param>
        /// <param name="SysType">系统类型（sysCardTypes)</param>
        /// <param name="UseSector">用户扇区</param>
        /// <param name="CommPassword">通讯密码</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Init_SysCard12_NewCreat")]
        public static extern int Init_SysCard12_NewCreat
            (
            IntPtr hwnd,
            string UserPassword,
            int SysType,
            int UseSector,
            StringBuilder CommPassword,
            ref byte SecretKey
            );

        /// <summary>
        /// 新建系统卡(含手机)
        /// </summary>
        /// <param name="hwnd">串口句柄</param>
        /// <param name="UserPassword">用户密码</param>
        /// <param name="User_KEYAB">rfsim密码</param>
        /// <param name="SysType">系统类型</param>
        /// <param name="UseSector">用户扇区</param>
        /// <param name="User_KEY">返回rfsim密码</param>
        /// <param name="CommPassword">通讯密码</param>
        /// <returns></returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Init_Mobile_SysCard")]
        public static extern int Init_Mobile_SysCard
            (
            IntPtr hwnd,
            string UserPassword,
            string User_KEYAB,
            int SysType,
            int UseSector,
            ref byte User_KEY,
            StringBuilder CommPassword
            );

        /// <summary>
        /// 改写系统卡
        /// </summary>
        /// <param name="hwnd">串口句柄</param>
        /// <param name="SysType">系统卡类型（sysCardTypes)</param>
        /// <param name="UseSector">用户扇区</param>
        /// <returns></returns>
        /// <remarks>适用于在同一张系统卡上追加子系统授权</remarks>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Init_SysCard12_NewReWrite")]
        public static extern int Init_SysCard12_NewReWrite
            (
            IntPtr hwnd,
            int SysType,
            int UseSector,
            ref byte SecretKey
            );

        /// <summary>
        /// 查询卡类型
        /// </summary>
        /// <param name="hwnd">串口句柄</param>
        /// <param name="SysType">系统卡类型（sysCardTypes)</param>
        /// <param name="CardType">卡类型（CardTypes)</param>
        /// <param name="intsalesOperatorId">操作员编号</param>
        /// <param name="cardserno">返回卡ID 号</param>
        /// <param name="WaitTime">超时等待时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Query_Card_Type")]
        public static extern int Query_Card_Type
            (
            IntPtr hwnd,
            ref int SysType,
            ref int CardType,
            ref int intsalesOperatorId,
            ref uint cardserno,
            int WaitTime,
            ref byte SecretKey
            );

        /// <summary>
        /// 修改用户卡类
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="Old_UserType">原用户卡卡类（1-8类）</param>
        /// <param name="UserType">待修改的卡类（1-8类）</param>
        /// <param name="Systype">用户卡类型，1为门禁、2为消费、3为节水
        /// </param>
        /// <returns>操作返回的结果</returns>
        /// <seealso cref="Change_Pos_UserType12"/>
        [DllImport(CardDLL, EntryPoint = "Change_Pos_UserType12_NEW")]
        public static extern int Change_Pos_UserType12_NEW
            (
            IntPtr hwnd,
            ref  int Old_UserType,
            int UserType,
            int Systype
            );

        /// <summary>
        /// 修改消费用户卡有效期
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="SysType">卡的系统类型(消费、节水、考勤……)</param>
        /// <param name="useterm">
        /// 新有效期
        /// (2008-3-4则为80304)
        /// </param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="WaitTime">等待放卡时间</param>
        [DllImport(CardDLL, EntryPoint = "WRT_UserCard_Term")]
        public static extern int WRT_UserCard_Term
            (
            IntPtr hwnd,
             int SysType,
             int useterm,
             int cardserno,
             int WaitTime
            );

        /// <summary>
        /// 修复卡片
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="Value">库中卡余（参考）（传0）</param>
        /// <param name="Systype">系统类型(2消费3水控)</param>
        /// <param name="CardSerno">物理卡号</param>
        /// <param name="Card_Value1">修复前标准块钱包余额</param>
        /// <param name="Card_Value2">修复前备份块钱包余额</param>
        /// <param name="Card_Value">修复后钱包余额</param>
        /// <param name="WaitTime">延时</param>
        /// <returns>０表示成功８８表示无法修复，－29表示需要参考金额，其它参考写卡异常表</returns>
        /// <remarks>
        /// Value不为零时，将根据情况考虑修成库余
        /// 修卡流程：
        /// 判断各块校验，以写块顺序为优先顺序，通过校验者优先
        /// 校验均不通过，报错。如参考库余不为0，以参考库余为准
        /// 
        /// 过程：尝试参考库余为0修卡，返回-29时，送入正确的库余，再次修卡。
        /// 若仍不成功（返回88），回收卡片，重新发卡
        /// </remarks>
        [DllImport(CardDLL, EntryPoint = "Make_UserCard_New091102")]
        public static extern int Make_UserCard_New091102
            (
                IntPtr hwnd,
                int Value,
                int Systype,
                int CardSerno,
                ref int Card_Value1,
                ref int Card_Value2,
                ref int Card_Value,
                int WaitTime
            );
        #endregion

        #region 消费卡的API

        #region 营业员卡


        /// <summary>
        /// 初始化消费营业员卡
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="Opt_Num">营业员编号</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>如果是加密卡片，则要求对所有扇区加密后才可以正常初始化</remarks>
        /// <seealso cref="RST_Pos_OPTCard12"/>
        [DllImport(CardDLL, EntryPoint = "Init_Pos_OPTCard12")]
        public static extern int Init_Pos_OPTCard12
            (
             IntPtr hwnd,
             int Opt_Num,
             int WaitTime,
             ref int cardserno,
             ref byte SecretKey
            );


        /// <summary>
        /// 回收消费营业员卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="waitime">等待放卡时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <seealso cref="Init_Pos_OPTCard12"/>
        [DllImport(CardDLL, EntryPoint = "RST_Pos_OPTCard12")]
        public static extern int RST_Pos_OPTCard12
            (
            IntPtr hwnd,
             int cardserno,
             int waitime,
             ref byte SecretKey
            );


        #endregion

        #region 用户卡
        /// <summary>
        /// 修复消费用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="userPwd">卡流水号</param>
        /// <param name="cardno">用户编号(少于5个字节的ASCII）</param>
        /// <param name="usertype">用户卡类型(一类、二类……）</param>
        /// <param name="Value">卡余额</param>
        /// <param name="UseCount">卡消费次数</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="Use_Term">有效期</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Init_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "Init_Pos_UserCard_N12")]
        public static extern int Init_Pos_UserCard_N12
            (
             IntPtr hwnd,
             int cardID,
             string cardno,
             int usertype,
             int Value,
             int UseCount,
             int WaitTime,
             ref int cardserno,
             int Use_Term,
             ref byte SecretKey
            );

        /// <summary>
        /// 初始化消费用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="cardID">卡流水号</param>
        /// <param name="cardno">用户编号(少于5个字节的ASCII）</param>
        /// <param name="usertype">用户卡类型(一类、二类……）</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="Use_Term">有效期</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Init_Pos_UserCard_N12"/>
        [DllImport(CardDLL, EntryPoint = "Init_Pos_UserCard12")]
        public static extern int Init_Pos_UserCard12
            (
             IntPtr hwnd,
             int cardID,
             string cardno,
             int usertype,
             int WaitTime,
             ref uint cardserno,
             int Use_Term,
             ref byte SecretKey
            );

        /*
 EXTERN	int  ActivatePosUserCard12
  * (HANDLE icdev,int Serno,unsigned char Lockflag,unsigned char NewCardFlag,
  * unsigned char MainCardFlag,int UserType,unsigned long * CardSerno
  * ,unsigned long use_term ,unsigned char * user_code_new);

 参数：
 icdev——串口句柄
 Serno——发卡流水号
 LockFlag——锁卡标记，如果卡片为黑名单，则为0xBB，否则为0
 NewCardFlag——新卡标记，置为0xAA，否则为0x00
 MainCardFlag——主副卡标记，主卡为0xCC，否则为0x00
 UserType——卡类
 CardSerno——物理卡号
 use_term——卡片使用有效期
 user_code_new——考勤扇区数据缓存指针
 */

        /// <summary>
        /// 在线交易系统新卡发卡函数
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="cardID"></param>
        /// <param name="LockFlag"></param>
        /// <param name="NewCardFlag"></param>
        /// <param name="MainCardFlag"></param>
        /// <param name="userType"></param>
        /// <param name="cardserno"></param>
        /// <param name="Use_Term"></param>
        /// <param name="SecretKey"></param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "ActivatePosUserCard12")]
        public static extern int ActivatePosUserCard12
            (
             IntPtr hwnd,
             int cardID,
             byte LockFlag,
             byte NewCardFlag,
             byte MainCardFlag,
             int userType,
             ref uint cardserno,
             int Use_Term,
             ref byte SecretKey
            );

        /// <summary>
        /// 查询消费用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="CardType">卡的功能类型(用户卡、空白卡……)</param>
        /// <param name="Opt_Num">营业员编号</param>
        /// <param name="CardID">卡流水号</param>
        /// <param name="CardNO">用户编号</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="Value">余额</param>
        /// <param name="count">充值次数</param>
        /// <param name="AddCount">消费次数</param>
        /// <param name="Consum_Add">启用日限次后的当日消费总额</param>
        /// <param name="UserType">卡类(一类、二类……)</param>
        /// <param name="useterm">有效期</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Query_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "Query_Pos_Card12")]
        public static extern int Query_Pos_Card12
            (
            IntPtr hwnd,
             ref int CardType,
             ref int Opt_Num,
             ref int CardID,
             ref string CardNO,
             ref int cardserno,
             ref int Value,
             ref int count,
             ref int UserType,
             ref int Consum_Add,
             ref int useterm,
             ref int AddCount,
             int WaitTime,
             ref byte SecretKey

            );

        /// <summary>
        /// 查询消费用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        ///  <param name="CardType">卡的功能类型(用户卡、空白卡……)</param>
        /// <param name="Opt_Num">营业员编号</param>
        /// <param name="CardID">卡流水号</param>
        /// <param name="UserNo">人员编号</param>
        /// <param name="UserType">卡类(一类、二类……)</param>
        /// <param name="cardserno">卡物理号</param>
        /// <param name="MlngChkSum1">使用块的校验状态，０为正确，１为错误</param>
        /// <param name="Value1">使用块的金额(分)</param>
        /// <param name="LastPay1">最后一次消费额(分)</param>
        /// <param name="Count1">使用块的消费次数 </param>
        /// <param name="Consume_Add1">使用块的消费累加额(分)</param>
        /// <param name="ChkSum2">备份块的校验状态，０为正确，１为错误</param>
        /// <param name="Value2">备份块的金额(分)</param>
        /// <param name="LastPay2">备份块的最后一次消费额(分)</param>
        /// <param name="Count2">备份块的消费次数</param>
        /// <param name="Consume_Add2">备份块的消费次数</param>
        /// <param name="Use_Term">有效期</param>
        /// <param name="AddCount">消费次数</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        ///<seealso cref="Query_Pos_Card12"/>
        ///<seealso cref="Init_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "Query_Pos_UserCard12")]
        public static extern int Query_Pos_UserCard12
            (
            IntPtr hwnd,
             ref int CardType,
             ref int Opt_Num,
             ref int CardID,
             StringBuilder UserNo,
             ref int UserType,
             ref uint cardserno,
             ref int MlngChkSum1,
             ref int Value1,
             ref int LastPay1,
             ref int Count1,
             ref int Consume_Add1,
             ref int ChkSum2,
             ref int Value2,
             ref int LastPay2,
             ref int Count2,
             ref int Consume_Add2,
             ref int Use_Term,
             ref int AddCount,
             int WaitTime,
             ref byte SecretKey

            );

        /// <summary>
        /// 回收消费用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="waitime">等待放卡时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Init_Pos_UserCard12"/>
        /// <seealso cref="Query_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "RST_Pos_UserCard12")]
        public static extern int RST_Pos_UserCard12
            (
            IntPtr hwnd,
             uint cardserno,
             int waitime,
             ref byte SecretKey
            );

        /// <summary>
        /// 消费用户卡充值
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="Value">新余额</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="waitime">等待放卡时间</param>
        /// <seealso cref="Query_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "WRT_Pos_UserCard_AddCount12")]
        public static extern int WRT_Pos_UserCard_AddCount12
            (
             IntPtr hwnd,
             int Value,
             uint cardserno,
             int waitime
            );

        /// <summary>
        /// 修卡函数（仅修改印刷卡号）
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="PressNO">印刷卡号</param>
        /// <param name="WaitTime">等待时间</param>
        /// <returns>操作结果</returns>
        /// <remarks>2008-11-25应北京杨凯反映，提供可以修改该部分的数据的功能</remarks>
        [DllImport(CardDLL, EntryPoint = "MAKE_UserCard12_CardName")]
        public static extern int MAKE_UserCard12_CardName
            (
            IntPtr hwnd,
            string PressNO,
            int CardFactoryID,
            int WaitTime
            );

        /// <summary>
        /// 修改用户卡卡类
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="typeID">新的卡类</param>
        /// <returns>操作返回值</returns>
        /// <seealso cref="Change_Pos_UserType12_NEW"/>
        [DllImport(CardDLL, EntryPoint = "Change_Pos_UserType12")]
        public static extern int Change_Pos_UserType12
            (
            IntPtr hwnd,
            int typeID
            );
        #endregion

        #region 钱包转帐
        /// <summary>
        /// 读取大小钱包金额
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="ValueXF">主帐户余额</param>
        /// <param name="ValueJS">小帐户余额</param>
        /// <param name="CardSerno">物理卡号</param>
        /// <param name="WaitTime">等待时间</param>
        /// <returns></returns>
        /// <seealso cref="Query_Pos_UserCard12"/>
        [DllImport(CardDLL, EntryPoint = "Read_Pos_Balance")]
        public static extern int Read_Pos_Balance
            (
             IntPtr hwnd,
             ref int ValueXF,
             ref int ValueJS,
             int CardSerno,
             int WaitTime
            );

        /// <summary>
        /// 钱包转帐
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="Value">转帐金额</param>
        /// <param name="CardSerno">物理卡号</param>
        /// <param name="flag">方向(为0表示从水控转消费，其他均为消费转水控)</param>
        /// <param name="WaitTime">等待时间</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "WRT_Pos_Virement")]
        public static extern int WRT_Pos_Virement
            (
            IntPtr hwnd,
            int Value,
            int CardSerno,
            int flag,
            int WaitTime
            );

        #endregion

        #region 智能修卡
        /// <summary>
        /// 智能修卡(成功返回0，如果返回-25表示用户卡错误类型较多，不适合自动修复，建议管理员采集所有数据后再进行回收、补卡业务。)
        /// </summary>
        /// <param name="icdev">通讯句柄</param>
        /// <param name="Value">数据库参考金额，0表示不需要参考</param>
        /// <param name="Systype">卡类型，1为门禁、2为消费、3为水控</param>
        /// <param name="CardSerno">卡物理ID号</param>
        /// <param name="WaitTime">等待读卡时间</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "Make_UserCard_New")]
        public static extern int Make_UserCard_New
            (IntPtr icdev,
            int Value,
            int Systype,
            int CardSerno,
            int WaitTime
            );
        #endregion

        #endregion

        #region 节水卡的API

        /// <summary>
        /// 初始化节水营业员卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="Opt_Num">营业员编号</param>
        /// <param name="WaitTime">等待放卡时间</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Init_Js_UserCard"/>
        /// <seealso cref="Init_Pos_UserCard12"/>
        /// <seealso cref="RST_Pos_OPTCard12"/>
        [DllImport(CardDLL, EntryPoint = "Init_JS_OPTCard12")]
        public static extern int Init_JS_OPTCard12
            (
             IntPtr hwnd,
             int Opt_Num,
             int WaitTime,
             ref int cardserno,
             ref byte SecretKey

            );
        /// <summary>
        /// 初始化节水用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="CardID">用户密码</param>
        /// <param name="CardNo">卡流水号</param>
        /// <param name="UserType">用户卡类型(一类、二类……）</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        /// <seealso cref="Init_JS_OPTCard12"/>
        /// <seealso cref="Init_Pos_UserCard12"/>
        /// <seealso cref="Init_Pos_OPTCard12"/>
        [DllImport(CardDLL, EntryPoint = "Init_Js_UserCard")]
        public static extern int Init_Js_UserCard
            (
            IntPtr hwnd,
             int CardID,
             string CardNo,
             int UserType,
             ref uint cardserno,
             ref byte SecretKey
            );
        /// <summary>
        /// 修复节水用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="CardNo">用户编号</param>
        /// <param name="cardID">卡流水号</param>
        /// <param name="UserType">用户卡类型(一类、二类……）</param>
        /// <param name="CardBlance">卡余额</param>
        /// <param name="ChargeTimes">卡消费次数</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Init_Js_UserCard_N")]
        public static extern int Init_Js_UserCard_N
            (
            IntPtr hwnd,
             int cardID,
             string CardNo,
             int UserType,
             int CardBlance,
             int ChargeTimes,
             ref int cardserno,
             ref byte SecretKey
            );


        /// <summary>
        /// 初始化节水系统卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="RateData">节水费率参数</param>
        /// <param name="AdvData">节水高级参数</param>
        /// <param name="Addr">
        /// 起始地址
        /// (用于设置机器地址,每用一次递增一个)
        /// </param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "MakeJSSubSysCard12")]
        public static extern int MakeJSSubSysCard12
            (
            IntPtr hwnd,
             ref byte RateData,
             ref byte AdvData,
             int Addr,
             ref byte SecretKey
            );
        /// <summary>
        /// 查询节水用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="CardType">卡的功能类型(用户卡、空白卡……)</param>
        /// <param name="Opt_Num">营业员编号</param>
        /// <param name="CardNo">卡流水号</param>
        /// <param name="userPwd">用户密码</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="Value">余额</param>
        /// <param name="count">使用次数</param>
        /// <param name="UserType">卡类(一类、二类……)</param>
        /// <param name="waitime">等待放卡时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "QueryJsCard")]
        public static extern int QueryJsCard
            (
            IntPtr hwnd,
             ref int CardType,
             ref int Opt_Num,
             ref int CardNo,
             StringBuilder userPwd,
             ref int cardserno,
             ref int Value,
             ref int count,
             ref int UserType,
             int waitime,
             ref byte SecretKey
            );
        /// <summary>
        /// 回收节水参数卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "RSTJSSubSysCard")]
        public static extern int RSTJSSubSysCard
            (
            IntPtr hwnd,
             ref byte SecretKey
            );
        /// <summary>
        /// 回收节水用户卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="cardserno">物理卡号</param>
        [DllImport(CardDLL, EntryPoint = "RSTJsUserCard")]
        public static extern int RSTJsUserCard
            (
            IntPtr hwnd,
             uint cardserno,
             ref byte SecretKey
            );
        /// <summary>
        /// 初始化节水机地址设置卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="Addr">
        /// 起始地址
        /// (每设置成功一次,数值递增1)
        /// </param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Set_Js_AddreCard")]
        public static extern int Set_Js_AddreCard
            (
            IntPtr hwnd,
             int Addr,
             ref byte SecretKey
            );
        /// <summary>
        /// 初始化节水机时间设置卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="js_date">日期</param>
        /// <param name="js_time">时间</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Set_Js_DateTimeCard")]
        public static extern int Set_Js_DateTimeCard
            (
            IntPtr hwnd,
             string js_date,
             string js_time,
             ref byte SecretKey
            );
        /// <summary>
        /// 初始化节水机查询卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "Set_Js_QueryCard")]
        public static extern int Set_Js_QueryCard
            (
            IntPtr hwnd,
             ref byte SecretKey
            );

        /// <summary>
        /// 节水用户卡充值
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="Blance">新余额</param>
        /// <param name="ChargeDateTime">时间</param>
        /// <param name="cardserno">物理卡号</param>
        /// <remarks>充值成功使用次数加1</remarks>
        [DllImport(CardDLL, EntryPoint = "WRT_Js_UserCard_AddCount")]
        public static extern int WRT_Js_UserCard_AddCount
            (
            IntPtr hwnd,
            int Blance,
            [MarshalAs(UnmanagedType.VBByRefStr)] ref string ChargeDateTime,
            uint cardserno
            );



        #region 黑名单卡


        /// <summary>
        /// 初始化黑名单卡
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="Block">块号</param>
        /// <param name="CardFixID">返回的卡物理号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns></returns>
        /// <remarks>每块4096个卡，即512个字节，超过该限制，则需要使用多张卡片进行一卡一块初始化</remarks>
        [DllImport(CardDLL, EntryPoint = "InitBlackCard")]
        public static extern int InitBlackCard
            (
                 IntPtr hwnd,
                 byte Block,
                 ref int CardFixID,
                 ref byte SecretKey
            );

        /// <summary>
        /// 写黑名单数据至黑名单卡
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="CardFixID">卡物理号</param>
        /// <param name="data">数据（该块的名单数据）</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "WriteBlackCard")]
        public static extern int WriteBlackCard
            (
                 IntPtr hwnd,
                 int CardFixID,
                 ref byte data
            );


        #endregion

        #region 采集卡

        #region 分体水控采集卡
        //==2009-6-9 一体水控函数库中被另一条函数覆盖==
        ///// <summary>
        ///// 初始化单机节水采集卡
        ///// </summary>
        ///// <param name="hwnd">句柄</param>
        ///// <param name="Oprt">营业员编号</param>
        ///// <param name="cardserno">返回的物理卡号</param>
        ///// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        ///// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        ///// <remarks>适用于V1.2卡结构</remarks>
        //[DllImport(CardDLL, EntryPoint = "InitCollectCard")]
        //public static extern int InitCollectCard
        //    (
        //     IntPtr hwnd,
        //     int Oprt,
        //     ref int cardserno,
        //     ref byte SecretKey
        //    );

        /// <summary>
        /// 读取采集卡内的所有记录
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="RecData">记录</param>
        /// <param name="recNum">记录索引</param>
        /// <returns>操作结果</returns>
        [DllImport(CardDLL, EntryPoint = "ReadAllRec")]
        public static extern int ReadAllRec
            (
            IntPtr hwnd,
             ref  byte RecData,
             ref int recNum
            );

        #endregion



        /// <summary>
        /// 回收节水采集卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="cardserno">物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "RSTCollectCard")]
        public static extern int RSTCollectCard
            (
            IntPtr hwnd,
             int cardserno,
             ref byte SecretKey
            );



        #region 一体机采集卡

        /// <summary>
        /// 初始化单机节水采集卡
        /// </summary>
        /// <param name="hwnd">句柄</param>
        /// <param name="Oprt">营业员编号</param>
        /// <param name="mode">采集卡的采集模式（0总额1明细）</param>
        /// <param name="cardserno">返回的物理卡号</param>
        /// <param name="SecretKey">卡片加密密钥数组的第一个字节（第一字节为加密方式，后8字节为密钥）</param>
        /// <returns>0为正常，其它数字为异常，异常代码见错误代码表</returns>
        /// <remarks>适用于V1.2卡结构</remarks>
        [DllImport(CardDLL, EntryPoint = "InitCollectCard")]
        public static extern int InitCollectCard
            (
            IntPtr hwnd,
             int Oprt,
             byte mode,
             ref int cardserno,
             ref byte SecretKey
            );


        /// <summary>
        /// 读采集卡基本信息
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="mode">采集卡的采集模式（0总额1明细）</param>
        /// <param name="CardFixID">采集卡物理号</param>
        /// <param name="RecNum">已采集记录条数（明细卡始终为0）</param>
        /// <returns>0成功，其它失败</returns>
        [DllImport(CardDLL, EntryPoint = "ReadRecNum_NEW")]
        public static extern int ReadRecNum_NEW
            (
                IntPtr hwnd,
                ref short mode,
                ref int CardFixID,
                ref short RecNum
            );



        /// <summary>
        /// 读采集卡内的设备明细记录
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="data">每台设备消费明细</param>
        /// <param name="CardFixID">采集卡物理号</param>
        /// <param name="All_Money">消费总额</param>
        /// <param name="All_Times">消费总次</param>
        /// <param name="All_Amount">消费总量（记时单位为秒，计量单位为升）</param>
        /// <param name="Address">设备地址</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "ReadDetailedRec_NEW")]
        public static extern int ReadDetailedRec_NEW
            (
                IntPtr hwnd,
                ref byte data,
                int CardFixID,
                ref uint All_Money,
                ref uint All_Times,
                ref uint All_Amount,
                ref uint Address
           );

        /// <summary>
        /// 读取总额采集卡数据
        /// </summary>
        /// <param name="hwnd">通讯句柄</param>
        /// <param name="data">数据</param>
        /// <param name="CardFixID">采集卡物理号</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "ReadAllRec_NEW")]
        public static extern int ReadAllRec_NEW
            (
                IntPtr hwnd,
                ref byte data,
                int CardFixID

            );

        #endregion



        #endregion


        #endregion

        #region 自助补发卡API
        /// <summary>
        /// 密钥计算接口
        /// </summary>
        /// <param name="CardID">卡片物理</param>
        /// <param name="baseKey">加密卡片密钥缓存（6个字节）</param>
        /// <param name="PassWords">
        /// 访问卡片的密钥缓存（12个字节）
        /// 如果加密模式等于2；则passwords前六个字节为A密钥，后六个字节为B密钥</param>
        /// <param name="EncryptMode">0-无加密卡；1-动态加密；2-发卡后要访问卡片的密钥计算方式</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_CalculateDynamicKey")]
        public static extern int SDFZ_CalculateDynamicKey(uint CardID, ref byte baseKey, ref byte PassWords, byte EncryptMode);

        /// <summary>
        /// 获取用户扇区接口
        /// </summary>
        /// <param name="XFSector">消费钱包扇区缓存指针，人员信息扇区为消费扇区数加1</param>
        /// <param name="JSSector">水控钱包扇区缓存指针</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_GetSectorNumber")]
        public static extern int SDFZ_GetSectorNumber(ref uint XFSector, ref uint JSSector);

        /// <summary>
        /// 初始化消费钱包数据转换接口
        /// </summary>
        /// <param name="Serno">卡序列号</param>
        /// <param name="CardID">物理ID</param>
        /// <param name="UserNumber">用户编号</param>
        /// <param name="CardType">卡类</param>
        /// <param name="use_term">有效期</param>
        /// <param name="user_code_new">用户信息缓存指针（48个字节）</param>
        /// <param name="databuff">写入卡片数据缓存指针（128个字节），前面64个字节属于消费钱包扇区，后面64个字节属于用户信息</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_InitPosUserCard12")]
        public static extern int SDFZ_InitPosUserCard12(uint Serno, int CardID, string UserNumber, int CardType, int use_term, ref byte user_code_new, ref byte databuff);

        /// <summary>
        /// 钱包充值数据转换接口
        /// </summary>
        /// <param name="Value">充值前余额+充值金额</param>
        /// <param name="InData">钱包扇区数据缓存指针（48个字节）</param>
        /// <param name="OutData">写入钱包的数据缓存指针（48个字节）</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_WalletReCharge")]
        public static extern int SDFZ_WalletReCharge(int Value, ref byte InData, ref byte OutData);

        /// <summary>
        /// 消费钱包读取数据转换接口
        /// </summary>
        /// <param name="Indata">消费钱包和用户信息扇区数据缓存指针（96个字节）</param>
        /// <param name="Serno">读取的卡序列号缓存指针</param>
        /// <param name="Cardno">读取的用户编号缓存指针</param>
        /// <param name="UserType">读取的用户卡类缓存指针</param>
        /// <param name="ChkSum1">钱包块校验结果缓存指针</param>
        /// <param name="Value1">读取的卡余缓存指针</param>
        /// <param name="LastPay1">读取上次消费额缓存指针</param>
        /// <param name="Count1">日消费次数缓存指针</param>
        /// <param name="Consume_Add1">日消费总额缓存指针</param>
        /// <param name="ChkSum2">钱包块校验结果缓存指针</param>
        /// <param name="Value2">读取的卡余缓存指针</param>
        /// <param name="LastPay2">读取上次消费额缓存指针</param>
        /// <param name="Count2">日消费次数缓存指针</param>
        /// <param name="Consume_Add2">日消费总额缓存指针</param>
        /// <param name="use_term">有效期</param>
        /// <param name="AddCount">充值次数</param>
        /// <param name="user_code_new">用户信息保存指针</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_ReadPosCard12")]
        public static extern int SDFZ_ReadPosCard12(ref byte Indata,ref ulong Serno, String Cardno, Int32 UserType, Int32 ChkSum1, Int32 Value1, Int32 LastPay1, Int32 Count1, Int32 Consume_Add1,Int32 ChkSum2, Int32 Value2, Int32 LastPay2, Int32 Count2, Int32 Consume_Add2, ref ulong use_term, Int32 AddCount, ref byte user_code_new);

        /// <summary>
        /// 卡片回收数据转换接口
        /// </summary>
        /// <param name="InitiaDataBuff">要写入卡片的初始化数据缓存指针</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_RecycleUserCard12")]
        public static extern int SDFZ_RecycleUserCard12(ref byte InitiaDataBuff);

        /// <summary>
        /// 水控发卡数据转换接口
        /// </summary>
        /// <param name="CardID"></param>
        /// <param name="Serno"></param>
        /// <param name="UserNumber"></param>
        /// <param name="CardType"></param>
        /// <param name="use_term"></param>
        /// <param name="databuff"></param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_InitJsUserCard12")]
        public static extern int SDFZ_InitJsUserCard12(int CardID, int Serno, String UserNumber, int CardType, ulong use_term, ref byte databuff);

        /// <summary>
        /// 水控扇区读取数据转换接口
        /// </summary>
        /// <param name="InData">消费钱包和用户信息扇区数据缓存指针（48个字节）</param>
        /// <param name="Serno">读取的卡序列号缓存指针</param>
        /// <param name="Cardno">读取的用户编号缓存指针</param>
        /// <param name="Value">卡余</param>
        /// <param name="Count">日消费次数</param>
        /// <param name="UserType">读取的用户卡类缓存指针</param>
        /// <returns></returns>
        [DllImport(CardDLL, EntryPoint = "SDFZ_ReadJsCard")]
        public static extern int SDFZ_ReadJsCard(ref byte InData, Int32 Serno, String Cardno, Int32 Value, Int32 Count, Int32 UserType);
        #endregion
    }
}

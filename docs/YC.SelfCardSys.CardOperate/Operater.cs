/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-08-02
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* NameSpace:       YC.Transfer.CardOperate
* Description:     卡片操作类
* ***********************************************/

using System;
using System.Text;

namespace YC.SelfCardSys.CardOperate
{
    public class Operater
    {
        #region 变量
        IntPtr mHandle = IntPtr.Zero;
        string mLastErrMsg = string.Empty;

        string mComPort = string.Empty;

        bool mIsOpen = false;
        #endregion

        #region 属性
        /// <summary>
        /// 已经打开的串口句柄
        /// </summary>
        public IntPtr Handle
        {
            set { mHandle = value; }
            get { return mHandle; }
        }

        /// <summary>
        /// 最后错误信息
        /// </summary>
        public string LastErrMsg
        {
            set { mLastErrMsg = value; }
            get { return mLastErrMsg; }
        }

        /// <summary>
        /// 串口号
        /// </summary>
        public string ComPort
        {
            set { mComPort = value; }
            get { return mComPort; }
        }

        /// <summary>
        /// 设备是否已打开
        /// </summary>
        public bool IsOpen
        {
            set { mIsOpen = value; }
            get { return mIsOpen; }
        }
        #endregion

        #region 构造函数
        /// <summary>
        /// 构造函数
        /// </summary>
        public Operater()
        {
            ErrMsg.Init();
        }
        #endregion

        #region 打开设备
        /// <summary>
        /// 打开设备
        /// </summary>
        /// <param name="pstrCom">串口信息 （如：COM3）</param>
        /// <returns>0－成功；其他则失败（失败返回其错误码）</returns>
        public bool OpenDevice()
        {
            int Ret = 0;

            try
            {
                LastErrMsg = string.Empty;

                IntPtr Handle = YCCard.OpenComm(0);

                if (Ret == 0)
                {
                    mIsOpen = true;
                    mHandle = Handle;
                }

                mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 关闭设备
        /// <summary>
        /// 关闭设备
        /// </summary>
        /// <param name="nHandle">打开的设备句柄</param>
        /// <returns>0－成功；其他则失败（失败返回其错误码）</returns>
        public bool CloseDevice()
        {
            int Ret = 0;

            try
            {
                LastErrMsg = string.Empty;

                Ret = YCCard.CloseComm(mHandle);

                if (Ret == 0)
                {
                    mIsOpen = false;
                }
                else
                {
                    mHandle = IntPtr.Zero;
                }

                mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 读物理卡号
        /// <summary>
        /// 读物理卡号
        /// </summary>
        /// <param name="SysType"></param>
        /// <param name="CardSerNo"></param>
        /// <returns></returns>
        public bool ReadCardID(ref int SysType, ref uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    Ret = YCCard.ReadCard_ID(mHandle, ref SysType, ref CardSerNo);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 读卡类型
        /// <summary>
        /// 读卡类型
        /// </summary>
        /// <param name="CardType">
        /// 卡片类型
        /// 0：用户卡
        /// 1：操作员卡
        /// 2：系统卡
        /// 3：初始化卡
        /// 4：白卡
        /// 5：节水设置卡
        /// 6：采集卡
        /// 7：加密卡
        /// 8：查询卡
        /// 9：机号设置卡
        /// 10：时间设置卡
        /// </param>
        /// <param name="CardSerNo"></param>
        /// <returns></returns>
        public bool ReadCardType(ref int CardType, ref uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    int SysType = 0;
                    int intsalesOperatorId = 0;
                    byte[] SecretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

                    Ret = YCCard.Query_Card_Type(mHandle, ref SysType, ref CardType, ref intsalesOperatorId, ref CardSerNo, 500, ref SecretKey[0]);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 写卡余
        /// <summary>
        /// 写卡余
        /// </summary>
        /// <param name="CardValue"></param>
        /// <returns></returns>
        public bool WriteCardValue(int CardValue, int SysType, uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    Ret = YCCard.WRT_Pos_UserCard_AddCount12(mHandle, CardValue, CardSerNo, 500);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 写卡余（水控）
        /// <summary>
        /// 写卡余（水控）
        /// </summary>
        /// <param name="CardValue"></param>
        /// <returns></returns>
        public bool WriteCardValueWater(int CardValue, uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    string Time = DateTime.Now.ToString("yyMMddHHmmss");

                    Ret = YCCard.WRT_Js_UserCard_AddCount(mHandle, CardValue, ref Time, CardSerNo);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 读用户卡
        /// <summary>
        /// 读用户卡
        /// </summary>
        /// <param name="mUserCard"></param>
        /// <returns></returns>
        public int ReadCard(UserCard12 mUserCard)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    int CardType = 0;
                    int Opt_Num = 0;
                    int CardID = 0;
                    StringBuilder UserNo = new StringBuilder();
                    int UserType = 0;
                    uint cardserno = 0;
                    int MlngChkSum1 = 0;
                    int Value1 = 0;
                    int LastPay1 = 0;
                    int Count1 = 0;
                    int Consume_Add1 = 0;
                    int ChkSum2 = 0;
                    int Value2 = 0;
                    int LastPay2 = 0;
                    int Count2 = 0;
                    int Consume_Add2 = 0;
                    int Use_Term = 0;
                    int AddCount = 0;
                    byte[] SecretKey = new byte[50];

                    Ret = YCCard.Query_Pos_UserCard12(mHandle,
                                                      ref CardType,
                                                      ref Opt_Num,
                                                      ref CardID,
                                                      UserNo,
                                                      ref UserType,
                                                      ref cardserno,
                                                      ref MlngChkSum1,
                                                      ref Value1,
                                                      ref LastPay1,
                                                      ref Count1,
                                                      ref Consume_Add1,
                                                      ref ChkSum2,
                                                      ref Value2,
                                                      ref LastPay2,
                                                      ref Count2,
                                                      ref Consume_Add2,
                                                      ref Use_Term,
                                                      ref AddCount,
                                                      500,
                                                      ref SecretKey[0]);

                    if (Ret == 0)
                    {
                        mUserCard.CardType = CardType;
                        mUserCard.Opt_Num = Opt_Num;
                        mUserCard.CardID = CardID;
                        mUserCard.UserNo = UserNo.ToString();
                        mUserCard.UserType = UserType;
                        mUserCard.cardserno = cardserno;
                        mUserCard.MlngChkSum1 = MlngChkSum1;
                        mUserCard.Value1 = Value1;
                        mUserCard.LastPay1 = LastPay1;
                        mUserCard.Count1 = Count1;
                        mUserCard.Consume_Add1 = Consume_Add1;
                        mUserCard.ChkSum2 = ChkSum2;
                        mUserCard.Value2 = Value2;
                        mUserCard.LastPay2 = LastPay2;
                        mUserCard.Count2 = Count2;
                        mUserCard.Consume_Add2 = Consume_Add2;
                        mUserCard.Use_Term = Use_Term;
                        mUserCard.AddCount = AddCount;
                        mUserCard.SecretKey = SecretKey;

                        mUserCard.GetOtherInfo();
                    }
                    else
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret;
        }
        #endregion

        #region 修复卡片
        /// <summary>
        /// 修复卡片
        /// </summary>
        /// <param name="Value"></param>
        /// <param name="Systype"></param>
        /// <param name="CardSerno"></param>
        /// <param name="Card_Value1"></param>
        /// <param name="Card_Value2"></param>
        /// <param name="Card_Value"></param>
        /// <returns></returns>
        public int Make_UserCard_New(int Value, int Systype, int CardSerno, ref int Card_Value1, ref int Card_Value2, ref int Card_Value)
        {
            IntPtr CommPtr = IntPtr.Zero;
            int Ret = -1;

            //CommPtr = CommOpen(0);

            //if (CommPtr.ToInt32() <= 0)
            //{
            //    return -1;
            //}

            //try
            //{
            //    Ret = YC_Card.Make_UserCard_New091102(CommPtr, Value, Systype, CardSerno, ref Card_Value1, ref Card_Value2, ref Card_Value, 500);

            //    if (Ret == 0)
            //    {
            //        YC_Card.rf_beep(CommPtr, 10);
            //    }
            //    else
            //    {
            //        LastErrMsg = YC_Card_Code.GetMsgByCode(Ret);
            //    }
            //}
            //catch (Exception ex)
            //{
            //    LastErrMsg = ex.Message;
            //}
            //finally
            //{
            //    CloseComm(CommPtr);
            //}

            return Ret;
        }
        #endregion

        #region 蜂鸣
        /// <summary>
        /// 蜂鸣
        /// </summary>
        /// <returns></returns>
        public void Beep()
        {
            if (mIsOpen)
            {
                YCCard.rf_beep(mHandle, 10);
            }
        }
        #endregion

        #region 初始化卡片
        /// <summary>
        /// 初始化卡片
        /// </summary>
        /// <param name="CardID"></param>
        /// <param name="EmpStrID"></param>
        /// <param name="CardTypeID"></param>
        /// <param name="UseDate"></param>
        /// <param name="IsUseTwoSystem"></param>
        /// <param name="username"></param>
        /// <param name="cardtypename"></param>
        /// <param name="CardSerNo"></param>
        /// <returns></returns>
        public bool InitCard(int CardID, string EmpStrID, int CardTypeID, int UseDate, string EmpName, string CardTypeName, int XCardValue, int WCardValue, bool IsCreditValue, int IssueType, ref uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    byte[] byte_userno;      //用户编号二进制表现形式
                    byte[] byte_username;    //用户姓名二进制表现形式
                    byte[] byte_cardtypename; //卡类名称二进制表现形式

                    //ASCII转成二进制
                    byte_userno = System.Text.Encoding.ASCII.GetBytes(EmpStrID);
                    //简体中文转二进制（gb2312）
                    byte_username = System.Text.Encoding.GetEncoding(936).GetBytes(EmpName);
                    byte_cardtypename = System.Text.Encoding.GetEncoding(936).GetBytes(CardTypeName);

                    int nolength = byte_userno.Length;    //用户编号长度
                    int namelength = byte_username.Length;    //用户姓名长度
                    int cardtypelength = byte_cardtypename.Length;    //用户姓名长度

                    byte[] SecretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

                    byte[] newbytes = new byte[59];
                    Array.Copy(SecretKey, 0, newbytes, 0, SecretKey.Length);

                    //人员编号和姓名不能超过50个字节
                    if ((nolength + namelength + cardtypelength + 12) <= 50)
                    {
                        //赋值数据
                        newbytes[9] = (byte)byte_userno.Length;
                        Array.Copy(byte_userno, 0, newbytes, 10, byte_userno.Length);
                        newbytes[byte_userno.Length + 10] = (byte)byte_username.Length;
                        Array.Copy(byte_username, 0, newbytes, byte_userno.Length + 11, byte_username.Length);
                        newbytes[byte_userno.Length + 11 + byte_username.Length] = (byte)byte_cardtypename.Length;
                        Array.Copy(byte_cardtypename, 0, newbytes, byte_userno.Length + byte_username.Length + 12, byte_cardtypename.Length);
                    }

                    if (IssueType == 0 || IssueType == 1)
                    {
                        //Ret = YCCard.Init_Pos_UserCard12(mHandle, CardID, EmpStrID.Length > 5 ? EmpStrID.Substring(0, 5) : EmpStrID, CardTypeID, 500, ref CardSerNo, UseDate, ref newbytes[0]);
                        Ret = YCCard.ActivatePosUserCard12(mHandle, CardID, 0, 0xAA, 0xCC, CardTypeID, ref CardSerNo, UseDate, ref newbytes[0]);

                        if (Ret != 0)
                        {
                            mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                            return false;
                        }

                        if (IsCreditValue && XCardValue != 0)
                        {
                            Ret = YCCard.WRT_Pos_UserCard_AddCount12(mHandle, XCardValue, CardSerNo, 500);

                            if (Ret != 0)
                            {
                                YCCard.RST_Pos_UserCard12(mHandle, CardSerNo, 500, ref SecretKey[0]);
                                mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                                return false;
                            }
                        }
                    }

                    if (IssueType == 0 || IssueType == 2)
                    {
                        Ret = YCCard.Init_Js_UserCard(mHandle, CardID, EmpStrID.Length > 5 ? EmpStrID.Substring(0, 5) : EmpStrID, CardTypeID, ref CardSerNo, ref SecretKey[0]);

                        if (Ret != 0)
                        {
                            mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                            return false;
                        }

                        if (IsCreditValue && WCardValue != 0)
                        {
                            string Time = DateTime.Now.ToString("yyMMddHHmmss");

                            Ret = YCCard.WRT_Js_UserCard_AddCount(mHandle, WCardValue, ref Time, CardSerNo);

                            if (Ret != 0)
                            {
                                if (IssueType == 0)
                                {
                                    YCCard.RST_Pos_UserCard12(mHandle, CardSerNo, 500, ref SecretKey[0]);
                                }

                                YCCard.RSTJsUserCard(mHandle, CardSerNo, ref SecretKey[0]);
                                mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                                return false;
                            }
                        }
                    }
 
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 初始化水控卡片
        /// <summary>
        /// 初始化水控卡片
        /// </summary>
        /// <param name="CardID"></param>
        /// <param name="EmpStrID"></param>
        /// <param name="CardTypeID"></param>
        /// <param name="UseDate"></param>
        /// <param name="IsUseTwoSystem"></param>
        /// <param name="username"></param>
        /// <param name="cardtypename"></param>
        /// <param name="CardSerNo"></param>
        /// <returns></returns>
        public bool InitJSCard(int CardID, string EmpStrID, int CardTypeID, string EmpName, string CardTypeName, ref uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    byte[] byte_userno;      //用户编号二进制表现形式
                    byte[] byte_username;    //用户姓名二进制表现形式
                    byte[] byte_cardtypename; //卡类名称二进制表现形式

                    //ASCII转成二进制
                    byte_userno = System.Text.Encoding.ASCII.GetBytes(EmpStrID);
                    //简体中文转二进制（gb2312）
                    byte_username = System.Text.Encoding.GetEncoding(936).GetBytes(EmpName);
                    byte_cardtypename = System.Text.Encoding.GetEncoding(936).GetBytes(CardTypeName);

                    int nolength = byte_userno.Length;    //用户编号长度
                    int namelength = byte_username.Length;    //用户姓名长度
                    int cardtypelength = byte_cardtypename.Length;    //用户姓名长度

                    byte[] SecretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

                    byte[] newbytes = new byte[59];
                    Array.Copy(SecretKey, 0, newbytes, 0, SecretKey.Length);

                    //人员编号和姓名不能超过50个字节
                    if ((nolength + namelength + cardtypelength + 12) <= 50)
                    {
                        //赋值数据
                        newbytes[9] = (byte)byte_userno.Length;
                        Array.Copy(byte_userno, 0, newbytes, 10, byte_userno.Length);
                        newbytes[byte_userno.Length + 10] = (byte)byte_username.Length;
                        Array.Copy(byte_username, 0, newbytes, byte_userno.Length + 11, byte_username.Length);
                        newbytes[byte_userno.Length + 11 + byte_username.Length] = (byte)byte_cardtypename.Length;
                        Array.Copy(byte_cardtypename, 0, newbytes, byte_userno.Length + byte_username.Length + 12, byte_cardtypename.Length);
                    }

                    mLastErrMsg = string.Empty;

                    Ret = YCCard.Init_Js_UserCard(mHandle, CardID, EmpStrID.Length > 5 ? EmpStrID.Substring(0, 5) : EmpStrID, CardTypeID, ref CardSerNo, ref SecretKey[0]);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                        return false;
                    }

                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 回收卡片
        /// <summary>
        /// 回收卡片
        /// </summary>
        /// <param name="CardValue"></param>
        /// <returns></returns>
        public bool RST_Card(uint CardSerNo)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    byte[] SecretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

                    Ret = YCCard.RST_Pos_UserCard12(mHandle, CardSerNo, 500, ref SecretKey[0]);

                    if (Ret != 0)
                    {
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 自助补发卡功能 

        #region 取扇区号
        /// <summary>
        /// 取得扇区号
        /// </summary>
        /// <param name="Sector"></param>
        /// <returns></returns>
        public bool GetSector(ref uint XSector, ref uint WSector)
        {
            int Ret = -1;

            try
            {
                mLastErrMsg = string.Empty;

                Ret = YCCard.SDFZ_GetSectorNumber(ref XSector, ref WSector);

                if (Ret != 0)
                {
                    mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 密钥计算
        /// <summary>
        /// 密钥计算
        /// </summary>
        /// <param name="Sector"></param>
        /// <returns></returns>
        public bool GetKey(uint CardSerNo, byte[] BaseKey, byte EncryptMode, ref byte[] Key)
        {
            int Ret = -1;

            try
            {
                mLastErrMsg = string.Empty;

                if (BaseKey == null)
                {
                    BaseKey = new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                }

                Ret = YCCard.SDFZ_CalculateDynamicKey(CardSerNo, ref BaseKey[0], ref Key[0], EncryptMode);

                if (Ret != 0)
                {
                    mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 初始化发卡数据
        /// <summary>
        /// 初始化发卡数据
        /// </summary>
        /// <param name="CardID">物理ID</param>
        /// <param name="Serno">卡序列号</param>
        /// <param name="UserNumber">用户编号</param>
        /// <param name="EmpName">用户姓名</param>
        /// <param name="CardTypeName">卡类名称</param>
        /// <param name="CardType">卡类</param>
        /// <param name="use_term">有效期</param>
        /// <param name="Buff">写入卡片数据缓存指针（128个字节），前面64个字节属于消费钱包扇区，后面64个字节属于用户信息</param>
        /// <returns></returns>
        public bool InitCard_CRT_591(int CardID, uint SerNo, string UserNumber, string EmpName, string CardTypeName, int CardTyp, int use_term, out byte[] Buff)
        {
            int Ret = -1;

            Buff = new byte[128];

            try
            {
                mLastErrMsg = string.Empty;

                byte[] byte_userno;      //用户编号二进制表现形式
                byte[] byte_username;    //用户姓名二进制表现形式
                byte[] byte_cardtypename; //卡类名称二进制表现形式

                //ASCII转成二进制
                byte_userno = System.Text.Encoding.ASCII.GetBytes(UserNumber);
                //简体中文转二进制（gb2312）
                byte_username = System.Text.Encoding.GetEncoding(936).GetBytes(EmpName);
                byte_cardtypename = System.Text.Encoding.GetEncoding(936).GetBytes(CardTypeName);

                int nolength = byte_userno.Length;    //用户编号长度
                int namelength = byte_username.Length;    //用户姓名长度
                int cardtypelength = byte_cardtypename.Length;    //用户姓名长度

                byte[] newbytes = new byte[50];

                //人员编号和姓名不能超过50个字节
                if ((nolength + namelength + cardtypelength + 12) <= 50)
                {
                    //赋值数据
                    newbytes[0] = (byte)byte_userno.Length;
                    Array.Copy(byte_userno, 0, newbytes, 1, byte_userno.Length);
                    newbytes[byte_userno.Length + 1] = (byte)byte_username.Length;
                    Array.Copy(byte_username, 0, newbytes, byte_userno.Length + 2, byte_username.Length);
                    newbytes[byte_userno.Length + 2 + byte_username.Length] = (byte)byte_cardtypename.Length;
                    Array.Copy(byte_cardtypename, 0, newbytes, byte_userno.Length + byte_username.Length + 3, byte_cardtypename.Length);
                }

                Ret = YCCard.SDFZ_InitPosUserCard12(SerNo, CardID, UserNumber.Length > 5 ? UserNumber.Substring(0, 5) : UserNumber, CardTyp, use_term, ref newbytes[0], ref Buff[0]);

                if (Ret != 0)
                {
                    mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #region 写卡余
        /// <summary>
        /// 写卡余
        /// </summary>
        /// <param name="Sector"></param>
        /// <returns></returns>
        public bool WalletReCharge(int Value, byte[] InData, ref byte[] OutData)
        {
            int Ret = -1;

            try
            {
                mLastErrMsg = string.Empty;

                Ret = YCCard.SDFZ_WalletReCharge(Value, ref InData[0], ref OutData[0]);

                if (Ret != 0)
                {
                    mLastErrMsg = ErrMsg.GetMsgByCode(Ret);
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret == 0;
        }
        #endregion

        #endregion

        #region 读用节能卡余
        /// <summary>
        /// 读用节能卡余
        /// </summary>
        /// <param name="mUserCard"></param>
        /// <returns></returns>
        public int ReadJSCard(UserCard12 mUserCard)
        {
            int Ret = -1;

            try
            {
                if (mIsOpen)
                {
                    mLastErrMsg = string.Empty;

                    int CardType = 0;
                    int Opt_Num = 0;
                    int CardID = 0;
                    StringBuilder UserNo = new StringBuilder();
                    int UserType = 0;
                    int cardserno = 0;
                    int JSValue = 0;
                    int JSCount = 0;
                    byte[] SecretKey = new byte[50];
                    SecretKey[8] = 1;

                    Ret = YCCard.QueryJsCard(mHandle,
                                              ref CardType,
                                              ref Opt_Num,
                                              ref CardID,
                                              UserNo,
                                              ref cardserno,
                                              ref JSValue,
                                              ref JSCount,
                                              ref UserType,
                                              500,
                                              ref SecretKey[0]);

                    if (Ret == 0 && CardID != 0 && CardID == mUserCard.CardID)
                    {
                        mUserCard.JSValue = JSValue;
                        mUserCard.JSCount = JSCount;
                        mUserCard.WaterBagValue = JSValue;
                    }
                    else
                    {
                        mUserCard.JSValue = JSValue;
                        mUserCard.JSCount = JSCount;
                        mLastErrMsg = ErrMsg.GetMsgByCode(Ret);

                        Ret = -1;
                    }
                }
            }
            catch (Exception ex)
            {
                mLastErrMsg = ex.Message;
            }

            return Ret;
        }
        #endregion
    }
}

/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-08-02
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* NameSpace:       YC.SelfCardSys.CardOperate
* Description:     V1.2用户卡结构
* ***********************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace YC.SelfCardSys.CardOperate
{
    public class UserCard12
    {
        #region 卡片数据
        public int CardType { get; set; }
        public int Opt_Num { get; set; }
        public int CardID { get; set; }
        public string UserNo { get; set; }
        public int UserType { get; set; }
        public uint cardserno { get; set; }
        public int MlngChkSum1 { get; set; }
        public int Value1 { get; set; }
        public int LastPay1 { get; set; }
        public int Count1 { get; set; }
        public int Consume_Add1 { get; set; }
        public int ChkSum2 { get; set; }
        public int Value2 { get; set; }
        public int LastPay2 { get; set; }
        public int Count2 { get; set; }
        public int Consume_Add2 { get; set; }
        public int Use_Term { get; set; }
        public int AddCount { get; set; }
        public int JSValue { get; set; }
        public int JSCount { get; set; }
        public byte[] SecretKey { get; set; }

        public string EmpStrID { get; set; }
        public string EmpName { get; set; }
        public string EmpCardType { get; set; }

        public int WaterBagValue { get; set; }
        #endregion

        #region 库中数据
        /// <summary>
        /// 人员序号
        /// </summary>
        public long EmployeeID { get; set; }
        /// <summary>
        /// 人员状态
        /// </summary>
        public string EmpStatusStr { get; set; }
        /// <summary>
        /// 卡类型
        /// </summary>
        public int CardTypeID { get; set; }
        /// <summary>
        /// 卡类型（字符串）
        /// </summary>
        public string CardTypeStr { get; set; }
        /// <summary>
        /// 卡片状态
        /// </summary>
        public int CardStatus { get; set; }
        /// <summary>
        /// 卡片状态（字符串）
        /// </summary>
        public string CardStatusStr { get; set; }
        /// <summary>
        /// 卡有效期
        /// </summary>
        public string CardValidate { get; set; }
        /// <summary>
        /// 身份证号
        /// </summary>
        public string Identifier { get; set; }
        /// <summary>
        /// 所在部门
        /// </summary>
        public string DepartmentName { get; set; }
        /// <summary>
        /// 入职日期
        /// </summary>
        public string ContractDate { get; set; }
        /// <summary>
        /// 家庭地址
        /// </summary>
        public string Address { get; set; }
        /// <summary>
        /// 用户密码
        /// </summary>
        public string EmpPwd { get; set; }
        /// <summary>
        /// 库中卡余（消费）
        /// </summary>
        public decimal XCardValue { get; set; }
        /// <summary>
        /// 库中卡余（水控）
        /// </summary>
        public decimal WCardValue { get; set; }
        /// <summary>
        /// 补卡费用
        /// </summary>
        public decimal FeeValue { get; set; }
        /// <summary>
        /// 补卡余款充值金额
        /// </summary>
        public decimal CreditValue { get; set; }
        /// <summary>
        /// 照片
        /// </summary>
        public byte[] Photo { get; set; }

        /// <summary>
        /// 其他图片
        /// </summary>
        public byte[] NImage { get; set; }

        /// <summary>
        /// 是否正在初始化
        /// </summary>
        public bool IsIniting { get; set; }
        #endregion

        #region 卡操作数据
        public int WriteCardStatus;           //写卡结果
        #endregion

        public UserCard12()
        {
            CardType = 0;
            Opt_Num = 0;
            CardID = 0;
            UserNo = string.Empty;
            UserType = 0;
            cardserno = 0;
            MlngChkSum1 = 0;
            Value1 = 0;
            LastPay1 = 0;
            Count1 = 0;
            Consume_Add1 = 0;
            ChkSum2 = 0;
            Value2 = 0;
            LastPay2 = 0;
            Count2 = 0;
            Consume_Add2 = 0;
            Use_Term = 0;
            AddCount = 0;
            SecretKey = null;
            WriteCardStatus = 4;
            SecretKey = new byte[50];
            EmployeeID = 0;
        }

        public void GetOtherInfo()
        {
            try
            {
                //取人员编号
                int StartIndex = 9;

                if (SecretKey[9] > SecretKey.Length - StartIndex - 1)
                {
                    EmpStrID = System.Text.Encoding.GetEncoding("ASCII").GetString(SecretKey, StartIndex + 1, SecretKey.Length - StartIndex - 1);
                }
                else
                {
                    EmpStrID = System.Text.Encoding.GetEncoding("ASCII").GetString(SecretKey, StartIndex + 1, SecretKey[9]);
                }

                //起始位+人员编号的长度位1+人员编号的长度
                StartIndex = StartIndex + 1 + SecretKey[9];

                if (StartIndex >= SecretKey.Length)
                {
                    EmpName = string.Empty;
                }
                else
                {
                    EmpName = System.Text.Encoding.GetEncoding("GB2312").GetString(SecretKey, StartIndex + 1, SecretKey[StartIndex]);

                    if (EmpName.IndexOf('\0') > 0)
                    {
                        EmpName = EmpName.Substring(0, EmpName.Length - EmpName.IndexOf('\0'));
                    }

                    //起始位+人员姓名的长度位1+人员姓名的长度
                    StartIndex = StartIndex + 1 + SecretKey[StartIndex];

                    if (StartIndex >= SecretKey.Length)
                    {
                        EmpCardType = string.Empty;
                    }
                    else
                    {
                        EmpCardType = System.Text.Encoding.GetEncoding("GB2312").GetString(SecretKey, StartIndex + 1, SecretKey[StartIndex]);

                        if (EmpCardType.IndexOf('\0') > 0)
                        {
                            EmpCardType = EmpCardType.Substring(0, EmpCardType.Length - EmpCardType.IndexOf('\0'));
                        }
                    }
                }
            }
            catch
            {
                EmpName = string.Empty;
                EmpCardType = string.Empty;
            }
        }
    }
}

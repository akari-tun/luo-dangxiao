/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-08-02
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* NameSpace:       YC.SelfCardSys.CardOperate
* Description:     读写器错误信息
* ***********************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace YC.SelfCardSys.CardOperate
{
    public static class ErrMsg
    {
        static Hashtable mErrMsg = null;

        public static void Init()
        {
            mErrMsg = new Hashtable();

            mErrMsg.Add(0, "命令正确执行");
            mErrMsg.Add(-1, "端口打开错误");
            mErrMsg.Add(-2, "读写器连接错误");
            mErrMsg.Add(-3, "没有消费系统授权信息");
            mErrMsg.Add(-4, "没有门禁系统授权信息");
            mErrMsg.Add(-5, "参数错误");
            mErrMsg.Add(-6, "操作超时");
            mErrMsg.Add(-7, "感应区找不到卡片");
            mErrMsg.Add(-8, "找不到门禁授权卡");
            mErrMsg.Add(-9, "找不到消费授权卡");
            mErrMsg.Add(-10, "系统卡错误");
            mErrMsg.Add(-11, "用户卡数据错误");
            mErrMsg.Add(-12, "读用户卡错误");
            mErrMsg.Add(-13, "写用户卡错误");
            mErrMsg.Add(-14, "创建授权文件失败");
            mErrMsg.Add(-15, "不可识别卡片（非法卡）");
            mErrMsg.Add(-16, "找不到授权文件");
            mErrMsg.Add(-17, "找不到加密KEY文件");
            mErrMsg.Add(-18, "没有节水系统授权信息");
            mErrMsg.Add(-19, "无结束系统权限");
            mErrMsg.Add(-20, "");
            mErrMsg.Add(-21, "不是CPU卡");
            mErrMsg.Add(-22, "选择文件错误");
            mErrMsg.Add(-23, "获取随机数错误");
            mErrMsg.Add(-24, "外部认证错误");
            mErrMsg.Add(-25, "写文件数据失败");
            mErrMsg.Add(-26, "读文件数据失败");
            mErrMsg.Add(-27, "");
            mErrMsg.Add(-28, "");
            mErrMsg.Add(-29, "用户卡修复失败");
            mErrMsg.Add(-30, "不是合法采集");
            mErrMsg.Add(-61, "读写器连接失败");
            mErrMsg.Add(-70, "数据接收错误");
            mErrMsg.Add(-71, "数据接收错误");
            mErrMsg.Add(-72, "数据接收错误");
            mErrMsg.Add(-73, "");
            mErrMsg.Add(-74, "");
            mErrMsg.Add(-75, "数据接收错误");
            mErrMsg.Add(-76, "数据接收错误");
            mErrMsg.Add(-77, "数据接收错误");
            mErrMsg.Add(-82, "数据接收错误");
            mErrMsg.Add(-84, "数据接收失败");
            mErrMsg.Add(-85, "数据发送失败");
        }

        public static string GetMsgByCode(int ErrCode)
        {
            string ErrMsg = string.Empty;

            if (mErrMsg.ContainsKey(ErrCode))
            {
                ErrMsg = (string)mErrMsg[ErrCode];
            }

            return ErrMsg;
        }
    }
}

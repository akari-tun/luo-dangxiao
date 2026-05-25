/* ***********************************************
* Author:          谭骏 (32965926@qq.com)
* Create Time:     2013-08-02
* CopyRight:       Copyright (C) 2008-2013 深圳市宇川智能系统有限公司 All Rights Reserved
* NameSpace:       YC.SelfCardSys.CardOperate
* Description:     卡片操作枚举类
* ***********************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace YC.SelfCardSys.CardOperate
{
    public enum CardOperatType
    {
        WaitOperate,
        WriteWaterValue,
        ReadCardID,
        ReadCardInfo,
        LostCard,
        ChangePwd,
        ShowMsg,
        PrintCard,
        ReCard,
        InitWater,
        CD800ReCard
    }
}
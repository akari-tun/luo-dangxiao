using Avalonia.Styling;
using luo.dangxiao.common.Enums;
using System;
using System.Collections.Generic;
using System.Text;

namespace luo.dangxiao.models
{
    public class CfgDataModel
    {
        public SelfServiceType ServiceType { get; set; } = SelfServiceType.StudentSelfService;
        public ThemeVariant Theme { get; set; } = ThemeVariant.Light;
    }
}

using luo.dangxiao.common.Enums;

namespace luo.dangxiao.models
{
    public sealed class SelfServiceConfig : ConfigModel
    {
        public SelfServiceType ServiceType { get; set; } = SelfServiceType.StudentSelfService;
    }
}

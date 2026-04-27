using luo.dangxiao.common.Enums;

namespace luo.dangxiao.models
{
    /// <summary>
    /// Root configuration for the self-service application.
    /// </summary>
public sealed class SelfServiceConfig : ConfigModel
{
    /// <summary>
    /// Gets or sets the self-service mode loaded by the application.
    /// </summary>
    public SelfServiceType ServiceType { get; set; } = SelfServiceType.StudentSelfService;

    /// <summary>
    /// Gets or sets the countdown duration, in seconds, used by self-service pages.
    /// </summary>
    public int CountdownSeconds { get; set; } = 60;

    /// <summary>
    /// Gets or sets the tenant identifier used by self-service API requests.
    /// </summary>
    public string TenantId { get; set; } = string.Empty;
}
}

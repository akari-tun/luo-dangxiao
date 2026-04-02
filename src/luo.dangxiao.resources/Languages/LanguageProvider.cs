using System.Globalization;
using System.Resources;

namespace luo.dangxiao.resources.Languages;

public static class LanguageProvider
{
    private static readonly ResourceManager ResourceManager = new("luo.dangxiao.resources.Languages.Language", typeof(LanguageProvider).Assembly);

    private static string Get(string key) => ResourceManager.GetString(key, CultureInfo.CurrentUICulture) ?? key;

    public static string App_Title => Get(nameof(App_Title));
    public static string SelfService_TakeCard => Get(nameof(SelfService_TakeCard));
    public static string SelfService_ReportLoss => Get(nameof(SelfService_ReportLoss));
    public static string SelfService_Replacement => Get(nameof(SelfService_Replacement));
    public static string SelfService_Query => Get(nameof(SelfService_Query));
    public static string SelfService_Recharge => Get(nameof(SelfService_Recharge));
    public static string SelfService_CheckIn => Get(nameof(SelfService_CheckIn));
}

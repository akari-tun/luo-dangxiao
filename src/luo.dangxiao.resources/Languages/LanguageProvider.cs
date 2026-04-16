using System.Globalization;
using System.Resources;

namespace luo.dangxiao.resources.Languages;

public static class LanguageProvider
{
    private static readonly ResourceManager ResourceManager = new("luo.dangxiao.resources.Languages.Language", typeof(LanguageProvider).Assembly);

    private static string Get(string key) => ResourceManager.GetString(key, CultureInfo.CurrentUICulture) ?? key;

    public static string App_Title => Get(nameof(App_Title));
    public static string Button_Back => Get(nameof(Button_Back));

    public static string SelfService_TakeCard => Get(nameof(SelfService_TakeCard));
    public static string SelfService_ReportLoss => Get(nameof(SelfService_ReportLoss));
    public static string SelfService_Replacement => Get(nameof(SelfService_Replacement));
    public static string SelfService_Query => Get(nameof(SelfService_Query));
    public static string SelfService_Recharge => Get(nameof(SelfService_Recharge));
    public static string SelfService_CheckIn => Get(nameof(SelfService_CheckIn));

    public static string SelfService_Verify_SelectMethod => Get(nameof(SelfService_Verify_SelectMethod));
    public static string SelfService_Verify_IDCard => Get(nameof(SelfService_Verify_IDCard));
    public static string SelfService_Verify_SMS => Get(nameof(SelfService_Verify_SMS));
    public static string SelfService_Verify_IDCardInstruction => Get(nameof(SelfService_Verify_IDCardInstruction));
    public static string SelfService_Verify_IDCardZoneTitle => Get(nameof(SelfService_Verify_IDCardZoneTitle));
    public static string SelfService_Verify_IDCardWaiting => Get(nameof(SelfService_Verify_IDCardWaiting));
    public static string SelfService_Verify_IDCardProcessing => Get(nameof(SelfService_Verify_IDCardProcessing));
    public static string SelfService_Verify_IDCardSuccess => Get(nameof(SelfService_Verify_IDCardSuccess));
    public static string SelfService_Verify_IDCardFailed => Get(nameof(SelfService_Verify_IDCardFailed));
    public static string SelfService_Verify_SwitchToSMS => Get(nameof(SelfService_Verify_SwitchToSMS));
    public static string SelfService_Verify_SMS_Title => Get(nameof(SelfService_Verify_SMS_Title));
    public static string SelfService_Verify_SMS_Phone => Get(nameof(SelfService_Verify_SMS_Phone));
    public static string SelfService_Verify_SMS_Code => Get(nameof(SelfService_Verify_SMS_Code));
    public static string SelfService_Verify_SMS_PhonePlaceholder => Get(nameof(SelfService_Verify_SMS_PhonePlaceholder));
    public static string SelfService_Verify_SMS_CodePlaceholder => Get(nameof(SelfService_Verify_SMS_CodePlaceholder));
    public static string SelfService_Verify_SMS_SendCode => Get(nameof(SelfService_Verify_SMS_SendCode));
    public static string SelfService_Verify_SMS_Resend => Get(nameof(SelfService_Verify_SMS_Resend));
    public static string SelfService_Verify_SMS_Next => Get(nameof(SelfService_Verify_SMS_Next));
    public static string SelfService_Verify_SwitchToIDCard => Get(nameof(SelfService_Verify_SwitchToIDCard));

    public static string SelfService_Info_BasicInfo => Get(nameof(SelfService_Info_BasicInfo));
    public static string SelfService_Info_Name => Get(nameof(SelfService_Info_Name));
    public static string SelfService_Info_StaffNumber => Get(nameof(SelfService_Info_StaffNumber));
    public static string SelfService_Info_CardType => Get(nameof(SelfService_Info_CardType));
    public static string SelfService_Info_CardExpiryDate => Get(nameof(SelfService_Info_CardExpiryDate));
    public static string SelfService_Info_Department => Get(nameof(SelfService_Info_Department));
    public static string SelfService_Info_ConsumptionBalance => Get(nameof(SelfService_Info_ConsumptionBalance));
    public static string SelfService_Info_SubsidyBalance => Get(nameof(SelfService_Info_SubsidyBalance));
    public static string SelfService_Info_CheckInRoom => Get(nameof(SelfService_Info_CheckInRoom));
    public static string SelfService_Info_CheckInTime => Get(nameof(SelfService_Info_CheckInTime));
    public static string SelfService_Info_RangeSeparator => Get(nameof(SelfService_Info_RangeSeparator));
    public static string SelfService_Info_ClassName => Get(nameof(SelfService_Info_ClassName));
    public static string SelfService_Info_TrainingTime => Get(nameof(SelfService_Info_TrainingTime));
    public static string SelfService_TakeCard_Title_PendingPickup => Get(nameof(SelfService_TakeCard_Title_PendingPickup));
    public static string SelfService_TakeCard_Title_Lost => Get(nameof(SelfService_TakeCard_Title_Lost));
    public static string SelfService_TakeCard_Title_Normal => Get(nameof(SelfService_TakeCard_Title_Normal));
    public static string SelfService_TakeCard_Title_Other => Get(nameof(SelfService_TakeCard_Title_Other));
    public static string SelfService_TakeCard_Button_Confirm => Get(nameof(SelfService_TakeCard_Button_Confirm));
    public static string SelfService_TakeCard_Button_PickedUp => Get(nameof(SelfService_TakeCard_Button_PickedUp));
    public static string SelfService_TakeCard_Button_Complete => Get(nameof(SelfService_TakeCard_Button_Complete));
}

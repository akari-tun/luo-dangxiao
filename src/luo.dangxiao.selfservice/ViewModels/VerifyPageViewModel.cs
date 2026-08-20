using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;

namespace luo.dangxiao.selfservice.ViewModels;



/// <summary>
/// ViewModel for VerifyPage.
/// </summary>
public partial class VerifyPageViewModel : ViewModelBase, IPageViewModel
{
    [ObservableProperty]
    private VerifyMethod _verifyMethod = VerifyMethod.IDCard;

    [ObservableProperty]
    private string _targetFunction = string.Empty;

    [ObservableProperty]
    private IPageView _verifyPageContent;

    private readonly IDCardVerifyPageView _idCardView;
    private readonly SMSVerifyPageView _smsModuleView;

    public VerifyPageViewModel() : base()
    {
        _idCardView = new IDCardVerifyPageView();
        _smsModuleView = new SMSVerifyPageView();

        if (_idCardView.DataContext is IDCardVerifyPageViewModel idCardViewModel)
        {
            idCardViewModel.VerificationSucceeded += OnVerificationSucceeded;
        }

        if (_smsModuleView.DataContext is SMSVerifyPageViewModel smsVerifyPageViewModel)
        {
            smsVerifyPageViewModel.VerificationSucceeded += OnSmsVerificationSucceeded;
        }

        VerifyPageContent = _idCardView;
    }

    public bool IsIDCardSelected => VerifyMethod == VerifyMethod.IDCard;

    public bool IsSMSSelected => VerifyMethod == VerifyMethod.SMS;

    partial void OnVerifyMethodChanged(VerifyMethod value)
    {
        OnPropertyChanged(nameof(IsIDCardSelected));
        OnPropertyChanged(nameof(IsSMSSelected));
    }

    [RelayCommand]
    private void SwitchToIDCard()
    {
        LogCommand(nameof(SwitchToIDCard));
        VerifyMethod = VerifyMethod.IDCard;
        VerifyPageContent = _idCardView;
    }

    [RelayCommand]
    private void SwitchToSMS()
    {
        LogCommand(nameof(SwitchToSMS));
        VerifyMethod = VerifyMethod.SMS;
        VerifyPageContent = _smsModuleView;
    }

    private void OnVerificationSucceeded(object? sender, IDCardVerificationSucceededEventArgs e)
    {
        NavigateAfterVerification(e.UserInfo);
    }

    private void OnSmsVerificationSucceeded(object? sender, SmsVerificationSucceededEventArgs e)
    {
        NavigateAfterVerification(e.UserInfo);
    }

    private void NavigateAfterVerification(UserInfoModel userInfo)
    {
        var homePage = Ioc.Default.GetRequiredService<HomePageViewModel>();

        if (TargetFunction == "TakeCard")
        {
            homePage.NavigateToTakeCard(userInfo);
            return;
        }

        if (TargetFunction == "CheckIn")
        {
            homePage.NavigateToCheckIn(userInfo);
            return;
        }

        if (TargetFunction == "ReportLoss")
        {
            homePage.NavigateToReportLoss(userInfo);
            return;
        }

        if (TargetFunction == "Replacement")
        {
            homePage.NavigateToReplacement(userInfo);
            return;
        }

        if (TargetFunction == "Recharge")
        {
            homePage.SubPageContent = new RechargePageView(new RechargePageParameter
            {
                TargetFunction = "Recharge",
                Data = userInfo
            });
            return;
        }

        homePage.NavigateToUserInfo(userInfo, TargetFunction);
    }
}

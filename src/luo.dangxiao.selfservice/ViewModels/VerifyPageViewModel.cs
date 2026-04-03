using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.Views;

namespace luo.dangxiao.selfservice.ViewModels;



/// <summary>
/// ViewModel for VerifyPage.
/// </summary>
public partial class VerifyPageViewModel : ViewModelBase
{
    [ObservableProperty]
    private VerifyMethod _verifyMethod = VerifyMethod.IDCard;

    [ObservableProperty]
    private string _targetFunction = string.Empty;

    [ObservableProperty]
    private IPageView _verifyPageContent;

    private IDCardVerifyPageView _idCardView;
    private SMSVerifyPageView _smsModuleView;

    public VerifyPageViewModel()
    {
        _idCardView = new IDCardVerifyPageView();
        _smsModuleView = new SMSVerifyPageView();

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
        VerifyMethod = VerifyMethod.IDCard;
        VerifyPageContent = _idCardView;
    }

    [RelayCommand]
    private void SwitchToSMS()
    {
        VerifyMethod = VerifyMethod.SMS;
        VerifyPageContent = _smsModuleView;
    }
}

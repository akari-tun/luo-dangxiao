using Avalonia.Controls;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.resources.Languages;
using System.Globalization;
using System.Threading.Tasks;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Parameter for recharge page.
/// </summary>
public sealed class RechargePageParameter
{
    public string TargetFunction { get; set; } = "Recharge";
    public UserInfoModel? Data { get; set; }
}

public enum RechargeFlowState
{
    Confirm,
    QRCode
}

public partial class RechargePageViewModel : ViewModelBase
{
    [ObservableProperty]
    private RechargeFlowState _currentState = RechargeFlowState.Confirm;

    [ObservableProperty]
    private UserInfoModel? _userInfo;

    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = "Recharge";

    [ObservableProperty]
    private string _pageTitle = LanguageProvider.SelfService_Recharge_Title_Confirm;

    [ObservableProperty]
    private string _amountSelectionTitle = LanguageProvider.SelfService_Recharge_AmountSelectionTitle;

    [ObservableProperty]
    private string _amount20Text = LanguageProvider.SelfService_Recharge_Amount_20;

    [ObservableProperty]
    private string _amount50Text = LanguageProvider.SelfService_Recharge_Amount_50;

    [ObservableProperty]
    private string _amount100Text = LanguageProvider.SelfService_Recharge_Amount_100;

    [ObservableProperty]
    private string _amount200Text = LanguageProvider.SelfService_Recharge_Amount_200;

    [ObservableProperty]
    private string _amount500Text = LanguageProvider.SelfService_Recharge_Amount_500;

    [ObservableProperty]
    private string _statusMessage = string.Empty;

    [ObservableProperty]
    private bool _isBusy;

    [ObservableProperty]
    private string _qrCodeImage = string.Empty; // resource path placeholder

    [ObservableProperty]
    private decimal _selectedAmount;

    public bool IsConfirmState => CurrentState == RechargeFlowState.Confirm;
    public bool IsQRCodeState => CurrentState == RechargeFlowState.QRCode;

    public bool CanRecharge => IsConfirmState && !IsBusy;

    partial void OnCurrentStateChanged(RechargeFlowState value)
    {
        if (value == RechargeFlowState.Confirm)
        {
            PageTitle = LanguageProvider.SelfService_Recharge_Title_Confirm;
            StatusMessage = string.Empty;
        }
        else
        {
            PageTitle = LanguageProvider.SelfService_Recharge_Title_QRCode;
        }

        OnPropertyChanged(nameof(IsConfirmState));
        OnPropertyChanged(nameof(IsQRCodeState));
        OnPropertyChanged(nameof(CanRecharge));
    }

    partial void OnIsBusyChanged(bool value)
    {
        IsBusy = value;
        OnPropertyChanged(nameof(CanRecharge));
        RechargeCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand]
    private void LoadData(RechargePageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        CurrentState = RechargeFlowState.Confirm;

        LoadUserInfoModule(parameter.Data);
    }

    [RelayCommand(CanExecute = nameof(CanRecharge))]
    private async Task RechargeAsync(object amount)
    {
        if (amount == null) return;

        if (!decimal.TryParse(amount.ToString(), out var amt))
            return;

        SelectedAmount = amt;

        // move to QR code state
        CurrentState = RechargeFlowState.QRCode;
        // placeholder qr code image (use resource)
        QrCodeImage = "avares://luo.dangxiao.resources/Images/qr_placeholder.png";
        StatusMessage = string.Format(CultureInfo.CurrentCulture, LanguageProvider.SelfService_Recharge_Status_PendingPayment, SelectedAmount);

        // Simulate generating order and QR code
        OnIsBusyChanged(true);
        await Task.Delay(300);
        OnIsBusyChanged(false);

        // TODO: start polling payment status and validity timer
    }

    protected override void Back()
    {
        if (IsQRCodeState)
        {
            CurrentState = RechargeFlowState.Confirm;
            QrCodeImage = string.Empty;
            StatusMessage = string.Empty;
            OnIsBusyChanged(false);

            return;
        }

        base.Back();
    }

    private void LoadUserInfoModule(UserInfoModel? data)
    {
        if (data is StaffInfoModel staff)
        {
            var moduleView = new StaffInfoPageView();
            if (moduleView.DataContext is StaffInfoPageViewModel vm)
            {
                vm.LoadDataCommand.Execute(new StaffInfoPageParameter
                {
                    Data = staff,
                    Mode = StaffInfoDisplayMode.Standard
                });
            }

            UserInfoModuleContent = moduleView;
            return;
        }

        UserInfoModuleContent = null;
    }
}

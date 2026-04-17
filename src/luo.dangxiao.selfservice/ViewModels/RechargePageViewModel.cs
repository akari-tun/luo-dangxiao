using Avalonia.Controls;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;
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
    private string _pageTitle = "请确认信息后，点击相应金额进行充值";

    [ObservableProperty]
    private string _amountSelectionTitle = "请选择充值金额";

    [ObservableProperty]
    private string _amount20Text = "￥20元";

    [ObservableProperty]
    private string _amount50Text = "￥50元";

    [ObservableProperty]
    private string _amount100Text = "￥100元";

    [ObservableProperty]
    private string _amount200Text = "￥200元";

    [ObservableProperty]
    private string _amount500Text = "￥500元";

    [ObservableProperty]
    private bool _isAmount20Selected;

    [ObservableProperty]
    private bool _isAmount50Selected;

    [ObservableProperty]
    private bool _isAmount100Selected;

    [ObservableProperty]
    private bool _isAmount200Selected;

    [ObservableProperty]
    private bool _isAmount500Selected;

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
            PageTitle = "请确认信息后，点击相应金额进行充值";
            StatusMessage = string.Empty;
        }
        else
        {
            PageTitle = "确认支付金额后，扫描二维码进行支付";
        }

        OnPropertyChanged(nameof(IsConfirmState));
        OnPropertyChanged(nameof(IsQRCodeState));
        OnPropertyChanged(nameof(CanRecharge));
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(CanRecharge));
        RechargeCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand]
    private void LoadData(RechargePageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        CurrentState = RechargeFlowState.Confirm;
        IsAmount20Selected = false;
        IsAmount50Selected = false;
        IsAmount100Selected = false;
        IsAmount200Selected = false;
        IsAmount500Selected = false;

        LoadUserInfoModule(parameter.Data);
    }

    [RelayCommand(CanExecute = nameof(CanRecharge))]
    private async Task RechargeAsync(object amount)
    {
        if (amount == null) return;

        if (!decimal.TryParse(amount.ToString(), out var amt))
            return;

        SelectedAmount = amt;

        IsAmount20Selected = amt == 20;
        IsAmount50Selected = amt == 50;
        IsAmount100Selected = amt == 100;
        IsAmount200Selected = amt == 200;
        IsAmount500Selected = amt == 500;

        // move to QR code state
        CurrentState = RechargeFlowState.QRCode;
        // placeholder qr code image (use resource)
        QrCodeImage = "avares://luo.dangxiao.resources/Images/qr_placeholder.png";
        StatusMessage = $"待支付金额：¥{SelectedAmount:0.##}";

        // Simulate generating order and QR code
        IsBusy = true;
        await Task.Delay(300);
        IsBusy = false;

        // TODO: start polling payment status and validity timer
    }

    protected override void Back()
    {
        if (IsQRCodeState)
        {
            CurrentState = RechargeFlowState.Confirm;
            QrCodeImage = string.Empty;
            StatusMessage = string.Empty;
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

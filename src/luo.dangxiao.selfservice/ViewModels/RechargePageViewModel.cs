using Avalonia.Controls;
using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Requests;
using System.Globalization;
using System.Text.Json;
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

/// <summary>
/// Represents the flow state of the recharge page.
/// </summary>
public enum RechargeFlowState
{
    Confirm,
    QRCode
}

/// <summary>
/// ViewModel for RechargePage.
/// </summary>
public partial class RechargePageViewModel : ViewModelBase
{
    public RechargePageViewModel() : base()
    {
    }

    private const int DefaultCountdownSeconds = 60;

    private DispatcherTimer? _countdownTimer;
    private bool _pageCleanupInProgress;
    private int _countdownSecondsRemaining;
    private string _operationStatusText = string.Empty;

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
    private string _qrCodeImage = string.Empty;

    [ObservableProperty]
    private decimal _selectedAmount;

    public bool IsConfirmState => CurrentState == RechargeFlowState.Confirm;
    public bool IsQRCodeState => CurrentState == RechargeFlowState.QRCode;
    public bool CanRecharge => IsConfirmState && !IsBusy;

    /// <summary>
    /// Gets a value indicating whether the amount selection buttons should be visible.
    /// Hidden during QR code generation to show operation status instead.
    /// </summary>
    public bool IsAmountSelectionVisible => IsConfirmState && !IsBusy;

    public string CountdownDisplay => string.Format(
        CultureInfo.CurrentUICulture,
        LanguageProvider.SelfService_Recharge_Countdown_Format,
        CountdownSecondsRemaining);

    public bool IsCountdownVisible => CountdownSecondsRemaining > 0;

    /// <summary>
    /// Gets the localized status text for the current recharge operation.
    /// </summary>
    public string OperationStatusText
    {
        get => _operationStatusText;
        private set
        {
            if (SetProperty(ref _operationStatusText, value))
            {
                OnPropertyChanged(nameof(IsOperationStatusVisible));
            }
        }
    }

    public bool IsOperationStatusVisible => !string.IsNullOrWhiteSpace(OperationStatusText);

    private int CountdownSecondsRemaining
    {
        get => _countdownSecondsRemaining;
        set
        {
            if (SetProperty(ref _countdownSecondsRemaining, value))
            {
                OnPropertyChanged(nameof(CountdownDisplay));
                OnPropertyChanged(nameof(IsCountdownVisible));
            }
        }
    }

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
        OnPropertyChanged(nameof(IsAmountSelectionVisible));
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(CanRecharge));
        OnPropertyChanged(nameof(IsAmountSelectionVisible));
        RechargeCommand.NotifyCanExecuteChanged();
    }

    partial void OnSelectedAmountChanged(decimal value)
    {
        StatusMessage = string.Format(
            CultureInfo.CurrentCulture,
            LanguageProvider.SelfService_Recharge_Status_PendingPayment,
            value);
    }

    /// <summary>
    /// Loads recharge page data and starts the countdown timer.
    /// </summary>
    /// <param name="parameter">The recharge page parameter containing user info.</param>
    [RelayCommand]
    private void LoadData(RechargePageParameter parameter)
    {
        LogCommand(nameof(LoadData), $"TargetFunction={parameter.TargetFunction}, User={MaskLogValue(parameter.Data?.Name)}");
        ResetRuntimeState();
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        CurrentState = RechargeFlowState.Confirm;

        LoadUserInfoModule(parameter.Data);
        ResetCountdown();
    }

    /// <summary>
    /// Initiates the recharge process: hides amount selection, calls the YktApi,
    /// and transitions to the QR code state on success or shows error on failure.
    /// </summary>
    /// <param name="amount">The selected recharge amount as a string.</param>
    [RelayCommand(CanExecute = nameof(CanRecharge))]
    private async Task RechargeAsync(object amount)
    {
        LogCommand(nameof(RechargeAsync), $"Amount={amount}, User={MaskLogValue(UserInfo?.Name)}");
        if (amount == null)
        {
            return;
        }

        if (!decimal.TryParse(amount.ToString(), out var amt))
        {
            return;
        }

        SelectedAmount = amt;

        var yktApiClient = GetYktApiClient();

        if (yktApiClient is null)
        {
            OperationStatusText = LanguageProvider.SelfService_Recharge_Status_QrCodeGenerationFailed_Default;
            await Task.Delay(3000);
            OperationStatusText = string.Empty;
            IsBusy = false;
            return;
        }

        // Hide amount selection and show processing status
        IsBusy = true;
        OperationStatusText = LanguageProvider.SelfService_Recharge_Status_GeneratingQrCode;
        ResetCountdown();

        var cfg = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        var request = new TeacherRechargeQrCodeRequestDto
        {
            UserId = ResolveUserId(UserInfo),
            DealValue = amt,
            WorkStationNumb = string.Empty
        };

        try
        {
            var response = await yktApiClient.GetTeacherRechargeQrCodeAsync(request);
            LogApiResponse(nameof(yktApiClient.GetTeacherRechargeQrCodeAsync), response);

            if (!IsApiSuccess(response))
            {
                ShowQrGenerationErrorAndRecover(response.Message);
                return;
            }

            var qrCodeString = ExtractQrCodeFromResponse(response.Data);

            if (string.IsNullOrEmpty(qrCodeString))
            {
                ShowQrGenerationErrorAndRecover(
                    LanguageProvider.SelfService_Recharge_Status_QrCodeGenerationFailed_Default);
                return;
            }

            // Success: transition to QR code state
            OperationStatusText = string.Empty;
            IsBusy = false;

            QrCodeImage = qrCodeString;
            CurrentState = RechargeFlowState.QRCode;
            ResetCountdown();
        }
        catch (OperationCanceledException) when (_pageCleanupInProgress)
        {
            return;
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "Recharge operation failed. Amount={0}, User={1}", amt, MaskLogValue(UserInfo?.Name));
            ShowQrGenerationErrorAndRecover(ex.Message);
        }
    }

    /// <summary>
    /// Returns from QR code state to Confirm state, or navigates home if already in Confirm state.
    /// </summary>
    protected override void Back()
    {
        if (IsQRCodeState)
        {
            CurrentState = RechargeFlowState.Confirm;
            QrCodeImage = string.Empty;
            StatusMessage = string.Empty;
            SelectedAmount = 0;
            return;
        }

        Cleanup();
        base.Back();
    }

    /// <summary>
    /// Cleans up resources when the page is dismissed.
    /// </summary>
    internal void Cleanup()
    {
        _pageCleanupInProgress = true;
        StopCountdown();
        DisposeCountdownTimer();
    }

    private void ResetRuntimeState()
    {
        _pageCleanupInProgress = false;
        StopCountdown();
        OperationStatusText = string.Empty;
        CountdownSecondsRemaining = 0;
        SelectedAmount = 0;
        QrCodeImage = string.Empty;
        StatusMessage = string.Empty;
    }

    private async void ShowQrGenerationErrorAndRecover(string? reason)
    {
        OperationStatusText = string.Format(
            CultureInfo.CurrentUICulture,
            LanguageProvider.SelfService_Recharge_Status_QrCodeGenerationFailed,
            string.IsNullOrWhiteSpace(reason)
                ? LanguageProvider.SelfService_Recharge_Status_QrCodeGenerationFailed_Default
                : reason);

        // Show error for 3 seconds, then return to Confirm state
        await Task.Delay(3000);

        if (_pageCleanupInProgress)
        {
            return;
        }

        OperationStatusText = string.Empty;
        IsBusy = false;
        CurrentState = RechargeFlowState.Confirm;
        ResetCountdown();
    }

    private static string? ExtractQrCodeFromResponse(JsonElement? data)
    {
        if (!data.HasValue || data.Value.ValueKind != JsonValueKind.Object)
        {
            return null;
        }

        // Common field names for QR code data in WeChat Pay APIs
        var fieldNames = new[] { "url", "qrCode", "codeUrl", "code_url", "qr_url", "qrUrl", "payUrl", "pay_url" };
        foreach (var name in fieldNames)
        {
            if (data.Value.TryGetProperty(name, out JsonElement prop) &&
                prop.ValueKind == JsonValueKind.String)
            {
                string url = prop.GetString() ?? string.Empty;
                if (!string.IsNullOrEmpty(url))
                {
                    return url;
                }
            }
        }

        // Fall back: use first string-valued property
        foreach (var prop in data.Value.EnumerateObject())
        {
            if (prop.Value.ValueKind == JsonValueKind.String)
            {
                string val = prop.Value.GetString() ?? string.Empty;
                if (!string.IsNullOrEmpty(val))
                {
                    return val;
                }
            }
        }

        return null;
    }

    private static string ResolveUserId(UserInfoModel? userInfo)
    {
        if (userInfo is StaffInfoModel staff && !string.IsNullOrEmpty(staff.UserId))
        {
            return staff.UserId;
        }

        return string.Empty;
    }

    private void EnsureCountdownTimer()
    {
        if (_countdownTimer is not null)
        {
            return;
        }

        _countdownTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(1)
        };
        _countdownTimer.Tick += OnCountdownTimerTick;
    }

    private void OnCountdownTimerTick(object? sender, EventArgs e)
    {
        CountdownSecondsRemaining--;

        if (CountdownSecondsRemaining <= 0)
        {
            CountdownSecondsRemaining = 0;
            StopCountdown();
            _ = HandleCountdownElapsedAsync();
        }
    }

    private async Task HandleCountdownElapsedAsync()
    {
        if (IsBusy)
        {
            return;
        }

        await Dispatcher.UIThread.InvokeAsync(() =>
        {
            if (!_pageCleanupInProgress)
            {
                Back();
            }
        });
    }

    private void ResetCountdown()
    {
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        CountdownSecondsRemaining = NormalizePositive(cfgData.CountdownSeconds, DefaultCountdownSeconds);

        if (CountdownSecondsRemaining <= 0)
        {
            StopCountdown();
            return;
        }

        EnsureCountdownTimer();
        _countdownTimer?.Stop();
        _countdownTimer?.Start();
    }

    private void StopCountdown()
    {
        _countdownTimer?.Stop();
    }

    private void DisposeCountdownTimer()
    {
        if (_countdownTimer is null)
        {
            return;
        }

        _countdownTimer.Stop();
        _countdownTimer.Tick -= OnCountdownTimerTick;
        _countdownTimer = null;
    }

    private static int NormalizePositive(int value, int fallbackValue)
    {
        return value > 0 ? value : fallbackValue;
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

using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.models;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Requests;
using Avalonia.Threading;
using System.Globalization;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Parameter for report loss page.
/// </summary>
public sealed class ReportLossPageParameter
{
    public string TargetFunction { get; set; } = "ReportLoss";

    public UserInfoModel? Data { get; set; }
}

/// <summary>
/// ViewModel for ReportLossPage.
/// </summary>
public partial class ReportLossPageViewModel : ViewModelBase
{
    public ReportLossPageViewModel() : base()
    {
    }

    private const int DefaultCountdownSeconds = 60;
    private const int DefaultOperationTimeoutSeconds = 30;

    private DispatcherTimer? _countdownTimer;
    private CancellationTokenSource? _operationCancellationTokenSource;
    private StudentInfoPageViewModel? _studentInfoModuleViewModel;
    private StaffInfoPageViewModel? _staffInfoModuleViewModel;
    private bool _operationCancelledByCountdown;
    private bool _pageCleanupInProgress;
    private int _countdownSecondsRemaining;
    private string _operationStatusText = string.Empty;

    [ObservableProperty]
    private UserInfoModel? _userInfo;

    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = "ReportLoss";

    [ObservableProperty]
    private bool _canReportLossByStatus;

    [ObservableProperty]
    private bool _isBusy;

    /// <summary>
    /// Gets the formatted countdown text shown on the report loss page.
    /// </summary>
    public string CountdownDisplay => string.Format(
        CultureInfo.CurrentUICulture,
        LanguageProvider.SelfService_ReportLoss_Countdown_Format,
        CountdownSecondsRemaining);

    /// <summary>
    /// Gets a value indicating whether the countdown text should be visible.
    /// </summary>
    public bool IsCountdownVisible => CountdownSecondsRemaining > 0;

    /// <summary>
    /// Gets the localized status text for the current report loss operation.
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

    /// <summary>
    /// Gets a value indicating whether the operation status text should be visible.
    /// </summary>
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

    public string PageTitle => CanReportLossByStatus
        ? LanguageProvider.SelfService_ReportLoss_Title_Normal
        : LanguageProvider.SelfService_ReportLoss_Title_Invalid;

    public bool ShowReportLossButton => CanReportLoss;

    public bool CanReportLoss => CanReportLossByStatus && !IsBusy;

    partial void OnCanReportLossByStatusChanged(bool value)
    {
        OnPropertyChanged(nameof(PageTitle));
        OnPropertyChanged(nameof(ShowReportLossButton));
        OnPropertyChanged(nameof(CanReportLoss));
        ReportLossCommand.NotifyCanExecuteChanged();
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(ShowReportLossButton));
        OnPropertyChanged(nameof(CanReportLoss));
        ReportLossCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand]
    private void LoadData(ReportLossPageParameter parameter)
    {
        LogCommand(nameof(LoadData), $"TargetFunction={parameter.TargetFunction}, User={MaskLogValue(parameter.Data?.Name)}");
        ResetRuntimeState();
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;

        ResolveReportLossState(parameter.Data);
        LoadUserInfoModule(parameter.Data);
        ResetCountdown();
    }

    [RelayCommand(CanExecute = nameof(CanReportLoss))]
    private async Task ReportLossAsync()
    {
        LogCommand(nameof(ReportLossAsync), $"User={MaskLogValue(UserInfo?.Name)}, CardNo={MaskLogValue(UserInfo is null ? null : GetCardNumber(UserInfo))}");
        if (UserInfo is null)
        {
            return;
        }

        string cardNumber = GetCardNumber(UserInfo);
        if (string.IsNullOrWhiteSpace(cardNumber))
        {
            OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Failed_NoCardNumber;
            ResetCountdown();
            return;
        }

        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        var yktApiClient = GetYktApiClient();

        if (yktApiClient is null)
        {
            OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Failed_ApiUnavailable;
            ResetCountdown();
            return;
        }

        if (!TryBuildLockCardRequest(UserInfo, cardNumber, cfgData, out var request, out var validationMessage))
        {
            OperationStatusText = validationMessage;
            ResetCountdown();
            return;
        }

        IsBusy = true;
        _operationCancelledByCountdown = false;
        _pageCleanupInProgress = false;
        OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Processing;
        ResetCountdown();

        var timeoutCancellationTokenSource = new CancellationTokenSource(
            TimeSpan.FromSeconds(NormalizePositive(cfgData.CountdownSeconds, DefaultCountdownSeconds)));
        var linkedCancellationTokenSource = CancellationTokenSource.CreateLinkedTokenSource(timeoutCancellationTokenSource.Token);

        _operationCancellationTokenSource = linkedCancellationTokenSource;

        try
        {
            var response = await yktApiClient.LockCardAsync(request, linkedCancellationTokenSource.Token);
            LogApiResponse(nameof(yktApiClient.LockCardAsync), response);
            if (!EnsureApiSuccess(response.Code, response.Message))
            {
                OperationStatusText = FormatApiError(
                    response.Message,
                    LanguageProvider.SelfService_ReportLoss_Status_Failed);
                ResetCountdown();
                return;
            }

            UpdateCardStatusToLost(UserInfo);
            ResolveReportLossState(UserInfo);
            RefreshUserInfoModule();
            OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Success;
        }
        catch (OperationCanceledException) when (_pageCleanupInProgress)
        {
            return;
        }
        catch (OperationCanceledException) when (_operationCancelledByCountdown)
        {
            OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Failed_CountdownCancelled;
        }
        catch (OperationCanceledException) when (timeoutCancellationTokenSource.IsCancellationRequested)
        {
            OperationStatusText = LanguageProvider.SelfService_ReportLoss_Status_Failed_Timeout;
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "Report-loss operation failed. CardNo={0}", MaskLogValue(cardNumber));
            OperationStatusText = string.Format(
                CultureInfo.CurrentUICulture,
                LanguageProvider.SelfService_ReportLoss_Status_Failed_WithReason,
                string.IsNullOrWhiteSpace(ex.Message)
                    ? LanguageProvider.SelfService_ReportLoss_Status_Failed
                    : ex.Message);
        }
        finally
        {
            IsBusy = false;

            if (!_pageCleanupInProgress)
            {
                ResetCountdown();
            }

            _operationCancellationTokenSource = null;
            linkedCancellationTokenSource.Dispose();
            timeoutCancellationTokenSource.Dispose();
        }
    }

    protected override void Back()
    {
        Cleanup();
        base.Back();
    }

    internal void Cleanup()
    {
        _pageCleanupInProgress = true;
        StopCountdown();
        DisposeCountdownTimer();
        CancelActiveOperation();

        if (!IsBusy)
        {
            DisposeOperationCancellationTokenSource();
        }
    }

    private void ResolveReportLossState(UserInfoModel? data)
    {
        var currentCard = data?.CurrentCard;
        CanReportLossByStatus = currentCard is not null && currentCard.CardStatusId == (int)UserCardStatus.Normal;
    }

    private void LoadUserInfoModule(UserInfoModel? data)
    {
        _studentInfoModuleViewModel = null;
        _staffInfoModuleViewModel = null;

        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();

        if (cfgData.ServiceType == SelfServiceType.StaffSelfService)
        {
            LoadStaffModule(data);
            return;
        }

        LoadStudentModule(data);
    }

    private void LoadStaffModule(UserInfoModel? data)
    {
        var moduleView = new StaffInfoPageView();
        if (moduleView.DataContext is StaffInfoPageViewModel vm)
        {
            vm.LoadDataCommand.Execute(new StaffInfoPageParameter
            {
                Data = data as StaffInfoModel ?? new StaffInfoModel
                {
                    Name = data?.Name ?? "测试教职工",
                    UserType = data?.UserType ?? UserType.Staff,
                    IdCardNumber = data?.IdCardNumber ?? "430101198502031234",
                    DeptId = "1001",
                    CardExpiryDate = DateTime.Today.AddYears(1),
                    ConsumptionBalance = 125.50m,
                    SubsidyBalance = 80m,
                    UserId = "1624",
                    PhoneNumber = "13800138001",
                    UserCards =
                    [
                        new CardInfoModel
                        {
                            CardId = "1501",
                            CardNo = "60001",
                            FactoryFixId = "1348446620",
                            CardStatusName = "正常",
                            CardStatusId = (int)UserCardStatus.Normal,
                            CardTypeName = "教职工卡",
                            TenantId = "25"
                        }
                    ]
                },
                Mode = StaffInfoDisplayMode.Standard
            });

            _staffInfoModuleViewModel = vm;
        }

        UserInfoModuleContent = moduleView;
    }

    private void LoadStudentModule(UserInfoModel? data)
    {
        var moduleView = new StudentInfoPageView();
        if (moduleView.DataContext is StudentInfoPageViewModel vm)
        {
            vm.LoadDataCommand.Execute(new StudentInfoPageParameter
            {
                Data = data as StudentInfoModel ?? new StudentInfoModel
                {
                    Name = data?.Name ?? "测试学员",
                    UserType = data?.UserType ?? UserType.Student,
                    IdCardNumber = data?.IdCardNumber ?? "430101199001011234",
                    RoomName = "301房",
                    RoomNumber = "301房",
                    ClassName = "测试培训班一",
                    CheckInStartTime = DateTime.Today,
                    CheckInEndTime = DateTime.Today.AddDays(5),
                    TrainingStartDate = DateTime.Today,
                    TrainingEndDate = DateTime.Today.AddDays(5),
                    UserId = "1955939983117803521",
                    UserCards =
                    [
                        new CardInfoModel
                        {
                            CardId = "1955939986590687233",
                            CardNo = "40001",
                            FactoryFixId = "1348446620",
                            CardStatusName = "正常",
                            CardStatusId = (int)UserCardStatus.Normal,
                            CardTypeName = "主体班学员卡",
                            TenantId = "25"
                        }
                    ]
                },
                Mode = StudentInfoDisplayMode.Standard
            });

            _studentInfoModuleViewModel = vm;
        }

        UserInfoModuleContent = moduleView;
    }

    private void ResetRuntimeState()
    {
        _pageCleanupInProgress = false;
        _operationCancelledByCountdown = false;
        StopCountdown();
        DisposeOperationCancellationTokenSource();
        EnsureCountdownTimer();
        OperationStatusText = string.Empty;
        CountdownSecondsRemaining = 0;
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
            _operationCancelledByCountdown = true;
            CancelActiveOperation();
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
        CountdownSecondsRemaining = NormalizePositive(
            Ioc.Default.GetRequiredService<SelfServiceConfig>().CountdownSeconds,
            DefaultCountdownSeconds);

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

    private void CancelActiveOperation()
    {
        if (_operationCancellationTokenSource is null)
        {
            return;
        }

        if (!_operationCancellationTokenSource.IsCancellationRequested)
        {
            _operationCancellationTokenSource.Cancel();
        }
    }

    private void DisposeOperationCancellationTokenSource()
    {
        if (_operationCancellationTokenSource is null)
        {
            return;
        }

        _operationCancellationTokenSource.Dispose();
        _operationCancellationTokenSource = null;
    }

    private void UpdateCardStatusToLost(UserInfoModel userInfo)
    {
        var currentCard = userInfo.CurrentCard;
        if (currentCard is not null)
        {
            currentCard.CardStatusId = (int)UserCardStatus.Lost;
            currentCard.CardStatusName = "挂失";
        }
    }

    private void RefreshUserInfoModule()
    {
        _studentInfoModuleViewModel?.RefreshCommand.Execute(null);
        _staffInfoModuleViewModel?.RefreshCommand.Execute(null);
    }

    private static string GetCardNumber(UserInfoModel userInfo)
    {
        return userInfo.CurrentCard?.CardNo ?? string.Empty;
    }

    private static int NormalizePositive(int value, int fallbackValue)
    {
        return value > 0 ? value : fallbackValue;
    }

    private static bool TryBuildLockCardRequest(
        UserInfoModel userInfo,
        string cardNumber,
        SelfServiceConfig config,
        out CardOperateRequestDto request,
        out string validationMessage)
    {
        var currentCard = userInfo.CurrentCard;
        var missingItems = new List<string>();

        request = new CardOperateRequestDto();

        if (string.IsNullOrWhiteSpace(config.TenantId))
        {
            missingItems.Add($"{nameof(SelfServiceConfig)}.{nameof(SelfServiceConfig.TenantId)}");
        }

        if (string.IsNullOrWhiteSpace(currentCard?.FactoryFixId))
        {
            missingItems.Add($"{nameof(CardInfoModel)}.{nameof(CardInfoModel.FactoryFixId)}");
        }

        string? userId = userInfo switch
        {
            StaffInfoModel staff => staff.UserId,
            StudentInfoModel student => student.UserId,
            _ => null
        };
        if (string.IsNullOrWhiteSpace(userId))
        {
            missingItems.Add("UserId");
        }

        if (missingItems.Count == 0)
        {
            request = new CardOperateRequestDto
            {
                CardNo = cardNumber,
                FactoryFixId = currentCard!.FactoryFixId,
                OperatorId = userId!,
                TenantId = config.TenantId
            };
            validationMessage = string.Empty;
            return true;
        }

        validationMessage = string.Format(
            CultureInfo.CurrentUICulture,
            LanguageProvider.SelfService_ReportLoss_Status_Failed_InvalidConfig,
            string.Join(", ", missingItems));
        return false;
    }
}

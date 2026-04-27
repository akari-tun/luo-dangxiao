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
        var yktApiClient = Ioc.Default.GetService<IYktApiClient>();

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
            EnsureApiSuccess(response.Code, response.Message);

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
        CanReportLossByStatus = data switch
        {
            StudentInfoModel { CardStatus: StudentCardStatus.Normal } => true,
            StaffInfoModel { CardStatus: StaffCardStatus.Normal } => true,
            _ => false
        };
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
                    Id = data?.Id ?? "STF_TEST_001",
                    Name = data?.Name ?? "测试教职工",
                    UserType = data?.UserType ?? UserType.Staff,
                    IdCardNumber = data?.IdCardNumber ?? "430101198502031234",
                    EmployeeNumber = "T2020001",
                    CardType = "教职工卡",
                    Department = "教务处",
                    CardExpiryDate = DateTime.Today.AddYears(1),
                    ConsumptionBalance = 125.50m,
                    SubsidyBalance = 80m,
                    CardStatus = StaffCardStatus.Normal
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
                    Id = data?.Id ?? "STU_TEST_001",
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
                    CardStatus = StudentCardStatus.Normal
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
        switch (userInfo)
        {
            case StudentInfoModel student:
                student.CardStatus = StudentCardStatus.Lost;
                break;
            case StaffInfoModel staff:
                staff.CardStatus = StaffCardStatus.Lost;
                break;
        }
    }

    private void RefreshUserInfoModule()
    {
        _studentInfoModuleViewModel?.RefreshCommand.Execute(null);
        _staffInfoModuleViewModel?.RefreshCommand.Execute(null);
    }

    private static string GetCardNumber(UserInfoModel userInfo)
    {
        return userInfo switch
        {
            StudentInfoModel student => student.CardNumber,
            StaffInfoModel staff => staff.CardNumber,
            _ => string.Empty
        };
    }

    private static void EnsureApiSuccess(int? code, string? message)
    {
        if (code is null or 0 or 200)
        {
            return;
        }

        throw new InvalidOperationException(string.IsNullOrWhiteSpace(message)
            ? LanguageProvider.SelfService_ReportLoss_Status_Failed
            : message);
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
        var missingItems = new List<string>();

        request = new CardOperateRequestDto();

        if (string.IsNullOrWhiteSpace(config.TenantId))
        {
            missingItems.Add($"{nameof(SelfServiceConfig)}.{nameof(SelfServiceConfig.TenantId)}");
        }

        if (userInfo is not StaffInfoModel staffInfo)
        {
            if (userInfo is not StudentInfoModel studentInfo)
            {
                missingItems.Add($"{nameof(StaffInfoModel)}/{nameof(StudentInfoModel)}");
            }
            else
            {
                if (string.IsNullOrWhiteSpace(studentInfo.FactoryFixId))
                {
                    missingItems.Add($"{nameof(StudentInfoModel)}.{nameof(StudentInfoModel.FactoryFixId)}");
                }

                if (string.IsNullOrWhiteSpace(studentInfo.UserId))
                {
                    missingItems.Add($"{nameof(StudentInfoModel)}.{nameof(StudentInfoModel.UserId)}");
                }
            }
        }
        else
        {
            if (string.IsNullOrWhiteSpace(staffInfo.FactoryFixId))
            {
                missingItems.Add($"{nameof(StaffInfoModel)}.{nameof(StaffInfoModel.FactoryFixId)}");
            }

            if (string.IsNullOrWhiteSpace(staffInfo.UserId))
            {
                missingItems.Add($"{nameof(StaffInfoModel)}.{nameof(StaffInfoModel.UserId)}");
            }
        }

        if (missingItems.Count == 0)
        {
            string factoryFixId;
            string operatorId;

            if (userInfo is StaffInfoModel requestStaffInfo)
            {
                factoryFixId = requestStaffInfo.FactoryFixId;
                operatorId = requestStaffInfo.UserId;
            }
            else
            {
                var requestStudentInfo = (StudentInfoModel)userInfo;
                factoryFixId = requestStudentInfo.FactoryFixId;
                operatorId = requestStudentInfo.UserId;
            }

            request = new CardOperateRequestDto
            {
                CardNo = cardNumber,
                FactoryFixId = factoryFixId,
                OperatorId = operatorId,
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

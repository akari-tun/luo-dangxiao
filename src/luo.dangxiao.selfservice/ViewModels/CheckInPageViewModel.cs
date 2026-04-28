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
/// Parameter for check-in page.
/// </summary>
public sealed class CheckInPageParameter
{
    public string TargetFunction { get; set; } = "CheckIn";

    public UserInfoModel? Data { get; set; }
}

/// <summary>
/// ViewModel for CheckInPage.
/// </summary>
public partial class CheckInPageViewModel : ViewModelBase
{
    private const int DefaultCountdownSeconds = 60;

    private DispatcherTimer? _countdownTimer;
    private StudentInfoPageViewModel? _studentInfoModuleViewModel;
    private bool _pageCleanupInProgress;
    private int _countdownSecondsRemaining;
    private string _operationStatusText = string.Empty;

    [ObservableProperty]
    private StudentInfoModel? _studentInfo;

    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = "CheckIn";

    [ObservableProperty]
    private bool _isCheckedIn;

    [ObservableProperty]
    private bool _isBusy;

    /// <summary>
    /// Gets the formatted countdown text shown on the check-in page.
    /// </summary>
    public string CountdownDisplay => string.Format(
        CultureInfo.CurrentUICulture,
        LanguageProvider.SelfService_CheckIn_Countdown_Format,
        CountdownSecondsRemaining);

    /// <summary>
    /// Gets a value indicating whether the countdown text should be visible.
    /// </summary>
    public bool IsCountdownVisible => CountdownSecondsRemaining > 0;

    /// <summary>
    /// Gets the localized status text for the current check-in operation.
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

    public string PageTitle => IsCheckedIn
        ? LanguageProvider.SelfService_CheckIn_Title_CheckedIn
        : LanguageProvider.SelfService_CheckIn_Title_NotCheckedIn;

    public bool ShowCheckInButton => !IsCheckedIn;

    public bool CanCheckIn => !IsCheckedIn && !IsBusy;

    partial void OnIsCheckedInChanged(bool value)
    {
        OnPropertyChanged(nameof(PageTitle));
        OnPropertyChanged(nameof(ShowCheckInButton));
        OnPropertyChanged(nameof(CanCheckIn));
        CheckInCommand.NotifyCanExecuteChanged();
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(CanCheckIn));
        CheckInCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand]
    private void LoadData(CheckInPageParameter parameter)
    {
        ResetRuntimeState();
        TargetFunction = parameter.TargetFunction;
        StudentInfo = ResolveStudentInfo(parameter.Data);
        IsCheckedIn = StudentInfo.CheckInStatus == StudentCheckInStatus.CheckedIn;
        LoadUserInfoModule(StudentInfo);
        ResetCountdown();
    }

    [RelayCommand(CanExecute = nameof(CanCheckIn))]
    private async Task CheckInAsync()
    {
        if (StudentInfo is null)
        {
            return;
        }

        var yktApiClient = Ioc.Default.GetService<IYktApiClient>();

        if (yktApiClient is null)
        {
            OperationStatusText = LanguageProvider.SelfService_CheckIn_Status_Failed_ApiUnavailable;
            ResetCountdown();
            return;
        }

        IsBusy = true;
        OperationStatusText = LanguageProvider.SelfService_CheckIn_Status_Processing;
        ResetCountdown();

        var request = new TraineeRegisterRequestDto
        {
            UserId = StudentInfo.UserId,
            CheckinState = 0,
            RoomCode = string.IsNullOrWhiteSpace(StudentInfo.RoomCode) ? string.Empty : StudentInfo.RoomCode,
            DeptId = StudentInfo.DeptId
        };

        try
        {
            var response = await yktApiClient.RegisterTraineeAsync(request);

            if (response.Code is not (null or 0 or 200))
            {
                OperationStatusText = string.Format(
                    CultureInfo.CurrentUICulture,
                    LanguageProvider.SelfService_CheckIn_Status_Failed_WithReason,
                    string.IsNullOrWhiteSpace(response.Message)
                        ? LanguageProvider.SelfService_CheckIn_Status_Failed
                        : response.Message);
                ResetCountdown();
                return;
            }

            StudentInfo.CheckInStatus = StudentCheckInStatus.CheckedIn;
            StudentInfo.CheckInStartTime ??= DateTime.Now;

            if (string.IsNullOrWhiteSpace(StudentInfo.RoomName) && !string.IsNullOrWhiteSpace(StudentInfo.RoomNumber))
            {
                StudentInfo.RoomName = StudentInfo.RoomNumber;
            }

            IsCheckedIn = true;
            _studentInfoModuleViewModel?.RefreshCommand.Execute(null);
            OperationStatusText = LanguageProvider.SelfService_CheckIn_Status_Success;
        }
        catch (OperationCanceledException) when (_pageCleanupInProgress)
        {
            return;
        }
        catch (Exception ex)
        {
            OperationStatusText = string.Format(
                CultureInfo.CurrentUICulture,
                LanguageProvider.SelfService_CheckIn_Status_Failed_WithReason,
                string.IsNullOrWhiteSpace(ex.Message)
                    ? LanguageProvider.SelfService_CheckIn_Status_Failed
                    : ex.Message);
        }
        finally
        {
            IsBusy = false;

            if (!_pageCleanupInProgress)
            {
                ResetCountdown();
            }
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
    }

    private void ResetRuntimeState()
    {
        _pageCleanupInProgress = false;
        StopCountdown();
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

    private void LoadUserInfoModule(StudentInfoModel student)
    {
        var moduleView = new StudentInfoPageView();
        if (moduleView.DataContext is StudentInfoPageViewModel vm)
        {
            vm.LoadDataCommand.Execute(new StudentInfoPageParameter
            {
                Data = student,
                Mode = StudentInfoDisplayMode.Standard
            });

            _studentInfoModuleViewModel = vm;
        }

        UserInfoModuleContent = moduleView;
    }

    private static StudentInfoModel ResolveStudentInfo(UserInfoModel? data)
    {
        if (data is StudentInfoModel student)
        {
            return student;
        }

        return new StudentInfoModel
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
            CheckInStatus = StudentCheckInStatus.NotCheckedIn
        };
    }
}

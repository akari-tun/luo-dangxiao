using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.wabapi.Clients;

namespace luo.dangxiao.selfservice.ViewModels;

public sealed class ReplacementPageParameter
{
    public string TargetFunction { get; set; } = "Replacement";
    public UserInfoModel? Data { get; set; }
}

public partial class ReplacementPageViewModel : CardOperationViewModelBase
{
    private readonly SelfServiceConfig _config;
    private readonly CardPrinterBase _cardPrinter;
    private readonly IYktApiClient? _yktApiClient;
    private readonly string _printerId;

    [ObservableProperty]
    private UserInfoModel? _userInfo;

    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = "Replacement";

    [ObservableProperty]
    private bool _canReplacementByStatus;

    [ObservableProperty]
    private string _pageTitle = "确认信息后，点击[补卡]按钮进行补卡。";

    public bool CanReplacement => CanReplacementByStatus && !IsBusy;
    public bool ShowReplacementButton => CanReplacement;

    public bool IsProcessingState => IsCardProcessingState;

    public ReplacementPageViewModel(SelfServiceConfig config, CardPrinterBase cardPrinter, IYktApiClient? yktApiClient = null)
    {
        _config = config;
        _cardPrinter = cardPrinter;
        _yktApiClient = yktApiClient;
        _printerId = config.PrinterConfig.DefaultPrinterId;
    }

    protected override SelfServiceConfig Config => _config;
    protected override CardPrinterBase CardPrinter => _cardPrinter;
    protected override IYktApiClient? YktApiClient => _yktApiClient;
    protected override string PrinterId => _printerId;
    protected override UserInfoModel? UserInfoData => UserInfo;

    partial void OnCanReplacementByStatusChanged(bool value)
    {
        ExecuteReplacementCommand.NotifyCanExecuteChanged();
        OnPropertyChanged(nameof(CanReplacement));
        OnPropertyChanged(nameof(ShowReplacementButton));
    }

    protected override void OnBusyChanged(bool value)
    {
        ExecuteReplacementCommand.NotifyCanExecuteChanged();
        OnPropertyChanged(nameof(CanReplacement));
        OnPropertyChanged(nameof(ShowReplacementButton));
    }

    protected override void HandleCountdownExpired()
    {
        if (IsConfirmState)
        {
            StopCountdownTimer();
            Back();
        }
        else if (IsCardReadyToPickupState)
        {
            OnPickupTimeoutDiscarded();
        }
        else if (IsCardProcessingState)
        {
            _ = HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
        }
    }

    [RelayCommand(CanExecute = nameof(CanReplacement))]
    private async Task ExecuteReplacementAsync() => await ExecuteCardProcessAsync("换卡");

    [RelayCommand]
    private void LoadData(ReplacementPageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        CurrentState = CardProcessingState.Confirm;
        IsBusy = false;

        ResolveReplacementState(parameter.Data);
        LoadUserInfoModule(parameter.Data);
        OnPropertyChanged(nameof(PageTitle));

        StartCountdownTimer();
    }

    protected override void SetProcessingTitle() { }

    protected override string GetPickupInstructionText() =>
        LanguageProvider.SelfService_TakeCard_Status_PickupInstruction;

    protected override void OnPickupTimeoutDiscarded() =>
        PageTitle = LanguageProvider.SelfService_TakeCard_Status_Timeout_Discard;

    protected override void OnPickupTimeoutReturned() =>
        PageTitle = LanguageProvider.SelfService_TakeCard_Status_CardReturned;

    private void ResolveReplacementState(UserInfoModel? data)
    {
        CanReplacementByStatus = data switch
        {
            StudentInfoModel s when s.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => true,
            StaffInfoModel f when f.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => true,
            _ => false
        };

        PageTitle = data switch
        {
            StudentInfoModel s when s.CurrentCard?.CardStatusId == (int)UserCardStatus.Normal
                => "您当前的卡片为正常状态，请挂失后再补卡。",
            StaffInfoModel f when f.CurrentCard?.CardStatusId == (int)UserCardStatus.Normal
                => "您当前的卡片为正常状态，请挂失后再补卡。",
            StudentInfoModel s when s.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost
                => "确认信息后，点击[补卡]按钮进行补卡。",
            StaffInfoModel f when f.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost
                => "确认信息后，点击[补卡]按钮进行补卡。",
            _ => "您没有已挂失的卡片，无法进行补卡。"
        };
    }

    private void LoadUserInfoModule(UserInfoModel? data)
    {
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
                    UserCards =
                    [
                        new CardInfoModel
                        {
                            CardNo = "2020001001",
                            CardStatusId = (int)UserCardStatus.Normal,
                            CardStatusName = "正常"
                        }
                    ]
                },
                Mode = StaffInfoDisplayMode.Standard
            });
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
                    UserCards =
                    [
                        new CardInfoModel
                        {
                            CardNo = "20200901001",
                            CardStatusId = (int)UserCardStatus.Normal,
                            CardStatusName = "正常"
                        }
                    ]
                },
                Mode = StudentInfoDisplayMode.Standard
            });
        }
        UserInfoModuleContent = moduleView;
    }
}

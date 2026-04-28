using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Parameter for replacement page.
/// </summary>
public sealed class ReplacementPageParameter
{
    public string TargetFunction { get; set; } = "Replacement";

    public UserInfoModel? Data { get; set; }
}

/// <summary>
/// ViewModel for ReplacementPage.
/// </summary>
public class ReplacementPageViewModel : ViewModelBase
{
    private UserInfoModel? _userInfo;
    private object? _userInfoModuleContent;
    private string _targetFunction = "Replacement";
    private bool _canReplacementByStatus;
    private bool _isBusy;

    public ReplacementPageViewModel()
    {
        LoadDataCommand = new RelayCommand<ReplacementPageParameter>(LoadData);
        ReplacementCommand = new AsyncRelayCommand(ReplacementAsync, () => CanReplacement);
    }

    public UserInfoModel? UserInfo
    {
        get => _userInfo;
        set
        {
            if (SetProperty(ref _userInfo, value))
            {
                OnPropertyChanged(nameof(PageTitle));
            }
        }
    }

    public object? UserInfoModuleContent
    {
        get => _userInfoModuleContent;
        set => SetProperty(ref _userInfoModuleContent, value);
    }

    public string TargetFunction
    {
        get => _targetFunction;
        set => SetProperty(ref _targetFunction, value);
    }

    public bool CanReplacementByStatus
    {
        get => _canReplacementByStatus;
        set
        {
            if (SetProperty(ref _canReplacementByStatus, value))
            {
                OnPropertyChanged(nameof(PageTitle));
                OnPropertyChanged(nameof(ShowReplacementButton));
                OnPropertyChanged(nameof(CanReplacement));
                ReplacementCommand.NotifyCanExecuteChanged();
            }
        }
    }

    public bool IsBusy
    {
        get => _isBusy;
        set
        {
            if (SetProperty(ref _isBusy, value))
            {
                OnPropertyChanged(nameof(ShowReplacementButton));
                OnPropertyChanged(nameof(CanReplacement));
                ReplacementCommand.NotifyCanExecuteChanged();
            }
        }
    }

    public string PageTitle => UserInfo switch
    {
        StudentInfoModel student when student.CurrentCard?.CardStatusId == (int)UserCardStatus.Normal => "您当前的卡片为正常状态，请挂失后再补卡。",
        StaffInfoModel staff when staff.CurrentCard?.CardStatusId == (int)UserCardStatus.Normal => "您当前的卡片为正常状态，请挂失后再补卡。",
        StudentInfoModel student when student.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => "确认信息后，点击[补卡]按钮进行补卡。",
        StaffInfoModel staff when staff.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => "确认信息后，点击[补卡]按钮进行补卡。",
        _ => "您没有已挂失的卡片，无法进行补卡。"
    };

    public bool ShowReplacementButton => CanReplacement;

    public bool CanReplacement => CanReplacementByStatus && !IsBusy;

    public IRelayCommand<ReplacementPageParameter> LoadDataCommand { get; }

    public IAsyncRelayCommand ReplacementCommand { get; }

    private void LoadData(ReplacementPageParameter? parameter)
    {
        if (parameter is null)
        {
            return;
        }

        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;

        ResolveReplacementState(parameter.Data);
        LoadUserInfoModule(parameter.Data);
        OnPropertyChanged(nameof(PageTitle));
    }

    private async Task ReplacementAsync()
    {
        if (UserInfo is null)
        {
            return;
        }

        IsBusy = true;
        await Task.Delay(1000);

        switch (UserInfo)
        {
            case StudentInfoModel student:
                student.UserCards = [];
                break;
            case StaffInfoModel staff:
                staff.UserCards = [];
                break;
        }

        CanReplacementByStatus = false;
        IsBusy = false;
        OnPropertyChanged(nameof(PageTitle));
    }

    private void ResolveReplacementState(UserInfoModel? data)
    {
        CanReplacementByStatus = data switch
        {
            StudentInfoModel student when student.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => true,
            StaffInfoModel staff when staff.CurrentCard?.CardStatusId == (int)UserCardStatus.Lost => true,
            _ => false
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
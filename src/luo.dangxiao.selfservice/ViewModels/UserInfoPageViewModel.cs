using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Parameter for user info page.
/// </summary>
public sealed class UserInfoPageParameter
{
    public string TargetFunction { get; set; } = string.Empty;

    public UserInfoModel? Data { get; set; }
}

/// <summary>
/// ViewModel for UserInfoPage.
/// </summary>
public partial class UserInfoPageViewModel : ViewModelBase, IPageViewModel
{
    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = string.Empty;

    [ObservableProperty]
    private UserInfoModel? _currentUserInfo;

    [ObservableProperty]
    private SelfServiceType _selfServiceType = SelfServiceType.StudentSelfService;

    public bool IsStaffModule => SelfServiceType == SelfServiceType.StaffSelfService;

    public bool IsStudentModule => SelfServiceType == SelfServiceType.StudentSelfService;

    public string ModuleTitle => IsStaffModule ? "教职工信息" : "学员信息";

    public string PrimaryActionText => TargetFunction switch
    {
        "CheckIn" => "报到",
        "TakeCard" => "领卡",
        "ReportLoss" => "挂失",
        "Replacement" => "补卡",
        "Query" => "查询",
        "Recharge" => "充值",
        _ => string.Empty
    };

    public string PrimaryActionIcon => TargetFunction switch
    {
        "CheckIn" => "avares://luo.dangxiao.resources/Images/check_in.png",
        "TakeCard" => "avares://luo.dangxiao.resources/Images/take_card.png",
        "ReportLoss" => "avares://luo.dangxiao.resources/Images/report_loss.png",
        "Replacement" => "avares://luo.dangxiao.resources/Images/replacement.png",
        "Query" => "avares://luo.dangxiao.resources/Images/query.png",
        "Recharge" => "avares://luo.dangxiao.resources/Images/recharge.png",
        _ => string.Empty
    };

    public bool ShowPrimaryAction => !string.IsNullOrWhiteSpace(PrimaryActionText);

    public UserInfoPageViewModel()
    {
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        SelfServiceType = cfgData.ServiceType;
    }

    partial void OnSelfServiceTypeChanged(SelfServiceType value)
    {
        OnPropertyChanged(nameof(IsStaffModule));
        OnPropertyChanged(nameof(IsStudentModule));
        OnPropertyChanged(nameof(ModuleTitle));
    }

    partial void OnTargetFunctionChanged(string value)
    {
        OnPropertyChanged(nameof(PrimaryActionText));
        OnPropertyChanged(nameof(PrimaryActionIcon));
        OnPropertyChanged(nameof(ShowPrimaryAction));
    }

    [RelayCommand]
    private void LoadData(UserInfoPageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        CurrentUserInfo = parameter.Data;

        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        SelfServiceType = cfgData.ServiceType;

        if (SelfServiceType == SelfServiceType.StaffSelfService)
        {
            LoadStaffModule(parameter.Data);
            return;
        }

        LoadStudentModule(parameter.Data);
    }

    private void LoadStaffModule(UserInfoModel? data)
    {
        var moduleView = new StaffInfoPageView();
        if (moduleView.DataContext is StaffInfoPageViewModel vm)
        {
            vm.StaffInfo = data as StaffInfoModel ?? new StaffInfoModel
            {
                Id = data?.Id ?? "STF_TEST_001",
                Name = data?.Name ?? "测试教职工",
                UserType = UserType.Staff,
                IdCardNumber = data?.IdCardNumber ?? "430101198502031234",
                EmployeeNumber = "T2020001",
                CardType = "教职工卡",
                Department = "教务处",
                CardExpiryDate = DateTime.Today.AddYears(1),
                ConsumptionBalance = 125.50m,
                SubsidyBalance = 80m
            };
        }

        UserInfoModuleContent = moduleView;
    }

    private void LoadStudentModule(UserInfoModel? data)
    {
        var moduleView = new StudentInfoPageView();
        if (moduleView.DataContext is StudentInfoPageViewModel vm)
        {
            vm.StudentInfo = data as StudentInfoModel ?? new StudentInfoModel
            {
                Id = data?.Id ?? "STU_TEST_001",
                Name = data?.Name ?? "测试学员",
                UserType = UserType.Student,
                IdCardNumber = data?.IdCardNumber ?? "430101199001011234",
                RoomName = "301房",
                ClassName = "测试培训班一",
                CheckInStartTime = DateTime.Today,
                CheckInEndTime = DateTime.Today.AddDays(5),
                TrainingStartDate = DateTime.Today,
                TrainingEndDate = DateTime.Today.AddDays(5)
            };
        }

        UserInfoModuleContent = moduleView;
    }
}

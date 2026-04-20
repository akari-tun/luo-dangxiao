using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.models;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;

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
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;

        ResolveReportLossState(parameter.Data);
        LoadUserInfoModule(parameter.Data);
    }

    [RelayCommand(CanExecute = nameof(CanReportLoss))]
    private async Task ReportLossAsync()
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
                student.CardStatus = StudentCardStatus.Lost;
                break;
            case StaffInfoModel staff:
                staff.CardStatus = StaffCardStatus.Lost;
                break;
        }

        CanReportLossByStatus = false;
        IsBusy = false;
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
        }

        UserInfoModuleContent = moduleView;
    }
}

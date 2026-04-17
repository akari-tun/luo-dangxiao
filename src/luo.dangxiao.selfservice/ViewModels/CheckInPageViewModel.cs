using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.models;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;

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

    private StudentInfoPageViewModel? _studentInfoModuleViewModel;

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
        TargetFunction = parameter.TargetFunction;
        StudentInfo = ResolveStudentInfo(parameter.Data);
        IsCheckedIn = StudentInfo.CheckInStatus == StudentCheckInStatus.CheckedIn;
        LoadUserInfoModule(StudentInfo);
    }

    [RelayCommand(CanExecute = nameof(CanCheckIn))]
    private async Task CheckInAsync()
    {
        if (StudentInfo is null)
        {
            return;
        }

        IsBusy = true;

        await Task.Delay(1000);

        StudentInfo.CheckInStatus = StudentCheckInStatus.CheckedIn;
        StudentInfo.CheckInStartTime ??= DateTime.Now;

        if (string.IsNullOrWhiteSpace(StudentInfo.RoomName) && !string.IsNullOrWhiteSpace(StudentInfo.RoomNumber))
        {
            StudentInfo.RoomName = StudentInfo.RoomNumber;
        }

        IsCheckedIn = true;
        _studentInfoModuleViewModel?.RefreshCommand.Execute(null);

        IsBusy = false;
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

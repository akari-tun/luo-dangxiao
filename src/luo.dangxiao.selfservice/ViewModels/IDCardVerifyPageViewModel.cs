using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Event args for a successful ID card verification.
/// </summary>
public sealed class IDCardVerificationSucceededEventArgs : EventArgs
{
    public IDCardVerificationSucceededEventArgs(UserInfoModel userInfo)
    {
        UserInfo = userInfo;
    }

    public UserInfoModel UserInfo { get; }
}

/// <summary>
/// Verify state for ID card verify page.
/// </summary>
public enum IDCardVerifyState
{
    Waiting,
    Processing,
    Success,
    Failed
}

/// <summary>
/// ViewModel for ID card verify module.
/// </summary>
public partial class IDCardVerifyPageViewModel : ViewModelBase, IPageViewModel
{
    public event EventHandler<IDCardVerificationSucceededEventArgs>? VerificationSucceeded;

    [ObservableProperty]
    private IDCardVerifyState _currentState = IDCardVerifyState.Waiting;

    [ObservableProperty]
    private string _statusMessage = "等待读取身份证...";

    [ObservableProperty]
    private string _userName = string.Empty;

    [ObservableProperty]
    private string _errorMessage = string.Empty;

    public bool IsWaiting => CurrentState == IDCardVerifyState.Waiting;

    public bool IsProcessing => CurrentState == IDCardVerifyState.Processing;

    public bool IsSuccess => CurrentState == IDCardVerifyState.Success;

    public bool IsFailed => CurrentState == IDCardVerifyState.Failed;

    partial void OnCurrentStateChanged(IDCardVerifyState value)
    {
        OnPropertyChanged(nameof(IsWaiting));
        OnPropertyChanged(nameof(IsProcessing));
        OnPropertyChanged(nameof(IsSuccess));
        OnPropertyChanged(nameof(IsFailed));
    }

    [RelayCommand]
    private async Task StartVerificationAsync()
    {
        ErrorMessage = string.Empty;
        UserName = string.Empty;
        CurrentState = IDCardVerifyState.Processing;
        StatusMessage = "正在模拟调用身份证读卡器，请稍候...";

        var userInfo = await ReadIdCardAsync();

        CurrentState = IDCardVerifyState.Success;
        UserName = userInfo.Name;
        StatusMessage = $"读卡成功，欢迎 {userInfo.Name}";
        VerificationSucceeded?.Invoke(this, new IDCardVerificationSucceededEventArgs(userInfo));
    }

    private static async Task<UserInfoModel> ReadIdCardAsync()
    {
        await Task.Delay(1200);

        var cfgData = Ioc.Default.GetRequiredService<CfgDataModel>();
        if (cfgData.ServiceType == SelfServiceType.StaffSelfService)
        {
            return new StaffInfoModel
            {
                Id = "STF_TEST_001",
                Name = "张明华",
                UserType = UserType.Staff,
                IdCardNumber = "430101198502031234",
                Gender = "男",
                Department = "教务处",
                EmployeeNumber = "T2020001",
                CardType = "教职工卡",
                CardNumber = "2020001001",
                CardStatus = StaffCardStatus.Normal,
                ConsumptionBalance = 125.50m,
                SubsidyBalance = 80.00m,
                CardBalance = 205.50m,
                CardIssueDate = new DateTime(2020, 1, 1),
                CardExpiryDate = new DateTime(2027, 12, 31),
                PhoneNumber = "13800138000"
            };
        }

        return new StudentInfoModel
        {
            Id = "STU_TEST_001",
            Name = "李天明",
            UserType = UserType.Student,
            IdCardNumber = "430101199001011234",
            Gender = "男",
            ClassName = "测试培训班一",
            CheckInStartTime = new DateTime(2026, 4, 10, 14, 0, 0),
            CheckInEndTime = new DateTime(2026, 4, 15, 12, 0, 0),
            TrainingStartDate = new DateTime(2026, 4, 11),
            TrainingEndDate = new DateTime(2026, 4, 15),
            CardNumber = "20260410001",
            CardStatus = StudentCardStatus.PendingPickup,
            RoomName = "A312",
            RoomNumber = "A312",
            CheckInStatus = StudentCheckInStatus.NotCheckedIn
        };
    }
}

using Avalonia.Media;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Display mode for student information module.
/// </summary>
public enum StudentInfoDisplayMode
{
    Standard,
    WithPhoto,
    FullInfo
}

/// <summary>
/// Student info parameter for the module.
/// </summary>
public sealed class StudentInfoPageParameter
{
    public string StudentId { get; set; } = string.Empty;

    public StudentInfoModel? Data { get; set; }

    public StudentInfoDisplayMode Mode { get; set; } = StudentInfoDisplayMode.Standard;

    public StudentFeeInfoModel? FeeInfo { get; set; }
}

/// <summary>
/// ViewModel for StudentInfoPage module.
/// </summary>
public partial class StudentInfoPageViewModel : ViewModelBase, IPageViewModel
{
    [ObservableProperty]
    private StudentInfoModel _studentInfo = new()
    {
        Id = "STU20260001",
        Name = "李天明",
        UserType = UserType.Student,
        Gender = "男",
        IdCardNumber = "430101199001011234",
        ClassName = "测试培训班一",
        CheckInStartTime = new DateTime(2020, 09, 01, 14, 0, 0),
        CheckInEndTime = new DateTime(2020, 09, 05, 12, 0, 0),
        TrainingStartDate = new DateTime(2020, 09, 01),
        TrainingEndDate = new DateTime(2020, 09, 05),
        CardNumber = "20200901001",
        UserCards =
        [
            new CardInfoModel
            {
                CardNo = "20200901001",
                CardStatusId = (int)UserCardStatus.Normal,
                CardStatusName = "正常"
            }
        ],
        RoomName = "301房",
        RoomNumber = "301房",
        CheckInStatus = StudentCheckInStatus.NotCheckedIn
    };

    [ObservableProperty]
    private StudentInfoDisplayMode _currentMode = StudentInfoDisplayMode.Standard;

    [ObservableProperty]
    private StudentFeeInfoModel _feeInfo = new()
    {
        TrainingFee = 2000m,
        AccommodationFee = 1000m,
        MealFee = 500m,
        TotalFee = 3500m,
        PaymentStatus = StudentPaymentStatus.Unpaid
    };

    public bool ShowPhoto => CurrentMode == StudentInfoDisplayMode.WithPhoto;

    public bool ShowFeeInfo => CurrentMode == StudentInfoDisplayMode.FullInfo;

    public bool ShowIdCardNumber => CurrentMode != StudentInfoDisplayMode.WithPhoto;

    public bool ShowRoomNumber => !string.IsNullOrWhiteSpace(RoomDisplay) && CurrentMode != StudentInfoDisplayMode.WithPhoto;

    public bool ShowCheckInStatus => CurrentMode != StudentInfoDisplayMode.WithPhoto;

    public bool ShowCardInfo => !string.IsNullOrWhiteSpace(StudentInfo.CardNumber);

    public string RoomDisplay => string.IsNullOrWhiteSpace(StudentInfo.RoomName)
        ? StudentInfo.RoomNumber
        : StudentInfo.RoomName;

    public string CheckInStartText => FormatDateTime(StudentInfo.CheckInStartTime);

    public string CheckInEndText => FormatDateTime(StudentInfo.CheckInEndTime);

    public string TrainingStartText => FormatDate(StudentInfo.TrainingStartDate);

    public string TrainingEndText => FormatDate(StudentInfo.TrainingEndDate);

    public string TrainingDateRange => $"{TrainingStartText} 至 {TrainingEndText}";

    public string CardStatusText
    {
        get
        {
            var currentCard = StudentInfo.CurrentCard;
            if (currentCard is null)
            {
                return "待领取";
            }

            return currentCard.CardStatusId switch
            {
                (int)UserCardStatus.Normal => "正常",
                (int)UserCardStatus.Lost => "已挂失",
                (int)UserCardStatus.Unissued => "未制卡",
                _ => "未知"
            };
        }
    }

    public IBrush CardStatusBrush
    {
        get
        {
            var currentCard = StudentInfo.CurrentCard;
            if (currentCard is null)
            {
                return Brush.Parse("#ff6000");
            }

            return currentCard.CardStatusId switch
            {
                (int)UserCardStatus.Normal => Brush.Parse("#4CAF50"),
                (int)UserCardStatus.Lost => Brush.Parse("#d9230b"),
                (int)UserCardStatus.Unissued => Brush.Parse("#70706d"),
                _ => Brush.Parse("#70706d")
            };
        }
    }

    public string CheckInStatusText => StudentInfo.CheckInStatus switch
    {
        StudentCheckInStatus.CheckedIn => "已报到 ✓",
        StudentCheckInStatus.NotCheckedIn => "未报到",
        _ => "未报到"
    };

    public IBrush CheckInStatusBrush => StudentInfo.CheckInStatus switch
    {
        StudentCheckInStatus.CheckedIn => Brush.Parse("#4CAF50"),
        StudentCheckInStatus.NotCheckedIn => Brush.Parse("#70706d"),
        _ => Brush.Parse("#70706d")
    };

    public string PaymentStatusText => FeeInfo.PaymentStatus switch
    {
        StudentPaymentStatus.Paid => "已缴费 ✓",
        StudentPaymentStatus.Unpaid => "未缴费",
        _ => "未缴费"
    };

    public IBrush PaymentStatusBrush => FeeInfo.PaymentStatus switch
    {
        StudentPaymentStatus.Paid => Brush.Parse("#4CAF50"),
        StudentPaymentStatus.Unpaid => Brush.Parse("#d9230b"),
        _ => Brush.Parse("#70706d")
    };

    partial void OnCurrentModeChanged(StudentInfoDisplayMode value)
    {
        OnPropertyChanged(nameof(ShowPhoto));
        OnPropertyChanged(nameof(ShowFeeInfo));
        OnPropertyChanged(nameof(ShowIdCardNumber));
        OnPropertyChanged(nameof(ShowRoomNumber));
        OnPropertyChanged(nameof(ShowCheckInStatus));
    }

    partial void OnStudentInfoChanged(StudentInfoModel value)
    {
        OnPropertyChanged(nameof(ShowRoomNumber));
        OnPropertyChanged(nameof(ShowCardInfo));
        OnPropertyChanged(nameof(RoomDisplay));
        OnPropertyChanged(nameof(CheckInStartText));
        OnPropertyChanged(nameof(CheckInEndText));
        OnPropertyChanged(nameof(TrainingStartText));
        OnPropertyChanged(nameof(TrainingEndText));
        OnPropertyChanged(nameof(TrainingDateRange));
        OnPropertyChanged(nameof(CardStatusText));
        OnPropertyChanged(nameof(CardStatusBrush));
        OnPropertyChanged(nameof(CheckInStatusText));
        OnPropertyChanged(nameof(CheckInStatusBrush));
    }

    partial void OnFeeInfoChanged(StudentFeeInfoModel value)
    {
        OnPropertyChanged(nameof(PaymentStatusText));
        OnPropertyChanged(nameof(PaymentStatusBrush));
    }

    [RelayCommand]
    private void LoadData(StudentInfoPageParameter parameter)
    {
        CurrentMode = parameter.Mode;

        if (parameter.Data is not null)
        {
            StudentInfo = parameter.Data;
        }

        if (parameter.FeeInfo is not null)
        {
            FeeInfo = parameter.FeeInfo;
        }
    }

    [RelayCommand]
    private void Refresh()
    {
        OnPropertyChanged(nameof(ShowPhoto));
        OnPropertyChanged(nameof(ShowFeeInfo));
        OnPropertyChanged(nameof(ShowIdCardNumber));
        OnPropertyChanged(nameof(ShowRoomNumber));
        OnPropertyChanged(nameof(ShowCheckInStatus));
        OnPropertyChanged(nameof(ShowCardInfo));
        OnPropertyChanged(nameof(RoomDisplay));
        OnPropertyChanged(nameof(CheckInStartText));
        OnPropertyChanged(nameof(CheckInEndText));
        OnPropertyChanged(nameof(TrainingStartText));
        OnPropertyChanged(nameof(TrainingEndText));
        OnPropertyChanged(nameof(TrainingDateRange));
        OnPropertyChanged(nameof(CardStatusText));
        OnPropertyChanged(nameof(CardStatusBrush));
        OnPropertyChanged(nameof(CheckInStatusText));
        OnPropertyChanged(nameof(CheckInStatusBrush));
        OnPropertyChanged(nameof(PaymentStatusText));
        OnPropertyChanged(nameof(PaymentStatusBrush));
    }

    private static string FormatDateTime(DateTime? value)
    {
        return value?.ToString("yyyy-MM-dd HH:mm") ?? string.Empty;
    }

    private static string FormatDate(DateTime value)
    {
        return value.ToString("yyyy-MM-dd");
    }
}

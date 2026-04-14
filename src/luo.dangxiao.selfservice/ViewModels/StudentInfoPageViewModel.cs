using Avalonia.Media;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

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
/// Student card status.
/// </summary>
public enum StudentCardStatus
{
    Normal,
    PendingPickup,
    Lost,
    Unissued
}

/// <summary>
/// Check-in status.
/// </summary>
public enum StudentCheckInStatus
{
    CheckedIn,
    NotCheckedIn
}

/// <summary>
/// Payment status.
/// </summary>
public enum StudentPaymentStatus
{
    Paid,
    Unpaid
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
/// Student basic/training/card data.
/// </summary>
public sealed class StudentInfoModel
{
    public string Id { get; set; } = string.Empty;

    public string Name { get; set; } = string.Empty;

    public string Gender { get; set; } = string.Empty;

    public string IdCardNumber { get; set; } = string.Empty;

    public string ClassName { get; set; } = string.Empty;

    public DateTime TrainingStartDate { get; set; }

    public DateTime TrainingEndDate { get; set; }

    public string CardNumber { get; set; } = string.Empty;

    public StudentCardStatus CardStatus { get; set; }

    public string RoomNumber { get; set; } = string.Empty;

    public StudentCheckInStatus CheckInStatus { get; set; }

    public string PhotoUrl { get; set; } = string.Empty;
}

/// <summary>
/// Student fee data.
/// </summary>
public sealed class StudentFeeInfoModel
{
    public decimal TrainingFee { get; set; }

    public decimal AccommodationFee { get; set; }

    public decimal MealFee { get; set; }

    public decimal TotalFee { get; set; }

    public StudentPaymentStatus PaymentStatus { get; set; }
}

/// <summary>
/// ViewModel for StudentInfoPage module.
/// </summary>
public partial class StudentInfoPageViewModel : ViewModelBase
{
    [ObservableProperty]
    private StudentInfoModel _studentInfo = new()
    {
        Id = "STU20260001",
        Name = "李天明",
        Gender = "男",
        IdCardNumber = "430101199001011234",
        ClassName = "测试培训班一",
        TrainingStartDate = new DateTime(2020, 09, 01),
        TrainingEndDate = new DateTime(2020, 09, 05),
        CardNumber = "20200901001",
        CardStatus = StudentCardStatus.PendingPickup,
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

    public bool ShowRoomNumber => !string.IsNullOrWhiteSpace(StudentInfo.RoomNumber) && CurrentMode != StudentInfoDisplayMode.WithPhoto;

    public bool ShowCheckInStatus => CurrentMode != StudentInfoDisplayMode.WithPhoto;

    public bool ShowCardInfo => !string.IsNullOrWhiteSpace(StudentInfo.CardNumber);

    public string TrainingDateRange => $"{StudentInfo.TrainingStartDate:yyyy-MM-dd} 至 {StudentInfo.TrainingEndDate:yyyy-MM-dd}";

    public string CardStatusText => StudentInfo.CardStatus switch
    {
        StudentCardStatus.Normal => "正常",
        StudentCardStatus.PendingPickup => "待领取",
        StudentCardStatus.Lost => "已挂失",
        StudentCardStatus.Unissued => "未制卡",
        _ => "未知"
    };

    public IBrush CardStatusBrush => StudentInfo.CardStatus switch
    {
        StudentCardStatus.Normal => Brush.Parse("#4CAF50"),
        StudentCardStatus.PendingPickup => Brush.Parse("#ff6000"),
        StudentCardStatus.Lost => Brush.Parse("#d9230b"),
        StudentCardStatus.Unissued => Brush.Parse("#70706d"),
        _ => Brush.Parse("#70706d")
    };

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
        OnPropertyChanged(nameof(TrainingDateRange));
        OnPropertyChanged(nameof(CardStatusText));
        OnPropertyChanged(nameof(CardStatusBrush));
        OnPropertyChanged(nameof(CheckInStatusText));
        OnPropertyChanged(nameof(CheckInStatusBrush));
        OnPropertyChanged(nameof(PaymentStatusText));
        OnPropertyChanged(nameof(PaymentStatusBrush));
    }
}

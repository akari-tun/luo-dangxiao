using luo.dangxiao.common.Enums;

namespace luo.dangxiao.models;

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
/// Student basic/training/card data.
/// </summary>
public sealed class StudentInfoModel : UserInfoModel
{
    public string Gender { get; set; } = string.Empty;

    public string ClassName { get; set; } = string.Empty;

    public DateTime? CheckInStartTime { get; set; }

    public DateTime? CheckInEndTime { get; set; }

    public DateTime TrainingStartDate { get; set; }

    public DateTime TrainingEndDate { get; set; }

    public string CardNumber { get; set; } = string.Empty;

    public StudentCardStatus CardStatus { get; set; }

    public string RoomName { get; set; } = string.Empty;

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

using luo.dangxiao.common.Enums;

namespace luo.dangxiao.models;

/// <summary>
/// Staff card status.
/// </summary>
public enum StaffCardStatus
{
    Normal,
    PendingPickup,
    Lost,
    Frozen
}

/// <summary>
/// Staff basic data.
/// </summary>
public sealed class StaffInfoModel : UserInfoModel
{
    public string Gender { get; set; } = string.Empty;

    public string Department { get; set; } = string.Empty;

    public string EmployeeNumber { get; set; } = string.Empty;

    public string CardType { get; set; } = string.Empty;

    public string CardNumber { get; set; } = string.Empty;

    public StaffCardStatus CardStatus { get; set; }

    public decimal ConsumptionBalance { get; set; }

    public decimal SubsidyBalance { get; set; }

    public decimal CardBalance { get; set; }

    public DateTime? CardIssueDate { get; set; }

    public DateTime? CardExpiryDate { get; set; }

    public string PhoneNumber { get; set; } = string.Empty;

    public string PhotoUrl { get; set; } = string.Empty;
}

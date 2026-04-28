namespace luo.dangxiao.models;

/// <summary>
/// User card information model, mapped from API userCards array.
/// </summary>
public sealed class CardInfoModel
{
    public string CardId { get; set; } = string.Empty;

    public string CardNo { get; set; } = string.Empty;

    public string FactoryFixId { get; set; } = string.Empty;

    public string CardStatusName { get; set; } = string.Empty;

    public int CardStatusId { get; set; }

    public string CardTypeName { get; set; } = string.Empty;

    public DateTime? ExpiryDate { get; set; }

    public DateTime? StatusChangeTime { get; set; }

    public int MainDeputyType { get; set; }

    public string MainDeputyTypeName { get; set; } = string.Empty;

    public string TenantId { get; set; } = string.Empty;

    public decimal Deposit { get; set; }

    public decimal IssueFee { get; set; }
}

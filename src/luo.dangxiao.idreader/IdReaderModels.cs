namespace luo.dangxiao.idreader;

/// <summary>
/// ID card type identifiers.
/// </summary>
public enum IdTypeEnum
{
    /// <summary>
    /// Chinese resident ID card.
    /// </summary>
    ChineseId = 0,

    /// <summary>
    /// Foreign permanent resident ID card.
    /// </summary>
    ForeignPermanentResident = 1,

    /// <summary>
    /// Hong Kong, Macao, or Taiwan resident ID card.
    /// </summary>
    HkMacaoTaiwanResident = 2,

    /// <summary>
    /// New foreign permanent resident ID card.
    /// </summary>
    NewForeignPermanentResident = 4,
}

/// <summary>
/// Unified ID card data structure shared across all ID reader implementations.
/// </summary>
public struct IdCardData
{
    /// <summary>
    /// Gets or sets the card holder name.
    /// </summary>
    public string Name { get; set; }

    /// <summary>
    /// Gets or sets the card holder gender.
    /// </summary>
    public string Gender { get; set; }

    /// <summary>
    /// Gets or sets the card holder nation.
    /// </summary>
    public string Nation { get; set; }

    /// <summary>
    /// Gets or sets the birth date.
    /// </summary>
    public string Birthday { get; set; }

    /// <summary>
    /// Gets or sets the ID number.
    /// </summary>
    public string IdNumber { get; set; }

    /// <summary>
    /// Gets or sets the address.
    /// </summary>
    public string Address { get; set; }

    /// <summary>
    /// Gets or sets the issuing department.
    /// </summary>
    public string Department { get; set; }

    /// <summary>
    /// Gets or sets the validity start date.
    /// </summary>
    public string StartDate { get; set; }

    /// <summary>
    /// Gets or sets the validity end date.
    /// </summary>
    public string EndDate { get; set; }

    /// <summary>
    /// Gets or sets the card holder photo bytes.
    /// </summary>
    public byte[]? PhotoBytes { get; set; }

    /// <summary>
    /// Gets or sets the card type.
    /// </summary>
    public IdTypeEnum CertType { get; set; }

    /// <summary>
    /// Gets or sets the UID.
    /// </summary>
    public string? Uid { get; set; }

    /// <summary>
    /// Gets or sets the SAM ID.
    /// </summary>
    public string? SamId { get; set; }
}

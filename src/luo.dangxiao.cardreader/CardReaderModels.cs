using System;

namespace luo.dangxiao.cardreader;

/// <summary>
/// Card type identifiers — matches hardware card classification.
/// </summary>
public enum CardTypeEnum
{
    UserCard = 0,
    OperatorCard = 1,
    SystemCard = 2,
    InitializationCard = 3,
    BlankCard = 4,
    WaterSavingSettingCard = 5,
    CollectionCard = 6,
    EncryptionCard = 7,
    QueryCard = 8,
    MachineNumberSettingCard = 9,
    TimeSettingCard = 10,
}

/// <summary>
/// Unified card data structure shared across all card reader implementations.
/// </summary>
public struct CardData
{
    /// <summary>
    /// Card serial number (卡流水号).
    /// </summary>
    public uint CardId { get; set; }

    /// <summary>
    /// Physical card ID / factory fixed ID (物理卡号).
    /// </summary>
    public uint FactoryFixId { get; set; }

    /// <summary>
    /// Card type identifier (卡类型).
    /// </summary>
    public int CardTypeId { get; set; }

    /// <summary>
    /// User number linked to the card (用户编号).
    /// </summary>
    public string UserNo { get; set; }

    /// <summary>
    /// Card expiration date (有效期).
    /// </summary>
    public DateTime ExpirDate { get; set; }

    /// <summary>
    /// Consumption card balance — consumer account (消费卡余).
    /// </summary>
    public decimal ConsumeValue { get; set; }

    /// <summary>
    /// Water control card balance — water account (水控卡余).
    /// </summary>
    public decimal WaterValue { get; set; }
}

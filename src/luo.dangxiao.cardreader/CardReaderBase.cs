using System;

namespace luo.dangxiao.cardreader;

/// <summary>
/// Base abstraction for card reader providers.
/// All concrete reader implementations must inherit from this class.
/// </summary>
public abstract class CardReaderBase : IDisposable
{
    /// <summary>
    /// Gets the reader provider name.
    /// </summary>
    public abstract string ProviderName { get; }

    /// <summary>
    /// Reads the factory fixed ID (physical card number) from the card on the reader.
    /// </summary>
    /// <param name="factoryFixId">The physical card ID (factory fixed ID).</param>
    /// <returns>True when the read succeeds.</returns>
    public abstract bool ReadCardId(out uint factoryFixId);

    /// <summary>
    /// Reads the card type from the card on the reader.
    /// </summary>
    /// <param name="cardType">
    /// Card type: 0=用户卡, 1=操作员卡, 2=系统卡, 3=初始化卡, 4=白卡,
    /// 5=节水设置卡, 6=采集卡, 7=加密卡, 8=查询卡, 9=机号设置卡, 10=时间设置卡.
    /// </param>
    /// <returns>True when the read succeeds.</returns>
    public abstract bool ReadCardType(out int cardType);

    /// <summary>
    /// Reads full user card data including consumption and water control sector data.
    /// </summary>
    /// <param name="cardData">The populated card data structure.</param>
    /// <returns>True when the read succeeds.</returns>
    public abstract bool ReadCard(out CardData cardData);

    /// <summary>
    /// Recycles (releases/halts) the current card session.
    /// </summary>
    /// <returns>True when the release succeeds.</returns>
    public abstract bool RecycleCard();

    /// <summary>
    /// Performs a consumption (payment) recharge on the card.
    /// </summary>
    /// <param name="amount">The recharge amount.</param>
    /// <param name="balance">The updated card balance after recharge.</param>
    /// <returns>True when the recharge succeeds.</returns>
    public abstract bool ConsumeRecharge(decimal amount, out decimal balance);

    /// <summary>
    /// Performs a water control recharge on the card.
    /// </summary>
    /// <param name="amount">The recharge amount.</param>
    /// <param name="balance">The updated water balance after recharge.</param>
    /// <returns>True when the recharge succeeds.</returns>
    public abstract bool WaterRecharge(decimal amount, out decimal balance);

    /// <summary>
    /// Releases resources held by the reader.
    /// </summary>
    public virtual void Dispose()
    {
    }
}

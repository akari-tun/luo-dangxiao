using System;

namespace luo.dangxiao.cardreader.Virtual;

/// <summary>
/// Simulated card reader for development and testing.
/// Returns deterministic mock data for all operations.
/// </summary>
public sealed class VirtualCardReader : CardReaderBase
{
    private bool _cardPresent = true;
    private decimal _consumeBalance = 100.00m;
    private decimal _waterBalance = 50.00m;

    /// <inheritdoc />
    public override string ProviderName => "Virtual";

    /// <inheritdoc />
    public override bool ReadCardId(out uint factoryFixId)
    {
        if (!_cardPresent)
        {
            factoryFixId = 0;
            return false;
        }

        factoryFixId = 1348446621;
        return true;
    }

    /// <inheritdoc />
    public override bool ReadCardType(out int cardType)
    {
        cardType = _cardPresent ? (int)CardTypeEnum.UserCard : -1;
        return _cardPresent;
    }

    /// <inheritdoc />
    public override bool ReadCard(out CardData cardData)
    {
        if (!_cardPresent)
        {
            cardData = default;
            return false;
        }

        cardData = new CardData
        {
            CardId = 40033,
            FactoryFixId = 1348446620,
            CardTypeId = (int)CardTypeEnum.UserCard,
            UserNo = "20260001",
            ExpirDate = new DateTime(2027, 12, 31),
            ConsumeValue = _consumeBalance,
            WaterValue = _waterBalance,
        };
        return true;
    }

    /// <inheritdoc />
    public override bool RecycleCard()
    {
        if (!_cardPresent) return false;
        _cardPresent = false;
        return true;
    }

    /// <inheritdoc />
    public override bool ConsumeRecharge(decimal amount, out decimal balance)
    {
        balance = 0;
        if (!_cardPresent || amount < 0) return false;

        _consumeBalance += amount;
        balance = _consumeBalance;
        return true;
    }

    /// <inheritdoc />
    public override bool WaterRecharge(decimal amount, out decimal balance)
    {
        balance = 0;
        if (!_cardPresent || amount < 0) return false;

        _waterBalance += amount;
        balance = _waterBalance;
        return true;
    }
}

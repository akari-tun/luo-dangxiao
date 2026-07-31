using System;
using System.Globalization;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;

namespace luo.dangxiao.cardreader.Yc;

/// <summary>
/// YC hardware card reader driver — adapts YCCARD low-level API to CardReaderBase.
/// </summary>
public sealed class YcCardReader : CardReaderBase
{
    private readonly CardReader _reader;
    private readonly CardConsumption _consumption;
    private bool _disposed;

    public YcCardReader()
    {
        _reader = new CardReader();
        _consumption = new CardConsumption(_reader);
    }

    /// <inheritdoc />
    public override string ProviderName => "Yc";

    /// <inheritdoc />
    public override bool ReadCardId(out uint factoryFixId)
    {
        factoryFixId = 0;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            int result = _consumption.ReadCardId(out _, out uint serialNumber);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            factoryFixId = serialNumber;
            return true;
        }
        finally
        {
            _reader.Halt();
        }
    }

    /// <inheritdoc />
    public override bool ReadCardType(out int cardType)
    {
        cardType = -1;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            int result = _consumption.QueryCardType(
                out int sysType, out int cType, out _, out _, 3000);

            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // sysType maps to our CardTypeEnum:
            // sysType 0 + data1[0]==3 => SystemCard (2)
            // sysType 0 + data1[0]==4 => OperatorCard (1)
            // sysType 0 + data1[0]==2 => InitializationCard (3)
            // sysType Payment/Water/Access => UserCard (0)
            cardType = MapToCardTypeEnum(sysType, cType);
            return true;
        }
        finally
        {
            _reader.Halt();
        }
    }

    /// <inheritdoc />
    public override bool ReadCard(out CardData cardData)
    {
        cardData = default;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            // Step 1: Get card serial number (physical card ID)
            int result = _consumption.ReadCardIdNew(out _, out uint cardSerno);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Step 2: Query card type
            result = _consumption.QueryCardType(
                out int sysType, out int cType, out _, out _, 3000);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Step 3: Try reading consumption sector data
            decimal consumeValue = 0;
            decimal waterValue = 0;
            string userNo = string.Empty;
            DateTime expirDate = DateTime.MinValue;

            // Attempt to read payment (consumption) card data
            result = _consumption.QueryPosUserCard12(
                out _, out _, out int serno, out string cardNo, out _,
                out _, out int value, out _, out _, out uint useTerm, out _,
                3000);

            if (result == (int)ErrorCode.Success)
            {
                consumeValue = value / 100m; // Value stored in cents
                userNo = serno.ToString(CultureInfo.InvariantCulture);
                expirDate = DecodeTermDate(useTerm);
            }

            // Attempt to read water billing card data
            result = _consumption.QueryJsCard(
                out _, out _, out int jsSerno, out _, out _,
                out int jsValue, out _, out _, 3000);

            if (result == (int)ErrorCode.Success)
            {
                waterValue = jsValue / 100m;
                if (string.IsNullOrEmpty(userNo))
                {
                    userNo = jsSerno.ToString(CultureInfo.InvariantCulture);
                }
            }

            cardData = new CardData
            {
                CardId = cardSerno,
                FactoryFixId = cardSerno,
                CardTypeId = MapToCardTypeEnum(sysType, cType),
                UserNo = userNo,
                ExpirDate = expirDate,
                ConsumeValue = consumeValue,
                WaterValue = waterValue,
            };

            return true;
        }
        finally
        {
            _reader.Halt();
        }
    }

    /// <inheritdoc />
    public override bool RecycleCard()
    {
        if (!_reader.IsOpen) return false;

        try
        {
            _reader.Halt();
            return true;
        }
        finally
        {
            _reader.Close();
        }
    }

    /// <inheritdoc />
    public override bool ConsumeRecharge(decimal amount, out decimal balance)
    {
        balance = 0;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            int result = _consumption.ReadCardIdNew(out _, out uint cardSerno);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Convert decimal amount to integer (cents) for card write
            int amountInt = (int)Math.Round(amount * 100m);

            result = _consumption.WrtPosUserCard12(amountInt, cardSerno, 3000);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Read back balance
            result = _consumption.ReadPosBalance(
                out uint balanceXf, out _, cardSerno, 3000);

            if (result == (int)ErrorCode.Success)
            {
                balance = balanceXf / 100m;
            }

            return result == (int)ErrorCode.Success;
        }
        finally
        {
            _reader.Halt();
        }
    }

    /// <inheritdoc />
    public override bool WaterRecharge(decimal amount, out decimal balance)
    {
        balance = 0;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            int result = _consumption.ReadCardIdNew(out _, out uint cardSerno);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Convert decimal amount to integer (cents) for card write
            int amountInt = (int)Math.Round(amount * 100m);

            result = _consumption.WrtJsUserCard(amountInt, cardSerno);
            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            // Read back balance via QueryJsCard
            result = _consumption.QueryJsCard(
                out _, out _, out _, out _, out _,
                out int jsValue, out _, out _, 3000);

            if (result == (int)ErrorCode.Success)
            {
                balance = jsValue / 100m;
            }

            return result == (int)ErrorCode.Success;
        }
        finally
        {
            _reader.Halt();
        }
    }

    private static int MapToCardTypeEnum(int sysType, int cardType)
    {
        // Map from YC QueryCardType results to our CardTypeEnum values
        if (sysType is (int)SystemTypes.Payment
            or (int)SystemTypes.WaterBilling
            or (int)SystemTypes.AccessControl)
        {
            return (int)CardTypeEnum.UserCard; // 0
        }

        // cardType from data1[0] parsing in QueryCardType:
        // 2 = system card, 1 = operator card, 3 = initialization card
        return cardType >= 0 && cardType <= 10 ? cardType : -1;
    }

    /// <inheritdoc />
    public override bool InitCard(
        int serno,
        string cardNo,
        int userType,
        int initialValue,
        int useCount,
        uint useTerm,
        out uint factoryFixId,
        int keyMode = 1,
        string empStrId = "U001",
        string empName = "",
        string cardTypeName = "")
    {
        factoryFixId = 0;

        if (!_reader.Open())
        {
            return false;
        }

        try
        {
            var userCode = UserCode.BuildUserCode(
                keyMode, empStrId, empName, cardTypeName);

            int result = _consumption.InitPosUserCardN12(
                serno, cardNo, userType, initialValue, useCount, 3000,
                out uint cardSerno, useTerm,
                userCode: userCode);

            if (result != (int)ErrorCode.Success)
            {
                return false;
            }

            factoryFixId = cardSerno;
            return true;
        }
        finally
        {
            _reader.Halt();
        }
    }

    private static DateTime DecodeTermDate(uint useTerm)
    {
        if (useTerm == 0) return DateTime.MinValue;

        // useTerm is encoded as YYYYMMDD packed integer
        int year = (int)(useTerm / 10000);
        int month = (int)((useTerm % 10000) / 100);
        int day = (int)(useTerm % 100);

        try
        {
            return new DateTime(year, month, day);
        }
        catch (ArgumentOutOfRangeException)
        {
            return DateTime.MinValue;
        }
    }

    /// <inheritdoc />
    public override void Dispose()
    {
        if (_disposed) return;

        try
        {
            RecycleCard();
            _reader.Dispose();
        }
        finally
        {
            _disposed = true;
        }
    }
}

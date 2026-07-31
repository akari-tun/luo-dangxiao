using luo.dangxiao.cardreader.Yc.Protocol;
using luo.dangxiao.cardreader.Yc.Structs;

namespace luo.dangxiao.cardreader.Yc;

public class CardConsumption : IDisposable
{
    private readonly CardReader _reader;
    private bool _disposed;

    public LicenseManager License => LicenseManager.Instance;

    private static byte[]? GetSystemKey6(byte[]? key)
    {
        if (key == null || key.Length < 6) return null;
        return key.Length == 6 ? key : key[..6];
    }

    private static void LogInit(string msg)
    {
        try
        {
            var logPath = System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "poscard_consumption.log");
            System.IO.File.AppendAllText(logPath, $"[CC] {DateTime.Now:HH:mm:ss.fff} {msg}\n");
        } catch { }
    }

    private static readonly byte[] LockCardKeyB = { 0xDD, 0xEA, 0xFC, 0xA0, 0xBC, 0xD1 };
    private static readonly byte[] FactoryKey = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    private static int ResolveKeyMode(UserCode? userCode)
    {
        if (userCode?.Data == null || userCode.Data.Length <= 8)
        {
            return 0;
        }

        return userCode.Data[8] switch
        {
            <= 2 => userCode.Data[8] + 1,
            _ => 0
        };
    }

    private bool LoadUserSectorKeys(byte sector, byte[] password, byte[] cardSerial, UserCode? userCode = null)
    {
        int keyMode = ResolveKeyMode(userCode);

        if (keyMode == 0)
        {
            return false;
        }

        byte[] keyA;
        byte[] keyB;

        switch (keyMode)
        {
            case 1:
                keyA = FactoryKey;
                keyB = FactoryKey;
                break;
            case 2:
                keyA = KeyCalculator.UserCard1KeyA;
                keyB = LockCardKeyB;
                break;
            case 3:
                byte[] calcKeyBytes = new byte[16];
                KeyCalculator.CalculateKey12(cardSerial, password, calcKeyBytes);
                keyA = calcKeyBytes[..6];
                keyB = calcKeyBytes[10..16];
                break;
            default:
                return false;
        }

        return _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, sector, keyA) == (int)ErrorCode.Success
            && _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet1, sector, keyA) == (int)ErrorCode.Success
            && _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet0, sector, keyB) == (int)ErrorCode.Success
            && _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet1, sector, keyB) == (int)ErrorCode.Success;
    }

    private bool AuthenticateLoadedUserSector(byte sector)
    {
        if (_reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, sector) == (int)ErrorCode.Success)
        {
            return true;
        }

        if (_reader.Halt() != (int)ErrorCode.Success || _reader.Card(0x52, out _) != (int)ErrorCode.Success)
        {
            return false;
        }

        if (_reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet1, sector) == (int)ErrorCode.Success)
        {
            return true;
        }

        if (_reader.Halt() != (int)ErrorCode.Success || _reader.Card(0x52, out _) != (int)ErrorCode.Success)
        {
            return false;
        }

        return _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, sector) == (int)ErrorCode.Success;
    }

    private bool AuthenticateUserSector(byte sector, byte[] password, byte[] cardSerial, UserCode? userCode = null)
    {
        return LoadUserSectorKeys(sector, password, cardSerial, userCode) && AuthenticateLoadedUserSector(sector);
    }

    private int ReadUserCardBlocks(byte sector, byte[] data0, byte[] data1, byte[] data2)
    {
        int result = _reader.Read((byte)(sector * 4), data0);
        if (result != (int)ErrorCode.Success)
        {
            return (int)ErrorCode.ReadCardError;
        }

        result = _reader.Read((byte)(sector * 4 + 1), data1);
        if (result != (int)ErrorCode.Success)
        {
            return (int)ErrorCode.ReadCardError;
        }

        result = _reader.Read((byte)(sector * 4 + 2), data2);
        if (result != (int)ErrorCode.Success)
        {
            return (int)ErrorCode.ReadCardError;
        }

        return (int)ErrorCode.Success;
    }

    private void ReadUserCodeSector(byte usercardSec, UserCode? userCode, uint cardSerno)
    {
        if (userCode?.Data == null) return;

        byte dataSec = (byte)(usercardSec + 1);
        LogInit($"ReadUserCodeSector: Trying sector {dataSec}, cardSerial={cardSerno}");

        // Reselect card
        if (_reader.Card(0x52, out _) != (int)ErrorCode.Success)
        {
            LogInit("ReadUserCodeSector: Card reselect FAILED");
            return;
        }

        // Try SystemKeyB with KEYB|KEYSET2
        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 != null)
        {
            LogInit($"ReadUserCodeSector: SYSKEYB={BitConverter.ToString(sysKey6).Replace("-", "")}");
            int loadR = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, dataSec, sysKey6);
            LogInit($"  LoadKey(SYSKEYB|SET2, dataSec={dataSec})={loadR}");
            if (loadR == (int)ErrorCode.Success)
            {
                int authR = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, dataSec);
                LogInit($"  Auth(SYSKEYB|SET2, dataSec={dataSec})={authR}");
                if (authR == (int)ErrorCode.Success)
                {
                    LogInit("ReadUserCodeSector: SYSKEYB auth OK, reading data...");
                    ReadUserCodeBlocks(dataSec, userCode.Data);
                    _reader.Halt();
                    return;
                }

                // Try with reselect + same key
                if (_reader.Halt() == (int)ErrorCode.Success && _reader.Card(0x52, out _) == (int)ErrorCode.Success)
                {
                    authR = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, dataSec);
                    LogInit($"  Auth(SYSKEYB|SET2, dataSec={dataSec}, reselect)={authR}");
                    if (authR == (int)ErrorCode.Success)
                    {
                        LogInit("ReadUserCodeSector: SYSKEYB auth OK (reselect), reading data...");
                        ReadUserCodeBlocks(dataSec, userCode.Data);
                        _reader.Halt();
                        return;
                    }
                }
            }
        }

        // Try fixed keys: UserCard1_KeyA and LockCard_KeyB
        {
            byte[] keyA = KeyCalculator.UserCard1KeyA;
            byte[] keyB = LockCardKeyB;
            LogInit($"ReadUserCodeSector: FIXED KeyA={BitConverter.ToString(keyA).Replace("-", "")}, KeyB={BitConverter.ToString(keyB).Replace("-", "")}");

            int r = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, dataSec, keyA);
            LogInit($"  LoadKey(KEYA|SET0, dataSec={dataSec})={r}");
            if (r == 0)
            {
                r = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, dataSec);
                LogInit($"  Auth(KEYA|SET0, dataSec={dataSec})={r}");
                if (r == 0)
                {
                    LogInit("ReadUserCodeSector: FIXED KeyA auth OK, reading data...");
                    ReadUserCodeBlocks(dataSec, userCode.Data);
                    _reader.Halt();
                    return;
                }
            }

            // Retry with KEYB
            if (_reader.Halt() == 0 && _reader.Card(0x52, out _) == 0)
            {
                r = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet0, dataSec, keyB);
                LogInit($"  LoadKey(KEYB|SET0, dataSec={dataSec})={r}");
                if (r == 0)
                {
                    r = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, dataSec);
                    LogInit($"  Auth(KEYB|SET0, dataSec={dataSec})={r}");
                    if (r == 0)
                    {
                        LogInit("ReadUserCodeSector: FIXED KeyB auth OK, reading data...");
                        ReadUserCodeBlocks(dataSec, userCode.Data);
                        _reader.Halt();
                        return;
                    }
                }
            }
        }

        // Try calculated keys
        {
            byte[] password = License.SystemInfo.OperatorPassword ?? Array.Empty<byte>();
            byte[] calcKey = new byte[16];
            KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), password, calcKey);
            byte[] calcKeyA = calcKey[..6];
            byte[] calcKeyB = calcKey[10..16];
            LogInit($"ReadUserCodeSector: CALC KeyA={BitConverter.ToString(calcKeyA).Replace("-", "")}, KeyB={BitConverter.ToString(calcKeyB).Replace("-", "")}");

            // Load keys into both slots (C++ loads both)
            int r = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, dataSec, calcKeyA);
            int r2 = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet1, dataSec, calcKeyA);
            int r3 = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet0, dataSec, calcKeyB);
            LogInit($"  LoadKey(CALC|SET0/SET1, dataSec={dataSec}) => A0={r}, A1={r2}, B0={r3}");
            if (r == 0)
            {
                int authR = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, dataSec);
                LogInit($"  Auth(CALC A|SET0, dataSec={dataSec})={authR}");
                if (authR == 0)
                {
                    LogInit("ReadUserCodeSector: CALC KeyA auth OK, reading data...");
                    ReadUserCodeBlocks(dataSec, userCode.Data);
                    _reader.Halt();
                    return;
                }

                if (_reader.Halt() == 0 && _reader.Card(0x52, out _) == 0)
                {
                    authR = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet1, dataSec);
                    LogInit($"  Auth(CALC A|SET1, dataSec={dataSec}, reselect)={authR}");
                    if (authR == 0)
                    {
                        LogInit("ReadUserCodeSector: CALC KeyA (SET1) auth OK, reading data...");
                        ReadUserCodeBlocks(dataSec, userCode.Data);
                        _reader.Halt();
                        return;
                    }
                }
            }

            // Try KEYB from calc
            if (_reader.Halt() == 0 && _reader.Card(0x52, out _) == 0)
            {
                int authR = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, dataSec);
                LogInit($"  Auth(CALC B|SET0, dataSec={dataSec})={authR}");
                if (authR == 0)
                {
                    LogInit("ReadUserCodeSector: CALC KeyB auth OK, reading data...");
                    ReadUserCodeBlocks(dataSec, userCode.Data);
                    _reader.Halt();
                    return;
                }
            }
        }

        LogInit($"ReadUserCodeSector: ALL MODES FAILED for sector {dataSec}");
    }

    private void ReadUserCodeBlocks(byte sector, byte[] data)
    {
        byte[] block0 = new byte[16];
        byte[] block1 = new byte[16];
        byte[] block2 = new byte[16];

        int r0 = _reader.Read((byte)(sector * 4), block0);
        int r1 = _reader.Read((byte)(sector * 4 + 1), block1);
        int r2 = _reader.Read((byte)(sector * 4 + 2), block2);

        LogInit($"ReadUserCodeBlocks: r0={r0}, r1={r1}, r2={r2}");
        if (r0 == 0 && r1 == 0 && r2 == 0)
        {
            Buffer.BlockCopy(block0, 0, data, 0, 16);
            Buffer.BlockCopy(block1, 0, data, 16, 16);
            Buffer.BlockCopy(block2, 0, data, 32, 16);
            LogInit("ReadUserCodeBlocks: DATA READ SUCCESS");
        }
        else
        {
            LogInit("ReadUserCodeBlocks: READ FAILED");
        }
    }

    private static void ClearQueriedUserCode(UserCode? userCode)
    {
        if (userCode?.Data == null)
        {
            return;
        }

        if (userCode.Data.Length > 9)
        {
            Array.Clear(userCode.Data, 9, Math.Min(16, userCode.Data.Length - 9));
        }

        if (userCode.Data.Length > 25)
        {
            Array.Clear(userCode.Data, 25, Math.Min(16, userCode.Data.Length - 25));
        }
    }

    private void PopulatePosUserCardResult(
        byte[] data0,
        byte[] data1,
        byte[] data2,
        out int serno,
        out string cardNo,
        out int userType,
        out int value,
        out int lastPay,
        out int count,
        out uint useTerm,
        out int addCount)
    {
        serno = (data0[2] << 16) | (data0[1] << 8) | data0[0];
        userType = data0[3];

        value = data1[0] | (data1[1] << 8) | (data1[2] << 16);
        lastPay = data1[3] | (data1[4] << 8) | (data1[5] << 16);
        count = data1[11] | (data1[12] << 8);
        addCount = data0[9];

        uint temp1 = (uint)(((data0[6] & 0xF0) >> 4) | ((data0[7] & 0xE0) >> 1));
        temp1 = temp1 * 10000 + (uint)((data0[6] & 0x0F) * 100);
        useTerm = temp1 + (uint)(data0[7] & 0x1F);

        byte[] cardNoBytes = new byte[5];
        Buffer.BlockCopy(data0, 10, cardNoBytes, 0, 5);
        cardNo = System.Text.Encoding.ASCII.GetString(cardNoBytes).TrimEnd('\0', ' ');
    }

    public CardConsumption(CardReader reader)
    {
        _reader = reader ?? throw new ArgumentNullException(nameof(reader));
    }

    #region Card ID Operations

    public int ReadCardId(out ushort tagType, out uint serialNumber)
    {
        tagType = 0;
        serialNumber = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        int result = _reader.Request(RequestModes.All, out tagType);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Anticoll(0, out serialNumber);
        if (result != (int)ErrorCode.Success) return result;

        result = _reader.Select(serialNumber, out _);
        if (result == (int)ErrorCode.Success)
        {
            // Halt the card to release it for subsequent reads
            _reader.Halt();
        }
        return result;
    }

    public int ReadCardIdNew(out ushort tagType, out uint serialNumber)
    {
        tagType = 0;
        serialNumber = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        for (int retry = 0; retry < 3; retry++)
        {
            int result = _reader.Request(RequestModes.All, out tagType);
            if (result != (int)ErrorCode.Success)
            {
                _reader.Halt();
                continue;
            }

            result = _reader.Anticoll(0, out serialNumber);
            if (result != (int)ErrorCode.Success)
            {
                _reader.Halt();
                continue;
            }

            result = _reader.Select(serialNumber, out _);
            if (result == (int)ErrorCode.Success)
            {
                // Halt the card to release it for subsequent reads
                _reader.Halt();
                return result;
            }

            _reader.Halt();
        }

        return (int)ErrorCode.NoCard;
    }

    #endregion

    #region Query Card Type

    public int QueryCardType(out int sysType, out int cardType, out uint cardSerno, out int optNum, int waitTime, UserCode? userCode = null)
    {
        sysType = 0;
        cardType = 0;
        cardSerno = 0;
        optNum = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out ushort tagType, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        byte[] data1 = new byte[16];
        byte[] data2 = new byte[16];
        byte[] codeWord = new byte[16];

        uint usercardSec;

        result = _reader.Card(0x52, out _);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        if (License.HasPaymentAuth)
        {
            usercardSec = (uint)License.SystemInfo.PaymentSector;
            var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
            if (sysKey6 == null) return (int)ErrorCode.ParameterError;
            result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

            result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
            if (result == (int)ErrorCode.Success)
            {
                result = _reader.Read((byte)(usercardSec * 4), data1);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;
                result = _reader.Read((byte)(usercardSec * 4 + 1), data2);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

                _reader.Halt();
                cardType = 0;
                sysType = (int)SystemTypes.Payment;
                return (int)ErrorCode.Success;
            }
        }

        if (License.HasWaterBillingAuth)
        {
            usercardSec = (uint)License.SystemInfo.WaterBillingSector;
            var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
            if (sysKey6 == null) return (int)ErrorCode.ParameterError;
            result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

            result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
            if (result == (int)ErrorCode.Success)
            {
                result = _reader.Read((byte)(usercardSec * 4), data1);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;
                result = _reader.Read((byte)(usercardSec * 4 + 1), data2);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

                _reader.Halt();
                cardType = 0;
                sysType = (int)SystemTypes.WaterBilling;
                return (int)ErrorCode.Success;
            }
        }

        if (License.HasAccessControlAuth)
        {
            usercardSec = (uint)License.SystemInfo.AccessControlSector;
            var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
            if (sysKey6 == null) return (int)ErrorCode.ParameterError;
            result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

            result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
            if (result == (int)ErrorCode.Success)
            {
                result = _reader.Read((byte)(usercardSec * 4), data1);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;
                result = _reader.Read((byte)(usercardSec * 4 + 1), data2);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

                _reader.Halt();
                cardType = 0;
                sysType = (int)SystemTypes.AccessControl;
                return (int)ErrorCode.Success;
            }
        }

        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, 1, KeyCalculator.SystemCardKeyB12);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        _reader.Halt();
        result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, 1);
        if (result == (int)ErrorCode.Success)
        {
            result = _reader.Read(4, data1);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;
            result = _reader.Read(5, data2);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

            _reader.Halt();

            if (data1[0] == 3)
            {
                cardType = 2;
                sysType = 2;
                return (int)ErrorCode.Success;
            }

            if (data1[0] == 4)
            {
                cardType = 1;
                optNum = data2[0];
                sysType = 1;
                return (int)ErrorCode.Success;
            }

            if (data1[0] == 0x02)
            {
                cardType = 3;
                sysType = 0;
                return (int)ErrorCode.Success;
            }
        }

        return (int)ErrorCode.NotIdentified;
    }

    #endregion

    #region POS User Card Operations

    public int QueryPosUserCard12(
        out int cardType, out int optNum, out int serno, out string cardNo, out int userType,
        out uint cardSerno, out int value, out int lastPay, out int count,
        out uint useTerm, out int addCount, int waitTime, UserCode? userCode = null)
    {
        cardType = 0;
        optNum = 0;
        serno = 0;
        cardNo = string.Empty;
        userType = 0;
        cardSerno = 0;
        value = 0;
        lastPay = 0;
        count = 0;
        useTerm = 0;
        addCount = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        byte usercardSec = (byte)License.SystemInfo.PaymentSector;
        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];
        byte[] data2 = new byte[16];

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 == null) return (int)ErrorCode.ParameterError;
        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec, sysKey6);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec);

        if (result == (int)ErrorCode.Success)
        {
            result = ReadUserCardBlocks(usercardSec, data0, data1, data2);
            if (result != (int)ErrorCode.Success) return result;

            if (_reader.Halt() != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

            if (!KeyCalculator.VerifyBcc(data1) || !KeyCalculator.VerifyBcc(data2))
            {
                return (int)ErrorCode.UserCardError;
            }

            PopulatePosUserCardResult(data0, data1, data2, out serno, out cardNo, out userType, out value, out lastPay, out count, out useTerm, out addCount);
            ReadUserCodeSector(usercardSec, userCode, cardSerno);
            cardType = 0;
            return (int)ErrorCode.Success;
        }

        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec, KeyCalculator.SystemCardKeyB12);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        if (_reader.Halt() != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;
        result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec);
        if (result == (int)ErrorCode.Success)
        {
            result = _reader.Read(4, data0);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;
            result = _reader.Read(5, data1);
            if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

            _reader.Halt();

            bool isPaymentSystemCard = data0[0] == 3
                && License.SystemInfo.SystemCardNumberPayment.Take(5).SequenceEqual(data0.Skip(1).Take(5));
            bool isBlankCard = data0[0] == 0x02;
            bool isPaymentOperatorCard = data0[0] == 4
                && License.SystemInfo.SystemCardNumberPayment.Take(5).SequenceEqual(data0.Skip(1).Take(5));

            if (isPaymentSystemCard)
            {
                cardType = 2;
                return (int)ErrorCode.Success;
            }

            if (isBlankCard)
            {
                cardType = 3;
                return (int)ErrorCode.Success;
            }

            if (isPaymentOperatorCard)
            {
                cardType = 1;
                optNum = data1[0];
                return (int)ErrorCode.Success;
            }

            return (int)ErrorCode.UserCardError;
        }

        if (_reader.Halt() != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;
        result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        if (!LoadUserSectorKeys(usercardSec, License.SystemInfo.OperatorPassword ?? Array.Empty<byte>(), BitConverter.GetBytes(cardSerno), userCode))
        {
            return (int)ErrorCode.ParameterError;
        }

        if (!AuthenticateLoadedUserSector(usercardSec))
        {
            return (int)ErrorCode.NotIdentified;
        }

        result = ReadUserCardBlocks(usercardSec, data0, data1, data2);
        if (result != (int)ErrorCode.Success) return result;

        if (_reader.Halt() != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        if (!KeyCalculator.VerifyBcc(data1) || !KeyCalculator.VerifyBcc(data2))
        {
            return (int)ErrorCode.UserCardError;
        }

        PopulatePosUserCardResult(data0, data1, data2, out serno, out cardNo, out userType, out value, out lastPay, out count, out useTerm, out addCount);
        ReadUserCodeSector(usercardSec, userCode, cardSerno);
        cardType = 4;
        return (int)ErrorCode.Success;
    }

    public int InitPosUserCard12(int serno, string cardNo, int userType, int waitTime, out uint cardSerno, uint useTerm, UserCode? userCode = null)
    {
        cardSerno = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;
        if (serno <= 0 || waitTime < 0 || userType <= 0 || userType > 32) return (int)ErrorCode.ParameterError;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        uint usercardSec = (uint)License.SystemInfo.PaymentSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];
        byte[] data2 = new byte[16];
        byte[] data3 = new byte[16];

        data0[0] = (byte)(serno & 0xFF);
        data0[1] = (byte)((serno >> 8) & 0xFF);
        data0[2] = (byte)((serno >> 16) & 0xFF);
        data0[3] = (byte)userType;

        uint temp1 = (useTerm / 10000) % 1000;
        uint temp2 = (useTerm % 10000) / 100;
        data0[6] = (byte)((temp1 & 0x0F) << 4 | temp2);
        data0[7] = (byte)((useTerm % 10000) % 100 | ((temp1 & 0xF0) << 1));

        var cardNoBytes = System.Text.Encoding.ASCII.GetBytes(cardNo.PadRight(5, ' '));
        for (int i = 0; i < Math.Min(5, cardNoBytes.Length); i++)
        {
            data0[14 - i] = cardNoBytes[cardNoBytes.Length - 1 - i];
        }

        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        data1[15] = KeyCalculator.CalculateBcc(data1, 15);
        data2[15] = KeyCalculator.CalculateBcc(data2, 15);

        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword ?? Array.Empty<byte>(), data3);
        data3[6] = 0x7F;
        data3[7] = 0x07;
        data3[8] = 0x88;
        data3[9] = 0xDA;
        Buffer.BlockCopy(License.SystemInfo.SystemKeyB, 0, data3, 10, 6);

        LogInit($"InitPosUserCard12: sector={usercardSec}, cardSerno={cardSerno}");
        LogInit($"License: OperatorPassword={BitConverter.ToString(License.SystemInfo.OperatorPassword ?? Array.Empty<byte>()).Replace("-", "")}");
        LogInit($"License: SystemKeyB={BitConverter.ToString(License.SystemInfo.SystemKeyB ?? Array.Empty<byte>()).Replace("-", "")}");

        if (!AuthenticateUserSector((byte)usercardSec, License.SystemInfo.OperatorPassword ?? Array.Empty<byte>(), BitConverter.GetBytes(cardSerno), userCode))
            return (int)ErrorCode.UserCardError;

        if (_reader.Write((byte)(usercardSec * 4), data0) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 1), data1) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 2), data2) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 3), data3) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int InitPosUserCardN12(int serno, string cardNo, int userType, int value, int useCount, int waitTime, out uint cardSerno, uint useTerm, UserCode? userCode = null)
    {
        int result = InitPosUserCard12(serno, cardNo, userType, waitTime, out cardSerno, useTerm, userCode);
        if (result != (int)ErrorCode.Success) return result;

        userCode ??= new UserCode();

        uint usercardSec = (uint)License.SystemInfo.PaymentSector + 1;

        int cardResult2 = _reader.Card(0x52, out _);
        if (cardResult2 != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        if (!AuthenticateUserSector((byte)usercardSec, License.SystemInfo.OperatorPassword ?? Array.Empty<byte>(), BitConverter.GetBytes(cardSerno), userCode))
            return (int)ErrorCode.UserCardError;

        byte[] data3 = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, data3);
        data3[6] = 0x7F;
        data3[7] = 0x07;
        data3[8] = 0x88;
        data3[9] = 0xDA;
        Buffer.BlockCopy(License.SystemInfo.SystemKeyB, 0, data3, 10, 6);

        if (_reader.Write((byte)(usercardSec * 4), userCode.Data.AsSpan(0, 16).ToArray()) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 1), userCode.Data.AsSpan(16, 16).ToArray()) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 2), userCode.Data.AsSpan(32, 16).ToArray()) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 3), data3) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        if (value > 0)
        {
            result = WrtPosUserCard12(value, cardSerno, waitTime);
        }

        return result;
    }

    public int WrtPosUserCard12(int value, uint cardSerno, int waitTime)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;
        if (waitTime < 0 || value < 0) return (int)ErrorCode.ParameterError;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        uint usercardSec = (uint)License.SystemInfo.PaymentSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data1 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        data1[0] = (byte)(value & 0xFF);
        data1[1] = (byte)((value >> 8) & 0xFF);
        data1[2] = (byte)((value >> 16) & 0xFF);

        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        result = _reader.Write((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        byte[] data2 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4 + 2), data2);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        Buffer.BlockCopy(data1, 0, data2, 0, 14);
        data2[15] = KeyCalculator.CalculateBcc(data2, 15);

        result = _reader.Write((byte)(usercardSec * 4 + 2), data2);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int WrtPosUserCardAddCount12(int value, uint cardSerno, int waitTime)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;
        if (waitTime < 0 || value < 0) return (int)ErrorCode.ParameterError;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        uint usercardSec = (uint)License.SystemInfo.PaymentSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];

        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        data1[0] = (byte)(value & 0xFF);
        data1[1] = (byte)((value >> 8) & 0xFF);
        data1[2] = (byte)((value >> 16) & 0xFF);

        data0[9]++;

        data0[15] = KeyCalculator.CalculateBcc(data0, 15);
        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        result = _reader.Write((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int RstPosUserCard12(uint cardSerno, int waitTime, UserCode? userCode = null)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        uint usercardSec = (uint)License.SystemInfo.PaymentSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 == null) return (int)ErrorCode.ParameterError;
        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] emptyData = new byte[16];
        byte[] keyData = new byte[16];

        if (userCode.EncryptionMode == 0x01)
        {
            Buffer.BlockCopy(new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, 0, keyData, 0, 16);
        }
        else if (userCode.EncryptionMode == 0x02)
        {
            Buffer.BlockCopy(new byte[] { 0xF1, 0xCF, 0xD3, 0xED, 0xF9, 0x58, 0xFF, 0x07, 0x80, 0x69, 0xDD, 0xEA, 0xFC, 0xA0, 0xBC, 0xD1 }, 0, keyData, 0, 16);
        }
        else if (userCode.EncryptionMode == 0x03)
        {
            KeyCalculator.CalculateKey12New(BitConverter.GetBytes(cardSerno), userCode.Data, keyData);
        }
        else
        {
            return (int)ErrorCode.ParameterError;
        }

        result = _reader.Write((byte)(usercardSec * 4), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 1), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 3), keyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    #endregion

    #region POS Operator Card Operations

    public int InitPosOptCard12(int optNum, int waitTime, out uint cardSerno, UserCode? userCode = null)
    {
        cardSerno = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;
        if (optNum < 0 || waitTime < 0) return (int)ErrorCode.ParameterError;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        byte[] data1 = new byte[16];
        byte[] data2 = new byte[16];
        byte[] data3 = new byte[16];
        byte[] data4 = new byte[16];

        byte usercardSec = 1;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, usercardSec, KeyCalculator.DefaultKeyA1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, usercardSec);
        if (result != (int)ErrorCode.Success)
        {
            result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet1, usercardSec);
            if (result != (int)ErrorCode.Success)
            {
                result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, usercardSec);
                if (result != (int)ErrorCode.Success) return (int)ErrorCode.NotIdentified;
            }
        }

        data1[0] = 0x04;
        Buffer.BlockCopy(License.SystemInfo.SystemCardNumberPayment, 0, data1, 1, 5);
        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        data2[0] = (byte)(optNum & 0xFF);
        data2[1] = (byte)((optNum >> 8) & 0xFF);
        data2[2] = (byte)((optNum >> 16) & 0xFF);
        data2[3] = (byte)((optNum >> 24) & 0xFF);
        data2[15] = KeyCalculator.CalculateBcc(data2, 15);

        Buffer.BlockCopy(KeyCalculator.SystemCardKeyA12, 0, data3, 0, 6);
        data3[6] = 0x7F;
        data3[7] = 0x07;
        data3[8] = 0x88;
        data3[9] = 0xDA;
        Buffer.BlockCopy(KeyCalculator.SystemCardKeyB12, 0, data3, 10, 6);

        if (_reader.Write(4, data1) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write(5, data2) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write(6, data4) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write(7, data3) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int RstPosOptCard12(uint cardSerno, int waitTime, UserCode? userCode = null)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        byte usercardSec = 1;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec, KeyCalculator.SystemCardKeyB12);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] emptyData = new byte[16];

        result = _reader.Write(4, emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write(5, emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write(6, emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write(7, emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    #endregion

    #region JS (Water Billing) Card Operations

    public int QueryJsCard(out int cardType, out int optNum, out int serno, out string cardNo, out uint cardSerno, out int value, out int count, out int userType, int waitTime, UserCode? userCode = null)
    {
        cardType = 0;
        optNum = 0;
        serno = 0;
        cardNo = string.Empty;
        cardSerno = 0;
        value = 0;
        count = 0;
        userType = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasWaterBillingAuth) return (int)ErrorCode.NoWaterBillingAuthorization;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        uint usercardSec = (uint)License.SystemInfo.WaterBillingSector;

        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 == null) return (int)ErrorCode.ParameterError;
        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
        if (result != (int)ErrorCode.Success)
        {
            _reader.Halt();
            return (int)ErrorCode.UserCardError;
        }

        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];

        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        _reader.Halt();

        if (!KeyCalculator.VerifyBcc(data0) || !KeyCalculator.VerifyBcc(data1))
            return (int)ErrorCode.UserCardError;

        serno = (data0[2] << 16) | (data0[1] << 8) | data0[0];
        userType = data0[3];
        value = data1[0] | (data1[1] << 8) | (data1[2] << 16);
        count = data0[8] | (data0[9] << 8);

        var cardNoBytes = new byte[6];
        Buffer.BlockCopy(data0, 10, cardNoBytes, 0, 5);
        cardNo = System.Text.Encoding.ASCII.GetString(cardNoBytes).TrimEnd('\0');

        return (int)ErrorCode.Success;
    }

    public int InitJsUserCard(int serno, string cardNo, int userType, out uint cardSerno, UserCode? userCode = null)
    {
        return InitJsUserCardImpl(serno, 0, cardNo, userType, 0, 0, out cardSerno, userCode);
    }

    public int InitJsUserCardN(int serno, string cardNo, int userType, int cardBalance, int chargeTimes, out uint cardSerno, UserCode? userCode = null)
    {
        return InitJsUserCardImpl(serno, 0, cardNo, userType, cardBalance, chargeTimes, out cardSerno, userCode);
    }

    public int InitJsUserCardNew(int newCardSerno, int oldCardSerno, string cardNo, int userType, int cardBalance, int chargeTimes, out uint cardSerno, UserCode? userCode = null)
    {
        return InitJsUserCardImpl(newCardSerno, oldCardSerno, cardNo, userType, cardBalance, chargeTimes, out cardSerno, userCode);
    }

    private int InitJsUserCardImpl(int serno, int oldSerno, string cardNo, int userType, int cardBalance, int chargeTimes, out uint cardSerno, UserCode? userCode = null)
    {
        cardSerno = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasWaterBillingAuth) return (int)ErrorCode.NoWaterBillingAuthorization;
        if (serno <= 0 || userType <= 0 || userType > 32) return (int)ErrorCode.ParameterError;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        uint usercardSec = (uint)License.SystemInfo.WaterBillingSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];
        byte[] data2 = new byte[16];
        byte[] data3 = new byte[16];

        data0[0] = (byte)(serno & 0xFF);
        data0[1] = (byte)((serno >> 8) & 0xFF);
        data0[2] = (byte)((serno >> 16) & 0xFF);
        data0[3] = (byte)userType;

        data0[8] = (byte)(chargeTimes & 0xFF);
        data0[9] = (byte)((chargeTimes >> 8) & 0xFF);

        var cardNoBytes = System.Text.Encoding.ASCII.GetBytes(cardNo.PadRight(5, ' '));
        for (int i = 0; i < Math.Min(5, cardNoBytes.Length); i++)
        {
            data0[14 - i] = cardNoBytes[cardNoBytes.Length - 1 - i];
        }

        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        data1[0] = (byte)(cardBalance & 0xFF);
        data1[1] = (byte)((cardBalance >> 8) & 0xFF);
        data1[2] = (byte)((cardBalance >> 16) & 0xFF);
        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, data3);
        data3[6] = 0x7F;
        data3[7] = 0x07;
        data3[8] = 0x88;
        data3[9] = 0xDA;
        Buffer.BlockCopy(License.SystemInfo.SystemKeyB, 0, data3, 10, 6);

        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 == null) return (int)ErrorCode.ParameterError;
        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        if (_reader.Write((byte)(usercardSec * 4), data0) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 1), data1) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 2), data1) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;
        if (_reader.Write((byte)(usercardSec * 4 + 3), data3) != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int WrtJsUserCard(int balance, uint cardSerno)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasWaterBillingAuth) return (int)ErrorCode.NoWaterBillingAuthorization;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        uint usercardSec = (uint)License.SystemInfo.WaterBillingSector;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data1 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        data1[0] = (byte)(balance & 0xFF);
        data1[1] = (byte)((balance >> 8) & 0xFF);
        data1[2] = (byte)((balance >> 16) & 0xFF);

        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        result = _reader.Write((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int WrtJsUserCardAddCount(int balance, string chargeDateTime, uint cardSerno)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasWaterBillingAuth && !License.HasPaymentAuth) return (int)ErrorCode.NoWaterBillingAuthorization;
        if (string.IsNullOrEmpty(chargeDateTime) || chargeDateTime.Length != 12) return (int)ErrorCode.ParameterError;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        uint usercardSec = License.HasWaterBillingAuth 
            ? (uint)License.SystemInfo.WaterBillingSector 
            : (uint)License.SystemInfo.PaymentSector;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data0 = new byte[16];
        byte[] data1 = new byte[16];

        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        data1[0] = (byte)(balance & 0xFF);
        data1[1] = (byte)((balance >> 8) & 0xFF);
        data1[2] = (byte)((balance >> 16) & 0xFF);
        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        int count = data0[8] | (data0[9] << 8);
        count++;
        data0[8] = (byte)(count & 0xFF);
        data0[9] = (byte)((count >> 8) & 0xFF);
        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        result = _reader.Write((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int RstJsUserCard(uint cardSerno, UserCode? userCode = null)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasWaterBillingAuth) return (int)ErrorCode.NoWaterBillingAuthorization;

        userCode ??= new UserCode();

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        uint usercardSec = (uint)License.SystemInfo.WaterBillingSector;

        var sysKey6 = GetSystemKey6(License.SystemInfo.SystemKeyB);
        if (sysKey6 == null) return (int)ErrorCode.ParameterError;
        result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec, sysKey6);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet2, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] emptyData = new byte[16];

        result = _reader.Write((byte)(usercardSec * 4), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 1), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), emptyData);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    #endregion

    #region Balance and Transfer Operations

    public int ReadPosBalance(out uint balanceXf, out uint balanceJs, uint cardSerno, int waitTime)
    {
        balanceXf = 0;
        balanceJs = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        if (License.HasPaymentAuth)
        {
            uint usercardSec = (uint)License.SystemInfo.PaymentSector;

            int cardResult = _reader.Card(0x52, out _);
            if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;
            byte[] password = new byte[16];
            KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

            result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
            if (result == (int)ErrorCode.Success)
            {
                result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
                if (result == (int)ErrorCode.Success)
                {
                    byte[] data1 = new byte[16];
                    result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
                    if (result == (int)ErrorCode.Success)
                    {
                        balanceXf = (uint)(data1[0] | (data1[1] << 8) | (data1[2] << 16));
                    }
                }
            }
        }

        if (License.HasWaterBillingAuth)
        {
            int cardResult = _reader.Card(0x52, out _);
            if (cardResult == (int)ErrorCode.Success)
            {
                uint usercardSec = (uint)License.SystemInfo.WaterBillingSector;
                byte[] password = new byte[16];
                KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

                result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
                if (result == (int)ErrorCode.Success)
                {
                    result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
                    if (result == (int)ErrorCode.Success)
                    {
                        byte[] data1 = new byte[16];
                        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
                        if (result == (int)ErrorCode.Success)
                        {
                            balanceJs = (uint)(data1[0] | (data1[1] << 8) | (data1[2] << 16));
                        }
                    }
                }
            }
        }

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int WrtPosVirement(uint value, uint cardSerno, uint flag, int waitTime)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (value == 0) return (int)ErrorCode.ParameterError;

        int result = ReadCardIdNew(out _, out uint useSnr);
        if (result != (int)ErrorCode.Success) return result;

        if (useSnr != cardSerno) return (int)ErrorCode.UserCardError;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        uint usercardSec;
        if (flag == 1 && License.HasPaymentAuth)
        {
            usercardSec = (uint)License.SystemInfo.PaymentSector;
        }
        else if (flag == 2 && License.HasWaterBillingAuth)
        {
            usercardSec = (uint)License.SystemInfo.WaterBillingSector;
        }
        else
        {
            return (int)ErrorCode.ParameterError;
        }

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data1 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        uint currentBalance = (uint)(data1[0] | (data1[1] << 8) | (data1[2] << 16));
        uint newBalance = currentBalance + value;

        data1[0] = (byte)(newBalance & 0xFF);
        data1[1] = (byte)((newBalance >> 8) & 0xFF);
        data1[2] = (byte)((newBalance >> 16) & 0xFF);
        data1[15] = KeyCalculator.CalculateBcc(data1, 15);

        result = _reader.Write((byte)(usercardSec * 4 + 1), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        result = _reader.Write((byte)(usercardSec * 4 + 2), data1);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    #endregion

    #region User Type and Period Operations

    public int ChangePosUserType12(int userType)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (!License.HasPaymentAuth) return (int)ErrorCode.NoPaymentAuthorization;
        if (userType < 1 || userType > 32) return (int)ErrorCode.ParameterError;

        int result = ReadCardIdNew(out _, out uint cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        uint usercardSec = (uint)License.SystemInfo.PaymentSector;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data0 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        data0[3] = (byte)userType;
        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        result = _reader.Write((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int ChangePosUserType12New(out int oldUserType, int userType, int sysType)
    {
        oldUserType = 0;

        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        if (userType < 1 || userType > 32) return (int)ErrorCode.ParameterError;

        uint usercardSec;
        if (sysType == (int)SystemTypes.Payment && License.HasPaymentAuth)
        {
            usercardSec = (uint)License.SystemInfo.PaymentSector;
        }
        else if (sysType == (int)SystemTypes.WaterBilling && License.HasWaterBillingAuth)
        {
            usercardSec = (uint)License.SystemInfo.WaterBillingSector;
        }
        else if (sysType == (int)SystemTypes.AccessControl && License.HasAccessControlAuth)
        {
            usercardSec = (uint)License.SystemInfo.AccessControlSector;
        }
        else
        {
            return (int)ErrorCode.ParameterError;
        }

        int result = ReadCardIdNew(out _, out uint cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data0 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        oldUserType = data0[3];
        data0[3] = (byte)userType;
        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        result = _reader.Write((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int UpdateUserCardPeriod(uint useTerm, int sysType, int number)
    {
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;

        uint usercardSec;
        if (sysType == (int)SystemTypes.Payment && License.HasPaymentAuth)
        {
            usercardSec = (uint)License.SystemInfo.PaymentSector;
        }
        else if (sysType == (int)SystemTypes.WaterBilling && License.HasWaterBillingAuth)
        {
            usercardSec = (uint)License.SystemInfo.WaterBillingSector;
        }
        else if (sysType == (int)SystemTypes.AccessControl && License.HasAccessControlAuth)
        {
            usercardSec = (uint)License.SystemInfo.AccessControlSector;
        }
        else
        {
            return (int)ErrorCode.ParameterError;
        }

        int result = ReadCardIdNew(out _, out uint cardSerno);
        if (result != (int)ErrorCode.Success) return result;

        int cardResult = _reader.Card(0x52, out _);
        if (cardResult != (int)ErrorCode.Success) return (int)ErrorCode.NoCard;

        byte[] password = new byte[16];
        KeyCalculator.CalculateKey12(BitConverter.GetBytes(cardSerno), License.SystemInfo.OperatorPassword, password);

        result = _reader.LoadKey(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec, password);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReaderError;

        result = _reader.Authentication(KeyTypes.KeyA | KeyTypes.KeySet0, (byte)usercardSec);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.UserCardError;

        byte[] data0 = new byte[16];
        result = _reader.Read((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.ReadCardError;

        uint temp1 = (useTerm / 10000) % 1000;
        uint temp2 = (useTerm % 10000) / 100;
        data0[6] = (byte)((temp1 & 0x0F) << 4 | temp2);
        data0[7] = (byte)((useTerm % 10000) % 100 | ((temp1 & 0xF0) << 1));

        data0[15] = KeyCalculator.CalculateBcc(data0, 15);

        result = _reader.Write((byte)(usercardSec * 4), data0);
        if (result != (int)ErrorCode.Success) return (int)ErrorCode.WriteCardError;

        _reader.Halt();
        return (int)ErrorCode.Success;
    }

    public int WrtUserCardTerm(int sysType, uint useTerm, uint cardSerno, int waitTime)
    {
        return UpdateUserCardPeriod(useTerm, sysType, 0);
    }

    #endregion

    #region System Card Operations

    /// <summary>
    /// Initialize system card with password and sector configuration
    /// </summary>
    /// <param name="userPassword">User password (8 characters)</param>
    /// <param name="sysType">System type (1=Payment, 2=Water, 3=Access)</param>
    /// <param name="useSector">Sector to use (1-31)</param>
    /// <param name="commPassword">Output communication password (8 bytes)</param>
    /// <returns>Error code (0 = success)</returns>
    public int InitSysCard(string userPassword, int sysType, int useSector, out byte[] commPassword)
    {
        commPassword = new byte[8];
        
        if (!_reader.IsOpen) return (int)ErrorCode.CommunicationError;
        
        // Validate parameters
        if (sysType < 1 || sysType > 3 || useSector > 31 || useSector < 1)
            return (int)ErrorCode.ParameterError;
        
        // Get card serial number
        int result = _reader.Card(0, out uint cardSerno);
        if (result != (int)ErrorCode.Success)
        {
            // Retry once
            result = _reader.Card(0, out cardSerno);
            if (result != (int)ErrorCode.Success)
                return (int)ErrorCode.NoCard;
        }
        
        // Load system card keys for sectors 1-4
        for (byte sector = 1; sector <= 4; sector++)
        {
            result = _reader.LoadKey(KeyTypes.KeyB | KeyTypes.KeySet0, sector, KeyCalculator.SystemCardKeyB12);
            if (result != (int)ErrorCode.Success)
                return (int)ErrorCode.ReaderError;
        }
        
        // Generate communication password (random 8-digit number)
        Random random = new Random();
        int commPassNum = random.Next(0, 100000000);
        string commPassStr = commPassNum.ToString("D8");
        commPassword = System.Text.Encoding.ASCII.GetBytes(commPassStr);
        
        // Process password with DES-like transformation
        byte[] password = new byte[8];
        byte[] userPassBytes = System.Text.Encoding.ASCII.GetBytes(userPassword.PadRight(8, ' ').Substring(0, 8));
        KeyCalculator.DesTransform(userPassBytes, password);
        
        // System type specific processing
        switch (sysType)
        {
            case 1: // Payment System (MJXT)
                return InitPaymentSystemCard(useSector, password, commPassword);
                
            case 2: // Water System (SFXT)  
                return InitWaterSystemCard(useSector, password, commPassword);
                
            case 3: // Access Control System (JSXT)
                return InitAccessSystemCard(useSector, password, commPassword);
                
            default:
                return (int)ErrorCode.ParameterError;
        }
    }
    
    private int InitPaymentSystemCard(int useSector, byte[] password, byte[] commPassword)
    {
        const byte MJ_Sec = 1; // Payment system sector
        
        // Authenticate sector
        int result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, MJ_Sec);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.NoPaymentSystemCard;
        
        // Read system card data
        byte[] mjdata = new byte[16];
        result = _reader.Read((byte)(MJ_Sec * 4), mjdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(mjdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Extract system card number
        byte[] sysCardNo = new byte[5];
        Buffer.BlockCopy(mjdata, 1, sysCardNo, 0, 5);
        
        // Read configuration block
        result = _reader.Read((byte)(MJ_Sec * 4 + 1), mjdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(mjdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Setup system info
        var sysInfo = new SystemInfoNew
        {
            CommunicationPassword = commPassword,
            OperatorPassword = password.ToArray(),
            UserPassword = commPassword.ToArray(),
            SystemKeyB = password.ToArray(),
            PaymentSystem = 1,
            PaymentSector = useSector,
            WaterBillingSystem = 0,
            WaterBillingSector = 0,
            AccessControlSystem = 0,
            AccessControlSector = 0
        };
        
        // Store system card number
        Buffer.BlockCopy(sysCardNo, 0, sysInfo.SystemCardNumberPayment, 0, 5);
        
        // XOR operator password with system card number
        for (int i = 0; i < 5; i++)
        {
            sysInfo.OperatorPassword[i] ^= sysCardNo[i];
        }
        
        // Add system card number to system key
        for (int i = 0; i < 5; i++)
        {
            sysInfo.SystemKeyB[i] += sysCardNo[i];
        }
        
        // Prepare write data
        Buffer.BlockCopy(sysInfo.OperatorPassword, 0, mjdata, 0, 6);
        mjdata[6] = (byte)useSector;
        mjdata[7] = 1; // Card type
        
        // Calculate BCC
        mjdata[15] = KeyCalculator.CalculateBcc(mjdata, 15);
        
        // Write configuration
        result = _reader.Write((byte)(MJ_Sec * 4 + 1), mjdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.WriteCardError;
        
        _reader.Halt();
        
        // Create license
        License.SetSystemInfo(sysInfo);
        return (int)ErrorCode.Success;
    }
    
    private int InitWaterSystemCard(int useSector, byte[] password, byte[] commPassword)
    {
        const byte SF_Sec = 2; // Water system sector
        
        // Authenticate sector
        int result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, SF_Sec);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.NoWaterBillingSystemCard;
        
        // Read system card data
        byte[] sfdata = new byte[16];
        result = _reader.Read((byte)(SF_Sec * 4), sfdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(sfdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Extract system card number
        byte[] sysCardNo = new byte[5];
        Buffer.BlockCopy(sfdata, 1, sysCardNo, 0, 5);
        
        // Read configuration block
        result = _reader.Read((byte)(SF_Sec * 4 + 1), sfdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(sfdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Setup system info
        var sysInfo = new SystemInfoNew
        {
            CommunicationPassword = commPassword,
            OperatorPassword = password.ToArray(),
            UserPassword = commPassword.ToArray(),
            SystemKeyB = password.ToArray(),
            PaymentSystem = 0,
            PaymentSector = 0,
            WaterBillingSystem = 1,
            WaterBillingSector = useSector,
            AccessControlSystem = 0,
            AccessControlSector = 0
        };
        
        // Store system card number
        Buffer.BlockCopy(sysCardNo, 0, sysInfo.SystemCardNumberWater, 0, 5);
        
        // XOR operator password with system card number
        for (int i = 0; i < 5; i++)
        {
            sysInfo.OperatorPassword[i] ^= sysCardNo[i];
        }
        
        // Add system card number to system key
        for (int i = 0; i < 5; i++)
        {
            sysInfo.SystemKeyB[i] += sysCardNo[i];
        }
        
        // Prepare and write configuration block 1
        Buffer.BlockCopy(sysInfo.OperatorPassword, 0, sfdata, 0, 6);
        sfdata[6] = (byte)useSector;
        sfdata[7] = 0; // Card type
        sfdata[15] = KeyCalculator.CalculateBcc(sfdata, 15);
        
        result = _reader.Write((byte)(SF_Sec * 4 + 1), sfdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.WriteCardError;
        
        // Write system key block
        Array.Clear(sfdata, 0, 16);
        Buffer.BlockCopy(sysInfo.SystemKeyB, 0, sfdata, 0, 8);
        sfdata[15] = KeyCalculator.CalculateBcc(sfdata, 15);
        
        result = _reader.Write((byte)(SF_Sec * 4 + 2), sfdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.WriteCardError;
        
        _reader.Halt();
        
        // Create license
        License.SetSystemInfo(sysInfo);
        return (int)ErrorCode.Success;
    }
    
    private int InitAccessSystemCard(int useSector, byte[] password, byte[] commPassword)
    {
        const byte JS_Sec = 3; // Access control system sector
        
        // Authenticate sector
        int result = _reader.Authentication(KeyTypes.KeyB | KeyTypes.KeySet0, JS_Sec);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.NoAccessControlSystemCard;
        
        // Read system card data
        byte[] jsdata = new byte[16];
        result = _reader.Read((byte)(JS_Sec * 4), jsdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(jsdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Extract system card number
        byte[] sysCardNo = new byte[5];
        Buffer.BlockCopy(jsdata, 1, sysCardNo, 0, 5);
        
        // Read configuration block
        result = _reader.Read((byte)(JS_Sec * 4 + 1), jsdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.ReadCardError;
        
        if (KeyCalculator.CalculateBcc(jsdata, 16) != 0)
            return (int)ErrorCode.SystemCardError;
        
        // Setup system info
        var sysInfo = new SystemInfoNew
        {
            CommunicationPassword = commPassword,
            OperatorPassword = password.ToArray(),
            UserPassword = commPassword.ToArray(),
            SystemKeyB = password.ToArray(),
            PaymentSystem = 0,
            PaymentSector = 0,
            WaterBillingSystem = 0,
            WaterBillingSector = 0,
            AccessControlSystem = 1,
            AccessControlSector = useSector
        };
        
        // Store system card number
        Buffer.BlockCopy(sysCardNo, 0, sysInfo.SystemCardNumberAccess, 0, 5);
        
        // XOR operator password with system card number
        for (int i = 0; i < 5; i++)
        {
            sysInfo.OperatorPassword[i] ^= sysCardNo[i];
        }
        
        // Add system card number to system key
        for (int i = 0; i < 5; i++)
        {
            sysInfo.SystemKeyB[i] += sysCardNo[i];
        }
        
        // Prepare write data
        Buffer.BlockCopy(sysInfo.OperatorPassword, 0, jsdata, 0, 6);
        jsdata[6] = (byte)useSector;
        jsdata[7] = 0; // Card type
        jsdata[15] = KeyCalculator.CalculateBcc(jsdata, 15);
        
        // Write configuration
        result = _reader.Write((byte)(JS_Sec * 4 + 1), jsdata);
        if (result != (int)ErrorCode.Success)
            return (int)ErrorCode.WriteCardError;
        
        _reader.Halt();
        
        // Create license
        License.SetSystemInfo(sysInfo);
        return (int)ErrorCode.Success;
    }

    #endregion

    #region Generic Sector Operations

    /// <summary>
    /// Write data blocks to a specified sector range with a custom authentication key.
    /// </summary>
    /// <param name="key">6-byte authentication key (KeyA or KeyB).</param>
    /// <param name="keyMode">Key mode byte: e.g. KeyA|KeySet0=0x00, KeyB|KeySet2=0x04+0x02=0x06. Use KeyTypes constants.</param>
    /// <param name="sector">Target sector number (0-15).</param>
    /// <param name="startBlock">Starting block offset within sector (0-3). Block 3 is the trailer block (keys + access bits) — write only if intentional.</param>
    /// <param name="endBlock">Ending block offset within sector (0-3). Must be &gt;= startBlock.</param>
    /// <param name="blockData">
    /// Array of 16-byte blocks to write. Length must equal (endBlock - startBlock + 1).
    /// Each element must be exactly 16 bytes.
    /// </param>
    /// <returns>Error code: 0 = success (ErrorCode.Success).</returns>
    public int WriteSectorData(byte[] key, byte keyMode, byte sector, byte startBlock, byte endBlock, byte[][] blockData)
    {
        if (key == null || key.Length != 6)
        {
            LogInit($"WriteSectorData: Invalid key (null or length={key?.Length})");
            return (int)ErrorCode.ParameterError;
        }
        if (startBlock > 3 || endBlock > 3 || startBlock > endBlock)
        {
            LogInit($"WriteSectorData: Invalid block range [{startBlock}-{endBlock}] for sector {sector}");
            return (int)ErrorCode.ParameterError;
        }
        int expectedBlocks = endBlock - startBlock + 1;
        if (blockData == null || blockData.Length != expectedBlocks)
        {
            LogInit($"WriteSectorData: blockData length {blockData?.Length} != expected {expectedBlocks}");
            return (int)ErrorCode.ParameterError;
        }
        for (int i = 0; i < blockData.Length; i++)
        {
            if (blockData[i] == null || blockData[i].Length != 16)
            {
                LogInit($"WriteSectorData: blockData[{i}] length {blockData[i]?.Length} != 16");
                return (int)ErrorCode.ParameterError;
            }
        }

        LogInit($"WriteSectorData: sector={sector}, blocks=[{startBlock}-{endBlock}], keyMode=0x{keyMode:X2}");

        // Reselect card
        int result = _reader.Card(0x52, out _);
        if (result != (int)ErrorCode.Success)
        {
            LogInit($"WriteSectorData: Card select failed, result={result}");
            return (int)ErrorCode.NoCard;
        }

        // Load key into reader
        result = _reader.LoadKey(keyMode, sector, key);
        if (result != (int)ErrorCode.Success)
        {
            LogInit($"WriteSectorData: LoadKey failed, result={result}");
            return result;
        }

        // Authenticate sector
        result = _reader.Authentication(keyMode, sector);
        if (result != (int)ErrorCode.Success)
        {
            LogInit($"WriteSectorData: Authentication failed, result={result}");
            return result;
        }

        // Write each block
        int blockCount = endBlock - startBlock + 1;
        for (int i = 0; i < blockCount; i++)
        {
            byte blockAddr = (byte)(sector * 4 + startBlock + i);
            result = _reader.Write(blockAddr, blockData[i]);
            if (result != (int)ErrorCode.Success)
            {
                LogInit($"WriteSectorData: Write block {blockAddr} (sector {sector}, offset {startBlock + i}) failed, result={result}");
                _reader.Halt();
                return result;
            }
        }

        _reader.Halt();
        LogInit($"WriteSectorData: success, sector={sector}, blocks=[{startBlock}-{endBlock}]");
        return (int)ErrorCode.Success;
    }

    #endregion


    public void Dispose()
    {
        if (_disposed) return;
        _disposed = true;
    }
}

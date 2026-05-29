using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using M30Demo.Core.Native;

namespace M30Demo.Core;

public sealed class YcCardService : IDisposable
{
    static YcCardService()
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
    }

    private static readonly string LogFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "initcard_debug.log");
    private static void Log(string msg)
    {
        try { File.AppendAllText(LogFilePath, $"[{DateTime.Now:HH:mm:ss.fff}] {msg}{Environment.NewLine}"); } catch { }
    }
    private IntPtr _handle = IntPtr.Zero;
    private bool _isDisposed;

    public bool IsOpen => _handle != IntPtr.Zero;
    public int LastErrorCode { get; private set; }
    public string LastErrorMessage => YcCardErrors.GetErrorMessage(LastErrorCode);

    private static byte[] DefaultSecretKey => [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01];

    public bool Open()
    {
        Close();
        
        try
        {
            _handle = YcCardNative.OpenComm(0);
            if (_handle == IntPtr.Zero || _handle.ToInt64() <= 0)
            {
                LastErrorCode = -1;
                return false;
            }
            return true;
        }
        catch
        {
            _handle = IntPtr.Zero;
            LastErrorCode = -1;
            return false;
        }
    }

    public void Close()
    {
        if (_handle != IntPtr.Zero)
        {
            try { YcCardNative.CloseComm(_handle); } catch { }
            _handle = IntPtr.Zero;
        }
    }

    public bool ReadPhysicalCardId(out uint cardId, out int tagType)
    {
        cardId = 0;
        tagType = 0;

        if (!IsOpen)
        {
            LastErrorCode = -2;
            return false;
        }

        int result = YcCardNative.ReadCard_ID(_handle, ref tagType, ref cardId);
        LastErrorCode = result;
        return result == 0;
    }

    public bool ReadCardType(out int sysType, out int cardType, out uint cardSerno)
    {
        sysType = 0;
        cardType = 0;
        cardSerno = 0;

        if (!IsOpen)
        {
            LastErrorCode = -2;
            return false;
        }

        int optId = 0;
        var secretKey = DefaultSecretKey;
        int result = YcCardNative.Query_Card_Type(
            _handle, ref sysType, ref cardType, ref optId, ref cardSerno, 500, ref secretKey[0]);
        
        LastErrorCode = result;
        return result == 0;
    }

    public bool ReadCard(out PosCardInfo info)
    {
        info = new PosCardInfo();

        if (!IsOpen)
        {
            LastErrorCode = -2;
            return false;
        }

        var secretKey = new byte[50];
        int cardType = 0;
        int optNum = 0;
        int cardID = 0;
        var userNo = new StringBuilder(64);
        int userType = 0;
        uint cardserno = 0;
        int mlngChkSum1 = 0;
        int value1 = 0;
        int lastPay1 = 0;
        int count1 = 0;
        int consumeAdd1 = 0;
        int chkSum2 = 0;
        int value2 = 0;
        int lastPay2 = 0;
        int count2 = 0;
        int consumeAdd2 = 0;
        int useTerm = 0;
        int addCount = 0;

        int result = YcCardNative.Query_Pos_UserCard12(
            _handle,
            ref cardType, ref optNum, ref cardID, userNo, ref userType,
            ref cardserno, ref mlngChkSum1, ref value1, ref lastPay1, ref count1,
            ref consumeAdd1, ref chkSum2, ref value2, ref lastPay2, ref count2,
            ref consumeAdd2, ref useTerm, ref addCount,
            500, ref secretKey[0]);

        LastErrorCode = result;

        if (result == 0)
        {
            info = new PosCardInfo
            {
                CardType = cardType,
                OptNum = optNum,
                CardID = cardID,
                UserNo = userNo.ToString(),
                UserType = userType,
                CardSerno = cardserno,
                MlngChkSum1 = mlngChkSum1,
                Value1 = value1,
                LastPay1 = lastPay1,
                Count1 = count1,
                ConsumeAdd1 = consumeAdd1,
                ChkSum2 = chkSum2,
                Value2 = value2,
                LastPay2 = lastPay2,
                Count2 = count2,
                ConsumeAdd2 = consumeAdd2,
                UseTerm = useTerm,
                AddCount = addCount,
                SecretKey = secretKey,
            };

            info.ParseEmbeddedInfo();

            // Also try reading water billing data
            int jsCardType = 0, jsOptNum = 0, jsCardNo = 0, jsCardSerno = 0;
            int jsValue = 0, jsCount = 0, jsUserType = 0;
            var jsUserPwd = new StringBuilder(64);
            var jsSecretKey = new byte[50];
            jsSecretKey[8] = 1;

            int jsResult = YcCardNative.QueryJsCard(
                _handle, ref jsCardType, ref jsOptNum, ref jsCardNo, jsUserPwd,
                ref jsCardSerno, ref jsValue, ref jsCount, ref jsUserType,
                500, ref jsSecretKey[0]);

            if (jsResult == 0 && jsCardNo == cardID)
            {
                info.JsValue = jsValue;
                info.JsCount = jsCount;
            }
        }

        return result == 0;
    }

    public bool InitCard(
        int cardID,
        string employeeId,
        int cardTypeId,
        int useTerm,
        string employeeName,
        string cardTypeName,
        int consumeValue,
        int waterValue,
        bool creditValue,
        int issueType,
        out uint cardSerno)
    {
        cardSerno = 0;

        if (!IsOpen)
        {
            LastErrorCode = -2;
            return false;
        }

            try
            {
                Log("[InitCard] Encoding employeeId...");
                byte[] byte_userno = string.IsNullOrEmpty(employeeId) ? Array.Empty<byte>() : Encoding.ASCII.GetBytes(employeeId);
                Log($"[InitCard] Encoding employeeName (GB2312)...");
                byte[] byte_username = string.IsNullOrEmpty(employeeName) ? Array.Empty<byte>() : Encoding.GetEncoding("GB2312").GetBytes(employeeName);
                Log($"[InitCard] Encoding cardTypeName (GB2312)...");
                byte[] byte_cardtypename = string.IsNullOrEmpty(cardTypeName) ? Array.Empty<byte>() : Encoding.GetEncoding("GB2312").GetBytes(cardTypeName);

                int cardCodeLen = byte_userno.Length;
                int nameLen = byte_username.Length;
                int typeLen = byte_cardtypename.Length;
                Log($"[InitCard] Lengths: empId={cardCodeLen}, name={nameLen}, type={typeLen}, total+12={cardCodeLen + nameLen + typeLen + 12}");

                var secretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
                byte[] newbytes = new byte[59];
                Array.Copy(secretKey, 0, newbytes, 0, secretKey.Length);

                if (cardCodeLen + nameLen + typeLen + 12 <= 50)
                {
                    newbytes[9] = (byte)cardCodeLen;
                    if (cardCodeLen > 0) Array.Copy(byte_userno, 0, newbytes, 10, cardCodeLen);
                    newbytes[cardCodeLen + 10] = (byte)nameLen;
                    if (nameLen > 0) Array.Copy(byte_username, 0, newbytes, cardCodeLen + 11, nameLen);
                    newbytes[cardCodeLen + 11 + nameLen] = (byte)typeLen;
                    if (typeLen > 0) Array.Copy(byte_cardtypename, 0, newbytes, cardCodeLen + nameLen + 12, typeLen);
                }
                else
                {
                    Log("[InitCard] WARNING: payload too large, leaving newbytes mostly empty");
                }

                int result;
                string shortUserCode = employeeId.Length > 5 ? employeeId.Substring(0, 5) : employeeId;
                Log($"[InitCard] shortUserCode={shortUserCode}, issueType={issueType}");

                // Consume card init (issueType 0 or 1)
                if (issueType is 0 or 1)
                {
                    Log($"[InitCard] Calling ActivatePosUserCard12(cardID={cardID}, userType={cardTypeId}, useTerm={useTerm})...");
                    result = YcCardNative.ActivatePosUserCard12(
                        _handle, cardID, 0, 0xAA, 0xCC, cardTypeId,
                        ref cardSerno, useTerm, ref newbytes[0]);
                    Log($"[InitCard] ActivatePosUserCard12 returned result={result}, cardSerno={cardSerno}");

                    if (result != 0)
                    {
                        LastErrorCode = result;
                        Log($"[InitCard] ActivatePosUserCard12 FAILED, returning false");
                        return false;
                    }

                    if (creditValue && consumeValue != 0)
                    {
                        Log($"[InitCard] Calling WRT_Pos_UserCard_AddCount12(value={consumeValue}, cardSerno={cardSerno})...");
                        result = YcCardNative.WRT_Pos_UserCard_AddCount12(
                            _handle, consumeValue, cardSerno, 500);
                        Log($"[InitCard] WRT_Pos_UserCard_AddCount12 returned result={result}");
                        if (result != 0)
                        {
                            YcCardNative.RST_Pos_UserCard12(_handle, cardSerno, 500, ref secretKey[0]);
                            LastErrorCode = result;
                            Log($"[InitCard] WRT_Pos_UserCard_AddCount12 FAILED, returning false");
                            return false;
                        }
                    }
                }

                // Water card init: skip for issueType=0 since ActivatePosUserCard12 already
                // initialized the sector. Only run for issueType=2 (water-only cards).
                if (issueType == 2)
                {
                    uint waterSerno = 0;
                    Log($"[InitCard] Calling Init_Js_UserCard(cardID={cardID}, shortUserCode={shortUserCode}, userType={cardTypeId})...");
                    result = YcCardNative.Init_Js_UserCard(
                        _handle, cardID, shortUserCode, cardTypeId,
                        ref waterSerno, ref secretKey[0]);
                    Log($"[InitCard] Init_Js_UserCard returned result={result}, waterSerno={waterSerno}");

                    if (result != 0)
                    {
                        LastErrorCode = result;
                        Log($"[InitCard] Init_Js_UserCard FAILED, returning false");
                        if (issueType == 0)
                        {
                            YcCardNative.RST_Pos_UserCard12(_handle, cardSerno, 500, ref secretKey[0]);
                            Log($"[InitCard] Rolled back consume card");
                        }
                        return false;
                    }

                    if (creditValue && waterValue != 0)
                    {
                        var timeStr = DateTime.Now.ToString("yyMMddHHmmss");
                        Log($"[InitCard] Calling WRT_Js_UserCard_AddCount(balance={waterValue}, time={timeStr}, waterSerno={waterSerno})...");
                        result = YcCardNative.WRT_Js_UserCard_AddCount(
                            _handle, waterValue, timeStr, waterSerno);
                        Log($"[InitCard] WRT_Js_UserCard_AddCount returned result={result}");
                        if (result != 0)
                        {
                            if (issueType == 0)
                                YcCardNative.RST_Pos_UserCard12(_handle, cardSerno, 500, ref secretKey[0]);
                            YcCardNative.RSTJsUserCard(_handle, waterSerno, ref secretKey[0]);
                            LastErrorCode = result;
                            Log($"[InitCard] WRT_Js_UserCard_AddCount FAILED, returning false");
                            return false;
                        }
                    }
                }

                Log("[InitCard] Returning true (SUCCESS)");
                return true;
            }
            catch (Exception ex)
            {
                Log($"[InitCard] EXCEPTION: {ex.GetType().Name}: {ex.Message}\n{ex.StackTrace}");
                LastErrorCode = -99;
                return false;
            }
    }

    public bool RecycleCard(uint cardSerno)
    {
        if (!IsOpen)
        {
            LastErrorCode = -2;
            return false;
        }

        var secretKey = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
        int result = YcCardNative.RST_Pos_UserCard12(_handle, cardSerno, 500, ref secretKey[0]);
        LastErrorCode = result;
        return result == 0;
    }

    public void Beep(int ms = 100)
    {
        if (IsOpen)
        {
            try { YcCardNative.rf_beep(_handle, ms); } catch { }
        }
    }

    public void Dispose()
    {
        if (!_isDisposed)
        {
            Close();
            _isDisposed = true;
        }
    }
}

public class PosCardInfo
{
    public int CardType { get; set; }
    public int OptNum { get; set; }
    public int CardID { get; set; }
    public string UserNo { get; set; } = string.Empty;
    public int UserType { get; set; }
    public uint CardSerno { get; set; }
    public int MlngChkSum1 { get; set; }
    /// <summary>Consume balance (in fen/分)</summary>
    public int Value1 { get; set; }
    public int LastPay1 { get; set; }
    public int Count1 { get; set; }
    public int ConsumeAdd1 { get; set; }
    public int ChkSum2 { get; set; }
    /// <summary>Consume backup balance (in fen/分)</summary>
    public int Value2 { get; set; }
    public int LastPay2 { get; set; }
    public int Count2 { get; set; }
    public int ConsumeAdd2 { get; set; }
    /// <summary>Validity as yymmdd packed int</summary>
    public int UseTerm { get; set; }
    public int AddCount { get; set; }
    public byte[] SecretKey { get; set; } = new byte[50];

    /// <summary>Water balance (in fen/分)</summary>
    public int JsValue { get; set; }
    /// <summary>Water usage count</summary>
    public int JsCount { get; set; }

    // Parsed info
    public string EmpId { get; set; } = string.Empty;
    public string EmpName { get; set; } = string.Empty;
    public string CardTypeName { get; set; } = string.Empty;

    public void ParseEmbeddedInfo()
    {
        try
        {
            int startIndex = 9;
            if (SecretKey[9] > SecretKey.Length - startIndex - 1)
            {
                EmpId = Encoding.ASCII.GetString(SecretKey, startIndex + 1, SecretKey.Length - startIndex - 1);
            }
            else
            {
                EmpId = Encoding.ASCII.GetString(SecretKey, startIndex + 1, SecretKey[9]);
            }

            startIndex = startIndex + 1 + SecretKey[9];
            if (startIndex >= SecretKey.Length)
            {
                EmpName = string.Empty;
            }
            else
            {
                EmpName = Encoding.GetEncoding("GB2312").GetString(SecretKey, startIndex + 1, SecretKey[startIndex]);
                if (EmpName.IndexOf('\0') > 0)
                    EmpName = EmpName.Substring(0, EmpName.IndexOf('\0'));

                startIndex = startIndex + 1 + SecretKey[startIndex];
                if (startIndex < SecretKey.Length)
                {
                    CardTypeName = Encoding.GetEncoding("GB2312").GetString(SecretKey, startIndex + 1, SecretKey[startIndex]);
                    if (CardTypeName.IndexOf('\0') > 0)
                        CardTypeName = CardTypeName.Substring(0, CardTypeName.IndexOf('\0'));
                }
            }
        }
        catch
        {
            EmpName = string.Empty;
            CardTypeName = string.Empty;
        }
    }
}

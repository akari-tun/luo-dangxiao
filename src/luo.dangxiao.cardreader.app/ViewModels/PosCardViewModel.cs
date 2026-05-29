using System;
using System.IO;
using System.Text;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;
using luo.dangxiao.cardreader.Yc.Structs;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class PosCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _serno;
    [ObservableProperty] private string _cardNo = string.Empty;
    [ObservableProperty] private int _userType = 1;
    [ObservableProperty] private int _value;
    [ObservableProperty] private int _useCount;
    [ObservableProperty] private uint _useTerm = 20251231;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _optNum;
    [ObservableProperty] private int _addCount;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    [ObservableProperty] private string _empStrId = "U001";
    [ObservableProperty] private string _employeeName = string.Empty;
    [ObservableProperty] private string _cardTypeName = string.Empty;

    [ObservableProperty] private int _queryCardType;
    [ObservableProperty] private int _querySerno;
    [ObservableProperty] private string _queryCardNo = string.Empty;
    [ObservableProperty] private int _queryUserType;
    [ObservableProperty] private int _queryValue;
    [ObservableProperty] private int _queryLastPay;
    [ObservableProperty] private int _queryCount;
    [ObservableProperty] private uint _queryUseTerm;
    [ObservableProperty] private string _queryEmpStrId = string.Empty;
    [ObservableProperty] private string _queryEmployeeName = string.Empty;
    [ObservableProperty] private string _queryCardTypeName = string.Empty;

    public PosCardViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void Query()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            var userCode = new UserCode();
            int result = _main.Consumption.QueryPosUserCard12(
                out int cardType, out int optNum, out int serno, out string cardNo,
                out int userType, out uint cardSerno, out int value, out int lastPay,
                out int count, out uint useTerm, out int addCount, WaitTime, userCode);
            
            QueryCardType = cardType;
            OptNum = optNum;
            QuerySerno = serno;
            QueryCardNo = cardNo;
            QueryUserType = userType;
            CardSerno = cardSerno;
            QueryValue = value;
            QueryLastPay = lastPay;
            QueryCount = count;
            QueryUseTerm = useTerm;
            AddCount = addCount;
            
            if (result == 0)
            {
                ParseUserCode(userCode);
                _main.KeyMode = userCode.Data[8];
            }
            
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Init()
    {
        if (_main.Consumption == null) { ResultMessage = "Consumption not initialized"; return; }
        
        IsBusy = true;
        try
        {
            using var sw = new StreamWriter(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "poscard_init.log"), append: false);
            void LogPosInit(string msg) { var line = $"[{DateTime.Now:HH:mm:ss.fff}] {msg}"; sw.WriteLine(line); }
            
            LogPosInit($"=== Pos Init Start ===");
            LogPosInit($"Params: Serno={Serno}, CardNo='{CardNo}', UserType={UserType}, WaitTime={WaitTime}, UseTerm={UseTerm}");
            LogPosInit($"Auth: HasPayment={_main.Consumption.License.HasPaymentAuth}, Sector={_main.Consumption.License.SystemInfo.PaymentSector}");
            LogPosInit($"License: OperatorPassword={BitConverter.ToString(_main.Consumption.License.SystemInfo.OperatorPassword).Replace("-", "")}");
            LogPosInit($"License: SystemKeyB={BitConverter.ToString(_main.Consumption.License.SystemInfo.SystemKeyB).Replace("-", "")}");
            LogPosInit($"License: UserKeyAB={BitConverter.ToString(_main.Consumption.License.SystemInfo.UserKeyAB).Replace("-", "")}");
            
            if (!_main.Reader?.IsOpen == true) { LogPosInit("FAIL: Reader not open"); ResultMessage = "Reader not open"; return; }
            LogPosInit("Reader: Open OK");
            
            if (!_main.Consumption.License.HasPaymentAuth) { LogPosInit("FAIL: No payment authorization"); ResultMessage = "No payment authorization - load licence first"; return; }
            LogPosInit("Payment auth: OK");
            
            if (Serno <= 0) { LogPosInit($"FAIL: Serno={Serno} must be > 0"); ResultMessage = "Serial number must be > 0"; return; }
            LogPosInit("Validation: OK");
            var userCode = BuildUserCode();
            LogPosInit("Calling InitPosUserCard12...");
            
            sw.Flush();
            
            int result = _main.Consumption.InitPosUserCard12(Serno, CardNo, UserType, WaitTime, out uint cardSerno, UseTerm, userCode);
            CardSerno = cardSerno;
            
            sw.WriteLine("--- CardConsumption Log ---");
            try
            {
                var logPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "poscard_consumption.log");
                if (File.Exists(logPath))
                    sw.WriteLine(File.ReadAllText(logPath));
            } catch { }
            
            LogPosInit($"InitPosUserCard12 returned: result={result}, cardSerno={cardSerno}");
            
            if (result == (int)ErrorCode.CommunicationError) { LogPosInit("→ Reader not open"); }
            else if (result == (int)ErrorCode.NoPaymentAuthorization) { LogPosInit("→ No payment auth in licence"); }
            else if (result == (int)ErrorCode.ParameterError) { LogPosInit("→ Bad parameter (serno/utype/waittime)"); }
            else if (result == (int)ErrorCode.NoCard) { LogPosInit("→ No card on antenna"); }
            else if (result == (int)ErrorCode.ReaderError) { LogPosInit("→ Reader communication error"); }
            else if (result == (int)ErrorCode.UserCardError) { LogPosInit("→ Authentication failed (wrong key or card not selected)"); }
            else if (result == (int)ErrorCode.WriteCardError) { LogPosInit("→ Write to card failed"); }
            else { LogPosInit($"→ Unknown error code: {result}"); }
            
            LogPosInit($"Final message: {MainViewModel.GetErrorMessage(result)}");
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        catch (Exception ex)
        {
            ResultMessage = $"Exception: {ex.Message}";
            try { using var sw = new StreamWriter(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "poscard_init.log"), append: false); sw.WriteLine($"[{DateTime.Now:HH:mm:ss.fff}] EXCEPTION: {ex.Message}\n{ex.StackTrace}"); } catch { }
        }
        finally { IsBusy = false; }
    }

    [RelayCommand]
    private void InitWithBalance()
    {
        if (_main.Consumption == null) { ResultMessage = "Consumption not initialized"; return; }
        
        IsBusy = true;
        try
        {
            using var sw = new StreamWriter(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "poscard_init.log"), append: false);
            void LogPosInitN(string msg) { var line = $"[{DateTime.Now:HH:mm:ss.fff}] {msg}"; sw.WriteLine(line); }
            
            LogPosInitN($"=== Pos+Balance Init Start ===");
            LogPosInitN($"Params: Serno={Serno}, CardNo='{CardNo}', UserType={UserType}, Value={Value}, UseCount={UseCount}, WaitTime={WaitTime}, UseTerm={UseTerm}");
            LogPosInitN($"Auth: HasPayment={_main.Consumption.License.HasPaymentAuth}, Sector={_main.Consumption.License.SystemInfo.PaymentSector}");
            
            if (!_main.Reader?.IsOpen == true) { LogPosInitN("FAIL: Reader not open"); ResultMessage = "Reader not open"; return; }
            if (!_main.Consumption.License.HasPaymentAuth) { LogPosInitN("FAIL: No payment authorization"); ResultMessage = "No payment authorization - load licence first"; return; }
            LogPosInitN("Pre-checks: OK");
            
            var userCode = BuildUserCode();
            LogPosInitN("Calling InitPosUserCardN12...");
            int result = _main.Consumption.InitPosUserCardN12(Serno, CardNo, UserType, Value, UseCount, WaitTime, out uint cardSerno, UseTerm, userCode);
            CardSerno = cardSerno;
            
            LogPosInitN($"InitPosUserCardN12 returned: result={result}, cardSerno={cardSerno}");
            LogPosInitN($"Message: {MainViewModel.GetErrorMessage(result)}");
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        catch (Exception ex)
        {
            ResultMessage = $"Exception: {ex.Message}";
            try { using var sw = new StreamWriter(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "poscard_init.log"), append: false); sw.WriteLine($"[{DateTime.Now:HH:mm:ss.fff}] EXCEPTION: {ex.Message}\n{ex.StackTrace}"); } catch { }
        }
        finally { IsBusy = false; }
    }

    private void ParseUserCode(UserCode userCode)
    {
        var data = userCode.Data;
        int offset = 0;

        if (offset < data.Length)
        {
            int empIdLen = data[offset++];
            if (offset + empIdLen <= data.Length && empIdLen > 0)
            {
                QueryEmpStrId = Encoding.ASCII.GetString(data, offset, empIdLen).TrimEnd('\0');
                offset += empIdLen;
            }
        }

        if (offset < data.Length)
        {
            int nameLen = data[offset++];
            if (offset + nameLen <= data.Length && nameLen > 0)
            {
                QueryEmployeeName = Encoding.GetEncoding("GB2312").GetString(data, offset, nameLen);
                offset += nameLen;
            }
        }

        if (offset < data.Length)
        {
            int typeLen = data[offset++];
            if (offset + typeLen <= data.Length && typeLen > 0)
            {
                QueryCardTypeName = Encoding.GetEncoding("GB2312").GetString(data, offset, typeLen);
            }
        }
    }

    private UserCode BuildUserCode()
    {
        var userCode = new UserCode();
        using var ms = new MemoryStream(userCode.Data);
        using var bw = new BinaryWriter(ms);

        byte[] empIdBytes = Encoding.ASCII.GetBytes(EmpStrId.Length > 5 ? EmpStrId.Substring(0, 5) : EmpStrId);
        byte[] empNameBytes = Encoding.GetEncoding("GB2312").GetBytes(EmployeeName);
        byte[] typeNameBytes = Encoding.GetEncoding("GB2312").GetBytes(CardTypeName);

        bw.Write((byte)empIdBytes.Length);
        bw.Write(empIdBytes);
        bw.Write((byte)empNameBytes.Length);
        bw.Write(empNameBytes);
        bw.Write((byte)typeNameBytes.Length);
        bw.Write(typeNameBytes);
        userCode.Data[8] = (byte)_main.KeyMode;

        return userCode;
    }

    [RelayCommand]
    private void WriteValue()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtPosUserCard12(Value, CardSerno, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteValueAddCount()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtPosUserCardAddCount12(Value, CardSerno, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Reset()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.RstPosUserCard12(CardSerno, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void InitOptCard()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitPosOptCard12(OptNum, WaitTime, out uint cardSerno);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void ResetOptCard()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.RstPosOptCard12(CardSerno, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

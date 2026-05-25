using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;
using luo.dangxiao.cardreader.Yc.Structs;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class MainViewModel : ViewModelBase
{
    private CardReader? _reader;
    private CardConsumption? _consumption;
    private bool _isConnected;
    private readonly StringBuilder _logBuilder = new();

    [ObservableProperty] private bool _isConnected_;
    [ObservableProperty] private string _statusMessage = "Disconnected";
    [ObservableProperty] private int _selectedTabIndex;
    [ObservableProperty] private string _logMessage = string.Empty;
    [ObservableProperty] private int _logEntryCount;

    public MainViewModel()
    {
        CardIdViewModel = new CardIdViewModel(this);
        QueryCardViewModel = new QueryCardViewModel(this);
        PosCardViewModel = new PosCardViewModel(this);
        JsCardViewModel = new JsCardViewModel(this);
        BalanceViewModel = new BalanceViewModel(this);
        UserTypeViewModel = new UserTypeViewModel(this);
        LowLevelViewModel = new LowLevelViewModel(this);
        LicenseViewModel = new LicenseViewModel(LowLevelViewModel);
        SysCardViewModel = new SysCardViewModel(this);

        // Subscribe to reader library logs
        ReaderLogger.LogMessageWritten += OnReaderLogMessage;
    }

    private void OnReaderLogMessage(object? sender, ReaderLogEventArgs e)
    {
        AppendLog(e.Message);
    }

    public CardIdViewModel CardIdViewModel { get; }
    public QueryCardViewModel QueryCardViewModel { get; }
    public PosCardViewModel PosCardViewModel { get; }
    public JsCardViewModel JsCardViewModel { get; }
    public BalanceViewModel BalanceViewModel { get; }
    public UserTypeViewModel UserTypeViewModel { get; }
    public LicenseViewModel LicenseViewModel { get; }
    public LowLevelViewModel LowLevelViewModel { get; }
    public SysCardViewModel SysCardViewModel { get; }

    public bool IsConnected
    {
        get => _isConnected;
        private set
        {
            SetProperty(ref _isConnected, value);
            IsConnected_ = value;
        }
    }

    public void AppendLog(string message)
    {
        _logBuilder.AppendLine($"[{DateTime.Now:HH:mm:ss.fff}] {message}");
        LogMessage = _logBuilder.ToString();
        LogEntryCount++;
    }

    [RelayCommand]
    private void ClearLog()
    {
        _logBuilder.Clear();
        LogMessage = string.Empty;
        LogEntryCount = 0;
    }

    [RelayCommand]
    private void Connect()
    {
        try
        {
            _reader = new CardReader();
            if (_reader.Open())
            {
                _consumption = new CardConsumption(_reader);
                IsConnected = true;
                StatusMessage = "Connected to device";
                AppendLog("Connected to device");
            }
            else
            {
                StatusMessage = "Failed to connect to device";
                AppendLog("ERROR: Failed to connect to device");
            }
        }
        catch (Exception ex)
        {
            StatusMessage = $"Error: {ex.Message}";
            AppendLog($"ERROR: {ex.Message}");
        }
    }

    [RelayCommand]
    private void Disconnect()
    {
        try
        {
            _consumption?.Dispose();
            _reader?.Dispose();
            _consumption = null;
            _reader = null;
            IsConnected = false;
            StatusMessage = "Disconnected";
            AppendLog("Disconnected from device");
        }
        catch (Exception ex)
        {
            StatusMessage = $"Error: {ex.Message}";
            AppendLog($"ERROR: {ex.Message}");
        }
    }

    public CardReader? Reader => _reader;
    public CardConsumption? Consumption => _consumption;

    public static string GetErrorMessage(int errorCode)
    {
        return ((ErrorCode)errorCode) switch
        {
            ErrorCode.Success => "Success",
            ErrorCode.CommunicationError => "Communication error",
            ErrorCode.ReaderError => "Reader error",
            ErrorCode.ParameterError => "Parameter error",
            ErrorCode.TimeoutError => "Timeout",
            ErrorCode.NoCard => "No card present",
            ErrorCode.ReadCardError => "Card read error",
            ErrorCode.WriteCardError => "Card write error",
            ErrorCode.UserCardError => "User card error",
            ErrorCode.NoPaymentAuthorization => "No payment system authorization",
            ErrorCode.NoWaterBillingAuthorization => "No water billing authorization",
            ErrorCode.NoAccessControlAuthorization => "No access control authorization",
            ErrorCode.NotIdentified => "Card not identified",
            _ => errorCode switch
            {
                1 => "Communication failed",
                2 => "Authentication failed — key mismatch (card sector key does not match)",
                0x03 => "Card not selected",
                0x10 => "Card type mismatch",
                0x20 => "RF field error",
                0x60 => "Card lost from antenna",
                0x82 => "Load key failed",
                0x83 => "Authentication rejected (wrong key or access denied)",
                0x88 => "Card lost during operation (136 — card removed or out of range)",
                0xFF => "Command not supported by device",
                _ => $"Device error code: {errorCode} (0x{errorCode:X2})"
            }
        };
    }
}

public partial class CardIdViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private ushort _tagType;
    [ObservableProperty] private uint _serialNumber;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public CardIdViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ReadCardId()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadCardId(out ushort tagType, out uint serialNumber);
            TagType = tagType;
            SerialNumber = serialNumber;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void ReadCardIdNew()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadCardIdNew(out ushort tagType, out uint serialNumber);
            TagType = tagType;
            SerialNumber = serialNumber;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Beep()
    {
        if (_main.Reader == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Reader.Beep(10);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

public partial class QueryCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _sysType;
    [ObservableProperty] private int _cardType;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _optNum;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public QueryCardViewModel(MainViewModel main)
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
            int result = _main.Consumption.QueryCardType(
                out int sysType, out int cardType, out uint cardSerno, out int optNum,
                WaitTime);
            
            SysType = sysType;
            CardType = cardType;
            CardSerno = cardSerno;
            OptNum = optNum;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

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

    [ObservableProperty] private int _queryCardType;
    [ObservableProperty] private int _querySerno;
    [ObservableProperty] private string _queryCardNo = string.Empty;
    [ObservableProperty] private int _queryUserType;
    [ObservableProperty] private int _queryValue;
    [ObservableProperty] private int _queryLastPay;
    [ObservableProperty] private int _queryCount;
    [ObservableProperty] private uint _queryUseTerm;

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
            int result = _main.Consumption.QueryPosUserCard12(
                out int cardType, out int optNum, out int serno, out string cardNo,
                out int userType, out uint cardSerno, out int value, out int lastPay,
                out int count, out uint useTerm, out int addCount, WaitTime);
            
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
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitPosUserCard12(
                Serno, CardNo, UserType, WaitTime, out uint cardSerno, UseTerm);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void InitWithBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitPosUserCardN12(
                Serno, CardNo, UserType, Value, UseCount, WaitTime, out uint cardSerno, UseTerm);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
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

public partial class JsCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _serno;
    [ObservableProperty] private string _cardNo = string.Empty;
    [ObservableProperty] private int _userType = 1;
    [ObservableProperty] private int _balance;
    [ObservableProperty] private int _chargeTimes;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _chargeDateTime = "251231120000";
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    [ObservableProperty] private int _queryCardType;
    [ObservableProperty] private int _queryOptNum;
    [ObservableProperty] private int _querySerno;
    [ObservableProperty] private string _queryCardNo = string.Empty;
    [ObservableProperty] private int _queryValue;
    [ObservableProperty] private int _queryCount;
    [ObservableProperty] private int _queryUserType;

    public JsCardViewModel(MainViewModel main)
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
            int result = _main.Consumption.QueryJsCard(
                out int cardType, out int optNum, out int serno, out string cardNo,
                out uint cardSerno, out int value, out int count, out int userType, WaitTime);
            
            QueryCardType = cardType;
            QueryOptNum = optNum;
            QuerySerno = serno;
            QueryCardNo = cardNo;
            CardSerno = cardSerno;
            QueryValue = value;
            QueryCount = count;
            QueryUserType = userType;
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
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitJsUserCard(
                Serno, CardNo, UserType, out uint cardSerno);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void InitWithBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitJsUserCardN(
                Serno, CardNo, UserType, Balance, ChargeTimes, out uint cardSerno);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtJsUserCard(Balance, CardSerno);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteBalanceAddCount()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtJsUserCardAddCount(Balance, ChargeDateTime, CardSerno);
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
            int result = _main.Consumption.RstJsUserCard(CardSerno);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

public partial class BalanceViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private uint _balanceXf;
    [ObservableProperty] private uint _balanceJs;
    [ObservableProperty] private uint _transferValue;
    [ObservableProperty] private uint _transferFlag = 1;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public BalanceViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ReadBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadPosBalance(
                out uint balanceXf, out uint balanceJs, CardSerno, WaitTime);
            BalanceXf = balanceXf;
            BalanceJs = balanceJs;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Transfer()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtPosVirement(TransferValue, CardSerno, TransferFlag, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

public partial class UserTypeViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _userType = 1;
    [ObservableProperty] private int _sysType = 2;
    [ObservableProperty] private uint _useTerm = 20251231;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _oldUserType;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public UserTypeViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ChangeUserType()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ChangePosUserType12(UserType);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void ChangeUserTypeNew()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ChangePosUserType12New(out int oldUserType, UserType, SysType);
            OldUserType = oldUserType;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void UpdatePeriod()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.UpdateUserCardPeriod(UseTerm, SysType, 0);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteTerm()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtUserCardTerm(SysType, UseTerm, CardSerno, 500);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

public partial class LicenseViewModel : ViewModelBase
{
    private readonly LowLevelViewModel _lowLevel;

    [ObservableProperty] private bool _hasPaymentAuth;
    [ObservableProperty] private bool _hasWaterBillingAuth;
    [ObservableProperty] private bool _hasAccessControlAuth;
    [ObservableProperty] private int _paymentSector = 1;
    [ObservableProperty] private int _waterBillingSector = 3;
    [ObservableProperty] private int _accessControlSector = 2;
    [ObservableProperty] private string _operatorPassword = "12345678";
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private string _licenceFilePath = string.Empty;
    [ObservableProperty] private string _userKeyAHex = string.Empty;
    [ObservableProperty] private string _userKeyBHex = string.Empty;
    [ObservableProperty] private string _systemKeyBHex = string.Empty;

    public LicenseViewModel(LowLevelViewModel lowLevel)
    {
        _lowLevel = lowLevel;
        RefreshStatus();
    }

    public void RefreshStatus()
    {
        var license = LicenseManager.Instance;
        HasPaymentAuth = license.HasPaymentAuth;
        HasWaterBillingAuth = license.HasWaterBillingAuth;
        HasAccessControlAuth = license.HasAccessControlAuth;

        var sysInfo = license.SystemInfo;
        // KeyA = OperatorPassword[0:6]
        if (sysInfo.OperatorPassword != null && sysInfo.OperatorPassword.Length >= 6)
        {
            UserKeyAHex = BitConverter.ToString(sysInfo.OperatorPassword, 0, 6).Replace("-", "");
        }
        // KeyB = SystemKeyB[0:6]
        if (sysInfo.SystemKeyB != null && sysInfo.SystemKeyB.Length >= 6)
        {
            UserKeyBHex = BitConverter.ToString(sysInfo.SystemKeyB, 0, 6).Replace("-", "");
            SystemKeyBHex = UserKeyBHex;
        }

        if (_lowLevel != null) _lowLevel.RefreshKeysFromLicense();
    }

    [RelayCommand]
    private void Configure()
    {
        try
        {
            var sysInfo = new SystemInfoNew
            {
                PaymentSystem = HasPaymentAuth ? 1 : 0,
                PaymentSector = PaymentSector,
                WaterBillingSystem = HasWaterBillingAuth ? 1 : 0,
                WaterBillingSector = WaterBillingSector,
                AccessControlSystem = HasAccessControlAuth ? 1 : 0,
                AccessControlSector = AccessControlSector
            };

            //var password = System.Text.Encoding.ASCII.GetBytes(OperatorPassword.PadRight(8, ' '));
            //var keyB = new byte[8];
            //Buffer.BlockCopy(password, 0, keyB, 0, Math.Min(8, password.Length));

            //sysInfo.OperatorPassword = new byte[8];
            //sysInfo.SystemKeyB = keyB;

            //LicenseManager.Instance.SetSystemInfo(sysInfo);
            ResultMessage = "License configured successfully";
            RefreshStatus();
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error: {ex.Message}";
        }
    }

    [RelayCommand]
    private void ClearAuth()
    {
        LicenseManager.Instance.ClearAuth();
        RefreshStatus();
        ResultMessage = "Authorization cleared";
    }

    [RelayCommand]
    private async Task BrowseLicenceFile()
    {
        try
        {
            // Get the main window to access the storage provider
            // Avalonia will use the appropriate native file picker for each platform
            var mainWindow = TopLevel.GetTopLevel(
                Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop
                    ? desktop.MainWindow
                    : null);

            if (mainWindow?.StorageProvider == null)
            {
                // Fallback for when storage provider is not available
                ResultMessage = "Storage provider not available";
                return;
            }

            // Configure file picker options
            var options = new FilePickerOpenOptions
            {
                Title = "Select Licence File",
                AllowMultiple = false,
                FileTypeFilter = new List<FilePickerFileType>
                {
                    new FilePickerFileType("Licence Files")
                    {
                        Patterns = new[] { "*.dat" },
                        MimeTypes = new[] { "application/octet-stream" }
                    },
                    new FilePickerFileType("All Files")
                    {
                        Patterns = new[] { "*" },
                        MimeTypes = new[] { "*/*" }
                    }
                }
            };

            // Open the file picker
            var files = await mainWindow.StorageProvider.OpenFilePickerAsync(options);

            if (files.Count > 0)
            {
                // Get the local path from the selected file
                LicenceFilePath = files[0].TryGetLocalPath() ?? files[0].Path.LocalPath;
            }
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error browsing file: {ex.Message}";
        }
    }

    [RelayCommand]
    private void LoadLicence()
    {
        try
        {
            if (string.IsNullOrEmpty(LicenceFilePath))
            {
                ResultMessage = "Please select a licence file";
                return;
            }

            if (!File.Exists(LicenceFilePath))
            {
                ResultMessage = "Licence file not found";
                return;
            }

            int result = LicenseManager.Instance.LoadLicence(LicenceFilePath);
            
            if (result == 0)
            {
                ResultMessage = "Licence loaded successfully";
                RefreshStatus();
            }
            else
            {
                ResultMessage = $"Failed to load licence (Error code: {result})";
            }
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error loading licence: {ex.Message}";
        }
    }
}

public partial class LowLevelViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private byte _sector = 1;
    [ObservableProperty] private byte _keyMode = 0x04;
    [ObservableProperty] private string _keyAInput = "FFFFFFFFFFFF";
    [ObservableProperty] private string _keyBInput = "FFFFFFFFFFFF";
    [Obsolete("Use KeyAInput or KeyBInput instead")]
    public string Key { get => KeyAInput; set => KeyAInput = value; }
    [ObservableProperty] private string _block0Data = string.Empty;
    [ObservableProperty] private string _block1Data = string.Empty;
    [ObservableProperty] private string _block2Data = string.Empty;
    [ObservableProperty] private string _block3Data = string.Empty;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;
    [ObservableProperty] private ObservableCollection<SectorReadResult> _sectorResults = new();
    [ObservableProperty] private int _selectedSectorTabIndex;

    public LowLevelViewModel(MainViewModel main)
    {
        _main = main;
    }

    public void RefreshKeysFromLicense()
    {
        var license = LicenseManager.Instance;
        var sysInfo = license.SystemInfo;
        if (sysInfo.OperatorPassword != null && sysInfo.OperatorPassword.Length >= 6)
        {
            var keyA = sysInfo.OperatorPassword.AsSpan(0, 6);
            if (!keyA.ToArray().All(b => b == 0))
                KeyAInput = BitConverter.ToString(keyA.ToArray()).Replace("-", "");
        }
        if (sysInfo.SystemKeyB != null && sysInfo.SystemKeyB.Length >= 6)
        {
            var keyB = sysInfo.SystemKeyB.AsSpan(0, 6);
            if (!keyB.ToArray().All(b => b == 0))
                KeyBInput = BitConverter.ToString(keyB.ToArray()).Replace("-", "");
        }
    }

    private byte[]? GetActiveKey()
    {
        var hex = (KeyMode & 0x04) != 0 ? KeyBInput : KeyAInput;
        return HexStringToBytes(hex);
    }

    [RelayCommand]
    private void ReadBlock()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        try
        {
            var keyA = HexStringToBytes(KeyAInput);
            var keyB = HexStringToBytes(KeyBInput);
            if (keyA == null || keyB == null) { ResultMessage = "Invalid key format"; return; }

            _main.Reader.Halt();
            var cardResult = _main.Reader.Card(0x52, out _);
            if (cardResult != 0) { ResultMessage = "No card detected"; return; }

            byte slot = (byte)(KeyMode & 0x03);
            byte useKeyMode = (byte)(slot | (KeyMode & 0x04));
            byte[] useKey = (KeyMode & 0x04) != 0 ? keyB : keyA;

            var loadResult = _main.Reader.LoadKey((byte)(useKeyMode | KeyTypes.KeySet2), Sector, useKey);
            if (loadResult != 0)
            {
                ResultMessage = $"Load key failed: {MainViewModel.GetErrorMessage(loadResult)}";
                return;
            }

            var authResult = _main.Reader.Authentication((byte)(useKeyMode | KeyTypes.KeySet2), Sector);
            if (authResult != 0)
            {
                ResultMessage = $"Authentication failed: {MainViewModel.GetErrorMessage(authResult)}";
                return;
            }

            var blockResults = new string[4];
            bool allOk = true;
            for (int bo = 0; bo < 4; bo++)
            {
                byte addr = (byte)(Sector * 4 + bo);
                byte[] data = new byte[16];
                int rr = _main.Reader.Read(addr, data);
                if (rr == 0)
                {
                    blockResults[bo] = BitConverter.ToString(data).Replace("-", " ");
                }
                else
                {
                    blockResults[bo] = $"ERR: {MainViewModel.GetErrorMessage(rr)}";
                    allOk = false;
                }
            }

            Block0Data = blockResults[0];
            Block1Data = blockResults[1];
            Block2Data = blockResults[2];
            Block3Data = blockResults[3];
            ResultMessage = allOk ? $"Sector {Sector} read OK" : $"Sector {Sector} read completed with errors";
        }
        finally { IsBusy = false; }
    }

    [RelayCommand]
    private void WriteBlock()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        try
        {
            var cardResult = _main.Reader.Card(0x52, out _);
            if (cardResult != 0) { ResultMessage = "No card detected"; return; }

            var key = GetActiveKey();
            if (key == null) { ResultMessage = "Invalid key format"; return; }

            var blockInputs = new[] { Block0Data, Block1Data, Block2Data, Block3Data };
            var blockBytes = new byte[4][];
            for (int i = 0; i < 4; i++)
            {
                var data = HexStringToBytes(blockInputs[i]);
                if (data == null || data.Length != 16)
                {
                    ResultMessage = $"Block {i} invalid format (need exactly 16 hex bytes, got {data?.Length ?? 0})";
                    return;
                }
                blockBytes[i] = data;
            }

            byte slot = (byte)(KeyMode & 0x03);
            byte useKeyMode = (byte)(slot | (KeyMode & 0x04));

            var loadResult = _main.Reader.LoadKey(useKeyMode, Sector, key);
            if (loadResult != 0) { ResultMessage = $"Load key failed: {MainViewModel.GetErrorMessage(loadResult)}"; return; }
            var authResult = _main.Reader.Authentication(useKeyMode, Sector);
            if (authResult != 0) { ResultMessage = $"Authentication failed: {MainViewModel.GetErrorMessage(authResult)}"; return; }

            bool allOk = true;
            for (int i = 0; i < 4; i++)
            {
                byte addr = (byte)(Sector * 4 + i);
                int wr = _main.Reader.Write(addr, blockBytes[i]);
                if (wr != 0)
                {
                    ResultMessage = $"Block {i} write failed: {MainViewModel.GetErrorMessage(wr)}";
                    allOk = false;
                    break;
                }
            }
            ResultMessage = allOk ? $"Sector {Sector} write OK" : $"Sector {Sector} write completed with errors";
        }
        finally { IsBusy = false; }
    }

    [RelayCommand]
    private async Task ReadAllSectorsAsync()
    {
        if (_main.Reader == null) return;
        IsBusy = true;
        SectorResults.Clear();
        SelectedSectorTabIndex = 0;
        var keyA = HexStringToBytes(KeyAInput);
        var keyB = HexStringToBytes(KeyBInput);
        if (keyA == null || keyB == null) { ResultMessage = "Invalid key format"; IsBusy = false; return; }

        var cardResult = _main.Reader.Card(0x52, out _);
        if (cardResult != 0) { ResultMessage = $"Card detect failed: {MainViewModel.GetErrorMessage(cardResult)}"; IsBusy = false; return; }

        ResultMessage = "Card detected. Reading block 0...";

        var block0 = new byte[16];
        var block0Result = _main.Reader.Read(0, block0);

        byte slot = (byte)(KeyMode & 0x03);
        byte useKeyMode = (byte)(slot | KeyMode & 0x04);
        byte[] useKey = (KeyMode & 0x04) != 0 ? keyB : keyA;

        try
        {
            _main.Reader.Halt();
            _main.Reader.Card(0x52, out _);

            for (byte s = 0; s < 16; s++)
            {
                var sr = new SectorReadResult { Sector = s };
                string st;

                if (s == 0)
                {
                    sr.Block0Result = block0Result == 0
                        ? BitConverter.ToString(block0).Replace("-", " ")
                        : $"ERR: {MainViewModel.GetErrorMessage(block0Result)}";
                }

                var loadResult = _main.Reader.LoadKey(useKeyMode, s, useKey);
                if (loadResult != 0)
                {
                    st = $"Load key ERR: {MainViewModel.GetErrorMessage(loadResult)}";
                    sr.Status = st;
                    if (s > 0) sr.Block0Result = st;
                    sr.Block1Result = st; sr.Block2Result = st; sr.Block3Result = st;
                    SectorResults.Add(sr);
                    _main.Reader.Halt();
                    _main.Reader.Card(0x52, out _);
                    continue;
                }

                var authResult = _main.Reader.Authentication(useKeyMode, s);
                if (authResult != 0)
                {
                    st = $"Auth ERR: {MainViewModel.GetErrorMessage(authResult)}";
                    sr.Status = st;
                    sr.Block1Result = st; sr.Block2Result = st; sr.Block3Result = st;
                    SectorResults.Add(sr);
                    _main.Reader.Halt();
                    _main.Reader.Card(0x52, out _);
                    continue;
                }

                for (int bo = 0; bo < 4; bo++)
                {
                    byte ba = (byte)(s * 4 + bo);
                    byte[] d = new byte[16];
                    int rr = _main.Reader.Read(ba, d);
                    string hd = rr == 0
                        ? BitConverter.ToString(d).Replace("-", " ")
                        : $"ERR: {MainViewModel.GetErrorMessage(rr)}";
                    switch (bo) { case 0: sr.Block0Result = hd; break; case 1: sr.Block1Result = hd; break; case 2: sr.Block2Result = hd; break; case 3: sr.Block3Result = hd; break; }
                }
                sr.Status = "OK";
                SectorResults.Add(sr);
            }
            ResultMessage = "All 16 sectors read completed";
        }
        catch (Exception ex) { ResultMessage = $"Error: {ex.Message}"; }
        finally { _main.Reader.Halt(); IsBusy = false; }
    }

    private static byte[]? HexStringToBytes(string hex)
    {
        try
        {
            hex = hex.Replace(" ", "").Replace("-", "");
            if (hex.Length % 2 != 0) return null;
            byte[] bytes = new byte[hex.Length / 2];
            for (int i = 0; i < bytes.Length; i++) { bytes[i] = Convert.ToByte(hex.Substring(i * 2, 2), 16); }
            return bytes;
        }
        catch { return null; }
    }
}

public class SectorReadResult
{
    public int Sector { get; set; }
    public string Status { get; set; } = "PENDING";
    public string Block0Result { get; set; } = string.Empty;
    public string Block1Result { get; set; } = string.Empty;
    public string Block2Result { get; set; } = string.Empty;
    public string Block3Result { get; set; } = string.Empty;
}

public partial class SysCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private string _userPassword = "password";
    [ObservableProperty] private int _systemTypeIndex = 0;
    [ObservableProperty] private int _useSector = 5;
    [ObservableProperty] private string _commPassword = string.Empty;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public SysCardViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void InitSysCard()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int sysType = SystemTypeIndex + 1; // Convert 0-based index to 1-based type
            
            int result = _main.Consumption.InitSysCard(UserPassword, sysType, UseSector, out byte[] commPassword);
            
            CommPassword = BitConverter.ToString(commPassword).Replace("-", "");
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}
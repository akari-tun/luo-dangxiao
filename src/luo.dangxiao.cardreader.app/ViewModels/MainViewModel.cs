using System;
using System.Text;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;

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
    [ObservableProperty] private int _keyMode = 1;

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

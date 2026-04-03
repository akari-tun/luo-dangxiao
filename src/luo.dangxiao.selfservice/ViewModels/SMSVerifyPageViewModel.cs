using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.resources.Languages;
using System.Text.RegularExpressions;
using System.Windows.Input;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// ViewModel for SMS verify module.
/// </summary>
public partial class SMSVerifyPageViewModel : ViewModelBase
{
    private readonly DispatcherTimer _countdownTimer;

    [ObservableProperty]
    private string _phoneNumber = string.Empty;

    [ObservableProperty]
    private string _verificationCode = string.Empty;

    [ObservableProperty]
    private int _countdownSeconds = 0;

    [ObservableProperty]
    private string _errorMessage = string.Empty;

    public SMSVerifyPageViewModel()
    {
        _countdownTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromSeconds(1)
        };
        _countdownTimer.Tick += OnCountdownTick;
    }

    public bool IsPhoneNumberValid => Regex.IsMatch(PhoneNumber, @"^\d{11}$");

    public bool CanSendCode => IsPhoneNumberValid && CountdownSeconds == 0;

    public bool IsCodeSended => CountdownSeconds > 0;

    public bool CanVerify => IsPhoneNumberValid && Regex.IsMatch(VerificationCode, @"^\d{4,6}$");

    partial void OnPhoneNumberChanged(string value)
    {
        if (!string.IsNullOrWhiteSpace(value) && !Regex.IsMatch(value, @"^\d*$"))
        {
            PhoneNumber = new string(value.Where(char.IsDigit).ToArray());
            return;
        }

        if (PhoneNumber.Length > 11)
        {
            PhoneNumber = PhoneNumber[..11];
            return;
        }

        ErrorMessage = string.Empty;
        OnPropertyChanged(nameof(IsPhoneNumberValid));
        OnPropertyChanged(nameof(CanSendCode));
        OnPropertyChanged(nameof(CanVerify));
        OnPropertyChanged(nameof(IsCodeSended));
        SendVerificationCodeCommand.NotifyCanExecuteChanged();
    }

    partial void OnVerificationCodeChanged(string value)
    {
        if (!string.IsNullOrWhiteSpace(value) && !Regex.IsMatch(value, @"^\d*$"))
        {
            VerificationCode = new string(value.Where(char.IsDigit).ToArray());
            return;
        }

        if (VerificationCode.Length > 6)
        {
            VerificationCode = VerificationCode[..6];
            return;
        }

        ErrorMessage = string.Empty;
        OnPropertyChanged(nameof(CanVerify));
        OnPropertyChanged(nameof(IsCodeSended));
        VerifyCodeCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand(CanExecute = nameof(CanSendCode))]
    private void SendVerificationCode()
    {
        CountdownSeconds = 60;
        ErrorMessage = string.Empty;
        _countdownTimer.Start();
        SendVerificationCodeCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand(CanExecute = nameof(CanVerify))]
    private void OnVerifyCode()
    {
        ErrorMessage = string.Empty;

        if (VerificationCode is "1234" or "123456")
        {
            VerificationCode = string.Empty;
            CountdownSeconds = 0;
            _countdownTimer.Stop();
            OnPropertyChanged(nameof(CanSendCode));
            SendVerificationCodeCommand.NotifyCanExecuteChanged();
        }
        else
        {
            ErrorMessage = "验证码错误，请重新输入";
            VerificationCode = string.Empty;
        }
    }

    private void OnCountdownTick(object? sender, EventArgs e)
    {
        if (CountdownSeconds > 0)
        {
            CountdownSeconds--;
        }

        if (CountdownSeconds <= 0)
        {
            CountdownSeconds = 0;
            _countdownTimer.Stop();
        }

        OnPropertyChanged(nameof(CanSendCode));
        OnPropertyChanged(nameof(IsCodeSended));
        SendVerificationCodeCommand.NotifyCanExecuteChanged();
    }
}

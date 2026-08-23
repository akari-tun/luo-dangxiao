using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.Mappers;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Dtos.Requests;
using System.Globalization;
using System.Security.Principal;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Windows.Input;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Event args for a successful SMS verification.
/// </summary>
public sealed class SmsVerificationSucceededEventArgs : EventArgs
{
    public SmsVerificationSucceededEventArgs(UserInfoModel userInfo)
    {
        UserInfo = userInfo;
    }

    public UserInfoModel UserInfo { get; }
}

/// <summary>
/// ViewModel for SMS verify module.
/// </summary>
public partial class SMSVerifyPageViewModel : ViewModelBase, IPageViewModel
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

    private string _generatedCode = string.Empty;

    public event EventHandler<SmsVerificationSucceededEventArgs>? VerificationSucceeded;

    public SMSVerifyPageViewModel() : base()
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
    private async Task SendVerificationCodeAsync()
    {
        var yktApiClient = GetYktApiClient();
        if (yktApiClient is null)
        {
            ErrorMessage = "短信服务不可用，请稍后重试";
            return;
        }

        _generatedCode = Random.Shared.Next(100000, 1000000).ToString();
        LogCommand(nameof(SendVerificationCodeAsync), $"Mobile={MaskLogValue(PhoneNumber)}");

        var request = new SendSmsRequestDto
        {
            Mobile = PhoneNumber,
            Code = _generatedCode
        };

        try
        {
            var response = await yktApiClient.SendSmsAsync(request);
            LogApiResponse(nameof(yktApiClient.SendSmsAsync), response);

            if (!IsApiSuccess(response))
            {
                ErrorMessage = FormatApiError(response.Message, "发送失败，请稍后重试");
                _generatedCode = string.Empty;
                return;
            }

            CountdownSeconds = 60;
            ErrorMessage = string.Empty;
            _countdownTimer.Start();
            SendVerificationCodeCommand.NotifyCanExecuteChanged();
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "Send verification SMS failed. Mobile={0}", MaskLogValue(PhoneNumber));
            ErrorMessage = FormatApiError(ex.Message, "发送失败，请稍后重试");
            _generatedCode = string.Empty;
        }
    }

    [RelayCommand(CanExecute = nameof(CanVerify))]
    private async Task OnVerifyCode()
    {
        LogCommand(nameof(OnVerifyCode), $"Mobile={MaskLogValue(PhoneNumber)}");
        ErrorMessage = string.Empty;

        if (!string.IsNullOrEmpty(_generatedCode) && VerificationCode == _generatedCode)
        {
            try
            {
                var userInfo = await GetUserInfoByIdentityAsync(PhoneNumber);

                VerificationCode = string.Empty;
                _generatedCode = string.Empty;
                CountdownSeconds = 0;
                _countdownTimer.Stop();
                OnPropertyChanged(nameof(CanSendCode));
                SendVerificationCodeCommand.NotifyCanExecuteChanged();
                VerificationSucceeded?.Invoke(this, new SmsVerificationSucceededEventArgs(userInfo));
            }
            catch (Exception ex)
            {
                Logger.Error(ex, "SMS verification failed. Mobile={0}", MaskLogValue(PhoneNumber));
                ErrorMessage = FormatApiError(ex.Message, "获取人员信息失败，请稍后重试");
            }
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

    private async Task<UserInfoModel> GetUserInfoByIdentityAsync(string phone)
    {
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        var yktApiClient = GetYktApiClient()
            ?? throw new InvalidOperationException("未配置 YktApi 服务，请检查配置文件中的 YktApiConfig。");
        var mapper = Ioc.Default.GetService<IYktUserInfoMapper>()
            ?? throw new InvalidOperationException("未配置 Ykt 用户映射服务。");

        var encodedPhone = EncodePhoneToBase64(phone);
        Logger.Debug("Querying user information by encoded mobile number. Mobile={0}", MaskLogValue(phone));

        if (cfgData.ServiceType == SelfServiceType.StaffSelfService)
        {
            var response = await yktApiClient.GetTeacherByMobileAsync(encodedPhone);
            LogApiResponse(nameof(yktApiClient.GetTeacherByMobileAsync), response);
            var responseIsSuccessful = EnsureApiSuccess(response.Code, response.Message);
            if (!responseIsSuccessful)
            {
                Logger.Warn("Continuing teacher mobile parsing because the response data is available. Mobile={0}", MaskLogValue(phone));
            }

            return mapper.MapStaff(response.Data, phone);
        }

        var checkInDate = DateTime.Today.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture);
        var traineeResponse = await yktApiClient.GetTraineeByMobileAsync(encodedPhone, checkInDate);
        LogApiResponse(nameof(yktApiClient.GetTraineeByMobileAsync), traineeResponse);
        var traineeResponseIsSuccessful = EnsureApiSuccess(traineeResponse.Code, traineeResponse.Message);
        if (!traineeResponseIsSuccessful)
        {
            Logger.Warn("Continuing trainee mobile parsing because the response data is available. Mobile={0}", MaskLogValue(phone));
        }

        return mapper.MapStudent(traineeResponse.Data, phone);
    }

    private static string EncodePhoneToBase64(string phone)
    {
        var bytes = Encoding.UTF8.GetBytes(phone);
        return Convert.ToBase64String(bytes);
    }
}

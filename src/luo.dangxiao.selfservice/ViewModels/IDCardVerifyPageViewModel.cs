using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.Mappers;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;
using luo.dangxiao.wabapi.Clients;
using Microsoft.Extensions.DependencyInjection;
using System.Globalization;
using System.Text;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Event args for a successful ID card verification.
/// </summary>
public sealed class IDCardVerificationSucceededEventArgs : EventArgs
{
    public IDCardVerificationSucceededEventArgs(UserInfoModel userInfo)
    {
        UserInfo = userInfo;
    }

    public UserInfoModel UserInfo { get; }
}

/// <summary>
/// Verify state for ID card verify page.
/// </summary>
public enum IDCardVerifyState
{
    Waiting,
    Processing,
    Success,
    Failed
}

/// <summary>
/// ViewModel for ID card verify module.
/// </summary>
public partial class IDCardVerifyPageViewModel : ViewModelBase, IPageViewModel
{
    private const string SimulatedIdCardNumber = "110101200007286106";

    public event EventHandler<IDCardVerificationSucceededEventArgs>? VerificationSucceeded;

    [ObservableProperty]
    private IDCardVerifyState _currentState = IDCardVerifyState.Waiting;

    [ObservableProperty]
    private string _statusMessage = "等待读取身份证...";

    [ObservableProperty]
    private string _userName = string.Empty;

    [ObservableProperty]
    private string _errorMessage = string.Empty;

    public bool IsWaiting => CurrentState == IDCardVerifyState.Waiting;

    public bool IsProcessing => CurrentState == IDCardVerifyState.Processing;

    public bool IsSuccess => CurrentState == IDCardVerifyState.Success;

    public bool IsFailed => CurrentState == IDCardVerifyState.Failed;

    partial void OnCurrentStateChanged(IDCardVerifyState value)
    {
        OnPropertyChanged(nameof(IsWaiting));
        OnPropertyChanged(nameof(IsProcessing));
        OnPropertyChanged(nameof(IsSuccess));
        OnPropertyChanged(nameof(IsFailed));
    }

    [RelayCommand]
    private async Task StartVerificationAsync()
    {
        ErrorMessage = string.Empty;
        UserName = string.Empty;
        CurrentState = IDCardVerifyState.Processing;
        StatusMessage = "正在模拟操作身份证读卡器，请稍候...";

        try
        {
            var identity = await SimulateReadIdCardNumberAsync();
            StatusMessage = $"读卡成功，身份证号 {identity}，正在查询信息...";

            var userInfo = await GetUserInfoByIdentityAsync(identity);

            CurrentState = IDCardVerifyState.Success;
            UserName = userInfo.Name;
            StatusMessage = $"验证成功，欢迎 {userInfo.Name}";
            VerificationSucceeded?.Invoke(this, new IDCardVerificationSucceededEventArgs(userInfo));
        }
        catch (Exception ex)
        {
            CurrentState = IDCardVerifyState.Failed;
            ErrorMessage = ex.Message;
            StatusMessage = "身份证验证失败";
        }
    }

    private static async Task<string> SimulateReadIdCardNumberAsync()
    {
        await Task.Delay(1000);
        return SimulatedIdCardNumber;
    }

    private static async Task<UserInfoModel> GetUserInfoByIdentityAsync(string identity)
    {
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        var yktApiClient = Ioc.Default.GetService<IYktApiClient>()
            ?? throw new InvalidOperationException("未配置 YktApi 服务，请检查配置文件中的 YktApiConfig。");
        var mapper = Ioc.Default.GetService<IYktUserInfoMapper>()
            ?? throw new InvalidOperationException("未配置 Ykt 用户映射服务。");

        var encodedIdentity = EncodeIdentityToBase64(identity);

        if (cfgData.ServiceType == SelfServiceType.StaffSelfService)
        {
            var response = await yktApiClient.GetTeacherByIdentityAsync(encodedIdentity);
            EnsureApiSuccess(response.Code, response.Message);
            return mapper.MapStaff(response.Data, identity);
        }

        var checkInDate = DateTime.Today.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture);
        var traineeResponse = await yktApiClient.GetTraineeByIdentityAsync(encodedIdentity, checkInDate);
        EnsureApiSuccess(traineeResponse.Code, traineeResponse.Message);
        return mapper.MapStudent(traineeResponse.Data, identity);
    }

    private static string EncodeIdentityToBase64(string identity)
    {
        var bytes = Encoding.UTF8.GetBytes(identity);
        return Convert.ToBase64String(bytes);
    }

    private static void EnsureApiSuccess(int? code, string? message)
    {
        if (code is null or 0 or 200)
        {
            return;
        }

        throw new InvalidOperationException(string.IsNullOrWhiteSpace(message)
            ? "接口调用失败。"
            : message);
    }
}

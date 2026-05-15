using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using Avalonia.Threading;
using luo.dangxiao.common.Enums;
using luo.dangxiao.idreader;
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
    private const int PollIntervalMs = 800;
    private const int CardRemovalThreshold = 3;

    private readonly IdReaderBase _idReader;
    private CancellationTokenSource? _pollingCts;
    private bool _isPolling;

    /// <summary>
    /// Initializes a new instance of the <see cref="IDCardVerifyPageViewModel"/> class.
    /// </summary>
    /// <param name="idReader">The configured ID card reader.</param>
    public IDCardVerifyPageViewModel(IdReaderBase idReader)
    {
        _idReader = idReader;
        EnsureLogger();
    }

    public event EventHandler<IDCardVerificationSucceededEventArgs>? VerificationSucceeded;

    public Func<Task<string?>>? RequestTestIdCardNumberAsync { get; set; }

#if DEBUG
    public bool ShowTestReadIdCardButton => true;
#else
    public bool ShowTestReadIdCardButton => false;
#endif

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

    /// <summary>
    /// Verifies an ID card by calling the YktApi to look up the user.
    /// Returns a <see cref="UserInfoModel"/> on success, or <see langword="null"/> on failure.
    /// </summary>
    /// <param name="cardData">The ID card data read from the device.</param>
    /// <returns>The matched user info on success; otherwise, <see langword="null"/>.</returns>
    public async Task<UserInfoModel?> VerifyCardAsync(IdCardData cardData)
    {
        var identity = cardData.IdNumber;
        if (string.IsNullOrEmpty(identity))
        {
            PostToUi(() => ErrorMessage = "身份证号码为空");
            return null;
        }

        try
        {
            var userInfo = await GetUserInfoByIdentityAsync(identity);
            return userInfo;
        }
        catch (Exception ex)
        {
            PostToUi(() => ErrorMessage = ex.Message);
            return null;
        }
    }

    /// <summary>
    /// Starts automatic polling of the ID card reader.
    /// </summary>
    public void StartAutoReadAsync()
    {
        if (_isPolling)
        {
            return;
        }

        CancelAutoRead();

        _isPolling = true;
        _pollingCts = new CancellationTokenSource();
        var token = _pollingCts.Token;
        PostToUi(() =>
        {
            ErrorMessage = string.Empty;
            UserName = string.Empty;
            CurrentState = IDCardVerifyState.Waiting;
            StatusMessage = "等待读取身份证...";
        });

        _ = Task.Run(() => PollReadIdCardsAsync(token), token);
    }

    /// <summary>
    /// Stops automatic polling of the ID card reader.
    /// </summary>
    public void CancelAutoRead()
    {
        if (_pollingCts is not null)
        {
            _pollingCts.Cancel();
            _pollingCts.Dispose();
            _pollingCts = null;
        }

        _isPolling = false;
    }

    partial void OnCurrentStateChanged(IDCardVerifyState value)
    {
        OnPropertyChanged(nameof(IsWaiting));
        OnPropertyChanged(nameof(IsProcessing));
        OnPropertyChanged(nameof(IsSuccess));
        OnPropertyChanged(nameof(IsFailed));
    }

    private async Task PollReadIdCardsAsync(CancellationToken token)
    {
        while (!token.IsCancellationRequested)
        {
            var initialized = false;

            try
            {
                initialized = _idReader.Init();
                if (!initialized)
                {
                    Logger.Warn("ID reader initialization failed during automatic polling.");
                    PostToUi(() =>
                    {
                        CurrentState = IDCardVerifyState.Waiting;
                        StatusMessage = "身份证读卡器初始化失败，请检查设备连接...";
                    });

                    await Task.Delay(1000, token);
                    PostToUi(() =>
                    {
                        if (!_isPolling)
                        {
                            return;
                        }

                        CurrentState = IDCardVerifyState.Waiting;
                        StatusMessage = "等待读取身份证...";
                    });
                    continue;
                }

                PostToUi(() =>
                {
                    CurrentState = IDCardVerifyState.Waiting;
                    StatusMessage = "等待读取身份证...";
                });

                while (!token.IsCancellationRequested)
                {
                    try
                    {
                        if (!_idReader.ReadIdCard(out var cardData))
                        {
                            await Task.Delay(PollIntervalMs, token);
                            continue;
                        }

                        PostToUi(() =>
                        {
                            ErrorMessage = string.Empty;
                            UserName = string.Empty;
                            CurrentState = IDCardVerifyState.Processing;
                            StatusMessage = "读卡成功，正在查询信息...";
                        });

                        var userInfo = await VerifyCardAsync(cardData);
                        if (userInfo is not null)
                        {
                            PostToUi(() =>
                            {
                                CurrentState = IDCardVerifyState.Success;
                                UserName = userInfo.Name;
                                StatusMessage = $"验证成功，欢迎 {userInfo.Name}";
                                VerificationSucceeded?.Invoke(this, new IDCardVerificationSucceededEventArgs(userInfo));
                                CancelAutoRead();
                            });
                            return;
                        }

                        PostToUi(() =>
                        {
                            CurrentState = IDCardVerifyState.Failed;
                            StatusMessage = "身份证验证失败，请移开身份证后重试";
                        });

                        await WaitForCardRemovalAsync(token);
                        if (token.IsCancellationRequested)
                        {
                            return;
                        }

                        PostToUi(() =>
                        {
                            if (!_isPolling)
                            {
                                return;
                            }

                            CurrentState = IDCardVerifyState.Waiting;
                            StatusMessage = "等待读取身份证...";
                            UserName = string.Empty;
                        });
                    }
                    catch (OperationCanceledException) when (token.IsCancellationRequested)
                    {
                        return;
                    }
                    catch (Exception ex)
                    {
                        Logger.Error(ex, "Unhandled exception occurred while polling the ID reader.");
                        PostToUi(() =>
                        {
                            if (!_isPolling)
                            {
                                return;
                            }

                            CurrentState = IDCardVerifyState.Waiting;
                            StatusMessage = "等待读取身份证...";
                        });
                        await Task.Delay(PollIntervalMs, token);
                    }
                }
            }
            catch (OperationCanceledException) when (token.IsCancellationRequested)
            {
                return;
            }
            catch (Exception ex)
            {
                Logger.Error(ex, "Unexpected exception occurred before ID reader polling completed.");
                await Task.Delay(1000, token);
            }
            finally
            {
                try
                {
                    _idReader.Close();
                }
                catch (Exception ex)
                {
                    Logger.Warn(ex, "Failed to close the ID reader after polling iteration.");
                }
            }
        }
    }

    private async Task WaitForCardRemovalAsync(CancellationToken token)
    {
        var consecutiveMisses = 0;

        while (!token.IsCancellationRequested)
        {
            try
            {
                if (_idReader.ReadIdCard(out _))
                {
                    consecutiveMisses = 0;
                }
                else
                {
                    consecutiveMisses++;
                    if (consecutiveMisses >= CardRemovalThreshold)
                    {
                        return;
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Warn(ex, "Error occurred while waiting for ID card removal.");
                consecutiveMisses = 0;
            }

            await Task.Delay(PollIntervalMs, token);
        }
    }

    private static void PostToUi(Action action)
    {
        var dispatcher = Dispatcher.UIThread;
        dispatcher.Post(action);
    }

#if DEBUG
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

            if (string.IsNullOrEmpty(identity))
            {
                StatusMessage = $"读身份证失败，请重新操作！";
                await Task.Delay(2000);
                StatusMessage = "等待读取身份证...";
                return;
            }

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

    private async Task<string> SimulateReadIdCardNumberAsync()
    {
        await Task.Delay(1000);

        if (RequestTestIdCardNumberAsync is not null)
        {
            var inputIdCardNumber = await RequestTestIdCardNumberAsync();
            if (!string.IsNullOrWhiteSpace(inputIdCardNumber))
            {
                return inputIdCardNumber.Trim().ToUpperInvariant();
            }
        }

        return string.Empty;
    }
#endif

    private static async Task<UserInfoModel> GetUserInfoByIdentityAsync(string identity)
    {
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();
        var yktApiClient = GetYktApiClient()
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
}

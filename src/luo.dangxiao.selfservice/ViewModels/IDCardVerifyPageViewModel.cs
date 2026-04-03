using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace luo.dangxiao.selfservice.ViewModels;

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
public partial class IDCardVerifyPageViewModel : ViewModelBase
{
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
        CurrentState = IDCardVerifyState.Processing;
        StatusMessage = "正在处理证件信息，请稍候...";
        await Task.Delay(1200);
        CurrentState = IDCardVerifyState.Success;
        UserName = "张三";
    }
}

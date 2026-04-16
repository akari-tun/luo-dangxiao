using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.models;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Parameter for take card page.
/// </summary>
public sealed class TakeCardPageParameter
{
    public string TargetFunction { get; set; } = "TakeCard";

    public UserInfoModel? Data { get; set; }
}

/// <summary>
/// Take card flow state.
/// </summary>
public enum TakeCardFlowState
{
    Confirm,
    Processing,
    CardReady,
    Completed
}

/// <summary>
/// ViewModel for TakeCardPage.
/// </summary>
public partial class TakeCardPageViewModel : ViewModelBase
{
    [ObservableProperty]
    private TakeCardFlowState _currentState = TakeCardFlowState.Confirm;

    [ObservableProperty]
    private UserInfoModel? _userInfo;

    [ObservableProperty]
    private object? _userInfoModuleContent;

    [ObservableProperty]
    private string _targetFunction = "TakeCard";

    [ObservableProperty]
    private string _pageTitle = LanguageProvider.SelfService_TakeCard_Title_PendingPickup;

    [ObservableProperty]
    private string _cardNumber = string.Empty;

    [ObservableProperty]
    private string _cardStatusText = string.Empty;

    [ObservableProperty]
    private DateTime? _takeCardTime;

    [ObservableProperty]
    private string _statusHint = "确认要领取卡片吗？";

    [ObservableProperty]
    private bool _canTakeCardByStatus;

    [ObservableProperty]
    private bool _isBusy;

    public bool IsConfirmState => CurrentState == TakeCardFlowState.Confirm;

    public bool IsProcessingState => CurrentState == TakeCardFlowState.Processing;

    public bool IsCardReadyState => CurrentState == TakeCardFlowState.CardReady;

    public bool IsCompletedState => CurrentState == TakeCardFlowState.Completed;

    public bool CanGoBack => IsConfirmState;

    public bool CanStartTakeCard => IsConfirmState && CanTakeCardByStatus && !IsBusy;

    public bool ShowTakeCardConfirmButton => CanStartTakeCard;

    public bool CanConfirmPickup => IsCardReadyState && !IsBusy;

    public bool CanComplete => IsCompletedState;

    partial void OnCurrentStateChanged(TakeCardFlowState value)
    {
        StatusHint = value switch
        {
            TakeCardFlowState.Confirm => "确认要领取卡片吗？",
            TakeCardFlowState.Processing => "正在制作卡片，请稍候...",
            TakeCardFlowState.CardReady => "卡片已制作完成，请从出卡口领取。",
            TakeCardFlowState.Completed => "请妥善保管您的卡片。",
            _ => string.Empty
        };

        OnPropertyChanged(nameof(IsConfirmState));
        OnPropertyChanged(nameof(IsProcessingState));
        OnPropertyChanged(nameof(IsCardReadyState));
        OnPropertyChanged(nameof(IsCompletedState));
        OnPropertyChanged(nameof(CanGoBack));
        OnPropertyChanged(nameof(CanStartTakeCard));
        OnPropertyChanged(nameof(ShowTakeCardConfirmButton));
        OnPropertyChanged(nameof(CanConfirmPickup));
        OnPropertyChanged(nameof(CanComplete));

        StartTakeCardCommand.NotifyCanExecuteChanged();
        ConfirmPickupCommand.NotifyCanExecuteChanged();
        CompleteCommand.NotifyCanExecuteChanged();
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(CanStartTakeCard));
        OnPropertyChanged(nameof(ShowTakeCardConfirmButton));
        OnPropertyChanged(nameof(CanConfirmPickup));
        StartTakeCardCommand.NotifyCanExecuteChanged();
        ConfirmPickupCommand.NotifyCanExecuteChanged();
    }

    partial void OnCanTakeCardByStatusChanged(bool value)
    {
        OnPropertyChanged(nameof(CanStartTakeCard));
        OnPropertyChanged(nameof(ShowTakeCardConfirmButton));
        StartTakeCardCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand]
    private void LoadData(TakeCardPageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        TakeCardTime = null;
        CurrentState = TakeCardFlowState.Confirm;

        ResolveCardInfo(parameter.Data);
        ResolveTitleAndActionByStatus(parameter.Data);
        LoadUserInfoModule(parameter.Data);
    }

    [RelayCommand(CanExecute = nameof(CanStartTakeCard))]
    private async Task StartTakeCardAsync()
    {
        IsBusy = true;
        CurrentState = TakeCardFlowState.Processing;
        await Task.Delay(1600);
        CurrentState = TakeCardFlowState.CardReady;
        IsBusy = false;
    }

    [RelayCommand(CanExecute = nameof(CanConfirmPickup))]
    private async Task ConfirmPickupAsync()
    {
        IsBusy = true;
        await Task.Delay(500);
        TakeCardTime = DateTime.Now;
        CurrentState = TakeCardFlowState.Completed;
        IsBusy = false;
    }

    [RelayCommand(CanExecute = nameof(CanComplete))]
    private void Complete()
    {
        Back();
    }

    private void ResolveCardInfo(UserInfoModel? data)
    {
        switch (data)
        {
            case StudentInfoModel student:
                CardNumber = student.CardNumber;
                CardStatusText = student.CardStatus switch
                {
                    StudentCardStatus.PendingPickup => "待领取",
                    StudentCardStatus.Normal => "正常",
                    StudentCardStatus.Lost => "挂失",
                    StudentCardStatus.Unissued => "未制卡",
                    _ => "未知"
                };
                break;
            case StaffInfoModel staff:
                CardNumber = staff.CardNumber;
                CardStatusText = staff.CardStatus switch
                {
                    StaffCardStatus.PendingPickup => "待领取",
                    StaffCardStatus.Normal => "正常",
                    StaffCardStatus.Lost => "挂失",
                    StaffCardStatus.Frozen => "冻结",
                    _ => "未知"
                };
                break;
            default:
                CardNumber = "-";
                CardStatusText = "未知";
                break;
        }
    }

    private void ResolveTitleAndActionByStatus(UserInfoModel? data)
    {
        switch (data)
        {
            case StudentInfoModel { CardStatus: StudentCardStatus.PendingPickup }:
            case StaffInfoModel { CardStatus: StaffCardStatus.PendingPickup }:
                PageTitle = LanguageProvider.SelfService_TakeCard_Title_PendingPickup;
                CanTakeCardByStatus = true;
                break;
            case StudentInfoModel { CardStatus: StudentCardStatus.Lost }:
            case StaffInfoModel { CardStatus: StaffCardStatus.Lost }:
                PageTitle = LanguageProvider.SelfService_TakeCard_Title_Lost;
                CanTakeCardByStatus = false;
                break;
            case StudentInfoModel { CardStatus: StudentCardStatus.Normal }:
            case StaffInfoModel { CardStatus: StaffCardStatus.Normal }:
                PageTitle = LanguageProvider.SelfService_TakeCard_Title_Normal;
                CanTakeCardByStatus = false;
                break;
            default:
                PageTitle = LanguageProvider.SelfService_TakeCard_Title_Other;
                CanTakeCardByStatus = false;
                break;
        }
    }

    private void LoadUserInfoModule(UserInfoModel? data)
    {
        if (data is StudentInfoModel student)
        {
            var moduleView = new StudentInfoPageView();
            if (moduleView.DataContext is StudentInfoPageViewModel vm)
            {
                vm.LoadDataCommand.Execute(new StudentInfoPageParameter
                {
                    Data = student,
                    Mode = StudentInfoDisplayMode.Standard
                });
            }

            UserInfoModuleContent = moduleView;
            return;
        }

        if (data is StaffInfoModel staff)
        {
            var moduleView = new StaffInfoPageView();
            if (moduleView.DataContext is StaffInfoPageViewModel vm)
            {
                vm.LoadDataCommand.Execute(new StaffInfoPageParameter
                {
                    Data = staff,
                    Mode = StaffInfoDisplayMode.Standard
                });
            }

            UserInfoModuleContent = moduleView;
            return;
        }

        UserInfoModuleContent = null;
    }
}

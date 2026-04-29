using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.wabapi.Clients;

namespace luo.dangxiao.selfservice.ViewModels;

public sealed class TakeCardPageParameter
{
    public string TargetFunction { get; set; } = "TakeCard";
    public UserInfoModel? Data { get; set; }
}

public partial class TakeCardPageViewModel : CardOperationViewModelBase
{
    private readonly SelfServiceConfig _config;
    private readonly CardPrinterBase _cardPrinter;
    private readonly IYktApiClient? _yktApiClient;
    private readonly string _printerId;

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

    public bool CanStartTakeCard => IsConfirmState && CanTakeCardByStatus && !IsBusy;
    public bool ShowTakeCardConfirmButton => CanStartTakeCard;

    public bool IsProcessingState => IsCardProcessingState;

    partial void OnCanTakeCardByStatusChanged(bool value)
    {
        OnPropertyChanged(nameof(CanStartTakeCard));
        OnPropertyChanged(nameof(ShowTakeCardConfirmButton));
        StartTakeCardCommand.NotifyCanExecuteChanged();
    }

    private void OnCanStartTakeCardChanged()
    {
        OnPropertyChanged(nameof(CanStartTakeCard));
        OnPropertyChanged(nameof(ShowTakeCardConfirmButton));
    }

    public TakeCardPageViewModel(SelfServiceConfig config, CardPrinterBase cardPrinter, IYktApiClient? yktApiClient = null)
    {
        _config = config;
        _cardPrinter = cardPrinter;
        _yktApiClient = yktApiClient;
        _printerId = config.PrinterConfig.DefaultPrinterId;
    }

    protected override SelfServiceConfig Config => _config;
    protected override CardPrinterBase CardPrinter => _cardPrinter;
    protected override IYktApiClient? YktApiClient => _yktApiClient;
    protected override string PrinterId => _printerId;
    protected override UserInfoModel? UserInfoData => UserInfo;

    protected override void OnStateChanged(CardProcessingState value)
    {
        StatusHint = value switch
        {
            CardProcessingState.Confirm => "确认要领取卡片吗？",
            CardProcessingState.Completed => "请妥善保管您的卡片。",
            CardProcessingState.OperationFailed => "卡片制作失败，请重试。",
            _ => StatusHint
        };
        OnCanStartTakeCardChanged();
        StartTakeCardCommand.NotifyCanExecuteChanged();
        ConfirmPickupCommand.NotifyCanExecuteChanged();
        CompleteCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand(CanExecute = nameof(CanStartTakeCard))]
    private async Task StartTakeCardAsync() => await ExecuteCardProcessAsync("新卡");

    protected override void ConfirmPickup()
    {
        TakeCardTime = DateTime.Now;
        base.ConfirmPickup();
    }

    [RelayCommand]
    private void LoadData(TakeCardPageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        TakeCardTime = null;
        CurrentState = CardProcessingState.Confirm;
        IsBusy = false;

        ResolveCardInfo(parameter.Data);
        ResolveTitleAndActionByStatus(parameter.Data);
        LoadUserInfoModule(parameter.Data);

        StartCountdownTimer();
    }

    protected override void HandleCountdownExpired()
    {
        if (IsConfirmState)
        {
            StopCountdownTimer();
            Back();
        }
        else if (IsCardReadyToPickupState)
        {
            OnPickupTimeoutDiscarded();
        }
        else if (IsCardProcessingState)
        {
            _ = HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
        }
    }

    protected override void SetProcessingTitle() { }

    protected override string GetPickupInstructionText() =>
        LanguageProvider.SelfService_TakeCard_Status_PickupInstruction;

    protected override void OnPickupTimeoutDiscarded() =>
        StatusHint = LanguageProvider.SelfService_TakeCard_Status_Timeout_Discard;

    protected override void OnPickupTimeoutReturned() =>
        StatusHint = LanguageProvider.SelfService_TakeCard_Status_CardReturned;

    private void ResolveCardInfo(UserInfoModel? data)
    {
        var currentCard = data?.CurrentCard;
        CardNumber = currentCard?.CardNo ?? "-";
        CardStatusText = currentCard?.CardStatusName ?? "待取卡";
    }

    private void ResolveTitleAndActionByStatus(UserInfoModel? data)
    {
        var currentCard = data?.CurrentCard;
        if (currentCard is null)
        {
            PageTitle = LanguageProvider.SelfService_TakeCard_Title_PendingPickup;
            CanTakeCardByStatus = true;
            return;
        }
        if (currentCard.CardStatusId == (int)UserCardStatus.Lost)
        {
            PageTitle = LanguageProvider.SelfService_TakeCard_Title_Lost;
            CanTakeCardByStatus = false;
            return;
        }
        if (currentCard.CardStatusId == (int)UserCardStatus.Normal)
        {
            PageTitle = LanguageProvider.SelfService_TakeCard_Title_Normal;
            CanTakeCardByStatus = false;
            return;
        }
        PageTitle = LanguageProvider.SelfService_TakeCard_Title_Other;
        CanTakeCardByStatus = false;
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

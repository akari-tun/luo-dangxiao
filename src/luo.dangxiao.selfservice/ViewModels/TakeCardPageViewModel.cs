using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Requests;
using System.Text.Json;

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
    CardProcessing,
    CardReadyToPickup,
    CardReady,
    Completed,
    OperationFailed
}

/// <summary>
/// ViewModel for TakeCardPage.
/// </summary>
public partial class TakeCardPageViewModel : ViewModelBase
{
    private readonly SelfServiceConfig _config;
    private readonly CardPrinterBase _cardPrinter;
    private readonly IYktApiClient? _yktApiClient;
    private string _printerId;

    private CancellationTokenSource? _countdownCts;
    private Task? _countdownTask;
    private CancellationTokenSource? _operationCts;

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

    [ObservableProperty]
    private int _countdownSeconds;

    [ObservableProperty]
    private string _countdownText = string.Empty;

    [ObservableProperty]
    private bool _isCountdownVisible;

    [ObservableProperty]
    private string _operationStepText = string.Empty;

    [ObservableProperty]
    private string _processingTitleText = "制卡中";

    [ObservableProperty]
    private string _pickupInstructionText = "请从取卡口取走您的卡片";

    public bool IsConfirmState => CurrentState == TakeCardFlowState.Confirm;

    public bool IsCardProcessingState => CurrentState == TakeCardFlowState.CardProcessing;

    public bool IsProcessingState => CurrentState == TakeCardFlowState.CardProcessing;

    public bool IsCardReadyToPickupState => CurrentState == TakeCardFlowState.CardReadyToPickup;

    public bool IsCardReadyState => CurrentState == TakeCardFlowState.CardReady;

    public bool IsCompletedState => CurrentState == TakeCardFlowState.Completed;

    public bool IsOperationFailedState => CurrentState == TakeCardFlowState.OperationFailed;

    public bool CanGoBack => IsConfirmState;

    public bool CanStartTakeCard => IsConfirmState && CanTakeCardByStatus && !IsBusy;

    public bool ShowTakeCardConfirmButton => CanStartTakeCard;

    public bool CanConfirmPickup => IsCardReadyToPickupState && !IsBusy;

    public bool CanComplete => IsCompletedState;

    partial void OnCurrentStateChanged(TakeCardFlowState value)
    {
        StatusHint = value switch
        {
            TakeCardFlowState.Confirm => "确认要领取卡片吗？",
            TakeCardFlowState.CardReady => "卡片已制作完成，请从出卡口领取。",
            TakeCardFlowState.Completed => "请妥善保管您的卡片。",
            TakeCardFlowState.OperationFailed => "卡片制作失败，请重试。",
            _ => string.Empty
        };

        OnPropertyChanged(nameof(IsConfirmState));
        OnPropertyChanged(nameof(IsCardProcessingState));
        OnPropertyChanged(nameof(IsProcessingState));
        OnPropertyChanged(nameof(IsCardReadyToPickupState));
        OnPropertyChanged(nameof(IsCardReadyState));
        OnPropertyChanged(nameof(IsCompletedState));
        OnPropertyChanged(nameof(IsOperationFailedState));
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

    public TakeCardPageViewModel(SelfServiceConfig config, CardPrinterBase cardPrinter, IYktApiClient? yktApiClient = null)
    {
        _config = config;
        _cardPrinter = cardPrinter;
        _yktApiClient = yktApiClient;
        _printerId = config.PrinterConfig.DefaultPrinterId;
    }

    [RelayCommand]
    private void LoadData(TakeCardPageParameter parameter)
    {
        TargetFunction = parameter.TargetFunction;
        UserInfo = parameter.Data;
        TakeCardTime = null;
        CurrentState = TakeCardFlowState.Confirm;
        IsBusy = false;

        ResolveCardInfo(parameter.Data);
        ResolveTitleAndActionByStatus(parameter.Data);
        LoadUserInfoModule(parameter.Data);

        StartCountdownTimer();
    }

    [RelayCommand(CanExecute = nameof(CanStartTakeCard))]
    private async Task StartTakeCardAsync()
    {
        ResetCountdown();
        IsBusy = true;
        CurrentState = TakeCardFlowState.CardProcessing;
        ProcessingTitleText = "制卡中";

        _operationCts = new CancellationTokenSource();
        var opToken = _operationCts.Token;

        try
        {
            // Step 1: Move card to reader position
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_MovingCard;
            if (!await MoveCardToReaderAsync())
            {
                await HandleOperationFailedAsync("Failed to move card to reader position.");
                return;
            }

            // Step 2: Simulate card reading
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_ReadingCard;
            var physicalCardId = await SimulateReadCardAsync();
            if (string.IsNullOrEmpty(physicalCardId))
            {
                await HandleOperationFailedAsync("Failed to read card information.");
                return;
            }

            // Step 3: Initialize card via YktApi
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_InitializingCard;
            var initResult = await InitCardAsync(physicalCardId, opToken);
            if (!initResult.Success)
            {
                await HandleOperationFailedAsync(initResult.ErrorMessage ?? "Card initialization failed.");
                // Move card to reject on init failure
                await DiscardCardToRejectAsync();
                return;
            }

            // Step 4: Simulate card writing
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_WritingCard;
            var writeResult = await SimulateWriteCardAsync(initResult);
            if (!writeResult.Success)
            {
                await WriteCardFailureApi(initResult, writeResult.ErrorMessage);
                await HandleOperationFailedAsync(writeResult.ErrorMessage ?? "Failed to write card data.");
                await DiscardCardToRejectAsync();
                return;
            }

            // Step 5: Print card
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_PrintingCard;
            var printSuccess = await PrintCardAsync(initResult);
            if (!printSuccess)
            {
                await HandleOperationFailedAsync("Failed to print card.");
                await DiscardCardToRejectAsync();
                return;
            }

            // Notify API success
            WriteCardSuccessApi(initResult);

            // Step 6: Move card to front holder
            await _cardPrinter.MoveCardAsync(_printerId, CardMoveCommand.MoveToFront);

            // Step 7: Transition to ready-to-pickup state
            PickupInstructionText = LanguageProvider.SelfService_TakeCard_Status_PickupInstruction;
            CurrentState = TakeCardFlowState.CardReadyToPickup;
            IsCountdownVisible = false;
            IsBusy = false;

            // Start pickup monitoring
            StartPickupMonitor(physicalCardId, initResult);
        }
        catch (OperationCanceledException)
        {
            await HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
        }
        catch (Exception ex)
        {
            await HandleOperationFailedAsync(ex.Message);
        }
    }

    [RelayCommand(CanExecute = nameof(CanConfirmPickup))]
    private async Task ConfirmPickupAsync()
    {
        IsBusy = true;
        TakeCardTime = DateTime.Now;
        CurrentState = TakeCardFlowState.Completed;
        IsBusy = false;
    }

    [RelayCommand(CanExecute = nameof(CanComplete))]
    private void Complete()
    {
        StopCountdownTimer();
        Back();
    }

    protected override void Back()
    {
        StopCountdownTimer();
        _operationCts?.Cancel();
        _operationCts?.Dispose();
        _operationCts = null;
        base.Back();
    }

    #region Countdown Timer

    private void StartCountdownTimer()
    {
        StopCountdownTimer();
        CountdownSeconds = _config.CountdownSeconds;
        CountdownText = FormatCountdownText(CountdownSeconds);
        IsCountdownVisible = true;

        _countdownCts = new CancellationTokenSource();
        var token = _countdownCts.Token;

        _countdownTask = Task.Run(async () =>
        {
            while (!token.IsCancellationRequested)
            {
                await Task.Delay(1000, token);
                if (token.IsCancellationRequested) break;

                Avalonia.Threading.Dispatcher.UIThread.Invoke(() =>
                {
                    if (CountdownSeconds > 0)
                    {
                        CountdownSeconds--;
                        CountdownText = FormatCountdownText(CountdownSeconds);
                    }

                    if (CountdownSeconds <= 0)
                    {
                        HandleCountdownExpired();
                    }
                });
            }
        }, token);
    }

    private void StopCountdownTimer()
    {
        _countdownCts?.Cancel();
        _countdownCts?.Dispose();
        _countdownCts = null;
        _countdownTask = null;
    }

    private void ResetCountdown()
    {
        Avalonia.Threading.Dispatcher.UIThread.Invoke(() =>
        {
            CountdownSeconds = _config.CountdownSeconds;
            CountdownText = FormatCountdownText(CountdownSeconds);
        });
    }

    private string FormatCountdownText(int seconds)
    {
        return string.Format(LanguageProvider.SelfService_TakeCard_Countdown, seconds);
    }

    private async Task<bool> CheckCountdownExpiredAsync()
    {
        if (CountdownSeconds <= 0)
        {
            await HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
            return true;
        }
        return false;
    }

    private void HandleCountdownExpired()
    {
        if (IsConfirmState)
        {
            // Idle timeout - return to home
            StopCountdownTimer();
            Avalonia.Threading.Dispatcher.UIThread.Invoke(() => Back());
        }
        else if (IsCardReadyToPickupState)
        {
            // Card not picked up - discard
            HandlePickupTimeout();
        }
        else if (IsCardProcessingState)
        {
            // Operation timeout
            _ = HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
        }
    }

    #endregion

    #region Card Operations

    private async Task<bool> MoveCardToReaderAsync()
    {
        try
        {
            bool connected = await _cardPrinter.ConnectAsync(_printerId);
            if (!connected)
            {
                System.Diagnostics.Debug.WriteLine($"[TakeCard] Failed to connect to printer: {_printerId}");
            }

            await _cardPrinter.MoveCardAsync(_printerId, CardMoveCommand.MoveFromStorageToPrepare);
            await Task.Delay(300);
            await _cardPrinter.MoveCardAsync(_printerId, CardMoveCommand.MoveToContact);
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TakeCard] MoveCardToReader failed: {ex.Message}");
            return false;
        }
    }

    private async Task<string> SimulateReadCardAsync()
    {
        // Simulate card reading - use UserInfo + timestamp as mock physical card number
        var mockPhysicalCardId = string.IsNullOrEmpty(_printerId)
            ? $"PHYS-{DateTime.Now:yyyyMMddHHmmss}"
            : $"PHYS-{_printerId}-{DateTime.Now:yyyyMMddHHmmss}";

        System.Diagnostics.Debug.WriteLine($"[TakeCard] Simulated card read: {mockPhysicalCardId}");
        await Task.Delay(500);
        return mockPhysicalCardId;
    }

    private async Task<CardInitResult> InitCardAsync(string physicalCardId, CancellationToken ct)
    {
        var result = new CardInitResult();

        if (_yktApiClient == null)
        {
            System.Diagnostics.Debug.WriteLine("[TakeCard] YktApiClient not configured, using mock init.");
            // Mock InitCard response for development
            result.Success = true;
            result.CardId = Guid.NewGuid().ToString("N");
            result.CardNo = GenerateCardNo();
            result.UserId = UserInfo?.Id ?? string.Empty;
            result.ExpiryDate = "2030-12-31";
            result.FactoryFixId = physicalCardId;
            result.MainDeputyType = "1";
            result.CardTypeId = "1";
            result.CardOperate = "新卡";
            result.WorkStationNumb = _printerId;
            result.TenantId = _config.TenantId;
            return result;
        }

        var request = new CardInitRequestDto
        {
            CardId = Guid.NewGuid().ToString("N"),
            UserId = UserInfo?.Id ?? string.Empty,
            CardTypeId = "1",
            ExpiryDate = "2030-12-31",
            FactoryFixId = physicalCardId,
            MainDeputyType = "1",
            CardNo = GenerateCardNo(),
            CardOperate = "新卡",
            WorkStationNumb = _printerId,
            TenantId = _config.TenantId
        };

        // Populate old card data if replacing an existing card
        if (UserInfo?.CurrentCard is not null)
        {
            request.OldCardNo = UserInfo.CurrentCard.CardNo;
            request.OldFactoryFixId = UserInfo.CurrentCard.FactoryFixId;
            request.OldCardId = UserInfo.CurrentCard.CardId;
            request.CardOperate = "换卡";
        }

        try
        {
            var response = await _yktApiClient.InitCardAsync(request, ct);
            result.Success = response.Code == 200 || response.Code == null;
            result.ErrorMessage = response.Message;

            // Copy request data for downstream use
            result.CardId = request.CardId;
            result.CardNo = request.CardNo;
            result.UserId = request.UserId;
            result.ExpiryDate = request.ExpiryDate;
            result.FactoryFixId = request.FactoryFixId;
            result.MainDeputyType = request.MainDeputyType;
            result.CardTypeId = request.CardTypeId;
            result.CardOperate = request.CardOperate;
            result.WorkStationNumb = request.WorkStationNumb;
            result.TenantId = request.TenantId;
        }
        catch (Exception ex)
        {
            result.Success = false;
            result.ErrorMessage = ex.Message;
        }

        return result;
    }

    private async Task<CardWriteResult> SimulateWriteCardAsync(CardInitResult initResult)
    {
        // Simulate writing card data: card serial number, personnel ID, card expiry date
        var writeResult = new CardWriteResult
        {
            Success = true,
            ErrorMessage = string.Empty,
            WrittenCardNo = initResult.CardNo,
            WrittenUserId = initResult.UserId,
            WrittenExpiryDate = initResult.ExpiryDate
        };

        System.Diagnostics.Debug.WriteLine($"[TakeCard] Simulated write card: CardNo={writeResult.WrittenCardNo}, UserId={writeResult.WrittenUserId}, ExpiryDate={writeResult.WrittenExpiryDate}");
        await Task.Delay(500);
        return writeResult;
    }

    private async Task<bool> PrintCardAsync(CardInitResult initResult)
    {
        try
        {
            using var session = _cardPrinter.BeginPrintSession(_printerId);
            session.BeginPage();

            // Print card holder name
            if (!string.IsNullOrEmpty(UserInfo?.Name))
            {
                session.PrintText(x: 120, y: 80, text: UserInfo.Name, fontName: "SimHei", fontSize: 18);
            }

            // Print card serial number
            if (!string.IsNullOrEmpty(initResult.CardNo))
            {
                session.PrintText(x: 120, y: 120, text: initResult.CardNo, fontName: "Arial", fontSize: 14);
            }

            // Print expiry date
            if (!string.IsNullOrEmpty(initResult.ExpiryDate))
            {
                session.PrintText(x: 120, y: 150, text: initResult.ExpiryDate, fontName: "Arial", fontSize: 12);
            }

            session.EndPage();
            session.Dispose();
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TakeCard] PrintCard failed: {ex.Message}");
            return false;
        }
    }

    private async Task HandleOperationFailedAsync(string errorMessage)
    {
        _operationCts?.Cancel();
        IsBusy = false;
        IsCountdownVisible = false;
        OperationStepText = string.Format(LanguageProvider.SelfService_TakeCard_Status_OperationFailed, errorMessage);
        CurrentState = TakeCardFlowState.OperationFailed;
    }

    private async Task DiscardCardToRejectAsync()
    {
        try
        {
            await _cardPrinter.MoveCardAsync(_printerId, CardMoveCommand.MoveToRejectBoxFront);
            System.Diagnostics.Debug.WriteLine("[TakeCard] Card moved to reject box.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TakeCard] DiscardCardToReject failed: {ex.Message}");
        }
    }

    #endregion

    #region Pickup Monitor

    private void StartPickupMonitor(string physicalCardId, CardInitResult initResult)
    {
        // Start countdown for pickup - if user doesn't pick up card in time, discard it
        CountdownSeconds = _config.CountdownSeconds;
        CountdownText = FormatCountdownText(CountdownSeconds);
        IsCountdownVisible = true;

        _countdownCts = new CancellationTokenSource();
        var token = _countdownCts.Token;

        _countdownTask = Task.Run(async () =>
        {
            while (!token.IsCancellationRequested)
            {
                await Task.Delay(1000, token);
                if (token.IsCancellationRequested) break;

                Avalonia.Threading.Dispatcher.UIThread.Invoke(() =>
                {
                    CountdownText = FormatCountdownText(CountdownSeconds);
                    if (CountdownSeconds > 0)
                    {
                        CountdownSeconds--;
                    }
                    else
                    {
                        HandlePickupTimeout();
                    }
                });
            }
        }, token);

        void HandlePickupTimeout()
        {
            Avalonia.Threading.Dispatcher.UIThread.Invoke(async () =>
            {
                // Check card position
                var position = await _cardPrinter.GetCardPositionAsync(_printerId);
                if (position != CardPositionState.OutOfPrinter)
                {
                    // Card still in printer - discard
                    await DiscardCardToRejectAsync();
                    StatusHint = LanguageProvider.SelfService_TakeCard_Status_Timeout_Discard;
                }
                else
                {
                    StatusHint = LanguageProvider.SelfService_TakeCard_Status_CardReturned;
                }
                IsCountdownVisible = false;
                StopCountdownTimer();
            });
        }
    }

    private void HandlePickupTimeout()
    {
        Avalonia.Threading.Dispatcher.UIThread.Invoke(async () =>
        {
            var position = await _cardPrinter.GetCardPositionAsync(_printerId);
            if (position != CardPositionState.OutOfPrinter)
            {
                await DiscardCardToRejectAsync();
                PickupInstructionText = LanguageProvider.SelfService_TakeCard_Status_Timeout_Discard;
            }
            else
            {
                PickupInstructionText = LanguageProvider.SelfService_TakeCard_Status_CardReturned;
            }
            IsCountdownVisible = false;
            StopCountdownTimer();
        });
    }

    #endregion

    #region API Calls

    private void WriteCardSuccessApi(CardInitResult initResult)
    {
        if (_yktApiClient == null) return;

        try
        {
            var request = new DynamicRequestDto
            {
                AdditionalData = new Dictionary<string, JsonElement>
                {
                    ["cardNo"] = JsonSerializer.SerializeToElement(initResult.CardNo),
                    ["factoryFixId"] = JsonSerializer.SerializeToElement(initResult.FactoryFixId),
                    ["userId"] = JsonSerializer.SerializeToElement(initResult.UserId),
                    ["tenantId"] = JsonSerializer.SerializeToElement(initResult.TenantId)
                }
            };
            _ = _yktApiClient.WriteCardSuccessAsync(request);
            System.Diagnostics.Debug.WriteLine("[TakeCard] WriteCardSuccess API called.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TakeCard] WriteCardSuccess API failed: {ex.Message}");
        }
    }

    private async Task WriteCardFailureApi(CardInitResult initResult, string? errorMessage)
    {
        if (_yktApiClient == null) return;

        try
        {
            var request = new DynamicRequestDto
            {
                AdditionalData = new Dictionary<string, JsonElement>
                {
                    ["cardNo"] = JsonSerializer.SerializeToElement(initResult.CardNo),
                    ["factoryFixId"] = JsonSerializer.SerializeToElement(initResult.FactoryFixId),
                    ["userId"] = JsonSerializer.SerializeToElement(initResult.UserId),
                    ["tenantId"] = JsonSerializer.SerializeToElement(initResult.TenantId),
                    ["errorReason"] = JsonSerializer.SerializeToElement(errorMessage ?? "Unknown error")
                }
            };
            await _yktApiClient.WriteCardFailureAsync(request);
            System.Diagnostics.Debug.WriteLine("[TakeCard] WriteCardFailure API called.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[TakeCard] WriteCardFailure API failed: {ex.Message}");
        }
    }

    #endregion

    #region Helper Methods

    private string GenerateCardNo()
    {
        return $"CARD{DateTime.Now:yyyyMMddHHmmss}{new Random().Next(1000, 9999)}";
    }

    private void ResolveCardInfo(UserInfoModel? data)
    {
        var currentCard = data?.CurrentCard;

        if (currentCard is not null)
        {
            CardNumber = currentCard.CardNo;
            CardStatusText = currentCard.CardStatusName;
            return;
        }

        CardNumber = "-";
        CardStatusText = "待取卡";
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

    #endregion

    #region Result Types

    private sealed class CardInitResult
    {
        public bool Success { get; set; }
        public string? ErrorMessage { get; set; }
        public string CardId { get; set; } = string.Empty;
        public string CardNo { get; set; } = string.Empty;
        public string UserId { get; set; } = string.Empty;
        public string CardTypeId { get; set; } = string.Empty;
        public string ExpiryDate { get; set; } = string.Empty;
        public string FactoryFixId { get; set; } = string.Empty;
        public string MainDeputyType { get; set; } = string.Empty;
        public string CardOperate { get; set; } = string.Empty;
        public string WorkStationNumb { get; set; } = string.Empty;
        public string TenantId { get; set; } = string.Empty;
    }

    private sealed class CardWriteResult
    {
        public bool Success { get; set; }
        public string? ErrorMessage { get; set; }
        public string WrittenCardNo { get; set; } = string.Empty;
        public string WrittenUserId { get; set; } = string.Empty;
        public string WrittenExpiryDate { get; set; } = string.Empty;
    }

    #endregion
}

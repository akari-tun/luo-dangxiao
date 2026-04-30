using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Requests;
using System.Text.Json;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Shared state for card processing operations (take card &amp; replacement).
/// </summary>
public enum CardProcessingState
{
    Confirm,
    CardProcessing,
    CardReadyToPickup,
    Completed,
    OperationFailed
}

/// <summary>
/// Abstract base class providing shared countdown management, card processing pipeline,
/// and pickup monitoring for both TakeCard and Replacement flows.
/// </summary>
public abstract partial class CardOperationViewModelBase : ViewModelBase
{
    #region Dependencies provided by subclasses

    /// <summary>
    /// Application configuration providing countdown seconds and tenant ID.
    /// </summary>
    protected abstract SelfServiceConfig Config { get; }

    /// <summary>
    /// Printer abstraction for card movement and printing.
    /// </summary>
    protected abstract CardPrinterBase CardPrinter { get; }

    /// <summary>
    /// Card reader abstraction for physical card operations.
    /// </summary>
    protected abstract CardReaderBase CardReader { get; }

    /// <summary>
    /// YKT API client; may be null (mock mode) if not configured.
    /// </summary>
    protected abstract IYktApiClient? YktApiClient { get; }

    /// <summary>
    /// Printer identifier resolved from configuration.
    /// </summary>
    protected abstract string PrinterId { get; }

    /// <summary>
    /// User information bound to the page. Must be set by the subclass before card processing.
    /// </summary>
    protected abstract UserInfoModel? UserInfoData { get; }

    #endregion

    #region Countdown Timer

    private CancellationTokenSource? _countdownCts;
    private Task? _countdownTask;
    private CancellationTokenSource? _operationCts;

    [ObservableProperty]
    protected int _countdownSeconds;

    [ObservableProperty]
    protected string _countdownText = string.Empty;

    [ObservableProperty]
    protected bool _isCountdownVisible;

    /// <summary>
    /// Text shown on the countdown display, formatted via localized "seconds" template.
    /// </summary>
    protected string FormatCountdown(int seconds) =>
        string.Format(LanguageProvider.SelfService_TakeCard_Countdown, seconds);

    public void StartCountdownTimer()
    {
        StopCountdownTimer();
        CountdownSeconds = Config.CountdownSeconds;
        CountdownText = FormatCountdown(CountdownSeconds);
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
                        CountdownText = FormatCountdown(CountdownSeconds);
                    }
                    if (CountdownSeconds <= 0)
                    {
                        HandleCountdownExpired();
                    }
                });
            }
        }, token);
    }

    public void StopCountdownTimer()
    {
        _countdownCts?.Cancel();
        _countdownCts?.Dispose();
        _countdownCts = null;
        _countdownTask = null;
    }

    protected void ResetCountdown()
    {
        Avalonia.Threading.Dispatcher.UIThread.Invoke(() =>
        {
            CountdownSeconds = Config.CountdownSeconds;
            CountdownText = FormatCountdown(CountdownSeconds);
        });
    }

    protected async Task<bool> CheckCountdownExpiredAsync()
    {
        if (CountdownSeconds <= 0)
        {
            await HandleOperationFailedAsync(LanguageProvider.SelfService_TakeCard_Status_Timeout);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Subclass hook invoked when the countdown reaches zero.
    /// Implement Confirm-to-home, CardReadyToPickup-to-discard, and Processing-to-fail behaviors.
    /// </summary>
    protected abstract void HandleCountdownExpired();

    #endregion

    #region State

    [ObservableProperty]
    protected CardProcessingState _currentState = CardProcessingState.Confirm;

    [ObservableProperty]
    protected bool _isBusy;

    [ObservableProperty]
    protected string _operationStepText = string.Empty;

    [ObservableProperty]
    protected string _pickupInstructionText = string.Empty;

    public bool IsConfirmState => CurrentState == CardProcessingState.Confirm;
    public bool IsCardProcessingState => CurrentState == CardProcessingState.CardProcessing;
    public bool IsCardReadyToPickupState => CurrentState == CardProcessingState.CardReadyToPickup;
    public bool IsCompletedState => CurrentState == CardProcessingState.Completed;
    public bool IsOperationFailedState => CurrentState == CardProcessingState.OperationFailed;
    public bool CanConfirmPickup => IsCardReadyToPickupState && !IsBusy;
    public bool CanComplete => IsCompletedState;

    [RelayCommand(CanExecute = nameof(CanConfirmPickup))]
    protected virtual void ConfirmPickup()
    {
        IsBusy = true;
        CurrentState = CardProcessingState.Completed;
        StopCountdownTimer();
        IsBusy = false;
    }

    [RelayCommand(CanExecute = nameof(CanComplete))]
    protected void Complete() => Back();

    partial void OnCurrentStateChanged(CardProcessingState value)
    {
        OnPropertyChanged(nameof(IsConfirmState));
        OnPropertyChanged(nameof(IsCardProcessingState));
        OnPropertyChanged(nameof(IsCardReadyToPickupState));
        OnPropertyChanged(nameof(IsCompletedState));
        OnPropertyChanged(nameof(IsOperationFailedState));
        OnPropertyChanged(nameof(CanConfirmPickup));
        OnPropertyChanged(nameof(CanComplete));
        OnStateChanged(value);
    }

    partial void OnIsBusyChanged(bool value)
    {
        OnPropertyChanged(nameof(CanConfirmPickup));
        OnBusyChanged(value);
    }

    protected virtual void OnStateChanged(CardProcessingState value) { }
    protected virtual void OnBusyChanged(bool value) { }

    protected override void Back()
    {
        StopCountdownTimer();
        _operationCts?.Cancel();
        _operationCts?.Dispose();
        _operationCts = null;
        base.Back();
    }

    #endregion

    #region Card Processing Pipeline

    /// <summary>
    /// Orchestrates the full card lifecycle: move �?read �?init �?write �?print �?output.
    /// Subclasses call this from their start/replace button command.
    /// </summary>
    /// <param name="cardOperate">
    /// Label used for the InitCard API operation ("新卡" for take card, "换卡" for replacement).
    /// </param>
    protected async Task ExecuteCardProcessAsync(string cardOperate)
    {
        ResetCountdown();
        IsBusy = true;
        CurrentState = CardProcessingState.CardProcessing;
        SetProcessingTitle();

        _operationCts = new CancellationTokenSource();
        var opToken = _operationCts.Token;

        try
        {
            // Move card to reader
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_MovingCard;
            if (!await MoveCardToReaderAsync())
            {
                await HandleOperationFailedAsync("Failed to move card to reader position.");
                return;
            }

            // Read card
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_ReadingCard;
            var factoryFixId = await ReadFactoryFixIdAsync();
            if (factoryFixId <= 0)
            {
                await HandleOperationFailedAsync("Failed to read card information.");
                return;
            }

            // Initialize card
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_InitializingCard;
            var initResult = await InitCardAsync(factoryFixId, cardOperate, opToken);
            if (!initResult.Success)
            {
                await HandleOperationFailedAsync(initResult.ErrorMessage ?? "Card initialization failed.");
                await DiscardCardToRejectAsync();
                return;
            }

            // Write card
            if (await CheckCountdownExpiredAsync()) return;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_WritingCard;
            var writeResult = await WriteCardAsync(initResult);
            if (!writeResult.Success)
            {
                await WriteCardFailureApi(initResult, writeResult.ErrorMessage);
                await HandleOperationFailedAsync(writeResult.ErrorMessage ?? "Failed to write card data.");
                await DiscardCardToRejectAsync();
                return;
            }

            // Print card
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

            // Report success
            WriteCardSuccessApi(initResult);

            // Move card to front holder
            await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToFront);

            // Transition to ready-to-pickup
            PickupInstructionText = GetPickupInstructionText();
            CurrentState = CardProcessingState.CardReadyToPickup;
            IsCountdownVisible = false;
            IsBusy = false;

            StartPickupMonitor(initResult);
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

    /// <summary>
    /// Subclass hook to set the processing title when entering CardProcessing state.
    /// </summary>
    protected abstract void SetProcessingTitle();

    /// <summary>
    /// Subclass hook to return the text shown when the card is ready for pickup.
    /// </summary>
    protected abstract string GetPickupInstructionText();

    #endregion

    #region Hardware & API Operations

    private async Task<bool> MoveCardToReaderAsync()
    {
        try
        {
            _ = await CardPrinter.ConnectAsync(PrinterId);
            await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveFromStorageToPrepare);
            await Task.Delay(300);
            await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToContact);
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[CardOperation] MoveCardToReader failed: {ex.Message}");
            return false;
        }
    }

    private async Task<uint> ReadFactoryFixIdAsync()
    {
        return await Task.Run(() => CardReader.ReadCardId(out uint factoryFixId) ? factoryFixId : 0u);
    }

    private async Task<CardInitResult> InitCardAsync(uint factoryFixId, string cardOperate, CancellationToken ct)
    {
        var result = new CardInitResult();

        if (YktApiClient == null)
        {
            System.Diagnostics.Debug.WriteLine("[CardOperation] YktApiClient is not configured. Card initialization aborted.");
            result.Success = false;
            result.ErrorMessage = "YKT API service is not available.";
            return result;
        }

        var userInfo = UserInfoData ?? throw new InvalidOperationException("UserInfo is not set.");
        var currentCard = userInfo.CurrentCard;
        var tenantId = string.IsNullOrWhiteSpace(currentCard?.TenantId) ? Config.TenantId : currentCard.TenantId;

        var request = new CardInitRequestDto
        {
            CardId = string.Empty,
            UserId = ResolveUserId(userInfo),
            CardTypeId = currentCard?.CardTypeId.ToString() ?? "1",
            ExpiryDate = GetExpiryDate(currentCard, userInfo),
            FactoryFixId = factoryFixId.ToString(),
            MainDeputyType = (currentCard?.MainDeputyType ?? 0).ToString(),
            CardNo = string.Empty,
            CardOperate = cardOperate,
            WorkStationNumb = Config.WorkStationNumb,
            TenantId = tenantId,
            OldCardNo = cardOperate == "REISSUE" ? currentCard?.CardNo : string.Empty,
            OldFactoryFixId = cardOperate == "REISSUE" ? currentCard?.FactoryFixId : string.Empty,
            OldCardId = cardOperate == "REISSUE" ? currentCard?.CardId : string.Empty
        };

        try
        {
            var response = await YktApiClient.InitCardAsync(request, ct);
            result.Success = IsApiSuccess(response);
            result.ErrorMessage = FormatApiError(response.Message, "卡片初始化失败。");

            if (response.Data.HasValue)
            {
                result.CardId = ExtractJsonString(response.Data, "cardId", "CardId") ?? string.Empty;
                result.CardNo = ExtractJsonString(response.Data, "cardNo", "CardNo") ?? string.Empty;
                result.UserId = ExtractJsonString(response.Data, "userId", "UserId") ?? string.Empty;
                result.CardTypeId = ExtractJsonString(response.Data, "cardTypeId", "CardTypeId") ?? string.Empty;
                result.ExpiryDate = ExtractJsonString(response.Data, "expiryDate", "ExpiryDate") ?? string.Empty;
                result.FactoryFixId = ExtractJsonString(response.Data, "factoryFixId", "FactoryFixId") ?? string.Empty;
                result.MainDeputyType = ExtractJsonString(response.Data, "mainDeputyType", "MainDeputyType") ?? "1";
                result.TenantId = ExtractJsonString(response.Data, "tenantId", "TenantId") ?? tenantId;
                var apiCardOperate = ExtractJsonString(response.Data, "cardOperate", "CardOperate");
                var apiWorkStation = ExtractJsonString(response.Data, "workStationNumb", "WorkStationNumb");
                result.CardOperate = apiCardOperate ?? request.CardOperate;
                result.WorkStationNumb = apiWorkStation ?? PrinterId;
            }

            result.CardOperate = request.CardOperate;
            result.WorkStationNumb = request.WorkStationNumb;
        }
        catch (Exception ex)
        {
            result.Success = false;
            result.ErrorMessage = ex.Message;
        }

        return result;
    }

    private static string GetExpiryDate(CardInfoModel? currentCard, UserInfoModel userInfo)
    {
        var date = currentCard?.ExpiryDate;
        if (!date.HasValue)
        {
            date = userInfo switch
            {
                StaffInfoModel staff => staff.CardExpiryDate,
                StudentInfoModel student => student.TrainingEndDate,
                _ => null
            };
        }
        return date?.ToString("yyyy-MM-dd HH:mm:ss") ?? DateTime.Today.AddYears(1).ToString("yyyy-MM-dd HH:mm:ss");
    }

    private async Task<CardWriteResult> WriteCardAsync(CardInitResult initResult)
    {
        var writeResult = new CardWriteResult
        {
            Success = true,
            ErrorMessage = string.Empty,
            WrittenCardNo = initResult.CardNo,
            WrittenUserId = initResult.UserId,
            WrittenExpiryDate = initResult.ExpiryDate
        };

        System.Diagnostics.Debug.WriteLine($"[CardOperation] Write card: CardNo={writeResult.WrittenCardNo}, UserId={writeResult.WrittenUserId}, ExpiryDate={writeResult.WrittenExpiryDate}");
        await Task.Delay(500);
        return writeResult;
    }

    private async Task<bool> PrintCardAsync(CardInitResult initResult)
    {
        try
        {
            using var session = CardPrinter.BeginPrintSession(PrinterId);
            session.BeginPage();
            if (!string.IsNullOrEmpty(UserInfoData?.Name))
                session.PrintText(x: 120, y: 80, text: UserInfoData.Name, fontName: "SimHei", fontSize: 18);
            if (!string.IsNullOrEmpty(initResult.CardNo))
                session.PrintText(x: 120, y: 120, text: initResult.CardNo, fontName: "Arial", fontSize: 14);
            if (!string.IsNullOrEmpty(initResult.ExpiryDate))
                session.PrintText(x: 120, y: 150, text: initResult.ExpiryDate, fontName: "Arial", fontSize: 12);
            session.EndPage();
            session.Dispose();
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[CardOperation] PrintCard failed: {ex.Message}");
            return false;
        }
    }

    protected async Task HandleOperationFailedAsync(string errorMessage)
    {
        _operationCts?.Cancel();
        IsBusy = false;
        IsCountdownVisible = false;
        OperationStepText = string.Format(LanguageProvider.SelfService_TakeCard_Status_OperationFailed, errorMessage);
        CurrentState = CardProcessingState.OperationFailed;
    }

    protected async Task DiscardCardToRejectAsync()
    {
        try
        {
            await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToRejectBoxFront);
            System.Diagnostics.Debug.WriteLine("[CardOperation] Card moved to reject box.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[CardOperation] DiscardCardToReject failed: {ex.Message}");
        }
    }

    private void WriteCardSuccessApi(CardInitResult initResult)
    {
        if (YktApiClient == null) return;
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
            _ = YktApiClient.WriteCardSuccessAsync(request);
            System.Diagnostics.Debug.WriteLine("[CardOperation] WriteCardSuccess API called.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[CardOperation] WriteCardSuccess API failed: {ex.Message}");
        }
    }

    private async Task WriteCardFailureApi(CardInitResult initResult, string? errorMessage)
    {
        if (YktApiClient == null) return;
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
            await YktApiClient.WriteCardFailureAsync(request);
            System.Diagnostics.Debug.WriteLine("[CardOperation] WriteCardFailure API called.");
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"[CardOperation] WriteCardFailure API failed: {ex.Message}");
        }
    }

    #endregion

    #region Pickup Monitor

    protected void StartPickupMonitor(CardInitResult initResult)
    {
        CountdownSeconds = Config.CountdownSeconds;
        CountdownText = FormatCountdown(CountdownSeconds);
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
                    CountdownText = FormatCountdown(CountdownSeconds);
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
    }

    private void HandlePickupTimeout()
    {
        Avalonia.Threading.Dispatcher.UIThread.Invoke(async () =>
        {
            var position = await CardPrinter.GetCardPositionAsync(PrinterId);
            if (position != CardPositionState.OutOfPrinter)
            {
                await DiscardCardToRejectAsync();
                OnPickupTimeoutDiscarded();
            }
            else
            {
                OnPickupTimeoutReturned();
            }
            IsCountdownVisible = false;
            StopCountdownTimer();
        });
    }

    /// <summary>
    /// Invoked when the pickup countdown expires and the card is still in the printer.
    /// Subclass should update status text / instructions.
    /// </summary>
    protected abstract void OnPickupTimeoutDiscarded();

    /// <summary>
    /// Invoked when the pickup countdown expires but the card is already out of the printer.
    /// Subclass should update status text / instructions.
    /// </summary>
    protected abstract void OnPickupTimeoutReturned();

    #endregion

    #region Helpers

    private static string ResolveUserId(UserInfoModel? userInfo)
    {
        return userInfo switch
        {
            StudentInfoModel student when !string.IsNullOrWhiteSpace(student.UserId) => student.UserId,
            StaffInfoModel staff when !string.IsNullOrWhiteSpace(staff.UserId) => staff.UserId,
            _ => string.Empty
        };
    }

    private static string ResolveCardNo(CardInfoModel? currentCard)
    {
        return string.IsNullOrWhiteSpace(currentCard?.CardNo)
            ? GenerateCardNo()
            : currentCard!.CardNo;
    }

    private static string GenerateCardNo() =>
        $"CARD{DateTime.Now:yyyyMMddHHmmss}{new Random().Next(1000, 9999)}";

    protected sealed class CardInitResult
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

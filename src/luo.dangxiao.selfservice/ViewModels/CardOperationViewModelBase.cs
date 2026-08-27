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
/// Result of a card operation pipeline run.
/// </summary>
/// <param name="Success">Whether the pipeline completed without error.</param>
/// <param name="ErrorMessage">Error description when <paramref name="Success"/> is false; null otherwise.</param>
/// <param name="Cancelled">Whether the pipeline was interrupted by countdown expiry.</param>
public readonly record struct CardOperationResult(bool Success, string? ErrorMessage, bool Cancelled = false)
{
    public static CardOperationResult SuccessResult => new(Success: true, ErrorMessage: null);
    public static CardOperationResult Failed(string message) => new(Success: false, ErrorMessage: message);
    public static CardOperationResult CountdownExpired => new(Success: false, ErrorMessage: null, Cancelled: true);
}

/// <summary>
/// Abstract base class providing shared countdown management, card processing pipeline,
/// and pickup monitoring for both TakeCard and Replacement flows.
/// </summary>
public abstract partial class CardOperationViewModelBase : ViewModelBase
{
    protected CardOperationViewModelBase() : base()
    {
    }

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
        LogCommand(nameof(ConfirmPickup));
        IsBusy = true;
        CurrentState = CardProcessingState.Completed;
        StopCountdownTimer();
        IsBusy = false;
    }

    [RelayCommand(CanExecute = nameof(CanComplete))]
    protected void Complete()
    {
        LogCommand(nameof(Complete));
        Back();
    }

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
        LogCommand(nameof(ExecuteCardProcessAsync), $"Operation={cardOperate}, User={MaskLogValue(UserInfoData?.Name)}, CardNo={MaskLogValue(UserInfoData?.CurrentCard?.CardNo)}");
        ResetCountdown();
        IsBusy = true;
        CurrentState = CardProcessingState.CardProcessing;
        SetProcessingTitle();

        _operationCts = new CancellationTokenSource();
        var opToken = _operationCts.Token;

        var result = await ExecuteCardOperationPipelineAsync(cardOperate, opToken);

        if (!result.Success)
        {
            await HandleOperationFailedAsync(result.ErrorMessage ?? "Failed to card operation.");
            await EnsureCardRecoveryToRejectAsync();
        }
    }

    /// <summary>
    /// Orchestrates the full card lifecycle: move to reader → read → init → write → print → output.
    /// Returns a result indicating success or failure with an error message.
    /// Does NOT handle card recovery — caller is responsible for ensuring the card is discarded.
    /// </summary>
    private async Task<CardOperationResult> ExecuteCardOperationPipelineAsync(string cardOperate, CancellationToken opToken)
    {
        try
        {
            if (!Config.PrinterConfig.EnableIssueCard)
            {
                OperationStepText = LanguageProvider.SelfService_TakeCard_Status_InitializingCard;
                var initResultSkip = await InitCardAsync(0, cardOperate, opToken);
                if (!initResultSkip.Success)
                {
                    return CardOperationResult.Failed(initResultSkip.ErrorMessage ?? "Card initialization failed.");
                }

                await WriteCardSuccessApiAsync(initResultSkip);
                PickupInstructionText = GetPickupInstructionText();
                CurrentState = CardProcessingState.CardReadyToPickup;
                IsCountdownVisible = false;
                IsBusy = false;
                return CardOperationResult.SuccessResult;
            }

            // Move card to reader
            if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_MovingCard;
            if (!await MoveCardToReaderAsync())
            {
                return CardOperationResult.Failed(GetPrinterLastError("Failed to move card to reader position."));
            }

            // Read card
            if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_ReadingCard;
            var factoryFixId = await ReadFactoryFixIdAsync();
            if (factoryFixId <= 0)
            {
                return CardOperationResult.Failed("Failed to read card information.");
            }

            // Initialize card
            if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_InitializingCard;
            var initResult = await InitCardAsync(factoryFixId, cardOperate, opToken);
            if (!initResult.Success)
            {
                await DiscardCardToRejectAsync();
                return CardOperationResult.Failed(initResult.ErrorMessage ?? "Card initialization failed.");
            }

            // Write card
            if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
            ResetCountdown();
            OperationStepText = LanguageProvider.SelfService_TakeCard_Status_WritingCard;
            var writeResult = await WriteCardAsync(initResult);
            if (!writeResult.Success)
            {
                await WriteCardFailureApi(initResult, writeResult.ErrorMessage);
                await DiscardCardToRejectAsync();
                return CardOperationResult.Failed(writeResult.ErrorMessage ?? "Failed to write card data.");
            }

            // Print card
            if (Config.PrinterConfig.EnablePrint && Config.PrinterConfig.PrintText?.Count > 0)
            {
                if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
                ResetCountdown();
                OperationStepText = LanguageProvider.SelfService_TakeCard_Status_PrintingCard;
                var printSuccess = await PrintCardAsync(initResult);
                if (!printSuccess)
                {
                    await DiscardCardToRejectAsync();
                    await WriteCardFailureApi(initResult, writeResult.ErrorMessage);
                    return CardOperationResult.Failed(GetPrinterLastError("Failed to print card."));
                }

                var status = PrinterStatus.Printing;
                while (status == PrinterStatus.Printing)
                {
                    await Task.Delay(1000, opToken);
                    status = await CardPrinter.GetPrinterStatusAsync(PrinterId);
                    Logger.Debug("Printer status queried. PrinterId={0} Status={1} LastErrorCode={2} LastErrorMsg={3}",
                        PrinterId, status, CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
                    if (await CheckCountdownExpiredAsync()) return CardOperationResult.CountdownExpired;
                }
            }
            else
            {
                // Move card to front holder
                var movedToHopper = await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToHopper);
                if (!movedToHopper)
                {
                    Logger.Warn("Failed to move card to output. PrinterId={0} LastErrorCode={1} LastErrorMsg={2}",
                        PrinterId, CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
                    return CardOperationResult.Failed(GetPrinterLastError("Failed to move card to output."));
                }
            }

            // Report success
            await WriteCardSuccessApiAsync(initResult);

            // Transition to ready-to-pickup
            PickupInstructionText = GetPickupInstructionText();
            CurrentState = CardProcessingState.CardReadyToPickup;
            IsCountdownVisible = false;
            IsBusy = false;

            StartPickupMonitor(initResult);
            return CardOperationResult.SuccessResult;
        }
        catch (OperationCanceledException)
        {
            return CardOperationResult.Failed(LanguageProvider.SelfService_TakeCard_Status_Timeout);
        }
        catch (Exception ex)
        {
            return CardOperationResult.Failed(ex.Message);
        }
    }

    /// <summary>
    /// Checks the printer card position and discards the card to the reject box
    /// if a card is still inside the printer.
    /// </summary>
    private async Task EnsureCardRecoveryToRejectAsync()
    {
        try
        {
            var position = await CardPrinter.GetCardPositionAsync(PrinterId);
            if (position != CardPositionState.OutOfPrinter)
            {
                await DiscardCardToRejectAsync();
            }
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "EnsureCardRecoveryToReject failed. CardNo={0} LastErrorCode={1} LastErrorMsg={2}",
                MaskLogValue(UserInfoData?.CurrentCard?.CardNo), CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
            // Fallback: attempt discard anyway in case position check failed
            await DiscardCardToRejectAsync();
        }
    }

    private string GetPrinterLastError(string fallback)
    {
        var msg = CardPrinter.LastErrorMsg;
        return string.IsNullOrEmpty(msg) ? fallback : msg;
    }

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
            var isConnected = await CardPrinter.ConnectAsync(PrinterId);
            if (!isConnected)
            {
                Logger.Warn("Failed to connect to CardPrinter. PrinterId={0} LastErrorCode={1} LastErrorMsg={2}", PrinterId, CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
                return false;
            }
            //await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveFromStorageToPrepare);
            //await Task.Delay(300);
            var moved = await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToContact);
            if (!moved)
            {
                Logger.Warn("Failed to move card to reader position. PrinterId={0} LastErrorCode={1} LastErrorMsg={2}",
                    PrinterId, CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
            }

            return moved;
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "MoveCardToReader failed. PrinterId={0} LastErrorCode={1} LastErrorMsg={2}",
                PrinterId, CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
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
            Logger.Warn("YktApiClient is not configured. Card initialization aborted. Operation={0}", cardOperate);
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
            Logger.Info("Calling InitCardAsync. Operation={0}, UserId={1}, FactoryFixId={2}, CardNo={3}", cardOperate, MaskLogValue(request.UserId), request.FactoryFixId, MaskLogValue(request.OldCardNo));
            var response = await YktApiClient.InitCardAsync(request, ct);
            LogApiResponse(nameof(YktApiClient.InitCardAsync), response);
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
                result.CardTypeName = ExtractJsonString(response.Data, "cardTypeName", "cardTypeName") ?? string.Empty;
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
            Logger.Error(ex, "InitCardAsync failed. Operation={0}, FactoryFixId={1}", cardOperate, factoryFixId);
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
            WrittenCardNo = initResult.CardNo,
            WrittenUserId = initResult.UserId,
            WrittenExpiryDate = initResult.ExpiryDate
        };

        // Parse expiry date from initResult.ExpiryDate as DateTime
        DateTime expiry;
        uint useTerm;
        if (DateTime.TryParse(initResult.ExpiryDate, out expiry))
        {
            useTerm = (uint)(expiry.Year * 10000 + expiry.Month * 100 + expiry.Day);
        }
        else
        {
            useTerm = (uint)(DateTime.Today.AddYears(1).Year * 10000 + DateTime.Today.AddYears(1).Month * 100 + DateTime.Today.AddYears(1).Day);
        }

        // Parse card type from initResult.CardTypeId (0=auto, 1=user)
        int cardType = 1;
        if (!string.IsNullOrEmpty(initResult.CardTypeId))
        {
            if (!int.TryParse(initResult.CardTypeId, out cardType) || cardType < 1 || cardType > 32)
            {
                cardType = 1;
            }
        }

        int cardId = 0;
        if (!string.IsNullOrEmpty(initResult.CardNo))
        {
            if (!int.TryParse(initResult.CardNo, out cardId))
            {
                cardId = 0;
            }
        }

        uint factoryFixId = 0;

        try
        {
            Logger.Info("Writing card. CardNo={0}, UserId={1}, UserType={2}, UseTerm={3}", MaskLogValue(initResult.CardNo), MaskLogValue(initResult.UserId), cardType, useTerm);

            bool ok = await Task.Run(() => CardReader.InitCard(
                serno: cardId,
                cardNo: initResult.CardNo,
                userType: cardType,
                initialValue: 0,
                useCount: 0,
                useTerm: useTerm,
                factoryFixId: out factoryFixId,
                keyMode: 1,
                empStrId: initResult.UserId,
                empName: UserInfoData?.Name ?? "",
                cardTypeName: initResult.CardTypeName));

            if (!ok)
            {
                writeResult.Success = false;
                writeResult.ErrorMessage = "卡片写入失败";
                Logger.Warn("CardReader.InitCard returned false. CardNo={0}", MaskLogValue(initResult.CardNo));
            }
            else
            {
                writeResult.Success = true;
                writeResult.ErrorMessage = string.Empty;
                writeResult.FactoryFixId = factoryFixId;
                Logger.Info("CardReader.InitCard succeeded. FactoryFixId={0}", factoryFixId);
            }
        }
        catch (Exception ex)
        {
            writeResult.Success = false;
            writeResult.ErrorMessage = ex.Message;
            Logger.Error(ex, "WriteCardAsync failed. CardNo={0}", MaskLogValue(initResult.CardNo));
        }

        return writeResult;
    }

    private async Task<bool> PrintCardAsync(CardInitResult initResult)
    {
        try
        {
            using var session = CardPrinter.BeginPrintSession(PrinterId);
            session.BeginPage();

            var printTextConfigs = Config.PrinterConfig.PrintText;
            if (printTextConfigs != null && printTextConfigs.Count > 0 && UserInfoData != null)
            {
                var userInfoType = typeof(UserInfoModel);
                foreach (var textConfig in printTextConfigs)
                {
                    if (string.IsNullOrEmpty(textConfig.PropertyName))
                        continue;

                    var prop = userInfoType.GetProperty(textConfig.PropertyName);
                    if (prop != null)
                    {
                        var value = prop.GetValue(UserInfoData);
                        var text = value?.ToString();
                        if (!string.IsNullOrEmpty(text))
                        {
                            var printText = string.IsNullOrEmpty(textConfig.Label)
                                ? text
                                : $"{textConfig.Label}{text}";
                            session.PrintText(x: textConfig.X, y: textConfig.Y,
                                text: printText, fontName: textConfig.BodyFont, fontSize: textConfig.BodySize);
                        }
                    }
                }
            }

            session.EndPage();
            return true;
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "PrintCard failed. CardNo={0} LastErrorCode={1} LastErrorMsg={2}",
                MaskLogValue(initResult.CardNo), CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
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
            var moved = await CardPrinter.MoveCardAsync(PrinterId, CardMoveCommand.MoveToRejectBoxFront);
            if (moved)
            {
                Logger.Info("Card moved to reject box. CardNo={0} LastErrorCode={1} LastErrorMsg={2}",
                    MaskLogValue(UserInfoData?.CurrentCard?.CardNo), CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
            }
            else
            {
                Logger.Warn("Failed to move card to reject box. CardNo={0} LastErrorCode={1} LastErrorMsg={2}",
                    MaskLogValue(UserInfoData?.CurrentCard?.CardNo), CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
            }
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "DiscardCardToReject failed. CardNo={0} LastErrorCode={1} LastErrorMsg={2}",
                MaskLogValue(UserInfoData?.CurrentCard?.CardNo), CardPrinter.LastErrorCode, CardPrinter.LastErrorMsg);
        }
    }

    private async Task WriteCardSuccessApiAsync(CardInitResult initResult)
    {
        if (YktApiClient == null)
        {
            Logger.Warn("WriteCardSuccessAsync skipped because YKT API is unavailable.");
            return;
        }

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
            Logger.Info("Calling WriteCardSuccessAsync. CardNo={0}, UserId={1}", MaskLogValue(initResult.CardNo), MaskLogValue(initResult.UserId));
            var response = await YktApiClient.WriteCardSuccessAsync(request);
            LogApiResponse(nameof(YktApiClient.WriteCardSuccessAsync), response);
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "WriteCardSuccessAsync failed. CardNo={0}", MaskLogValue(initResult.CardNo));
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
            Logger.Info("Calling WriteCardFailureAsync. CardNo={0}, UserId={1}, Reason={2}", MaskLogValue(initResult.CardNo), MaskLogValue(initResult.UserId), errorMessage ?? string.Empty);
            var response = await YktApiClient.WriteCardFailureAsync(request);
            LogApiResponse(nameof(YktApiClient.WriteCardFailureAsync), response);
        }
        catch (Exception ex)
        {
            Logger.Error(ex, "WriteCardFailureAsync failed. CardNo={0}", MaskLogValue(initResult.CardNo));
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
        public string CardTypeName { get; set; } = string.Empty;
    }

    private sealed class CardWriteResult
    {
        public bool Success { get; set; }
        public string? ErrorMessage { get; set; }
        public string WrittenCardNo { get; set; } = string.Empty;
        public string WrittenUserId { get; set; } = string.Empty;
        public string WrittenExpiryDate { get; set; } = string.Empty;
        public uint FactoryFixId { get; set; }
    }

    #endregion
}

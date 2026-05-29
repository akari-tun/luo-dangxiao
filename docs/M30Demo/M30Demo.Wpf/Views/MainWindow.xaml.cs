using System;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using M30Demo.Core;

namespace M30Demo.Wpf.Views;

public partial class MainWindow : Window
{
    private static readonly string LogPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "initcard_debug.log");

    public MainWindow()
    {
        InitializeComponent();
    }

    private static void Log(string msg)
    {
        try
        {
            string line = $"[{DateTime.Now:HH:mm:ss.fff}] {msg}";
            File.AppendAllText(LogPath, line + Environment.NewLine);
        }
        catch { }
    }

    private async void BtnExecute_Click(object sender, RoutedEventArgs e)
    {
        btnExecute.IsEnabled = false;
        txtStatus.Text = "Executing...";

        int tab = tabOperations.SelectedIndex;
        CardOperationResponse result;
        try
        {
            if (tab == 2)
            {
                int.TryParse(txtCardID.Text, out int cardID);
                string empID = txtEmpID.Text?.Trim() ?? "U001";
                string empName = txtEmpName.Text?.Trim() ?? "";
                string cardTypeName = txtCardTypeName.Text?.Trim() ?? "";
                int.TryParse(txtCardTypeID.Text, out int cardType);
                int.TryParse(txtUseTerm.Text, out int useTerm);
                decimal.TryParse(txtXCardValue.Text, out decimal xValue);
                decimal.TryParse(txtWCardValue.Text, out decimal wValue);
                bool credit = chkCredit.IsChecked ?? true;
                int issueType = cmbIssueType.SelectedIndex;
                Log($"[UI] Starting InitCard, tab={tab}");
                result = await Task.Run(() => DoInitCard(cardID, empID, empName, cardTypeName, cardType, useTerm, xValue, wValue, credit, issueType));
                Log($"[UI] Result: Success={result.Success}, Msg={result.ErrorMessage}");
            }
            else if (tab == 3)
            {
                uint.TryParse(txtCardSerno.Text, out uint recycleCardSerno);
                Log($"[UI] Starting RecycleCard, cardSerno={recycleCardSerno}");
                result = await Task.Run(() => DoRecycleCard(recycleCardSerno));
                Log($"[UI] Result: Success={result.Success}, Msg={result.ErrorMessage}");
            }
            else
            {
                result = tab switch
                {
                    0 => await Task.Run(DoReadCardId),
                    1 => await Task.Run(DoReadCard),
                    _ => new CardOperationResponse(false, null, "Unknown operation")
                };
            }
        }
        catch (Exception ex)
        {
            Log($"[UI] UNHANDLED EXCEPTION: {ex.GetType().Name}: {ex.Message}\n{ex.StackTrace}");
            result = new CardOperationResponse(false, null, $"CRASH: {ex.GetType().Name}: {ex.Message}");
        }

        var json = JsonSerializer.Serialize(result, new JsonSerializerOptions
        {
            WriteIndented = true,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull
        });
        txtOutput.Text = json;

        txtStatus.Text = result.Success ? "Success" : $"Error ({result.ErrorMessage})";
        btnExecute.IsEnabled = true;
    }

    private void BtnClear_Click(object sender, RoutedEventArgs e)
    {
        txtOutput.Text = string.Empty;
        txtStatus.Text = "Ready";
    }

    CardOperationResponse DoReadCardId()
    {
        using var svc = new YcCardService();
        if (!svc.Open())
            return new(false, null, svc.LastErrorMessage);

        bool ok = svc.ReadPhysicalCardId(out uint cardId, out int tagType);
        if (!ok)
            return new(false, null, svc.LastErrorMessage);

        svc.Beep(100);
        return new(true, new { cardId, tagType, tagTypeHex = $"0x{tagType:X2}" },
            $"Card ID: {cardId}");
    }

    CardOperationResponse DoReadCard()
    {
        using var svc = new YcCardService();
        if (!svc.Open())
            return new(false, null, svc.LastErrorMessage);

        bool ok = svc.ReadCard(out var info);
        if (!ok)
            return new(false, null, svc.LastErrorMessage);

        svc.Beep(100);
        return new(true, new
        {
            info.CardType,
            info.OptNum,
            info.CardID,
            info.UserNo,
            info.UserType,
            info.CardSerno,
            ConsumeValue = info.Value1 / 100m,
            LastPay = info.LastPay1 / 100m,
            info.Count1,
            BackupValue = info.Value2 / 100m,
            ExpiryDate = DecodeTerm(info.UseTerm),
            info.AddCount,
            EmpId = info.EmpId,
            EmpName = info.EmpName,
            WaterValue = info.JsValue / 100m,
            WaterCount = info.JsCount
        }, $"Card read OK. ID: {info.CardSerno}");
    }

    CardOperationResponse DoInitCard(int cardID, string empID, string empName, string cardTypeName, int cardType, int useTerm, decimal xValue, decimal wValue, bool credit, int issueType)
    {
        Log($"[InitCard] Parsing UI inputs...");
        Log($"[InitCard] cardID={cardID}, empID={empID}, empName={empName}, cardTypeName={cardTypeName}, cardType={cardType}, useTerm={useTerm}, xValue={xValue}, wValue={wValue}, credit={credit}, issueType={issueType}");

        Log($"[InitCard] Creating YcCardService...");
        using var svc = new YcCardService();
        
        Log($"[InitCard] Calling Open()...");
        if (!svc.Open())
        {
            Log($"[InitCard] Open() FAILED: {svc.LastErrorMessage}");
            return new(false, null, svc.LastErrorMessage);
        }
        Log($"[InitCard] Open() OK");

        Log($"[InitCard] Calling svc.InitCard()...");
        bool ok = svc.InitCard(
            cardID, empID, cardType, useTerm,
            empName, cardTypeName,
            (int)Math.Round(xValue * 100),
            (int)Math.Round(wValue * 100),
            credit, issueType,
            out uint cardSerno);

        Log($"[InitCard] InitCard returned: ok={ok}, cardSerno={cardSerno}, LastError={svc.LastErrorCode} ({svc.LastErrorMessage})");

        if (!ok)
            return new(false, null, svc.LastErrorMessage);

        svc.Beep(100);
        return new(true, new { cardSerno }, $"Card initialized. ID: {cardSerno}");
    }

    CardOperationResponse DoRecycleCard(uint cardSerno)
    {
        Log($"[RecycleCard] cardSerno={cardSerno}");

        using var svc = new YcCardService();
        if (!svc.Open())
        {
            Log($"[RecycleCard] Open() FAILED: {svc.LastErrorMessage}");
            return new(false, null, svc.LastErrorMessage);
        }

        if (cardSerno == 0)
        {
            bool idOk = svc.ReadPhysicalCardId(out cardSerno, out _);
            if (!idOk)
                return new(false, null, "No card detected and no CardSerno provided");
            Log($"[RecycleCard] Read card ID: {cardSerno}");
        }

        Log($"[RecycleCard] Calling RecycleCard({cardSerno})...");
        bool ok = svc.RecycleCard(cardSerno);
        Log($"[RecycleCard] RecycleCard returned: ok={ok}, LastError={svc.LastErrorCode} ({svc.LastErrorMessage})");

        if (!ok)
            return new(false, null, svc.LastErrorMessage);

        svc.Beep(100);
        return new(true, new { cardSerno }, $"Card {cardSerno} recycled");
    }

    static string DecodeTerm(int useTerm)
    {
        if (useTerm == 0) return "N/A";
        int yy = useTerm / 10000 + 2000;
        int mm = (useTerm % 10000) / 100;
        int dd = useTerm % 100;
        return $"{yy:D4}-{mm:D2}-{dd:D2}";
    }
}

public record CardOperationResponse(
    bool Success,
    object? Data,
    string? ErrorMessage = null)
{
    public int ErrorCode => Success ? 0 : -1;
    public long Timestamp { get; } = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
}

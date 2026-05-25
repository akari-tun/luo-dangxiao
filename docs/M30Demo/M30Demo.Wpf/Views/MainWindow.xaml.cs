using System;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using M30Demo.Core;

namespace M30Demo.Wpf.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    private async void BtnExecute_Click(object sender, RoutedEventArgs e)
    {
        btnExecute.IsEnabled = false;
        txtStatus.Text = "Executing...";

        int tab = tabOperations.SelectedIndex;
        var result = tab switch
        {
            0 => await Task.Run(DoReadCardId),
            1 => await Task.Run(DoReadCard),
            2 => await Task.Run(DoInitCard),
            3 => await Task.Run(DoRecycleCard),
            _ => new CardOperationResponse(false, null, "Unknown operation")
        };

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

    CardOperationResponse DoInitCard()
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

        using var svc = new YcCardService();
        if (!svc.Open())
            return new(false, null, svc.LastErrorMessage);

        bool ok = svc.InitCard(
            cardID, empID, cardType, useTerm,
            empName, cardTypeName,
            (int)Math.Round(xValue * 100),
            (int)Math.Round(wValue * 100),
            credit, issueType,
            out uint cardSerno);

        if (!ok)
            return new(false, null, svc.LastErrorMessage);

        svc.Beep(100);
        return new(true, new { cardSerno }, $"Card initialized. ID: {cardSerno}");
    }

    CardOperationResponse DoRecycleCard()
    {
        uint.TryParse(txtCardSerno.Text, out uint cardSerno);

        using var svc = new YcCardService();
        if (!svc.Open())
            return new(false, null, svc.LastErrorMessage);

        if (cardSerno == 0)
        {
            bool idOk = svc.ReadPhysicalCardId(out cardSerno, out _);
            if (!idOk)
                return new(false, null, "No card detected and no CardSerno provided");
        }

        bool ok = svc.RecycleCard(cardSerno);
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

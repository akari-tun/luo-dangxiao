using System;
using System.Text.Json;
using System.Text.Json.Serialization;
using M30Demo.Core;

namespace M30Demo.Cli;

public static class Program
{
    static int Main(string[] args)
    {
        string? jsonInput = null;

        foreach (var arg in args)
        {
            if (arg.StartsWith("--json="))
            {
                jsonInput = arg.Substring("--json=".Length).Trim('"');
            }
            else if (arg == "--stdin")
            {
                jsonInput = Console.In.ReadToEndAsync().GetAwaiter().GetResult();
            }
            else if (arg.StartsWith("--device="))
            {
                // Reserved for future device path config
            }
        }

        if (string.IsNullOrEmpty(jsonInput))
        {
            PrintUsage();
            return 1;
        }

        try
        {
            var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
            var request = JsonSerializer.Deserialize<CardOperationRequest>(jsonInput, options);

            if (request == null)
            {
                WriteResponse(new CardOperationResponse(false, null, "Invalid JSON input"));
                return 1;
            }

            var response = request.Operation.ToLowerInvariant() switch
            {
                "readcardid" => RunReadCardId(),
                "readcard" => RunReadCard(),
                "initcard" => RunInitCard(request),
                "recyclecard" => RunRecycleCard(request),
                _ => new CardOperationResponse(false, null, $"Unknown operation: {request.Operation}")
            };

            WriteResponse(response);
            return response.Success ? 0 : 1;
        }
        catch (Exception ex)
        {
            WriteResponse(new CardOperationResponse(false, null, ex.Message));
            return 1;
        }
    }

    static CardOperationResponse RunReadCardId()
    {
        using var svc = new YcCardService();
        if (!svc.Open())
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        bool ok = svc.ReadPhysicalCardId(out uint cardId, out int tagType);
        if (!ok)
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        return new CardOperationResponse(true, new
        {
            cardId,
            tagType,
            tagTypeName = GetTagTypeName(tagType)
        }, $"Card ID: {cardId}");
    }

    static CardOperationResponse RunReadCard()
    {
        using var svc = new YcCardService();
        if (!svc.Open())
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        bool ok = svc.ReadCard(out var info);
        if (!ok)
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        return new CardOperationResponse(true, new
        {
            info.CardType,
            info.CardTypeName,
            info.OptNum,
            info.CardID,
            info.UserNo,
            info.UserType,
            info.CardSerno,
            Value = info.Value1 / 100m,
            LastPay = info.LastPay1 / 100m,
            info.Count1,
            info.ConsumeAdd1,
            BackupValue = info.Value2 / 100m,
            UseTerm = DecodeTerm(info.UseTerm),
            info.AddCount,
            EmpId = info.EmpId,
            EmpName = info.EmpName,
            WaterValue = info.JsValue / 100m,
            WaterCount = info.JsCount
        }, $"Card read OK. ID: {info.CardSerno}");
    }

    static CardOperationResponse RunInitCard(CardOperationRequest request)
    {
        var config = request.InitConfig ?? new InitCardRequest();

        using var svc = new YcCardService();
        if (!svc.Open())
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        bool ok = svc.InitCard(
            config.CardID,
            config.EmpStrID ?? "0",
            config.CardTypeID ?? 1,
            config.UseTerm ?? 0,
            config.EmpName ?? "",
            config.CardTypeName ?? "",
            (int)Math.Round((config.XCardValue ?? 0) * 100),
            (int)Math.Round((config.WCardValue ?? 0) * 100),
            config.CreditValue ?? true,
            config.IssueType ?? 0,
            out uint cardSerno);

        if (!ok)
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        return new CardOperationResponse(true, new { cardSerno }, $"Card initialized. ID: {cardSerno}");
    }

    static CardOperationResponse RunRecycleCard(CardOperationRequest request)
    {
        using var svc = new YcCardService();
        if (!svc.Open())
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        uint cardSerno = request.CardSerno ?? 0;
        if (cardSerno == 0)
        {
            // Try to read card ID first
            bool idOk = svc.ReadPhysicalCardId(out cardSerno, out _);
            if (!idOk)
            {
                return new CardOperationResponse(false, null, "No card detected and no cardSerno provided");
            }
        }

        bool ok = svc.RecycleCard(cardSerno);
        if (!ok)
        {
            return new CardOperationResponse(false, null, svc.LastErrorMessage);
        }

        return new CardOperationResponse(true, new { cardSerno }, $"Card {cardSerno} recycled");
    }

    static string GetTagTypeName(int tagType) => tagType switch
    {
        0x01 => "S50/Mifare 1K",
        0x02 => "S70/Mifare 4K",
        0x04 => "Mifare 1",
        _ => $"0x{tagType:X2}"
    };

    static string DecodeTerm(int useTerm)
    {
        if (useTerm == 0) return "N/A";
        // Format: yymmdd → yyyy-mm-dd
        int yy = useTerm / 10000 + 2000;
        int mm = (useTerm % 10000) / 100;
        int dd = useTerm % 100;
        return $"{yy:D4}-{mm:D2}-{dd:D2}";
    }

    static void WriteResponse(CardOperationResponse response)
    {
        var json = JsonSerializer.Serialize(response, new JsonSerializerOptions
        {
            WriteIndented = true,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull
        });
        Console.WriteLine(json);
    }

    static void PrintUsage()
    {
        Console.WriteLine(@"M30Demo Card Operations CLI
=================================
Usage: M30Demo.Cli.exe --json='<JSON>'

Operations:
  ReadCardId    - Read physical card ID
  ReadCard      - Read full card data (consume + water)
  InitCard      - Initialize / issue card
  RecycleCard   - Reset / recycle card

Examples:
  ReadCardId:
    --json='{""operation"":""ReadCardId""}'

  ReadCard:
    --json='{""operation"":""ReadCard""}'

  InitCard:
    --json='{
      ""operation"":""InitCard"",
      ""initConfig"": {
        ""cardID"": 10001,
        ""empStrID"": ""U001"",
        ""cardTypeID"": 1,
        ""useTerm"": 271231,
        ""empName"": ""张三"",
        ""cardTypeName"": ""一类卡"",
        ""xCardValue"": 100.00,
        ""wCardValue"": 50.00,
        ""creditValue"": true,
        ""issueType"": 0
      }
    }'

  RecycleCard:
    --json='{""operation"":""RecycleCard"",""cardSerno"":12345678}'
");
    }
}

public record CardOperationRequest
{
    [JsonPropertyName("operation")]
    public string Operation { get; init; } = string.Empty;

    [JsonPropertyName("cardSerno")]
    public uint? CardSerno { get; init; }

    [JsonPropertyName("initConfig")]
    public InitCardRequest? InitConfig { get; init; }
}

public record InitCardRequest
{
    [JsonPropertyName("cardID")]
    public int CardID { get; init; }

    [JsonPropertyName("empStrID")]
    public string? EmpStrID { get; init; }

    [JsonPropertyName("cardTypeID")]
    public int? CardTypeID { get; init; }

    [JsonPropertyName("useTerm")]
    public int? UseTerm { get; init; }

    [JsonPropertyName("empName")]
    public string? EmpName { get; init; }

    [JsonPropertyName("cardTypeName")]
    public string? CardTypeName { get; init; }

    [JsonPropertyName("xCardValue")]
    public decimal? XCardValue { get; init; }

    [JsonPropertyName("wCardValue")]
    public decimal? WCardValue { get; init; }

    [JsonPropertyName("creditValue")]
    public bool? CreditValue { get; init; }

    [JsonPropertyName("issueType")]
    public int? IssueType { get; init; }
}

public record CardOperationResponse
{
    public bool Success { get; }
    public object? Data { get; }
    public string? ErrorMessage { get; }
    public int ErrorCode { get; }
    public long Timestamp { get; }

    public CardOperationResponse(bool success, object? data, string? errorMessage = null)
    {
        Success = success;
        Data = data;
        ErrorMessage = errorMessage;
        ErrorCode = success ? 0 : -1;
        Timestamp = DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
    }
}

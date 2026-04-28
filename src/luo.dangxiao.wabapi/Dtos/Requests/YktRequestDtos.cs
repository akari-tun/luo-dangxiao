using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.wabapi.Dtos.Requests
{
    public sealed class DynamicRequestDto
    {
        [JsonExtensionData]
        public Dictionary<string, JsonElement>? AdditionalData { get; set; }
    }

    public sealed class TeacherRechargeQrCodeRequestDto
    {
        public string UserId { get; set; } = string.Empty;

        public decimal DealValue { get; set; }

        public string WorkStationNumb { get; set; } = string.Empty;
    }

    public sealed class WechatRechargeResultRequestDto
    {
        public string OrderSn { get; set; } = string.Empty;
    }

    public sealed class TraineeRegisterRequestDto
    {
        public string UserId { get; set; } = string.Empty;

        public int CheckinState { get; set; }

        public string RoomCode { get; set; } = string.Empty;

        public string DeptId { get; set; } = string.Empty;
    }

    public sealed class CardInitRequestDto
    {
        public string CardId { get; set; } = string.Empty;

        public string UserId { get; set; } = string.Empty;

        public string CardTypeId { get; set; } = string.Empty;

        public string ExpiryDate { get; set; } = string.Empty;

        public string FactoryFixId { get; set; } = string.Empty;

        public string MainDeputyType { get; set; } = string.Empty;

        public string CardNo { get; set; } = string.Empty;

        public string CardOperate { get; set; } = string.Empty;

        public string WorkStationNumb { get; set; } = string.Empty;

        public string TenantId { get; set; } = string.Empty;

        public string? OldCardNo { get; set; }

        public string? OldFactoryFixId { get; set; }

        public string? OldCardId { get; set; }
    }

    public sealed class CardOperateRequestDto
    {
        public string CardNo { get; set; } = string.Empty;

        public string FactoryFixId { get; set; } = string.Empty;

        public string OperatorId { get; set; } = string.Empty;

        public string TenantId { get; set; } = string.Empty;
    }
}

using System.Text.Json;
using System.Text.Json.Serialization;

namespace luo.dangxiao.wabapi.Dtos.Responses
{
    public class ApiResponseDto<TData>
    {
        public int? Code { get; set; }

        public string? Message { get; set; }

        public TData? Data { get; set; }

        [JsonExtensionData]
        public Dictionary<string, JsonElement>? AdditionalData { get; set; }
    }

    public sealed class SendSmsResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TeacherByMobileResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TeacherByIdentityResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TeacherRechargeQrCodeResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class WechatRechargeResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class WechatRechargeResultResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TraineeByMobileResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TraineeByIdentityResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class TraineeRegisterResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class CardInitResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class WriteCardSuccessResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class WriteCardFailureResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class LockCardResponseDto : ApiResponseDto<JsonElement?>
    {
    }

    public sealed class RecycleCardResponseDto : ApiResponseDto<JsonElement?>
    {
    }
}

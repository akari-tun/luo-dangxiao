using luo.dangxiao.wabapi.Dtos.Requests;
using luo.dangxiao.wabapi.Dtos.Responses;
using System.Text;
using System.Text.Json;

namespace luo.dangxiao.wabapi.Clients
{
    public sealed class YktApiClient : IYktApiClient
    {
        private static readonly JsonSerializerOptions s_jsonOptions = new()
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            PropertyNameCaseInsensitive = true
        };

        private readonly HttpClient _httpClient;

        public YktApiClient(HttpClient httpClient)
        {
            _httpClient = httpClient;
        }

        public Task<SendSmsResponseDto> SendSmsAsync(DynamicRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<DynamicRequestDto, SendSmsResponseDto>("self/api/v1/sendSms", request, cancellationToken);
        }

        public Task<TeacherByMobileResponseDto> GetTeacherByMobileAsync(string mobile, CancellationToken cancellationToken = default)
        {
            return GetAsync<TeacherByMobileResponseDto>($"teacher/api/v1/account/mobile/{Uri.EscapeDataString(mobile)}", cancellationToken);
        }

        public Task<TeacherByIdentityResponseDto> GetTeacherByIdentityAsync(string identity, CancellationToken cancellationToken = default)
        {
            return GetAsync<TeacherByIdentityResponseDto>($"teacher/api/v1/account/identity/{Uri.EscapeDataString(identity)}", cancellationToken);
        }

        public Task<TeacherRechargeQrCodeResponseDto> GetTeacherRechargeQrCodeAsync(TeacherRechargeQrCodeRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<TeacherRechargeQrCodeRequestDto, TeacherRechargeQrCodeResponseDto>("teacher/api/v1/wechat/recharge/qrcode", request, cancellationToken);
        }

        public Task<WechatRechargeResponseDto> WechatRechargeAsync(string userId, decimal dealValue, string workStationNumb, string orderSn, CancellationToken cancellationToken = default)
        {
            return GetAsync<WechatRechargeResponseDto>($"teacher/recharge/{Uri.EscapeDataString(userId)}/{dealValue}/{Uri.EscapeDataString(workStationNumb)}/{Uri.EscapeDataString(orderSn)}", cancellationToken);
        }

        public Task<WechatRechargeResultResponseDto> QueryWechatRechargeResultAsync(WechatRechargeResultRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<WechatRechargeResultRequestDto, WechatRechargeResultResponseDto>("teacher/api/v1/wechat/recharge/result", request, cancellationToken);
        }

        public Task<TraineeByMobileResponseDto> GetTraineeByMobileAsync(string mobile, string checkInDate, CancellationToken cancellationToken = default)
        {
            return GetAsync<TraineeByMobileResponseDto>($"trainee/api/v1/mobile/{Uri.EscapeDataString(mobile)}/{Uri.EscapeDataString(checkInDate)}", cancellationToken);
        }

        public Task<TraineeByIdentityResponseDto> GetTraineeByIdentityAsync(string identity, string checkInDate, CancellationToken cancellationToken = default)
        {
            return GetAsync<TraineeByIdentityResponseDto>($"trainee/api/v1/identity/{Uri.EscapeDataString(identity)}/{Uri.EscapeDataString(checkInDate)}", cancellationToken);
        }

        public Task<TraineeRegisterResponseDto> RegisterTraineeAsync(TraineeRegisterRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<TraineeRegisterRequestDto, TraineeRegisterResponseDto>("trainee/api/v1/register", request, cancellationToken);
        }

        public Task<CardInitResponseDto> InitCardAsync(CardInitRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<CardInitRequestDto, CardInitResponseDto>("card/api/v1/init", request, cancellationToken);
        }

        public Task<WriteCardSuccessResponseDto> WriteCardSuccessAsync(DynamicRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<DynamicRequestDto, WriteCardSuccessResponseDto>("card/api/v1/write/success", request, cancellationToken);
        }

        public Task<WriteCardFailureResponseDto> WriteCardFailureAsync(DynamicRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<DynamicRequestDto, WriteCardFailureResponseDto>("card/api/v1/write/failure", request, cancellationToken);
        }

        public Task<LockCardResponseDto> LockCardAsync(CardOperateRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<CardOperateRequestDto, LockCardResponseDto>("card/api/v1/lock", request, cancellationToken);
        }

        public Task<RecycleCardResponseDto> RecycleCardAsync(CardOperateRequestDto request, CancellationToken cancellationToken = default)
        {
            return PostAsync<CardOperateRequestDto, RecycleCardResponseDto>("card/api/v1/recycle", request, cancellationToken);
        }

        private async Task<TResponse> GetAsync<TResponse>(string path, CancellationToken cancellationToken) where TResponse : class, new()
        {
            using HttpResponseMessage response = await _httpClient.GetAsync(path, cancellationToken);
            return await DeserializeResponseAsync<TResponse>(response, cancellationToken);
        }

        private async Task<TResponse> PostAsync<TRequest, TResponse>(string path, TRequest request, CancellationToken cancellationToken)
            where TResponse : class, new()
        {
            string requestJson = JsonSerializer.Serialize(request, s_jsonOptions);
            System.Diagnostics.Debug.WriteLine($"[YktApiClient] POST {path} => {requestJson}");

            using StringContent content = new(requestJson, System.Text.Encoding.UTF8, "application/json");
            using HttpResponseMessage response = await _httpClient.PostAsync(path, content, cancellationToken);
            return await DeserializeResponseAsync<TResponse>(response, cancellationToken);
        }

        private static async Task<TResponse> DeserializeResponseAsync<TResponse>(HttpResponseMessage response, CancellationToken cancellationToken)
            where TResponse : class, new()
        {
            string responseContent = await response.Content.ReadAsStringAsync(cancellationToken);
            response.EnsureSuccessStatusCode();

            if (string.IsNullOrWhiteSpace(responseContent))
            {
                return new TResponse();
            }

            TResponse? result = JsonSerializer.Deserialize<TResponse>(responseContent, s_jsonOptions);
            return result ?? new TResponse();
        }
    }
}

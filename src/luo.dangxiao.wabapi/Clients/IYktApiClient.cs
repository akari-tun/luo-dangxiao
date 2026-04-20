using luo.dangxiao.wabapi.Dtos.Requests;
using luo.dangxiao.wabapi.Dtos.Responses;

namespace luo.dangxiao.wabapi.Clients
{
    public interface IYktApiClient
    {
        Task<SendSmsResponseDto> SendSmsAsync(DynamicRequestDto request, CancellationToken cancellationToken = default);

        Task<TeacherByMobileResponseDto> GetTeacherByMobileAsync(string mobile, CancellationToken cancellationToken = default);

        Task<TeacherByIdentityResponseDto> GetTeacherByIdentityAsync(string identity, CancellationToken cancellationToken = default);

        Task<TeacherRechargeQrCodeResponseDto> GetTeacherRechargeQrCodeAsync(TeacherRechargeQrCodeRequestDto request, CancellationToken cancellationToken = default);

        Task<WechatRechargeResponseDto> WechatRechargeAsync(string userId, decimal dealValue, string workStationNumb, string orderSn, CancellationToken cancellationToken = default);

        Task<WechatRechargeResultResponseDto> QueryWechatRechargeResultAsync(WechatRechargeResultRequestDto request, CancellationToken cancellationToken = default);

        Task<TraineeByMobileResponseDto> GetTraineeByMobileAsync(string mobile, string checkInDate, CancellationToken cancellationToken = default);

        Task<TraineeByIdentityResponseDto> GetTraineeByIdentityAsync(string identity, string checkInDate, CancellationToken cancellationToken = default);

        Task<TraineeRegisterResponseDto> RegisterTraineeAsync(TraineeRegisterRequestDto request, CancellationToken cancellationToken = default);

        Task<CardInitResponseDto> InitCardAsync(CardInitRequestDto request, CancellationToken cancellationToken = default);

        Task<WriteCardSuccessResponseDto> WriteCardSuccessAsync(DynamicRequestDto request, CancellationToken cancellationToken = default);

        Task<WriteCardFailureResponseDto> WriteCardFailureAsync(DynamicRequestDto request, CancellationToken cancellationToken = default);

        Task<LockCardResponseDto> LockCardAsync(CardOperateRequestDto request, CancellationToken cancellationToken = default);

        Task<RecycleCardResponseDto> RecycleCardAsync(CardOperateRequestDto request, CancellationToken cancellationToken = default);
    }
}

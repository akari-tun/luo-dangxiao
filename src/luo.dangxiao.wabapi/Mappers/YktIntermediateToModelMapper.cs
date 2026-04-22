using luo.dangxiao.models;
using luo.dangxiao.wabapi.Dtos.Responses;
using Riok.Mapperly.Abstractions;

namespace luo.dangxiao.wabapi.Mappers;

[Mapper]
public static partial class YktIntermediateToModelMapper
{
    [MapProperty(source: nameof(StaffUserInfoIntermediateDto.GenderRaw), target: nameof(StaffInfoModel.Gender))]
    [MapProperty(source: nameof(StaffUserInfoIntermediateDto.CardStatusRaw), target: nameof(StaffInfoModel.CardStatus))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.UserType))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.ConsumptionBalance))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.SubsidyBalance))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.CardBalance))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.ConsumptionBalance))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.CardBalanceFallback))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.SubsidyBalanceFallback))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.UserBags))]
    public static partial StaffInfoModel ToStaffInfoModel(StaffUserInfoIntermediateDto source);

    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.GenderRaw), target: nameof(StudentInfoModel.Gender))]
    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.CardStatusRaw), target: nameof(StudentInfoModel.CardStatus))]
    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.CheckInStatusRaw), target: nameof(StudentInfoModel.CheckInStatus))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.UserType))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.TrainingStartDate))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.TrainingEndDate))]
    public static partial StudentInfoModel ToStudentInfoModel(StudentUserInfoIntermediateDto source);

    private static string MapGenderRaw(string raw)
    {
        return raw.Trim().ToLowerInvariant() switch
        {
            "1" or "男" or "male" => "男",
            "2" or "女" or "female" => "女",
            _ => raw
        };
    }

    private static StaffCardStatus MapCardStatusRaw(string status)
    {
        return status.Trim().ToLowerInvariant() switch
        {
            "normal" or "1" or "正常" => StaffCardStatus.Normal,
            "pendingpickup" or "pending_pickup" or "2" or "待领取" => StaffCardStatus.PendingPickup,
            "lost" or "3" or "已挂失" => StaffCardStatus.Lost,
            "frozen" or "4" or "已冻结" => StaffCardStatus.Frozen,
            _ => StaffCardStatus.Normal
        };
    }

    private static StudentCardStatus MapCardStatusRawToStudentCardStatus(string status)
    {
        return status.Trim().ToLowerInvariant() switch
        {
            "normal" or "1" or "正常" => StudentCardStatus.Normal,
            "pendingpickup" or "pending_pickup" or "2" or "待领取" => StudentCardStatus.PendingPickup,
            "lost" or "3" or "已挂失" => StudentCardStatus.Lost,
            "unissued" or "0" or "未制卡" => StudentCardStatus.Unissued,
            _ => StudentCardStatus.PendingPickup
        };
    }

    private static StudentCheckInStatus MapCheckInStatusRaw(string status)
    {
        return status.Trim().ToLowerInvariant() switch
        {
            "checkedin" or "checked_in" or "1" or "已报到" => StudentCheckInStatus.CheckedIn,
            _ => StudentCheckInStatus.NotCheckedIn
        };
    }
}

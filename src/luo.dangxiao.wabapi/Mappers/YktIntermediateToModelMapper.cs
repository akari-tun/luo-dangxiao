using luo.dangxiao.models;
using luo.dangxiao.wabapi.Dtos.Responses;
using Riok.Mapperly.Abstractions;

namespace luo.dangxiao.wabapi.Mappers;

[Mapper]
public static partial class YktIntermediateToModelMapper
{
    [MapProperty(source: nameof(StaffUserInfoIntermediateDto.UserCards), target: nameof(StaffInfoModel.UserCards))]
    [MapProperty(source: nameof(StaffUserInfoIntermediateDto.GenderRaw), target: nameof(StaffInfoModel.Gender))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.UserType))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.ConsumptionBalance))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.SubsidyBalance))]
    [MapperIgnoreTarget(nameof(StaffInfoModel.CardBalance))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.ConsumptionBalance))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.CardBalanceFallback))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.SubsidyBalanceFallback))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.UserBags))]
    [MapperIgnoreSource(nameof(StaffUserInfoIntermediateDto.CardStatusRaw))]
    public static partial StaffInfoModel ToStaffInfoModel(StaffUserInfoIntermediateDto source);

    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.UserCards), target: nameof(StudentInfoModel.UserCards))]
    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.GenderRaw), target: nameof(StudentInfoModel.Gender))]
    [MapProperty(source: nameof(StudentUserInfoIntermediateDto.CheckInStatusRaw), target: nameof(StudentInfoModel.CheckInStatus))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.UserType))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.TrainingStartDate))]
    [MapperIgnoreTarget(nameof(StudentInfoModel.TrainingEndDate))]
    [MapperIgnoreSource(nameof(StudentUserInfoIntermediateDto.CardStatusRaw))]
    public static partial StudentInfoModel ToStudentInfoModel(StudentUserInfoIntermediateDto source);

    public static partial CardInfoModel ToCardInfoModel(UserCardIntermediateDto source);

    private static string MapGenderRaw(string raw)
    {
        return raw.Trim().ToLowerInvariant() switch
        {
            "1" or "男" or "male" => "男",
            "2" or "女" or "female" => "女",
            _ => raw
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

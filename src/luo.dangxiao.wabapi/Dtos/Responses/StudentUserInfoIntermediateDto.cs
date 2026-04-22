namespace luo.dangxiao.wabapi.Dtos.Responses;

public sealed class StudentUserInfoIntermediateDto
{
    public string Id { get; set; } = string.Empty;

    public string Name { get; set; } = string.Empty;

    public string IdCardNumber { get; set; } = string.Empty;

    public string GenderRaw { get; set; } = string.Empty;

    public string ClassName { get; set; } = string.Empty;

    public DateTime? CheckInStartTime { get; set; }

    public DateTime? CheckInEndTime { get; set; }

    public DateTime? TrainingStartDate { get; set; }

    public DateTime? TrainingEndDate { get; set; }

    public string CardNumber { get; set; } = string.Empty;

    public string CardStatusRaw { get; set; } = string.Empty;

    public string RoomName { get; set; } = string.Empty;

    public string RoomNumber { get; set; } = string.Empty;

    public string CheckInStatusRaw { get; set; } = string.Empty;

    public string PhotoUrl { get; set; } = string.Empty;
}

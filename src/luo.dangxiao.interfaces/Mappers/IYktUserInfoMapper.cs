using luo.dangxiao.models;
using System.Text.Json;

namespace luo.dangxiao.interfaces.Mappers;

public interface IYktUserInfoMapper
{
    StaffInfoModel MapStaff(JsonElement? data, string identity);

    StudentInfoModel MapStudent(JsonElement? data, string identity);
}

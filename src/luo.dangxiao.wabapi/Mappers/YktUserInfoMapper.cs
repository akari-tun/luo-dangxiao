using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.Mappers;
using luo.dangxiao.models;
using System.Globalization;
using System.Text.Json;

namespace luo.dangxiao.wabapi.Mappers;

public sealed class YktUserInfoMapper : IYktUserInfoMapper
{
    public StaffInfoModel MapStaff(JsonElement? data, string identity)
    {
        var root = GetApiDataObject(data);
        var userBase = GetNestedObjectOrDefault(root, "userBase");

        var cardBalanceByBag = ReadUserBagBalance(root, "1");
        var subsidyBalanceByBag = ReadUserBagBalance(root, "3");

        var cardBalance = cardBalanceByBag == 0m ? ReadDecimal(root, "cardBalance", "totalBalance") : cardBalanceByBag;
        var subsidyBalance = subsidyBalanceByBag == 0m ? ReadDecimal(userBase, "subsidyBalance", "subsidyAmt") : subsidyBalanceByBag;

        var consumptionBalance = ReadDecimal(userBase, "consumptionBalance", "consumeBalance", "balance", "amount");
        if (consumptionBalance == 0m && cardBalance > 0m)
        {
            consumptionBalance = cardBalance;
        }

        var hasUserCard = TryGetUserCard(root, out var userCard);

        return new StaffInfoModel
        {
            Id = ReadString(userBase, ReadString(root, identity, "userId", "id", "teacherId", "staffId"), "userId", "id"),
            Name = ReadString(userBase, ReadString(root, "测试职工", "name", "teacherName", "staffName", "xm"), "userXm", "name", "xm"),
            UserType = UserType.Staff,
            IdCardNumber = ReadString(userBase, identity, "idNumber"),
            Gender = ParseGender(ReadString(userBase, string.Empty, "userSexName", "userSex", "gender", "sex")),
            Department = ReadString(userBase, ReadString(root, string.Empty, "department", "deptName", "orgName"), "deptName", "department"),
            EmployeeNumber = ReadString(userBase, ReadString(root, string.Empty, "employeeNumber", "workNo", "jobNo", "teacherNo"), "userNo", "userNumb", "employeeNumber"),
            CardType = hasUserCard
                ? ReadString(userCard, ReadString(userBase, "教职工卡", "userCardTypeName", "cardTypeName"), "cardTypeName", "cardType")
                : ReadString(userBase, "教职工卡", "userCardTypeName", "cardTypeName"),
            CardNumber = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "cardNumber", "cardNo"), "cardNo", "cardNumber")
                : ReadString(root, string.Empty, "cardNumber", "cardNo"),
            CardStatus = ParseStaffCardStatus(hasUserCard
                ? ReadString(userCard, ReadString(userBase, string.Empty, "stateName", "state"), "cardStatusName", "cardStatus", "cardStatusId", "status", "state")
                : ReadString(userBase, string.Empty, "stateName", "state", "cardStatus", "status")),
            ConsumptionBalance = consumptionBalance,
            SubsidyBalance = subsidyBalance,
            CardBalance = cardBalance,
            CardIssueDate = hasUserCard
                ? ReadDateTime(userCard, "statusChangeTime", "cardIssueDate", "issueDate", "enableDate", "openDate")
                : ReadDateTime(userBase, "cardIssueDate", "issueDate"),
            CardExpiryDate = hasUserCard
                ? ReadDateTime(userCard, "expiryDate", "cardExpiryDate", "expireDate") ?? ReadDateTime(userBase, "userExpiryDate", "expiryDate")
                : ReadDateTime(userBase, "userExpiryDate", "expiryDate"),
            PhoneNumber = ReadString(userBase, ReadString(root, string.Empty, "phone", "mobile"), "mobilePhone", "mobile", "phone", "userNumb"),
            PhotoUrl = ReadString(userBase, ReadString(root, string.Empty, "photoUrl", "avatar"), "photoUrl", "avatar")
        };
    }

    public StudentInfoModel MapStudent(JsonElement? data, string identity)
    {
        var root = GetApiDataObject(data);
        var userBase = GetNestedObjectOrDefault(root, "userBase");
        var hasUserCard = TryGetUserCard(root, out var userCard);

        return new StudentInfoModel
        {
            Id = ReadString(userBase, ReadString(root, identity, "userId", "id", "traineeId", "studentId"), "userId", "id"),
            Name = ReadString(userBase, ReadString(root, "测试学员", "name", "traineeName", "studentName", "xm"), "userXm", "name", "xm"),
            UserType = UserType.Student,
            IdCardNumber = ReadString(userBase, identity, "idNumber"),
            Gender = ParseGender(ReadString(userBase, ReadString(root, string.Empty, "gender", "sex"), "userSexName", "userSex", "gender", "sex")),
            ClassName = ReadString(userBase, ReadString(root, string.Empty, "className", "trainingClassName", "classNm"), "collegeName", "className", "trainingClassName"),
            CheckInStartTime = ReadDateTime(root, "checkInStartTime", "checkinStartTime", "inStartTime"),
            CheckInEndTime = ReadDateTime(root, "checkInEndTime", "checkinEndTime", "inEndTime"),
            TrainingStartDate = ReadDateTime(root, "trainingStartDate", "startDate") ?? DateTime.Today,
            TrainingEndDate = ReadDateTime(root, "trainingEndDate", "endDate") ?? DateTime.Today,
            CardNumber = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "cardNumber", "cardNo"), "cardNo", "cardNumber")
                : ReadString(root, string.Empty, "cardNumber", "cardNo"),
            CardStatus = ParseStudentCardStatus(hasUserCard
                ? ReadString(userCard, ReadString(userBase, string.Empty, "stateName", "state"), "cardStatusName", "cardStatus", "cardStatusId", "status", "state")
                : ReadString(userBase, ReadString(root, string.Empty, "cardStatus", "status"), "stateName", "state", "cardStatus", "status")),
            RoomName = ReadString(root, string.Empty, "roomName", "roomNo", "roomNumber"),
            RoomNumber = ReadString(root, string.Empty, "roomNumber", "roomNo"),
            CheckInStatus = ParseStudentCheckInStatus(ReadString(root, string.Empty, "checkInStatus", "checkinState")),
            PhotoUrl = ReadString(userBase, ReadString(root, string.Empty, "photoUrl", "avatar"), "photoUrl", "avatar")
        };
    }

    private static JsonElement GetApiDataObject(JsonElement? data)
    {
        if (data is not JsonElement element || element.ValueKind is JsonValueKind.Null or JsonValueKind.Undefined)
        {
            throw new InvalidOperationException("接口未返回有效数据。");
        }

        if (element.ValueKind == JsonValueKind.Array)
        {
            foreach (var item in element.EnumerateArray())
            {
                if (item.ValueKind == JsonValueKind.Object)
                {
                    return item;
                }
            }

            throw new InvalidOperationException("接口返回数据格式不正确。");
        }

        if (element.ValueKind != JsonValueKind.Object)
        {
            throw new InvalidOperationException("接口返回数据格式不正确。");
        }

        return element;
    }

    private static string ReadString(JsonElement element, string defaultValue, params string[] propertyNames)
    {
        foreach (var propertyName in propertyNames)
        {
            if (!TryGetPropertyIgnoreCase(element, propertyName, out var property))
            {
                continue;
            }

            if (property.ValueKind == JsonValueKind.String)
            {
                return property.GetString() ?? defaultValue;
            }

            if (property.ValueKind is JsonValueKind.Number or JsonValueKind.True or JsonValueKind.False)
            {
                return property.ToString();
            }
        }

        return defaultValue;
    }

    private static decimal ReadDecimal(JsonElement element, params string[] propertyNames)
    {
        foreach (var propertyName in propertyNames)
        {
            if (!TryGetPropertyIgnoreCase(element, propertyName, out var property))
            {
                continue;
            }

            if (property.ValueKind == JsonValueKind.Number && property.TryGetDecimal(out var number))
            {
                return number;
            }

            if (property.ValueKind == JsonValueKind.String
                && decimal.TryParse(property.GetString(), NumberStyles.Any, CultureInfo.InvariantCulture, out var parsed))
            {
                return parsed;
            }
        }

        return 0m;
    }

    private static DateTime? ReadDateTime(JsonElement element, params string[] propertyNames)
    {
        foreach (var propertyName in propertyNames)
        {
            if (!TryGetPropertyIgnoreCase(element, propertyName, out var property))
            {
                continue;
            }

            if (property.ValueKind == JsonValueKind.String
                && DateTime.TryParse(property.GetString(), CultureInfo.InvariantCulture, DateTimeStyles.None, out var dt))
            {
                return dt;
            }
        }

        return null;
    }

    private static bool TryGetPropertyIgnoreCase(JsonElement element, string propertyName, out JsonElement value)
    {
        foreach (var property in element.EnumerateObject())
        {
            if (string.Equals(property.Name, propertyName, StringComparison.OrdinalIgnoreCase))
            {
                value = property.Value;
                return true;
            }
        }

        value = default;
        return false;
    }

    private static StaffCardStatus ParseStaffCardStatus(string status)
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

    private static StudentCardStatus ParseStudentCardStatus(string status)
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

    private static StudentCheckInStatus ParseStudentCheckInStatus(string status)
    {
        return status.Trim().ToLowerInvariant() switch
        {
            "checkedin" or "checked_in" or "1" or "已报到" => StudentCheckInStatus.CheckedIn,
            _ => StudentCheckInStatus.NotCheckedIn
        };
    }

    private static decimal ReadUserBagBalance(JsonElement element, string targetBagCode)
    {
        if (!TryGetPropertyIgnoreCase(element, "userBags", out var userBags) || userBags.ValueKind != JsonValueKind.Array)
        {
            return 0m;
        }

        foreach (var bag in userBags.EnumerateArray())
        {
            if (bag.ValueKind != JsonValueKind.Object)
            {
                continue;
            }

            var bagCode = ReadString(bag, string.Empty, "bagCode", "code").Trim();
            if (!string.Equals(bagCode, targetBagCode, StringComparison.OrdinalIgnoreCase))
            {
                continue;
            }

            return ReadDecimal(bag, "cardValue", "bagBalance", "balance", "amount", "money", "value", "bagAmount");
        }

        return 0m;
    }

    private static bool TryGetUserCard(JsonElement element, out JsonElement userCard)
    {
        if (TryGetPropertyIgnoreCase(element, "userCards", out var userCards))
        {
            if (userCards.ValueKind == JsonValueKind.Array)
            {
                foreach (var card in userCards.EnumerateArray())
                {
                    if (card.ValueKind == JsonValueKind.Object)
                    {
                        userCard = card;
                        return true;
                    }
                }
            }
            else if (userCards.ValueKind == JsonValueKind.Object)
            {
                userCard = userCards;
                return true;
            }
        }

        userCard = default;
        return false;
    }

    private static JsonElement GetNestedObjectOrDefault(JsonElement element, string propertyName)
    {
        if (TryGetPropertyIgnoreCase(element, propertyName, out var nested)
            && nested.ValueKind == JsonValueKind.Object)
        {
            return nested;
        }

        return element;
    }

    private static string ParseGender(string raw)
    {
        return raw.Trim().ToLowerInvariant() switch
        {
            "1" or "男" or "male" => "男",
            "2" or "女" or "female" => "女",
            _ => raw
        };
    }
}

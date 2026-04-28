using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.Mappers;
using luo.dangxiao.models;
using luo.dangxiao.wabapi.Dtos.Responses;
using System.Globalization;
using System.Text.Json;

namespace luo.dangxiao.wabapi.Mappers;

public sealed class YktUserInfoMapper : IYktUserInfoMapper
{
    public StaffInfoModel MapStaff(JsonElement? data, string identity)
    {
        var root = GetApiDataObject(data);
        var source = BuildStaffIntermediateDto(root, identity);
        return MapStaffInfo(source);
    }

    public StudentInfoModel MapStudent(JsonElement? data, string identity)
    {
        var root = GetApiDataObject(data);
        var source = BuildStudentIntermediateDto(root, identity);
        return MapStudentInfo(source);
    }

    private static StaffUserInfoIntermediateDto BuildStaffIntermediateDto(JsonElement root, string identity)
    {
        var userBase = GetNestedObjectOrDefault(root, "userBase");
        var hasUserCard = TryGetUserCard(root, out var userCard);

        return new StaffUserInfoIntermediateDto
        {
            Id = ReadString(userBase, ReadString(root, identity, "userId", "id", "teacherId", "staffId"), "userId", "id"),
            UserId = hasUserCard
                ? ReadString(userCard, ReadString(userBase, ReadString(root, identity, "userId", "id", "teacherId", "staffId"), "userId", "id"), "userId")
                : ReadString(userBase, ReadString(root, identity, "userId", "id", "teacherId", "staffId"), "userId", "id"),
            Name = ReadString(userBase, ReadString(root, "测试职工", "name", "teacherName", "staffName", "xm"), "userXm", "name", "xm"),
            IdCardNumber = ReadString(userBase, identity, "idNumber"),
            GenderRaw = ReadString(userBase, string.Empty, "userSexName", "userSex", "gender", "sex"),
            Department = ReadString(userBase, ReadString(root, string.Empty, "department", "deptName", "orgName"), "deptName", "department"),
            EmployeeNumber = ReadString(userBase, ReadString(root, string.Empty, "employeeNumber", "workNo", "jobNo", "teacherNo"), "userNo", "userNumb", "employeeNumber"),
            CardType = hasUserCard
                ? ReadString(userCard, ReadString(userBase, "教职工卡", "userCardTypeName", "cardTypeName"), "cardTypeName", "cardType")
                : ReadString(userBase, "教职工卡", "userCardTypeName", "cardTypeName"),
            CardNumber = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "cardNumber", "cardNo"), "cardNo", "cardNumber")
                : ReadString(root, string.Empty, "cardNumber", "cardNo"),
            FactoryFixId = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "factoryFixId"), "factoryFixId")
                : ReadString(root, string.Empty, "factoryFixId"),
            CardStatusRaw = hasUserCard
                ? ReadString(userCard, ReadString(userBase, string.Empty, "stateName", "state"), "cardStatusName", "cardStatus", "cardStatusId", "status", "state")
                : ReadString(userBase, string.Empty, "stateName", "state", "cardStatus", "status"),
            ConsumptionBalance = ReadDecimal(userBase, "consumptionBalance", "consumeBalance", "balance", "amount"),
            CardBalanceFallback = ReadDecimal(root, "cardBalance", "totalBalance"),
            SubsidyBalanceFallback = ReadDecimal(userBase, "subsidyBalance", "subsidyAmt"),
            CardIssueDate = hasUserCard
                ? ReadDateTime(userCard, "statusChangeTime", "cardIssueDate", "issueDate", "enableDate", "openDate")
                : ReadDateTime(userBase, "cardIssueDate", "issueDate"),
            CardExpiryDate = hasUserCard
                ? ReadDateTime(userCard, "expiryDate", "cardExpiryDate", "expireDate") ?? ReadDateTime(userBase, "userExpiryDate", "expiryDate")
                : ReadDateTime(userBase, "userExpiryDate", "expiryDate"),
            PhoneNumber = ReadString(userBase, ReadString(root, string.Empty, "phone", "mobile"), "mobilePhone", "mobile", "phone", "userNumb"),
            PhotoUrl = ReadString(userBase, ReadString(root, string.Empty, "photoUrl", "avatar"), "photoUrl", "avatar"),
            UserBags = ReadUserBags(root),
            UserCards = ReadUserCards(root)
        };
    }

    private static StudentUserInfoIntermediateDto BuildStudentIntermediateDto(JsonElement root, string identity)
    {
        var userBase = GetNestedObjectOrDefault(root, "userBase");
        var hasUserCard = TryGetUserCard(root, out var userCard);
        var userClass = GetNestedObjectOrDefault(root, "userClass");

        return new StudentUserInfoIntermediateDto
        {
            Id = ReadString(userBase, ReadString(root, identity, "userId", "id", "traineeId", "studentId"), "userId", "id"),
            UserId = hasUserCard
                ? ReadString(userCard, ReadString(userBase, ReadString(root, identity, "userId", "id", "traineeId", "studentId"), "userId", "id"), "userId")
                : ReadString(userBase, ReadString(root, identity, "userId", "id", "traineeId", "studentId"), "userId", "id"),
            Name = ReadString(userBase, ReadString(root, "测试学员", "name", "traineeName", "studentName", "xm"), "userXm", "name", "xm"),
            IdCardNumber = ReadString(userBase, identity, "idNumber"),
            GenderRaw = ReadString(userBase, ReadString(root, string.Empty, "gender", "sex"), "userSexName", "userSex", "gender", "sex"),
            ClassName = ReadString(userBase, ReadString(root, string.Empty, "className", "trainingClassName", "classNm", "department", "deptName", "orgName"), "collegeName", "className", "trainingClassName"),
            CheckInStartTime = ReadDateTime(root, "checkinDate", "checkInStartTime", "checkinStartTime", "inStartTime"),
            CheckInEndTime = ReadDateTime(root, "checkoutDate", "checkInEndTime", "checkinEndTime", "inEndTime"),
            TrainingStartDate = ReadDateTime(userClass, "beginDate", "startDate"),
            TrainingEndDate = ReadDateTime(userClass, "endDate"),
            CardNumber = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "cardNumber", "cardNo"), "cardNo", "cardNumber")
                : ReadString(root, string.Empty, "cardNumber", "cardNo"),
            FactoryFixId = hasUserCard
                ? ReadString(userCard, ReadString(root, string.Empty, "factoryFixId"), "factoryFixId")
                : ReadString(root, string.Empty, "factoryFixId"),
            CardStatusRaw = hasUserCard
                ? ReadString(userCard, ReadString(userBase, string.Empty, "stateName", "state"), "cardStatusName", "cardStatus", "cardStatusId", "status", "state")
                : ReadString(userBase, ReadString(root, string.Empty, "cardStatus", "status"), "stateName", "state", "cardStatus", "status"),
            RoomName = ReadString(root, string.Empty, "roomName", "roomNo", "roomNumber"),
            RoomNumber = ReadString(root, string.Empty, "roomNumber", "roomNo"),
            RoomCode = ReadString(root, string.Empty, "roomCode"),
            DeptId = ReadString(root, string.Empty, "deptId"),
            CheckInStatusRaw = ReadString(root, string.Empty, "checkInStatus", "checkinState"),
            PhotoUrl = ReadString(userBase, ReadString(root, string.Empty, "photoUrl", "avatar"), "photoUrl", "avatar"),
            UserCards = ReadUserCards(root)
        };
    }

    private static List<UserCardIntermediateDto> ReadUserCards(JsonElement element)
    {
        var result = new List<UserCardIntermediateDto>();

        if (!TryGetPropertyIgnoreCase(element, "userCards", out var userCards) || userCards.ValueKind != JsonValueKind.Array)
        {
            return result;
        }

        foreach (var card in userCards.EnumerateArray())
        {
            if (card.ValueKind != JsonValueKind.Object)
            {
                continue;
            }

            result.Add(new UserCardIntermediateDto
            {
                CardId = ReadString(card, string.Empty, "cardId"),
                CardNo = ReadString(card, string.Empty, "cardNo"),
                FactoryFixId = ReadString(card, string.Empty, "factoryFixId"),
                CardStatusName = ReadString(card, string.Empty, "cardStatusName"),
                CardStatusId = ReadInt(card, 0, "cardStatusId"),
                CardTypeName = ReadString(card, string.Empty, "cardTypeName"),
                ExpiryDate = ReadDateTime(card, "expiryDate"),
                StatusChangeTime = ReadDateTime(card, "statusChangeTime"),
                MainDeputyType = ReadInt(card, 0, "mainDeputyType"),
                MainDeputyTypeName = ReadString(card, string.Empty, "mainDeputyTypeName"),
                TenantId = ReadString(card, string.Empty, "tenantId"),
                Deposit = ReadDecimal(card, "deposit"),
                IssueFee = ReadDecimal(card, "issueFee")
            });
        }

        return result;
    }

    private static List<StaffUserBagIntermediateDto> ReadUserBags(JsonElement element)
    {
        var result = new List<StaffUserBagIntermediateDto>();

        if (!TryGetPropertyIgnoreCase(element, "userBags", out var userBags) || userBags.ValueKind != JsonValueKind.Array)
        {
            return result;
        }

        foreach (var bag in userBags.EnumerateArray())
        {
            if (bag.ValueKind != JsonValueKind.Object)
            {
                continue;
            }

            result.Add(new StaffUserBagIntermediateDto
            {
                BagCode = ReadString(bag, string.Empty, "bagCode", "code").Trim(),
                CardValue = ReadDecimal(bag, "cardValue", "bagBalance", "balance", "amount", "money", "value", "bagAmount")
            });
        }

        return result;
    }

    private static StaffInfoModel MapStaffInfo(StaffUserInfoIntermediateDto source)
    {
        var hasCardBalance = TryGetBagBalance(source.UserBags, "1", out var cardBalanceByBag);
        var hasSubsidyBalance = TryGetBagBalance(source.UserBags, "3", out var subsidyBalanceByBag);

        var cardBalance = hasCardBalance ? cardBalanceByBag : source.CardBalanceFallback;
        var subsidyBalance = hasSubsidyBalance ? subsidyBalanceByBag : source.SubsidyBalanceFallback;

        var consumptionBalance = source.ConsumptionBalance;
        if (consumptionBalance == 0m && cardBalance > 0m)
        {
            consumptionBalance = cardBalance;
        }

        var model = YktIntermediateToModelMapper.ToStaffInfoModel(source);
        model.UserType = UserType.Staff;
        model.ConsumptionBalance = consumptionBalance;
        model.SubsidyBalance = subsidyBalance;
        model.CardBalance = cardBalance;

        return model;
    }

    private static StudentInfoModel MapStudentInfo(StudentUserInfoIntermediateDto source)
    {
        var model = YktIntermediateToModelMapper.ToStudentInfoModel(source);
        model.UserType = UserType.Student;
        model.TrainingStartDate = source.TrainingStartDate ?? DateTime.Today;
        model.TrainingEndDate = source.TrainingEndDate ?? DateTime.Today;
        return model;
    }

    private static bool TryGetBagBalance(IEnumerable<StaffUserBagIntermediateDto> userBags, string targetBagCode, out decimal balance)
    {
        foreach (var bag in userBags)
        {
            if (!string.Equals(bag.BagCode, targetBagCode, StringComparison.OrdinalIgnoreCase))
            {
                continue;
            }

            balance = bag.CardValue;
            return true;
        }

        balance = 0m;
        return false;
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

    private static int ReadInt(JsonElement element, int defaultValue, params string[] propertyNames)
    {
        foreach (var propertyName in propertyNames)
        {
            if (!TryGetPropertyIgnoreCase(element, propertyName, out var property))
            {
                continue;
            }

            if (property.ValueKind == JsonValueKind.Number && property.TryGetInt32(out var number))
            {
                return number;
            }

            if (property.ValueKind == JsonValueKind.String
                && int.TryParse(property.GetString(), NumberStyles.Any, CultureInfo.InvariantCulture, out var parsed))
            {
                return parsed;
            }
        }

        return defaultValue;
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
}

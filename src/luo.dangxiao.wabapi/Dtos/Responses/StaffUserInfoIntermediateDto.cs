namespace luo.dangxiao.wabapi.Dtos.Responses;

public sealed class StaffUserInfoIntermediateDto
{
    public string Id { get; set; } = string.Empty;

    public string Name { get; set; } = string.Empty;

    public string IdCardNumber { get; set; } = string.Empty;

    public string GenderRaw { get; set; } = string.Empty;

    public string Department { get; set; } = string.Empty;

    public string EmployeeNumber { get; set; } = string.Empty;

    public string CardType { get; set; } = string.Empty;

    public string CardNumber { get; set; } = string.Empty;

    public string FactoryFixId { get; set; } = string.Empty;

    public string UserId { get; set; } = string.Empty;

    public string CardStatusRaw { get; set; } = string.Empty;

    public decimal ConsumptionBalance { get; set; }

    public decimal CardBalanceFallback { get; set; }

    public decimal SubsidyBalanceFallback { get; set; }

    public DateTime? CardIssueDate { get; set; }

    public DateTime? CardExpiryDate { get; set; }

    public string PhoneNumber { get; set; } = string.Empty;

    public string PhotoUrl { get; set; } = string.Empty;

    public List<StaffUserBagIntermediateDto> UserBags { get; set; } = [];
}

public sealed class StaffUserBagIntermediateDto
{
    public string BagCode { get; set; } = string.Empty;

    public decimal CardValue { get; set; }
}

using luo.dangxiao.common.Enums;

namespace luo.dangxiao.models;

/// <summary>
/// Base model for user information.
/// </summary>
public class UserInfoModel
{
    public string Id { get; set; } = string.Empty;

    public string Name { get; set; } = string.Empty;

    public UserType UserType { get; set; }

    public string IdCardNumber { get; set; } = string.Empty;

    public List<CardInfoModel> UserCards { get; set; } = [];

    /// <summary>
    /// Gets the card with the highest CardNo from UserCards. Null if no cards exist.
    /// </summary>
    public CardInfoModel? CurrentCard => UserCards.Count == 0
        ? null
        : UserCards.OrderByDescending(c => c.CardNo).First();
}

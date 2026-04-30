using luo.dangxiao.cardreader.Virtual;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader;
/// <summary>
/// Creates card reader implementations based on configured provider.
/// </summary>
public static class CardReaderFactory
{
    /// <summary>
    /// Creates a card reader implementation.
    /// </summary>
    /// <param name="provider">The configured reader provider enum value.</param>
    /// <returns>The matching card reader implementation.</returns>
    public static CardReaderBase Create(Enum? provider = null)
    {
        return provider?.ToString() switch
        {
            "Virtual" => new VirtualCardReader(),
            "Yc" => new YcCardReader(),
            "Unknown" or null or "" => new VirtualCardReader(),
            _ => new VirtualCardReader(),
        };
    }
}

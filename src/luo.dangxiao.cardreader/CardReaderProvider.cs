namespace luo.dangxiao.cardreader;

/// <summary>
/// Available card reader provider types for factory instantiation.
/// </summary>
public enum CardReaderProvider
{
    /// <summary>
    /// Unknown or unspecified provider.
    /// </summary>
    Unknown,

    /// <summary>
    /// Virtual/simulated card reader for development and testing.
    /// </summary>
    Virtual,

    /// <summary>
    /// YC (YuanChuang) hardware card reader.
    /// </summary>
    Yc,
}

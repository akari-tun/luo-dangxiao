namespace luo.dangxiao.idreader;

/// <summary>
/// Available ID reader provider types for factory instantiation.
/// </summary>
public enum IdReader
{
    /// <summary>
    /// Unknown or unspecified provider.
    /// </summary>
    Unknown,

    /// <summary>
    /// Virtual/simulated ID reader for development and testing.
    /// </summary>
    Virtual,

    /// <summary>
    /// Huashi CV100U hardware ID reader.
    /// </summary>
    HuashiCvr100U,
}

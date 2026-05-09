using System;
using luo.dangxiao.idreader.CVR100U;
using luo.dangxiao.idreader.Virtual;

namespace luo.dangxiao.idreader;

/// <summary>
/// Creates ID reader implementations based on configured provider.
/// </summary>
public static class IdReaderFactory
{
    /// <summary>
    /// Creates an ID reader implementation.
    /// </summary>
    /// <param name="provider">The configured reader provider enum value.</param>
    /// <returns>The matching ID reader implementation.</returns>
    public static IdReaderBase Create(Enum? provider = null)
    {
        return provider?.ToString() switch
        {
            "Virtual" => new VirtualIdReader(),
            "HuashiCvr100U" => new Cv100UIdReader(),
            "Unknown" or null or "" => new VirtualIdReader(),
            _ => new VirtualIdReader(),
        };
    }
}

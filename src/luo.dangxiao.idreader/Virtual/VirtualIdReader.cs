using System;

namespace luo.dangxiao.idreader.Virtual;

/// <summary>
/// Simulated ID reader for development and testing.
/// Returns deterministic mock data for all operations.
/// </summary>
public sealed class VirtualIdReader : IdReaderBase
{
    /// <summary>
    /// Gets the reader provider name.
    /// </summary>
    public override string ProviderName => "Virtual";

    /// <summary>
    /// Initializes the simulated reader.
    /// </summary>
    /// <returns>Always returns <see langword="true" /> to indicate success.</returns>
    public override bool Init()
    {
        return true;
    }

    /// <summary>
    /// Closes the simulated reader.
    /// </summary>
    public override void Close()
    {
    }

    /// <summary>
    /// Reads deterministic mock ID card data.
    /// </summary>
    /// <param name="data">The populated ID card data.</param>
    /// <returns>Always returns <see langword="true" />.</returns>
    public override bool ReadIdCard(out IdCardData data)
    {
        data = new IdCardData
        {
            Name = "张三",
            Gender = "男",
            Nation = "汉",
            Birthday = "19900101",
            IdNumber = "110101199001011234",
            Address = "北京市东城区长安街1号",
            Department = "北京市公安局东城分局",
            StartDate = "20200101",
            EndDate = "20400101",
            PhotoBytes = Array.Empty<byte>(),
            CertType = IdTypeEnum.ChineseId,
            Uid = "VIRT-UID-00001",
            SamId = "VIRT-SAM-00001",
        };

        return true;
    }

    /// <summary>
    /// Releases resources held by the simulated reader.
    /// </summary>
    public override void Dispose()
    {
        Close();
    }
}

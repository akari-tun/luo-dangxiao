using System;

namespace luo.dangxiao.idreader;

/// <summary>
/// Base abstraction for ID reader providers.
/// </summary>
public abstract class IdReaderBase : IDisposable
{
    /// <summary>
    /// Gets the reader provider name.
    /// </summary>
    public abstract string ProviderName { get; }

    /// <summary>
    /// Initializes the reader device.
    /// </summary>
    /// <returns>True when initialization succeeds.</returns>
    public abstract bool Init();

    /// <summary>
    /// Closes the reader device.
    /// </summary>
    public abstract void Close();

    /// <summary>
    /// Reads ID card data from the reader.
    /// </summary>
    /// <param name="data">The populated ID card data.</param>
    /// <returns>True when the read succeeds.</returns>
    public abstract bool ReadIdCard(out IdCardData data);

    /// <summary>
    /// Releases resources held by the reader.
    /// </summary>
    public virtual void Dispose()
    {
    }
}

namespace luo.dangxiao.log;

/// <summary>
/// Static accessor for obtaining NLog <see cref="NLog.ILogger"/> instances.
/// </summary>
public static class LoggerResolver
{
    /// <summary>
    /// Gets a logger for the specified name.
    /// </summary>
    public static NLog.ILogger GetLogger(string name) => NLog.LogManager.GetLogger(name);

    /// <summary>
    /// Gets a logger named after the specified type.
    /// </summary>
    public static NLog.ILogger GetLogger<T>() => GetLogger(typeof(T).FullName ?? typeof(T).Name);

    /// <summary>
    /// Gets a logger named after the specified type.
    /// </summary>
    public static NLog.ILogger GetLogger(Type type) => GetLogger(type.FullName ?? type.Name);
}

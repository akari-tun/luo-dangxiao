using System;

namespace luo.dangxiao.cardreader.Yc;

/// <summary>
/// Static class for logging debug messages from the reader library
/// </summary>
public static class ReaderLogger
{
    /// <summary>
    /// Event raised when a log message is written
    /// </summary>
    public static event EventHandler<ReaderLogEventArgs>? LogMessageWritten;

    /// <summary>
    /// Writes a log message
    /// </summary>
    public static void Log(string message)
    {
        LogMessageWritten?.Invoke(null, new ReaderLogEventArgs(message));
    }

    /// <summary>
    /// Writes a formatted log message
    /// </summary>
    public static void Log(string format, params object[] args)
    {
        Log(string.Format(format, args));
    }
}

/// <summary>
/// Event arguments for log messages
/// </summary>
public class ReaderLogEventArgs : EventArgs
{
    public string Message { get; }
    public DateTime Timestamp { get; }

    public ReaderLogEventArgs(string message)
    {
        Message = message;
        Timestamp = DateTime.Now;
    }
}

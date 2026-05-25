using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using luo.dangxiao.cardreader.app.ViewModels;

namespace luo.dangxiao.cardreader.app.Services;

/// <summary>
/// Service to capture debug output from the reader library and forward it to the UI
/// </summary>
public static class DebugLogService
{
    private static MainViewModel? _mainViewModel;
    private static readonly object LockObj = new();
    private static StringWriter? _stringWriter;
    private static TextWriterTraceListener? _traceListener;

    public static void Initialize(MainViewModel mainViewModel)
    {
        _mainViewModel = mainViewModel;
        
        // Create a custom trace listener that forwards to the ViewModel
        _stringWriter = new StringWriter();
        _traceListener = new TextWriterTraceListener(_stringWriter);
        Trace.Listeners.Add(_traceListener);
    }

    public static void Log(string message)
    {
        if (_mainViewModel != null)
        {
            lock (LockObj)
            {
                _mainViewModel.AppendLog(message);
            }
        }
    }

    public static void Log(string format, params object[] args)
    {
        Log(string.Format(format, args));
    }
}

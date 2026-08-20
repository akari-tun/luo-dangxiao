using NLog.Config;
using NLog.Layouts;
using NLog.Targets;

namespace luo.dangxiao.log;

/// <summary>
/// NLog configuration helper.
/// Loads the application nlog.config file when it is available, with a programmatic fallback.
/// </summary>
public static class NLogConfig
{
    private const string DefaultLogFile = "logs/app.log";

    public static readonly Layout SimpleLayout = "${longdate}|${level:uppercase=true}|${logger}|${message}${onexception:${newline}${exception:format=toString}}";

    public static void Setup(string? logFilePath = null)
    {
        if (logFilePath is null)
        {
            var configFilePath = Path.Combine(AppContext.BaseDirectory, "nlog.config");
            if (File.Exists(configFilePath))
            {
                NLog.LogManager.Configuration = new XmlLoggingConfiguration(configFilePath);
                return;
            }
        }

        var config = new LoggingConfiguration();

        var layout = SimpleLayout;

        var consoleTarget = new ColoredConsoleTarget("console")
        {
            Layout = layout,
        };

        var logPath = logFilePath ?? DefaultLogFile;
        var fileTarget = new FileTarget("file")
        {
            FileName = logPath,
            Layout = layout,
            ArchiveFileName = Path.Combine(Path.GetDirectoryName(logPath) ?? ".", "app.{#}.log"),
            ArchiveSuffixFormat = "yyyyMMdd",
            ArchiveEvery = FileArchivePeriod.Day,
            MaxArchiveFiles = 30,
            KeepFileOpen = true,
            Encoding = System.Text.Encoding.UTF8,
        };

        config.AddTarget(consoleTarget);
        config.AddTarget(fileTarget);

        config.AddRule(NLog.LogLevel.Debug, NLog.LogLevel.Info, consoleTarget);
        config.AddRule(NLog.LogLevel.Info, NLog.LogLevel.Fatal, fileTarget);

        NLog.LogManager.Configuration = config;
    }
}

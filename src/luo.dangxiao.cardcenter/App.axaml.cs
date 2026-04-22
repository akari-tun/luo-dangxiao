using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;
using luo.dangxiao.cardcenter.ViewModels;
using luo.dangxiao.cardcenter.Views;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using System.Diagnostics;
using System.Linq;

namespace luo.dangxiao.cardcenter
{
    public partial class App : Application
    {
        /// <summary>
        /// Gets the configured card printer instance.
        /// </summary>
        public static CardPrinterBase? CardPrinter { get; private set; }

        private string? _providerWarningMessage;

        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);

            var cfgData = ConfigModel.Load<CardCenterConfig>();
            cfgData.PrinterConfig.RawProviderValue = PrinterProviderJsonConverter.LastInvalidValue ?? string.Empty;
            var provider = cfgData.PrinterConfig.ResolveProvider(out var providerWarning);
            if (providerWarning is not null)
            {
                _providerWarningMessage = string.Format(
                    LanguageProvider.Msg_PrinterProviderFallback,
                    string.IsNullOrWhiteSpace(providerWarning.InvalidProviderValue) ? "Unknown" : providerWarning.InvalidProviderValue,
                    providerWarning.ResolvedProvider);
                Trace.TraceWarning(_providerWarningMessage);
            }

            CardPrinter = CardPrinterFactory.Create(provider);
            if (Current is { } app)
            {
                app.RequestedThemeVariant = ResolveThemeVariant(cfgData.Theme);
            }
        }

        public override void OnFrameworkInitializationCompleted()
        {
            if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                // Avoid duplicate validations from both Avalonia and the CommunityToolkit. 
                // More info: https://docs.avaloniaui.net/docs/guides/development-guides/data-validation#manage-validationplugins
                DisableAvaloniaDataAnnotationValidation();
                var mainVm = new MainWindowViewModel();
                if (!string.IsNullOrWhiteSpace(_providerWarningMessage))
                {
                    mainVm.StartupWarningMessage = _providerWarningMessage;
                }
                
                desktop.MainWindow = new MainWindow
                {
                    DataContext = mainVm,
                };
            }

            base.OnFrameworkInitializationCompleted();
        }

        private void DisableAvaloniaDataAnnotationValidation()
        {
            // Get an array of plugins to remove
            var dataValidationPluginsToRemove =
                BindingPlugins.DataValidators.OfType<DataAnnotationsValidationPlugin>().ToArray();

            // remove each entry found
            foreach (var plugin in dataValidationPluginsToRemove)
            {
                BindingPlugins.DataValidators.Remove(plugin);
            }
        }

        private static ThemeVariant ResolveThemeVariant(string? theme)
        {
            return theme?.Trim().ToLowerInvariant() switch
            {
                "dark" => ThemeVariant.Dark,
                "default" => ThemeVariant.Default,
                _ => ThemeVariant.Light
            };
        }
    }
}

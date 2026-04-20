using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;
using luo.dangxiao.cardcenter.ViewModels;
using luo.dangxiao.cardcenter.Views;
using luo.dangxiao.models;
using System.Linq;

namespace luo.dangxiao.cardcenter
{
    public partial class App : Application
    {
        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);

            var cfgData = ConfigModel.Load<CardCenterConfig>();
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
                desktop.MainWindow = new MainWindow
                {
                    DataContext = new MainWindowViewModel(),
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
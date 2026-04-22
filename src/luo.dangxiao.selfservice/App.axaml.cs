using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.Mappers;
using luo.dangxiao.models;
using luo.dangxiao.printer;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.selfservice.ViewModels;
using luo.dangxiao.selfservice.Views;
using luo.dangxiao.wabapi.Extensions;
using luo.dangxiao.wabapi.Mappers;
using Microsoft.Extensions.DependencyInjection;
using System.Diagnostics;
using System.Linq;

namespace luo.dangxiao.selfservice
{
    public partial class App : Application
    {
        private string? _providerWarningMessage;

        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);

            var cfgData = ConfigModel.Load<SelfServiceConfig>();
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

            var cardPrinter = CardPrinterFactory.Create(provider);

            IServiceCollection serviceCollection = new ServiceCollection()
                .AddSingleton(cfgData)
                .AddSingleton<ConfigModel>(cfgData)
                .AddSingleton(cfgData.PrinterConfig)
                .AddSingleton(cardPrinter)
                .AddSingleton<CardPrinterBase>(cardPrinter)
                .AddSingleton<IYktUserInfoMapper, YktUserInfoMapper>()
                .AddSingleton<MainWindowViewModel>()
                .AddSingleton<HomePageViewModel>()
                .AddTransient<VerifyPageViewModel>()
                .AddTransient<IDCardVerifyPageViewModel>()
                .AddTransient<SMSVerifyPageViewModel>()
                .AddTransient<UserInfoPageViewModel>()
                .AddTransient<CheckInPageViewModel>()
                .AddTransient<TakeCardPageViewModel>()
                .AddTransient<ReportLossPageViewModel>()
                .AddTransient<StudentInfoPageViewModel>()
                .AddTransient<StaffInfoPageViewModel>()
                .AddTransient<ReplacementPageViewModel>()
                .AddTransient<RechargePageViewModel>();

            if (!string.IsNullOrWhiteSpace(cfgData.YktApiConfig.BaseUrl))
            {
                serviceCollection.AddYktWabApi(cfgData.YktApiConfig.BaseUrl, cfgData.YktApiConfig.TimeoutSeconds);
            }

            Ioc.Default.ConfigureServices(serviceCollection.BuildServiceProvider());

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
                var mainVm = Ioc.Default.GetRequiredService<MainWindowViewModel>();
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

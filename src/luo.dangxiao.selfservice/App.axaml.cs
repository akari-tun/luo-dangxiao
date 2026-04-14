using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.ViewModels;
using luo.dangxiao.selfservice.Views;
using Microsoft.Extensions.DependencyInjection;
using System.ComponentModel.Design;
using System.Linq;

namespace luo.dangxiao.selfservice
{
    public partial class App : Application
    {
        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);

            IServiceCollection serviceCollection = new ServiceCollection()
                .AddSingleton<MainWindowViewModel>()
                .AddSingleton<HomePageViewModel>()
                .AddSingleton<CfgDataModel>()
                .AddTransient<VerifyPageViewModel>()
                .AddTransient<IDCardVerifyPageViewModel>()
                .AddTransient<SMSVerifyPageViewModel>()
                .AddTransient<StudentInfoPageViewModel>()
                .AddTransient<StaffInfoPageViewModel>();

            Ioc.Default.ConfigureServices(serviceCollection.BuildServiceProvider());

            var cfgData = Ioc.Default.GetRequiredService<CfgDataModel>();
            if (Current is { } app)
            {
                app.RequestedThemeVariant = cfgData.Theme;
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
    }
}
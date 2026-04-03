using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for SMSVerifyPage module.
/// </summary>
public partial class SMSVerifyPageView : UserControl, IPageView
{
    SMSVerifyPageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public SMSVerifyPageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<SMSVerifyPageViewModel>();
        DataContext = _viewModel;
    }
}

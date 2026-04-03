using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for IDCardVerifyPage module.
/// </summary>
public partial class IDCardVerifyPageView : UserControl, IPageView
{
    IDCardVerifyPageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public IDCardVerifyPageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<IDCardVerifyPageViewModel>();
        DataContext = _viewModel;
    }
}

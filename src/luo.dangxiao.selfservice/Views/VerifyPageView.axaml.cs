using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for VerifyPage.
/// </summary>
public partial class VerifyPageView : UserControl, IPageView
{
    VerifyPageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public VerifyPageView(): this("TakeCard") {}

    public VerifyPageView(string targetFunction)
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<VerifyPageViewModel>();
        _viewModel.TargetFunction = targetFunction;
        DataContext = _viewModel;
    }
}

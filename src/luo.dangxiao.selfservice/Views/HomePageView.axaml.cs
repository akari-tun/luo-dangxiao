using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for HomePage
/// </summary>
public partial class HomePageView : UserControl
{
    HomePageViewModel _viewModel;

    public HomePageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<HomePageViewModel>();
        DataContext = _viewModel;
    }
}

using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for StaffInfoPage module.
/// </summary>
public partial class StaffInfoPageView : UserControl, IPageView
{
    private readonly StaffInfoPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public StaffInfoPageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<StaffInfoPageViewModel>();
        DataContext = _viewModel;
    }
}

using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for CheckInPage.
/// </summary>
public partial class CheckInPageView : UserControl, IPageView
{
    private readonly CheckInPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public CheckInPageView() : this(new CheckInPageParameter())
    {
    }

    public CheckInPageView(CheckInPageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<CheckInPageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}

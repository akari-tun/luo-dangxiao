using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for TakeCardPage.
/// </summary>
public partial class TakeCardPageView : UserControl, IPageView
{
    private readonly TakeCardPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public TakeCardPageView() : this(new TakeCardPageParameter())
    {
    }

    public TakeCardPageView(TakeCardPageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<TakeCardPageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}

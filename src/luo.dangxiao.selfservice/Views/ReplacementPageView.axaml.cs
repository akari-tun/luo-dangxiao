using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for ReplacementPage.
/// </summary>
public partial class ReplacementPageView : UserControl, IPageView
{
    private readonly ReplacementPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public ReplacementPageView() : this(new ReplacementPageParameter())
    {
    }

    public ReplacementPageView(ReplacementPageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<ReplacementPageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}
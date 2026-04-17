using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for ReportLossPage.
/// </summary>
public partial class ReportLossPageView : UserControl, IPageView
{
    private readonly ReportLossPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public ReportLossPageView() : this(new ReportLossPageParameter())
    {
    }

    public ReportLossPageView(ReportLossPageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<ReportLossPageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}

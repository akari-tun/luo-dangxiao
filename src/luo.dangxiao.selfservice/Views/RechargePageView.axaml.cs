using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for RechargePage.
/// </summary>
public partial class RechargePageView : UserControl, IPageView
{
    private readonly RechargePageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public RechargePageView() : this(new RechargePageParameter())
    {
    }

    public RechargePageView(RechargePageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<RechargePageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}

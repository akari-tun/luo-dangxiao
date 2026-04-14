using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for StudentInfoPage module.
/// </summary>
public partial class StudentInfoPageView : UserControl, IPageView
{
    private readonly StudentInfoPageViewModel _viewModel;

    public IPageViewModel ViewModel => _viewModel;

    public StudentInfoPageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<StudentInfoPageViewModel>();
        DataContext = _viewModel;
    }
}

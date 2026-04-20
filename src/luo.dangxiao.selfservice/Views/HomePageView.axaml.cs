using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for HomePage
/// </summary>
public partial class HomePageView : UserControl, IPageView
{
    HomePageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public HomePageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<HomePageViewModel>();
        var cfgData = Ioc.Default.GetRequiredService<SelfServiceConfig>();

        switch (cfgData.ServiceType)
        {
            case common.Enums.SelfServiceType.StaffSelfService:
                _viewModel.SelfServiceType = cfgData.ServiceType;
                break;
            case common.Enums.SelfServiceType.StudentSelfService:
                _viewModel.SelfServiceType = cfgData.ServiceType;
                break;
        }
        
        DataContext = _viewModel;
    }
}

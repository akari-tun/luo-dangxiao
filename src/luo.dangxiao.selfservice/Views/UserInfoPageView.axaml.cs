using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.ViewModels;
namespace luo.dangxiao.selfservice.Views;

public partial class UserInfoPageView : UserControl, IPageView
{
    private readonly UserInfoPageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public UserInfoPageView() : this(new UserInfoPageParameter())
    {

    }

    public UserInfoPageView(UserInfoModel userInfo, string targetFunction = "") : this(new UserInfoPageParameter { Data = userInfo, TargetFunction = targetFunction })
    {

    }

    public UserInfoPageView(UserInfoPageParameter parameter)
    {
        InitializeComponent();
        _viewModel = Ioc.Default.GetRequiredService<UserInfoPageViewModel>();
        _viewModel.LoadDataCommand.Execute(parameter);
        DataContext = _viewModel;
    }
}
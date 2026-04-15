using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.interfaces.ViewModels;

namespace luo.dangxiao.selfservice.ViewModels
{
    public abstract partial class ViewModelBase : ObservableObject, IPageViewModel
    {
        [RelayCommand]
        protected virtual void Back()
        {
            Ioc.Default.GetRequiredService<HomePageViewModel>().ReturnHome();
        }
    }
}

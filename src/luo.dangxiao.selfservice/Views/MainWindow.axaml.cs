using Avalonia.Controls;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.ViewModels;

namespace luo.dangxiao.selfservice.Views
{
    public partial class MainWindow : Window
    {
        MainWindowViewModel _viewModel;

        public MainWindow()
        {
            InitializeComponent();
            _viewModel = Ioc.Default.GetRequiredService<MainWindowViewModel>();
            DataContext = _viewModel;
        }
    }
}
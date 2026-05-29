using System;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class SysCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private string _userPassword = "password";
    [ObservableProperty] private int _systemTypeIndex = 0;
    [ObservableProperty] private int _useSector = 5;
    [ObservableProperty] private string _commPassword = string.Empty;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public SysCardViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void InitSysCard()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int sysType = SystemTypeIndex + 1;
            
            int result = _main.Consumption.InitSysCard(UserPassword, sysType, UseSector, out byte[] commPassword);
            
            CommPassword = BitConverter.ToString(commPassword).Replace("-", "");
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

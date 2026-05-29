using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class BalanceViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private uint _balanceXf;
    [ObservableProperty] private uint _balanceJs;
    [ObservableProperty] private uint _transferValue;
    [ObservableProperty] private uint _transferFlag = 1;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public BalanceViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ReadBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadPosBalance(
                out uint balanceXf, out uint balanceJs, CardSerno, WaitTime);
            BalanceXf = balanceXf;
            BalanceJs = balanceJs;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Transfer()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtPosVirement(TransferValue, CardSerno, TransferFlag, WaitTime);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

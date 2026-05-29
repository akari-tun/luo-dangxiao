using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class QueryCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _sysType;
    [ObservableProperty] private int _cardType;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _optNum;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public QueryCardViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void Query()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.QueryCardType(
                out int sysType, out int cardType, out uint cardSerno, out int optNum,
                WaitTime);
            
            SysType = sysType;
            CardType = cardType;
            CardSerno = cardSerno;
            OptNum = optNum;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

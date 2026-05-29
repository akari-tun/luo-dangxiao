using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class JsCardViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _serno;
    [ObservableProperty] private string _cardNo = string.Empty;
    [ObservableProperty] private int _userType = 1;
    [ObservableProperty] private int _balance;
    [ObservableProperty] private int _chargeTimes;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _waitTime = 500;
    [ObservableProperty] private string _chargeDateTime = "251231120000";
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    [ObservableProperty] private int _queryCardType;
    [ObservableProperty] private int _queryOptNum;
    [ObservableProperty] private int _querySerno;
    [ObservableProperty] private string _queryCardNo = string.Empty;
    [ObservableProperty] private int _queryValue;
    [ObservableProperty] private int _queryCount;
    [ObservableProperty] private int _queryUserType;

    public JsCardViewModel(MainViewModel main)
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
            int result = _main.Consumption.QueryJsCard(
                out int cardType, out int optNum, out int serno, out string cardNo,
                out uint cardSerno, out int value, out int count, out int userType, WaitTime);
            
            QueryCardType = cardType;
            QueryOptNum = optNum;
            QuerySerno = serno;
            QueryCardNo = cardNo;
            CardSerno = cardSerno;
            QueryValue = value;
            QueryCount = count;
            QueryUserType = userType;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Init()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitJsUserCard(
                Serno, CardNo, UserType, out uint cardSerno);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void InitWithBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.InitJsUserCardN(
                Serno, CardNo, UserType, Balance, ChargeTimes, out uint cardSerno);
            CardSerno = cardSerno;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteBalance()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtJsUserCard(Balance, CardSerno);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteBalanceAddCount()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtJsUserCardAddCount(Balance, ChargeDateTime, CardSerno);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Reset()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.RstJsUserCard(CardSerno);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

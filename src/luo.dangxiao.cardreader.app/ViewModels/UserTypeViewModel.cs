using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class UserTypeViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private int _userType = 1;
    [ObservableProperty] private int _sysType = 2;
    [ObservableProperty] private uint _useTerm = 20251231;
    [ObservableProperty] private uint _cardSerno;
    [ObservableProperty] private int _oldUserType;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public UserTypeViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ChangeUserType()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ChangePosUserType12(UserType);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void ChangeUserTypeNew()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ChangePosUserType12New(out int oldUserType, UserType, SysType);
            OldUserType = oldUserType;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void UpdatePeriod()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.UpdateUserCardPeriod(UseTerm, SysType, 0);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void WriteTerm()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.WrtUserCardTerm(SysType, UseTerm, CardSerno, 500);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

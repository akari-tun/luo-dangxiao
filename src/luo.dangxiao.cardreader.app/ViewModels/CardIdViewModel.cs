using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class CardIdViewModel : ViewModelBase
{
    private readonly MainViewModel _main;

    [ObservableProperty] private ushort _tagType;
    [ObservableProperty] private uint _serialNumber;
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private bool _isBusy;

    public CardIdViewModel(MainViewModel main)
    {
        _main = main;
    }

    [RelayCommand]
    private void ReadCardId()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadCardId(out ushort tagType, out uint serialNumber);
            TagType = tagType;
            SerialNumber = serialNumber;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void ReadCardIdNew()
    {
        if (_main.Consumption == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Consumption.ReadCardIdNew(out ushort tagType, out uint serialNumber);
            TagType = tagType;
            SerialNumber = serialNumber;
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }

    [RelayCommand]
    private void Beep()
    {
        if (_main.Reader == null) return;
        
        IsBusy = true;
        try
        {
            int result = _main.Reader.Beep(10);
            ResultMessage = MainViewModel.GetErrorMessage(result);
        }
        finally
        {
            IsBusy = false;
        }
    }
}

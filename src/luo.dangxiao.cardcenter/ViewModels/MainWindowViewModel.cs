using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace luo.dangxiao.cardcenter.ViewModels
{
    public partial class MainWindowViewModel : ViewModelBase
    {
        public string Greeting { get; } = "Welcome to Avalonia!";

        [ObservableProperty]
        [NotifyPropertyChangedFor(nameof(HasStartupWarning))]
        [NotifyPropertyChangedFor(nameof(IsStartupWarningVisible))]
        private string? _startupWarningMessage;

        [ObservableProperty]
        [NotifyPropertyChangedFor(nameof(IsStartupWarningVisible))]
        private bool _isStartupWarningDismissed;

        public bool HasStartupWarning => !string.IsNullOrWhiteSpace(StartupWarningMessage);

        public bool IsStartupWarningVisible => HasStartupWarning && !IsStartupWarningDismissed;

        partial void OnStartupWarningMessageChanged(string? value)
        {
            if (!string.IsNullOrWhiteSpace(value))
            {
                IsStartupWarningDismissed = false;
            }
        }

        [RelayCommand]
        private void DismissStartupWarning()
        {
            IsStartupWarningDismissed = true;
        }
    }
}

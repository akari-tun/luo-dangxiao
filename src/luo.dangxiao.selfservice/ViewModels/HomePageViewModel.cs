using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Home page state enumeration
/// </summary>
public enum HomePageState
{
    StaffSelfService,
    StudentSelfService,
    SubPageContainer
}

/// <summary>
/// User type enumeration
/// </summary>
public enum UserType
{
    Staff,
    Student
}

/// <summary>
/// ViewModel for HomePage - Main container for self-service system
/// </summary>
public partial class HomePageViewModel : ViewModelBase
{
    [ObservableProperty]
    private HomePageState _currentState = HomePageState.StudentSelfService;

    [ObservableProperty]
    private object? _subPageContent;

    [ObservableProperty]
    private bool _isCountdownVisible;

    [ObservableProperty]
    private int _countdownSeconds = 120;

    [ObservableProperty]
    private UserType _userType = UserType.Staff;

    [ObservableProperty]
    private string _selectedFunction = string.Empty;

    partial void OnCurrentStateChanged(HomePageState value)
    {
        switch (value)
        {
            case HomePageState.StaffSelfService:
            case HomePageState.StudentSelfService:
                IsCountdownVisible = false;
                SubPageContent = null;
                break;
            case HomePageState.SubPageContainer:
                IsCountdownVisible = true;
                StartCountdown();
                break;
        }
    }

    [RelayCommand]
    private void OnFunctionButtonClick(string functionName)
    {
        SelectedFunction = functionName;
        CurrentState = HomePageState.SubPageContainer;
        LoadVerifyPage();
    }

    private void LoadVerifyPage()
    {
    }

    private void StartCountdown()
    {
    }
}

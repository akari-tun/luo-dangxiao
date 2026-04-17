using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;
using luo.dangxiao.selfservice.Views;
using System.ComponentModel;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// ViewModel for HomePage - Main container for self-service system
/// </summary>
public partial class HomePageViewModel : ViewModelBase, IPageViewModel
{
    [ObservableProperty]
    private HomePageState _currentState = HomePageState.HomePage;

    [ObservableProperty]
    private SelfServiceType _selfServiceType = SelfServiceType.StudentSelfService;

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
            case HomePageState.HomePage:
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

    public void NavigateToUserInfo(UserInfoModel userInfo, string targetFunction)
    {
        SelectedFunction = targetFunction;
        CurrentState = HomePageState.SubPageContainer;
        SubPageContent = new UserInfoPageView(new UserInfoPageParameter
        {
            TargetFunction = targetFunction,
            Data = userInfo
        });
    }

    public void NavigateToCheckIn(UserInfoModel userInfo)
    {
        SelectedFunction = "CheckIn";
        CurrentState = HomePageState.SubPageContainer;
        SubPageContent = new CheckInPageView(new CheckInPageParameter
        {
            TargetFunction = "CheckIn",
            Data = userInfo
        });
    }

    public void NavigateToTakeCard(UserInfoModel userInfo)
    {
        SelectedFunction = "TakeCard";
        CurrentState = HomePageState.SubPageContainer;
        SubPageContent = new TakeCardPageView(new TakeCardPageParameter
        {
            TargetFunction = "TakeCard",
            Data = userInfo
        });
    }

    public void NavigateToReportLoss(UserInfoModel userInfo)
    {
        SelectedFunction = "ReportLoss";
        CurrentState = HomePageState.SubPageContainer;
        SubPageContent = new ReportLossPageView(new ReportLossPageParameter
        {
            TargetFunction = "ReportLoss",
            Data = userInfo
        });
    }

    public void ReturnHome()
    {
        SelectedFunction = string.Empty;
        CurrentState = HomePageState.HomePage;
    }

    public void LoadVerifyPage()
    {
        SubPageContent = new VerifyPageView(SelectedFunction);
    }

    private void StartCountdown()
    {
    }
}

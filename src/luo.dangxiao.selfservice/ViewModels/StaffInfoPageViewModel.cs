using Avalonia.Media;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.common.Enums;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.models;

namespace luo.dangxiao.selfservice.ViewModels;

/// <summary>
/// Display mode for staff information module.
/// </summary>
public enum StaffInfoDisplayMode
{
    Standard,
    WithPhoto,
    RechargePreview,
    FullInfo
}

/// <summary>
/// Staff info parameter for the module.
/// </summary>
public sealed class StaffInfoPageParameter
{
    public string StaffId { get; set; } = string.Empty;

    public StaffInfoModel? Data { get; set; }

    public StaffInfoDisplayMode Mode { get; set; } = StaffInfoDisplayMode.Standard;

    public decimal? RechargeAmount { get; set; }
}

/// <summary>
/// ViewModel for StaffInfoPage module.
/// </summary>
public partial class StaffInfoPageViewModel : ViewModelBase, IPageViewModel
{
    [ObservableProperty]
    private StaffInfoModel _staffInfo = new()
    {
        Id = "STF2020001",
        Name = "张明华",
        UserType = UserType.Staff,
        Gender = "男",
        IdCardNumber = "430101198502031234",
        Department = "教务处",
        EmployeeNumber = "T2020001",
        CardType = "教职工卡",
        CardNumber = "2020001001",
        UserCards =
        [
            new CardInfoModel
            {
                CardNo = "2020001001",
                CardStatusId = (int)UserCardStatus.Normal,
                CardStatusName = "正常"
            }
        ],
        ConsumptionBalance = 125.50m,
        SubsidyBalance = 80.00m,
        CardBalance = 125.50m,
        CardIssueDate = new DateTime(2020, 1, 1),
        CardExpiryDate = new DateTime(2025, 12, 31),
        PhoneNumber = "13800138000"
    };

    [ObservableProperty]
    private StaffInfoDisplayMode _currentMode = StaffInfoDisplayMode.Standard;

    [ObservableProperty]
    private decimal? _rechargeAmount = 100m;

    [ObservableProperty]
    private decimal _balanceAfterRecharge = 225.50m;

    public bool ShowPhoto => CurrentMode == StaffInfoDisplayMode.WithPhoto;

    public bool ShowRechargePreview => CurrentMode == StaffInfoDisplayMode.RechargePreview;

    public bool ShowCardDetailInfo => CurrentMode == StaffInfoDisplayMode.FullInfo;

    public bool ShowGender => CurrentMode != StaffInfoDisplayMode.RechargePreview;

    public bool ShowIdCardNumber => CurrentMode != StaffInfoDisplayMode.WithPhoto && CurrentMode != StaffInfoDisplayMode.RechargePreview;

    public bool ShowPhoneNumber => CurrentMode != StaffInfoDisplayMode.WithPhoto;

    public bool ShowCardBalance => !ShowRechargePreview;

    public bool ShowCardInfo => !string.IsNullOrWhiteSpace(StaffInfo.CardNumber);

    public string CardStatusText
    {
        get
        {
            var currentCard = StaffInfo.CurrentCard;
            if (currentCard is null)
            {
                return "待领取";
            }

            return currentCard.CardStatusId switch
            {
                (int)UserCardStatus.Normal => "正常",
                (int)UserCardStatus.Lost => "已挂失",
                (int)UserCardStatus.Unissued => "未制卡",
                _ => "未知"
            };
        }
    }

    public IBrush CardStatusBrush
    {
        get
        {
            var currentCard = StaffInfo.CurrentCard;
            if (currentCard is null)
            {
                return Brush.Parse("#ff6000");
            }

            return currentCard.CardStatusId switch
            {
                (int)UserCardStatus.Normal => Brush.Parse("#4CAF50"),
                (int)UserCardStatus.Lost => Brush.Parse("#d9230b"),
                (int)UserCardStatus.Unissued => Brush.Parse("#70706d"),
                _ => Brush.Parse("#70706d")
            };
        }
    }

    public string CardIssueDateText => StaffInfo.CardIssueDate?.ToString("yyyy-MM-dd") ?? "-";

    public string CardExpiryDateText => StaffInfo.CardExpiryDate?.ToString("yyyy-MM-dd") ?? "-";

    partial void OnCurrentModeChanged(StaffInfoDisplayMode value)
    {
        OnPropertyChanged(nameof(ShowPhoto));
        OnPropertyChanged(nameof(ShowRechargePreview));
        OnPropertyChanged(nameof(ShowCardDetailInfo));
        OnPropertyChanged(nameof(ShowGender));
        OnPropertyChanged(nameof(ShowIdCardNumber));
        OnPropertyChanged(nameof(ShowPhoneNumber));
        OnPropertyChanged(nameof(ShowCardBalance));
    }

    partial void OnRechargeAmountChanged(decimal? value)
    {
        UpdateBalanceAfterRecharge();
    }

    partial void OnStaffInfoChanged(StaffInfoModel value)
    {
        OnPropertyChanged(nameof(ShowCardInfo));
        OnPropertyChanged(nameof(CardStatusText));
        OnPropertyChanged(nameof(CardStatusBrush));
        OnPropertyChanged(nameof(CardIssueDateText));
        OnPropertyChanged(nameof(CardExpiryDateText));
        UpdateBalanceAfterRecharge();
    }

    [RelayCommand]
    private void LoadData(StaffInfoPageParameter parameter)
    {
        CurrentMode = parameter.Mode;
        RechargeAmount = parameter.RechargeAmount;

        if (parameter.Data is not null)
        {
            StaffInfo = parameter.Data;
        }

        UpdateBalanceAfterRecharge();
    }

    [RelayCommand]
    private void Refresh()
    {
        OnPropertyChanged(nameof(ShowPhoto));
        OnPropertyChanged(nameof(ShowRechargePreview));
        OnPropertyChanged(nameof(ShowCardDetailInfo));
        OnPropertyChanged(nameof(ShowGender));
        OnPropertyChanged(nameof(ShowIdCardNumber));
        OnPropertyChanged(nameof(ShowPhoneNumber));
        OnPropertyChanged(nameof(ShowCardBalance));
        OnPropertyChanged(nameof(ShowCardInfo));
        OnPropertyChanged(nameof(CardStatusText));
        OnPropertyChanged(nameof(CardStatusBrush));
        OnPropertyChanged(nameof(CardIssueDateText));
        OnPropertyChanged(nameof(CardExpiryDateText));
        UpdateBalanceAfterRecharge();
    }

    private void UpdateBalanceAfterRecharge()
    {
        BalanceAfterRecharge = StaffInfo.ConsumptionBalance + (RechargeAmount ?? 0m);
    }
}

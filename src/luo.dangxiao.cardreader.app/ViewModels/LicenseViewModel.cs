using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.cardreader.Yc;
using luo.dangxiao.cardreader.Yc.Protocol;
using luo.dangxiao.cardreader.Yc.Structs;

namespace luo.dangxiao.cardreader.app.ViewModels;

public partial class LicenseViewModel : ViewModelBase
{
    private readonly LowLevelViewModel _lowLevel;

    [ObservableProperty] private bool _hasPaymentAuth;
    [ObservableProperty] private bool _hasWaterBillingAuth;
    [ObservableProperty] private bool _hasAccessControlAuth;
    [ObservableProperty] private int _paymentSector = 1;
    [ObservableProperty] private int _waterBillingSector = 3;
    [ObservableProperty] private int _accessControlSector = 2;
    [ObservableProperty] private string _operatorPassword = "12345678";
    [ObservableProperty] private string _resultMessage = string.Empty;
    [ObservableProperty] private string _licenceFilePath = string.Empty;
    [ObservableProperty] private string _userKeyAHex = string.Empty;
    [ObservableProperty] private string _userKeyBHex = string.Empty;
    [ObservableProperty] private string _systemKeyBHex = string.Empty;

    public LicenseViewModel(LowLevelViewModel lowLevel)
    {
        _lowLevel = lowLevel;
        RefreshStatus();
    }

    public void RefreshStatus()
    {
        var license = LicenseManager.Instance;
        HasPaymentAuth = license.HasPaymentAuth;
        HasWaterBillingAuth = license.HasWaterBillingAuth;
        HasAccessControlAuth = license.HasAccessControlAuth;

        var sysInfo = license.SystemInfo;
        if (sysInfo.OperatorPassword != null && sysInfo.OperatorPassword.Length >= 6)
        {
            UserKeyAHex = BitConverter.ToString(sysInfo.OperatorPassword, 0, 6).Replace("-", "");
        }
        if (sysInfo.SystemKeyB != null && sysInfo.SystemKeyB.Length >= 6)
        {
            UserKeyBHex = BitConverter.ToString(sysInfo.SystemKeyB, 0, 6).Replace("-", "");
            SystemKeyBHex = UserKeyBHex;
        }

        if (_lowLevel != null) _lowLevel.RefreshKeysFromLicense();
    }

    [RelayCommand]
    private void Configure()
    {
        try
        {
            var sysInfo = new SystemInfoNew
            {
                PaymentSystem = HasPaymentAuth ? 1 : 0,
                PaymentSector = PaymentSector,
                WaterBillingSystem = HasWaterBillingAuth ? 1 : 0,
                WaterBillingSector = WaterBillingSector,
                AccessControlSystem = HasAccessControlAuth ? 1 : 0,
                AccessControlSector = AccessControlSector
            };

            ResultMessage = "License configured successfully";
            RefreshStatus();
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error: {ex.Message}";
        }
    }

    [RelayCommand]
    private void ClearAuth()
    {
        LicenseManager.Instance.ClearAuth();
        RefreshStatus();
        ResultMessage = "Authorization cleared";
    }

    [RelayCommand]
    private async Task BrowseLicenceFile()
    {
        try
        {
            var mainWindow = TopLevel.GetTopLevel(
                Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop
                    ? desktop.MainWindow
                    : null);

            if (mainWindow?.StorageProvider == null)
            {
                ResultMessage = "Storage provider not available";
                return;
            }

            var options = new FilePickerOpenOptions
            {
                Title = "Select Licence File",
                AllowMultiple = false,
                FileTypeFilter = new List<FilePickerFileType>
                {
                    new FilePickerFileType("Licence Files")
                    {
                        Patterns = new[] { "*.dat" },
                        MimeTypes = new[] { "application/octet-stream" }
                    },
                    new FilePickerFileType("All Files")
                    {
                        Patterns = new[] { "*" },
                        MimeTypes = new[] { "*/*" }
                    }
                }
            };

            var files = await mainWindow.StorageProvider.OpenFilePickerAsync(options);

            if (files.Count > 0)
            {
                LicenceFilePath = files[0].TryGetLocalPath() ?? files[0].Path.LocalPath;
            }
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error browsing file: {ex.Message}";
        }
    }

    [RelayCommand]
    private void LoadLicence()
    {
        try
        {
            if (string.IsNullOrEmpty(LicenceFilePath))
            {
                ResultMessage = "Please select a licence file";
                return;
            }

            if (!File.Exists(LicenceFilePath))
            {
                ResultMessage = "Licence file not found";
                return;
            }

            int result = LicenseManager.Instance.LoadLicence(LicenceFilePath);
            
            if (result == 0)
            {
                ResultMessage = "Licence loaded successfully";
                RefreshStatus();
            }
            else
            {
                ResultMessage = $"Failed to load licence (Error code: {result})";
            }
        }
        catch (Exception ex)
        {
            ResultMessage = $"Error loading licence: {ex.Message}";
        }
    }
}

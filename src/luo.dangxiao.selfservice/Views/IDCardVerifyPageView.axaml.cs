using Avalonia;
using Avalonia.Controls;
using Avalonia.Layout;
using Avalonia.Media;
using CommunityToolkit.Mvvm.DependencyInjection;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.interfaces.Views;
using luo.dangxiao.selfservice.ViewModels;
using System.Globalization;
using System.Text.RegularExpressions;

namespace luo.dangxiao.selfservice.Views;

/// <summary>
/// View for IDCardVerifyPage module.
/// </summary>
public partial class IDCardVerifyPageView : UserControl, IPageView
{
    private static readonly Regex IdCardRegex = new(@"^\d{17}[\dXx]$", RegexOptions.Compiled);

    IDCardVerifyPageViewModel _viewModel;
    public IPageViewModel ViewModel => _viewModel;

    public IDCardVerifyPageView()
    {
        InitializeComponent();

        _viewModel = Ioc.Default.GetRequiredService<IDCardVerifyPageViewModel>();
#if DEBUG
        _viewModel.RequestTestIdCardNumberAsync = ShowTestIdCardInputDialogAsync;
#endif
        DataContext = _viewModel;
    }

#if DEBUG
    private async Task<string?> ShowTestIdCardInputDialogAsync()
    {
        var owner = TopLevel.GetTopLevel(this) as Window;
        if (owner is null)
        {
            return null;
        }

        var inputBox = new TextBox
        {
            Watermark = "请输入18位身份证号",
            MaxLength = 18,
        };

        var validationMessage = new TextBlock
        {
            Foreground = Brush.Parse("#d9230b"),
            FontSize = 12,
            IsVisible = false
        };

        var confirmButton = new Button
        {
            Content = "确定",
            MinWidth = 80,
            IsEnabled = false
        };

        var cancelButton = new Button
        {
            Content = "取消",
            MinWidth = 80
        };

        var dialog = new Window
        {
            Title = "模拟读卡",
            Width = 360,
            Height = 172,
            CanResize = false,
            WindowStartupLocation = WindowStartupLocation.CenterOwner,
            Content = new StackPanel
            {
                Spacing = 10,
                Margin = new Thickness(8),
                Children =
                {
                    new TextBlock { Text = "请输入用于模拟读卡的身份证号：" },
                    inputBox,
                    new StackPanel
                    {
                        Orientation = Orientation.Horizontal,
                        Spacing = 10,
                        HorizontalAlignment = HorizontalAlignment.Right,
                        Children =
                        {
                            cancelButton,
                            confirmButton
                        }
                    },
                    validationMessage
                }
            }
        };

        inputBox.TextChanged += (_, _) =>
        {
            var value = inputBox.Text?.Trim() ?? string.Empty;
            var isValid = IsValidIdCardNumber(value);
            confirmButton.IsEnabled = isValid;
            validationMessage.IsVisible = !string.IsNullOrEmpty(value) && !isValid;
            validationMessage.Text = isValid ? string.Empty : "身份证号码格式不正确";
        };

        cancelButton.Click += (_, _) => dialog.Close(null);
        confirmButton.Click += (_, _) => dialog.Close(inputBox.Text?.Trim().ToUpperInvariant());

        return await dialog.ShowDialog<string?>(owner);
    }

    private static bool IsValidIdCardNumber(string idCardNumber)
    {
        if (!IdCardRegex.IsMatch(idCardNumber))
        {
            return false;
        }

        var birthday = idCardNumber[6..14];
        if (!DateTime.TryParseExact(
                birthday,
                "yyyyMMdd",
                CultureInfo.InvariantCulture,
                DateTimeStyles.None,
                out _))
        {
            return false;
        }

        return true;
    }
#endif
}

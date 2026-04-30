# AGENTS.md - luo.dangxiao Project Specification

> **OpenCode Exclusive Configuration**
> This document defines the project structure, patterns, and conventions for all code generation work.

---

## 1. Project Overview

**Solution Name**: luo.dangxiao  
**Technology Stack**: Avalonia UI + CommunityToolkit.Mvvm + .NET 10.0  
**Architecture Pattern**: MVVM (Model-View-ViewModel)

### 1.1 Technology Versions (MUST USE)
```xml
<TargetFramework>net10.0</TargetFramework>
<Avalonia Version="11.3.12" />
<CommunityToolkit.Mvvm Version="8.4.1" />
<Avalonia.Themes.Fluent Version="11.3.12" />
<Avalonia.Fonts.Inter Version="11.3.12" />
```

---

## 2. Project Structure

### 2.1 Project Hierarchy

```
luo.dangxiao/
├── luo.dangxiao.common/              # Common utilities, enums, converters
├── luo.dangxiao.interfaces/          # Interface definitions (Controls, Models, Views, ViewModels)
├── luo.dangxiao.models/              # Data models (DTOs, Entities), Config classes
├── luo.dangxiao.resources/           # Resource files (Images, Languages, Styles)
├── luo.dangxiao.controls/            # Custom Avalonia controls
├── luo.dangxiao.printer/             # Card printer abstraction (CardPrinterBase + Virtual/Seaory)
├── luo.dangxiao.cardreader/          # Card reader abstraction (CardReaderBase + Virtual/YC)
├── luo.dangxiao.selfservice/         # Self-service library (Views + ViewModels)
├── luo.dangxiao.selfservice.app/     # Self-service executable entry point
├── luo.dangxiao.cardcenter/          # Card center library (Views + ViewModels)
├── luo.dangxiao.cardcenter.app/      # Card center executable entry point
└── luo.dangxiao.wabapi/              # YKT API client wrapper + DTOs
```

### 2.2 Project Roles & Responsibilities

| Project | Role | Output Type | References |
|---------|------|-------------|------------|
| `luo.dangxiao.common` | Shared utilities library | Library | None |
| `luo.dangxiao.interfaces` | Contract definitions | Library | None |
| `luo.dangxiao.models` | Data models (DTOs, Entities), Config classes | Library | None |
| `luo.dangxiao.resources` | Resource files (Images, Languages, Styles) | Library | Avalonia |
| `luo.dangxiao.controls` | Custom Avalonia controls | Library | Avalonia, luo.dangxiao.interfaces |
| `luo.dangxiao.printer` | Card printer abstraction (base + Virtual/Seaory) | Library | None |
| `luo.dangxiao.cardreader` | Card reader abstraction (base + Virtual/YC) | Library | None |
| `luo.dangxiao.wabapi` | YKT API client wrapper | Library | luo.dangxiao.models |
| `luo.dangxiao.selfservice` | Self-service library (Views + ViewModels) | Library | Avalonia, CommunityToolkit.Mvvm, luo.dangxiao.common, luo.dangxiao.interfaces, luo.dangxiao.models, luo.dangxiao.resources, luo.dangxiao.controls, luo.dangxiao.printer, luo.dangxiao.cardreader, luo.dangxiao.wabapi |
| `luo.dangxiao.selfservice.app` | Self-service application | WinExe | luo.dangxiao.selfservice, Avalonia.Desktop |
| `luo.dangxiao.cardcenter` | Card center library (Views + ViewModels) | Library | Avalonia, CommunityToolkit.Mvvm, luo.dangxiao.common, luo.dangxiao.interfaces, luo.dangxiao.models, luo.dangxiao.resources, luo.dangxiao.controls, luo.dangxiao.printer, luo.dangxiao.cardreader |
| `luo.dangxiao.cardcenter.app` | Card center application | WinExe | luo.dangxiao.cardcenter, Avalonia.Desktop |

---

## 3. Directory Conventions

### 3.1 Source Code Paths

```
Workspace Root: /mnt/d/github/luo-dangxiao/src/

Source Code:
- /mnt/d/github/luo-dangxiao/src/{project-name}/

Configuration:
- /mnt/d/github/luo-dangxiao/src/AGENTS.md (this file)
```

### 3.2 Per-Project Folder Structure

#### Library Projects (common, interfaces, controls, selfservice, cardcenter)
```
{project}/
├── *.csproj                    # Project file
├── {Folder}/                   # Organized by feature/type
│   └── *.cs
└── (Optional) *.axaml          # For controls project only
```

#### Application Projects (.app)
```
{project}.app/
├── *.csproj                    # Project file
├── app.manifest               # Windows manifest
├── Program.cs                 # Entry point
└── (No other folders - keep minimal)
```

### 3.3 Standard Folder Names

| Folder | Purpose | Allowed Projects |
|--------|---------|------------------|
| `Utils/` | Utility/helper classes | `luo.dangxiao.common` only |
| `Enum/` | Enumeration definitions | `luo.dangxiao.common` only |
| `Converter/` | IValueConverter implementations | `luo.dangxiao.common` only |
| `Models/` | Data models/DTOs | `luo.dangxiao.models`, `luo.dangxiao.interfaces` |
| `Controls/` | Custom control definitions | `luo.dangxiao.controls`, `luo.dangxiao.interfaces` |
| `Images/` | Image resources (png, jpg, svg, ico) | `luo.dangxiao.resources` only |
| `Languages/` | Localization resource files (.resx) | `luo.dangxiao.resources` only |
| `Styles/` | Avalonia style files (.axaml) | `luo.dangxiao.resources` only |
| `Views/` | Avalonia views (.axaml + .axaml.cs) | `luo.dangxiao.selfservice`, `luo.dangxiao.cardcenter` |
| `ViewModels/` | View model classes | `luo.dangxiao.selfservice`, `luo.dangxiao.cardcenter` |

---

## 4. Naming Conventions

### 4.1 Project Naming

```
luo.dangxiao.{modulename}          # Library projects
luo.dangxiao.{modulename}.app      # Application entry points
```

### 4.2 Namespace Naming

```csharp
// MUST match project name exactly
namespace luo.dangxiao.common.Utils;
namespace luo.dangxiao.selfservice.ViewModels;
namespace luo.dangxiao.selfservice.Views;
namespace luo.dangxiao.interfaces.Controls;
```

### 4.3 File Naming

| Type | Pattern | Example |
|------|---------|---------|
| Class | PascalCase | `MainWindowViewModel.cs` |
| View (AXAML) | {Name}View.axaml | `HomePageView.axaml` |
| View Code-behind | {Name}View.axaml.cs | `HomePageView.axaml.cs` |
| Utility | PascalCase | `DoubleUtil.cs` |
| Interface | IPascalCase | `IMainWindowViewModel.cs` |
| Enum | PascalCase | `ApplicationState.cs` |
| Converter | PascalCaseConverter | `BoolToVisibilityConverter.cs` |
| Colors | Colors.axaml | `Colors.axaml` (in `Styles/` folder) |

### 4.4 Class Naming

| Type | Pattern | Example |
|------|---------|---------|
| ViewModel | {Name}ViewModel | `MainWindowViewModel` |
| View | {Name}View | `HomePageView` (Window/UserControl) |
| Base Class | {Name}Base | `ViewModelBase` |
| Utility | {Name}Util | `DoubleUtil`, `EnumUtility` |
| Interface | I{Name} | `IMainWindowViewModel` |
| Converter | {Name}Converter | `BoolToVisibilityConverter` |
| Enum | PascalCase | `ApplicationState` |

---

## 5. Code Patterns & Templates

### 5.1 ViewModel Pattern (MUST USE CommunityToolkit.Mvvm)

```csharp
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace luo.dangxiao.{module}.ViewModels;

/// <summary>
/// ViewModel for {ViewName}
/// </summary>
public partial class {ViewName}ViewModel : ViewModelBase
{
    [ObservableProperty]
    private string _fieldName;

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(ComputedProperty))]
    private int _otherField;

    public string ComputedProperty => $"{_otherField} items";

    [RelayCommand]
    private void DoSomething()
    {
        // Command implementation
    }
}
```

**RULES:**
- MUST inherit from `ViewModelBase` (not ObservableObject directly)
- MUST use `[ObservableProperty]` for auto-generated properties
- MUST use `[RelayCommand]` for commands
- MUST mark class as `partial` for source generators
- MUST use `_camelCase` for backing fields

### 5.2 ViewModelBase (Standard)

```csharp
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.DependencyInjection;
using CommunityToolkit.Mvvm.Input;
using luo.dangxiao.interfaces.ViewModels;
using luo.dangxiao.resources.Languages;
using luo.dangxiao.wabapi.Clients;
using luo.dangxiao.wabapi.Dtos.Responses;
using System.Text.Json;

namespace luo.dangxiao.{module}.ViewModels
{
    /// <summary>
    /// Base class for all ViewModels providing shared navigation and YktApi calling helpers.
    /// </summary>
    public abstract partial class ViewModelBase : ObservableObject, IPageViewModel
    {
        #region Navigation

        [RelayCommand]
        protected virtual void Back()
        {
            Ioc.Default.GetRequiredService<HomePageViewModel>().ReturnHome();
        }

        #endregion

        #region YktApi Helpers

        /// <summary>
        /// Gets the configured IYktApiClient from the DI container.
        /// Returns null if the client is not registered.
        /// </summary>
        protected static IYktApiClient? GetYktApiClient()
        {
            return Ioc.Default.GetService<IYktApiClient>();
        }

        /// <summary>
        /// Determines whether an API response code indicates success.
        /// </summary>
        protected static bool IsApiSuccess(int? code)
        {
            return code is null or 0 or 200;
        }

        /// <summary>
        /// Determines whether an API response indicates success using both Success and Code fields.
        /// A response is successful when Success is true, OR Code is null/0/200.
        /// </summary>
        protected static bool IsApiSuccess<TData>(ApiResponseDto<TData> response)
        {
            return response.Success == true || IsApiSuccess(response.Code);
        }

        /// <summary>
        /// Validates an API response and throws InvalidOperationException on failure.
        /// </summary>
        protected static void EnsureApiSuccess(int? code, string? message, string? fallbackResourceKey = null)
        {
            if (IsApiSuccess(code))
            {
                return;
            }

            var errorMsg = string.IsNullOrWhiteSpace(message)
                ? LanguageProvider.GetLocalizedText(fallbackResourceKey ?? "Msg_Error")
                : message;
            throw new InvalidOperationException(errorMsg);
        }

        /// <summary>
        /// Validates an API response and throws InvalidOperationException on failure.
        /// </summary>
        protected static void EnsureApiSuccess<TData>(ApiResponseDto<TData> response, string? fallbackResourceKey = null)
        {
            if (IsApiSuccess(response))
            {
                return;
            }

            var errorMsg = string.IsNullOrWhiteSpace(response.Message)
                ? LanguageProvider.GetLocalizedText(fallbackResourceKey ?? "Msg_Error")
                : response.Message;
            throw new InvalidOperationException(errorMsg);
        }

        /// <summary>
        /// Extracts a string property value from a JsonElement by trying field names in order.
        /// </summary>
        protected static string? ExtractJsonString(JsonElement? data, params string[] fieldNames)
        {
            if (!data.HasValue || data.Value.ValueKind != JsonValueKind.Object)
            {
                return null;
            }

            foreach (var name in fieldNames)
            {
                if (data.Value.TryGetProperty(name, out var prop) && prop.ValueKind == JsonValueKind.String)
                {
                    return prop.GetString();
                }
            }

            return null;
        }

        /// <summary>
        /// Formats an API error message for user-friendly UI display.
        /// Returns the localized fallback message if apiMessage is empty.
        /// </summary>
        protected static string FormatApiError(string? apiMessage, string localizedFallback)
        {
            return string.IsNullOrWhiteSpace(apiMessage)
                ? localizedFallback
                : apiMessage;
        }

        #endregion
    }
}
```

**RULES:**
- MUST be declared `abstract partial` (partial for CommunityToolkit source generators)
- MUST inherit from `ObservableObject`
- MUST provide `Back()` command for navigation
- MUST provide YktApi helper methods (Section 5.3)

### 5.3 YktApi Calling Convention

All YktApi calls in ViewModels MUST use the base class helper methods from `ViewModelBase`.

**Standard Pattern:**
```csharp
[RelayCommand]
private async Task MyOperationAsync()
{
    // 1. Get client via base method
    var yktApiClient = GetYktApiClient();
    if (yktApiClient is null)
    {
        OperationStatusText = LanguageProvider.SelfService_Module_Status_Failed_ApiUnavailable;
        return;
    }

    // 2. Build request
    var request = new MyRequestDto { /* ... */ };

    try
    {
        // 3. Call API
        var response = await yktApiClient.MyMethodAsync(request);

        // 4. Check success using base method (checks both Success and Code fields)
        if (!IsApiSuccess(response))
        {
            // Display response.Message directly — it already contains the user-friendly error
            OperationStatusText = FormatApiError(response.Message,
                LanguageProvider.SelfService_Module_Status_Failed);
            return;
        }

        // 5. Use response.Data...
    }
    catch (OperationCanceledException) when (_pageCleanupInProgress)
    {
        return;
    }
    catch (Exception ex)
    {
        OperationStatusText = string.Format(CultureInfo.CurrentUICulture,
            LanguageProvider.SelfService_Module_Status_Failed_WithReason,
            FormatApiError(ex.Message, LanguageProvider.SelfService_Module_Status_Failed));
    }
}
```

**EnsureApiSuccess Pattern** (for validation scenarios that throw):
```csharp
var response = await yktApiClient.GetTeacherByIdentityAsync(encodedIdentity);
EnsureApiSuccess(response);
// If success: continue. If failure: InvalidOperationException with response.Message is thrown.
// The catch block should format ex.Message for UI display.
```

**ExtractJsonString Pattern** (for parsing JsonElement? responses):
```csharp
// Simple field name fallback
var cardNo = ExtractJsonString(response.Data, "cardNo", "CardNo");

// Multiple fields
result.CardId = ExtractJsonString(response.Data, "cardId", "CardId") ?? string.Empty;
result.UserId = ExtractJsonString(response.Data, "userId", "UserId") ?? string.Empty;
```

**FailResult.json Structure:**
When the API returns an error (code != 200), the JSON structure is:
```json
{"success":false,"code":50001,"message":"物理卡号[1,348,446,620]和卡流水号[40,033]对应旧卡是正常卡片","data":null,"currentTime":1777455643169}
```

| Field | Type | Description |
|-------|------|-------------|
| `success` | `bool?` | `false` on failure, `true` on success |
| `code` | `int?` | Business status code. `null`, `0`, or `200` = success; otherwise = error |
| `message` | `string` | **User-facing error message** — display this directly in UI |
| `data` | varies | `null` on failure; payload on success |
| `currentTime` | `long?` | Server timestamp (milliseconds) |

The `response.Code` is auto-deserialized to `Code`, `response.Message` to `Message`, etc. via camelCase JSON policy.

**Error Message Display Rule:** The `message` field in the API response contains Chinese error text meant for end users. Always display it in the UI via `OperationStatusText`/`StatusMessage`/`ErrorMessage` bindings. Use `FormatApiError(response.Message, fallback)` to provide a localized fallback when the message is empty.

**FORBIDDEN:**
- MUST NOT use `Ioc.Default.GetService<IYktApiClient>()` directly — use `GetYktApiClient()`
- MUST NOT use `response.Code is not (null or 0 or 200)` — use `!IsApiSuccess(response)` or `!IsApiSuccess(response.Code)`
- MUST NOT inline duplicate `EnsureApiSuccess` implementations — use the base class method
- MUST NOT discard `response.Message` — it contains the user-facing error text from the API
- MUST NOT parse JSON with raw `TryGetProperty` loops — use `ExtractJsonString()`

### 5.4 View Pattern (AXAML)

```xml
<Window xmlns="https://github.com/avaloniaui"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:vm="using:luo.dangxiao.{module}.ViewModels"
        xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
        xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
        mc:Ignorable="d" d:DesignWidth="800" d:DesignHeight="450"
        x:Class="luo.dangxiao.{module}.Views.{ViewName}"
        x:DataType="vm:{ViewName}ViewModel"
        Title="{ViewName}">

    <Design.DataContext>
        <vm:{ViewName}ViewModel/>
    </Design.DataContext>

    <!-- View content -->

</Window>
```

**RULES:**
- MUST declare `x:DataType` for compile-time binding validation
- MUST include `Design.DataContext` for IDE preview
- MUST use `vm:` prefix for ViewModel namespaces
- MUST set `mc:Ignorable="d"` for design-time attributes

### 5.5 View Code-Behind

```csharp
using Avalonia.Controls;

namespace luo.dangxiao.{module}.Views;

/// <summary>
/// View for {ViewName}
/// </summary>
public partial class {ViewName} : Window  // or UserControl
{
    public {ViewName}()
    {
        InitializeComponent();
    }
}
```

### 5.6 Application Entry Point (.app projects)

```csharp
using Avalonia;
using luo.dangxiao.{module};

namespace luo.dangxiao.{module}.app;

/// <summary>
/// Entry point for {Module} application
/// </summary>
internal sealed class Program
{
    [STAThread]
    public static void Main(string[] args) => BuildAvaloniaApp()
        .StartWithClassicDesktopLifetime(args);

    public static AppBuilder BuildAvaloniaApp()
        => AppBuilder.Configure<App>()
            .UsePlatformDetect()
            .WithInterFont()
            .LogToTrace();
}
```

### 5.7 Application AXAML

```xml
<Application xmlns="https://github.com/avaloniaui"
             xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
             x:Class="luo.dangxiao.{module}.App"
             RequestedThemeVariant="Default">
  
    <Application.Styles>
        <FluentTheme />
    </Application.Styles>
</Application>
```

### 5.8 Utility Class Pattern (luo.dangxiao.common)

```csharp
namespace luo.dangxiao.common.Utils;

/// <summary>
/// Utility methods for {purpose}
/// </summary>
public static class {Name}Util
{
    // Static methods only
}
```

### 5.9 Enum Pattern (luo.dangxiao.common)

```csharp
using System.ComponentModel;

namespace luo.dangxiao.common.Enum;

/// <summary>
/// {Description}
/// </summary>
public enum {Name}
{
    [Description("Description text")]
    Value1,
    
    [Description("Another description")]
    Value2
}
```

### 5.10 Converter Pattern (luo.dangxiao.common)

```csharp
using Avalonia.Data.Converters;
using System;
using System.Globalization;

namespace luo.dangxiao.common.Converter;

/// <summary>
/// Converts {source type} to {target type}
/// </summary>
public class {Name}Converter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        // Implementation
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        // Implementation
    }
}
```

### 5.11 Model Pattern (luo.dangxiao.models)

```csharp
namespace luo.dangxiao.models;

/// <summary>
/// Data model for {EntityName}
/// </summary>
public class {EntityName}
{
    /// <summary>
    /// Unique identifier
    /// </summary>
    public Guid Id { get; set; }
    
    /// <summary>
    /// Creation timestamp
    /// </summary>
    public DateTime CreatedAt { get; set; }
    
    /// <summary>
    /// Last modification timestamp
    /// </summary>
    public DateTime? ModifiedAt { get; set; }
    
    // Add entity-specific properties here
}
```

**RULES:**
- Models are simple POCOs (Plain Old CLR Objects)
- Use nullable reference types where appropriate
- Include standard audit fields (Id, CreatedAt, ModifiedAt)
- Use init-only setters for immutable properties

### 5.12 Localization Pattern (luo.dangxiao.resources)

**Resource File Structure:**
```
Languages/
├── Language.resx              # Default (en-US)
├── Language.zh-Hans.resx      # Simplified Chinese
├── Language.zh-Hant.resx      # Traditional Chinese
└── Language.{culture}.resx    # Additional cultures
```

**Usage in XAML:**
```xml
<Window xmlns:lang="using:luo.dangxiao.resources.Languages">
    <Button Content="{x:Static lang:Language.Button_OK}"/>
</Window>
```

**Usage in Code-Behind:**
```csharp
using luo.dangxiao.resources.Languages;

var message = Language.Msg_Success;
```

**Naming Convention for Resource Keys:**
- `App_` - Application-level strings
- `Button_` - Button labels
- `Label_` - Field labels
- `Msg_` - Messages and notifications
- `Nav_` - Navigation items
- `Error_` - Error messages
- `Title_` - Window/dialog titles

---

## 6. Project Reference Rules

### 6.1 Allowed References

| Project | Can Reference |
|---------|---------------|
| `luo.dangxiao.common` | None (base layer) |
| `luo.dangxiao.interfaces` | None (contract layer) |
| `luo.dangxiao.models` | None (data layer) |
| `luo.dangxiao.resources` | None (resource layer) |
| `luo.dangxiao.printer` | None (device layer) |
| `luo.dangxiao.cardreader` | None (device layer) |
| `luo.dangxiao.controls` | `luo.dangxiao.interfaces` |
| `luo.dangxiao.wabapi` | `luo.dangxiao.models` |
| `luo.dangxiao.selfservice` | `luo.dangxiao.common`, `luo.dangxiao.interfaces`, `luo.dangxiao.models`, `luo.dangxiao.resources`, `luo.dangxiao.controls`, `luo.dangxiao.printer`, `luo.dangxiao.cardreader`, `luo.dangxiao.wabapi` |
| `luo.dangxiao.cardcenter` | `luo.dangxiao.common`, `luo.dangxiao.interfaces`, `luo.dangxiao.models`, `luo.dangxiao.resources`, `luo.dangxiao.controls`, `luo.dangxiao.printer`, `luo.dangxiao.cardreader` |
| `luo.dangxiao.selfservice.app` | `luo.dangxiao.selfservice` only |
| `luo.dangxiao.cardcenter.app` | `luo.dangxiao.cardcenter` only |

### 6.2 Reference Direction
```
.app -> Module -> controls -> interfaces
              -> models (DTOs/Entities)
              -> resources (Images/Languages/Styles)
              -> common (shared utilities)
              -> printer (card printer abstraction)
              -> cardreader (card reader abstraction)
```

---

## 7. Csproj Template

### 7.1 Library Project Template

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>

  <ItemGroup>
    <PackageReference Include="Avalonia" Version="11.3.12" />
    <PackageReference Include="Avalonia.Themes.Fluent" Version="11.3.12" />
    <PackageReference Include="CommunityToolkit.Mvvm" Version="8.4.1" />
  </ItemGroup>

  <!-- For views with code-behind -->
  <ItemGroup>
    <Compile Update="Views\*.axaml.cs">
      <SubType>Code</SubType>
      <DependentUpon>%(Filename)</DependentUpon>
    </Compile>
  </ItemGroup>

  <ItemGroup>
    <None Update="Views\*.axaml">
      <SubType>Designer</SubType>
    </None>
  </ItemGroup>

  <!-- Project references -->
  <ItemGroup>
    <ProjectReference Include="..\luo.dangxiao.common\luo.dangxiao.common.csproj" />
    <ProjectReference Include="..\luo.dangxiao.interfaces\luo.dangxiao.interfaces.csproj" />
    <ProjectReference Include="..\luo.dangxiao.controls\luo.dangxiao.controls.csproj" />
    <ProjectReference Include="..\luo.dangxiao.models\luo.dangxiao.models.csproj" />
    <ProjectReference Include="..\luo.dangxiao.resources\luo.dangxiao.resources.csproj" />
  </ItemGroup>

</Project>
```

### 7.2 Application Project Template

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>WinExe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <Nullable>enable</Nullable>
    <ApplicationManifest>app.manifest</ApplicationManifest>
    <AvaloniaUseCompiledBindingsByDefault>true</AvaloniaUseCompiledBindingsByDefault>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="Avalonia" Version="11.3.12" />
    <PackageReference Include="Avalonia.Desktop" Version="11.3.12" />
    <PackageReference Include="Avalonia.Diagnostics" Version="11.3.12" />
    <PackageReference Include="Avalonia.Fonts.Inter" Version="11.3.12" />
  </ItemGroup>

  <ItemGroup>
    <ProjectReference Include="..\luo.dangxiao.{module}\luo.dangxiao.{module}.csproj" />
  </ItemGroup>
</Project>
```

### 7.3 Common/Interfaces/Controls/Models Project Template

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>

</Project>
```

### 7.4 Resources Project Template

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>

  <ItemGroup>
    <PackageReference Include="Avalonia" Version="11.3.12" />
  </ItemGroup>

  <!-- Resource files configuration -->
  <ItemGroup>
    <EmbeddedResource Update="Languages\*.resx">
      <Generator>PublicResXFileCodeGenerator</Generator>
      <LastGenOutput>%(Filename).Designer.cs</LastGenOutput>
    </EmbeddedResource>
    <Compile Update="Languages\*.Designer.cs">
      <DesignTime>True</DesignTime>
      <AutoGen>True</AutoGen>
      <DependentUpon>%(Filename).resx</DependentUpon>
    </Compile>
  </ItemGroup>

  <!-- Images as content -->
  <ItemGroup>
    <None Update="Images\**\*">
      <CopyToOutputDirectory>PreserveNewest</CopyToOutputDirectory>
    </None>
  </ItemGroup>

  <!-- Styles as content -->
  <ItemGroup>
    <None Update="Styles\*.axaml">
      <CopyToOutputDirectory>PreserveNewest</CopyToOutputDirectory>
    </None>
  </ItemGroup>

</Project>
```

---

## 8. Documentation Standards

### 8.1 Required Documentation

**MUST document:**
- All public classes
- All public methods
- All public properties
- Complex algorithms
- Non-obvious behavior

**Documentation Format:**
```csharp
/// <summary>
/// Brief description of what this does
/// </summary>
/// <param name="paramName">Description of parameter</param>
/// <returns>Description of return value</returns>
/// <exception cref="ExceptionType">When thrown</exception>
```

### 8.2 File Header

Optional but recommended:
```csharp
// Copyright (c) luo.dangxiao. All rights reserved.
// Licensed under the MIT License.
```

---

## 9. Code Generation Rules for OpenCode

### 9.1 When Generating Views
1. Create both `.axaml` and `.axaml.cs` files
2. Place in `{module}/Views/` folder
3. Generate corresponding ViewModel in `{module}/ViewModels/`
4. Follow AXAML template exactly
5. Set `x:DataType` to ViewModel type

### 9.2 When Generating ViewModels
1. Inherit from `ViewModelBase`
2. Use `[ObservableProperty]` for bindable properties
3. Use `[RelayCommand]` for commands
4. Mark class as `partial`
5. Place in `{module}/ViewModels/` folder
6. Use base class `GetYktApiClient()` for API access (Section 5.3)
7. Use base class `IsApiSuccess()` for response validation (Section 5.3)
8. Use base class `EnsureApiSuccess()` for throw-on-failure scenarios
9. Use base class `ExtractJsonString()` for JsonElement? parsing

### 9.3 When Generating Utilities
1. Place in `luo.dangxiao.common/Utils/`
2. Use `static class` with `Util` suffix
3. Follow existing utility patterns

### 9.4 When Generating Enums
1. Place in `luo.dangxiao.common/Enum/`
2. Use `[Description]` attribute for display text
3. Use `EnumUtility.GetName()` for display

### 9.5 When Generating Converters
1. Place in `luo.dangxiao.common/Converter/`
2. Implement `IValueConverter`
3. Use `Converter` suffix

### 9.6 When Generating Models
1. Place in `luo.dangxiao.models/`
2. Use standard POCO pattern (template 5.10)
3. Include audit fields (Id, CreatedAt, ModifiedAt)
4. Use nullable types for optional properties

### 9.7 When Generating Localization Resources
1. Add entries to `luo.dangxiao.resources/Languages/Language.resx`
2. Add translated entries to `Language.zh-Hans.resx`
3. Follow naming convention: `Category_Descriptor` (e.g., `Button_OK`, `Msg_Success`)
4. Regenerate Designer.cs files

---

## 10. Validation Checklist

Before declaring code complete, verify:

- [ ] Project references follow dependency rules (Section 6)
- [ ] Namespaces match project names (Section 4.2)
- [ ] ViewModels inherit from `ViewModelBase`
- [ ] ViewModels use `GetYktApiClient()` for API calls (Section 5.3)
- [ ] ViewModels use `IsApiSuccess()` for response validation (Section 5.3)
- [ ] No inline `Ioc.Default.GetService<IYktApiClient>()` calls (Section 5.3 FORBIDDEN)
- [ ] No duplicate `EnsureApiSuccess` implementations in derived ViewModels
- [ ] ViewModels marked `partial`
- [ ] AXAML files have `x:DataType` declared
- [ ] Package versions match Section 1.1
- [ ] Code follows templates in Section 5
- [ ] Public APIs are documented (Section 8)
- [ ] All files in correct folders (Section 3)
- [ ] Models include standard audit fields (Id, CreatedAt, ModifiedAt)
- [ ] Localization resources updated for all supported cultures

---

## 11. Quick Reference

### Adding a New View + ViewModel

1. Create `Views/{Name}.axaml` (use template 5.3)
2. Create `Views/{Name}.axaml.cs` (use template 5.4)
3. Create `ViewModels/{Name}ViewModel.cs` (use template 5.1)
4. Update `.csproj` if needed (template 7.1)

### Adding a New Utility

1. Create `Utils/{Name}Util.cs` in `luo.dangxiao.common`
2. Use template 5.7
3. Reference from other projects via `luo.dangxiao.common.Utils`

### Adding a New Model

1. Create `Models/{Name}.cs` in `luo.dangxiao.models`
2. Use template 5.10
3. Include Id, CreatedAt, ModifiedAt fields
4. Reference from other projects via `luo.dangxiao.models`

### Adding Localization Resources

1. Add entries to `Languages/Language.resx` (English)
2. Add translations to `Languages/Language.zh-Hans.resx` (Simplified Chinese)
3. Follow naming: `Category_Descriptor` (e.g., `Button_OK`)
4. Reference in code via `luo.dangxiao.resources.Languages`

### Adding Color Resources

1. Create `Styles/Colors.axaml` in `luo.dangxiao.resources`
2. Define all color resources as `Color` elements with `x:Key`
3. Create corresponding `SolidColorBrush` and `LinearGradientBrush` resources
4. Reference in XAML via `{StaticResource ResourceKey}`
5. Include in App.axaml: `<StyleInclude Source="avares://luo.dangxiao.resources/Styles/Colors.axaml"/>`

**Color Naming Convention:**
- Primary colors: `Primary{Name}Color` (e.g., `PrimaryRedColor`)
- Secondary colors: `Secondary{Name}Color` (e.g., `SecondaryGoldColor`)
- Background colors: `Background{Name}Color` (e.g., `BackgroundCreamColor`)
- Text colors: `Text{Name}Color` (e.g., `TextPrimaryColor`)
- Status colors: `{Status}Color` (e.g., `SuccessColor`, `ErrorColor`)
- Brush resources: `{ColorName}Brush` (e.g., `PrimaryRedBrush`)

### Adding a Device Library (printer/cardreader pattern)

1. Create `luo.dangxiao.{device}/` with minimal csproj (template 7.3)
2. Create `Card{Device}Base.cs` — abstract base class at root level
3. Create `Card{Device}Models.cs` — shared data models and enums at root level
4. Create `Card{Device}{Provider}.cs` — provider enum at root level
5. Create `Card{Device}Factory.cs` — static factory at root level, `Create(Enum?)` with string switch
6. Create `Virtual/VirtualCard{Device}.cs` — simulated implementation
7. Create `{Vendor}/{Vendor}Card{Device}.cs` — hardware driver extending base class
8. If vendor has native SDK: create `{Vendor}/Native/` with P/Invoke wrappers
9. Update csproj: add `<None Update="{Vendor}/libs\**">` entries for .dll/.so copy-to-output
10. In App.axaml.cs: create instance via factory, register to DI or static property

### Adding a Device Config (in luo.dangxiao.models)

1. Create `DeviceConfig.cs` alongside `PrinterConfig.cs`
2. Include `DeviceProvider` enum, `DeviceProviderJsonConverter`, and `DeviceConfig` class
3. Follow `PrinterConfig` pattern exactly: JSON converter for graceful fallback, `ResolveProvider()` method
4. Add `DeviceConfig` property to `ConfigModel` (e.g., `ReaderConfig`)
5. In App.axaml.cs: load config, resolve provider, capture warnings

### Adding a New Module (e.g., admin)

1. Create `luo.dangxiao.admin/` (use template 7.1)
2. Create `luo.dangxiao.admin.app/` (use template 7.2)
3. Follow folder structure: `Views/`, `ViewModels/`
4. Add references per Section 6 rules

---

*Last Updated: 2026-04-29*  
*Maintainer: OpenCode Agent*  
*Version: 1.3*

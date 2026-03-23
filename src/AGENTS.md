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
├── luo.dangxiao.models/              # Data models (DTOs, Entities)
├── luo.dangxiao.resources/           # Resource files (Images, Languages, Styles)
├── luo.dangxiao.controls/            # Custom Avalonia controls
├── luo.dangxiao.selfservice/         # Self-service library (Views + ViewModels)
├── luo.dangxiao.selfservice.app/     # Self-service executable entry point
├── luo.dangxiao.cardcenter/          # Card center library (Views + ViewModels)
└── luo.dangxiao.cardcenter.app/      # Card center executable entry point
```

### 2.2 Project Roles & Responsibilities

| Project | Role | Output Type | References |
|---------|------|-------------|------------|
| `luo.dangxiao.common` | Shared utilities library | Library | None |
| `luo.dangxiao.interfaces` | Contract definitions | Library | None |
| `luo.dangxiao.models` | Data models (DTOs, Entities) | Library | None |
| `luo.dangxiao.resources` | Resource files (Images, Languages, Styles) | Library | Avalonia |
| `luo.dangxiao.controls` | Custom Avalonia controls | Library | Avalonia, luo.dangxiao.interfaces |
| `luo.dangxiao.selfservice` | Self-service UI module | Library | Avalonia, CommunityToolkit.Mvvm, luo.dangxiao.common, luo.dangxiao.interfaces, luo.dangxiao.models, luo.dangxiao.resources, luo.dangxiao.controls |
| `luo.dangxiao.selfservice.app` | Self-service application | WinExe | luo.dangxiao.selfservice, Avalonia.Desktop |
| `luo.dangxiao.cardcenter` | Card center UI module | Library | Avalonia, CommunityToolkit.Mvvm, luo.dangxiao.common, luo.dangxiao.interfaces, luo.dangxiao.models, luo.dangxiao.resources, luo.dangxiao.controls |
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
| View (AXAML) | PascalCase.axaml | `MainWindow.axaml` |
| View Code-behind | PascalCase.axaml.cs | `MainWindow.axaml.cs` |
| Utility | PascalCase | `DoubleUtil.cs` |
| Interface | IPascalCase | `IMainWindowViewModel.cs` |
| Enum | PascalCase | `ApplicationState.cs` |
| Converter | PascalCaseConverter | `BoolToVisibilityConverter.cs` |

### 4.4 Class Naming

| Type | Pattern | Example |
|------|---------|---------|
| ViewModel | {Name}ViewModel | `MainWindowViewModel` |
| View | {Name} | `MainWindow` (Window/UserControl) |
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

namespace luo.dangxiao.{module}.ViewModels;

/// <summary>
/// Base class for all ViewModels
/// </summary>
public abstract class ViewModelBase : ObservableObject
{
}
```

### 5.3 View Pattern (AXAML)

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

### 5.4 View Code-Behind

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

### 5.5 Application Entry Point (.app projects)

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

### 5.6 Application AXAML

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

### 5.7 Utility Class Pattern (luo.dangxiao.common)

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

### 5.8 Enum Pattern (luo.dangxiao.common)

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

### 5.9 Converter Pattern (luo.dangxiao.common)

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

### 5.10 Model Pattern (luo.dangxiao.models)

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

### 5.11 Localization Pattern (luo.dangxiao.resources)

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
| `luo.dangxiao.controls` | `luo.dangxiao.interfaces` |
| `luo.dangxiao.selfservice` | `luo.dangxiao.common`, `luo.dangxiao.interfaces`, `luo.dangxiao.models`, `luo.dangxiao.resources`, `luo.dangxiao.controls` |
| `luo.dangxiao.cardcenter` | `luo.dangxiao.common`, `luo.dangxiao.interfaces`, `luo.dangxiao.models`, `luo.dangxiao.resources`, `luo.dangxiao.controls` |
| `luo.dangxiao.selfservice.app` | `luo.dangxiao.selfservice` only |
| `luo.dangxiao.cardcenter.app` | `luo.dangxiao.cardcenter` only |

### 6.2 Reference Direction
```
.app -> Module -> controls -> interfaces
              -> models (DTOs/Entities)
              -> resources (Images/Languages/Styles)
              -> common (shared utilities)
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

### Adding a New Module (e.g., admin)

1. Create `luo.dangxiao.admin/` (use template 7.1)
2. Create `luo.dangxiao.admin.app/` (use template 7.2)
3. Follow folder structure: `Views/`, `ViewModels/`
4. Add references per Section 6 rules

---

*Last Updated: 2025-03-23*  
*Maintainer: OpenCode Agent*  
*Version: 1.1*

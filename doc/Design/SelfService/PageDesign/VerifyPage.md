# 验证页面设计文档 (VerifyPage)

> **页面代码**: VerifyPage  
> **适用用户**: 学员、教职工  
> **验证要求**: 无 (入口页面)  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| swipe_card_page | 现有设计 | 身份证验证模块参考 |
| sms_page | 现有设计 | 短信验证模块参考 |

### 1.2 模块页面

| 页面 | 说明 |
|------|------|
| [IDCardVerifyPage](./IDCardVerifyPage.md) | 身份证验证模块 |
| [SMSVerifyPage](./SMSVerifyPage.md) | 短信验证模块 |

---

## 二、页面布局结构

### 2.1 页面架构

VerifyPage 作为统一验证入口，嵌入两种验证模块：

```
┌─────────────────────────────────────┐
│  [此区域由HomePage顶部栏提供]        │
│  [Logo] 自助服务系统 [倒计时]       │
├─────────────────────────────────────┤
│                                     │
│  ┌─────────────────────────────┐   │
│  │      请选择验证方式         │   │
│  │                             │   │
│  │   [身份证验证] [短信验证]   │   │
│  │                             │   │
│  │  ┌─────────────────────┐   │   │
│  │  │                     │   │   │
│  │  │   验证模块区域       │   │   │
│  │  │                     │   │   │
│  │  │  • IDCardVerifyPage │   │   │
│  │  │  • SMSVerifyPage    │   │   │
│  │  │                     │   │   │
│  │  └─────────────────────┘   │   │
│  │                             │   │
│  └─────────────────────────────┘   │
│                                     │
├─────────────────────────────────────┤
│  [此区域由HomePage底部栏提供]        │
│  [← 返回]                           │
└─────────────────────────────────────┘
```

### 2.2 验证方式切换

```
┌─────────────────────────────────────┐
│  验证方式：                          │
│  ┌────────────┐  ┌────────────┐    │
│  │  身份证    │  │   短信     │    │
│  │  验证  ◄───┤  │   验证     │    │
│  └────────────┘  └────────────┘    │
│       选中状态        未选状态      │
├─────────────────────────────────────┤
│                                     │
│  ┌─────────────────────────────┐   │
│  │   请将二代身份证放置于      │   │
│  │      右下方感应区           │   │
│  │                             │   │
│  │   [身份证感应区域]          │   │
│  │                             │   │
│  └─────────────────────────────┘   │
│                                     │
└─────────────────────────────────────┘
```

---

## 三、UI 元素详解

### 3.1 验证方式选择

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 标题 | 顶部 | "请选择验证方式" | 黑体 36px |
| 身份证验证按钮 | 左侧 | 150×60px | 切换至身份证验证 |
| 短信验证按钮 | 右侧 | 150×60px | 切换至短信验证 |
| 选中样式 | - | 主题色背景 #d9230b | 表示当前选中 |
| 未选样式 | - | 白色背景，灰色边框 | 表示可切换 |

### 3.2 验证模块区域

**容器规格**:
| 属性 | 值 |
|------|-----|
| 位置 | 验证方式按钮下方 |
| 尺寸 | 占满可用宽度 |
| 背景 | #faf9f3 米黄色 |
| 圆角 | 7px |
| 内边距 | 40px |

**模块嵌入**:
- 选中"身份证验证"时，显示 [IDCardVerifyPage](./IDCardVerifyPage.md) 模块
- 选中"短信验证"时，显示 [SMSVerifyPage](./SMSVerifyPage.md) 模块

---

## 四、功能描述

### 4.1 页面用途

VerifyPage 是所有功能的统一身份验证入口页面。用户在 HomePage 点击功能按钮后，首先进入 VerifyPage 进行身份验证。验证通过后，根据功能类型自动跳转到对应的功能子页面。

### 4.2 核心功能

1. **验证方式选择**
   - 提供身份证验证和短信验证两种方式
   - 支持手动切换验证方式
   - 默认验证方式由系统参数配置

2. **模块嵌入**
   - 嵌入 IDCardVerifyPage 模块（身份证验证）
   - 嵌入 SMSVerifyPage 模块（短信验证）
   - 两个验证模块共享验证结果处理

3. **验证结果处理**
   - 验证成功：保存用户信息，跳转到对应功能页面
   - 验证失败：显示错误提示，允许重新验证
   - 验证通过后将用户信息传递给后续页面

### 4.3 验证流程

```
用户点击功能按钮（如"自助取卡"）
    ↓
HomePage 加载 VerifyPage
    ↓
显示默认验证方式（身份证/短信）
    ↓
用户完成验证
    ↓
验证成功
    ↓
保存用户信息到 Session
    ↓
根据功能类型跳转到对应页面
    （如：TakeCardPage）
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 显示身份证验证
    ↓ 点击"短信验证"按钮
状态 2: 显示短信验证
    ↓ 点击"身份证验证"按钮
状态 1: 显示身份证验证
```

### 5.2 详细交互

**1. 页面加载**:
- 从系统参数读取默认验证方式
- 显示对应的验证模块
- 启动倒计时计时器
- 高亮显示当前选中的验证方式按钮

**2. 切换验证方式**:
- 用户点击另一验证方式按钮
- 切换按钮选中状态
- 隐藏当前验证模块
- 显示新选择的验证模块
- 保留已输入的信息（如手机号）

**3. 身份证验证成功**:
- 读取身份证信息
- 验证身份合法性
- 获取用户类型（学员/教职工）
- 保存用户信息到 Session
- 触发验证成功事件
- 通知 HomePage 加载对应功能页面

**4. 短信验证成功**:
- 验证手机号和验证码
- 获取用户类型和信息
- 保存用户信息到 Session
- 触发验证成功事件
- 通知 HomePage 加载对应功能页面

**5. 返回操作**:
- 点击返回按钮
- 清空验证状态
- 返回 HomePage 主菜单状态

### 5.3 参数传递

```csharp
public class VerifyPageParameter
{
    public string TargetPage { get; set; }  // 验证通过后跳转的页面
    public string TargetFunction { get; set; }  // 功能类型（TakeCard/ReportLoss等）
    public UserType ExpectedUserType { get; set; }  // 期望的用户类型（可选）
}

public class VerifyResult
{
    public bool Success { get; set; }
    public UserInfo UserInfo { get; set; }
    public string ErrorMessage { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 标题 | #1a1919 |
| 选中按钮背景 | #d9230b |
| 选中按钮文字 | #ffffff |
| 未选按钮背景 | #ffffff |
| 未选按钮边框 | #c7c6bf |
| 未选按钮文字 | #70706d |
| 模块区域背景 | #faf9f3 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 标题 | 黑体 | 36px | bold |
| 按钮文字 | 黑体 | 28px | normal |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 标题距顶 | 40px |
| 按钮间距 | 20px |
| 按钮尺寸 | 150×60px |
| 模块区域距按钮 | 30px |
| 模块区域内边距 | 40px |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class VerifyViewModel : ViewModelBase
{
    [ObservableProperty]
    private VerifyMethod _currentMethod;
    
    [ObservableProperty]
    private object _verifyModuleContent;
    
    [ObservableProperty]
    private string _targetPage;
    
    public enum VerifyMethod
    {
        IDCard,
        SMS
    }
    
    partial void OnCurrentMethodChanged(VerifyMethod value)
    {
        switch (value)
        {
            case VerifyMethod.IDCard:
                VerifyModuleContent = new IDCardVerifyViewModel();
                break;
            case VerifyMethod.SMS:
                VerifyModuleContent = new SMSVerifyViewModel();
                break;
        }
    }
    
    // 处理验证成功
    private void OnVerificationSuccess(UserInfo user)
    {
        // 保存用户信息
        SessionService.CurrentUser = user;
        
        // 通知 HomePage 跳转
        NavigationService.NavigateTo(TargetPage);
    }
}
```

### 7.2 与 HomePage 交互

```csharp
// HomePage 中加载 VerifyPage
public void LoadVerifyPage(string targetPage)
{
    CurrentState = HomePageState.SubPageContainer;
    
    var parameter = new VerifyPageParameter
    {
        TargetPage = targetPage
    };
    
    SubPageContent = new VerifyPage { DataContext = new VerifyViewModel(parameter) };
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [身份证验证模块](./IDCardVerifyPage.md)
- [短信验证模块](./SMSVerifyPage.md)

---

*文档结束*

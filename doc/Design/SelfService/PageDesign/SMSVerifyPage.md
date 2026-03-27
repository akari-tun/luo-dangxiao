# 短信验证模块设计文档 (SMSVerifyPage)

> **页面代码**: SMSVerifyPage  
> **适用用户**: 学员、教职工  
> **页面类型**: 模块页面（嵌入 VerifyPage）  
> **文档版本**: 2.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

短信验证模块是一个可复用的验证模块，嵌入到 VerifyPage 中使用。模块提供手机号验证功能，用户输入手机号并获取验证码，输入验证码后完成身份验证。

### 1.2 设计图源文件

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| sms_page_1.png | `/doc/Design/SelfService/自助机/sms_page_1.png` | 初始状态 |
| sms_page_2.png | `/doc/Design/SelfService/自助机/sms_page_2.png` | 验证码已发送状态 |

### 1.3 切图资源

**位置**: `/doc/Design/SelfService/自助机裁图/`

| 文件名 | 规格 | 用途 |
|--------|------|------|
| - | - | 暂无专用切图资源，使用系统标准控件 |

---

## 二、模块架构

### 2.1 嵌入位置

SMSVerifyPage 作为模块嵌入到 VerifyPage 的验证模块区域：

```
VerifyPage (功能子页面)
├── 顶部栏 (HomePage 提供)
├── 验证方式选择 (VerifyPage)
│   ├── [身份证验证]
│   └── [短信验证] (选中)
├── 模块容器 (VerifyPage)
│   └── SMSVerifyPage (本模块)
│       ├── 手机号输入
│       ├── 验证码输入
│       └── 验证状态
└── 底部栏 (HomePage 提供)
```

### 2.2 模块布局

```
┌─────────────────────────────────────────┐
│  ▎短信验证                              │
│                                         │
│  手机号                                 │
│  ┌─────────────────────────────────┐   │
│  │ [________________________]      │   │
│  └─────────────────────────────────┘   │
│                                         │
│  验证码                                 │
│  ┌──────────────────┐ ┌────────────┐  │
│  │ [________]       │ │获取验证码  │  │
│  └──────────────────┘ └────────────┘  │
│                                         │
│                                         │
│       ┌───────────────────────┐        │
│       │       下一步          │        │
│       └───────────────────────┘        │
│                                         │
│  [切换到身份证验证 →]                   │
└─────────────────────────────────────────┘
```

---

## 三、UI 元素详解

### 3.1 模块容器

| 属性 | 值 |
|------|-----|
| 背景 | #faf9f3 米黄色 |
| 圆角 | 7px |
| 内边距 | 40px |
| 宽度 | 占满父容器 |

### 3.2 标题区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 红色竖线 | 标题左侧 | 6px宽，主题红色 | 区块标记 |
| 标题 | 竖线右侧 | "短信验证" | 黑体 36px bold, #1a1919 |
| 下边距 | - | 40px | 与表单间距 |

### 3.3 手机号输入区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 标签 | 输入框上方 | "手机号" | 黑体 28px, #70706d |
| 输入框 | 标签下方 | 100%宽度，60px高 | 手机号输入 |
| 占位符 | 输入框内 | "请输入手机号" | 黑体 24px, #c7c6bf |
| 边框 | 输入框 | 1px solid #c7c6bf, 5px圆角 | 标准输入框样式 |
| 键盘 | 数字键盘 | 限制11位数字 | 只允许输入数字 |
| 下边距 | - | 30px | 与验证码间距 |

### 3.4 验证码输入区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 标签 | 输入框上方 | "验证码" | 黑体 28px, #70706d |
| 验证码输入框 | 左侧 | 60%宽度，60px高 | 验证码输入（4-6位） |
| 获取按钮 | 右侧 | 40%宽度，60px高 | "获取验证码" |
| 按钮样式 | - | 主题红色渐变 | #d9230b |
| 按钮文字 | - | "获取验证码" / "重新获取 60秒" | 黑体 24px |
| 占位符 | 输入框内 | "请输入验证码" | 黑体 24px, #c7c6bf |
| 下边距 | - | 50px | 与下一步按钮间距 |

**倒计时状态**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 按钮背景 | 灰色 #e0e0e0 | 禁用状态 |
| 按钮文字 | "重新获取 {秒数}秒" | 黑体 24px, #70706d |
| 倒计时 | 60秒 | 发送后可重新获取间隔 |

### 3.5 下一步按钮

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 位置 | 中央 | 居中显示 | 主要操作按钮 |
| 尺寸 | 280×70px | - | 标准大按钮 |
| 背景 | 主题红色渐变 | #d9230b → #C42828 | 渐变效果 |
| 文字 | "下一步" | 黑体 32px bold | 白色 #ffffff |
| 圆角 | 8px | - | 圆角按钮 |
| 阴影 | 3px | #350200 | 投影效果 |
| 下边距 | - | 40px | 与切换链接间距 |

**禁用状态**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 背景 | 灰色 #e0e0e0 | 未输入完整时禁用 |
| 文字 | "下一步" | 黑体 32px, #a0a0a0 |

### 3.6 切换验证方式

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 切换链接 | 底部中央 | "切换到身份证验证 →" | 可点击文字 |
| 文字颜色 | - | #00345e 蓝色 | 链接样式 |
| 点击效果 | - | 下划线或颜色加深 | 悬停反馈 |

---

## 四、功能描述

### 4.1 模块用途

SMSVerifyPage 是一个嵌入在 VerifyPage 中的验证模块，提供短信验证码方式的身份验证。模块独立于顶部栏和底部栏，专注于短信验证的核心功能。

### 4.2 核心功能

1. **手机号输入**
   - 接收用户手机号输入
   - 验证手机号格式（11位数字）
   - 显示数字键盘

2. **验证码发送**
   - 发送短信验证码到手机号
   - 显示60秒倒计时
   - 倒计时结束前禁用重发

3. **验证码验证**
   - 接收用户输入的验证码
   - 验证验证码正确性
   - 验证手机号和验证码匹配

4. **身份获取**
   - 验证成功后获取用户信息
   - 判断用户类型（学员/教职工）
   - 返回验证结果

5. **验证切换**
   - 支持切换到身份证验证模块
   - 通知 VerifyPage 切换模块

### 4.3 验证流程

```
模块加载 → 输入手机号 → 点击获取验证码 → 接收短信 → 输入验证码 → 点击下一步 → 验证 → 通知父页面
                ↓              ↓
          格式验证      发送失败提示
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 初始状态
    ↓ 输入手机号（格式正确）
    ↓ 点击"获取验证码"
状态 2: 验证码已发送
    ↓ 输入验证码
    ↓ 点击"下一步"
状态 3: 验证中
    ↓ 验证完成
状态 4: 验证结果
    ├─ 验证通过 → 通知父页面 → 自动跳转
    └─ 验证失败 → 显示错误 → 返回状态 2
```

### 5.2 详细交互

**1. 模块加载**:
- 嵌入到 VerifyPage 的模块容器中
- 显示状态 1 (初始状态)
- 手机号和验证码输入框为空
- "获取验证码"按钮可点击（需先输入手机号）
- "下一步"按钮禁用

**2. 手机号输入**:
- 用户点击手机号输入框
- 弹出数字键盘
- 输入时实时验证格式（11位数字）
- 输入完整11位后，"获取验证码"按钮可用
- 格式不正确时显示提示

**3. 获取验证码**:
- 用户点击"获取验证码"
- 验证手机号格式
- 调用短信接口发送验证码
- 按钮变为"重新获取 60秒"，进入倒计时
- 按钮禁用，显示倒计时
- 显示提示"验证码已发送"

**4. 验证码输入**:
- 用户接收短信验证码
- 点击验证码输入框
- 输入收到的验证码（4-6位数字）
- 输入完成后"下一步"按钮可用

**5. 下一步点击**:
- 用户点击"下一步"
- 验证手机号和验证码
- 显示处理状态
- 等待验证结果

**6. 验证结果处理**:
- **成功**:
  - 显示成功状态
  - 保存用户信息
  - 触发验证成功事件
  - 通知 VerifyPage 验证成功
  - 延迟后自动跳转到功能页面
- **失败**:
  - 显示错误提示（验证码错误/过期等）
  - 清空验证码输入框
  - 返回状态 2

**7. 倒计时处理**:
- 60秒内按钮禁用
- 每秒更新倒计时显示
- 倒计时结束后按钮恢复"获取验证码"
- 可重新发送验证码

**8. 切换验证**:
- 点击"切换到身份证验证"
- 触发切换验证方式事件
- 通知 VerifyPage 切换到 IDCardVerifyPage

**9. 异常处理**:
- **手机号不存在**: 提示"手机号未注册"
- **验证码错误**: 提示"验证码错误，请重新输入"
- **验证码过期**: 提示"验证码已过期，请重新获取"
- **发送失败**: 提示"短信发送失败，请重试"
- **网络错误**: 提示"网络连接失败"

### 5.3 事件接口

```csharp
// 模块事件
public interface ISMSVerifyModule
{
    event EventHandler<VerificationSuccessEventArgs> OnVerificationSuccess;
    event EventHandler<VerificationFailedEventArgs> OnVerificationFailed;
    event EventHandler OnSwitchToIDCardRequested;
}

public class VerificationSuccessEventArgs : EventArgs
{
    public UserInfo UserInfo { get; set; }
    public UserType UserType { get; set; }
}

public class VerificationFailedEventArgs : EventArgs
{
    public string ErrorMessage { get; set; }
    public VerificationErrorType ErrorType { get; set; }
}

public enum VerificationErrorType
{
    InvalidPhone,    // 手机号格式错误
    PhoneNotFound,   // 手机号未注册
    CodeError,       // 验证码错误
    CodeExpired,     // 验证码过期
    SendFailed,      // 发送失败
    NetworkError     // 网络错误
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 模块背景 | #faf9f3 |
| 标题竖线 | #d9230b |
| 标题文字 | #1a1919 |
| 标签文字 | #70706d |
| 输入框边框 | #c7c6bf |
| 输入框聚焦 | #d9230b |
| 占位符 | #c7c6bf |
| 获取按钮背景 | #d9230b |
| 获取按钮文字 | #ffffff |
| 禁用按钮背景 | #e0e0e0 |
| 禁用按钮文字 | #70706d |
| 下一步按钮 | #d9230b → #C42828 |
| 下一步文字 | #ffffff |
| 切换链接 | #00345e |
| 成功颜色 | #4CAF50 |
| 错误颜色 | #d9230b |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 标题 | 黑体 | 36px | bold |
| 标签 | 黑体 | 28px | normal |
| 输入框占位符 | 黑体 | 24px | normal |
| 输入内容 | 黑体 | 28px | normal |
| 获取按钮 | 黑体 | 24px | normal |
| 下一步按钮 | 黑体 | 32px | bold |
| 切换链接 | 黑体 | 24px | normal |
| 倒计时数字 | Arial | 24px | normal |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 40px |
| 标题距顶 | 20px |
| 标题下边距 | 40px |
| 标签下边距 | 10px |
| 输入框高度 | 60px |
| 手机号与验证码间距 | 30px |
| 验证码输入与按钮间距 | 15px |
| 验证码区与下一步间距 | 50px |
| 下一步按钮尺寸 | 280×70px |
| 下一步与切换链接间距 | 40px |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class SMSVerifyViewModel : ViewModelBase, ISMSVerifyModule
{
    public event EventHandler<VerificationSuccessEventArgs> OnVerificationSuccess;
    public event EventHandler<VerificationFailedEventArgs> OnVerificationFailed;
    public event EventHandler OnSwitchToIDCardRequested;
    
    [ObservableProperty]
    private string _phoneNumber;
    
    [ObservableProperty]
    private string _verificationCode;
    
    [ObservableProperty]
    private int _countdownSeconds;
    
    [ObservableProperty]
    private bool _canSendCode;
    
    [ObservableProperty]
    private bool _canVerify;
    
    [ObservableProperty]
    private string _errorMessage;
    
    private Timer _countdownTimer;
    
    partial void OnPhoneNumberChanged(string value)
    {
        // 验证手机号格式（11位数字）
        CanSendCode = !string.IsNullOrEmpty(value) && 
                      value.Length == 11 && 
                      Regex.IsMatch(value, @"^\d{11}$");
        UpdateCanVerify();
    }
    
    partial void OnVerificationCodeChanged(string value)
    {
        UpdateCanVerify();
    }
    
    private void UpdateCanVerify()
    {
        CanVerify = !string.IsNullOrEmpty(PhoneNumber) && 
                    !string.IsNullOrEmpty(VerificationCode) &&
                    PhoneNumber.Length == 11;
    }
    
    [RelayCommand]
    private async Task SendVerificationCodeAsync()
    {
        if (!CanSendCode) return;
        
        try
        {
            var result = await SMSService.SendVerificationCodeAsync(PhoneNumber);
            
            if (result.Success)
            {
                // 开始60秒倒计时
                StartCountdown();
            }
            else
            {
                ErrorMessage = result.ErrorMessage;
            }
        }
        catch (Exception ex)
        {
            ErrorMessage = "短信发送失败，请重试";
        }
    }
    
    private void StartCountdown()
    {
        CountdownSeconds = 60;
        CanSendCode = false;
        
        _countdownTimer = new Timer(1000);
        _countdownTimer.Elapsed += (s, e) =>
        {
            CountdownSeconds--;
            
            if (CountdownSeconds <= 0)
            {
                _countdownTimer.Stop();
                CanSendCode = !string.IsNullOrEmpty(PhoneNumber) && PhoneNumber.Length == 11;
            }
        };
        _countdownTimer.Start();
    }
    
    [RelayCommand]
    private async Task VerifyAsync()
    {
        if (!CanVerify) return;
        
        try
        {
            var result = await VerifyService.VerifyBySMSAsync(PhoneNumber, VerificationCode);
            
            if (result.Success)
            {
                OnVerificationSuccess?.Invoke(this, new VerificationSuccessEventArgs
                {
                    UserInfo = result.UserInfo,
                    UserType = result.UserType
                });
            }
            else
            {
                ErrorMessage = result.ErrorMessage;
                OnVerificationFailed?.Invoke(this, new VerificationFailedEventArgs
                {
                    ErrorMessage = result.ErrorMessage,
                    ErrorType = result.ErrorType
                });
            }
        }
        catch (Exception ex)
        {
            ErrorMessage = "验证失败，请重试";
        }
    }
    
    [RelayCommand]
    private void SwitchToIDCard()
    {
        OnSwitchToIDCardRequested?.Invoke(this, EventArgs.Empty);
    }
    
    public void Cleanup()
    {
        _countdownTimer?.Stop();
        _countdownTimer?.Dispose();
    }
}
```

### 7.2 短信服务接口

```csharp
public interface ISMSService
{
    Task<SendCodeResult> SendVerificationCodeAsync(string phoneNumber);
    Task<VerifyResult> VerifyCodeAsync(string phoneNumber, string code);
}

public class SendCodeResult
{
    public bool Success { get; set; }
    public string ErrorMessage { get; set; }
}

public class VerifyResult
{
    public bool Success { get; set; }
    public UserInfo UserInfo { get; set; }
    public UserType UserType { get; set; }
    public string ErrorMessage { get; set; }
    public VerificationErrorType ErrorType { get; set; }
}
```

### 7.3 父页面交互

```csharp
// VerifyPage 中使用模块
public partial class VerifyViewModel : ViewModelBase
{
    [ObservableProperty]
    private object _verifyModuleContent;
    
    public void LoadSMSModule()
    {
        var module = new SMSVerifyViewModel();
        module.OnVerificationSuccess += OnSMSVerificationSuccess;
        module.OnVerificationFailed += OnSMSVerificationFailed;
        module.OnSwitchToIDCardRequested += OnSwitchToIDCard;
        
        VerifyModuleContent = module;
    }
    
    private void OnSMSVerificationSuccess(object sender, VerificationSuccessEventArgs e)
    {
        // 保存用户信息
        SessionService.CurrentUser = e.UserInfo;
        
        // 延迟后跳转到功能页面
        Dispatcher.UIThread.Post(async () =>
        {
            await Task.Delay(500);
            NavigationService.NavigateTo(TargetPage);
        });
    }
    
    private void OnSwitchToIDCard(object sender, EventArgs e)
    {
        // 切换到身份证验证模块
        LoadIDCardModule();
    }
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [验证页面](./VerifyPage.md) - 父页面
- [身份证验证模块](./IDCardVerifyPage.md) - 切换目标
- [学员信息模块](./StudentInfoPage.md)
- [教职工信息模块](./StaffInfoPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，作为独立页面设计 | OpenCode Agent |
| 2.0 | 2026-03-26 | 更新为模块页面：明确模块页面类型，描述嵌入 VerifyPage 的架构，更新布局为模块容器适配，添加模块事件接口，更新实现注意事项 | OpenCode Agent |

---

*文档结束*

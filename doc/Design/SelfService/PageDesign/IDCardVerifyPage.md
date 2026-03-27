# 身份证验证模块设计文档 (IDCardVerifyPage)

> **页面代码**: IDCardVerifyPage  
> **适用用户**: 学员、教职工  
> **页面类型**: 模块页面（嵌入 VerifyPage）  
> **文档版本**: 2.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

身份证验证模块是一个可复用的验证模块，嵌入到 VerifyPage 中使用。模块提供身份证读取和验证功能，用户放置身份证后系统自动读取并验证身份。

### 1.2 设计图源文件

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| swipe_card_page_1.png | `/doc/Design/SelfService/自助机/swipe_card_page_1.png` | 等待刷卡状态 |
| swipe_card_page_2.png | `/doc/Design/SelfService/自助机/swipe_card_page_2.png` | 处理中状态 |

### 1.3 切图资源

**位置**: `/doc/Design/SelfService/自助机裁图/swipe_card_page/`

| 文件名 | 用途 |
|--------|------|
| bg.png | 刷卡页背景 (浅色) |
| page1_layout.png | 等待刷卡状态布局参考 |
| page2_layout.png | 处理中状态布局参考 |
| swipe_id_card.png | 身份证刷卡示意图 |

### 1.4 相关设计说明

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| 身份证扫描切图说明.png | `/doc/Design/SelfService/切图说明/身份证扫描切图说明.png` | 布局说明图 |
| 刷身份证提示.png | `/doc/Design/SelfService/切图说明/刷身份证提示.png` | 刷卡提示说明 |

---

## 二、模块架构

### 2.1 嵌入位置

IDCardVerifyPage 作为模块嵌入到 VerifyPage 的验证模块区域：

```
VerifyPage (功能子页面)
├── 顶部栏 (HomePage 提供)
├── 验证方式选择 (VerifyPage)
│   ├── [身份证验证] (选中)
│   └── [短信验证]
├── 模块容器 (VerifyPage)
│   └── IDCardVerifyPage (本模块)
│       ├── 等待刷卡状态
│       ├── 处理中状态
│       └── 验证结果状态
└── 底部栏 (HomePage 提供)
```

### 2.2 模块布局

```
┌─────────────────────────────────────────┐
│  请将二代身份证放置于感应区             │
│                                         │
│  ┌─────────────────────────────────┐   │
│  │  二代身份证感应                 │   │
│  │                                 │   │
│  │   ┌─────────┐    ┌───────┐     │   │
│  │   │ [身份证]│ ↔  │[手形图]│     │   │
│  │   │ [波纹]  │    │       │     │   │
│  │   └─────────┘    └───────┘     │   │
│  │                                 │   │
│  │   [🔄 正在处理证件信息...]      │   │
│  │                                 │   │
│  └─────────────────────────────────┘   │
│                                         │
│  [切换到短信验证 →]                     │
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

### 3.2 操作指示区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 指示文字 | 顶部 | "请将二代身份证放置于感应区" | 黑体 36px bold, #d9230b |
| 下边距 | - | 30px | 与感应区间距 |

### 3.3 身份证感应区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 区域标题栏 | 感应区顶部 | 蓝色背景 #00345e | "二代身份证感应" |
| 标题文字 | 标题栏内 | 白色, 黑体 28px | "二代身份证感应" |
| 感应区背景 | 内容区中央 | 黑色/深色背景 | 身份证放置区域 |
| 身份证图示 | 感应区内 | 居中显示 | 中华人民共和国居民身份证样式 |
| 手形图标 | 身份证左侧 | 白色剪影 | 指示手持身份证动作 |
| 感应波纹 | 身份证两侧 | 白色弧线动画 | 表示无线感应区域 |
| 区域尺寸 | - | 400×250px | 感应区整体尺寸 |

### 3.4 状态提示框

**状态 1 - 等待刷卡**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 提示文字 | "等待读取身份证..." | 黑体 28px, #70706d |
| 动画 | 感应波纹动画 | 持续播放 |

**状态 2 - 处理中**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 提示框 | 浅蓝色/白色半透明背景 #e3f2fd | 状态提示容器 |
| Loading 图标 | 🔄 旋转动画 | 表示正在处理中 |
| 处理文字 | "正在处理证件信息，请稍候..." | 黑体 28px, #00345e |

**状态 3 - 验证成功**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 成功图标 | ✓ 大号图标 | #4CAF50 绿色 |
| 成功文字 | "验证成功" | 黑体 32px bold, #4CAF50 |
| 用户姓名 | "李天明" | 黑体 28px |

**状态 4 - 验证失败**:
| 元素 | 规格 | 说明 |
|------|------|------|
| 失败图标 | ✗ 图标 | #d9230b 红色 |
| 失败文字 | "验证失败" | 黑体 32px bold, #d9230b |
| 错误信息 | 具体错误原因 | 黑体 24px, #70706d |

### 3.5 切换验证方式

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 切换链接 | 底部中央 | "切换到短信验证 →" | 可点击文字 |
| 文字颜色 | - | #00345e 蓝色 | 链接样式 |
| 点击效果 | - | 下划线或颜色加深 | 悬停反馈 |

---

## 四、功能描述

### 4.1 模块用途

IDCardVerifyPage 是一个嵌入在 VerifyPage 中的验证模块，提供身份证读取和身份验证功能。模块独立于顶部栏和底部栏，专注于身份证验证的核心功能。

### 4.2 核心功能

1. **身份证感应**
   - 检测身份证放置
   - 读取身份证芯片信息
   - 解析身份证号码和姓名

2. **身份验证**
   - 与后台系统验证身份
   - 判断用户类型（学员/教职工）
   - 获取用户基本信息

3. **状态反馈**
   - 等待状态：感应波纹动画
   - 处理状态：Loading 动画
   - 成功状态：成功提示
   - 失败状态：错误提示

4. **验证切换**
   - 支持切换到短信验证模块
   - 通知 VerifyPage 切换模块

### 4.3 验证流程

```
模块加载 → 等待刷卡 → 检测到身份证 → 读取信息 → 验证身份 → 验证通过 → 通知父页面
                ↓
          验证失败 → 显示错误 → 返回等待状态
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 等待刷卡
    ↓ 检测到身份证
状态 2: 处理中
    ↓ 验证完成
状态 3: 验证结果
    ├─ 验证通过 → 通知父页面 → 自动跳转
    └─ 验证失败 → 显示错误 → 返回状态 1
```

### 5.2 详细交互

**1. 模块加载**:
- 嵌入到 VerifyPage 的模块容器中
- 显示状态 1 (等待刷卡)
- 初始化身份证读卡器
- 启动感应波纹动画

**2. 身份证感应**:
- 用户放置身份证到感应区
- 系统检测到身份证
- 切换到状态 2 (处理中)
- 显示 loading 动画和提示文字

**3. 身份验证**:
- 读取身份证信息
- 发送验证请求到后台
- 等待验证结果
- 显示处理状态

**4. 验证结果处理**:
- **成功**:
  - 显示成功图标和文字
  - 显示用户姓名
  - 保存用户信息
  - 触发验证成功事件
  - 通知 VerifyPage 验证成功
  - 延迟后自动跳转到功能页面
- **失败**:
  - 显示失败图标和错误信息
  - 3秒后自动返回状态 1
  - 或等待用户手动重试

**5. 切换验证**:
- 点击"切换到短信验证"
- 触发切换验证方式事件
- 通知 VerifyPage 切换到 SMSVerifyPage

**6. 异常处理**:
- **读卡失败**: 提示"读卡失败，请重新放置身份证"
- **身份不存在**: 提示"身份信息未找到"
- **网络错误**: 提示"网络连接失败"
- **读卡器故障**: 提示"设备故障"

### 5.3 事件接口

```csharp
// 模块事件
public interface IIDCardVerifyModule
{
    event EventHandler<VerificationSuccessEventArgs> OnVerificationSuccess;
    event EventHandler<VerificationFailedEventArgs> OnVerificationFailed;
    event EventHandler OnSwitchToSMSRequested;
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
    ReadFailed,     // 读卡失败
    NotFound,       // 身份不存在
    NetworkError,   // 网络错误
    DeviceError     // 设备故障
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 模块背景 | #faf9f3 |
| 指示文字 | #d9230b |
| 感应区标题栏 | #00345e |
| 感应区背景 | #1a1919 |
| 处理提示背景 | #e3f2fd |
| 处理文字 | #00345e |
| 成功颜色 | #4CAF50 |
| 失败颜色 | #d9230b |
| 切换链接 | #00345e |
| 波纹颜色 | #ffffff |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 指示文字 | 黑体 | 36px | bold |
| 感应区标题 | 黑体 | 28px | normal |
| 等待提示 | 黑体 | 28px | normal |
| 处理提示 | 黑体 | 28px | normal |
| 成功文字 | 黑体 | 32px | bold |
| 失败文字 | 黑体 | 32px | bold |
| 错误信息 | 黑体 | 24px | normal |
| 切换链接 | 黑体 | 24px | normal |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 40px |
| 指示文字距顶 | 20px |
| 感应区距指示文字 | 30px |
| 感应区尺寸 | 400×250px |
| 状态提示距感应区 | 20px |
| 切换链接距底 | 20px |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class IDCardVerifyViewModel : ViewModelBase, IIDCardVerifyModule
{
    public event EventHandler<VerificationSuccessEventArgs> OnVerificationSuccess;
    public event EventHandler<VerificationFailedEventArgs> OnVerificationFailed;
    public event EventHandler OnSwitchToSMSRequested;
    
    [ObservableProperty]
    private VerifyState _currentState;
    
    [ObservableProperty]
    private string _statusMessage;
    
    [ObservableProperty]
    private string _errorMessage;
    
    public enum VerifyState
    {
        Waiting,      // 等待刷卡
        Processing,   // 处理中
        Success,      // 验证成功
        Failed        // 验证失败
    }
    
    public async Task InitializeAsync()
    {
        CurrentState = VerifyState.Waiting;
        StatusMessage = "等待读取身份证...";
        
        // 初始化读卡器并订阅事件
        IDCardReader.CardDetected += OnCardDetected;
        await IDCardReader.InitializeAsync();
    }
    
    private async void OnCardDetected(object sender, IDCardEventArgs e)
    {
        CurrentState = VerifyState.Processing;
        StatusMessage = "正在处理证件信息，请稍候...";
        
        try
        {
            var cardInfo = await IDCardReader.ReadCardAsync();
            var result = await VerifyService.VerifyByIDCardAsync(cardInfo.IDNumber);
            
            if (result.Success)
            {
                CurrentState = VerifyState.Success;
                StatusMessage = $"验证成功 - {result.UserInfo.Name}";
                
                OnVerificationSuccess?.Invoke(this, new VerificationSuccessEventArgs
                {
                    UserInfo = result.UserInfo,
                    UserType = result.UserType
                });
            }
            else
            {
                CurrentState = VerifyState.Failed;
                ErrorMessage = result.ErrorMessage;
                
                OnVerificationFailed?.Invoke(this, new VerificationFailedEventArgs
                {
                    ErrorMessage = result.ErrorMessage,
                    ErrorType = result.ErrorType
                });
                
                // 3秒后返回等待状态
                await Task.Delay(3000);
                CurrentState = VerifyState.Waiting;
            }
        }
        catch (Exception ex)
        {
            CurrentState = VerifyState.Failed;
            ErrorMessage = "读卡失败，请重试";
        }
    }
    
    [RelayCommand]
    private void SwitchToSMS()
    {
        OnSwitchToSMSRequested?.Invoke(this, EventArgs.Empty);
    }
    
    public void Cleanup()
    {
        IDCardReader.CardDetected -= OnCardDetected;
    }
}
```

### 7.2 身份证读卡器集成

```csharp
public interface IIDCardReader
{
    event EventHandler<IDCardEventArgs> CardDetected;
    event EventHandler<IDCardEventArgs> CardRemoved;
    Task<bool> InitializeAsync();
    Task<IDCardInfo> ReadCardAsync();
    bool IsReady { get; }
}

public class IDCardInfo
{
    public string IDNumber { get; set; }
    public string Name { get; set; }
    public string Gender { get; set; }
    public DateTime BirthDate { get; set; }
    public byte[] Photo { get; set; }
}
```

### 7.3 父页面交互

```csharp
// VerifyPage 中使用模块
public partial class VerifyViewModel : ViewModelBase
{
    [ObservableProperty]
    private object _verifyModuleContent;
    
    public void LoadIDCardModule()
    {
        var module = new IDCardVerifyViewModel();
        module.OnVerificationSuccess += OnIDCardVerificationSuccess;
        module.OnVerificationFailed += OnIDCardVerificationFailed;
        module.OnSwitchToSMSRequested += OnSwitchToSMS;
        
        VerifyModuleContent = module;
    }
    
    private void OnIDCardVerificationSuccess(object sender, VerificationSuccessEventArgs e)
    {
        // 保存用户信息
        SessionService.CurrentUser = e.UserInfo;
        
        // 延迟后跳转到功能页面
        Dispatcher.UIThread.Post(async () =>
        {
            await Task.Delay(1500);
            NavigationService.NavigateTo(TargetPage);
        });
    }
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [验证页面](./VerifyPage.md) - 父页面
- [短信验证模块](./SMSVerifyPage.md) - 切换目标
- [学员信息模块](./StudentInfoPage.md)
- [教职工信息模块](./StaffInfoPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，作为独立页面设计 | OpenCode Agent |
| 2.0 | 2026-03-26 | 更新为模块页面：<br>• 明确模块页面类型<br>• 描述嵌入 VerifyPage 的架构<br>• 更新布局为模块容器适配<br>• 添加模块事件接口<br>• 更新实现注意事项 | OpenCode Agent |

---

*文档结束*

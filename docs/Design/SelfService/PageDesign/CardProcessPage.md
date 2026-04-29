# 卡片处理页面设计文档 (CardProcessPage)

> **页面代码**: CardProcessPage  
> **适用用户**: 学员、教职工  
> **验证要求**: 已通过身份验证  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

卡片处理页面用于展示卡片操作的实时过程，包括制卡、写卡、挂失、补卡、充值等操作的进度和状态。页面提供动画效果和实时状态更新，让用户了解当前操作进展。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| user_info_page | 现有设计 | 布局风格参考 |
| swipe_card_page | 现有设计 | 状态提示风格参考 |

---

## 二、页面布局结构

### 2.1 通用处理界面布局

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 120]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         卡片处理中              │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │   ┌─────────────┐     │    │ │
│ │    │   │             │     │    │ │
│ │    │   │   [卡片]    │     │    │ │
│ │    │   │   [动画]    │     │    │ │
│ │    │   │             │     │    │ │
│ │    │   └─────────────┘     │    │ │
│ │    │                       │    │ │
│ │    │   正在处理卡片...     │    │ │
│ │    │   请稍候              │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │ 进度：████████░░░ 70% │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    请勿离开，操作正在进行中     │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回(禁用)]                      │
└─────────────────────────────────────┘
```

### 2.2 制卡状态

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 118]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         🔄 正在制卡            │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │   [制卡机动画]        │    │ │
│ │    │   卡片正在打印...     │    │ │
│ │    │                       │    │ │
│ │    │   姓名：李天明        │    │ │
│ │    │   卡号：20200901001   │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    进度：正在打印持卡人信息     │ │
│ │                                 │ │
│ │    ⚠ 请勿触摸卡片             │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回(禁用)]                      │
└─────────────────────────────────────┘
```

### 2.3 写卡状态

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 115]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         💾 正在写卡            │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │   [IC卡接触动画]      │    │ │
│ │    │   正在写入芯片数据...  │    │ │
│ │    │                       │    │ │
│ │    │   操作：挂失           │    │ │
│ │    │   状态：写入中         │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    进度：████████████████ 100% │ │
│ │                                 │ │
│ │    即将完成...                 │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回(禁用)]                      │
└─────────────────────────────────────┘
```

### 2.4 充值写卡状态

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 110]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         💳 正在充值            │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │   [IC卡接触动画]      │    │ │
│ │    │   正在写入金额...     │    │ │
│ │    │                       │    │ │
│ │    │   充值金额：¥100.00   │    │ │
│ │    │   当前余额：¥50.00    │    │ │
│ │    │   充值后余额：¥150.00 │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    进度：████████████████ 95%  │ │
│ │                                 │ │
│ │    正在更新卡片余额...         │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回(禁用)]                      │
└─────────────────────────────────────┘
```

### 2.5 处理成功

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 108]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         ✓ 处理完成             │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │      [成功图标]       │    │ │
│ │    │        ✓              │    │ │
│ │    │                       │    │ │
│ │    │    卡片处理成功       │    │ │
│ │    │                       │    │ │
│ │    │    操作：充值         │    │ │
│ │    │    金额：¥100.00      │    │ │
│ │    │    新余额：¥150.00    │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    请取走您的卡片              │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│                  [下一步]           │
└─────────────────────────────────────┘
```

---

## 三、UI 元素详解

### 3.1 顶部栏 (Header)

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| Logo | 左上角 | 标准 Header 规格 | 党校党徽 |
| 主标题 | Logo 右侧 | 黑体 bold, #ffffff, 56px | "自助服务系统" |
| 副标题 | 主标题下方 | Arial, #ffffff, 24px | "SELF SERVICE SYSTEM" |
| 倒计时 | 左上角 Logo 旁 | Arial bold 28px, #ff6000 | 显示剩余秒数 |

### 3.2 内容区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 内容卡片 | 中央 | #faf9f3 背景，7px 圆角 | 内容容器 |
| 状态标题 | 卡片顶部 | 动态显示当前操作 | 黑体 36px |

### 3.3 动画区域

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 动画容器 | 中央 | 300×200px | 操作动画展示 |
| 卡片图标 | 动画内 | 根据操作类型变化 | 制卡/写卡动画 |
| 操作文字 | 动画下方 | "正在{操作}..." | 黑体 32px |

### 3.4 信息显示区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 操作类型 | 信息区 | "操作：{类型}" | 制卡/挂失/补卡/充值 |
| 进度条 | 信息区下方 | 水平进度条 | 显示处理进度 |
| 进度文字 | 进度条旁 | "进度：{描述}" | 详细进度说明 |
| 金额信息 | 充值时显示 | "充值金额/余额" | 充值操作特有 |

### 3.5 警告提示

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 警告图标 | 底部 | ⚠ 图标 | 提醒用户 |
| 警告文字 | 图标旁 | "请勿离开/请勿触摸" | 红色醒目文字 |

### 3.6 底部按钮

#### 处理中状态

| 按钮 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 返回 | 左下角 | 灰色禁用态 | 处理中不可返回 |

#### 完成状态

| 按钮 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 下一步 | 底部中央 | 大尺寸，主题色 | 进入完成界面 |

---

## 四、功能描述

### 4.1 页面用途

卡片处理页面用于展示卡片硬件操作的实时过程，包括制卡、写卡、挂失、补卡、充值等。页面与硬件设备交互，实时显示操作进度和状态，确保用户了解当前操作情况。

### 4.2 核心功能

1. **制卡**
   - 控制制卡机打印卡片
   - 显示制卡进度
   - 处理制卡异常

2. **写卡**
   - 写入IC卡芯片数据
   - 更新卡片状态
   - 验证写入结果

3. **挂失**
   - 将卡片标记为挂失状态
   - 更新系统记录
   - 禁用卡片功能

4. **补卡**
   - 制作新卡并写入数据
   - 转移旧卡余额和信息
   - 激活新卡

5. **充值**
   - 写入充值金额到卡片
   - 更新卡片余额
   - 记录交易信息

### 4.3 操作流程

```
接收操作参数
    ↓
初始化硬件
    ↓
开始处理
    ↓
实时更新进度
    ↓
处理完成
    ↓
验证结果
    ↓
进入完成界面
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 初始化
    ↓
状态 2: 处理中（制卡/写卡/挂失/充值）
    ↓ 处理成功
状态 3: 完成
    ↓ 点击"下一步"
完成界面 (CompletionPage)

或

状态 2: 处理中
    ↓ 处理失败
错误提示
    ↓
返回重试 / 联系管理员
```

### 5.2 详细交互

**1. 页面加载**:
- 接收操作参数（操作类型、用户信息、金额等）
- 初始化硬件设备连接
- 显示初始化状态
- 启动倒计时计时器

**2. 处理开始**:
- 根据操作类型显示对应标题
- 启动操作动画
- 发送指令到硬件设备
- 开始进度更新

**3. 进度更新**:
- 实时接收硬件状态反馈
- 更新进度条和进度文字
- 显示当前处理步骤
- 禁用返回按钮

**4. 制卡过程**:
- 显示"正在制卡"
- 展示制卡机动画
- 显示持卡人信息
- 进度：打印信息 → 覆膜 → 完成

**5. 写卡过程**:
- 显示"正在写卡"
- 展示IC卡接触动画
- 写入芯片数据
- 验证写入结果

**6. 充值过程**:
- 显示"正在充值"
- 展示充值金额信息
- 写入新余额到卡片
- 显示当前/新余额对比

**7. 处理完成**:
- 显示成功图标
- 显示操作结果
- 提示用户取卡（如需要）
- 显示"下一步"按钮

**8. 异常处理**:
- **设备故障**: 提示"设备故障，请联系管理员"
- **写卡失败**: 提示"写卡失败，请重试"
- **卡片异常**: 提示"卡片异常，请换卡重试"

### 5.3 硬件接口

```csharp
public interface ICardDevice
{
    Task<bool> InitializeAsync();
    Task<CardOperationResult> PrintCardAsync(CardData data);
    Task<CardOperationResult> WriteCardAsync(CardData data);
    Task<CardOperationResult> RechargeAsync(string cardId, decimal amount);
    Task<CardOperationResult> ReportLossAsync(string cardId);
    Task<DeviceStatus> GetStatusAsync();
    event EventHandler<ProgressEventArgs> OnProgress;
}

public class CardOperationResult
{
    public bool Success { get; set; }
    public string Message { get; set; }
    public int Progress { get; set; }
    public string CurrentStep { get; set; }
}

public enum CardOperationType
{
    Print,      // 制卡
    Write,      // 写卡
    Recharge,   // 充值
    ReportLoss, // 挂失
    Replace     // 补卡
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 标题 | #1a1919 |
| 状态文字 | #70706d |
| 操作类型 | #1a1919 |
| 进度条背景 | #e0e0e0 |
| 进度条填充 | #d9230b |
| 成功图标 | #4CAF50 |
| 警告文字 | #d9230b |
| 下一步按钮 | 主题红色渐变 |
| 倒计时 | #ff6000 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 状态标题 | 黑体 | 36px | bold |
| 操作文字 | 黑体 | 32px | normal |
| 信息文字 | 黑体 | 28px | normal |
| 进度文字 | 黑体 | 24px | normal |
| 警告文字 | 黑体 | 28px | bold |
| 按钮文字 | 黑体 | 36px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 标题距顶 | 40px |
| 动画区域距标题 | 30px |
| 动画区域尺寸 | 300×200px |
| 信息区距动画 | 30px |
| 进度条宽度 | 80%容器宽度 |
| 进度条高度 | 20px |
| 警告提示距底 | 30px |
| 按钮尺寸 | 270×76px |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class CardProcessViewModel : ViewModelBase
{
    [ObservableProperty]
    private CardOperationType _operationType;
    
    [ObservableProperty]
    private int _progress;
    
    [ObservableProperty]
    private string _currentStep;
    
    [ObservableProperty]
    private string _statusMessage;
    
    [ObservableProperty]
    private bool _isProcessing;
    
    [ObservableProperty]
    private bool _isCompleted;
    
    [ObservableProperty]
    private CardOperationResult _result;
    
    public async Task StartOperationAsync(CardProcessParameter parameter)
    {
        OperationType = parameter.OperationType;
        IsProcessing = true;
        IsCompleted = false;
        Progress = 0;
        
        // 订阅硬件进度事件
        CardDevice.OnProgress += OnDeviceProgress;
        
        try
        {
            CardOperationResult result;
            
            switch (OperationType)
            {
                case CardOperationType.Print:
                    result = await CardDevice.PrintCardAsync(parameter.CardData);
                    break;
                case CardOperationType.Write:
                    result = await CardDevice.WriteCardAsync(parameter.CardData);
                    break;
                case CardOperationType.Recharge:
                    result = await CardDevice.RechargeAsync(parameter.CardId, parameter.Amount);
                    break;
                case CardOperationType.ReportLoss:
                    result = await CardDevice.ReportLossAsync(parameter.CardId);
                    break;
                default:
                    throw new NotSupportedException();
            }
            
            Result = result;
            IsCompleted = result.Success;
            
            if (result.Success)
            {
                StatusMessage = "处理完成";
                Progress = 100;
            }
            else
            {
                StatusMessage = $"处理失败：{result.Message}";
            }
        }
        finally
        {
            CardDevice.OnProgress -= OnDeviceProgress;
            IsProcessing = false;
        }
    }
    
    private void OnDeviceProgress(object sender, ProgressEventArgs e)
    {
        Progress = e.Progress;
        CurrentStep = e.StepDescription;
        StatusMessage = $"正在{GetOperationName()}... {e.StepDescription}";
    }
}
```

### 7.2 进度更新

```csharp
public class ProgressEventArgs : EventArgs
{
    public int Progress { get; set; }
    public string StepDescription { get; set; }
}

// 硬件驱动中触发进度事件
public async Task<CardOperationResult> WriteCardAsync(CardData data)
{
    OnProgress?.Invoke(this, new ProgressEventArgs { Progress = 0, StepDescription = "初始化写卡器" });
    
    await InitializeWriterAsync();
    OnProgress?.Invoke(this, new ProgressEventArgs { Progress = 30, StepDescription = "连接卡片" });
    
    await ConnectCardAsync();
    OnProgress?.Invoke(this, new ProgressEventArgs { Progress = 60, StepDescription = "写入数据" });
    
    await WriteDataAsync(data);
    OnProgress?.Invoke(this, new ProgressEventArgs { Progress = 90, StepDescription = "验证数据" });
    
    var verified = await VerifyDataAsync();
    OnProgress?.Invoke(this, new ProgressEventArgs { Progress = 100, StepDescription = "完成" });
    
    return new CardOperationResult { Success = verified };
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [自助取卡页面](./TakeCardPage.md)
- [自助挂失页面](./ReportLossPage.md)
- [自助补卡页面](./ReplacementPage.md)
- [自助充值页面](./RechargePage.md)
- [操作完成界面](./CompletionPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，描述卡片处理流程和多种操作类型 | OpenCode Agent |

---

*文档结束*

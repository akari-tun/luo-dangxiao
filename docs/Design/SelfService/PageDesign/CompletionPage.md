# 操作完成界面设计文档 (CompletionPage)

> **页面代码**: CompletionPage  
> **适用用户**: 学员、教职工  
> **验证要求**: 已通过身份验证并完成业务操作  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

操作完成界面在现有设计图中未提供单独的设计图，但根据《客户端程序界面.docx》文档，该界面应沿用人员信息界面的布局风格，并做以下调整：

- 上部显示"操作完成"提示
- 中间根据自助类型展示教职工或学员信息
- 下部仅显示"退出"按钮

### 1.2 参考设计

| 参考设计 | 文件路径 | 说明 |
|----------|----------|------|
| user_info_page_1.jpg | `/doc/Design/自助机/user_info_page_1.jpg` | 布局风格参考 |
| special_tip_page.jpg | `/doc/Design/自助机/special_tip_page.jpg` | 成功提示风格参考 |

---

## 二、页面布局结构

### 2.1 学员操作完成

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 60]  SELF SERVICE SYSTEM       │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         ✓ 操作完成              │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │  姓名：李天明         │    │ │
│ │    │  班级：测试培训班一   │    │ │
│ │    │  房间：A312           │    │ │
│ │    │  (根据操作类型显示)   │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│           [退 出]                    │
└─────────────────────────────────────┘
```

### 2.2 教职工操作完成

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 60]  SELF SERVICE SYSTEM       │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         ✓ 操作完成              │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │  姓名：张三           │    │ │
│ │    │  部门：行政处         │    │ │
│ │    │  卡号：12345678       │    │ │
│ │    │  (根据操作类型显示)   │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│           [退 出]                    │
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
| 倒计时 | 左上角 Logo 旁 | Arial bold 28px, #ff6000 | 显示剩余秒数 (较短，如 60秒) |

### 3.2 成功提示区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 成功图标 | 内容区上部中央 | ✓ 勾选标记，大号 | 绿色或主题色 |
| 提示文字 | 图标下方 | "操作完成" | 黑体 48px bold |
| 文字颜色 | - | #1a1919 或主题绿色 | 成功状态色 |

### 3.3 信息展示区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 信息卡片 | 成功提示下方 | #faf9f3 背景，7px 圆角 | 信息容器 |
| 信息列表 | 卡片内 | 标签:内容 格式 | 关键信息展示 |

#### 3.3.1 学员信息展示 (根据操作类型)

| 操作类型 | 显示信息 |
|----------|----------|
| 自助报到 | 姓名、班级、房间、报到时间 |
| 自助取卡 | 姓名、班级、卡号、取卡时间 |
| 自助缴费 | 姓名、缴费项目、金额、缴费时间 |
| 自助挂失 | 姓名、卡号、挂失时间、状态 |
| 自助补卡 | 姓名、卡号、补卡时间、新卡号 |

#### 3.3.2 教职工信息展示 (根据操作类型)

| 操作类型 | 显示信息 |
|----------|----------|
| 自助取卡 | 姓名、部门、卡号、取卡时间 |
| 自助挂失 | 姓名、部门、卡号、挂失时间 |
| 自助补卡 | 姓名、部门、新卡号、补卡时间 |
| 自助查询 | 姓名、卡号、余额、状态 |
| 自助充值 | 姓名、充值金额、余额、时间 |

### 3.4 退出按钮

| 属性 | 值 |
|------|-----|
| 位置 | 底部中央 |
| 尺寸 | 285×66px (大尺寸) |
| 文字 | "退出" |
| 字体 | 黑体 36px bold |
| 文字颜色 | #ffffff |
| 背景 | 主题红色渐变 #C42828 → #d9230b |
| 圆角 | 8px |
| 投影 | 3px, #260100 |

---

## 四、功能描述

### 4.1 页面用途

操作完成界面是业务流程的最终页面，用于向用户确认操作已成功完成，并展示相关的结果信息。用户确认后可退出当前自助流程，返回系统初始状态。

### 4.2 核心功能

1. **操作结果确认**
   - 显示"操作完成"成功提示
   - 展示操作成功的视觉反馈
   - 提供清晰的成功状态指示

2. **信息展示**
   - 根据操作类型展示相关信息
   - 展示关键操作结果数据
   - 提供用户确认凭据

3. **退出导航**
   - 提供退出按钮
   - 点击后返回主界面
   - 清空会话数据

4. **倒计时管理**
   - 显示自动退出倒计时
   - 超时自动返回主界面
   - 倒计时通常较短 (如 60秒)

### 4.3 不同操作的信息展示

#### 4.3.1 报到完成

```
✓ 操作完成

┌──────────────────┐
│ 姓名：李天明     │
│ 班级：测试培训班一│
│ 房间：A312       │
│ 报到时间：2020/09/01 09:30 │
└──────────────────┘
```

#### 4.3.2 缴费完成

```
✓ 操作完成

┌──────────────────┐
│ 姓名：李天明     │
│ 缴费项目：培训费、住宿费 │
│ 缴费金额：7100元 │
│ 缴费时间：2020/09/01 09:30 │
│ 交易单号：P20200901001 │
└──────────────────┘
```

#### 4.3.3 取卡完成

```
✓ 操作完成

┌──────────────────┐
│ 姓名：李天明     │
│ 卡号：20200901001│
│ 取卡时间：2020/09/01 09:30 │
│ 提示：请妥善保管卡片 │
└──────────────────┘
```

#### 4.3.4 充值完成 (教职工)

```
✓ 操作完成

┌──────────────────┐
│ 姓名：张三       │
│ 充值金额：500元  │
│ 当前余额：1200元 │
│ 充值时间：2020/09/01 09:30 │
└──────────────────┘
```

---

## 五、交互描述

### 5.1 交互流程

```
业务操作完成 → 显示完成页面 → 查看信息 → 点击退出 → 返回主界面
                                      ↓
                                 倒计时超时
                                      ↓
                                 自动返回主界面
```

### 5.2 详细交互

**1. 页面加载**:
- 接收操作结果参数
- 根据操作类型确定展示信息
- 启动倒计时计时器 (默认 60秒)
- 显示成功提示和信息

**2. 查看信息**:
- 用户查看操作结果
- 确认操作成功
- 查看相关详细信息

**3. 退出操作**:
- 用户点击"退出"按钮
- 清空会话数据
- 返回主界面 (HomePage)
- 重置系统状态

**4. 倒计时超时**:
- 倒计时归零
- 自动清空会话
- 自动返回主界面

### 5.3 参数传递

```csharp
public class CompletionPageParameter
{
    public string OperationType { get; set; }      // 操作类型
    public UserType UserType { get; set; }         // 用户类型
    public OperationResult Result { get; set; }    // 操作结果
    public Dictionary<string, string> Info { get; set; }  // 展示信息
    public int CountdownSeconds { get; set; }      // 倒计时秒数
}

public enum OperationResult
{
    Success,   // 成功
    Partial,   // 部分成功
    Failed     // 失败 (一般不使用完成页)
}
```

### 5.4 信息构建

```csharp
public Dictionary<string, string> BuildInfo(OperationType type, UserInfo user)
{
    var info = new Dictionary<string, string>();
    
    switch (type)
    {
        case OperationType.CheckIn:
            info["姓名"] = user.Name;
            info["班级"] = user.ClassName;
            info["房间"] = user.RoomNumber;
            info["报到时间"] = DateTime.Now.ToString("yyyy/MM/dd HH:mm");
            break;
            
        case OperationType.Payment:
            info["姓名"] = user.Name;
            info["缴费项目"] = string.Join(",", user.SelectedItems);
            info["缴费金额"] = $"{user.TotalAmount}元";
            info["缴费时间"] = DateTime.Now.ToString("yyyy/MM/dd HH:mm");
            break;
            
        case OperationType.TakeCard:
            info["姓名"] = user.Name;
            info["卡号"] = user.CardNumber;
            info["取卡时间"] = DateTime.Now.ToString("yyyy/MM/dd HH:mm");
            break;
            
        // 其他操作类型...
    }
    
    return info;
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 成功图标 | #4CAF50 (绿色) 或主题红色 |
| 完成提示文字 | #1a1919 |
| 信息卡片背景 | #faf9f3 |
| 信息标签 | #70706d |
| 信息内容 | #1a1919 |
| 退出按钮背景 | #C42828 → #d9230b |
| 退出按钮文字 | #ffffff |
| 倒计时 | #ff6000 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 完成提示 | 黑体 | 48px | bold |
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 32px | normal |
| 退出按钮 | 黑体 | 36px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 成功图标距顶 | 60px |
| 完成提示距图标 | 30px |
| 信息卡片距提示 | 50px |
| 信息行间距 | 20px |
| 退出按钮距卡片 | 60px |
| 退出按钮尺寸 | 285×66px |
| 退出按钮圆角 | 8px |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class CompletionViewModel : ViewModelBase
{
    [ObservableProperty]
    private string _operationTitle;
    
    [ObservableProperty]
    private ObservableCollection<InfoItem> _infoItems;
    
    [ObservableProperty]
    private int _countdownSeconds;
    
    [ObservableProperty]
    private bool _isSuccess;
    
    public void Initialize(CompletionPageParameter parameter)
    {
        OperationTitle = "操作完成";
        IsSuccess = parameter.Result == OperationResult.Success;
        CountdownSeconds = parameter.CountdownSeconds;
        
        InfoItems = new ObservableCollection<InfoItem>();
        foreach (var item in parameter.Info)
        {
            InfoItems.Add(new InfoItem 
            { 
                Label = item.Key, 
                Value = item.Value 
            });
        }
        
        StartCountdown();
    }
    
    [RelayCommand]
    private async Task ExitAsync()
    {
        // 清空会话
        SessionService.Clear();
        
        // 返回主界面
        await NavigationService.NavigateToAsync("HomePage");
    }
}

public class InfoItem
{
    public string Label { get; set; }
    public string Value { get; set; }
}
```

### 7.2 自动退出

```csharp
private void StartCountdown()
{
    _timer = new Timer(1000);
    _timer.Elapsed += async (s, e) =>
    {
        CountdownSeconds--;
        
        if (CountdownSeconds <= 0)
        {
            _timer.Stop();
            await ExitAsync();
        }
    };
    _timer.Start();
}
```

### 7.3 打印凭据 (可选)

```csharp
[RelayCommand]
private async Task PrintReceiptAsync()
{
    var receipt = new Receipt
    {
        Title = "操作完成凭据",
        OperationType = OperationType,
        Items = InfoItems.ToList(),
        Timestamp = DateTime.Now
    };
    
    await PrinterService.PrintAsync(receipt);
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [学员信息界面](./UserInfoPage.md)
- [特别提示页](./SpecialTipPage.md)

---

*文档结束*

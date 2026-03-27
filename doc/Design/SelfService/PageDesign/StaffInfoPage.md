# 教职工信息模块设计文档 (StaffInfoPage)

> **页面代码**: StaffInfoPage  
> **适用用户**: 教职工  
> **页面类型**: 模块页面（嵌入功能子页面）  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

教职工信息模块是一个可复用的信息展示模块，嵌入到需要展示教职工信息的功能子页面中。模块展示教职工的基本信息、部门信息、卡片信息、余额信息等。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| user_info_page_2.jpg | 自助机/ | 教职工信息展示参考 |
| 基本信息切图说明.png | 切图说明/ | 布局切图说明 |

---

## 二、模块布局结构

### 2.1 标准信息卡片布局

```
┌─────────────────────────────────────────┐
│  教职工信息                              │
├─────────────────────────────────────────┤
│                                         │
│   姓名：张明华          性别：男        │
│                                         │
│   身份证号：430101198502031234          │
│                                         │
│   部门：教务处                          │
│                                         │
│   工号：T2020001                        │
│                                         │
│   卡号：2020001001                      │
│                                         │
│   卡片状态：正常                        │
│                                         │
│   卡片余额：¥125.50                     │
│                                         │
│   手机号：13800138000                   │
│                                         │
└─────────────────────────────────────────┘
```

### 2.2 取卡/挂失/补卡场景布局

```
┌─────────────────────────────────────────┐
│  教职工信息                              │
├─────────────────────────────────────────┤
│                                         │
│   ┌──────────┐   姓名：张明华          │
│   │          │   性别：男              │
│   │  照片    │                         │
│   │          │   部门：教务处          │
│   │  120×150 │                        │
│   │          │   工号：T2020001        │
│   │          │                         │
│   └──────────┘   卡号：2020001001      │
│                                         │
│   卡片状态：待领取                      │
│                                         │
│   卡片余额：¥0.00                       │
│                                         │
└─────────────────────────────────────────┘
```

### 2.3 充值场景布局

```
┌─────────────────────────────────────────┐
│  教职工信息                              │
├─────────────────────────────────────────┤
│                                         │
│   姓名：张明华          部门：教务处    │
│                                         │
│   工号：T2020001                        │
│                                         │
│   卡号：2020001001                      │
│                                         │
│   卡片状态：正常                        │
│                                         │
│   ┌─────────────────────────────────┐   │
│   │      当前余额：¥125.50         │   │
│   │                                 │   │
│   │      充值金额：¥100.00         │   │
│   │                                 │   │
│   │      充值后余额：¥225.50       │   │
│   │              ★                │   │
│   └─────────────────────────────────┘   │
│                                         │
│   请确认充值金额是否正确                │
│                                         │
└─────────────────────────────────────────┘
```

### 2.4 查询场景布局（完整信息）

```
┌─────────────────────────────────────────┐
│  教职工信息                              │
├─────────────────────────────────────────┤
│                                         │
│   姓名：张明华          性别：男        │
│                                         │
│   身份证号：430101198502031234          │
│                                         │
│   部门：教务处                          │
│                                         │
│   工号：T2020001                        │
│                                         │
│   ┌─────────────────────────────────┐   │
│   │        卡片信息                 │   │
│   │                                 │   │
│   │   卡号：2020001001             │   │
│   │   状态：正常                   │   │
│   │   余额：¥125.50                │   │
│   │   发卡日期：2020-01-01         │   │
│   │   有效期至：2025-12-31         │   │
│   │                                 │   │
│   └─────────────────────────────────┘   │
│                                         │
│   手机号：13800138000                   │
│                                         │
└─────────────────────────────────────────┘
```

---

## 三、UI 元素详解

### 3.1 模块容器

| 属性 | 值 |
|------|-----|
| 背景 | #ffffff 白色 |
| 边框 | 1px solid #e0e0e0 |
| 圆角 | 7px |
| 内边距 | 30px |
| 外边距 | 根据父页面调整 |

### 3.2 标题区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 标题 | 顶部 | "教职工信息" | 黑体 32px bold |
| 分隔线 | 标题下方 | 1px solid #e0e0e0 | 分隔标题和内容 |

### 3.3 信息字段布局

**标准布局（双列）**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 标签 | 左侧 | 黑体 28px, #70706d | 如"姓名：" |
| 内容 | 标签右侧 | 黑体 28px, #1a1919 | 如"张明华" |
| 行间距 | - | 25px | 字段间垂直间距 |

**单行长字段**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 标签+内容 | 整行 | 黑体 28px | 身份证号等长字段 |

### 3.4 照片区域（可选）

| 属性 | 值 |
|------|-----|
| 尺寸 | 120×150px |
| 位置 | 左侧 |
| 边框 | 1px solid #c7c6bf |
| 背景 | #f5f5f5 |
| 占位图 | 默认头像图标 |

### 3.5 状态标签

| 状态 | 颜色 | 说明 |
|------|------|------|
| 正常 | #4CAF50 | 卡片状态正常 |
| 待领取 | #ff6000 | 卡片待领取 |
| 已挂失 | #d9230b | 卡片已挂失 |
| 已冻结 | #d9230b | 卡片已冻结 |

### 3.6 余额显示

| 场景 | 样式 | 说明 |
|------|------|------|
| 普通显示 | 黑体 28px, #1a1919 | 正常余额 |
| 充值预览 | 黑体 32px bold, #d9230b | 充值金额高亮 |
| 余额变化 | 动画过渡 | 数字变化动画 |

### 3.7 卡片信息区（可选）

| 属性 | 值 |
|------|-----|
| 背景 | #faf9f3 |
| 边框 | 1px solid #e6b84d |
| 圆角 | 5px |
| 内边距 | 20px |
| 标题 | 黑体 30px bold, #1a1919 |
| 信息项 | 黑体 28px |
| 余额数字 | 黑体 32px bold, #d9230b |

### 3.8 充值预览区

| 属性 | 值 |
|------|-----|
| 背景 | #fff8e1 金黄色浅色 |
| 边框 | 2px solid #e6b84d |
| 圆角 | 5px |
| 内边距 | 25px |
| 当前余额 | 黑体 28px |
| 充值金额 | 黑体 32px bold, #d9230b |
| 充值后余额 | 黑体 32px bold, #4CAF50 |
| 提示文字 | 黑体 28px, #70706d |

---

## 四、功能描述

### 4.1 模块用途

StaffInfoPage 是一个可复用的信息展示模块，用于在功能子页面中展示教职工的详细信息。模块根据使用场景可以显示不同组合的信息字段，支持充值金额的预览展示。

### 4.2 显示模式

**模式 1: 标准信息模式**
- 显示基本身份信息
- 显示部门信息
- 显示卡片信息和余额
- 用于：验证后的信息确认

**模式 2: 照片信息模式**
- 显示教职工照片
- 显示基本信息
- 显示卡片状态
- 用于：取卡、挂失、补卡

**模式 3: 充值预览模式**
- 显示基本信息
- 显示当前余额
- 显示充值金额
- 显示充值后余额预览
- 用于：充值确认

**模式 4: 完整信息模式**
- 显示所有信息
- 显示详细卡片信息
- 用于：查询

### 4.3 数据字段

| 字段名 | 说明 | 显示条件 |
|--------|------|----------|
| 姓名 | 教职工姓名 | 始终显示 |
| 性别 | 男/女 | 始终显示 |
| 身份证号 | 完整身份证号 | 始终显示 |
| 部门 | 所属部门 | 始终显示 |
| 工号 | 教职工工号 | 始终显示 |
| 卡号 | 教职工卡号 | 有卡时显示 |
| 卡片状态 | 正常/待领取/已挂失等 | 有卡时显示 |
| 卡片余额 | 当前余额 | 有卡时显示 |
| 发卡日期 | 卡片发放日期 | 完整信息模式 |
| 有效期至 | 卡片有效期 | 完整信息模式 |
| 手机号 | 联系手机 | 始终显示 |
| 照片 | 教职工照片 | 照片模式 |

---

## 五、交互描述

### 5.1 模块加载

```
接收参数
    ↓
根据模式确定显示字段
    ↓
加载教职工数据
    ↓
渲染信息卡片
    ↓
显示在父页面中
```

### 5.2 详细交互

**1. 参数接收**:
- 从父页面接收显示模式参数
- 接收教职工ID或教职工信息对象
- 接收可选的充值金额（充值模式）

**2. 数据加载**:
- 根据教职工ID查询详细信息
- 或直接使用传入的教职工信息
- 格式化数据显示

**3. 动态字段**:
- 根据显示模式隐藏/显示字段
- 根据数据状态显示不同颜色标签
- 照片区域根据模式决定是否显示

**4. 充值预览**:
- 显示当前余额
- 显示充值金额（从参数获取）
- 计算并显示充值后余额
- 充值金额高亮显示

**5. 余额动画**:
- 充值成功后显示余额变化动画
- 数字从旧余额滚动到新余额
- 使用过渡动画效果

**6. 刷新机制**:
- 支持外部触发刷新
- 刷新后更新显示数据
- 保持当前显示模式

### 5.3 参数定义

```csharp
public class StaffInfoPageParameter
{
    public string StaffId { get; set; }
    public StaffInfo Data { get; set; }  // 可选，直接传入数据
    public DisplayMode Mode { get; set; }
    public decimal? RechargeAmount { get; set; }  // 充值模式使用
}

public enum DisplayMode
{
    Standard,        // 标准信息模式
    WithPhoto,       // 照片信息模式
    RechargePreview, // 充值预览模式
    FullInfo         // 完整信息模式
}

public class StaffInfo
{
    public string Id { get; set; }
    public string Name { get; set; }
    public string Gender { get; set; }
    public string IdCardNumber { get; set; }
    public string Department { get; set; }
    public string EmployeeNumber { get; set; }
    public string CardNumber { get; set; }
    public CardStatus CardStatus { get; set; }
    public decimal CardBalance { get; set; }
    public DateTime? CardIssueDate { get; set; }
    public DateTime? CardExpiryDate { get; set; }
    public string PhoneNumber { get; set; }
    public string PhotoUrl { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 模块背景 | #ffffff |
| 模块边框 | #e0e0e0 |
| 标题文字 | #1a1919 |
| 信息标签 | #70706d |
| 信息内容 | #1a1919 |
| 分隔线 | #e0e0e0 |
| 照片边框 | #c7c6bf |
| 状态-正常 | #4CAF50 |
| 状态-警告 | #ff6000 |
| 状态-错误 | #d9230b |
| 卡片信息区边框 | #e6b84d |
| 卡片信息区背景 | #faf9f3 |
| 充值预览区背景 | #fff8e1 |
| 充值金额 | #d9230b |
| 余额数字 | #1a1919 |
| 充值后余额 | #4CAF50 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 模块标题 | 黑体 | 32px | bold |
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 28px | normal |
| 卡片信息标题 | 黑体 | 30px | bold |
| 余额数字 | 黑体 | 32px | bold |
| 充值金额 | 黑体 | 32px | bold |
| 状态标签 | 黑体 | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 30px |
| 标题下边距 | 20px |
| 信息行间距 | 25px |
| 标签与内容间距 | 10px |
| 照片与信息间距 | 30px |
| 卡片信息区内边距 | 20px |
| 卡片信息区上边距 | 25px |
| 充值预览区内边距 | 25px |

### 6.4 尺寸规范

| 元素 | 尺寸 |
|------|------|
| 模块最小宽度 | 500px |
| 照片尺寸 | 120×150px |
| 信息列宽 | 左列30%，右列70% |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class StaffInfoViewModel : ViewModelBase
{
    [ObservableProperty]
    private StaffInfo _staffInfo;
    
    [ObservableProperty]
    private DisplayMode _currentMode;
    
    [ObservableProperty]
    private decimal? _rechargeAmount;
    
    [ObservableProperty]
    private decimal _balanceAfterRecharge;
    
    [ObservableProperty]
    private bool _showPhoto;
    
    [ObservableProperty]
    private bool _showCardInfo;
    
    [ObservableProperty]
    private bool _showRechargePreview;
    
    partial void OnCurrentModeChanged(DisplayMode value)
    {
        ShowPhoto = value == DisplayMode.WithPhoto;
        ShowCardInfo = value == DisplayMode.FullInfo;
        ShowRechargePreview = value == DisplayMode.RechargePreview;
    }
    
    partial void OnRechargeAmountChanged(decimal? value)
    {
        if (value.HasValue && StaffInfo != null)
        {
            BalanceAfterRecharge = StaffInfo.CardBalance + value.Value;
        }
    }
    
    public async Task LoadDataAsync(StaffInfoPageParameter parameter)
    {
        CurrentMode = parameter.Mode;
        RechargeAmount = parameter.RechargeAmount;
        
        if (parameter.Data != null)
        {
            StaffInfo = parameter.Data;
        }
        else if (!string.IsNullOrEmpty(parameter.StaffId))
        {
            StaffInfo = await StaffService.GetStaffInfoAsync(parameter.StaffId);
        }
        
        if (RechargeAmount.HasValue)
        {
            BalanceAfterRecharge = StaffInfo.CardBalance + RechargeAmount.Value;
        }
    }
    
    // 余额变化动画
    public async Task AnimateBalanceChange(decimal newBalance)
    {
        var oldBalance = StaffInfo.CardBalance;
        StaffInfo.CardBalance = newBalance;
        
        // 触发属性变更通知，触发UI动画
        OnPropertyChanged(nameof(StaffInfo));
    }
    
    // 状态颜色转换
    public string GetStatusColor(CardStatus status)
    {
        return status switch
        {
            CardStatus.Normal => "#4CAF50",
            CardStatus.PendingPickup => "#ff6000",
            CardStatus.Lost => "#d9230b",
            CardStatus.Frozen => "#d9230b",
            _ => "#70706d"
        };
    }
}
```

### 7.2 使用示例

```csharp
// 在父页面中嵌入 - 标准模式
public void LoadStaffInfoModule(string staffId, DisplayMode mode)
{
    var parameter = new StaffInfoPageParameter
    {
        StaffId = staffId,
        Mode = mode
    };
    
    StaffInfoContent = new StaffInfoPage 
    { 
        DataContext = new StaffInfoViewModel(parameter) 
    };
}

// 充值预览模式
public void LoadStaffInfoForRecharge(StaffInfo info, decimal rechargeAmount)
{
    var parameter = new StaffInfoPageParameter
    {
        Data = info,
        Mode = DisplayMode.RechargePreview,
        RechargeAmount = rechargeAmount
    };
    
    StaffInfoContent = new StaffInfoPage 
    { 
        DataContext = new StaffInfoViewModel(parameter) 
    };
}

// 完整信息模式
public void LoadFullStaffInfo(string staffId)
{
    var parameter = new StaffInfoPageParameter
    {
        StaffId = staffId,
        Mode = DisplayMode.FullInfo
    };
    
    StaffInfoContent = new StaffInfoPage 
    { 
        DataContext = new StaffInfoViewModel(parameter) 
    };
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [验证页面](./VerifyPage.md)
- [自助取卡页面](./TakeCardPage.md)
- [自助挂失页面](./ReportLossPage.md)
- [自助补卡页面](./ReplacementPage.md)
- [自助查询页面](./QueryPage.md)
- [自助充值页面](./RechargePage.md)
- [学员信息模块](./StudentInfoPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，描述模块四种显示模式和使用方式 | OpenCode Agent |

---

*文档结束*

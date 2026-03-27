# 学员信息模块设计文档 (StudentInfoPage)

> **页面代码**: StudentInfoPage  
> **适用用户**: 学员  
> **页面类型**: 模块页面（嵌入功能子页面）  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

学员信息模块是一个可复用的信息展示模块，嵌入到需要展示学员信息的功能子页面中。模块展示学员的基本信息、培训信息、卡片信息等。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| user_info_page_1.jpg | 自助机/ | 学员信息展示参考 |
| 基本信息切图说明.png | 切图说明/ | 布局切图说明 |

---

## 二、模块布局结构

### 2.1 标准信息卡片布局

```
┌─────────────────────────────────────────┐
│  学员信息                                │
├─────────────────────────────────────────┤
│                                         │
│   姓名：李天明          性别：男        │
│                                         │
│   身份证号：430101199001011234          │
│                                         │
│   班级：测试培训班一                    │
│                                         │
│   培训时间：2020-09-01 至 2020-09-05   │
│                                         │
│   卡号：20200901001                     │
│                                         │
│   卡片状态：正常                        │
│                                         │
│   房间号：301房                         │
│                                         │
│   报到状态：已报到 ✓                    │
│                                         │
└─────────────────────────────────────────┘
```

### 2.2 取卡/挂失/补卡场景布局

```
┌─────────────────────────────────────────┐
│  学员信息                                │
├─────────────────────────────────────────┤
│                                         │
│   ┌──────────┐   姓名：李天明          │
│   │          │   性别：男              │
│   │  照片    │                         │
│   │          │   班级：测试培训班一    │
│   │  120×150 │                        │
│   │          │   卡号：20200901001     │
│   │          │                         │
│   └──────────┘   卡片状态：待领取      │
│                                         │
│   培训时间：2020-09-01 至 2020-09-05   │
│                                         │
└─────────────────────────────────────────┘
```

### 2.3 报到场景布局（含缴费信息）

```
┌─────────────────────────────────────────┐
│  学员信息                                │
├─────────────────────────────────────────┤
│                                         │
│   姓名：李天明          性别：男        │
│                                         │
│   班级：测试培训班一                    │
│                                         │
│   培训时间：2020-09-01 至 2020-09-05   │
│                                         │
│   卡号：20200901001                     │
│                                         │
│   卡片状态：未制卡                      │
│                                         │
│   报到状态：未报到                      │
│                                         │
│   ┌─────────────────────────────────┐   │
│   │        费用信息                 │   │
│   │  培训费：¥2000.00              │   │
│   │  住宿费：¥1000.00              │   │
│   │  餐费：¥500.00                 │   │
│   │  合计：¥3500.00                │   │
│   │  缴费状态：未缴费              │   │
│   └─────────────────────────────────┘   │
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
| 标题 | 顶部 | "学员信息" | 黑体 32px bold |
| 分隔线 | 标题下方 | 1px solid #e0e0e0 | 分隔标题和内容 |

### 3.3 信息字段布局

**标准布局（双列）**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 标签 | 左侧 | 黑体 28px, #70706d | 如"姓名：" |
| 内容 | 标签右侧 | 黑体 28px, #1a1919 | 如"李天明" |
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
| 已报到 ✓ | #4CAF50 | 报到状态 |
| 未报到 | #70706d | 未报到状态 |
| 未缴费 | #d9230b | 缴费状态 |
| 已缴费 ✓ | #4CAF50 | 缴费状态 |

### 3.6 费用信息区（可选）

| 属性 | 值 |
|------|-----|
| 背景 | #faf9f3 |
| 边框 | 1px solid #e6b84d |
| 圆角 | 5px |
| 内边距 | 20px |
| 标题 | 黑体 30px bold, #d9230b |
| 费用项 | 黑体 28px |
| 合计 | 黑体 32px bold, #d9230b |

---

## 四、功能描述

### 4.1 模块用途

StudentInfoPage 是一个可复用的信息展示模块，用于在功能子页面中展示学员的详细信息。模块根据使用场景可以显示不同组合的信息字段。

### 4.2 显示模式

**模式 1: 标准信息模式**
- 显示基本身份信息
- 显示培训信息
- 显示卡片信息
- 用于：验证后的信息确认

**模式 2: 照片信息模式**
- 显示学员照片
- 显示基本信息
- 显示卡片状态
- 用于：取卡、挂失、补卡

**模式 3: 完整信息模式**
- 显示所有信息
- 显示费用信息
- 显示报到/缴费状态
- 用于：报到、查询

### 4.3 数据字段

| 字段名 | 说明 | 显示条件 |
|--------|------|----------|
| 姓名 | 学员姓名 | 始终显示 |
| 性别 | 男/女 | 始终显示 |
| 身份证号 | 完整身份证号 | 始终显示 |
| 班级 | 培训班名称 | 始终显示 |
| 培训时间 | 培训起止日期 | 始终显示 |
| 卡号 | 学员卡号 | 有卡时显示 |
| 卡片状态 | 正常/待领取/已挂失等 | 有卡时显示 |
| 房间号 | 分配的房间 | 报到后显示 |
| 报到状态 | 已报到/未报到 | 始终显示 |
| 照片 | 学员照片 | 照片模式 |
| 费用信息 | 各项费用明细 | 费用模式 |

---

## 五、交互描述

### 5.1 模块加载

```
接收参数
    ↓
根据模式确定显示字段
    ↓
加载学员数据
    ↓
渲染信息卡片
    ↓
显示在父页面中
```

### 5.2 详细交互

**1. 参数接收**:
- 从父页面接收显示模式参数
- 接收学员ID或学员信息对象
- 接收可选的额外信息（费用、状态等）

**2. 数据加载**:
- 根据学员ID查询详细信息
- 或直接使用传入的学员信息
- 格式化数据显示

**3. 动态字段**:
- 根据显示模式隐藏/显示字段
- 根据数据状态显示不同颜色标签
- 照片区域根据模式决定是否显示

**4. 刷新机制**:
- 支持外部触发刷新
- 刷新后更新显示数据
- 保持当前显示模式

### 5.3 参数定义

```csharp
public class StudentInfoPageParameter
{
    public string StudentId { get; set; }
    public StudentInfo Data { get; set; }  // 可选，直接传入数据
    public DisplayMode Mode { get; set; }
    public FeeInfo FeeInfo { get; set; }   // 可选，费用信息
}

public enum DisplayMode
{
    Standard,   // 标准信息模式
    WithPhoto,  // 照片信息模式
    FullInfo    // 完整信息模式（含费用）
}

public class StudentInfo
{
    public string Id { get; set; }
    public string Name { get; set; }
    public string Gender { get; set; }
    public string IdCardNumber { get; set; }
    public string ClassName { get; set; }
    public DateTime TrainingStartDate { get; set; }
    public DateTime TrainingEndDate { get; set; }
    public string CardNumber { get; set; }
    public CardStatus CardStatus { get; set; }
    public string RoomNumber { get; set; }
    public CheckInStatus CheckInStatus { get; set; }
    public string PhotoUrl { get; set; }
}

public class FeeInfo
{
    public decimal TrainingFee { get; set; }
    public decimal AccommodationFee { get; set; }
    public decimal MealFee { get; set; }
    public decimal TotalFee { get; set; }
    public PaymentStatus PaymentStatus { get; set; }
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
| 费用区边框 | #e6b84d |
| 费用区背景 | #faf9f3 |
| 金额数字 | #d9230b |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 模块标题 | 黑体 | 32px | bold |
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 28px | normal |
| 费用标题 | 黑体 | 30px | bold |
| 费用金额 | 黑体 | 32px | bold |
| 状态标签 | 黑体 | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 30px |
| 标题下边距 | 20px |
| 信息行间距 | 25px |
| 标签与内容间距 | 10px |
| 照片与信息间距 | 30px |
| 费用区内边距 | 20px |
| 费用区上边距 | 25px |

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
public partial class StudentInfoViewModel : ViewModelBase
{
    [ObservableProperty]
    private StudentInfo _studentInfo;
    
    [ObservableProperty]
    private DisplayMode _currentMode;
    
    [ObservableProperty]
    private FeeInfo _feeInfo;
    
    [ObservableProperty]
    private bool _showPhoto;
    
    [ObservableProperty]
    private bool _showFeeInfo;
    
    partial void OnCurrentModeChanged(DisplayMode value)
    {
        ShowPhoto = value == DisplayMode.WithPhoto;
        ShowFeeInfo = value == DisplayMode.FullInfo;
    }
    
    public async Task LoadDataAsync(StudentInfoPageParameter parameter)
    {
        CurrentMode = parameter.Mode;
        
        if (parameter.Data != null)
        {
            StudentInfo = parameter.Data;
        }
        else if (!string.IsNullOrEmpty(parameter.StudentId))
        {
            StudentInfo = await StudentService.GetStudentInfoAsync(parameter.StudentId);
        }
        
        if (parameter.FeeInfo != null)
        {
            FeeInfo = parameter.FeeInfo;
        }
    }
    
    // 状态颜色转换
    public string GetStatusColor(CardStatus status)
    {
        return status switch
        {
            CardStatus.Normal => "#4CAF50",
            CardStatus.PendingPickup => "#ff6000",
            CardStatus.Lost => "#d9230b",
            _ => "#70706d"
        };
    }
}
```

### 7.2 使用示例

```csharp
// 在父页面中嵌入
public void LoadStudentInfoModule(string studentId, DisplayMode mode)
{
    var parameter = new StudentInfoPageParameter
    {
        StudentId = studentId,
        Mode = mode
    };
    
    StudentInfoContent = new StudentInfoPage 
    { 
        DataContext = new StudentInfoViewModel(parameter) 
    };
}

// 完整信息模式（含费用）
public void LoadFullStudentInfo(StudentInfo info, FeeInfo fee)
{
    var parameter = new StudentInfoPageParameter
    {
        Data = info,
        Mode = DisplayMode.FullInfo,
        FeeInfo = fee
    };
    
    StudentInfoContent = new StudentInfoPage 
    { 
        DataContext = new StudentInfoViewModel(parameter) 
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
- [报到页面](./CheckInPage.md)
- [教职工信息模块](./StaffInfoPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，描述模块三种显示模式和使用方式 | OpenCode Agent |

---

*文档结束*

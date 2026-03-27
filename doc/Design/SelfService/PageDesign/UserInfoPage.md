# 学员信息界面设计文档 (UserInfoPage)

> **页面代码**: UserInfoPage  
> **适用用户**: 学员  
> **验证要求**: 已通过身份验证  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计图源文件

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| user_info_page_1.jpg | `/doc/Design/自助机/user_info_page_1.jpg` | 默认状态 (复选框未选中) |
| user_info_page_2.jpg | `/doc/Design/自助机/user_info_page_2.jpg` | 选中状态 (培训费已选) |

### 1.2 切图资源

**位置**: `/doc/Design/自助机裁图/user_info_page/`

| 文件名 | 用途 |
|--------|------|
| user_info_layout.png | 用户信息页布局参考 |
| user_icon.png | 人像图标 (报到按钮) |
| payment_icon.png | ¥符号图标 (缴费按钮) |
| back_icon.png | 返回箭头图标 |
| checkbox_normal.png | 复选框未选中态 |
| checkboxpress.png | 复选框选中态 (红色背景) |

### 1.3 设计说明

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| 基本信息切图说明.png | `/doc/Design/切图说明/基本信息切图说明.png` | 布局和标注说明 |

---

## 二、页面布局结构

### 2.1 状态 1 - 默认显示

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 120]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │ ▎基本信息                       │ │
│ │   姓名 [李天明        ]         │ │
│ │   入住房间 [A312      ]         │ │
│ │   入住时间 [2020/08/31] 至 [...]│ │
│ │   班级名称 [测试培训班一        ]│ │
│ │   培训时间 [2020/09/01] 至 [...]│ │
│ ├─────────────────────────────────┤ │
│ │ ▎缴费信息                       │ │
│ │   ┌─────────────────────────┐   │ │
│ │   │序号│□│缴费项目│金额│状态│   │ │
│ │   │ 1  │□│培训费  │2300│未缴费│  │ │
│ │   │ 2  │□│住宿费  │4800│未缴费│  │ │
│ │   └─────────────────────────┘   │ │
│ └─────────────────────────────────┘ │
│ [← 返回]          [¥ 缴费] [👤 报到]│
└─────────────────────────────────────┘
```

### 2.2 状态 2 - 培训费已选中

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 118]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │ ▎基本信息                       │ │
│ │   姓名 [李天明        ]         │ │
│ │   入住房间 [A312      ]         │ │
│ │   入住时间 [2020/08/31] 至 [...]│ │
│ │   班级名称 [测试培训班一        ]│ │
│ │   培训时间 [2020/09/01] 至 [...]│ │
│ ├─────────────────────────────────┤ │
│ │ ▎缴费信息                       │ │
│ │   ┌─────────────────────────┐   │ │
│ │   │序号│  │缴费项目│金额│状态│   │ │
│ │   │ 1  │✓│培训费  │2300│未缴费│  │ │
│ │   │ 2  │□│住宿费  │4800│未缴费│  │ │
│ │   └─────────────────────────┘   │ │
│ └─────────────────────────────────┘ │
│ [← 返回]          [¥ 缴费] [👤 报到]│
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

### 3.2 内容区卡片

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 卡片背景 | 中央 | #faf9f3, 7px 圆角 | 内容容器 |
| 卡片边距 | 距边缘 | 上下左右红色边距 | 内容区边距 |

### 3.3 基本信息区块

#### 3.3.1 区块标题

| 属性 | 值 |
|------|-----|
| 标题文字 | "基本信息" |
| 字体 | 黑体 36px |
| 颜色 | #1a1919 |
| 左侧装饰 | 红色竖线 8×32px |
| 竖线颜色 | #d9230b |
| 竖线距标题 | 12px |
| 标题距上 | 45px |
| 标题距左 | 66px |

#### 3.3.2 表单字段

| 字段 | 标签 | 内容示例 | 输入框尺寸 | 布局 |
|------|------|----------|-----------|------|
| 姓名 | 姓名 | 李天明 | - | 第一行左侧 |
| 入住房间 | 入住房间 | A312 | 53×238px | 第一行右侧 |
| 入住时间 | 入住时间 | 2020/08/31 至 2020/09/10 | - | 第二行 |
| 班级名称 | 班级名称 | 测试培训班一 | 53×806px | 第三行 |
| 培训时间 | 培训时间 | 2020/09/01 至 2020/09/10 | 53×375px | 第四行 |

#### 3.3.3 输入框样式

| 属性 | 值 |
|------|-----|
| 背景 | #ffffff |
| 边框 | #c7c6bf |
| 圆角 | 7px |
| 标签字体 | 28px |
| 内容字体 | 28px, #1a1919 |
| 标签与输入框间距 | 8px |
| 行间距 | 20px |

### 3.4 缴费信息区块

#### 3.4.1 区块标题

| 属性 | 值 |
|------|-----|
| 标题文字 | "缴费信息" |
| 字体 | 黑体 36px |
| 颜色 | #1a1919 |
| 左侧装饰 | 红色竖线 |
| 距上区块 | 50px |

#### 3.4.2 缴费信息表格

| 列名 | 内容示例 | 宽度 |
|------|----------|------|
| 序号 | 1, 2 | 58px |
| 选择框 | 复选框 (□/✓) | - |
| 缴费项目 | 培训费, 住宿费 | - |
| 缴费金额 | 2300, 4800 | 202px |
| 缴费状态 | 未缴费 | - |
| 缴费日期 | 空白 | - |

#### 3.4.3 表格样式

| 属性 | 值 |
|------|-----|
| 表头背景 | #faf9f3 |
| 内容行背景 | #ffffff |
| 分隔线颜色 | #e3e2df, #c7c6bf |
| 表头文字 | 26px, #70706d |
| 内容文字 | 28px, #1a1919 |
| 行高 | 58px |
| 列间距 | 55px |
| 选中行分隔线 | #e3e2df |
| 表格圆角 | 7px |

#### 3.4.4 复选框样式

**未选中态**:
- 方形，白色背景
- 浅灰色边框
- 轻微圆角

**选中态**:
- 红色/橙色背景 (#FF4040)
- 白色勾选标记
- 圆角方形

### 3.5 底部按钮区

#### 3.5.1 返回按钮 (左侧)

| 属性 | 值 |
|------|-----|
| 位置 | 左下角 |
| 图标 | 返回箭头 (back_icon.png) |
| 文字 | "返回" |
| 背景 | 金黄色渐变 |
| 尺寸 | 标准按钮尺寸 |

#### 3.5.2 缴费按钮 (右下角左侧)

| 属性 | 值 |
|------|-----|
| 位置 | 右下角左侧 |
| 图标 | ¥符号 (payment_icon.png) |
| 文字 | "缴费" |
| 背景 | 白色 |
| 启用条件 | 至少选中一个未缴费项目 |

#### 3.5.3 报到按钮 (右下角右侧)

| 属性 | 值 |
|------|-----|
| 位置 | 右下角右侧 |
| 图标 | 人像图标 (user_icon.png) |
| 文字 | "报到" |
| 背景 | 白色 |
| 启用条件 | 学员身份验证通过 |

---

## 四、功能描述

### 4.1 页面用途

学员信息界面展示学员的基本信息和缴费信息，是学员进行自助报到和缴费的核心界面。用户可以查看个人信息、选择缴费项目，并执行报到或缴费操作。

### 4.2 核心功能

1. **基本信息展示**
   - 显示学员姓名、入住房间
   - 显示入住时间、班级名称
   - 显示培训时间
   - 信息只读，不可编辑

2. **缴费信息展示**
   - 显示待缴费项目列表
   - 显示缴费金额和状态
   - 支持多选缴费项目
   - 显示缴费日期（已缴费项目）

3. **缴费操作**
   - 选择待缴费项目
   - 点击缴费按钮
   - 跳转到支付界面

4. **报到操作**
   - 点击报到按钮
   - 完成报到流程
   - 显示报到结果

5. **倒计时管理**
   - 左上角显示倒计时
   - 超时自动返回主界面

### 4.3 数据字段说明

#### 4.3.1 基本信息

| 字段名 | 字段代码 | 数据类型 | 示例值 |
|--------|----------|----------|--------|
| 姓名 | Name | string | 李天明 |
| 入住房间 | RoomNumber | string | A312 |
| 入住时间 | CheckInDate | DateTime | 2020/08/31 |
| 退房时间 | CheckOutDate | DateTime | 2020/09/10 |
| 班级名称 | ClassName | string | 测试培训班一 |
| 培训开始时间 | TrainingStartDate | DateTime | 2020/09/01 |
| 培训结束时间 | TrainingEndDate | DateTime | 2020/09/10 |

#### 4.3.2 缴费信息

| 字段名 | 字段代码 | 数据类型 | 示例值 |
|--------|----------|----------|--------|
| 序号 | Index | int | 1, 2 |
| 缴费项目 | ItemName | string | 培训费、住宿费 |
| 缴费金额 | Amount | decimal | 2300, 4800 |
| 缴费状态 | Status | enum | 未缴费/已缴费 |
| 缴费日期 | PaymentDate | DateTime? | null |
| 是否选中 | IsSelected | bool | true/false |

### 4.4 交互流程

```
查看信息 → 选择缴费项目 → 点击缴费 → 支付流程 → 支付成功 → 更新状态
    ↓            ↓             ↓
直接报到    不选项目    支付失败
    ↓            ↓             ↓
报到流程    缴费按钮     提示重试
           禁用态
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 默认显示
    ↓ 点击复选框
状态 2: 项目选中
    ↓ 点击缴费按钮
状态 3: 支付流程
    ↓ 支付完成
状态 4: 支付结果
    ├─ 支付成功 → 更新缴费状态 → 状态 1
    └─ 支付失败 → 提示错误 → 状态 2
```

### 5.2 详细交互

**1. 页面加载**:
- 从后台获取学员信息
- 显示基本信息和缴费信息
- 启动倒计时计时器
- 所有复选框初始未选中

**2. 查看信息**:
- 滚动查看完整信息（如需要）
- 信息为只读状态
- 不可编辑任何字段

**3. 选择缴费项目**:
- 点击复选框切换选中状态
- 未缴费项目可选中/取消
- 已缴费项目不可选（禁用态）
- 可多选多个项目

**4. 缴费按钮状态**:
- **未选中项目**: 缴费按钮禁用，灰色
- **已选中项目**: 缴费按钮启用，可点击
- **点击缴费**: 
  - 校验选中项目和金额
  - 跳转到支付界面
  - 携带缴费项目信息

**5. 报到操作**:
- 点击报到按钮
- 判断是否需要先缴费
- 跳转到报到流程

**6. 支付返回**:
- 支付成功：
  - 刷新缴费信息
  - 更新缴费状态为"已缴费"
  - 显示缴费日期
  - 清空选中状态
- 支付失败：
  - 返回当前页面
  - 保留选中状态
  - 显示错误提示

**7. 倒计时超时**:
- 倒计时归零
- 自动返回主界面
- 清除会话数据

**8. 返回操作**:
- 点击返回按钮
- 返回主界面
- 重置倒计时

### 5.3 复选框交互

| 操作 | 效果 |
|------|------|
| 点击未选中复选框 | 变为选中态，缴费按钮启用 |
| 点击已选中复选框 | 变为未选中态，如全未选则缴费按钮禁用 |
| 点击已缴费项目复选框 | 无反应，保持禁用态 |

### 5.4 错误处理

| 错误类型 | 处理方式 |
|----------|----------|
| 获取信息失败 | 提示"获取信息失败，请重试" |
| 网络错误 | 提示"网络连接失败" |
| 支付超时 | 提示"支付超时，请重新选择" |

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 区块标题 | #1a1919 |
| 标题左侧竖线 | #d9230b |
| 标签文字 | #1a1919 |
| 输入框背景 | #ffffff |
| 输入框边框 | #c7c6bf |
| 表头文字 | #70706d |
| 表格内容 | #1a1919 |
| 表头背景 | #faf9f3 |
| 内容行背景 | #ffffff |
| 分隔线 | #e3e2df, #c7c6bf |
| 复选框选中背景 | #FF4040 |
| 复选框勾选标记 | #ffffff |
| 返回按钮背景 | 金黄色渐变 |
| 缴费/报到按钮背景 | 白色 |
| 倒计时 | #ff6000 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 区块标题 | 黑体 | 36px | bold |
| 标签文字 | 黑体 | 28px | normal |
| 表头文字 | 黑体 | 26px | normal |
| 表格内容 | 黑体 | 28px | normal |
| 按钮文字 | 黑体 | 28-36px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 区块标题距上 | 45px |
| 区块标题距左 | 66px |
| 标题左侧竖线距标题 | 12px |
| 竖线尺寸 | 8×32px |
| 标签与输入框 | 8px |
| 行间距 | 20px |
| 表格行高 | 58px |
| 表格列间距 | 55px |
| 金额列宽度 | 202px |
| 区块间间距 | 50px |

---

## 七、实现注意事项

### 7.1 数据模型

```csharp
// 学员信息
public class StudentInfo
{
    public string Name { get; set; }
    public string RoomNumber { get; set; }
    public DateTime CheckInDate { get; set; }
    public DateTime CheckOutDate { get; set; }
    public string ClassName { get; set; }
    public DateTime TrainingStartDate { get; set; }
    public DateTime TrainingEndDate { get; set; }
    public List<PaymentItem> PaymentItems { get; set; }
}

// 缴费项目
public class PaymentItem
{
    public int Index { get; set; }
    public string ItemName { get; set; }
    public decimal Amount { get; set; }
    public PaymentStatus Status { get; set; }
    public DateTime? PaymentDate { get; set; }
    public bool IsSelected { get; set; }
}

public enum PaymentStatus
{
    Unpaid,   // 未缴费
    Paid      // 已缴费
}
```

### 7.2 ViewModel 实现

```csharp
public partial class UserInfoViewModel : ViewModelBase
{
    [ObservableProperty]
    private StudentInfo _studentInfo;
    
    [ObservableProperty]
    private ObservableCollection<PaymentItem> _paymentItems;
    
    [ObservableProperty]
    private bool _isPaymentButtonEnabled;
    
    [ObservableProperty]
    private int _countdownSeconds;
    
    // 计算属性：选中的缴费项目
    public IEnumerable<PaymentItem> SelectedItems => 
        PaymentItems?.Where(p => p.IsSelected && p.Status == PaymentStatus.Unpaid);
    
    // 计算属性：选中金额总计
    public decimal SelectedAmount => 
        SelectedItems?.Sum(p => p.Amount) ?? 0;
    
    partial void OnPaymentItemsChanged(ObservableCollection<PaymentItem> value)
    {
        UpdatePaymentButtonState();
    }
    
    private void UpdatePaymentButtonState()
    {
        IsPaymentButtonEnabled = PaymentItems?.Any(p => p.IsSelected && p.Status == PaymentStatus.Unpaid) ?? false;
    }
}
```

### 7.3 复选框命令

```csharp
[RelayCommand]
private void TogglePaymentItemSelection(PaymentItem item)
{
    if (item == null || item.Status == PaymentStatus.Paid)
        return;
    
    item.IsSelected = !item.IsSelected;
    UpdatePaymentButtonState();
}
```

### 7.4 缴费按钮命令

```csharp
[RelayCommand]
private async Task PaymentAsync()
{
    var selectedItems = SelectedItems.ToList();
    if (selectedItems.Count == 0)
        return;
    
    // 携带选中项目信息跳转到支付界面
    var navigationParameter = new PaymentParameter
    {
        Items = selectedItems,
        TotalAmount = SelectedAmount
    };
    
    await NavigationService.NavigateToAsync("PaymentPage", navigationParameter);
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [身份证验证界面](./IDCardVerifyPage.md)
- [短信验证界面](./SMSVerifyPage.md)
- [特别提示页](./SpecialTipPage.md)

---

*文档结束*

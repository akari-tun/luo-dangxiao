# 教职工信息模块设计文档 (StaffInfoPage)

> **页面代码**: StaffInfoPage  
> **适用用户**: 教职工  
> **页面类型**: 模块页面（嵌入功能子页面）  
> **文档版本**: 1.1  
> **最后更新**: 2026-04-10

---

## 一、设计图引用

### 1.1 设计说明

教职工信息模块是一个可复用的信息展示模块，嵌入到需要展示教职工信息的功能子页面中。模块采用固定四行表单结构，展示姓名、人员编号、卡片类型、卡有效期、所在部门、消费余额与补助余额。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| staff_info_layout.png | 自助机裁图/user_info_page/ | 教职工信息模块布局参考 |

---

## 二、模块布局结构

### 2.1 参考图实际布局

参考图展示的是一个固定的四行表单式信息区，不是多场景切换的信息卡片。整体采用“两列字段 + 单行通栏 + 两列金额”的组合布局。

```
┌──────────────────────────────────────────────────────────────┐
│ 姓名        [________________]   人员编号   [______________] │
│                                                              │
│ 卡片类型    [________________]   卡有效期   [______________] │
│                                                              │
│ 所在部门    [______________________________________________] │
│                                                              │
│ 消费余额    [________________]   补助余额   [______________] │
└──────────────────────────────────────────────────────────────┘
```

### 2.2 行级布局说明

| 行号 | 左侧内容 | 右侧内容 | 布局特征 |
|------|----------|----------|----------|
| 第 1 行 | 姓名 | 人员编号 | 左右双列，均为“标签 + 白色输入框” |
| 第 2 行 | 卡片类型 | 卡有效期 | 左右双列，结构与第 1 行一致 |
| 第 3 行 | 所在部门 | - | 单行通栏，部门值横向占满主要内容区 |
| 第 4 行 | 消费余额 | 补助余额 | 左右双列，两个金额字段并排展示 |

### 2.3 模块结构要点

- 所有字段均按行对齐，标签位于输入框左侧。
- 第 1、2、4 行为左右双列布局，左列与右列宽度保持一致。
- 第 3 行“所在部门”为唯一通栏字段，占据整行内容宽度。
- 页面中未出现头像区、卡片信息子卡片、充值预览区或查询场景扩展块。

---

## 三、UI 元素详解

### 3.1 模块容器

| 属性 | 值 |
|------|-----|
| 背景 | #faf9f3 米黄色 |
| 边框 | 无明显描边，以内容块分区为主 |
| 圆角 | 模块整体弱圆角 |
| 内边距 | 20px ~ 30px |
| 外边距 | 根据父页面调整 |

### 3.2 标题区

参考图裁切范围内未展示独立标题栏，模块通常以内嵌信息区形式出现在父页面中，因此布局重点在字段行本身，而不是标题装饰。

### 3.3 信息字段布局

**双列字段行**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 标签 | 每列左侧 | 黑体 28px, #70706d | 如“姓名”“人员编号” |
| 值区 | 标签右侧 | 黑体 28px, #1a1919 | 白色矩形信息框 |
| 行间距 | - | 20px~25px | 字段行垂直间距 |

**通栏字段行**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 所在部门 | 第 3 行 | 黑体 28px | 标签位于左侧，值区横向占满主要内容区 |

### 3.4 字段值区

| 属性 | 值 |
|------|-----|
| 背景 | #ffffff 白色 |
| 圆角 | 6px |
| 内边距 | 12px 20px |
| 内容对齐 | 垂直居中，文本左对齐 |
| 适用字段 | 姓名、人员编号、卡片类型、卡有效期、消费余额、补助余额、所在部门 |

### 3.5 金额字段

| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 消费余额 | 第 4 行左侧 | 黑体 28px, #1a1919 | 显示消费账户余额，保留两位小数 |
| 补助余额 | 第 4 行右侧 | 黑体 28px, #1a1919 | 显示补助账户余额，保留两位小数 |

### 3.6 对齐规则

| 规则 | 说明 |
|------|------|
| 左右双列对齐 | 第 1、2、4 行左右两列保持统一宽度与起始位置 |
| 通栏优先级 | 第 3 行“所在部门”单独占据整行 |
| 字段顺序 | 页面按“基础身份 → 卡务信息 → 所在部门 → 余额信息”自上而下排列 |

---

## 四、功能描述

### 4.1 模块用途

StaffInfoPage 是一个可复用的信息展示模块，用于在功能子页面中展示教职工的固定信息摘要。模块按照参考图中的四行表单结构呈现身份信息、卡务信息、部门信息与余额信息。

### 4.2 固定显示内容

- 第 1 行：姓名（左）+ 人员编号（右）
- 第 2 行：卡片类型（左）+ 卡有效期（右）
- 第 3 行：所在部门（通栏）
- 第 4 行：消费余额（左）+ 补助余额（右）

### 4.3 数据字段

| 字段名 | 说明 | 页面位置 |
|--------|------|----------|
| 姓名 | 教职工姓名 | 第 1 行左侧 |
| 人员编号 | 教职工唯一编号 | 第 1 行右侧 |
| 卡片类型 | 卡片或人员类别 | 第 2 行左侧 |
| 卡有效期 | 卡片有效截止时间 | 第 2 行右侧 |
| 所在部门 | 所属部门名称 | 第 3 行通栏 |
| 消费余额 | 消费账户余额 | 第 4 行左侧 |
| 补助余额 | 补助账户余额 | 第 4 行右侧 |

---

## 五、交互描述

### 5.1 模块加载

```
接收参数
    ↓
加载教职工数据
    ↓
字段映射与格式化
    ↓
渲染四行表单
    ↓
显示在父页面中
```

### 5.2 详细交互

**1. 参数接收**:
- 接收教职工ID或教职工信息对象

**2. 数据加载**:
- 根据教职工ID查询详细信息
- 或直接使用传入的教职工信息
- 提取页面所需的七个字段

**3. 字段映射**:
- 姓名映射到第 1 行左侧
- 人员编号映射到第 1 行右侧
- 卡片类型与卡有效期映射到第 2 行
- 所在部门映射到第 3 行通栏
- 消费余额与补助余额映射到第 4 行左右两侧

**4. 格式化显示**:
- 卡有效期按统一日期格式显示
- 消费余额与补助余额保留两位小数
- 空值字段显示为空字符串或默认值

**5. 刷新机制**:
- 支持外部触发刷新
- 刷新后更新显示数据
- 重新按固定布局渲染字段

### 5.3 参数定义

```csharp
public class StaffInfoPageParameter
{
    public string StaffId { get; set; }
    public StaffInfo Data { get; set; }  // 可选，直接传入数据
}

public class StaffInfo
{
    public string Id { get; set; }
    public string Name { get; set; }
    public string EmployeeNumber { get; set; }
    public string CardType { get; set; }
    public DateTime? CardExpiryDate { get; set; }
    public string Department { get; set; }
    public decimal ConsumptionBalance { get; set; }
    public decimal SubsidyBalance { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 模块背景 | #faf9f3 |
| 信息标签 | #70706d |
| 信息内容 | #1a1919 |
| 字段值区背景 | #ffffff |
| 辅助说明 | #70706d |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 28px | normal |
| 余额数字 | 黑体 | 28px | normal |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 30px |
| 信息行间距 | 20px~25px |
| 标签与内容间距 | 10px |
| 左右列间距 | 40px |
| 通栏字段上下间距 | 25px |

### 6.4 尺寸规范

| 元素 | 尺寸 |
|------|------|
| 模块最小宽度 | 500px |
| 左侧字段区 | 约48% |
| 右侧字段区 | 约48% |
| 通栏字段区 | 100% |

---

## 七、实现注意事项

### 7.1 ViewModel 实现

```csharp
public partial class StaffInfoViewModel : ViewModelBase
{
    [ObservableProperty]
    private StaffInfo _staffInfo;

    public async Task LoadDataAsync(StaffInfoPageParameter parameter)
    {
        if (parameter.Data != null)
        {
            StaffInfo = parameter.Data;
        }
        else if (!string.IsNullOrEmpty(parameter.StaffId))
        {
            StaffInfo = await StaffService.GetStaffInfoAsync(parameter.StaffId);
        }
    }

    public string CardExpiryText =>
        StaffInfo?.CardExpiryDate?.ToString("yyyy-MM-dd") ?? string.Empty;

    public string ConsumptionBalanceText =>
        StaffInfo == null ? "0.00" : StaffInfo.ConsumptionBalance.ToString("F2");

    public string SubsidyBalanceText =>
        StaffInfo == null ? "0.00" : StaffInfo.SubsidyBalance.ToString("F2");
}
```

### 7.2 使用示例

```csharp
// 在父页面中嵌入 - 根据 staffId 加载
public void LoadStaffInfoModule(string staffId)
{
    var parameter = new StaffInfoPageParameter
    {
        StaffId = staffId
    };

    StaffInfoContent = new StaffInfoPage
    {
        DataContext = new StaffInfoViewModel(parameter)
    };
}

// 直接传入数据对象
public void LoadStaffInfoModule(StaffInfo info)
{
    var parameter = new StaffInfoPageParameter
    {
        Data = info
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
| 1.1 | 2026-04-10 | 按 staff_info_layout.png 修正固定四行布局及相关字段说明 | OpenCode Agent |
| 1.0 | 2026-03-26 | 初始版本，描述模块四种显示模式和使用方式 | OpenCode Agent |

---

*文档结束*

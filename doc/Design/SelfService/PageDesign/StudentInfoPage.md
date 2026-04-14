# 学员信息模块设计文档 (StudentInfoPage)

> **页面代码**: StudentInfoPage  
> **适用用户**: 学员  
> **页面类型**: 模块页面（嵌入功能子页面）  
> **文档版本**: 1.1  
> **最后更新**: 2026-04-10

---

## 一、设计图引用

### 1.1 设计说明

学员信息模块是一个可复用的信息展示模块，嵌入到需要展示学员信息的功能子页面中。模块采用固定四行表单结构，展示姓名、入住房间、入住时间区间、班级名称与培训时间区间。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| student_info_layout.png | 自助机裁图/user_info_page/ | 学员信息模块布局参考 |

---

## 二、模块布局结构

### 2.1 参考图实际布局

参考图展示的是一个固定的四行表单式信息区，核心结构是“基本信息 + 房间信息 + 两组区间字段 + 单行通栏班级信息”。

```
┌──────────────────────────────────────────────────────────────┐
│ 姓名        [________________]   入住房间   [______________] │
│                                                              │
│ 入住时间    [______________]  至  [_______________________]  │
│                                                              │
│ 班级名称    [______________________________________________] │
│                                                              │
│ 培训时间    [______________]  至  [_______________________]  │
└──────────────────────────────────────────────────────────────┘
```

### 2.2 行级布局说明

| 行号 | 内容 | 布局特征 |
|------|------|----------|
| 第 1 行 | 姓名、入住房间 | 左右双列，两个字段并排展示 |
| 第 2 行 | 入住时间起止 | 区间布局，开始值与结束值之间使用“至”分隔 |
| 第 3 行 | 班级名称 | 单行通栏，班级名称横向占满主要内容区 |
| 第 4 行 | 培训时间起止 | 区间布局，结构与入住时间行一致 |

### 2.3 模块结构要点

- 第 1 行采用左右双列字段布局，标签位于输入框左侧。
- 第 2 行与第 4 行均为日期/时间区间结构，由“起始值 + 至 + 结束值”组成。
- 第 3 行“班级名称”为唯一通栏字段，占据整行内容宽度。
- 页面中未出现照片区、费用信息卡片、卡片状态区或报到状态扩展块。

---

## 三、UI 元素详解

### 3.1 模块容器

| 属性 | 值 |
|------|-----|
| 背景 | #efe8bf 浅米色 |
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
| 标签 | 每列左侧 | 黑体 28px, #70706d | 如“姓名”“入住房间” |
| 值区 | 标签右侧 | 黑体 28px, #1a1919 | 白色矩形信息框 |
| 行间距 | - | 20px~25px | 字段行垂直间距 |

**区间字段行**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 入住时间 | 第 2 行 | 黑体 28px | 起始值与结束值之间以“至”分隔 |
| 培训时间 | 第 4 行 | 黑体 28px | 结构与入住时间一致 |

**通栏字段行**:
| 字段 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 班级名称 | 第 3 行 | 黑体 28px | 标签位于左侧，值区横向占满主要内容区 |

### 3.4 字段值区

| 属性 | 值 |
|------|-----|
| 背景 | #ffffff 白色 |
| 圆角 | 6px |
| 内边距 | 12px 20px |
| 内容对齐 | 垂直居中，文本左对齐 |
| 适用字段 | 姓名、入住房间、入住时间、班级名称、培训时间 |

### 3.5 区间字段

| 元素 | 位置 | 字体 | 说明 |
|------|------|------|------|
| 起始值框 | 左侧 | 黑体 28px, #1a1919 | 显示起始日期/时间 |
| “至”分隔符 | 中间 | 黑体 28px, #70706d | 固定置于两个值框之间 |
| 结束值框 | 右侧 | 黑体 28px, #1a1919 | 显示结束日期/时间 |

### 3.6 对齐规则

| 规则 | 说明 |
|------|------|
| 左右双列对齐 | 第 1 行左右两列保持统一宽度与起始位置 |
| 区间字段结构 | 第 2、4 行均采用“起始值 + 至 + 结束值”布局 |
| 通栏优先级 | 第 3 行“班级名称”单独占据整行 |

---

## 四、功能描述

### 4.1 模块用途

StudentInfoPage 是一个可复用的信息展示模块，用于在功能子页面中展示学员的固定信息摘要。模块按照参考图中的四行表单结构呈现姓名、房间信息、入住时间、班级名称与培训时间。

### 4.2 固定显示内容

- 第 1 行：姓名（左）+ 入住房间（右）
- 第 2 行：入住时间起止区间
- 第 3 行：班级名称（通栏）
- 第 4 行：培训时间起止区间

### 4.3 数据字段

| 字段名 | 说明 | 页面位置 |
|--------|------|----------|
| 姓名 | 学员姓名 | 第 1 行左侧 |
| 入住房间 | 房间名称或编号 | 第 1 行右侧 |
| 入住开始时间 | 入住区间起始时间 | 第 2 行左侧值框 |
| 入住结束时间 | 入住区间结束时间 | 第 2 行右侧值框 |
| 班级名称 | 所属班级或培训班名称 | 第 3 行通栏 |
| 培训开始时间 | 培训区间起始时间 | 第 4 行左侧值框 |
| 培训结束时间 | 培训区间结束时间 | 第 4 行右侧值框 |

---

## 五、交互描述

### 5.1 模块加载

```
接收参数
    ↓
加载学员数据
    ↓
字段映射与格式化
    ↓
渲染四行表单
    ↓
显示在父页面中
```

### 5.2 详细交互

**1. 参数接收**:
- 接收学员ID或学员信息对象

**2. 数据加载**:
- 根据学员ID查询详细信息
- 或直接使用传入的学员信息
- 提取页面所需的七个字段

**3. 字段映射**:
- 姓名与入住房间映射到第 1 行左右两侧
- 入住开始时间与结束时间映射到第 2 行区间字段
- 班级名称映射到第 3 行通栏
- 培训开始时间与结束时间映射到第 4 行区间字段

**4. 格式化显示**:
- 入住时间与培训时间按统一日期或时间格式显示
- 区间字段左右值框保持同一展示规则
- 空值字段显示为空字符串或默认值

**5. 刷新机制**:
- 支持外部触发刷新
- 刷新后更新显示数据
- 重新按固定布局渲染字段

### 5.3 参数定义

```csharp
public class StudentInfoPageParameter
{
    public string StudentId { get; set; }
    public StudentInfo Data { get; set; }  // 可选，直接传入数据
}

public class StudentInfo
{
    public string Id { get; set; }
    public string Name { get; set; }
    public string RoomName { get; set; }
    public DateTime? CheckInStartTime { get; set; }
    public DateTime? CheckInEndTime { get; set; }
    public string ClassName { get; set; }
    public DateTime? TrainingStartDate { get; set; }
    public DateTime? TrainingEndDate { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 模块背景 | #efe8bf |
| 信息标签 | #70706d |
| 信息内容 | #1a1919 |
| 字段值区背景 | #ffffff |
| 区间分隔符 | #70706d |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 28px | normal |
| 区间分隔符 | 黑体 | 28px | normal |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 模块内边距 | 30px |
| 信息行间距 | 20px~25px |
| 标签与内容间距 | 10px |
| 左右列间距 | 40px |
| 区间分隔符左右间距 | 15px |

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
public partial class StudentInfoViewModel : ViewModelBase
{
    [ObservableProperty]
    private StudentInfo _studentInfo;

    public async Task LoadDataAsync(StudentInfoPageParameter parameter)
    {
        if (parameter.Data != null)
        {
            StudentInfo = parameter.Data;
        }
        else if (!string.IsNullOrEmpty(parameter.StudentId))
        {
            StudentInfo = await StudentService.GetStudentInfoAsync(parameter.StudentId);
        }
    }

    public string CheckInStartText =>
        FormatDateTime(StudentInfo?.CheckInStartTime);

    public string CheckInEndText =>
        FormatDateTime(StudentInfo?.CheckInEndTime);

    public string TrainingStartText =>
        FormatDateTime(StudentInfo?.TrainingStartDate);

    public string TrainingEndText =>
        FormatDateTime(StudentInfo?.TrainingEndDate);

    private string FormatDateTime(DateTime? value)
    {
        return value?.ToString("yyyy-MM-dd HH:mm") ?? string.Empty;
    }
}
```

### 7.2 使用示例

```csharp
// 在父页面中嵌入 - 根据 studentId 加载
public void LoadStudentInfoModule(string studentId)
{
    var parameter = new StudentInfoPageParameter
    {
        StudentId = studentId
    };

    StudentInfoContent = new StudentInfoPage
    {
        DataContext = new StudentInfoViewModel(parameter)
    };
}

// 直接传入数据对象
public void LoadStudentInfoModule(StudentInfo info)
{
    var parameter = new StudentInfoPageParameter
    {
        Data = info
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
| 1.1 | 2026-04-10 | 按 student_info_layout.png 修正固定四行布局及相关字段说明 | OpenCode Agent |
| 1.0 | 2026-03-26 | 初始版本，描述模块三种显示模式和使用方式 | OpenCode Agent |

---

*文档结束*

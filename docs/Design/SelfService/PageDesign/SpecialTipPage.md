# 特别提示页设计文档 (SpecialTipPage)

> **页面代码**: SpecialTipPage  
> **适用用户**: 学员、教职工  
> **验证要求**: 已通过身份验证  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计图源文件

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| special_tip_page.jpg | `/doc/Design/自助机/special_tip_page.jpg` | 特别提示页面 |

### 1.2 切图资源

**位置**: `/doc/Design/自助机裁图/special_tip_page/`

| 文件名 | 用途 |
|--------|------|
| spacial_tip_layout.png | 特别提示页布局参考 |
| icon.png | 橙色圆形编号图标 |
| spacial_tip.png | "特别提示"标题标识 |

### 1.3 设计说明

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| 特别提示切图说明.png | `/doc/Design/切图说明/特别提示切图说明.png` | 布局和标注说明 |

---

## 二、页面布局结构

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 120]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │   💡 特别提示                   │ │
│ │   ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─   │ │
│ │                                 │ │
│ │   ① 学员卡在校内就餐、住宿和   │ │
│ │     考勤一卡通用。请妥善保管； │ │
│ │                                 │ │
│ │   [二维码区域 - 动态内容]       │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回]                            │
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
| 卡片背景 | 中央 | 米黄色渐变 #faf9f3, 7px 圆角 | 内容容器 |
| 建筑水印 | 卡片背景 | 淡色校园建筑线稿 | 装饰性背景 |

### 3.3 提示标题区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 信息图标 | 标题左侧 | 橙色圆形"i"图标 | 提示标识 |
| 图标距左 | 66px | - | 距卡片左边缘 |
| 标题文字 | "特别提示" | 橙色，白边效果 | 黑体 |
| 标题距顶 | 50px | - | 距卡片顶部 |
| 分割线 | 标题下方 | 虚线，红色 | 分隔装饰线 |
| 分割线距标题 | 25px | - | 间距 |

### 3.4 提示内容区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 编号图标 | 内容左侧 | 橙色圆形，白色数字"1" | 列表编号 |
| 图标距左 | 43px | - | 距分割线 |
| 内容文字 | 编号右侧 | 黑体 32px, #1a1919 | 提示内容 |
| 行间距 | 20px | - | 多行内容间距 |

**当前提示内容**:
> 学员卡在校内就餐、住宿和考勤一卡通用。请妥善保管；

### 3.5 二维码区域 (如适用)

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 二维码 | 卡片底部 | 动态生成 | 支付/信息二维码 |
| 说明文字 | 二维码旁 | "这个二维码是动态内容" | 提示说明 |
| 背景 | 白色区域 | 与卡片背景区分 | 突出显示 |

### 3.6 底部按钮

#### 3.6.1 返回按钮

| 属性 | 值 |
|------|-----|
| 位置 | 左下角 |
| 文字 | "返回" |
| 图标 | 左箭头 |
| 背景 | 金黄色渐变 #e6b84d → #ffe3a3 |
| 尺寸 | 标准返回按钮尺寸 |

---

## 四、功能描述

### 4.1 页面用途

特别提示页用于向用户展示重要提示信息，通常出现在某些关键操作前或操作后。页面显示一条或多条特别注意事项，帮助用户了解相关规则或注意事项。

### 4.2 使用场景

1. **支付前提示**
   - 展示支付说明
   - 显示支付二维码
   - 提示支付后的操作

2. **取卡前提示**
   - 说明取卡流程
   - 提示保管注意事项

3. **报到后提示**
   - 展示报到成功信息
   - 提示后续事项

### 4.3 核心功能

1. **提示信息展示**
   - 显示标题"特别提示"
   - 展示编号提示内容
   - 支持多条提示（滚动或分页）

2. **二维码展示** (如适用)
   - 动态生成二维码
   - 支持微信支付扫码
   - 显示二维码说明

3. **导航功能**
   - 返回上一页
   - 继续下一步操作
   - 倒计时超时返回

4. **倒计时管理**
   - 左上角显示倒计时
   - 超时自动返回主界面

### 4.4 内容配置

提示内容可配置，支持以下类型：

| 提示类型 | 内容示例 | 二维码 |
|----------|----------|--------|
| 支付提示 | 请使用微信扫码支付 | ✓ |
| 取卡提示 | 请妥善保管学员卡 | ✗ |
| 报到提示 | 报到成功，请查收房卡 | ✗ |
| 挂失提示 | 挂失成功，请补办新卡 | ✗ |

---

## 五、交互描述

### 5.1 交互流程

```
进入页面 → 阅读提示 → 扫码/确认 → 返回/继续
                ↓
            倒计时超时
                ↓
            自动返回主界面
```

### 5.2 详细交互

**1. 页面加载**:
- 从配置或参数获取提示内容
- 生成动态二维码 (如需要)
- 启动倒计时计时器
- 显示提示信息

**2. 阅读提示**:
- 用户阅读提示内容
- 如有多条可滚动查看
- 二维码区域实时显示

**3. 二维码操作** (如适用):
- 用户使用手机扫描二维码
- 二维码可定时刷新
- 显示二维码过期提示

**4. 返回操作**:
- 点击返回按钮
- 返回上一页
- 取消当前操作

**5. 继续操作**:
- 如配置有"完成"按钮
- 点击后进入下一步
- 或返回主界面

**6. 倒计时超时**:
- 倒计时归零
- 自动返回主界面
- 清除会话数据

### 5.3 二维码交互

| 场景 | 行为 |
|------|------|
| 用户扫码 | 二维码保持显示，等待支付结果 |
| 支付成功 | 提示支付成功，自动跳转 |
| 支付失败 | 提示失败原因，可重新扫码 |
| 二维码过期 | 自动刷新二维码，提示重新扫描 |

### 5.4 状态管理

```csharp
public enum TipPageMode
{
    Payment,      // 支付提示模式
    Notification, // 普通通知模式
    Completion    // 完成提示模式
}

public class SpecialTipPageParameter
{
    public TipPageMode Mode { get; set; }
    public string Title { get; set; }
    public List<string> Tips { get; set; }
    public bool ShowQRCode { get; set; }
    public string QRCodeData { get; set; }
    public string NextPage { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 信息图标背景 | 橙色 #F48C3E |
| 标题文字 | 橙色 #FF8C42 |
| 标题白边 | 白色描边 |
| 分割线 | 红色虚线 |
| 编号图标背景 | 橙色 |
| 编号数字 | 白色 |
| 内容文字 | #1a1919 |
| 返回按钮背景 | 金黄色渐变 |
| 卡片背景 | #faf9f3 |
| 倒计时 | #ff6000 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 提示标题 | 黑体 | - | bold |
| 内容文字 | 黑体 | 32px | normal |
| 返回按钮 | 黑体 | 36px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 标题距左 | 66px |
| 标题距顶 | 50px |
| 分割线距标题 | 25px |
| 编号图标距分割线 | 43px |
| 编号图标间距 | 20px |
| 内容与编号间距 | 12px |

---

## 七、实现注意事项

### 7.1 动态二维码

```csharp
public class QRCodeService
{
    // 生成支付二维码
    public async Task<QRCodeData> GeneratePaymentQRCodeAsync(
        decimal amount, 
        string orderId)
    {
        // 调用支付接口生成二维码
        var qrData = await paymentService.CreateOrderAsync(amount, orderId);
        
        // 生成二维码图片
        var qrImage = GenerateQRImage(qrData.CodeUrl);
        
        return new QRCodeData
        {
            Image = qrImage,
            ExpireTime = qrData.ExpireTime,
            OrderId = orderId
        };
    }
    
    // 定时刷新二维码
    public async Task RefreshQRCodeIfNeededAsync()
    {
        if (DateTime.Now > QRCodeData.ExpireTime.AddSeconds(-30))
        {
            // 提前30秒刷新
            var newQRCode = await GeneratePaymentQRCodeAsync(
                Amount, OrderId);
            UpdateQRCodeDisplay(newQRCode);
        }
    }
}
```

### 7.2 内容配置化

```csharp
public class TipContentConfig
{
    public static readonly Dictionary<string, TipContent> PresetTips = 
        new Dictionary<string, TipContent>
    {
        ["Payment"] = new TipContent
        {
            Title = "特别提示",
            Tips = new List<string>
            {
                "请打开微信，扫描下方的二维码进行缴费支付；",
                "支付完成后请从下方出卡口领取您的学员卡。"
            },
            ShowQRCode = true
        },
        ["TakeCard"] = new TipContent
        {
            Title = "特别提示",
            Tips = new List<string>
            {
                "学员卡在校内就餐、住宿和考勤一卡通用。请妥善保管；"
            },
            ShowQRCode = false
        }
    };
}
```

### 7.3 ViewModel 实现

```csharp
public partial class SpecialTipViewModel : ViewModelBase
{
    [ObservableProperty]
    private string _pageTitle;
    
    [ObservableProperty]
    private ObservableCollection<string> _tips;
    
    [ObservableProperty]
    private bool _showQRCode;
    
    [ObservableProperty]
    private Bitmap _qrCodeImage;
    
    [ObservableProperty]
    private int _countdownSeconds;
    
    [ObservableProperty]
    private string _nextPageRoute;
    
    public void Initialize(SpecialTipPageParameter parameter)
    {
        PageTitle = parameter.Title;
        Tips = new ObservableCollection<string>(parameter.Tips);
        ShowQRCode = parameter.ShowQRCode;
        NextPageRoute = parameter.NextPage;
        
        if (ShowQRCode && !string.IsNullOrEmpty(parameter.QRCodeData))
        {
            GenerateQRCode(parameter.QRCodeData);
        }
    }
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [设计图资源](../../自助机/)
- [切图资源](../../自助机裁图/special_tip_page/)

---

*文档结束*

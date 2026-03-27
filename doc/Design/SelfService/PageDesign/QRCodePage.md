# 二维码页面设计文档 (QRCodePage)

> **页面代码**: QRCodePage  
> **适用用户**: 教职工  
> **验证要求**: 已通过身份验证  
> **文档版本**: 1.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计说明

二维码页面用于展示支付二维码，供教职工进行自助充值时使用。用户选择充值金额后，系统生成支付二维码，用户使用手机扫码完成支付。

### 1.2 参考设计

| 参考设计 | 来源 | 说明 |
|----------|------|------|
| user_info_page | 现有设计 | 布局风格参考 |
| RechargePage | 相关页面 | 金额选择页面 |

---

## 二、页面布局结构

### 2.1 主界面布局

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 120]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │         充值支付                │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │    教职工信息卡片      │    │ │
│ │    │    (StaffInfoPage)    │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │      ┌───────┐        │    │ │
│ │    │      │       │        │    │ │
│ │    │      │  QR   │        │    │ │
│ │    │      │ 码图片 │        │    │ │
│ │    │      │       │        │    │ │
│ │    │      └───────┘        │    │ │
│ │    │                       │    │ │
│ │    │   请使用微信/支付宝    │    │ │
│ │    │      扫描二维码        │    │ │
│ │    │                       │    │ │
│ │    │   充值金额：¥100.00   │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │    ⏱ 二维码有效期：05:00       │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回]              [已完成支付]  │
└─────────────────────────────────────┘
```

### 2.2 支付成功状态

```
┌─────────────────────────────────────┐
│ [Logo] 自助服务系统                 │
│ [⏱ 115]  SELF SERVICE SYSTEM      │
├─────────────────────────────────────
│ ┌─────────────────────────────────┐ │
│ │                                 │ │
│ │          ✓ 支付成功            │ │
│ │                                 │ │
│ │    ┌───────────────────────┐    │ │
│ │    │                       │    │ │
│ │    │    教职工信息卡片      │    │ │
│ │    │    (StaffInfoPage)    │    │ │
│ │    │                       │    │ │
│ │    └───────────────────────┘    │ │
│ │                                 │ │
│ │         充值金额：¥100.00      │ │
│ │         支付方式：微信支付     │ │
│ │         交易单号：1234567890   │ │
│ │                                 │ │
│ │    正在写入卡片，请稍候...     │ │
│ │                                 │ │
│ └─────────────────────────────────┘ │
│ [← 返回(禁用)]                      │
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
| 标题 | 卡片顶部 | "充值支付" | 黑体 36px |

### 3.3 信息展示模块

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| StaffInfoPage | 标题下方 | 嵌入模块 | 展示教职工信息 |
| 信息内容 | 姓名、部门、卡号、余额 | 标签+内容 | 当前余额在充值前显示 |

### 3.4 二维码区域

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| QR码图片 | 中央 | 250×250px | 支付二维码 |
| 提示文字 | QR码下方 | "请使用微信/支付宝扫描二维码" | 黑体 28px |
| 充值金额 | 提示下方 | "充值金额：¥{Amount}" | 黑体 32px bold, #d9230b |
| 有效期倒计时 | 底部 | "二维码有效期：{MM:SS}" | Arial 24px, #ff6000 |

### 3.5 底部按钮

#### 状态 1 - 等待支付

| 按钮 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 返回 | 左下角 | 金黄色渐变 | 取消充值，返回 |
| 已完成支付 | 右下角 | 主题色渐变 | 用户确认已完成扫码支付 |

#### 状态 2 - 支付处理中

| 按钮 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 返回 | 左下角 | 灰色禁用态 | 处理中不可返回 |

---

## 四、功能描述

### 4.1 页面用途

二维码页面用于展示充值支付二维码，引导教职工使用手机扫码完成支付。页面显示充值金额、支付二维码、二维码有效期，并在支付成功后展示支付结果。

### 4.2 核心功能

1. **二维码生成**
   - 根据充值金额生成支付二维码
   - 二维码包含支付金额、订单号等信息
   - 二维码有效期5分钟

2. **支付轮询**
   - 定时轮询支付状态
   - 检测用户是否已完成支付
   - 支付成功后进入写卡流程

3. **信息展示**
   - 嵌入 StaffInfoPage 展示教职工信息
   - 显示当前余额（充值前）
   - 显示充值金额

4. **超时处理**
   - 二维码5分钟有效期
   - 超时后提示重新生成
   - 自动刷新或返回重选金额

### 4.3 支付流程

```
用户选择充值金额
    ↓
生成支付二维码
    ↓
显示二维码页面
    ↓
用户手机扫码支付
    ↓
系统检测到支付成功
    ↓
显示支付成功
    ↓
进入 CardProcessPage 写卡
    ↓
完成充值
```

---

## 五、交互描述

### 5.1 状态转换

```
状态 1: 显示二维码
    ↓ 检测到支付成功 / 点击"已完成支付"
状态 2: 支付成功
    ↓ 自动跳转
CardProcessPage (写卡)
    ↓ 写卡完成
CompletionPage
```

### 5.2 详细交互

**1. 页面加载**:
- 接收充值金额参数
- 生成支付订单
- 获取支付二维码图片
- 嵌入 StaffInfoPage 模块
- 启动二维码有效期倒计时（5分钟）
- 启动支付状态轮询

**2. 二维码显示**:
- 显示250×250px二维码图片
- 显示充值金额
- 显示有效期倒计时
- 提示用户使用微信/支付宝扫码

**3. 支付检测**:
- 每3秒轮询一次支付状态
- 检测到支付成功后进入状态2
- 二维码过期后提示重新生成

**4. 用户确认支付**:
- 用户扫码支付后点击"已完成支付"
- 系统验证支付状态
- 支付成功：进入状态2
- 支付未完成：提示"支付未完成，请重新扫码"

**5. 支付成功**:
- 显示支付成功图标
- 显示支付详情（金额、方式、单号）
- 显示"正在写入卡片，请稍候..."
- 2秒后自动跳转到 CardProcessPage

**6. 异常处理**:
- **二维码过期**: 提示"二维码已过期"，提供"重新生成"按钮
- **支付失败**: 提示"支付失败，请重试"
- **网络异常**: 提示"网络异常，请检查网络连接"

### 5.3 参数传递

```csharp
public class QRCodePageParameter
{
    public decimal RechargeAmount { get; set; }  // 充值金额
    public StaffInfo StaffInfo { get; set; }     // 教职工信息
}

public class PaymentResult
{
    public bool Success { get; set; }
    public string TransactionId { get; set; }
    public string PaymentMethod { get; set; }  // WeChat/Alipay
    public DateTime PaymentTime { get; set; }
    public string ErrorMessage { get; set; }
}
```

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 标题 | #1a1919 |
| 信息标签 | #70706d |
| 信息内容 | #1a1919 |
| 充值金额 | #d9230b |
| 成功图标 | #4CAF50 |
| 返回按钮 | 金黄色渐变 |
| 已完成支付按钮 | 主题红色渐变 |
| 二维码有效期 | #ff6000 |
| 倒计时 | #ff6000 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 标题 | 黑体 | 36px | bold |
| 信息标签 | 黑体 | 28px | normal |
| 信息内容 | 黑体 | 28px | normal |
| 提示文字 | 黑体 | 28px | normal |
| 充值金额 | 黑体 | 32px | bold |
| 有效期 | Arial | 24px | normal |
| 按钮文字 | 黑体 | 36px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| 标题距顶 | 40px |
| 信息卡片距标题 | 30px |
| QR码区域距信息卡 | 40px |
| QR码尺寸 | 250×250px |
| 提示文字距QR码 | 30px |
| 金额距提示文字 | 20px |
| 有效期距底部 | 30px |
| 按钮尺寸 | 270×76px |

---

## 七、实现注意事项

### 7.1 支付轮询实现

```csharp
public partial class QRCodeViewModel : ViewModelBase
{
    [ObservableProperty]
    private string _qrCodeImage;
    
    [ObservableProperty]
    private decimal _rechargeAmount;
    
    [ObservableProperty]
    private TimeSpan _qrCodeValidity;
    
    [ObservableProperty]
    private bool _isPaymentSuccess;
    
    private Timer _paymentPollingTimer;
    private Timer _validityTimer;
    
    public void Initialize(QRCodePageParameter parameter)
    {
        RechargeAmount = parameter.RechargeAmount;
        QrCodeValidity = TimeSpan.FromMinutes(5);
        
        // 生成二维码
        GenerateQRCode();
        
        // 启动轮询
        _paymentPollingTimer = new Timer(PollPaymentStatus, null, TimeSpan.Zero, TimeSpan.FromSeconds(3));
        _validityTimer = new Timer(UpdateValidity, null, TimeSpan.Zero, TimeSpan.FromSeconds(1));
    }
    
    private async void PollPaymentStatus(object state)
    {
        var result = await PaymentService.CheckPaymentStatusAsync(orderId);
        if (result.IsPaid)
        {
            IsPaymentSuccess = true;
            _paymentPollingTimer?.Change(Timeout.Infinite, Timeout.Infinite);
            
            // 延迟后跳转
            await Task.Delay(2000);
            NavigationService.NavigateTo("CardProcessPage", new CardProcessParameter 
            { 
                Operation = CardOperation.Recharge,
                Amount = RechargeAmount 
            });
        }
    }
    
    partial void OnQrCodeValidityChanged(TimeSpan value)
    {
        if (value <= TimeSpan.Zero)
        {
            // 二维码过期
            ShowExpiredMessage();
        }
    }
}
```

### 7.2 二维码生成

```csharp
public interface IQRCodeGenerator
{
    Task<string> GeneratePaymentQRCodeAsync(decimal amount, string orderId);
}

public class QRCodeGenerator : IQRCodeGenerator
{
    public async Task<string> GeneratePaymentQRCodeAsync(decimal amount, string orderId)
    {
        // 调用支付接口生成二维码
        var paymentUrl = await PaymentService.CreatePaymentOrderAsync(amount, orderId);
        
        // 生成二维码图片
        var qrCodeImage = QRCodeHelper.Generate(paymentUrl, size: 250);
        
        return qrCodeImage;
    }
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [主界面设计文档](./HomePage.md)
- [充值页面](./RechargePage.md)
- [卡片处理页面](./CardProcessPage.md)
- [教职工信息模块](./StaffInfoPage.md)
- [操作完成界面](./CompletionPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，描述二维码展示和支付流程 | OpenCode Agent |

---

*文档结束*

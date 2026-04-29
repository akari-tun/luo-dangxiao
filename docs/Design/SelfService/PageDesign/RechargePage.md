# RechargePage Design Document

> **Page**: RechargePage  
> **Users**: Staff only  
> **Requires verification**: Yes  
> **Version**: 4.0  
> **Last updated**: 2026-04-29

---

## 1. 参考样图

| Sample | Path | Purpose |
|--------|------|---------|
| recharge_page.png | `/doc/Design/SelfService/Sample/recharge_page.png` | 信息确认状态视觉参考 |
| recharge_qr_code_page.png | `/doc/Design/SelfService/Sample/recharge_qr_code_page.png` | 扫码支付状态视觉参考 |

## 2. 页面定位与约束

1. 页面整体样式与 CheckInPage / TakeCardPage / ReportLossPage 一致。  
2. 仅教职工使用该页面，信息模块固定加载 `StaffInfoPage`。  
3. 页面在一个 View 内完成多种状态切换，不再拆分独立 QRCodePage。  

## 3. 页面状态

### 3.1 状态A：信息确认

- Title：`请确认信息后，点击相应金额进行充值`  
- 右上角：倒计时显示（从 `config.json` 的 `CountdownSeconds` 读取，默认 60 秒）
- 上半区：教职工信息（StaffInfoPage）  
- 中间区：操作状态提示文本（生成二维码时显示"正在生成支付二维码，请稍候..."，失败时显示错误信息）
- 下半区：充值金额按钮（￥20 / ￥50 / ￥100 / ￥200 / ￥500）  
- 交互：点击金额后，先隐藏金额按钮，显示操作提示，调用 YktApi 获取支付二维码

### 3.2 状态B：扫码支付

- Title：`确认支付金额后，扫描二维码进行支付`  
- 主体左右布局：  
  - 左侧：待支付金额（示例：`Amount pending payment: ¥100`）及提示文案  
  - 右侧：二维码区域（微信/支付宝扫码提示）  
- 交互：点击返回，回到"信息确认"状态。  

### 3.3 二维码生成中（中间状态）

- 当用户点击金额按钮后，先隐藏金额选择区
- 显示操作状态："正在生成支付二维码，请稍候..."
- 调用 `YktApi.GetTeacherRechargeQrCodeAsync` 接口
- **成功**：跳转到扫码支付状态（3.2），显示二维码
- **失败**：显示错误信息 3 秒后，重新显示金额选择区

## 4. 行为规则

### 4.1 倒计时
- 进入页面后，在右上角显示倒计时
- 倒计时时间从 `config.json` 的 `CountdownSeconds` 读取
- 倒计时结束且无操作时，自动返回主页
- 执行操作期间（正在生成二维码），倒计时不触发返回
- 操作完成后重置倒计时

### 4.2 返回行为
- 扫码支付状态点击返回：返回到信息确认状态。  
- 信息确认状态点击返回：返回 HomePage。  

### 4.3 二维码生成失败恢复
- 接口调用失败时，显示错误信息
- 错误信息显示 3 秒后自动清除并重新显示金额选择
- 倒计时在恢复后重新计时

## 5. 导航流程

```text
HomePage(自助充值)
  -> VerifyPage
  -> RechargePage(信息确认, 倒计时开始)
    -> [点击金额] -> 隐藏金额按钮 -> 显示操作状态 -> 调用YktApi
      -> [成功] -> RechargePage(扫码支付) -> 显示二维码
      -> [失败] -> 显示错误3秒 -> 重新显示金额按钮
    -> [倒计时结束] -> HomePage
```

## 6. 实现对齐说明

- HomePage "自助充值"按钮仍先进入 VerifyPage。  
- VerifyPage 验证成功后，`TargetFunction == "Recharge"` 跳转 RechargePage。  
- RechargePageViewModel 负责状态切换、标题切换、金额选择与返回逻辑。  
- 进入页面后，启动倒计时计时器，倒计时来源于 `SelfServiceConfig.CountdownSeconds`。  
- 点击金额时调用 `IYktApiClient.GetTeacherRechargeQrCodeAsync`，成功后展示二维码。  

## 7. RechargePageViewModel API 集成

### 7.1 接口调用

```csharp
// 请求参数
TeacherRechargeQrCodeRequestDto
{
    UserId: string,      // 从 StaffInfoModel.UserId 获取
    DealValue: decimal,  // 用户选择的充值金额
    WorkStationNumb: string  // 当前为空
}

// 响应处理
TeacherRechargeQrCodeResponseDto
{
    Code: int?,          // null/0/200 为成功
    Message: string?,    // 错误信息
    Data: JsonElement?   // 二维码 URL 字符串
}
```

### 7.2 二维码数据解析

从响应 `Data`（JsonElement）中提取二维码 URL，依次尝试以下字段名：
`url`, `qrCode`, `codeUrl`, `code_url`, `qr_url`, `qrUrl`, `payUrl`, `pay_url`，
若均不存在则取第一个字符串类型的属性值。

## 8. 版本历史

| Version | Date | Change | Author |
|---------|------|--------|--------|
| 1.0 | 2026-03-26 | Initial draft | OpenCode Agent |
| 2.0 | 2026-04-17 | Added Verify->Recharge route | GitHub Copilot |
| 3.0 | 2026-04-17 | 合并二维码内容到RechargePage，明确双状态与返回规则 | GitHub Copilot |
| 4.0 | 2026-04-29 | 添加倒计时、YktApi 集成、二维码生成中间状态及错误恢复 | Sisyphus |

# RechargePage Design Document

> **Page**: RechargePage  
> **Users**: Staff only  
> **Requires verification**: Yes  
> **Version**: 3.0  
> **Last updated**: 2026-04-17

---

## 1. 参考样图

| Sample | Path | Purpose |
|--------|------|---------|
| recharge_page.png | `/doc/Design/SelfService/Sample/recharge_page.png` | 信息确认状态视觉参考 |
| recharge_qr_code_page.png | `/doc/Design/SelfService/Sample/recharge_qr_code_page.png` | 扫码支付状态视觉参考 |

## 2. 页面定位与约束

1. 页面整体样式与 CheckInPage / TakeCardPage / ReportLossPage 一致。  
2. 仅教职工使用该页面，信息模块固定加载 `StaffInfoPage`。  
3. 页面在一个 View 内完成两种状态切换，不再拆分独立 QRCodePage。  

## 3. 页面状态

### 3.1 状态A：信息确认

- Title：`请确认信息后，点击相应金额进行充值`  
- 上半区：教职工信息（StaffInfoPage）  
- 下半区：充值金额按钮（￥20 / ￥50 / ￥100 / ￥200 / ￥500）  
- 交互：点击金额后进入“扫码支付”状态。  

### 3.2 状态B：扫码支付

- Title：`确认支付金额后，扫描二维码进行支付`  
- 主体左右布局：  
  - 左侧：待支付金额（示例：`待支付金额：¥100`）及提示文案  
  - 右侧：二维码区域（微信/支付宝扫码提示）  
- 交互：点击返回，回到“信息确认”状态。  

## 4. 返回行为

- 扫码支付状态点击返回：返回到信息确认状态。  
- 信息确认状态点击返回：返回 HomePage。  

## 5. 导航流程

```text
HomePage(自助充值)
  -> VerifyPage
  -> RechargePage(信息确认)
  -> RechargePage(扫码支付)
```

## 6. 实现对齐说明

- HomePage “自助充值”按钮仍先进入 VerifyPage。  
- VerifyPage 验证成功后，`TargetFunction == "Recharge"` 跳转 RechargePage。  
- RechargePageViewModel 负责状态切换、标题切换、金额选择与返回逻辑。  

## 7. 版本历史

| Version | Date | Change | Author |
|---------|------|--------|--------|
| 1.0 | 2026-03-26 | Initial draft | OpenCode Agent |
| 2.0 | 2026-04-17 | Added Verify->Recharge route | GitHub Copilot |
| 3.0 | 2026-04-17 | 合并二维码内容到RechargePage，明确双状态与返回规则 | GitHub Copilot |

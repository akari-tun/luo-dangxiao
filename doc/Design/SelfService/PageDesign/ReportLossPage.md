# 自助挂失页面设计文档 (`ReportLossPage`)

> 页面代码: `ReportLossPage`  
> 入口路径: `HomePage(自助挂失)` → `VerifyPage` → `ReportLossPage`  
> 文档版本: 1.1  
> 最后更新: 2026-04-17

## 一、页面目标
- 根据 `Sample/report_loss_page.png` 完成页面实现。
- 整体样式与 `CheckInPage`、`TakeCardPage` 保持一致。
- 标题文案与“挂失”按钮按卡状态动态变化。
- 用户信息区按系统配置（教职工/学员）加载不同子模块。

## 二、页面结构
- 背景: `bg.png`
- 顶部标题区: 图标 + 红色标题文案（与 `CheckInPage` 同风格）
- 信息区: 直接承载用户信息子页模块
- 底部按钮区:
  - 左侧固定 `返回`
  - 右侧 `挂失`（仅可挂失状态显示）

## 三、状态规则

### 3.1 正常卡状态
- 条件:
  - `StudentInfoModel.CardStatus == Normal`
  - 或 `StaffInfoModel.CardStatus == Normal`
- 标题显示: `确认信息后，点击[挂失]按钮进行挂失。`
- 右下角按钮: 显示 `挂失`

### 3.2 非正常卡状态
- 条件: 非 `Normal`（含已挂失、待领取、冻结、未制卡等）
- 标题显示: `您没有正常卡片或卡片已挂失，无法再挂失。`
- 右下角按钮: 隐藏

## 四、用户信息模块加载规则
- 配置为 `StaffSelfService`：加载 `StaffInfoPageView`
- 配置为 `StudentSelfService`：加载 `StudentInfoPageView`

## 五、交互流程
```text
HomePage 点击“自助挂失”
  -> Load VerifyPage(TargetFunction=ReportLoss)
  -> 身份证验证成功
  -> VerifyPageViewModel 按 TargetFunction 跳转
  -> HomePageViewModel.NavigateToReportLoss(userInfo)
  -> ReportLossPage
```

## 六、代码落地
- `ReportLossPageViewModel`
  - 新增页面参数 `ReportLossPageParameter`
  - 根据卡状态计算标题与按钮可见性
  - 执行 `ReportLossCommand` 后将卡状态更新为 `Lost`
  - 按配置选择 `StaffInfoPageView` 或 `StudentInfoPageView`
- `ReportLossPageView.axaml`
  - 复用 `CheckInPage`/`TakeCardPage` 的主框架布局
  - 标题绑定 `PageTitle`
  - 挂失按钮绑定 `ReportLossCommand` 与 `ShowReportLossButton`
- 导航改动
  - `HomePageViewModel` 新增 `NavigateToReportLoss`
  - `VerifyPageViewModel` 在 `TargetFunction == "ReportLoss"` 时跳转挂失页
- DI 与资源
  - `App.axaml.cs` 注册 `ReportLossPageViewModel`
  - `LanguageProvider` 与 `Language*.resx` 新增挂失标题/按钮文案键

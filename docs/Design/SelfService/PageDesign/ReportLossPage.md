# 自助挂失页面设计文档 (`ReportLossPage`)

> 页面代码: `ReportLossPage`  
> 入口路径: `HomePage(自助挂失)` → `VerifyPage` → `ReportLossPage`  
> 文档版本: 1.4  
> 最后更新: 2026-04-22

## 一、页面目标
- 根据 `Sample/report_loss_page.png` 完成页面实现。
- 整体样式与 `CheckInPage`、`TakeCardPage` 保持一致。
- 标题文案与“挂失”按钮按卡状态动态变化。
- 用户信息区按系统配置（教职工/学员）加载不同子模块。
- 页面进入后启动可配置倒计时，空闲超时自动返回首页。
- 点击“挂失”后调用一卡通挂失接口，并显示本地化处理状态。

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

## 六、倒计时与取消规则

### 6.1 页面倒计时
- 页面加载完成后立即启动 `SelfServiceConfig.CountdownSeconds` 倒计时。
- 倒计时文本绑定 `CountdownDisplay`，可见性绑定 `IsCountdownVisible`。
- 倒计时归零且当前**没有**进行挂失操作时：
  - 页面调用 `HomePageViewModel.ReturnHome()` 返回首页。

### 6.2 挂失操作期间
- 点击 `挂失` 时先重置倒计时，再调用 `IYktApiClient.LockCardAsync`。
- 当前实现中，挂失请求参数来源如下：
  - `CardNo` ← `StaffInfoModel.CardNumber` / `StudentInfoModel.CardNumber`
  - `FactoryFixId` ← `StaffInfoModel.FactoryFixId` / `StudentInfoModel.FactoryFixId`（物理卡号）
  - `OperatorId` ← `StaffInfoModel.UserId` / `StudentInfoModel.UserId`
  - `TenantId` ← `SelfServiceConfig.TenantId`
- 挂失超时与页面倒计时统一采用 `SelfServiceConfig.CountdownSeconds`：
  - 页面倒计时归零时取消当前挂失请求；
  - 同一个倒计时配置项也用于控制挂失请求的超时取消。
- 倒计时导致取消时：
  - 页面显示失败状态文案；
  - 不立即跳首页；
  - 而是重新开始一轮倒计时，留给用户查看结果。

### 6.3 成功后的状态刷新
- 接口返回 `code == null / 0 / 200` 视为成功。
- 成功后将当前用户卡状态更新为 `Lost`。
- 再次按卡状态重新计算标题与按钮显示：
  - 标题切换为不可挂失提示；
  - `挂失` 按钮隐藏。
- 同步刷新嵌入的用户信息模块卡状态展示。

## 七、代码落地
- `ReportLossPageViewModel`
  - 新增页面参数 `ReportLossPageParameter`
  - 根据卡状态计算标题与按钮可见性
  - 提供 `CountdownDisplay`、`IsCountdownVisible`、`OperationStatusText`、`IsOperationStatusVisible`
  - 执行 `ReportLossCommand` 时调用 `IYktApiClient.LockCardAsync(CardOperateRequestDto, CancellationToken)`
  - `LockCard` 入参中的 `FactoryFixId` / `OperatorId` / `TenantId` 分别来自 `StaffInfoModel|StudentInfoModel.FactoryFixId` / `StaffInfoModel|StudentInfoModel.UserId` / `SelfServiceConfig.TenantId`
  - 成功后将卡状态更新为 `Lost` 并刷新信息模块
  - 统一管理倒计时定时器与挂失请求 `CancellationTokenSource`
- 按配置选择 `StaffInfoPageView` 或 `StudentInfoPageView`
- `ReportLossPageView.axaml`
  - 复用 `CheckInPage`/`TakeCardPage` 的主框架布局
  - 标题绑定 `PageTitle`
  - 挂失按钮绑定 `ReportLossCommand` 与 `ShowReportLossButton`
- `ReportLossPageView.axaml.cs`
  - 页面卸载时调用 ViewModel 清理逻辑，确保不遗留倒计时与未完成请求
- 配置与资源
  - `SelfServiceConfig` 直接承载 `CountdownSeconds` 与 `TenantId`
  - `config.json` 提供默认配置项
  - `StaffInfoModel` 与 `StudentInfoModel` 新增 `FactoryFixId` 与 `UserId`
  - `luo.dangxiao.wabapi` 从 YktApi 返回数据的 `userCards` 中映射这两个字段
  - `LanguageProvider` 与 `Language*.resx` 新增挂失状态/倒计时文案键

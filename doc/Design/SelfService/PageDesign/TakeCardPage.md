# 自助取卡页面设计文档 (`TakeCardPage`)

> 页面代码: `TakeCardPage`
> 入口路径: `HomePage(自助取卡)` → `VerifyPage` → `TakeCardPage`
> 文档版本: 2.1

## 一、页面目标
- 依据 `take_card_sample.png`，并参考 `UserInfoPage` 布局生成页面。
- 验证成功后进入 `TakeCardPage`。
- 确认信息区按当前卡状态动态提示，并控制“确认取卡”按钮显示。

## 二、页面结构
- 背景: `bg.png`
- 顶部标题区: 样式参考 `UserInfoPageView` 的“您的个人账户信息”区域（图标 + 红色大标题）
- 内容容器: 居中圆角面板
- 底部按钮区: 左侧返回/右侧操作（完成态为中间按钮）

## 三、确认信息区规则

### 3.1 标题文案与按钮可见性（按卡状态）
- 待领取: 标题显示“请确认信息无误后取卡”，显示“确认取卡”按钮。
- 挂失: 标题显示“卡片已挂失，请使用自助补卡”，隐藏“确认取卡”按钮。
- 正常: 标题显示“当前卡片状态正常，无需补卡”，隐藏“确认取卡”按钮。
- 其他状态: 标题显示“您无需取卡”，隐藏“确认取卡”按钮。

### 3.2 领卡人信息模块
- 学员数据(`StudentInfoModel`)：直接加载 `StudentInfoPageView`。
- 教职工数据(`StaffInfoModel`)：直接加载 `StaffInfoPageView`。

### 3.3 领卡状态展示
- 保留“当前领卡状态：{状态}”展示区域。

## 四、状态机
1. `Confirm`：显示动态标题 + 用户信息模块 + 当前领卡状态，按规则显示“确认取卡”。
2. `Processing`：显示“制卡中”，返回不可用。
3. `CardReady`：显示“卡片已制作完成”，按钮为“已取卡”。
4. `Completed`：显示姓名、卡号、取卡时间，按钮为“完成”。

## 五、导航流程（已实现）
```text
HomePage 点击“自助取卡”
  -> 加载 VerifyPage(TargetFunction=TakeCard)
  -> 身份证验证成功
  -> VerifyPageViewModel 根据 TargetFunction 跳转
  -> HomePageViewModel.NavigateToTakeCard(userInfo)
  -> TakeCardPage
```

## 六、代码落地
- `TakeCardPageViewModel`：新增状态驱动标题文案与按钮控制逻辑。
- `TakeCardPageViewModel`：按用户类型加载 `StaffInfoPageView` / `StudentInfoPageView`。
- `TakeCardPageView.axaml`：确认信息区改为模块化展示，并保留当前领卡状态区域。

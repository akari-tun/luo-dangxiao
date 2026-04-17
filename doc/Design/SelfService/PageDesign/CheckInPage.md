# 报到页面设计文档 (CheckInPage)

> **页面代码**: `CheckInPage`  
> **适用用户**: 学员  
> **验证要求**: 已通过身份验证  
> **文档版本**: 1.1  
> **最后更新**: 2026-04-16

---

## 一、页面定位

`CheckInPage` 是“自助报到”功能专用页面，基于 `UserInfoPage` 的视觉结构实现。

- 内容区复用 `StudentInfoPage` 展示学员信息。
- 左下角固定显示“返回”按钮。
- 右下角 `报到` 按钮按报到状态动态显示。

---

## 二、页面结构

1. **背景层**：使用 `bg.png`。
2. **内容容器**：沿用 `UserInfoPage` 样式（圆角容器 + 标题区 + 内容区）。
3. **底部按钮区**：
   - 左侧：`返回`
   - 右侧：`报到`（仅未报到时显示）

---

## 三、状态规则

### 3.1 未报到状态

- 条件：`StudentInfoModel.CheckInStatus == NotCheckedIn`
- 标题显示：`确认信息后，点击[报到]按钮进行报到。`
- 右下角按钮：显示 `报到`

### 3.2 已报到状态

- 条件：`StudentInfoModel.CheckInStatus == CheckedIn`
- 标题显示：`你已成功报到。`
- 右下角按钮：隐藏

---

## 四、交互流程

```text
HomePage 点击“自助报到”
    ↓
进入 VerifyPage
    ↓
验证成功
    ↓
进入 CheckInPage
    ↓
根据 CheckInStatus 显示状态
    ├─ 未报到：可点击“报到”
    └─ 已报到：仅展示成功提示
```

---

## 五、实现要点

- `CheckInPageViewModel` 负责：
  - 解析传入用户数据为 `StudentInfoModel`
  - 根据 `CheckInStatus` 计算 `PageTitle` 与 `ShowCheckInButton`
  - 执行 `CheckInCommand` 后将状态更新为 `CheckedIn`
- `VerifyPageViewModel` 在 `TargetFunction == "CheckIn"` 时，验证成功后跳转 `CheckInPage`。
- `HomePageViewModel` 通过 `NavigateToCheckIn` 承载页面跳转。

---

## 六、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本 | OpenCode Agent |
| 1.1 | 2026-04-16 | 按实现重写：标题文案状态化、报到按钮按状态显示、Verify 成功后跳转 CheckInPage | GitHub Copilot |

---

*文档结束*

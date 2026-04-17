# 用户信息功能子页面设计文档 (UserInfoPage)

> **页面代码**: `UserInfoPage`  
> **适用用户**: 教职工、学员  
> **页面类型**: 功能子页面（父页面容器）  
> **文档版本**: 2.1  
> **最后更新**: 2026-04-16

---

## 一、页面定位

`UserInfoPage` 是验证成功后的公共信息展示页，作为 `StaffInfoPage` / `StudentInfoPage` 的承载容器。

- 终端配置为 `StaffSelfService` 时，加载 `StaffInfoPage`。
- 终端配置为 `StudentSelfService` 时，加载 `StudentInfoPage`。
- 页面负责统一背景、标题与内容容器，不承载业务操作按钮。

---

## 二、页面结构

1. **背景层**：使用 `bg.png`。
2. **内容容器**：显示标题“您的个人账户信息”，并通过 `ContentControl` 承载子模块。
3. **底部按钮区**：仅保留左侧 `返回` 按钮。

---

## 三、UI 要求

| 元素 | 要求 |
|------|------|
| 页面背景 | 使用 `bg.png` |
| 内容容器背景 | `#faf9f3` |
| 内容容器圆角 | `7px` |
| 标题 | 固定显示“您的个人账户信息” |
| 底部按钮 | 仅左侧“返回”，**无右下角业务按钮** |

---

## 四、交互流程

```text
HomePage 选择功能
    ↓
进入 VerifyPage
    ↓
验证成功
    ↓
跳转到对应目标页面
    ├─ CheckIn 功能 -> CheckInPage
    └─ 其他信息展示型功能 -> UserInfoPage
```

```text
UserInfoPage 点击返回
    ↓
返回 HomePage
```

---

## 五、实现要点

- `UserInfoPageViewModel` 继续负责根据 `CfgDataModel.ServiceType` 选择并加载子模块。
- `UserInfoPageView` 仅负责公共背景、标题与内容布局。
- 右下角业务操作按钮已从 `UserInfoPage` 移除。

---

## 六、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 2.0 | 2026-04-14 | 将 UserInfoPage 规范为公共信息容器 | OpenCode Agent |
| 2.1 | 2026-04-16 | 移除右下角按钮；明确 CheckIn 功能转由 CheckInPage 处理 | GitHub Copilot |

---

*文档结束*

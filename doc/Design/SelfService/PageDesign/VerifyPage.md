# 验证页面设计文档 (VerifyPage)

> **页面代码**: `VerifyPage`  
> **适用用户**: 学员、教职工  
> **验证要求**: 无（功能入口）  
> **文档版本**: 1.2  
> **最后更新**: 2026-04-16

---

## 一、页面定位

`VerifyPage` 是自助业务统一验证入口。用户在 `HomePage` 点击功能后，先进入本页完成身份验证，再根据目标功能跳转到对应业务页面。

---

## 二、页面结构

1. 验证模块区：通过 `ContentControl` 承载
   - `IDCardVerifyPage`
   - `SMSVerifyPage`
2. 底部按钮区：
   - 左侧固定 `返回`
   - 右侧为验证方式切换按钮（身份证/短信）

---

## 三、功能规则

### 3.1 验证方式

- 默认显示身份证验证模块。
- 可在身份证验证与短信验证间切换。

### 3.2 验证成功后的跳转分流

按 `TargetFunction` 跳转：

- `CheckIn` -> `CheckInPage`
- `TakeCard` -> `TakeCardPage`
- 其他功能 -> `UserInfoPage`

---

## 四、交互流程

```text
HomePage 点击功能按钮
    ↓
进入 VerifyPage
    ↓
完成身份验证
    ↓
验证成功
    ↓
按 TargetFunction 分流
    ├─ CheckIn  -> CheckInPage
    ├─ TakeCard -> TakeCardPage
    └─ Other    -> UserInfoPage
```

---

## 五、实现对齐说明

### 5.1 ViewModel 关键点

- `VerifyPageViewModel.TargetFunction`：记录当前业务目标。
- `SwitchToIDCardCommand` / `SwitchToSMSCommand`：切换验证模块。
- `OnVerificationSucceeded(...)`：处理验证成功并执行页面分流。

### 5.2 与 HomePage 协作

- `HomePageViewModel.OnFunctionButtonClick(...)` 设置 `SelectedFunction` 并加载 `VerifyPage`。
- `HomePageViewModel` 提供以下跳转方法：
  - `NavigateToCheckIn(...)`
  - `NavigateToTakeCard(...)`
  - `NavigateToUserInfo(...)`

---

## 六、相关文档

- [主界面设计文档](./HomePage.md)
- [报到页面设计文档](./CheckInPage.md)
- [用户信息页面设计文档](./UserInfoPage.md)
- [身份证验证模块](./IDCardVerifyPage.md)
- [短信验证模块](./SMSVerifyPage.md)

---

## 七、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本 | OpenCode Agent |
| 1.1 | 2026-04-16 | 增加 TargetFunction 分流描述 | GitHub Copilot |
| 1.2 | 2026-04-16 | 与现有代码完全对齐，明确 CheckIn/TakeCard/UserInfo 跳转链路 | GitHub Copilot |

---

*文档结束*

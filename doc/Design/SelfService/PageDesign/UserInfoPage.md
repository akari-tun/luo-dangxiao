# 用户信息功能子页面设计文档 (UserInfoPage)

> **页面代码**: UserInfoPage  
> **适用用户**: 教职工、学员  
> **页面类型**: 功能子页面（父页面容器）  
> **文档版本**: 2.0  
> **最后更新**: 2026-04-14

---

## 一、页面定位

`UserInfoPage` 是身份验证成功后的公共信息页，不直接定义固定字段，而是作为承载子模块的父页面使用。

- 终端配置为 `StaffSelfService` 时，加载 `StaffInfoPage`。
- 终端配置为 `StudentSelfService` 时，加载 `StudentInfoPage`。
- 页面负责统一背景、标题说明、内容容器与底部按钮；字段布局由子模块负责。

---

## 二、相关文档

| 文档 | 作用 |
|------|------|
| `./StaffInfoPage.md` | 教职工信息模块设计 |
| `./StudentInfoPage.md` | 学员信息模块设计 |
| `../自助终端设计规格说明书.md` | 页面层级与整体规范 |
| `./VerifyPage.md` | 身份验证完成后的上游页面 |

---

## 三、页面结构

1. **背景层**：使用与 `VerifyPage` 相同的 `bg.png`。
2. **内容容器**：显示标题、说明文案，并通过 `ContentControl` 承载 `StaffInfoPage` 或 `StudentInfoPage`。
3. **底部按钮区**：左侧固定为“返回”，右侧根据目标功能显示业务按钮。

```text
读取 CfgDataModel.ServiceType
        ↓
判断终端配置
        ├─ StaffSelfService   → StaffInfoPage
        └─ StudentSelfService → StudentInfoPage
        ↓
将验证成功后的用户数据传入对应模块
        ↓
渲染到 UserInfoPage 内容区
```

---

## 四、UI 要求

| 元素 | 要求 |
|------|------|
| 页面背景 | 使用 `bg.png` |
| 内容容器背景 | `#faf9f3` |
| 内容容器圆角 | `7px` |
| 容器内边距 | `30px` |
| `StaffInfoPage` | 只负责字段布局，不绘制页面背景色 |
| `StudentInfoPage` | 只负责字段布局，不绘制页面背景色 |
| 标题 | 根据配置显示“教职工信息”或“学员信息” |
| 业务按钮 | 根据目标功能显示，如“报到”“领卡”“挂失”“补卡”“查询”“充值” |

---

## 五、数据模型

用户数据统一定义在 `luo.dangxiao.models`：

```csharp
public class UserInfoModel
{
    public string Id { get; set; }
    public string Name { get; set; }
    public UserType UserType { get; set; }
    public string IdCardNumber { get; set; }
}

public sealed class StaffInfoModel : UserInfoModel
{
    public string EmployeeNumber { get; set; }
    public string CardType { get; set; }
    public DateTime? CardExpiryDate { get; set; }
    public string Department { get; set; }
    public decimal ConsumptionBalance { get; set; }
    public decimal SubsidyBalance { get; set; }
}

public sealed class StudentInfoModel : UserInfoModel
{
    public string RoomName { get; set; }
    public DateTime? CheckInStartTime { get; set; }
    public DateTime? CheckInEndTime { get; set; }
    public string ClassName { get; set; }
    public DateTime TrainingStartDate { get; set; }
    public DateTime TrainingEndDate { get; set; }
}
```

---

## 六、交互流程

```text
HomePage 选择业务
    ↓
进入 VerifyPage
    ↓
点击 Test Read ID Card
    ↓
模拟调用读身份证方法
    ↓
模拟读卡成功并生成用户数据
    ↓
跳转到 UserInfoPage
    ↓
根据配置加载 StaffInfoPage / StudentInfoPage
```

```text
UserInfoPage 点击返回
    ↓
清空当前子页面内容
    ↓
返回 HomePage
```

---

## 七、实现要点

- `UserInfoPageViewModel` 负责读取 `CfgDataModel.ServiceType` 并决定加载哪个子模块。
- `UserInfoPageView` 负责绘制 `bg.png` 背景与公共容器。
- `IDCardVerifyPage` 的测试按钮需要模拟读卡成功，并把生成的 `UserInfoModel` 子类传递到 `UserInfoPage`。
- `StaffInfoPageView` 与 `StudentInfoPageView` 不重复绘制页面背景色。

---

## 八、相关文档

- [教职工信息模块](./StaffInfoPage.md)
- [学员信息模块](./StudentInfoPage.md)
- [身份验证页面](./VerifyPage.md)
- [设计规格说明书](../自助终端设计规格说明书.md)

---

*文档结束*

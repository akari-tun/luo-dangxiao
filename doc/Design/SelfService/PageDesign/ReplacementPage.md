# 自助补卡界面设计文档 (ReplacementPage)

> 页面代码: `ReplacementPage`
> 设计参考: `doc/Design/SelfService/Sample/replacement_page.png`
> 文档版本: 2.0

---

## 1. 页面定位

`ReplacementPage` 用于"自助补卡"业务的确认页。
整体视觉风格与 `CheckInPage`、`TakeCardPage`、`ReportLossPage` 保持一致（背景、标题区、用户信息区、底部按钮区一致）。

---

## 2. 跳转流程

### 2.1 自助补卡流程

```
访问自助服务主页
    ↓
点击"补办卡片"入口
    ↓
加载 VerifyPage(TargetFunction=Replacement)
    ↓
Verify 成功后跳转 ReplacementPage
    ↓
显示倒计时 + 用户信息及补卡须知
    ↓
点击"补卡"按钮
    ↓
进入制卡流程（移动卡片→读卡→初始化→写卡→打印→出卡）
    ↓
卡片就绪后提示取卡
    ↓
完成补卡
```

- HomePage 功能按钮参数：`Replacement`
- Verify 成功后根据 `TargetFunction == "Replacement"` 跳转 `ReplacementPage`

---

## 3. 页面结构

1. **标题区**（左图标 + 标题文本 + 右上角倒计时）
   - 倒计时显示：橙红色 `#FF6000` 粗体 36px，`config.json` 中 `CountdownSeconds` 控制时长
   - 倒计时结束（无操作时）自动返回主页
2. **用户信息区**（嵌入子页面）
3. **状态面板**（圆角背景面板，按状态显示不同内容）
4. **底部按钮区**
   - 左侧：返回按钮（确认状态可见）
   - 右侧：补卡按钮 / 已补卡按钮 / 完成按钮

---

## 4. 用户信息模块加载规则

根据 App 配置中的 `CfgDataModel.ServiceType` 决定加载模块：

- `StaffSelfService` -> `StaffInfoPage`
- `StudentSelfService` -> `StudentInfoPage`

> 注意：此规则优先于用户数据运行时类型判断，确保与设备配置一致。

---

## 5. 标题与按钮显示规则（核心）

根据卡片的 `CurrentCard.CardStatusId` 状态显示标题与按钮：

### 5.1 正常状态
- 标题："您当前的卡片为正常状态，请挂失后再补卡。"
- 按钮：不显示"补卡"按钮

### 5.2 挂失状态（CardStatusId == Lost）
- 标题："确认信息后，点击[补卡]按钮进行补卡。"
- 按钮：显示"补卡"按钮

### 5.3 其他状态
- 标题："您没有已挂失的卡片，无法进行补卡。"
- 按钮：不显示"补卡"按钮

---

## 6. 状态机

### 6.1 CardProcessingState 枚举
| 状态值 | 说明 | 显示内容 |
|--------|------|----------|
| `Confirm` | 确认状态（初始） | 标题 + 用户信息模块 + "补卡"按钮（仅挂失时可见） |
| `CardProcessing` | 制卡处理中 | "补卡制卡中" + 操作步骤文字（移动/读卡/初始化/写卡/打印） |
| `CardReadyToPickup` | 卡片完成待取 | "补卡制作完成" + "↓↓↓↓↓↓" + 取卡提示 + "已补卡"按钮 |
| `Completed` | 补卡完成 | "补卡完成" + "完成"按钮 |
| `OperationFailed` | 操作失败 | "补卡失败" + 失败原因 + 倒计时已结束 |

### 6.2 制卡流程
补卡的制卡流程与取卡完全相同，共享 `CardOperationViewModelBase` 中的实现：

1. 移动卡片到读卡位置 → 2. 模拟读卡 → 3. 调用 API 初始化 → 4. 模拟写卡 → 5. 打印卡片 → 6. 移动到持卡位 → 7. 提示用户取卡

唯一区别在于：
- API 初始化时 `CardOperate` 参数传 **"换卡"**（取卡传"新卡"）
- 界面提示文字为补卡相关文案

### 6.3 倒计时逻辑
| 场景 | 行为 |
|------|------|
| `Confirm` 状态无操作 | 倒计时结束后自动返回主页 |
| `CardProcessing` 状态 | 倒计时重置用于操作超时保护，超时则返回失败 |
| `CardReadyToPickup` 状态 | 倒计时监控，超时则检查卡片位置并移至废卡槽 |

---

## 7. 代码落地

### 7.1 ViewModel 继承关系

`ReplacementPageViewModel` 继承自 `CardOperationViewModelBase`，与 `TakeCardPageViewModel` 共享：
| 共享功能 | 说明 |
|----------|------|
| 倒计时管理 | 启动/停止/重置，超时自动返回主页 |
| 卡片处理管线 | `ExecuteCardProcessAsync()` 方法执行完整制卡流程 |
| 取卡监控 | 倒计时结束后检查卡片位置，未取走则作废 |
| 打印机操作 | 移动卡片、读卡、写卡、打印 |
| API 调用 | InitCard、WriteCardSuccess、WriteCardFailure |

`ReplacementPageViewModel` 自行实现：
| 自有功能 | 说明 |
|----------|------|
| 挂失状态判断 | 仅 `CardStatusId == Lost` 时可补卡 |
| LoadData | 加载用户信息模块 |
| 标题/按钮逻辑 | 按状态显示标题和补卡按钮 |

### 7.2 相关文件
- `ReplacementPageViewModel.cs` — ViewModel，继承 `CardOperationViewModelBase`
- `ReplacementPageView.axaml` — 页面定义，倒计时+状态面板+按钮区
- `CardOperationViewModelBase.cs` — 抽象基类，提供共享的卡片操作管线
- `TakeCardPageViewModel.cs` — 取卡 ViewModel，同样继承 `CardOperationViewModelBase`

---

## 8. 相关文档
- [`TakeCardPage` 设计文档](./TakeCardPage.md)
- [`CardOperationViewModelBase` 抽象基类](../../../SelfService/card-operation.md)
- [打印机模块设计文档](../../../SelfService/printer-design.md)
- [YktApi 接口文档](../../Api/YktApi.md)

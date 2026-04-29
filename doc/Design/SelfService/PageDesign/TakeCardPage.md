# 自助取卡页面设计文档 (`TakeCardPage`)

> 页面代码: `TakeCardPage`
> 入口路径: `HomePage(自助取卡)` → `VerifyPage` → `TakeCardPage`
> 文档版本: 3.0

## 一、页面目标
- 依据 `take_card_sample.png`，并参考 `UserInfoPage` 布局生成页面。
- 验证成功后进入 `TakeCardPage`。
- 确认信息区按当前卡状态动态提示，并控制"确认取卡"按钮显示。
- **新增**：倒计时功能，倒计时结束后自动返回主页。
- **新增**：完整制卡流程（读卡→初始化→写卡→打印→出卡）。

## 二、页面结构
- 背景: `bg.png`
- 顶部标题区: 样式参考 `UserInfoPageView` 的"您的个人账户信息"区域（图标 + 红色大标题 + 倒计时）
  - **左侧**：图标 + 动态标题文字
  - **右侧**：倒计时显示（橙红色 `#FF6000`，36px 粗体），仅在 `IsCountdownVisible = true` 时可见
- 内容容器: 居中圆角面板
- 底部按钮区: 左侧返回/右侧操作（完成态为中间按钮）

## 三、确认信息区规则

### 3.1 标题文案与按钮可见性（按卡状态）
- 待领取 (`CurrentCard == null`): 标题显示"请确认信息无误后取卡"，显示"确认取卡"按钮。
- 挂失 (`CardStatusId == Lost`): 标题显示"卡片已挂失，请使用自助补卡"，隐藏"确认取卡"按钮。
- 正常 (`CardStatusId == Normal`): 标题显示"当前卡片状态正常，无需补卡"，隐藏"确认取卡"按钮。
- 其他状态: 标题显示"您无需取卡"，隐藏"确认取卡"按钮。

### 3.2 领卡人信息模块
- 学员数据(`StudentInfoModel`)：直接加载 `StudentInfoPageView`。
- 教职工数据(`StaffInfoModel`)：直接加载 `StaffInfoPageView`。

### 3.3 领卡状态展示
- 保留"当前领卡状态：{状态}"展示区域。

## 四、状态机

### 4.1 TakeCardFlowState 枚举
| 状态值 | 说明 | 显示内容 |
|--------|------|----------|
| `Confirm` | 确认状态（初始） | 动态标题 + 用户信息模块 + 当前领卡状态 + "确认取卡"按钮 |
| `CardProcessing` | 制卡处理中 | "制卡中"标题 + 操作步骤文字 + "请勿离开，卡片正在制作中"提示 |
| `CardReadyToPickup` | 卡片已完成待取 | "卡片已制作完成" + "↓↓↓↓↓↓" + 取卡提示 + "已取卡"按钮 |
| `CardReady` | 兼容状态 | "卡片已制作完成，请从出卡口领取。" |
| `Completed` | 取卡完成 | 姓名/卡号/取卡时间 + "完成"按钮 |
| `OperationFailed` | 操作失败 | "卡片制作失败，请重试。" |

### 4.2 制卡流程步骤
```
Confirm (点击"确认取卡")
  → CardProcessing
    → 移动卡片到读卡位置 (MoveCard: MoveFromStorageToPrepare → MoveToContact)
    → 模拟读卡，返回物理卡号 (FactoryFixId)
    → 调用 YktApi.InitCard 初始化卡片
    → 模拟写卡（写入卡号、人员编号、有效期）
    → 调用打印机打印卡片
    → 移动卡片到前端持卡位 (MoveCard: MoveToFront)
  → CardReadyToPickup (提示用户取卡)
    → 用户点击"已取卡"
  → Completed (显示取卡完成详情)
    → 用户点击"完成"
  → 返回主页

任意步骤倒计时到期:
  → 处理中 → OperationFailed
  → 待取卡 → 检查卡片位置，若仍在打印机 → 移至废卡槽
```

### 4.3 倒计时逻辑
| 场景 | 行为 |
|------|------|
| `Confirm` 状态无操作 | 倒计时结束后自动返回主页 |
| `CardProcessing` 状态 | 倒计时重置用于操作超时保护，超时则返回失败 |
| `CardReadyToPickup` 状态 | 倒计时监控用户是否取卡，超时则检查卡片位置并移至废卡槽 |
| 操作期间 | 每步操作重置倒计时 |

**配置来源**：`config.json` 中的 `CountdownSeconds` 字段（默认 60 秒）。

## 五、制卡流程详情

### 5.1 打印机接口
- 使用 `luo.dangxiao.printer` 命名空间的 `CardPrinterBase` 抽象类。
- 根据配置选择具体实现：`VirtualCardPrinter`（开发/测试）或 `SeaoryPrinterDriver`（生产）。
- 配置来源：`config.json` → `PrinterConfig.Provider`。

### 5.2 API 接口
| 接口 | 说明 | 请求位置 |
|------|------|----------|
| `InitCardAsync` | 卡片初始化 | `card/api/v1/init` |
| `WriteCardSuccessAsync` | 写卡成功上报 | `card/api/v1/write/success` |
| `WriteCardFailureAsync` | 写卡失败上报 | `card/api/v1/write/failure` |

### 5.3 InitCard 请求参数
```json
{
  "cardId": "<生成的GUID>",
  "userId": "<用户Id>",
  "cardTypeId": "1",
  "expiryDate": "2030-12-31",
  "factoryFixId": "<物理卡号(读卡结果)>",
  "mainDeputyType": "1",
  "cardNo": "<生成的卡流水号>",
  "cardOperate": "新卡/换卡",
  "workStationNumb": "<打印机ID>",
  "tenantId": "<配置中的TenantId>",
  "oldCardNo": "<原卡流水号(换卡时)>",
  "oldFactoryFixId": "<原卡物理卡号(换卡时)>",
  "oldCardId": "<原卡Id(换卡时)>"
}
```

### 5.4 测试代码说明
- **读卡**：使用模拟数据 `PHYS-{printerId}-{timestamp}` 格式生成物理卡号。
- **写卡**：模拟写入卡片流水号、人员编号、卡有效期，返回写卡成功/失败结果。
- **打印**：使用 `CardPrintSessionBase` 打印持卡人姓名、卡号、有效期。

## 六、导航流程（已实现）
```text
HomePage 点击"自助取卡"
  -> 加载 VerifyPage(TargetFunction=TakeCard)
  -> 身份证验证成功
  -> VerifyPageViewModel 根据 TargetFunction 跳转
  -> HomePageViewModel.NavigateToTakeCard(userInfo)
  -> TakeCardPage (启动倒计时)
```

## 七、依赖注入
`TakeCardPageViewModel` 通过 DI 注入以下依赖：
| 依赖类型 | 说明 |
|----------|------|
| `SelfServiceConfig` | 配置数据（倒计时秒数、租户Id） |
| `CardPrinterBase` | 打印机接口（由 `CardPrinterFactory` 创建） |
| `IYktApiClient?` | YKT API 客户端（可选，未配置时使用模拟数据） |

## 八、代码落地
- `CardOperationViewModelBase.cs`：抽象基类，抽取取卡与补卡共享的倒计时管理、卡片处理管线、取卡监控等逻辑。
- `TakeCardPageViewModel.cs`：继承 `CardOperationViewModelBase`，实现取卡特有逻辑（状态驱动标题文案、按钮控制、用户信息模块加载）。
- `TakeCardPageView.axaml`：确认信息区改为模块化展示，并保留当前领卡状态区域。
- `TakeCardPageView.axaml`：**新增**右上角倒计时显示（橙红色加粗文字）。
- `TakeCardPageView.axaml`：**新增**制卡进度文字显示（`OperationStepText`）。
- `TakeCardPageView.axaml`：**新增**`CardReadyToPickup` 状态 UI。

## 九、相关文档
- [`ReplacementPage` 设计文档](./ReplacementPage.md)（共享同一卡片操作管线）
- [`CardOperationViewModelBase` 抽象基类](../../../SelfService/card-operation.md)
- [`config.json` 配置说明](../../../SelfService/config-readme.md)
- [打印机模块设计文档](../../../SelfService/printer-design.md)
- [YktApi 接口文档](../../Api/YktApi.md)

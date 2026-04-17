# 自助补卡界面设计文档 (ReplacementPage)

> 页面代码: `ReplacementPage`  
> 设计参考: `doc/Design/SelfService/Sample/replacement_page.png`  
> 最后更新: 2026-04-17

---

## 1. 页面定位

`ReplacementPage` 用于“自助补卡”业务的确认页。  
整体视觉风格与 `CheckInPage`、`TakeCardPage`、`ReportLossPage` 保持一致（背景、标题区、用户信息区、底部按钮区一致）。

---

## 2. 跳转流程

### 2.1 自助补卡流程

```
访问自助服务主页
    ↓
点击"补办卡片"入口
    ↓
加载 ReplacementPage
    ↓
显示用户信息及补卡须知
    ↓
用户确认补卡申请
    ↓
判断原卡状态
    ↓
┌──────────┬───────────┐
│          │           │
原卡正常    原卡已作废   需补办新卡片
    │          │           │
直接进入     提示用户      跳转补卡页面
制卡流程     原卡已作废
```

- HomePage 功能按钮参数：`Replacement`
- Verify 成功后根据 `TargetFunction == "Replacement"` 跳转 `ReplacementPage`

---

## 3. 页面结构

1. 标题区（左图标 + 标题文本）
2. 用户信息区（嵌入子页面）
3. 底部按钮区
   - 左侧：返回按钮
   - 右侧：补卡按钮（按状态控制显隐）

---

## 4. 用户信息模块加载规则

根据 App 配置中的 `CfgDataModel.ServiceType` 决定加载模块：

- `StaffSelfService` -> `StaffInfoPage`
- `StudentSelfService` -> `StudentInfoPage`

> 注意：此规则优先于用户数据运行时类型判断，确保与设备配置一致。

---

## 5. 标题与按钮显示规则（核心）

根据卡片状态显示标题与按钮：

### 5.1 正常状态
- 条件：
  - 学员：`StudentCardStatus.Normal`
  - 教职工：`StaffCardStatus.Normal`
- 标题：
  - **您当前的卡片为正常状态，请挂失后再补卡。**
- 按钮：
  - 不显示“补卡”按钮

### 5.2 挂失状态
- 条件：
  - 学员：`StudentCardStatus.Lost`
  - 教职工：`StaffCardStatus.Lost`
- 标题：
  - **确认信息后，点击[补卡]按钮进行补卡。**
- 按钮：
  - 显示“补卡”按钮

### 5.3 其他状态
- 条件：除上述外（如待领卡、未制卡、冻结等）
- 标题：
  - **您没有已挂失的卡片，无法进行补卡。**
- 按钮：
  - 不显示“补卡”按钮

---

## 6. 交互说明

- 点击“补卡”后执行补卡命令（当前实现为模拟流程），可在后续对接设备制卡服务。
- 点击“返回”回到 HomePage 主界面。

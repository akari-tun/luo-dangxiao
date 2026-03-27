# 自助服务页面设计文档目录

> **文档说明**: 本目录包含自助服务系统所有页面的详细设计文档  
> **系统**: 自助服务终端 (SelfService)  
> **用户**: 学员、教职工  
> **最后更新**: 2026-03-26

---

## 一、页面分类

系统页面分为三类：

### 1.1 主页面 (1个)

| 文档 | 页面代码 | 说明 |
|------|----------|------|
| [HomePage](./HomePage.md) | HomePage | 主界面容器，三种状态：教职工自助/学员自助/子页面容器 |

### 1.2 功能子页面 (10个)

| 文档 | 页面代码 | 说明 | 用户类型 |
|------|----------|------|----------|
| VerifyPage.md | VerifyPage | 统一验证入口 | 全部 |
| TakeCardPage.md | TakeCardPage | 自助取卡 | 全部 |
| ReportLossPage.md | ReportLossPage | 自助挂失 | 全部 |
| ReplacementPage.md | ReplacementPage | 自助补卡 | 全部 |
| QueryPage.md | QueryPage | 自助查询 | 教职工 |
| RechargePage.md | RechargePage | 自助充值 | 教职工 |
| QRCodePage.md | QRCodePage | 二维码展示 | 教职工 |
| CheckInPage.md | CheckInPage | 自助报到 | 学员 |
| CardProcessPage.md | CardProcessPage | 卡片处理过程 | 全部 |
| CompletionPage.md | CompletionPage | 操作完成 | 全部 |

### 1.3 模块页面 (4个)

| 文档 | 页面代码 | 说明 | 用途 |
|------|----------|------|------|
| StudentInfoPage.md | StudentInfoPage | 学员信息模块 | 嵌入功能子页面展示学员信息 |
| StaffInfoPage.md | StaffInfoPage | 教职工信息模块 | 嵌入功能子页面展示教职工信息 |
| IDCardVerifyPage.md | IDCardVerifyPage | 身份证验证模块 | 嵌入VerifyPage |
| SMSVerifyPage.md | SMSVerifyPage | 短信验证模块 | 嵌入VerifyPage |

---

## 二、页面架构

### 2.1 HomePage 作为容器

```
┌─────────────────────────────────────┐
│  HEADER (顶部栏)                    │
│  [Logo] 标题 [倒计时]               │
├─────────────────────────────────────┤
│                                     │
│     状态1: 教职工/学员自助          │
│     ┌───────────────────────┐       │
│     │ 功能按钮菜单          │       │
│     │ [取卡][挂失][补卡]... │       │
│     └───────────────────────┘       │
│                                     │
│     状态2: 子页面容器               │
│     ┌───────────────────────┐       │
│     │                       │       │
│     │   功能子页面内容       │       │
│     │   (嵌入显示)          │       │
│     │                       │       │
│     └───────────────────────┘       │
│                                     │
├─────────────────────────────────────┤
│  [返回按钮]  [操作按钮]             │
└─────────────────────────────────────┘
```

### 2.2 页面加载关系

```
HomePage (容器)
    ├── 加载 VerifyPage
    │       ├── 嵌入 IDCardVerifyPage (身份证验证)
    │       └── 嵌入 SMSVerifyPage (短信验证)
    │
    ├── 加载 TakeCardPage
    │       ├── 嵌入 StaffInfoPage (教职工)
    │       └── 嵌入 StudentInfoPage (学员)
    │
    ├── 加载 ReportLossPage
    │       ├── 嵌入 StaffInfoPage (教职工)
    │       └── 嵌入 StudentInfoPage (学员)
    │
    ├── 加载 ReplacementPage
    │       ├── 嵌入 StaffInfoPage (教职工)
    │       └── 嵌入 StudentInfoPage (学员)
    │
    ├── 加载 QueryPage
    │       └── 嵌入 StaffInfoPage (教职工)
    │
    ├── 加载 RechargePage
    │       ├── 嵌入 StaffInfoPage (教职工)
    │       └── 加载 QRCodePage (支付)
    │
    ├── 加载 CheckInPage
    │       └── 嵌入 StudentInfoPage (学员)
    │
    ├── 加载 CardProcessPage
    │
    └── 加载 CompletionPage
```

---

## 三、功能流程

### 3.1 教职工自助流程

| 功能 | 页面流程 |
|------|----------|
| **自助取卡** | HomePage → VerifyPage → TakeCardPage → CardProcessPage → CompletionPage |
| **自助挂失** | HomePage → VerifyPage → ReportLossPage → CardProcessPage → CompletionPage |
| **自助补卡** | HomePage → VerifyPage → ReplacementPage → CardProcessPage → CompletionPage |
| **自助查询** | HomePage → VerifyPage → QueryPage |
| **自助充值** | HomePage → VerifyPage → RechargePage → QRCodePage → CardProcessPage → CompletionPage |

### 3.2 学员自助流程

| 功能 | 页面流程 |
|------|----------|
| **自助取卡** | HomePage → VerifyPage → TakeCardPage → CardProcessPage → CompletionPage |
| **自助挂失** | HomePage → VerifyPage → ReportLossPage → CardProcessPage → CompletionPage |
| **自助补卡** | HomePage → VerifyPage → ReplacementPage → CardProcessPage → CompletionPage |
| **自助报到** | HomePage → VerifyPage → CheckInPage → CompletionPage |

---

## 四、文档阅读指南

### 4.1 文档结构

每个页面设计文档包含：
1. **设计图引用** - 设计图源文件、切图资源
2. **页面布局结构** - ASCII 布局图、状态展示
3. **UI 元素详解** - 顶部栏、内容区、底部按钮
4. **功能描述** - 页面用途、核心功能
5. **交互描述** - 状态转换、交互流程
6. **设计规范** - 颜色、字体、间距
7. **实现注意事项** - 数据模型、ViewModel
8. **相关文档** - 关联文档链接

### 4.2 通用规范

| 规范项 | 值 |
|--------|-----|
| 主色调 | 红色系 #C42828, #d9230b |
| 辅助色 | 金黄色 #e6b84d, #ffe3a3 |
| 内容区背景 | 米黄色 #faf9f3 |
| 中文字体 | 黑体 (SimHei) |
| 英文字体 | Arial |
| 卡片圆角 | 7px |
| 按钮圆角 | 8px |
| 倒计时位置 | 左上角，#ff6000 |

---

## 五、快速参考

### 5.1 页面类型速查

| 页面 | 类型 | 容器 | 嵌入模块 |
|------|------|------|----------|
| HomePage | 主页面 | - | - |
| VerifyPage | 功能子页面 | HomePage | IDCardVerifyPage, SMSVerifyPage |
| TakeCardPage | 功能子页面 | HomePage | StaffInfoPage/StudentInfoPage |
| ReportLossPage | 功能子页面 | HomePage | StaffInfoPage/StudentInfoPage |
| ReplacementPage | 功能子页面 | HomePage | StaffInfoPage/StudentInfoPage |
| QueryPage | 功能子页面 | HomePage | StaffInfoPage |
| RechargePage | 功能子页面 | HomePage | StaffInfoPage |
| QRCodePage | 功能子页面 | HomePage | - |
| CheckInPage | 功能子页面 | HomePage | StudentInfoPage |
| CardProcessPage | 功能子页面 | HomePage | - |
| CompletionPage | 功能子页面 | HomePage | - |
| StudentInfoPage | 模块页面 | 嵌入功能子页面 | - |
| StaffInfoPage | 模块页面 | 嵌入功能子页面 | - |
| IDCardVerifyPage | 模块页面 | 嵌入VerifyPage | - |
| SMSVerifyPage | 模块页面 | 嵌入VerifyPage | - |

### 5.2 用户类型可用页面

**学员可用**:
- HomePage (学员自助状态)
- VerifyPage → TakeCardPage, ReportLossPage, ReplacementPage, CheckInPage
- CardProcessPage, CompletionPage
- StudentInfoPage (模块)

**教职工可用**:
- HomePage (教职工自助状态)
- VerifyPage → TakeCardPage, ReportLossPage, ReplacementPage, QueryPage, RechargePage, QRCodePage
- CardProcessPage, CompletionPage
- StaffInfoPage (模块)

---

## 六、相关文档

- [自助服务设计规格说明书](../设计规格说明书.md)
- [卡务中心设计](../../CardCenter/)

---

## 七、版本历史

| 版本 | 日期 | 更新内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 创建页面设计文档目录 | OpenCode Agent |
| 2.0 | 2026-03-26 | 更新页面分类：<br>• 明确三类页面（主页面/功能子页面/模块页面）<br>• 描述HomePage容器机制<br>• 完善功能流程<br>• 添加页面关系图 | OpenCode Agent |

---

*文档结束*

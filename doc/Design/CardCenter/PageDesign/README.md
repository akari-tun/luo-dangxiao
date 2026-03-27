# 卡务中心页面设计文档目录

> **文档说明**: 本目录包含卡务中心所有页面的详细设计文档  
> **系统**: 卡务中心 (CardCenter)  
> **用户**: 管理员、卡务工作人员  
> **最后更新**: 2026-03-26

---

## 一、页面文档列表

### 1.1 认证与主界面

| 文档 | 页面代码 | 说明 |
|------|----------|------|
| CardCenterLogin.md | CardCenterLogin | 登录页面，管理员账号密码登录 |
| CardCenterHome.md | CardCenterHome | 主界面，功能菜单入口 |

### 1.2 卡片管理

| 文档 | 页面代码 | 说明 |
|------|----------|------|
| ResetValidityPage.md | ResetValidityPage | 重置有效期，修改卡片有效期 |
| ResetCardTypePage.md | ResetCardTypePage | 重置卡类，修改卡片类型 |
| RefundPage.md | RefundPage | 充值退款，卡片余额充值和退款 |
| ReturnCardPage.md | ReturnCardPage | 退卡，卡片退卡处理 |
| ReadCardPage.md | ReadCardPage | 读卡，读取并显示卡片完整信息 |

### 1.3 系统管理

| 文档 | 页面代码 | 说明 |
|------|----------|------|
| OperationLogPage.md | OperationLogPage | 操作日志，查看操作记录 |
| DataQueryPage.md | DataQueryPage | 数据查询，卡片和交易查询 |

---

## 二、文档阅读指南

### 2.1 文档结构

每个页面设计文档包含以下章节：

1. **设计图引用**
   - 设计图源文件路径
   - 切图资源路径

2. **页面布局结构**
   - ASCII 布局图
   - 不同状态展示

3. **UI 元素详解**
   - 顶部栏
   - 内容区
   - 底部按钮

4. **功能描述**
   - 页面用途
   - 核心功能
   - 操作流程

5. **交互描述**
   - 状态转换
   - 详细交互流程
   - 错误处理

6. **设计规范**
   - 颜色规范
   - 字体规范
   - 间距规范

7. **实现注意事项**
   - 数据模型
   - ViewModel 实现

8. **相关文档**
   - 关联文档链接

### 2.2 通用规范

| 规范项 | 值 |
|--------|-----|
| 主色调 | 红色系 #C42828, #d9230b |
| 辅助色 | 金黄色 #e6b84d, #ffe3a3 |
| 内容区背景 | 米黄色 #faf9f3 |
| 中文字体 | 黑体 (SimHei) |
| 英文字体 | Arial |
| 卡片圆角 | 7px |
| 按钮圆角 | 8px |

---

## 三、页面关系图

```
┌─────────────────────────────────────┐
│         CardCenterLogin             │
│           (登录页面)                │
└───────────────┬─────────────────────┘
                │ 登录成功
                ▼
┌─────────────────────────────────────┐
│         CardCenterHome              │
│           (主界面)                  │
└───────┬───────────┬───────────┬─────┘
        │           │           │
        ▼           ▼           ▼
┌──────────┐ ┌──────────┐ ┌──────────┐
│Reset     │ │Refund    │ │ReturnCard│
│Validity  │ │Page      │ │Page      │
│Page      │ │(充值退款) │ │(退卡)    │
│(重置有效期)│ └─────┬────┘ └─────┬────┘
└──────────┘       │            │
┌──────────┐       │            │
│Reset     │       │            │
│CardType  │       │            │
│Page      │       │            │
│(重置卡类) │       │            │
└──────────┘       │            │
                   │            │
        ┌──────────┴────────────┴─────┐
        │                             │
        ▼                             ▼
┌──────────────┐            ┌──────────────┐
│   ReadCard   │            │OperationLog  │
│    (读卡)    │            │   (操作日志)  │
└──────────────┘            └──────────────┘
```

---

## 四、权限矩阵

| 页面 | 超级管理员 | 卡务管理员 | 财务人员 | 操作员 |
|------|----------|----------|----------|--------|
| CardCenterLogin | ✓ | ✓ | ✓ | ✓ |
| CardCenterHome | ✓ | ✓ | ✓ | ✓ |
| ResetValidityPage | ✓ | ✓ | ✗ | ✗ |
| ResetCardTypePage | ✓ | ✓ | ✗ | ✗ |
| RefundPage | ✓ | ✗ | ✓ | ✗ |
| ReturnCardPage | ✓ | ✗ | ✓ | ✗ |
| ReadCardPage | ✓ | ✓ | ✓ | ✓ |
| OperationLogPage | ✓ | ✓ | ✓ | ✓ |

---

## 五、快速参考

### 5.1 按功能查找

| 功能 | 相关页面 |
|------|----------|
| 卡片有效期管理 | CardCenterHome → ResetValidityPage |
| 卡片类型管理 | CardCenterHome → ResetCardTypePage |
| 充值退款 | CardCenterHome → RefundPage |
| 退卡处理 | CardCenterHome → ReturnCardPage |
| 读卡查询 | CardCenterHome → ReadCardPage |
| 日志查询 | CardCenterHome → OperationLogPage |

---

## 六、相关文档

- [卡务中心设计规格说明书](../设计规格说明书.md)
- [自助服务设计](../../SelfService/)

---

## 七、文档更新记录

| 版本 | 日期 | 更新内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 创建卡务中心页面设计文档目录 | OpenCode Agent |

---

*文档结束*

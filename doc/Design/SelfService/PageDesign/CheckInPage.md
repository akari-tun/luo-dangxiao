# 报到页面设计文档 (CheckInPage)

> **页面代码**: `CheckInPage`  
> **适用用户**: 学员  
> **验证要求**: 已通过身份验证  
> **文档版本**: 2.0  
> **最后更新**: 2026-04-27

---

## 一、页面定位

`CheckInPage` 是"自助报到"功能专用页面，基于 `UserInfoPage` 的视觉结构实现。

- 内容区复用 `StudentInfoPage` 展示学员信息。
- 左下角固定显示"返回"按钮。
- 右下角 `报到` 按钮按报到状态动态显示。
- 右上角显示倒计时，倒计时结束后自动返回主页。

---

## 二、页面结构

1. **背景层**：使用 `bg.png`。
2. **内容容器**：沿用 `UserInfoPage` 的样式（圆角容器 + 标题区 + 内容区）。
3. **标题栏**：
   - 左侧：图标 + 标题文字（按报到状态动态变化）
   - 右侧：倒计时显示（进入页面后启动，格式：`{N}秒`，颜色 `#ff6000`）
4. **底部按钮区**：
   - 左侧：`返回`
   - 右侧：`报到`（仅未报到时显示）

---

## 三、状态规则

### 3.1 未报到状态

- 条件：`StudentInfoModel.CheckInStatus == NotCheckedIn`
- 标题显示：`确认信息后，点击[报到]按钮进行报到。`
- 右下角按钮：显示 `报到`
- 报到按钮点击后调用 `YktApi.RegisterTraineeAsync` 接口，传入：
  ```json
  {
    "userId": "<从查询接口返回的UserId>",
    "checkinState": 0,
    "roomCode": "<从查询接口返回的RoomCode，可能为空>",
    "deptId": "<从查询接口返回的DeptId>"
  }
  ```

### 3.2 已报到状态

- 条件：`StudentInfoModel.CheckInStatus == CheckedIn`
- 标题显示：`您已成功报到。`
- 右下角按钮：隐藏

---

## 四、倒计时逻辑

### 4.1 倒计时启动

- 进入页面后从 `config.json` 的 `CountdownSeconds` 字段读取倒计时秒数（默认 60 秒）。
- 倒计时显示在页面右上角，格式为 `{N}秒`。

### 4.2 倒计时结束行为

- **未在执行报到操作时**：倒计时结束后自动调用 `Back()` 返回主页。
- **正在执行报到操作时**：倒计时继续但不触发返回，等待操作完成后重新开始倒计时。

### 4.3 页面销毁

- 页面从可视树分离时（`DetachedFromVisualTree`），自动停止倒计时并释放资源。

---

## 五、交互流程

```text
HomePage 点击"自助报到"
    ↓
进入 VerifyPage
    ↓
验证成功
    ↓
进入 CheckInPage
    ↓
启动倒计时（config.json.CountdownSeconds）
    ↓
根据 CheckInStatus 显示状态
    ├─ 未报到：可点击"报到"
    │   ↓
    │   调用 YktApi.RegisterTraineeAsync
    │   ↓
    │   成功后更新 CheckInStatus = CheckedIn
    │   ↓
    │   重新开始倒计时
    └─ 已报到：仅展示成功提示
    ↓
倒计时归零且无操作进行中
    ↓
自动返回 HomePage
```

---

## 六、数据模型

### 6.1 StudentInfoModel 新增字段

| 字段 | 类型 | 说明 | API 映射 |
|------|------|------|----------|
| `RoomCode` | string | 房间编码（用于报到注册） | `roomCode` |
| `DeptId` | string | 培训班 ID（用于报到注册） | `deptId` |

### 6.2 报到接口参数

| 参数 | 来源 | 说明 |
|------|------|------|
| `userId` | `StudentInfo.UserId` | 学员唯一标识 |
| `checkinState` | 固定值 `0` | 报到状态（0=未报到） |
| `roomCode` | `StudentInfo.RoomCode` | 房间编码，可能为空 |
| `deptId` | `StudentInfo.DeptId` | 培训班 ID |

---

## 七、实现要点

- `CheckInPageViewModel` 负责：
  - 解析传入用户数据为 `StudentInfoModel`
  - 根据 `CheckInStatus` 计算 `PageTitle` 与 `ShowCheckInButton`
  - 管理倒计时：`DispatcherTimer` 每秒递减，归零后自动返回主页
  - 执行 `CheckInCommand` 时调用 `YktApiClient.RegisterTraineeAsync` 真实接口
  - 页面销毁时清理定时器资源
- `YktUserInfoMapper` 在 `BuildStudentIntermediateDto` 中从 API 响应读取 `roomCode` 和 `deptId` 字段。
- `VerifyPageViewModel` 在 `TargetFunction == "CheckIn"` 时，验证成功后跳转 `CheckInPage`。
- `HomePageViewModel` 通过 `NavigateToCheckIn` 承载页面跳转。

---

## 八、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本 | OpenCode Agent |
| 1.1 | 2026-04-16 | 按实现重写：标题文案状态化、报到按钮按状态显示、Verify 成功后跳转 CheckInPage | GitHub Copilot |
| 2.0 | 2026-04-27 | 新增：页面倒计时及自动返回主页、报到接口接入 YktApi.RegisterTraineeAsync、StudentInfoModel 新增 RoomCode/DeptId 字段 | Sisyphus |

---

*文档结束*

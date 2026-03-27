# 主界面设计文档 (HomePage)

> **页面代码**: HomePage  
> **适用用户**: 学员、教职工  
> **文档版本**: 2.0  
> **最后更新**: 2026-03-26

---

## 一、设计图引用

### 1.1 设计图源文件

| 文件名称 | 文件路径 | 说明 |
|----------|----------|------|
| home_page_1.jpg | `/doc/Design/SelfService/自助机/home_page_1.jpg` | 学员自助主界面 |
| home_page_2.jpg | `/doc/Design/SelfService/自助机/home_page_2.jpg` | 教职工自助主界面 |
| background.png | `/doc/Design/SelfService/自助机/background.png` | 通用背景图 |

### 1.2 切图资源

**位置**: `/doc/Design/SelfService/自助机裁图/home_page/`

| 文件名 | 规格 | 用途 |
|--------|------|------|
| background.png | 960×1024 | 首页背景图 (可更换) |
| logo.png | - | 党徽 Logo (PNG) |
| logo2.ico | - | 党徽 Logo (ICO) |
| photo.png | - | 主内容区背景照片 |
| home_page_layout.png | - | 首页布局参考图 |

### 1.3 功能图标资源

| 功能 | 32px | 64px | 128px |
|------|------|------|-------|
| 自助报到 (check_in) | ✓ | ✓ | ✓ |
| 自助取卡 (take_card) | ✓ | ✓ | ✓ |
| 自助挂失 (lost_card) | ✓ | ✓ | ✓ |
| 自助补卡 (replacement) | ✓ | ✓ | ✓ |
| 自助查询 (query) | 待补充 | 待补充 | 待补充 |
| 自助充值 (recharge) | 待补充 | 待补充 | 待补充 |

---

## 二、页面架构

### 2.1 页面角色

HomePage 是整个自助服务系统的主容器页面，具有以下角色：

1. **主菜单容器** - 显示功能按钮菜单（教职工自助/学员自助）
2. **子页面容器** - 承载所有功能子页面
3. **全局 UI 容器** - 提供顶部栏和底部栏

### 2.2 三种状态

HomePage 具有三种显示状态：

#### 状态 1: 教职工自助状态 (StaffSelfService)

```
┌─────────────────────────────────────────────────────────────┐
│  HEADER (顶部栏)                                            │
│  [Logo]  自助服务系统  [倒计时]  SELF SERVICE SYSTEM       │
├──────────────────────────────┬──────────────────────────────┤
│                              │  ┌──────────────────────┐   │
│                              │  │   自助取卡           │   │
│     主内容区                  │  ├──────────────────────┤   │
│     (校园风景背景图)           │  │   自助挂失           │   │
│                              │  ├──────────────────────┤   │
│         [中央大 Logo]          │  │   自助补卡           │   │
│                              │  ├──────────────────────┤   │
│                              │  │   自助查询           │   │
│                              │  ├──────────────────────┤   │
│                              │  │   自助充值           │   │
│                              │  └──────────────────────┘   │
└──────────────────────────────┴──────────────────────────────┘
```

#### 状态 2: 学员自助状态 (StudentSelfService)

```
┌─────────────────────────────────────────────────────────────┐
│  HEADER (顶部栏)                                            │
│  [Logo]  自助服务系统  [倒计时]  SELF SERVICE SYSTEM       │
├──────────────────────────────┬──────────────────────────────┤
│                              │  ┌──────────────────────┐   │
│                              │  │   自助报到           │   │
│     主内容区                  │  ├──────────────────────┤   │
│     (校园风景背景图)           │  │   自助取卡           │   │
│                              │  ├──────────────────────┤   │
│         [中央大 Logo]          │  │   自助挂失           │   │
│                              │  ├──────────────────────┤   │
│                              │  │   自助补卡           │   │
│                              │  └──────────────────────┘   │
└──────────────────────────────┴──────────────────────────────┘
```

#### 状态 3: 子页面容器状态 (SubPageContainer)

```
┌─────────────────────────────────────────────────────────────┐
│  HEADER (顶部栏)                                            │
│  [Logo]  自助服务系统  [倒计时]  SELF SERVICE SYSTEM       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                                                     │   │
│  │              子页面容器区域                          │   │
│  │                                                     │   │
│  │     [加载的功能子页面显示在此区域]                    │   │
│  │                                                     │   │
│  │     - VerifyPage                                    │   │
│  │     - TakeCardPage                                  │   │
│  │     - ReportLossPage                                │   │
│  │     - ...                                           │   │
│  │                                                     │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│  [返回按钮]                                    [其他按钮]   │
└─────────────────────────────────────────────────────────────┘
```

### 2.3 状态转换

```
┌──────────────────────┐
│   初始配置状态        │
│  (系统参数配置)       │
└──────────┬───────────┘
           │
     ┌─────┴─────┐
     │           │
     ▼           ▼
┌─────────┐  ┌─────────┐
│ 教职工  │  │ 学员    │
│ 自助    │  │ 自助    │
│ 状态    │  │ 状态    │
└────┬────┘  └────┬────┘
     │            │
     └─────┬──────┘
           │ 点击功能按钮
           ▼
┌──────────────────────┐
│   子页面容器状态      │
│  (加载功能子页面)     │
└──────────┬───────────┘
           │ 操作完成/返回
           ▼
     ┌─────┴─────┐
     │           │
     ▼           ▼
┌─────────┐  ┌─────────┐
│ 返回    │  │ 返回    │
│ 教职工  │  │ 学员    │
│ 状态    │  │ 状态    │
└─────────┘  └─────────┘
```

---

## 三、页面布局详解

### 3.1 固定区域（所有状态）

#### 3.1.1 顶部栏 (Header)

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 背景 | 顶部全宽 | 红色渐变 (#f9f0f0 → #d4afaf), 高度 95px | 顶部标题栏背景 |
| Logo | 左上角 | 距顶部 35px, 距左边 40px | 中共湖南省委党校/湖南行政学院党徽 |
| 主标题 | Logo 右侧 | 黑体 bold, #ffffff, 56px | "自助服务系统" |
| 副标题 | 主标题下方 | Arial, #ffffff, 24px | "SELF SERVICE SYSTEM" |
| **倒计时** | 标题右侧 | Arial bold 28px, #ff6000 | 子页面容器状态下显示 |

**说明**: 倒计时仅在子页面容器状态下显示，主菜单状态下隐藏。

#### 3.1.2 底部栏（子页面容器状态下显示）

| 元素 | 位置 | 说明 |
|------|------|------|
| 返回按钮 | 左下角 | 返回主菜单状态 |
| 操作按钮 | 右下角 | 根据子页面需要显示 |

### 3.2 主菜单状态布局

#### 3.2.1 主内容区

| 元素 | 位置 | 规格 | 说明 |
|------|------|------|------|
| 背景图 | 左侧大部分区域 | 校园建筑入口照片，距左边 45px | 可更换的背景图片 |
| 中央 Logo | 内容区中央 | 圆形徽章，直径约 200px | 大尺寸的党校 Logo |
| 背景内容 | 校园照片 | 含"实事求是"石碑和校园建筑 | 体现党校文化 |

#### 3.2.2 右侧功能按钮区

**按钮通用规格**:
| 属性 | 值 |
|------|-----|
| 尺寸 | 322×95px |
| 圆角 | 8px |
| 文字字体 | 黑体 40px |
| 文字颜色 | #ac0600 |
| 图标尺寸 | 64px |
| 图标位置 | 左侧，文字右侧 |
| 垂直间距 | 15px |

**学员自助按钮 (4个)**:
| 按钮 | 图标 | 点击后加载 |
|------|------|-----------|
| 自助报到 | 人像 + 勾选标记 | VerifyPage → CheckInPage |
| 自助取卡 | 卡片图标 | VerifyPage → TakeCardPage |
| 自助挂失 | 卡片 + 禁止符 | VerifyPage → ReportLossPage |
| 自助补卡 | 卡片 + 加号 | VerifyPage → ReplacementPage |

**教职工自助按钮 (5个)**:
| 按钮 | 图标 | 点击后加载 |
|------|------|-----------|
| 自助取卡 | 卡片图标 | VerifyPage → TakeCardPage |
| 自助挂失 | 卡片 + 禁止符 | VerifyPage → ReportLossPage |
| 自助补卡 | 卡片 + 加号 | VerifyPage → ReplacementPage |
| 自助查询 | 放大镜/文档 | VerifyPage → QueryPage |
| 自助充值 | ¥符号 + 加号 | VerifyPage → RechargePage |

### 3.3 子页面容器状态布局

#### 3.3.1 子页面容器区域

| 属性 | 值 |
|------|-----|
| 位置 | 顶部栏下方，底部栏上方 |
| 尺寸 | 占据大部分屏幕区域 |
| 背景 | #faf9f3 米黄色渐变 |
| 圆角 | 7px |
| 内边距 | 40px |

#### 3.3.2 子页面加载

**加载方式**:
- 功能子页面（如 TakeCardPage）作为内容加载到容器区域
- 子页面不拥有自己的顶部栏和底部栏，使用 HomePage 的
- 子页面内容自适应容器大小

**支持的子页面**:
- VerifyPage - 统一验证入口
- TakeCardPage - 取卡操作
- ReportLossPage - 挂失操作
- ReplacementPage - 补卡操作
- QueryPage - 查询操作
- RechargePage - 充值操作
- QRCodePage - 二维码展示
- CheckInPage - 报到操作
- CardProcessPage - 卡片处理过程
- CompletionPage - 完成展示

---

## 四、功能描述

### 4.1 页面用途

HomePage 是整个自助服务系统的核心容器页面：

1. **作为主菜单** - 根据配置显示教职工或学员功能入口
2. **作为子页面容器** - 承载所有功能子页面的显示
3. **提供全局 UI** - 顶部栏（含倒计时）和底部栏的统一管理

### 4.2 三种状态详解

#### 4.2.1 教职工自助状态

**显示内容**:
- 顶部栏（不含倒计时）
- 主内容区背景图 + Logo
- 5个功能按钮（取卡、挂失、补卡、查询、充值）

**行为**:
- 点击任一功能按钮
- 切换到子页面容器状态
- 加载 VerifyPage 到容器中

#### 4.2.2 学员自助状态

**显示内容**:
- 顶部栏（不含倒计时）
- 主内容区背景图 + Logo
- 4个功能按钮（报到、取卡、挂失、补卡）

**行为**:
- 点击任一功能按钮
- 切换到子页面容器状态
- 加载 VerifyPage 到容器中

#### 4.2.3 子页面容器状态

**显示内容**:
- 顶部栏（含倒计时）
- 子页面容器区域（加载功能子页面）
- 底部栏（返回按钮等）

**行为**:
- 加载指定的功能子页面
- 子页面使用容器区域显示内容
- 点击返回按钮回到主菜单状态

### 4.3 状态管理

```csharp
public enum HomePageState
{
    StaffSelfService,     // 教职工自助状态
    StudentSelfService,   // 学员自助状态
    SubPageContainer      // 子页面容器状态
}

public partial class HomeViewModel : ViewModelBase
{
    [ObservableProperty]
    private HomePageState _currentState;
    
    [ObservableProperty]
    private object _subPageContent;  // 子页面内容
    
    [ObservableProperty]
    private bool _isCountdownVisible;  // 倒计时可见性
    
    partial void OnCurrentStateChanged(HomePageState value)
    {
        switch (value)
        {
            case HomePageState.StaffSelfService:
            case HomePageState.StudentSelfService:
                IsCountdownVisible = false;
                SubPageContent = null;
                break;
            case HomePageState.SubPageContainer:
                IsCountdownVisible = true;
                break;
        }
    }
    
    // 加载子页面
    public void LoadSubPage(string pageName, object parameter = null)
    {
        CurrentState = HomePageState.SubPageContainer;
        SubPageContent = NavigationService.CreatePage(pageName, parameter);
    }
    
    // 返回主菜单
    public void ReturnToMenu()
    {
        // 根据用户类型返回对应的主菜单状态
        CurrentState = UserType == UserType.Staff 
            ? HomePageState.StaffSelfService 
            : HomePageState.StudentSelfService;
        SubPageContent = null;
    }
}
```

---

## 五、交互描述

### 5.1 页面加载

**1. 初始加载**:
- 读取系统参数确定用户模式（教职工/学员/自动）
- 自动模式下检测用户身份后确定显示哪种主菜单
- 显示对应的教职工或学员自助状态
- 倒计时隐藏

**2. 切换到子页面容器**:
- 用户点击功能按钮
- 触发状态切换命令
- CurrentState 变为 SubPageContainer
- 倒计时显示
- 加载 VerifyPage 到 SubPageContent

**3. 子页面切换**:
- 验证通过后，SubPageContent 切换到对应的功能子页面
- 顶部栏和底部栏保持不变
- 倒计时继续运行

**4. 返回主菜单**:
- 用户点击返回按钮或操作完成
- 触发 ReturnToMenu 命令
- SubPageContent 置空
- 恢复到对应的主菜单状态
- 倒计时隐藏

### 5.2 功能按钮点击

**交互流程**:
```
用户点击功能按钮
    ↓
记录选择的功能类型
    ↓
切换到子页面容器状态
    ↓
加载 VerifyPage
    ↓
验证通过
    ↓
加载对应的功能子页面（如 TakeCardPage）
    ↓
操作完成
    ↓
返回主菜单
```

### 5.3 倒计时管理

**显示时机**:
- 仅在子页面容器状态下显示
- 主菜单状态下隐藏

**行为**:
- 进入子页面容器状态时启动倒计时
- 倒计时在顶部栏右侧显示
- 倒计时归零自动返回主菜单

---

## 六、设计规范

### 6.1 颜色规范

| 用途 | 颜色值 |
|------|--------|
| 顶部背景 | 红色渐变 #f9f0f0 → #d4afaf |
| 标题文字 | #ffffff |
| 按钮文字 | #ac0600 |
| 按钮默认背景 | 白色渐变 |
| 按钮按下背景 | #ffe748 → #ffde00 |
| 倒计时 | #ff6000 |
| 子页面容器背景 | #faf9f3 |

### 6.2 字体规范

| 元素 | 字体 | 字号 | 字重 |
|------|------|------|------|
| 主标题 | 黑体 | 56px | bold |
| 副标题 | Arial | 24px | normal |
| 按钮文字 | 黑体 | 40px | bold |
| 倒计时 | Arial | 28px | bold |

### 6.3 间距规范

| 元素 | 间距 |
|------|------|
| Logo 距顶部 | 35px |
| Logo 距左边 | 40px |
| 主内容区距左边 | 45px |
| 按钮尺寸 | 322×95px |
| 按钮圆角 | 8px |
| 按钮间距 | 15px |
| 子页面容器内边距 | 40px |

---

## 七、实现注意事项

### 7.1 页面容器实现

```csharp
<!-- HomePage.axaml -->
<Grid>
    <!-- 顶部栏 - 始终显示 -->
    <Grid RowDefinitions="Auto,*">
        <!-- Header -->
        <Grid Grid.Row="0" Height="95">
            <Image Source="logo.png" />
            <TextBlock Text="自助服务系统" />
            <TextBlock Text="SELF SERVICE SYSTEM" />
            <TextBlock Text="{Binding CountdownSeconds}" 
                       IsVisible="{Binding IsCountdownVisible}" />
        </Grid>
        
        <!-- 内容区 -->
        <Grid Grid.Row="1">
            <!-- 主菜单状态 -->
            <Grid IsVisible="{Binding IsMenuVisible}">
                <!-- 背景图 -->
                <Image Source="background.png" />
                <!-- Logo -->
                <Image Source="logo_large.png" />
                <!-- 功能按钮 -->
                <StackPanel>
                    <Button Content="自助取卡" ... />
                    <Button Content="自助挂失" ... />
                    <!-- ... -->
                </StackPanel>
            </Grid>
            
            <!-- 子页面容器状态 -->
            <ContentControl Content="{Binding SubPageContent}" 
                          IsVisible="{Binding IsSubPageVisible}" />
        </Grid>
    </Grid>
    
    <!-- 底部栏 - 子页面容器状态下显示 -->
    <Grid IsVisible="{Binding IsBottomBarVisible}">
        <Button Content="返回" Command="{Binding ReturnCommand}" />
    </Grid>
</Grid>
```

### 7.2 状态切换动画

建议添加状态切换动画：
- 主菜单 → 子页面容器：滑动过渡
- 子页面容器 → 主菜单：滑动返回
- 使用 Avalonia 的动画系统实现

### 7.3 配置管理

```csharp
public class HomePageConfig
{
    public UserMode DefaultUserMode { get; set; }
    public bool AutoDetectUserType { get; set; }
    public int CountdownSeconds { get; set; } = 120;
}
```

---

## 八、相关文档

- [设计规格说明书](../../设计规格说明书.md)
- [验证页面](../VerifyPage.md)
- [学员信息模块](../StudentInfoPage.md)
- [教职工信息模块](../StaffInfoPage.md)

---

## 九、版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2026-03-26 | 初始版本，描述基本布局和功能 | OpenCode Agent |
| 2.0 | 2026-03-26 | 更新架构设计：<br>• 明确三种状态（教职工/学员/容器）<br>• 描述页面容器机制<br>• 添加状态转换图<br>• 更新交互流程 | OpenCode Agent |

---

*文档结束*

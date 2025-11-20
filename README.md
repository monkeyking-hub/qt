# Flight Management System

基于Qt Creator开发的航班管理系统前端，使用qmake构建系统。

## 项目概述

这是一个功能完整的航班管理系统前端应用程序，提供了航班查询、预订、用户管理和航班详情等核心功能。系统采用现代化的UI设计，支持明暗主题切换，具有友好的用户体验。

## 主要功能

### 1. 航班查询 (Flight Search)
- 按出发地和目的地搜索航班
- 支持单程、往返、多程航班类型
- 日期选择和乘客人数设置
- 舱位等级选择（经济舱、商务舱、头等舱）
- 实时搜索结果显示
- 航班状态指示（准点、延误、取消）

### 2. 航班预订 (Flight Booking)
- 航班选择和详情查看
- 乘客信息录入
- 多种支付方式支持
- 附加服务选择（保险、餐食、座位）
- 价格自动计算
- 预订确认和摘要

### 3. 用户管理 (User Management)
- 用户信息增删改查
- 用户角色管理（普通用户、管理员、VIP用户、客服）
- 用户状态管理（活跃、暂停、禁用）
- 高级搜索功能
- 用户数据导出

### 4. 航班详情 (Flight Details)
- 航班详细信息展示
- 航线信息可视化
- 乘客信息管理
- 实时状态更新
- 航班统计数据
- 地图视图支持

## 技术特性

### 核心技术栈
- **Qt Framework**: 5.15+ / 6.x
- **构建系统**: qmake
- **编程语言**: C++17
- **数据库**: SQLite (本地存储)
- **网络**: Qt Network (API集成)

### UI特性
- 现代化扁平设计
- 明暗主题切换
- 响应式布局
- 自定义组件库
- 流畅的动画效果
- 系统托盘集成

### 架构设计
- 模块化组件设计
- MVC架构模式
- 信号槽通信机制
- API管理器封装
- 数据库抽象层

## 项目结构

```
FlightSystem/
├── FlightSystem.pro          # qmake项目文件
├── main.cpp                  # 程序入口
├── mainwindow.h/cpp          # 主窗口
├── mainwindow.ui             # 主窗口UI文件
├── flightsearchwidget.h/cpp   # 航班查询组件
├── flightbookingwidget.h/cpp  # 航班预订组件
├── usermanagementwidget.h/cpp # 用户管理组件
├── flightdetailswidget.h/cpp # 航班详情组件
├── apimanager.h/cpp          # API管理器
├── databasehelper.h/cpp      # 数据库助手
├── customwidgets.h/cpp       # 自定义组件
├── resources.qrc             # 资源文件
├── styles/                   # 样式文件
│   ├── darkstyle.qss         # 暗色主题
│   └── lightstyle.qss        # 亮色主题
├── icons/                    # 图标资源
├── docs/                     # 文档目录
│   ├── API.md                # API文档
│   ├── USER_GUIDE.md         # 用户指南
│   └── DEVELOPMENT.md        # 开发文档
└── README.md                 # 项目说明
```

## 安装和运行

### 系统要求
- Qt 5.15+ 或 Qt 6.x
- C++17 兼容的编译器
- Windows 10+, macOS 10.14+, 或 Linux (Ubuntu 18.04+)

### 编译和运行

#### Windows
```bash
# 1. 打开Qt Creator
# 2. 打开FlightSystem.pro项目
# 3. 选择构建套件
# 4. 构建并运行
```

#### macOS/Linux
```bash
# 1. 进入项目目录
cd FlightSystem

# 2. 生成Makefile
qmake FlightSystem.pro

# 3. 编译
make

# 4. 运行
./FlightSystem
```

### 依赖项
- Qt Widgets
- Qt SQL
- Qt Network
- Qt Core
- Qt GUI

## API文档

### FlightSearch API
```cpp
// 搜索航班
void searchFlights(const QString &departure, const QString &destination, const QDate &date);

// 获取航班详情
void getFlightDetails(const QString &flightNumber);

// 信号
void flightSearchCompleted(const QJsonArray &flights);
void flightDetailsReceived(const QJsonObject &details);
```

### UserManagement API
```cpp
// 用户登录
void loginUser(const QString &username, const QString &password);

// 用户注册
void registerUser(const QJsonObject &userData);

// 获取用户资料
void getUserProfile(const QString &userId);
```

### Booking API
```cpp
// 预订航班
void bookFlight(const QJsonObject &bookingData);

// 获取预订详情
void getBookingDetails(const QString &bookingId);
```

详细API文档请参考 [docs/API.md](docs/API.md)

## 使用指南

### 基本操作流程

1. **启动应用**
   - 双击应用程序图标或从命令行启动
   - 系统将显示主界面，默认打开航班查询页面

2. **航班查询**
   - 输入出发地和目的地
   - 选择出发日期和航班类型
   - 点击"搜索航班"按钮
   - 在结果列表中选择航班

3. **航班预订**
   - 切换到航班预订页面
   - 选择航班并填写乘客信息
   - 选择支付方式和附加服务
   - 确认预订信息并提交

4. **用户管理**
   - 切换到用户管理页面
   - 添加、编辑或删除用户信息
   - 使用搜索功能查找特定用户

5. **查看航班详情**
   - 切换到航班详情页面
   - 搜索或选择航班查看详细信息
   - 查看航线、乘客和状态信息

### 快捷键
- `Ctrl+S`: 航班查询
- `Ctrl+B`: 航班预订
- `Ctrl+U`: 用户管理
- `Ctrl+D`: 航班详情
- `Ctrl+Q`: 退出应用

## 开发指南

### 添加新功能
1. 在相应的组件类中添加功能
2. 更新UI界面（如需要）
3. 实现信号槽连接
4. 添加相应的API调用
5. 更新数据库结构（如需要）

### 自定义组件
项目提供了多个自定义组件：
- `ModernButton`: 现代化按钮
- `StatusCard`: 状态卡片
- `AnimatedProgressBar`: 动画进度条
- `FlightStatusIndicator`: 航班状态指示器
- `LoadingWidget`: 加载动画
- `ModernSearchBox`: 现代搜索框
- `FlightInfoCard`: 航班信息卡片

### 主题定制
- 修改 `styles/darkstyle.qss` 和 `styles/lightstyle.qss`
- 添加新的颜色变量和样式规则
- 使用 `applyTheme()` 方法应用主题

## 常见问题

### Q: 如何添加新的航空公司？
A: 在数据库的flights表中添加新的airline记录，或在代码中更新航班数据。

### Q: 如何修改主题颜色？
A: 编辑样式文件中的颜色值，或使用主题管理器进行动态修改。

### Q: 如何集成真实的后端API？
A: 修改 `APIManager` 类中的 `baseApiUrl` 和相关的API端点。

### Q: 数据库文件在哪里？
A: 默认在应用程序目录下的 `flightsystem.db` 文件中。

## 贡献指南

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开 Pull Request

## 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 联系方式

- 项目维护者: Flight Systems Inc.
- 邮箱: support@flightsystems.com
- 网站: https://www.flightsystems.com

## 更新日志

### v1.0.0 (2024-01-01)
- 初始版本发布
- 实现基本航班查询功能
- 添加用户管理模块
- 实现航班预订功能
- 添加航班详情展示
- 支持明暗主题切换
- 完善API和数据库集成

---

感谢使用 Flight Management System！如有任何问题或建议，请随时联系我们。
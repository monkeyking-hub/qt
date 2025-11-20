# 航班管理系统 - 完整项目文档

## 📋 项目概述

航班管理系统是一个功能完整、设计优雅的Qt桌面应用程序，提供了全面的航班搜索、预订、用户管理和客户服务功能。

### 核心特性
- 🔍 **高级搜索**: 强大的搜索过滤和排序功能
- 🎫 **完整预订**: 从选择到支付的完整预订流程
- 👤 **用户管理**: 个人资料、预订历史、收藏管理
- ⭐ **评价系统**: 用户评价和评分展示
- 🔐 **安全认证**: JWT Token认证机制
- 🎨 **美观UI**: 现代化深浅两种主题
- 📱 **响应式**: 自适应不同屏幕尺寸
- 🔔 **通知系统**: 实时通知提醒

---

## 🚀 快速开始

### 系统要求
- **操作系统**: Windows 7+, macOS 10.12+, Linux (Ubuntu 18.04+)
- **Qt版本**: Qt 5.15+ 或 Qt 6.x
- **编译器**: GCC 5.0+, MSVC 2015+, Clang 3.5+
- **内存**: 最少 512MB
- **磁盘**: 最少 100MB

### 安装和编译

**1. 克隆或下载项目**
```bash
git clone <repository-url>
cd FlightSystem
```

**2. 配置Qt项目**
```bash
qmake FlightSystem.pro
```

**3. 编译项目**
```bash
make
# 或在Windows上
nmake
```

**4. 运行应用**
```bash
./FlightSystem
# 或在Windows上
FlightSystem.exe
```

### 开发环境设置

**使用Qt Creator**
1. 打开 Qt Creator
2. 点击 File → Open File or Project
3. 选择 FlightSystem.pro
4. 配置Kit (选择合适的Qt版本和编译器)
5. 点击 Build → Build Project

---

## 📁 项目结构

```
FlightSystem/
├── docs/
│   ├── API_COMPLETE.md          # 完整API文档
│   ├── UI_DESIGN.md             # UI设计规范
│   ├── FEATURES.md              # 功能详细文档
│   ├── API.md                   # API快速参考
│   ├── DEVELOPMENT.md           # 开发指南
│   └── USER_GUIDE.md            # 用户指南
│
├── src/
│   ├── main.cpp                 # 应用入口
│   ├── mainwindow.h/cpp         # 主窗口
│   ├── authwidget.h/cpp         # 认证模块 ✨ NEW
│   ├── flightsearchwidget.h/cpp # 航班搜索
│   ├── flightbookingwidget.h/cpp # 航班预订
│   ├── flightdetailswidget.h/cpp # 航班详情
│   ├── usermanagementwidget.h/cpp # 用户管理
│   ├── apimanager.h/cpp         # API管理器 (已增强)
│   ├── databasehelper.h/cpp     # 数据库助手
│   └── customwidgets.h/cpp      # 自定义组件
│
├── styles/                      # 样式表
├── icons/                       # 图标资源
├── resources.qrc                # 资源文件
├── FlightSystem.pro             # qmake项目文件
├── README.md                    # 项目说明
├── LICENSE                      # 许可证
└── .gitignore                   # Git忽略规则
```

---

## 🔧 主要模块说明

### 1. 主窗口 (MainWindow)
- 应用主容器
- 菜单栏、工具栏、状态栏管理
- 页面切换控制
- 主题切换功能
- 系统托盘集成

### 2. 认证模块 (AuthWidget) ✨ NEW
- 用户登录
- 用户注册
- 邮箱和用户名验证
- 密码强度检查
- 会话管理

### 3. 航班搜索 (FlightSearchWidget)
- 多条件搜索表单
- 结果列表展示
- 排序和筛选
- 收藏功能
- 搜索历史

### 4. 航班预订 (FlightBookingWidget)
- 4步预订流程
- 乘客信息填写
- 座位选择
- 增值服务选择
- 价格计算

### 5. 用户管理 (UserManagementWidget)
- 个人资料编辑
- 预订历史查看
- 收藏管理
- 后台用户管理 (管理员)

### 6. 航班详情 (FlightDetailsWidget)
- 详细航班信息
- 座位地图展示
- 用户评价展示
- 便利设施信息
- 行李政策显示

### 7. API管理器 (APIManager) ✨ ENHANCED
- RESTful API调用
- 请求/响应处理
- 认证管理
- 错误处理
- 请求超时控制
- 自动重试机制

---

## 📡 API集成

### 基础URL配置
```cpp
APIManager manager;
manager.setBaseUrl("https://api.flightsystem.com/v1");
```

### 主要API端点

| 功能 | 方法 | 端点 |
|------|------|------|
| 登录 | POST | /auth/login |
| 注册 | POST | /auth/register |
| 搜索航班 | POST | /flights/search |
| 航班详情 | GET | /flights/{flightNumber} |
| 创建预订 | POST | /bookings |
| 用户资料 | GET | /users/{userId} |
| 预订历史 | GET | /users/{userId}/bookings |
| 提交评价 | POST | /bookings/{bookingId}/review |

详见 [完整API文档](./API_COMPLETE.md)

---

## 🎨 UI设计特点

### 主题系统
- **深色主题** (默认): 减少眼睛疲劳，夜间使用舒适
- **浅色主题**: 明亮清爽，适合白天使用

### 设计规范
- 统一色彩方案
- 清晰的视觉层级
- 响应式布局
- 无障碍设计
- 现代化组件

### 自定义组件
- `ModernButton`: 现代按钮
- `StatusCard`: 状态卡片
- `FlightInfoCard`: 航班卡片
- `LoadingWidget`: 加载动画
- `AnimatedProgressBar`: 动画进度条

详见 [UI设计文档](./UI_DESIGN.md)

---

## 🔐 安全性

### 认证
- JWT Token认证
- Token自动刷新
- 安全的会话管理

### 数据传输
- HTTPS加密
- SSL/TLS支持
- 请求签名验证

### 数据验证
- 输入字段验证
- 邮箱格式验证
- 电话号码验证
- 身份证号验证

### 错误处理
- 异常捕获
- 错误日志记录
- 用户友好的错误提示

---

## 💡 代码示例

### 航班搜索示例
```cpp
#include "apimanager.h"
#include <QJsonObject>

APIManager apiManager;

// 搜索航班
apiManager.searchFlights("PEK", "SHA", QDate(2024, 2, 15),
                         "", 2, "economy");

// 连接信号
connect(&apiManager, &APIManager::flightSearchCompleted,
        this, [](const QJsonArray &flights) {
    for (const auto &flight : flights) {
        QJsonObject obj = flight.toObject();
        qDebug() << "Flight:" << obj["flight_number"];
        qDebug() << "Price:" << obj["price"];
    }
});
```

### 用户登录示例
```cpp
APIManager apiManager;

// 登录
apiManager.loginUser("user@example.com", "password123");

// 处理登录结果
connect(&apiManager, &APIManager::loginCompleted,
        this, [&apiManager](const QJsonObject &result) {
    if (result["code"].toInt() == 0) {
        QString token = result["data"].toObject()["token"].toString();
        apiManager.setAuthToken(token);
        qDebug() << "Login successful!";
    }
});
```

### 创建预订示例
```cpp
QJsonObject bookingData;
bookingData["flight_number"] = "CA123";
bookingData["search_id"] = "search_123456";

QJsonArray passengers;
QJsonObject passenger;
passenger["first_name"] = "John";
passenger["last_name"] = "Doe";
passenger["gender"] = "male";
passenger["date_of_birth"] = "1990-01-01";
passengers.append(passenger);

bookingData["passengers"] = passengers;
bookingData["seats"] = QJsonArray({"1A", "1B"});

apiManager.bookFlight(bookingData);
```

---

## 🧪 测试

### 单元测试
```bash
cd tests
qmake
make
./test_main
```

### 集成测试
- 使用模拟后端服务
- 测试UI交互
- 测试API调用

### 性能测试
- 大数据集搜索 (1000+ 航班)
- 响应时间测试
- 内存使用分析

---

## 📊 功能清单

### 已实现 ✅
- [x] 用户认证 (登录/注册)
- [x] 航班搜索
- [x] 航班详情展示
- [x] 航班预订
- [x] 用户管理
- [x] 收藏系统
- [x] 评价系统
- [x] API集成
- [x] 主题切换
- [x] 快捷键支持
- [x] 系统托盘

### 计划中 🔮
- [ ] 离线模式
- [ ] 推送通知
- [ ] 多语言支持 (完整)
- [ ] 移动应用版本
- [ ] 机票二维码
- [ ] 虚拟助手
- [ ] AI推荐系统

---

## 🐛 常见问题

### Q: 如何修改API地址?
A: 在 `APIManager` 初始化时使用 `setBaseUrl()` 方法:
```cpp
apiManager.setBaseUrl("https://your-api.com/v1");
```

### Q: 如何切换主题?
A: 使用菜单栏的"工具" → "切换主题"或直接在代码中:
```cpp
isDarkTheme = !isDarkTheme;
applyTheme();
```

### Q: 如何添加新的搜索条件?
A: 编辑 `FlightSearchWidget::setupSearchForm()` 方法并在API请求中添加参数

### Q: Token过期怎么处理?
A: `APIManager` 会自动使用 `refresh_token` 刷新，无需手动处理

### Q: 如何自定义UI样式?
A: 修改 `mainwindow.cpp` 中的 `applyTheme()` 方法中的样式表

---

## 📖 进一步学习

### 相关文档
- [完整功能文档](./FEATURES.md) - 详细功能说明
- [API文档](./API_COMPLETE.md) - API端点和调用方式
- [UI设计文档](./UI_DESIGN.md) - 设计规范和组件说明
- [开发指南](./DEVELOPMENT.md) - 开发最佳实践
- [用户指南](./USER_GUIDE.md) - 用户操作说明

### 参考资源
- [Qt官方文档](https://doc.qt.io/)
- [QML Book](https://qmlbook.github.io/)
- [C++ 参考](https://cppreference.com/)

---

## 🤝 贡献指南

欢迎提交issue、pull request和建议！

### 开发流程
1. Fork项目
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add some amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 开启Pull Request

### 代码规范
- 遵循Qt命名规范
- 使用PascalCase类名
- 使用camelCase函数名
- 添加必要的注释
- 运行代码格式化工具

---

## 📝 许可证

本项目采用 MIT 许可证。详见 [LICENSE](../LICENSE) 文件。

---

## 👥 关于

**开发者**: Flight Systems Team  
**版本**: 1.0.0  
**发布日期**: 2024-01-20  
**状态**: 生产就绪

---

## 📞 联系方式

- **问题报告**: 提交GitHub Issue
- **功能建议**: 提交GitHub Discussion
- **邮件**: support@flightsystem.com
- **官网**: https://www.flightsystem.com

---

## 🙏 鸣谢

感谢所有贡献者和用户的支持！

---

**最后更新**: 2024-01-20  
**文档版本**: 1.0.0


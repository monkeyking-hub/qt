# 🚀 航班管理系统 - 快速开始指南

## 5分钟快速上手

### 1️⃣ 编译项目

```bash
# 进入项目目录
cd FlightSystem

# 配置qmake
qmake FlightSystem.pro

# 编译项目
make

# 运行应用
./FlightSystem
```

### 2️⃣ 启动Mock后端 API

选择一种方式启动后端服务:

**方式A: Node.js** (推荐)
```bash
# 安装依赖
npm install express cors body-parser jsonwebtoken

# 创建并运行服务器 (参考 docs/BACKEND_MOCK_API.md)
node server.js

# 访问: http://localhost:3000
```

**方式B: Python**
```bash
# 安装依赖
pip install flask flask-cors flask-jwt-extended

# 运行服务器
python app.py

# 访问: http://localhost:3000
```

### 3️⃣ 配置API地址

在代码中修改API地址:

```cpp
// 在 mainwindow.cpp 中
APIManager apiManager;
apiManager.setBaseUrl("http://localhost:3000/api/v1");  // 本地开发
// apiManager.setBaseUrl("https://api.flightsystem.com/v1");  // 生产环境
```

### 4️⃣ 测试账户

使用以下测试账户登录:

| 用户名 | 密码 | 说明 |
|-------|------|------|
| user@example.com | password123 | 普通用户 |
| admin@example.com | admin123 | 管理员 |
| test@example.com | test123 | 测试账户 |

### 5️⃣ 开始探索

1. **注册新账户** → 填写个人信息 → 验证邮箱
2. **搜索航班** → 选择出发地/目的地/日期 → 查看结果
3. **查看详情** → 选择航班 → 查看航班信息和评价
4. **预订航班** → 填写乘客信息 → 选择座位 → 选择增值服务 → 支付
5. **管理预订** → 查看我的预订 → 下载行程单

---

## 📁 项目文件说明

### 核心文件

```
FlightSystem/
├── main.cpp                    # 应用入口
├── mainwindow.h/cpp           # 主窗口
├── authwidget.h/cpp           # 登录/注册 ✨ NEW
├── flightsearchwidget.h/cpp   # 航班搜索
├── flightbookingwidget.h/cpp  # 航班预订
├── flightdetailswidget.h/cpp  # 航班详情
├── usermanagementwidget.h/cpp # 用户管理
├── apimanager.h/cpp           # API管理器 (已增强) ✨
├── customwidgets.h/cpp        # 自定义UI组件
├── FlightSystem.pro           # 项目配置
└── docs/                      # 完整文档
    ├── README_COMPLETE.md     # 完整说明
    ├── API_COMPLETE.md        # API文档
    ├── UI_DESIGN.md           # UI规范
    ├── FEATURES.md            # 功能列表
    └── BACKEND_MOCK_API.md    # 后端指南
```

---

## 🔧 常见操作

### 修改主题

**方式1: 通过菜单**
- 菜单栏 → 工具 → 切换主题

**方式2: 通过代码**
```cpp
// mainwindow.cpp
isDarkTheme = !isDarkTheme;  // 切换主题
applyTheme();                 // 应用主题
```

### 添加新的搜索条件

编辑 `flightsearchwidget.cpp`:

```cpp
void FlightSearchWidget::setupSearchForm()
{
    // ... 现有代码 ...
    
    // 添加新的搜索条件
    QLabel *newLabel = new QLabel("新条件:");
    QComboBox *newCombo = new QComboBox();
    newCombo->addItem("选项1");
    newCombo->addItem("选项2");
    searchLayout->addRow(newLabel, newCombo);
}
```

### 自定义API端点

编辑 `apimanager.h` 和 `apimanager.cpp`:

```cpp
// apimanager.h
void getCustomData(const QString &param1);
void customDataReceived(const QJsonObject &data);

// apimanager.cpp
void APIManager::getCustomData(const QString &param1)
{
    QJsonObject requestData;
    requestData["param1"] = param1;
    makeApiCall("/custom/endpoint", "POST", requestData);
}
```

### 修改样式和颜色

编辑 `mainwindow.cpp` 中的 `applyTheme()` 函数:

```cpp
void MainWindow::applyTheme()
{
    QString styleSheet = R"(
        QPushButton {
            background-color: #YOUR_COLOR;
            color: #WHITE;
            border-radius: 4px;
        }
    )";
    this->setStyleSheet(styleSheet);
}
```

---

## 🐛 常见问题

### Q: 应用无法编译?
A: 确保已安装Qt 5.15+或Qt 6.x，并配置了正确的Kit

### Q: API连接超时?
A: 检查Mock API服务是否在运行，地址是否正确

### Q: 登录失败?
A: 使用提供的测试账户，或在Mock API中添加新用户

### Q: UI显示异常?
A: 尝试切换主题，或检查样式表设置

### Q: 如何提交建议?
A: 查看 `COMPLETION_SUMMARY.md` 中的反馈方式

---

## 📚 推荐阅读

按照以下顺序阅读文档，快速了解项目:

1. **本文件** (QUICKSTART.md) - 快速开始 ✅ 当前
2. **docs/README_COMPLETE.md** - 项目完整说明
3. **docs/FEATURES.md** - 功能详细说明
4. **docs/API_COMPLETE.md** - API文档
5. **docs/UI_DESIGN.md** - UI设计规范

---

## 🎯 学习路径

### 初级 (理解基础)
- 运行应用并探索功能
- 阅读 UI 设计文档
- 测试所有功能模块

### 中级 (理解代码)
- 学习 AuthWidget 的实现
- 研究 APIManager 的工作流程
- 理解信号槽机制

### 高级 (扩展功能)
- 添加新的API端点
- 实现自定义UI组件
- 优化性能

---

## 💡 开发提示

### 调试
```cpp
// 启用调试输出
qDebug() << "Variable:" << variable;

// 在Release模式中使用输出
qWarning() << "Warning message";
qCritical() << "Error message";
```

### 性能分析
```cpp
// 测量代码运行时间
QTime timer;
timer.start();
// ... 代码 ...
qDebug() << "Elapsed:" << timer.elapsed() << "ms";
```

### 内存管理
```cpp
// 使用智能指针管理内存
QPointer<QWidget> widget = new QWidget();
// 不需要手动delete，自动管理
```

---

## 🚀 下一步

1. ✅ 编译并运行应用
2. ✅ 启动Mock API
3. ✅ 测试所有功能
4. ✅ 查看相关文档
5. ✅ 修改代码进行扩展
6. ✅ 连接真实后端API

---

## 📞 需要帮助?

查看以下资源:
- 📖 完整文档: `docs/` 目录
- 🔌 API参考: `docs/API_COMPLETE.md`
- 🎨 UI规范: `docs/UI_DESIGN.md`
- 💻 代码示例: 各个文件的注释

---

**准备好了吗? 开始你的航班管理系统之旅吧! 🛫**

祝开发愉快! 😊


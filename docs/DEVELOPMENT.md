# Flight Management System 开发文档

## 目录

1. [架构概述](#架构概述)
2. [开发环境搭建](#开发环境搭建)
3. [代码结构](#代码结构)
4. [组件开发](#组件开发)
5. [API集成](#api集成)
6. [数据库设计](#数据库设计)
7. [UI开发指南](#ui开发指南)
8. [测试指南](#测试指南)
9. [部署指南](#部署指南)
10. [性能优化](#性能优化)

## 架构概述

### 系统架构

Flight Management System 采用分层架构设计：

```
┌─────────────────────────────────────────────────────────────┐
│                    表示层 (UI Layer)                        │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────┐ │
│  │ 航班查询     │ │ 航班预订     │ │ 用户管理     │ │ 详情    │ │
│  │ Widget      │ │ Widget      │ │ Widget      │ │ Widget  │ │
│  └─────────────┘ └─────────────┘ └─────────────┘ └─────────┘ │
├─────────────────────────────────────────────────────────────┤
│                   业务逻辑层 (Business Layer)                │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐ │
│  │ 航班管理器   │ │ 预订管理器   │ │ 用户管理器              │ │
│  │ FlightMgr   │ │ BookingMgr  │ │ UserMgr                │ │
│  └─────────────┘ └─────────────┘ └─────────────────────────┘ │
├─────────────────────────────────────────────────────────────┤
│                    数据访问层 (Data Layer)                   │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐ │
│  │ API管理器    │ │ 数据库助手   │ │ 缓存管理器              │ │
│  │ APIManager  │ │ DBHelper    │ │ CacheManager           │ │
│  └─────────────┘ └─────────────┘ └─────────────────────────┘ │
├─────────────────────────────────────────────────────────────┤
│                     数据存储层 (Storage)                     │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐ │
│  │ 远程API     │ │ 本地数据库   │ │ 配置文件                │ │
│  │ REST API    │ │ SQLite      │ │ INI/JSON               │ │
│  └─────────────┘ └─────────────┘ └─────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### 设计模式

- **MVC模式**: 视图与业务逻辑分离
- **观察者模式**: 信号槽机制实现组件通信
- **单例模式**: 全局管理器类
- **工厂模式**: 自定义组件创建
- **策略模式**: 主题切换和数据处理

## 开发环境搭建

### 必需软件

1. **Qt Framework**
   - Qt 5.15+ 或 Qt 6.x
   - Qt Creator IDE
   - Qt Charts (可选)

2. **编译器**
   - Windows: MSVC 2019+ 或 MinGW
   - macOS: Xcode 11+
   - Linux: GCC 7+ 或 Clang 8+

3. **开发工具**
   - Git 版本控制
   - CMake (可选)
   - qmake

### 环境配置

#### Windows
```bash
# 1. 下载并安装 Qt
# 2. 设置环境变量
set QT_DIR=C:\Qt\5.15.2\msvc2019_64
set PATH=%QT_DIR%\bin;%PATH%

# 3. 编译项目
qmake FlightSystem.pro
nmake
```

#### macOS
```bash
# 1. 安装 Qt (使用 Homebrew)
brew install qt

# 2. 设置环境变量
export QT_DIR=/usr/local/opt/qt
export PATH=$QT_DIR/bin:$PATH

# 3. 编译项目
qmake FlightSystem.pro
make
```

#### Linux
```bash
# 1. 安装依赖
sudo apt-get install qt5-default qttools5-dev-tools

# 2. 编译项目
qmake FlightSystem.pro
make
```

### IDE配置

#### Qt Creator设置
1. 打开Qt Creator
2. 设置工具链 (Kits)
3. 配构构建套件
4. 导入项目文件

#### 代码格式化
```json
{
    "clang-format": {
        "BasedOnStyle": "Google",
        "IndentWidth": 4,
        "TabWidth": 4,
        "UseTab": "Never"
    }
}
```

## 代码结构

### 目录结构说明

```
FlightSystem/
├── src/                      # 源代码目录
│   ├── core/                 # 核心功能
│   │   ├── mainwindow.h/cpp
│   │   └── application.h/cpp
│   ├── widgets/              # UI组件
│   │   ├── flightsearchwidget.h/cpp
│   │   ├── flightbookingwidget.h/cpp
│   │   ├── usermanagementwidget.h/cpp
│   │   └── flightdetailswidget.h/cpp
│   ├── managers/             # 管理器类
│   │   ├── apimanager.h/cpp
│   │   └── databasehelper.h/cpp
│   ├── custom/               # 自定义组件
│   │   └── customwidgets.h/cpp
│   └── utils/                # 工具类
│       └── helpers.h/cpp
├── resources/                # 资源文件
│   ├── styles/               # 样式文件
│   ├── icons/                # 图标资源
│   └── images/               # 图片资源
├── ui/                       # UI文件
├── docs/                     # 文档目录
├── tests/                    # 测试代码
└── build/                    # 构建输出
```

### 命名规范

#### 类命名
```cpp
// 类名使用 PascalCase
class FlightSearchWidget;
class APIManager;
class DatabaseHelper;
```

#### 函数命名
```cpp
// 函数名使用 camelCase
void searchFlights();
void updateFlightStatus();
QString getFlightNumber();
```

#### 变量命名
```cpp
// 成员变量使用 m_ 前缀
QString m_flightNumber;
int m_passengerCount;

// 局部变量使用 camelCase
QString flightNumber;
int passengerCount;
```

#### 常量命名
```cpp
// 常量使用 UPPER_SNAKE_CASE
const int MAX_PASSENGERS = 9;
const QString API_BASE_URL = "https://api.flightsystem.com";
```

## 组件开发

### 自定义组件基类

```cpp
// BaseWidget.h
class BaseWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BaseWidget(QWidget *parent = nullptr);
    virtual ~BaseWidget();
    
protected:
    virtual void setupUI() = 0;
    virtual void connectSignals() = 0;
    virtual void applyStyles() = 0;
    
    void loadSettings();
    void saveSettings();
    
private:
    void initializeBase();
};
```

### 组件生命周期

```cpp
// FlightSearchWidget.cpp
FlightSearchWidget::FlightSearchWidget(QWidget *parent)
    : BaseWidget(parent)
    , isSearching(false)
{
    initializeBase();
}

void FlightSearchWidget::setupUI()
{
    // 创建UI组件
    mainLayout = new QVBoxLayout(this);
    // ... UI设置
}

void FlightSearchWidget::connectSignals()
{
    // 连接信号槽
    connect(searchButton, &QPushButton::clicked, 
            this, &FlightSearchWidget::searchFlights);
}

void FlightSearchWidget::applyStyles()
{
    // 应用样式
    setStyleSheet(loadStyleSheet("flightsearch.qss"));
}
```

### 信号槽设计

```cpp
// 信号定义
signals:
    void flightSearchStarted();
    void flightSearchCompleted(const QList<Flight> &flights);
    void flightSearchError(const QString &error);
    void flightSelected(const Flight &flight);

// 槽函数实现
private slots:
    void onSearchButtonClicked();
    void onSearchResponseReceived(QNetworkReply *reply);
    void onFlightTableSelectionChanged();
```

## API集成

### API管理器设计

```cpp
class APIManager : public QObject
{
    Q_OBJECT
    
public:
    explicit APIManager(QObject *parent = nullptr);
    
    // 航班相关API
    void searchFlights(const FlightSearchRequest &request);
    void getFlightDetails(const QString &flightNumber);
    
    // 用户相关API
    void authenticateUser(const LoginRequest &request);
    void createUser(const UserRegistrationRequest &request);
    
signals:
    void flightSearchCompleted(const FlightSearchResponse &response);
    void authenticationCompleted(const AuthResponse &response);
    void errorOccurred(const APIError &error);
    
private:
    QNetworkAccessManager *m_networkManager;
    QString m_baseUrl;
    QString m_apiToken;
    
    void sendRequest(const APIRequest &request);
    void handleResponse(QNetworkReply *reply);
};
```

### 请求/响应模型

```cpp
// 请求模型
struct FlightSearchRequest {
    QString departure;
    QString destination;
    QDate date;
    int passengers;
    QString cabinClass;
};

// 响应模型
struct FlightSearchResponse {
    bool success;
    QList<Flight> flights;
    QString message;
    int totalCount;
};

// API错误模型
struct APIError {
    int code;
    QString message;
    QString details;
};
```

### API调用示例

```cpp
void FlightSearchWidget::searchFlights()
{
    // 构建请求
    FlightSearchRequest request;
    request.departure = departureEdit->text();
    request.destination = destinationEdit->text();
    request.date = departureDateEdit->date();
    request.passengers = passengerCountCombo->currentText().toInt();
    
    // 发送请求
    emit searchStarted();
    APIManager::getInstance()->searchFlights(request);
}

// 处理响应
void FlightSearchWidget::onFlightSearchCompleted(const FlightSearchResponse &response)
{
    if (response.success) {
        populateFlightTable(response.flights);
        emit searchCompleted();
    } else {
        QMessageBox::warning(this, "搜索失败", response.message);
    }
}
```

## 数据库设计

### 数据库架构

```sql
-- 航班表
CREATE TABLE flights (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    flight_number TEXT UNIQUE NOT NULL,
    airline TEXT NOT NULL,
    departure TEXT NOT NULL,
    destination TEXT NOT NULL,
    departure_time TEXT NOT NULL,
    arrival_time TEXT NOT NULL,
    status TEXT DEFAULT 'on_time',
    gate TEXT,
    aircraft TEXT,
    price_economy REAL,
    price_business REAL,
    price_first REAL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 用户表
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    phone TEXT,
    first_name TEXT,
    last_name TEXT,
    role TEXT DEFAULT 'user',
    status TEXT DEFAULT 'active',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    last_login DATETIME
);

-- 预订表
CREATE TABLE bookings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    booking_reference TEXT UNIQUE NOT NULL,
    user_id INTEGER NOT NULL,
    flight_number TEXT NOT NULL,
    passenger_count INTEGER DEFAULT 1,
    total_price REAL NOT NULL,
    status TEXT DEFAULT 'confirmed',
    booking_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (flight_number) REFERENCES flights(flight_number)
);

-- 乘客表
CREATE TABLE passengers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    booking_id INTEGER NOT NULL,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    id_number TEXT NOT NULL,
    birth_date TEXT,
    gender TEXT,
    seat_number TEXT,
    cabin_class TEXT DEFAULT 'economy',
    FOREIGN KEY (booking_id) REFERENCES bookings(id)
);
```

### 数据库助手类

```cpp
class DatabaseHelper : public QObject
{
    Q_OBJECT
    
public:
    static DatabaseHelper* getInstance();
    
    bool connect(const QString &databasePath);
    bool isConnected() const;
    
    // 航班操作
    bool insertFlight(const Flight &flight);
    QList<Flight> getFlights(const QString &departure = "", 
                            const QString &destination = "");
    Flight getFlight(const QString &flightNumber);
    bool updateFlightStatus(const QString &flightNumber, 
                           const QString &status);
    
    // 用户操作
    bool insertUser(const User &user);
    User getUser(const QString &username);
    bool updateUser(const User &user);
    
    // 预订操作
    bool insertBooking(const Booking &booking);
    QList<Booking> getUserBookings(int userId);
    
private:
    QSqlDatabase m_database;
    bool m_connected;
    
    DatabaseHelper();
    bool createTables();
    QSqlQuery executeQuery(const QString &query, 
                          const QVariantList &params = {});
};
```

### 数据模型

```cpp
// Flight模型
struct Flight {
    QString flightNumber;
    QString airline;
    QString departure;
    QString destination;
    QString departureTime;
    QString arrivalTime;
    QString status;
    QString gate;
    QString aircraft;
    double priceEconomy;
    double priceBusiness;
    double priceFirst;
    
    // 序列化
    QJsonObject toJson() const;
    static Flight fromJson(const QJsonObject &json);
};

// User模型
struct User {
    int id;
    QString username;
    QString email;
    QString phone;
    QString firstName;
    QString lastName;
    QString role;
    QString status;
    QDateTime createdAt;
    QDateTime lastLogin;
};
```

## UI开发指南

### 样式系统

#### 全局样式表
```css
/* 暗色主题 */
QMainWindow {
    background-color: #2b2b2b;
    color: #ffffff;
}

QPushButton {
    background-color: #4a90e2;
    color: white;
    border: none;
    padding: 10px 20px;
    border-radius: 6px;
}

QPushButton:hover {
    background-color: #357abd;
}
```

#### 主题管理器
```cpp
class ThemeManager : public QObject
{
    Q_OBJECT
    
public:
    enum Theme { Light, Dark };
    
    static ThemeManager* getInstance();
    
    void setTheme(Theme theme);
    Theme currentTheme() const;
    
    QString getStyleSheet() const;
    void applyTheme(QWidget *widget);
    
signals:
    void themeChanged(Theme theme);
    
private:
    Theme m_currentTheme;
    QString m_lightStyleSheet;
    QString m_darkStyleSheet;
    
    ThemeManager();
    void loadStyleSheets();
};
```

### 响应式布局

```cpp
void FlightSearchWidget::setupResponsiveLayout()
{
    // 创建响应式布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    // 左侧搜索面板
    QWidget *searchPanel = new QWidget();
    searchPanel->setMaximumWidth(400);
    searchPanel->setMinimumWidth(300);
    
    // 右侧结果面板
    QWidget *resultPanel = new QWidget();
    
    // 使用比例设置
    mainLayout->addWidget(searchPanel, 1);
    mainLayout->addWidget(resultPanel, 3);
    
    // 响应式调整
    connect(this, &FlightSearchWidget::resized, 
            this, &FlightSearchWidget::adjustLayout);
}
```

### 国际化支持

```cpp
// 翻译文件设置
void FlightSystemApplication::setupTranslations()
{
    QTranslator *translator = new QTranslator(this);
    
    // 加载翻译文件
    QString locale = QLocale::system().name();
    if (translator->load(QString(":/translations/FlightSystem_%1.qm").arg(locale))) {
        installTranslator(translator);
    }
}

// 在代码中使用可翻译字符串
void FlightSearchWidget::setupUI()
{
    searchButton = new QPushButton(tr("Search Flights"), this);
    clearButton = new QPushButton(tr("Clear"), this);
}
```

## 测试指南

### 单元测试

```cpp
// test_flightsearchwidget.cpp
class TestFlightSearchWidget : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testSearchButtonClicked();
    void testClearButtonClicked();
    void testFlightSelection();
    
private:
    FlightSearchWidget *widget;
};

void TestFlightSearchWidget::testSearchButtonClicked()
{
    // 设置测试数据
    widget->departureEdit->setText("北京");
    widget->destinationEdit->setText("上海");
    
    // 模拟点击
    QTest::mouseClick(widget->searchButton, Qt::LeftButton);
    
    // 验证结果
    QVERIFY(widget->isSearching);
    QCOMPARE(widget->resultsTable->rowCount(), 0);
}
```

### 集成测试

```cpp
// test_apiintegration.cpp
class TestAPIIntegration : public QObject
{
    Q_OBJECT
    
private slots:
    void testFlightSearchAPI();
    void testUserAuthenticationAPI();
    void testBookingAPI();
    
private:
    APIManager *apiManager;
    QSignalSpy *spy;
};

void TestAPIIntegration::testFlightSearchAPI()
{
    // 准备测试请求
    FlightSearchRequest request;
    request.departure = "北京";
    request.destination = "上海";
    request.date = QDate(2024, 1, 15);
    
    // 发送请求
    apiManager->searchFlights(request);
    
    // 等待响应
    spy->wait(5000);
    
    // 验证响应
    QCOMPARE(spy->count(), 1);
    QList<QVariant> arguments = spy->takeFirst();
    FlightSearchResponse response = arguments.at(0).value<FlightSearchResponse>();
    QVERIFY(response.success);
    QVERIFY(response.flights.size() > 0);
}
```

### UI测试

```cpp
// test_ui.cpp
void TestUI::testMainWindowStartup()
{
    MainWindow window;
    window.show();
    
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    QCOMPARE(window.windowTitle(), "Flight Management System");
    QVERIFY(window.findChild<FlightSearchWidget*>() != nullptr);
}

void TestUI::testThemeSwitching()
{
    MainWindow window;
    
    // 测试主题切换
    QAction *themeAction = window.findChild<QAction*>("themeAction");
    QVERIFY(themeAction != nullptr);
    
    QTest::mouseClick(themeAction, Qt::LeftButton);
    
    // 验证主题应用
    QString styleSheet = window.styleSheet();
    QVERIFY(styleSheet.contains("background-color"));
}
```

## 部署指南

### 构建配置

#### qmake配置
```pro
# FlightSystem.pro
QT += core gui widgets sql network

CONFIG += c++17
CONFIG += release

TARGET = FlightSystem
TEMPLATE = app

# Windows特定配置
win32 {
    RC_ICONS = icons/flight_icon.ico
    LIBS += -luser32 -lshell32
}

# macOS特定配置
macx {
    ICON = icons/flight_icon.icns
    QMAKE_INFO_PLIST = Info.plist
}

# Linux特定配置
unix:!macx {
    target.path = /usr/local/bin
    INSTALLS += target
}
```

### 打包部署

#### Windows
```bash
# 使用 windeployqt
windeployqt FlightSystem.exe

# 创建安装包
makensis installer.nsi
```

#### macOS
```bash
# 使用 macdeployqt
macdeployqt FlightSystem.app

# 创建DMG
create-dmg --volname "Flight System" \
           --window-pos 200 120 \
           --window-size 600 300 \
           --icon-size 100 \
           --icon "FlightSystem.app" 175 120 \
           --hide-extension "FlightSystem.app" \
           --app-drop-link 425 120 \
           "FlightSystem.dmg" \
           "dist/"
```

#### Linux
```bash
# 创建AppImage
linuxdeploy --appdir AppDir \
            --executable FlightSystem \
            --create-desktop-file \
            --icon-file icons/flight_icon.png \
            --output appimage

# 或创建DEB包
dpkg-deb --build flightsystem-deb
```

### 安装脚本

#### Windows安装脚本
```nsis
; installer.nsi
!define APPNAME "Flight Management System"
!define VERSION "1.0.0"

Name "${APPNAME}"
OutFile "FlightSystemSetup.exe"
InstallDir "$PROGRAMFILES\FlightSystem"

Page directory
Page instfiles

Section "MainSection" SEC01
    SetOutPath "$INSTDIR"
    File "FlightSystem.exe"
    File /r "platforms"
    File /r "styles"
    File /r "icons"
    
    CreateShortCut "$DESKTOP\FlightSystem.lnk" "$INSTDIR\FlightSystem.exe"
    CreateDirectory "$SMPROGRAMS\FlightSystem"
    CreateShortCut "$SMPROGRAMS\FlightSystem\FlightSystem.lnk" "$INSTDIR\FlightSystem.exe"
SectionEnd
```

## 性能优化

### 内存管理

```cpp
// 使用智能指针
class FlightSearchWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FlightSearchWidget(QWidget *parent = nullptr);
    
private:
    std::unique_ptr<APIManager> m_apiManager;
    std::shared_ptr<DatabaseHelper> m_database;
};

// 避免内存泄漏
void FlightSearchWidget::cleanup()
{
    // 清理资源
    if (m_networkReply) {
        m_networkReply->abort();
        m_networkReply->deleteLater();
        m_networkReply = nullptr;
    }
}
```

### 异步处理

```cpp
// 异步数据加载
void FlightSearchWidget::loadFlightsAsync()
{
    QFuture<QList<Flight>> future = QtConcurrent::run([this]() {
        return DatabaseHelper::getInstance()->getFlights();
    });
    
    QFutureWatcher<QList<Flight>> *watcher = 
        new QFutureWatcher<QList<Flight>>(this);
    
    connect(watcher, &QFutureWatcher<QList<Flight>>::finished, [this, watcher]() {
        QList<Flight> flights = watcher->result();
        populateFlightTable(flights);
        watcher->deleteLater();
    });
    
    watcher->setFuture(future);
}
```

### 缓存策略

```cpp
class CacheManager : public QObject
{
    Q_OBJECT
    
public:
    static CacheManager* getInstance();
    
    template<typename T>
    void cache(const QString &key, const T &data, int ttl = 3600);
    
    template<typename T>
    T getCached(const QString &key, const T &defaultValue = T());
    
    bool hasCached(const QString &key);
    void clearCache();
    
private:
    QHash<QString, QPair<QVariant, QDateTime>> m_cache;
    QTimer *m_cleanupTimer;
    
    void cleanupExpired();
};
```

### 性能监控

```cpp
class PerformanceMonitor : public QObject
{
    Q_OBJECT
    
public:
    static PerformanceMonitor* getInstance();
    
    void startTimer(const QString &operation);
    void endTimer(const QString &operation);
    
    void logMemoryUsage();
    void logPerformanceStats();
    
private:
    QHash<QString, QElapsedTimer> m_timers;
    QElapsedTimer m_globalTimer;
    
    void writeLog(const QString &message);
};
```

---

这份开发文档提供了完整的开发指南，包括架构设计、代码规范、测试策略等。遵循这些指南可以确保代码质量和项目的可维护性。
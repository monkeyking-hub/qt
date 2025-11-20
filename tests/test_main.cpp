#include <QtTest/QtTest>
#include <QApplication>
#include "mainwindow.h"

class TestFlightSystem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testMainWindowCreation();
    void testFlightSearchWidget();
    void testUserManagement();
    void testThemeSwitching();

private:
    MainWindow *mainWindow;
};

void TestFlightSystem::initTestCase()
{
    // 初始化测试环境
    QApplication::setApplicationName("FlightSystem Test");
}

void TestFlightSystem::cleanupTestCase()
{
    // 清理测试环境
    if (mainWindow) {
        delete mainWindow;
        mainWindow = nullptr;
    }
}

void TestFlightSystem::testMainWindowCreation()
{
    mainWindow = new MainWindow();
    QVERIFY(mainWindow != nullptr);
    QCOMPARE(mainWindow->windowTitle(), "Flight Management System");
    QVERIFY(mainWindow->findChild<QWidget*>() != nullptr);
}

void TestFlightSystem::testFlightSearchWidget()
{
    // 测试航班搜索组件
    MainWindow window;
    window.show();
    
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    
    // 查找航班搜索组件
    FlightSearchWidget *searchWidget = window.findChild<FlightSearchWidget*>();
    QVERIFY(searchWidget != nullptr);
    
    // 测试搜索功能
    QLineEdit *departureEdit = searchWidget->findChild<QLineEdit*>("departureEdit");
    QLineEdit *destinationEdit = searchWidget->findChild<QLineEdit*>("destinationEdit");
    QPushButton *searchButton = searchWidget->findChild<QPushButton*>("searchButton");
    
    QVERIFY(departureEdit != nullptr);
    QVERIFY(destinationEdit != nullptr);
    QVERIFY(searchButton != nullptr);
    
    // 填写搜索条件
    departureEdit->setText("北京");
    destinationEdit->setText("上海");
    
    // 模拟点击搜索按钮
    QTest::mouseClick(searchButton, Qt::LeftButton);
    
    // 验证搜索结果（这里需要根据实际实现调整）
    QTest::qWait(2000); // 等待搜索完成
}

void TestFlightSystem::testUserManagement()
{
    // 测试用户管理功能
    MainWindow window;
    window.show();
    
    // 切换到用户管理页面
    QAction *userAction = window.findChild<QAction*>("userAction");
    QVERIFY(userAction != nullptr);
    
    userAction->trigger();
    QTest::qWait(100);
    
    // 查找用户管理组件
    UserManagementWidget *userWidget = window.findChild<UserManagementWidget*>();
    QVERIFY(userWidget != nullptr);
    
    // 测试添加用户功能
    QPushButton *addButton = userWidget->findChild<QPushButton*>("addButton");
    QVERIFY(addButton != nullptr);
    
    QTest::mouseClick(addButton, Qt::LeftButton);
    QTest::qWait(100);
}

void TestFlightSystem::testThemeSwitching()
{
    // 测试主题切换功能
    MainWindow window;
    window.show();
    
    QAction *themeAction = window.findChild<QAction*>("themeAction");
    QVERIFY(themeAction != nullptr);
    
    // 获取初始样式
    QString initialStyle = window.styleSheet();
    
    // 切换主题
    themeAction->trigger();
    QTest::qWait(100);
    
    // 验证样式已改变
    QString newStyle = window.styleSheet();
    QVERIFY(initialStyle != newStyle);
}

QTEST_MAIN(TestFlightSystem)
#include "test_main.moc"
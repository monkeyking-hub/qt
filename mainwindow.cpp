#include "mainwindow.h"
#include "flightsearchwidget.h"
#include "flightbookingwidget.h"
#include "usermanagementwidget.h"
#include "flightdetailswidget.h"
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QStackedWidget>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralStack(nullptr)
    , systemTray(nullptr)
    , isDarkTheme(true)
{
    setupUI();
    setupSystemTray();
    applyTheme();
    
    // 设置窗口属性
    setWindowTitle("Flight Management System");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // 初始化时间定时器
    timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    timeTimer->start(1000);
    updateTime();
    
    // 显示默认页面
    showFlightSearch();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
    setupStatusBar();
}

void MainWindow::setupMenuBar()
{
    menuBar = this->menuBar();
    
    // 文件菜单
    fileMenu = menuBar->addMenu("文件(&F)");
    
    searchAction = new QAction("航班查询(&S)", this);
    searchAction->setShortcut(QKeySequence("Ctrl+S"));
    searchAction->setStatusTip("查询航班信息");
    connect(searchAction, &QAction::triggered, this, &MainWindow::showFlightSearch);
    fileMenu->addAction(searchAction);
    
    bookingAction = new QAction("航班预订(&B)", this);
    bookingAction->setShortcut(QKeySequence("Ctrl+B"));
    bookingAction->setStatusTip("预订航班");
    connect(bookingAction, &QAction::triggered, this, &MainWindow::showFlightBooking);
    fileMenu->addAction(bookingAction);
    
    fileMenu->addSeparator();
    
    exitAction = new QAction("退出(&X)", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    exitAction->setStatusTip("退出应用程序");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
    
    // 视图菜单
    viewMenu = menuBar->addMenu("视图(&V)");
    
    userAction = new QAction("用户管理(&U)", this);
    userAction->setShortcut(QKeySequence("Ctrl+U"));
    userAction->setStatusTip("管理用户信息");
    connect(userAction, &QAction::triggered, this, &MainWindow::showUserManagement);
    viewMenu->addAction(userAction);
    
    detailsAction = new QAction("航班详情(&D)", this);
    detailsAction->setShortcut(QKeySequence("Ctrl+D"));
    detailsAction->setStatusTip("查看航班详细信息");
    connect(detailsAction, &QAction::triggered, this, &MainWindow::showFlightDetails);
    viewMenu->addAction(detailsAction);
    
    // 工具菜单
    toolsMenu = menuBar->addMenu("工具(&T)");
    
    themeAction = new QAction("切换主题(&T)", this);
    themeAction->setStatusTip("切换明暗主题");
    connect(themeAction, &QAction::triggered, this, [this]() {
        isDarkTheme = !isDarkTheme;
        applyTheme();
    });
    toolsMenu->addAction(themeAction);
    
    // 帮助菜单
    helpMenu = menuBar->addMenu("帮助(&H)");
    
    aboutAction = new QAction("关于(&A)", this);
    aboutAction->setStatusTip("关于航班管理系统");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupToolBar()
{
    mainToolBar = this->addToolBar("主工具栏");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    
    // 添加工具栏按钮
    mainToolBar->addAction(searchAction);
    mainToolBar->addAction(bookingAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(userAction);
    mainToolBar->addAction(detailsAction);
    
    // 设置图标
    searchAction->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    bookingAction->setIcon(this->style()->standardIcon(QStyle::SP_DialogApplyButton));
    userAction->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogInfoView));
    detailsAction->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogContentsView));
}

void MainWindow::setupCentralWidget()
{
    centralStack = new QStackedWidget(this);
    setCentralWidget(centralStack);
    
    // 创建各个功能页面
    flightSearchWidget = new FlightSearchWidget(this);
    flightBookingWidget = new FlightBookingWidget(this);
    userManagementWidget = new UserManagementWidget(this);
    flightDetailsWidget = new FlightDetailsWidget(this);
    
    // 添加到堆栈窗口
    centralStack->addWidget(flightSearchWidget);
    centralStack->addWidget(flightBookingWidget);
    centralStack->addWidget(userManagementWidget);
    centralStack->addWidget(flightDetailsWidget);
}

void MainWindow::setupStatusBar()
{
    statusBar = this->statusBar();
    
    // 状态标签
    statusLabel = new QLabel("就绪", this);
    statusBar->addWidget(statusLabel);
    
    // 用户标签
    userLabel = new QLabel("用户: 访客", this);
    statusBar->addPermanentWidget(userLabel);
    
    // 时间标签
    timeLabel = new QLabel(this);
    statusBar->addPermanentWidget(timeLabel);
}

void MainWindow::setupSystemTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        systemTray = new QSystemTrayIcon(this);
        systemTray->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
        systemTray->setToolTip("Flight Management System");
        
        connect(systemTray, &QSystemTrayIcon::activated, 
                this, &MainWindow::onSystemTrayActivated);
        
        // 创建托盘菜单
        QMenu *trayMenu = new QMenu(this);
        trayMenu->addAction(searchAction);
        trayMenu->addAction(bookingAction);
        trayMenu->addSeparator();
        trayMenu->addAction(exitAction);
        
        systemTray->setContextMenu(trayMenu);
        systemTray->show();
    }
}

void MainWindow::applyTheme()
{
    QString styleSheet;
    
    if (isDarkTheme) {
        styleSheet = R"(
            QMainWindow {
                background-color: #2b2b2b;
                color: #ffffff;
            }
            QMenuBar {
                background-color: #3c3c3c;
                color: #ffffff;
                border-bottom: 1px solid #555555;
            }
            QMenuBar::item {
                background-color: transparent;
                padding: 8px 16px;
            }
            QMenuBar::item:selected {
                background-color: #4a4a4a;
            }
            QToolBar {
                background-color: #3c3c3c;
                border: 1px solid #555555;
                spacing: 3px;
            }
            QStatusBar {
                background-color: #3c3c3c;
                color: #ffffff;
                border-top: 1px solid #555555;
            }
            QStackedWidget {
                background-color: #2b2b2b;
            }
            QPushButton {
                background-color: #4a4a4a;
                color: #ffffff;
                border: 1px solid #666666;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #5a5a5a;
            }
            QPushButton:pressed {
                background-color: #3a3a3a;
            }
        )";
    } else {
        styleSheet = R"(
            QMainWindow {
                background-color: #ffffff;
                color: #000000;
            }
            QMenuBar {
                background-color: #f0f0f0;
                color: #000000;
                border-bottom: 1px solid #cccccc;
            }
            QMenuBar::item {
                background-color: transparent;
                padding: 8px 16px;
            }
            QMenuBar::item:selected {
                background-color: #e0e0e0;
            }
            QToolBar {
                background-color: #f0f0f0;
                border: 1px solid #cccccc;
                spacing: 3px;
            }
            QStatusBar {
                background-color: #f0f0f0;
                color: #000000;
                border-top: 1px solid #cccccc;
            }
            QStackedWidget {
                background-color: #ffffff;
            }
            QPushButton {
                background-color: #e0e0e0;
                color: #000000;
                border: 1px solid #cccccc;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #d0d0d0;
            }
            QPushButton:pressed {
                background-color: #c0c0c0;
            }
        )";
    }
    
    this->setStyleSheet(styleSheet);
}

void MainWindow::showFlightSearch()
{
    centralStack->setCurrentWidget(flightSearchWidget);
    statusLabel->setText("航班查询");
}

void MainWindow::showFlightBooking()
{
    centralStack->setCurrentWidget(flightBookingWidget);
    statusLabel->setText("航班预订");
}

void MainWindow::showUserManagement()
{
    centralStack->setCurrentWidget(userManagementWidget);
    statusLabel->setText("用户管理");
}

void MainWindow::showFlightDetails()
{
    centralStack->setCurrentWidget(flightDetailsWidget);
    statusLabel->setText("航班详情");
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "关于航班管理系统",
                      "Flight Management System v1.0.0\n\n"
                      "一个基于Qt的航班管理系统前端\n"
                      "功能包括航班查询、预订、用户管理等\n\n"
                      "© 2024 Flight Systems Inc.");
}

void MainWindow::updateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    timeLabel->setText(currentDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (isVisible()) {
            hide();
        } else {
            show();
            raise();
            activateWindow();
        }
    }
}
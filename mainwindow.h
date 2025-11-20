#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QTimer>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QToolBar;
class QStatusBar;
class QStackedWidget;
QT_END_NAMESPACE

class FlightSearchWidget;
class FlightBookingWidget;
class UserManagementWidget;
class FlightDetailsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showFlightSearch();
    void showFlightBooking();
    void showUserManagement();
    void showFlightDetails();
    void showAbout();
    void updateTime();
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupSystemTray();
    void applyTheme();
    
    // UI组件
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QStackedWidget *centralStack;
    QSystemTrayIcon *systemTray;
    
    // 菜单和动作
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    
    QAction *searchAction;
    QAction *bookingAction;
    QAction *userAction;
    QAction *detailsAction;
    QAction *exitAction;
    QAction *aboutAction;
    QAction *themeAction;
    
    // 状态栏组件
    QLabel *statusLabel;
    QLabel *timeLabel;
    QLabel *userLabel;
    
    // 主要窗口组件
    FlightSearchWidget *flightSearchWidget;
    FlightBookingWidget *flightBookingWidget;
    UserManagementWidget *userManagementWidget;
    FlightDetailsWidget *flightDetailsWidget;
    
    // 定时器
    QTimer *timeTimer;
    
    // 主题相关
    bool isDarkTheme;
};

#endif // MAINWINDOW_H
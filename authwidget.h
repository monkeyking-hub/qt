#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QStackedWidget>
#include <QProgressBar>

class APIManager;

class AuthWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWidget(APIManager *apiManager, QWidget *parent = nullptr);

signals:
    void loginSuccessful(const QString &userId, const QString &token);
    void registerSuccessful(const QString &message);
    void authFailed(const QString &error);

private slots:
    void showLoginPage();
    void showRegisterPage();
    void performLogin();
    void performRegister();
    void onLoginCompleted(const QJsonObject &result);
    void onRegisterCompleted(const QJsonObject &result);
    void onAuthError(const QString &error);
    void togglePasswordVisibility();
    void validateEmail(const QString &email);
    void validateUsername(const QString &username);

private:
    void setupUI();
    void setupLoginUI();
    void setupRegisterUI();
    void setupConnections();
    void showError(const QString &error);
    void clearForms();
    bool validateLoginForm();
    bool validateRegisterForm();
    void setLoading(bool loading);
    void applyStyles();
    
    APIManager *apiManager;
    QStackedWidget *stackedWidget;
    
    // 登录页面
    QWidget *loginWidget;
    QLineEdit *loginEmailEdit;
    QLineEdit *loginPasswordEdit;
    QCheckBox *rememberCheckBox;
    QPushButton *loginButton;
    QPushButton *registerLinkButton;
    QLabel *loginErrorLabel;
    QProgressBar *loginProgressBar;
    
    // 注册页面
    QWidget *registerWidget;
    QLineEdit *registerEmailEdit;
    QLineEdit *registerUsernameEdit;
    QLineEdit *registerPasswordEdit;
    QLineEdit *registerConfirmPasswordEdit;
    QLineEdit *registerNameEdit;
    QLineEdit *registerPhoneEdit;
    QPushButton *registerButton;
    QPushButton *loginLinkButton;
    QLabel *registerErrorLabel;
    QLabel *usernameStatusLabel;
    QLabel *emailStatusLabel;
    QProgressBar *registerProgressBar;
    QPushButton *togglePasswordButton;
    
    // 状态
    bool isLoading;
};

#endif // AUTHWIDGET_H

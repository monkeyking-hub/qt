#include "authwidget.h"
#include "apimanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QStackedWidget>
#include <QProgressBar>
#include <QRegularExpression>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QApplication>

AuthWidget::AuthWidget(APIManager *apiManager, QWidget *parent)
    : QWidget(parent)
    , apiManager(apiManager)
    , isLoading(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    showLoginPage();
}

void AuthWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    stackedWidget = new QStackedWidget(this);
    
    // 设置登录页面
    setupLoginUI();
    // 设置注册页面
    setupRegisterUI();
    
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void AuthWidget::setupLoginUI()
{
    loginWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    layout->setContentsMargins(40, 60, 40, 60);
    layout->setSpacing(20);
    
    // 标题
    QLabel *titleLabel = new QLabel("航班管理系统");
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #007AFF;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel("登录您的账户");
    subtitleLabel->setStyleSheet("font-size: 14px; color: #999999;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitleLabel);
    
    layout->addSpacing(30);
    
    // 邮箱
    QLabel *emailLabel = new QLabel("邮箱或用户名:");
    emailLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    layout->addWidget(emailLabel);
    
    loginEmailEdit = new QLineEdit();
    loginEmailEdit->setPlaceholderText("请输入邮箱或用户名");
    loginEmailEdit->setMinimumHeight(40);
    loginEmailEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 8px 12px; "
        "  font-size: 14px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    layout->addWidget(loginEmailEdit);
    
    // 密码
    QLabel *passwordLabel = new QLabel("密码:");
    passwordLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    layout->addWidget(passwordLabel);
    
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    
    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setPlaceholderText("请输入密码");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setMinimumHeight(40);
    loginPasswordEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 8px 12px; "
        "  font-size: 14px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    passwordLayout->addWidget(loginPasswordEdit);
    
    togglePasswordButton = new QPushButton("👁️");
    togglePasswordButton->setFixedWidth(40);
    togglePasswordButton->setMinimumHeight(40);
    togglePasswordButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "} "
        "QPushButton:hover { background-color: #4a4a4a; }"
    );
    connect(togglePasswordButton, &QPushButton::clicked, this, &AuthWidget::togglePasswordVisibility);
    passwordLayout->addWidget(togglePasswordButton);
    
    layout->addLayout(passwordLayout);
    
    // 记住我
    rememberCheckBox = new QCheckBox("记住我");
    rememberCheckBox->setStyleSheet("color: #999999;");
    layout->addWidget(rememberCheckBox);
    
    layout->addSpacing(10);
    
    // 错误提示
    loginErrorLabel = new QLabel();
    loginErrorLabel->setStyleSheet("color: #FF3B30; font-size: 12px;");
    loginErrorLabel->setWordWrap(true);
    loginErrorLabel->setVisible(false);
    layout->addWidget(loginErrorLabel);
    
    // 进度条
    loginProgressBar = new QProgressBar();
    loginProgressBar->setVisible(false);
    loginProgressBar->setStyleSheet(
        "QProgressBar { "
        "  border: none; "
        "  background-color: #3c3c3c; "
        "  border-radius: 2px; height: 4px; "
        "} "
        "QProgressBar::chunk { background-color: #007AFF; }"
    );
    layout->addWidget(loginProgressBar);
    
    // 登录按钮
    loginButton = new QPushButton("登录");
    loginButton->setMinimumHeight(44);
    loginButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #007AFF; "
        "  color: #ffffff; "
        "  border: none; "
        "  border-radius: 4px; "
        "  font-size: 14px; "
        "  font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #0051D5; } "
        "QPushButton:pressed { background-color: #003DB3; }"
    );
    connect(loginButton, &QPushButton::clicked, this, &AuthWidget::performLogin);
    layout->addWidget(loginButton);
    
    layout->addSpacing(10);
    
    // 注册链接
    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->addStretch();
    
    QLabel *noAccountLabel = new QLabel("没有账户?");
    noAccountLabel->setStyleSheet("color: #999999;");
    linkLayout->addWidget(noAccountLabel);
    
    registerLinkButton = new QPushButton("立即注册");
    registerLinkButton->setStyleSheet(
        "QPushButton { "
        "  background-color: transparent; "
        "  color: #007AFF; "
        "  border: none; "
        "  text-decoration: underline; "
        "  font-size: 12px; "
        "} "
        "QPushButton:hover { color: #0051D5; }"
    );
    connect(registerLinkButton, &QPushButton::clicked, this, &AuthWidget::showRegisterPage);
    linkLayout->addWidget(registerLinkButton);
    
    linkLayout->addStretch();
    layout->addLayout(linkLayout);
    
    layout->addStretch();
    
    stackedWidget->addWidget(loginWidget);
}

void AuthWidget::setupRegisterUI()
{
    registerWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(registerWidget);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(15);
    
    // 标题
    QLabel *titleLabel = new QLabel("创建账户");
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #007AFF;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel("加入我们的航班管理系统");
    subtitleLabel->setStyleSheet("font-size: 14px; color: #999999;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitleLabel);
    
    layout->addSpacing(20);
    
    // 使用网格布局
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(12);
    
    // 邮箱
    QLabel *emailLabel = new QLabel("邮箱:");
    emailLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(emailLabel, 0, 0);
    
    QHBoxLayout *emailLayout = new QHBoxLayout();
    registerEmailEdit = new QLineEdit();
    registerEmailEdit->setPlaceholderText("请输入邮箱");
    registerEmailEdit->setMinimumHeight(36);
    registerEmailEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    connect(registerEmailEdit, &QLineEdit::textChanged, this, &AuthWidget::validateEmail);
    emailLayout->addWidget(registerEmailEdit);
    
    emailStatusLabel = new QLabel();
    emailStatusLabel->setStyleSheet("font-size: 14px;");
    emailLayout->addWidget(emailStatusLabel);
    emailLayout->addStretch();
    gridLayout->addLayout(emailLayout, 0, 1);
    
    // 用户名
    QLabel *usernameLabel = new QLabel("用户名:");
    usernameLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(usernameLabel, 1, 0);
    
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    registerUsernameEdit = new QLineEdit();
    registerUsernameEdit->setPlaceholderText("3-20个字符");
    registerUsernameEdit->setMinimumHeight(36);
    registerUsernameEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    connect(registerUsernameEdit, &QLineEdit::textChanged, this, &AuthWidget::validateUsername);
    usernameLayout->addWidget(registerUsernameEdit);
    
    usernameStatusLabel = new QLabel();
    usernameStatusLabel->setStyleSheet("font-size: 14px;");
    usernameLayout->addWidget(usernameStatusLabel);
    usernameLayout->addStretch();
    gridLayout->addLayout(usernameLayout, 1, 1);
    
    // 姓名
    QLabel *nameLabel = new QLabel("姓名:");
    nameLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(nameLabel, 2, 0);
    
    registerNameEdit = new QLineEdit();
    registerNameEdit->setPlaceholderText("请输入姓名");
    registerNameEdit->setMinimumHeight(36);
    registerNameEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    gridLayout->addWidget(registerNameEdit, 2, 1);
    
    // 电话
    QLabel *phoneLabel = new QLabel("电话:");
    phoneLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(phoneLabel, 3, 0);
    
    registerPhoneEdit = new QLineEdit();
    registerPhoneEdit->setPlaceholderText("请输入电话号码");
    registerPhoneEdit->setMinimumHeight(36);
    registerPhoneEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    gridLayout->addWidget(registerPhoneEdit, 3, 1);
    
    // 密码
    QLabel *passwordLabel = new QLabel("密码:");
    passwordLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(passwordLabel, 4, 0);
    
    registerPasswordEdit = new QLineEdit();
    registerPasswordEdit->setPlaceholderText("至少8个字符");
    registerPasswordEdit->setEchoMode(QLineEdit::Password);
    registerPasswordEdit->setMinimumHeight(36);
    registerPasswordEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    gridLayout->addWidget(registerPasswordEdit, 4, 1);
    
    // 确认密码
    QLabel *confirmLabel = new QLabel("确认密码:");
    confirmLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #ffffff;");
    gridLayout->addWidget(confirmLabel, 5, 0);
    
    registerConfirmPasswordEdit = new QLineEdit();
    registerConfirmPasswordEdit->setPlaceholderText("请再次输入密码");
    registerConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    registerConfirmPasswordEdit->setMinimumHeight(36);
    registerConfirmPasswordEdit->setStyleSheet(
        "QLineEdit { "
        "  background-color: #3c3c3c; "
        "  color: #ffffff; "
        "  border: 1px solid #555555; "
        "  border-radius: 4px; "
        "  padding: 6px 10px; "
        "  font-size: 12px; "
        "} "
        "QLineEdit:focus { border: 2px solid #007AFF; }"
    );
    gridLayout->addWidget(registerConfirmPasswordEdit, 5, 1);
    
    layout->addLayout(gridLayout);
    
    layout->addSpacing(10);
    
    // 错误提示
    registerErrorLabel = new QLabel();
    registerErrorLabel->setStyleSheet("color: #FF3B30; font-size: 12px;");
    registerErrorLabel->setWordWrap(true);
    registerErrorLabel->setVisible(false);
    layout->addWidget(registerErrorLabel);
    
    // 进度条
    registerProgressBar = new QProgressBar();
    registerProgressBar->setVisible(false);
    registerProgressBar->setStyleSheet(
        "QProgressBar { "
        "  border: none; "
        "  background-color: #3c3c3c; "
        "  border-radius: 2px; height: 4px; "
        "} "
        "QProgressBar::chunk { background-color: #007AFF; }"
    );
    layout->addWidget(registerProgressBar);
    
    // 注册按钮
    registerButton = new QPushButton("注册");
    registerButton->setMinimumHeight(44);
    registerButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #007AFF; "
        "  color: #ffffff; "
        "  border: none; "
        "  border-radius: 4px; "
        "  font-size: 14px; "
        "  font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #0051D5; } "
        "QPushButton:pressed { background-color: #003DB3; }"
    );
    connect(registerButton, &QPushButton::clicked, this, &AuthWidget::performRegister);
    layout->addWidget(registerButton);
    
    layout->addSpacing(10);
    
    // 登录链接
    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->addStretch();
    
    QLabel *hasAccountLabel = new QLabel("已有账户?");
    hasAccountLabel->setStyleSheet("color: #999999;");
    linkLayout->addWidget(hasAccountLabel);
    
    loginLinkButton = new QPushButton("立即登录");
    loginLinkButton->setStyleSheet(
        "QPushButton { "
        "  background-color: transparent; "
        "  color: #007AFF; "
        "  border: none; "
        "  text-decoration: underline; "
        "  font-size: 12px; "
        "} "
        "QPushButton:hover { color: #0051D5; }"
    );
    connect(loginLinkButton, &QPushButton::clicked, this, &AuthWidget::showLoginPage);
    linkLayout->addWidget(loginLinkButton);
    
    linkLayout->addStretch();
    layout->addLayout(linkLayout);
    
    layout->addStretch();
    
    stackedWidget->addWidget(registerWidget);
}

void AuthWidget::setupConnections()
{
    connect(apiManager, &APIManager::loginCompleted, this, &AuthWidget::onLoginCompleted);
    connect(apiManager, &APIManager::registerCompleted, this, &AuthWidget::onRegisterCompleted);
    connect(apiManager, &APIManager::errorOccurred, this, &AuthWidget::onAuthError);
}

void AuthWidget::showLoginPage()
{
    stackedWidget->setCurrentWidget(loginWidget);
    clearForms();
}

void AuthWidget::showRegisterPage()
{
    stackedWidget->setCurrentWidget(registerWidget);
    clearForms();
}

void AuthWidget::performLogin()
{
    if (!validateLoginForm()) {
        return;
    }
    
    setLoading(true);
    apiManager->loginUser(loginEmailEdit->text(), loginPasswordEdit->text());
}

void AuthWidget::performRegister()
{
    if (!validateRegisterForm()) {
        return;
    }
    
    setLoading(true);
    
    QJsonObject userData;
    userData["email"] = registerEmailEdit->text();
    userData["username"] = registerUsernameEdit->text();
    userData["password"] = registerPasswordEdit->text();
    userData["full_name"] = registerNameEdit->text();
    userData["phone"] = registerPhoneEdit->text();
    
    apiManager->registerUser(userData);
}

void AuthWidget::onLoginCompleted(const QJsonObject &result)
{
    setLoading(false);
    
    if (result.value("code").toInt() == 0) {
        QString userId = result.value("data").toObject().value("user_id").toString();
        QString token = result.value("data").toObject().value("token").toString();
        emit loginSuccessful(userId, token);
    } else {
        showError(result.value("message").toString());
    }
}

void AuthWidget::onRegisterCompleted(const QJsonObject &result)
{
    setLoading(false);
    
    if (result.value("code").toInt() == 0) {
        emit registerSuccessful(result.value("message").toString());
        showLoginPage();
    } else {
        showError(result.value("message").toString());
    }
}

void AuthWidget::onAuthError(const QString &error)
{
    setLoading(false);
    showError(error);
}

void AuthWidget::togglePasswordVisibility()
{
    if (loginPasswordEdit->echoMode() == QLineEdit::Password) {
        loginPasswordEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordButton->setText("🙈");
    } else {
        loginPasswordEdit->setEchoMode(QLineEdit::Password);
        togglePasswordButton->setText("👁️");
    }
}

void AuthWidget::validateEmail(const QString &email)
{
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (emailRegex.match(email).hasMatch()) {
        emailStatusLabel->setText("✓");
        emailStatusLabel->setStyleSheet("color: #34C759; font-size: 14px;");
    } else if (!email.isEmpty()) {
        emailStatusLabel->setText("✗");
        emailStatusLabel->setStyleSheet("color: #FF3B30; font-size: 14px;");
    } else {
        emailStatusLabel->clear();
    }
}

void AuthWidget::validateUsername(const QString &username)
{
    if (username.length() >= 3 && username.length() <= 20) {
        usernameStatusLabel->setText("✓");
        usernameStatusLabel->setStyleSheet("color: #34C759; font-size: 14px;");
    } else if (!username.isEmpty()) {
        usernameStatusLabel->setText("✗");
        usernameStatusLabel->setStyleSheet("color: #FF3B30; font-size: 14px;");
    } else {
        usernameStatusLabel->clear();
    }
}

void AuthWidget::showError(const QString &error)
{
    if (stackedWidget->currentWidget() == loginWidget) {
        loginErrorLabel->setText(error);
        loginErrorLabel->setVisible(true);
    } else {
        registerErrorLabel->setText(error);
        registerErrorLabel->setVisible(true);
    }
}

void AuthWidget::clearForms()
{
    loginEmailEdit->clear();
    loginPasswordEdit->clear();
    loginErrorLabel->clear();
    loginErrorLabel->setVisible(false);
    
    registerEmailEdit->clear();
    registerUsernameEdit->clear();
    registerPasswordEdit->clear();
    registerConfirmPasswordEdit->clear();
    registerNameEdit->clear();
    registerPhoneEdit->clear();
    registerErrorLabel->clear();
    registerErrorLabel->setVisible(false);
    emailStatusLabel->clear();
    usernameStatusLabel->clear();
}

bool AuthWidget::validateLoginForm()
{
    if (loginEmailEdit->text().isEmpty()) {
        showError("请输入邮箱或用户名");
        return false;
    }
    if (loginPasswordEdit->text().isEmpty()) {
        showError("请输入密码");
        return false;
    }
    return true;
}

bool AuthWidget::validateRegisterForm()
{
    if (registerEmailEdit->text().isEmpty()) {
        showError("请输入邮箱");
        return false;
    }
    if (registerUsernameEdit->text().length() < 3 || registerUsernameEdit->text().length() > 20) {
        showError("用户名长度必须在3-20之间");
        return false;
    }
    if (registerNameEdit->text().isEmpty()) {
        showError("请输入姓名");
        return false;
    }
    if (registerPhoneEdit->text().isEmpty()) {
        showError("请输入电话号码");
        return false;
    }
    if (registerPasswordEdit->text().length() < 8) {
        showError("密码长度必须至少8个字符");
        return false;
    }
    if (registerPasswordEdit->text() != registerConfirmPasswordEdit->text()) {
        showError("两次输入的密码不一致");
        return false;
    }
    return true;
}

void AuthWidget::setLoading(bool loading)
{
    isLoading = loading;
    loginButton->setEnabled(!loading);
    registerButton->setEnabled(!loading);
    loginProgressBar->setVisible(loading);
    registerProgressBar->setVisible(loading);
    
    if (loading) {
        loginProgressBar->setMaximum(0);
        registerProgressBar->setMaximum(0);
    } else {
        loginProgressBar->setMaximum(100);
        registerProgressBar->setMaximum(100);
    }
}

void AuthWidget::applyStyles()
{
    setStyleSheet(
        "QWidget { background-color: #2b2b2b; color: #ffffff; } "
        "QLabel { color: #ffffff; }"
    );
}

#include "usermanagementwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QSplitter>
#include <QScrollArea>

UserManagementWidget::UserManagementWidget(QWidget *parent)
    : QWidget(parent)
    , isEditing(false)
    , currentEditRow(-1)
{
    setupUI();
    connectSignals();
    loadSampleUsers();
    applyStyles();
}

void UserManagementWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // 设置标题
    QLabel *titleLabel = new QLabel("用户管理", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0;");
    mainLayout->addWidget(titleLabel);
    
    setupSearchBar();
    setupUserForm();
    setupUserTable();
    
    // 按钮
    buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("添加用户", this);
    editButton = new QPushButton("编辑用户", this);
    deleteButton = new QPushButton("删除用户", this);
    saveButton = new QPushButton("保存", this);
    cancelButton = new QPushButton("取消", this);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    
    saveButton->setVisible(false);
    cancelButton->setVisible(false);
    
    mainLayout->addLayout(buttonLayout);
}

void UserManagementWidget::setupSearchBar()
{
    searchLayout = new QHBoxLayout();
    
    searchLayout->addWidget(new QLabel("搜索:", this));
    searchTypeCombo = new QComboBox(this);
    searchTypeCombo->addItems({"用户名", "邮箱", "电话", "用户ID"});
    searchLayout->addWidget(searchTypeCombo);
    
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("输入搜索关键词");
    searchLayout->addWidget(searchEdit);
    
    searchButton = new QPushButton("搜索", this);
    clearSearchButton = new QPushButton("清除", this);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearSearchButton);
    searchLayout->addStretch();
    
    mainLayout->addLayout(searchLayout);
}

void UserManagementWidget::setupUserForm()
{
    userFormGroup = new QGroupBox("用户信息", this);
    formLayout = new QGridLayout(userFormGroup);
    
    // 基本信息第一行
    formLayout->addWidget(new QLabel("用户ID:", this), 0, 0);
    userIdEdit = new QLineEdit(this);
    userIdEdit->setEnabled(false);
    formLayout->addWidget(userIdEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("用户名:", this), 0, 2);
    usernameEdit = new QLineEdit(this);
    formLayout->addWidget(usernameEdit, 0, 3);
    
    // 基本信息第二行
    formLayout->addWidget(new QLabel("密码:", this), 1, 0);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addWidget(passwordEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("邮箱:", this), 1, 2);
    emailEdit = new QLineEdit(this);
    formLayout->addWidget(emailEdit, 1, 3);
    
    // 联系信息
    formLayout->addWidget(new QLabel("电话:", this), 2, 0);
    phoneEdit = new QLineEdit(this);
    formLayout->addWidget(phoneEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("姓名:", this), 2, 2);
    firstNameEdit = new QLineEdit(this);
    formLayout->addWidget(firstNameEdit, 2, 3);
    
    formLayout->addWidget(new QLabel("姓氏:", this), 3, 0);
    lastNameEdit = new QLineEdit(this);
    formLayout->addWidget(lastNameEdit, 3, 1);
    
    formLayout->addWidget(new QLabel("出生日期:", this), 3, 2);
    birthDateEdit = new QDateEdit(QDate(1990, 1, 1), this);
    birthDateEdit->setCalendarPopup(true);
    formLayout->addWidget(birthDateEdit, 3, 3);
    
    // 其他信息
    formLayout->addWidget(new QLabel("性别:", this), 4, 0);
    genderCombo = new QComboBox(this);
    genderCombo->addItems({"男", "女", "其他"});
    formLayout->addWidget(genderCombo, 4, 1);
    
    formLayout->addWidget(new QLabel("角色:", this), 4, 2);
    roleCombo = new QComboBox(this);
    roleCombo->addItems({"普通用户", "管理员", "VIP用户", "客服"});
    formLayout->addWidget(roleCombo, 4, 3);
    
    formLayout->addWidget(new QLabel("状态:", this), 5, 0);
    statusCombo = new QComboBox(this);
    statusCombo->addItems({"活跃", "暂停", "禁用"});
    formLayout->addWidget(statusCombo, 5, 1);
    
    formLayout->addWidget(new QLabel("地址:", this), 5, 2);
    addressEdit = new QTextEdit(this);
    addressEdit->setMaximumHeight(60);
    formLayout->addWidget(addressEdit, 6, 0, 1, 4);
    
    enableUserForm(false);
    mainLayout->addWidget(userFormGroup);
}

void UserManagementWidget::setupUserTable()
{
    userCountLabel = new QLabel("用户总数: 0", this);
    mainLayout->addWidget(userCountLabel);
    
    userTable = new QTableWidget(this);
    userTable->setColumnCount(8);
    
    QStringList headers;
    headers << "用户ID" << "用户名" << "姓名" << "邮箱" << "电话" << "角色" << "状态" << "注册时间";
    userTable->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setSelectionMode(QAbstractItemView::SingleSelection);
    userTable->setAlternatingRowColors(true);
    userTable->horizontalHeader()->setStretchLastSection(true);
    userTable->verticalHeader()->setVisible(false);
    
    // 设置列宽
    userTable->setColumnWidth(0, 80);  // 用户ID
    userTable->setColumnWidth(1, 120); // 用户名
    userTable->setColumnWidth(2, 100); // 姓名
    userTable->setColumnWidth(3, 180); // 邮箱
    userTable->setColumnWidth(4, 120); // 电话
    userTable->setColumnWidth(5, 100); // 角色
    userTable->setColumnWidth(6, 80);  // 状态
    userTable->setColumnWidth(7, 150); // 注册时间
    
    mainLayout->addWidget(userTable);
}

void UserManagementWidget::connectSignals()
{
    connect(addButton, &QPushButton::clicked, this, &UserManagementWidget::addUser);
    connect(editButton, &QPushButton::clicked, this, &UserManagementWidget::editUser);
    connect(deleteButton, &QPushButton::clicked, this, &UserManagementWidget::deleteUser);
    connect(saveButton, &QPushButton::clicked, this, &UserManagementWidget::saveUser);
    connect(cancelButton, &QPushButton::clicked, this, &UserManagementWidget::cancelEdit);
    
    connect(searchButton, &QPushButton::clicked, this, &UserManagementWidget::searchUsers);
    connect(clearSearchButton, &QPushButton::clicked, this, &UserManagementWidget::clearSearch);
    connect(userTable, &QTableWidget::cellClicked, this, &UserManagementWidget::onUserSelected);
}

void UserManagementWidget::addUser()
{
    clearUserForm();
    enableUserForm(true);
    isEditing = false;
    currentEditRow = -1;
    
    // 生成新的用户ID
    static int nextUserId = 1001;
    userIdEdit->setText(QString::number(nextUserId++));
    
    // 切换按钮状态
    addButton->setVisible(false);
    editButton->setVisible(false);
    deleteButton->setVisible(false);
    saveButton->setVisible(true);
    cancelButton->setVisible(true);
}

void UserManagementWidget::editUser()
{
    int currentRow = userTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "选择用户", "请先选择要编辑的用户");
        return;
    }
    
    isEditing = true;
    currentEditRow = currentRow;
    enableUserForm(true);
    
    // 加载用户数据到表单
    userIdEdit->setText(userTable->item(currentRow, 0)->text());
    usernameEdit->setText(userTable->item(currentRow, 1)->text());
    emailEdit->setText(userTable->item(currentRow, 3)->text());
    phoneEdit->setText(userTable->item(currentRow, 4)->text());
    roleCombo->setCurrentText(userTable->item(currentRow, 5)->text());
    statusCombo->setCurrentText(userTable->item(currentRow, 6)->text());
    
    // 切换按钮状态
    addButton->setVisible(false);
    editButton->setVisible(false);
    deleteButton->setVisible(false);
    saveButton->setVisible(true);
    cancelButton->setVisible(true);
}

void UserManagementWidget::deleteUser()
{
    int currentRow = userTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "选择用户", "请先选择要删除的用户");
        return;
    }
    
    QString username = userTable->item(currentRow, 1)->text();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", 
        QString("确定要删除用户 '%1' 吗？").arg(username),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        userTable->removeRow(currentRow);
        updateUserTable();
        QMessageBox::information(this, "删除成功", "用户已删除");
    }
}

void UserManagementWidget::saveUser()
{
    validateUserForm();
    
    if (isEditing && currentEditRow >= 0) {
        // 更新现有用户
        userTable->setItem(currentEditRow, 0, new QTableWidgetItem(userIdEdit->text()));
        userTable->setItem(currentEditRow, 1, new QTableWidgetItem(usernameEdit->text()));
        userTable->setItem(currentEditRow, 2, new QTableWidgetItem(firstNameEdit->text() + " " + lastNameEdit->text()));
        userTable->setItem(currentEditRow, 3, new QTableWidgetItem(emailEdit->text()));
        userTable->setItem(currentEditRow, 4, new QTableWidgetItem(phoneEdit->text()));
        userTable->setItem(currentEditRow, 5, new QTableWidgetItem(roleCombo->currentText()));
        userTable->setItem(currentEditRow, 6, new QTableWidgetItem(statusCombo->currentText()));
        userTable->setItem(currentEditRow, 7, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    } else {
        // 添加新用户
        int row = userTable->rowCount();
        userTable->insertRow(row);
        
        userTable->setItem(row, 0, new QTableWidgetItem(userIdEdit->text()));
        userTable->setItem(row, 1, new QTableWidgetItem(usernameEdit->text()));
        userTable->setItem(row, 2, new QTableWidgetItem(firstNameEdit->text() + " " + lastNameEdit->text()));
        userTable->setItem(row, 3, new QTableWidgetItem(emailEdit->text()));
        userTable->setItem(row, 4, new QTableWidgetItem(phoneEdit->text()));
        userTable->setItem(row, 5, new QTableWidgetItem(roleCombo->currentText()));
        userTable->setItem(row, 6, new QTableWidgetItem(statusCombo->currentText()));
        userTable->setItem(row, 7, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
    
    updateUserTable();
    cancelEdit();
    QMessageBox::information(this, "保存成功", "用户信息已保存");
}

void UserManagementWidget::cancelEdit()
{
    clearUserForm();
    enableUserForm(false);
    isEditing = false;
    currentEditRow = -1;
    
    // 恢复按钮状态
    addButton->setVisible(true);
    editButton->setVisible(true);
    deleteButton->setVisible(true);
    saveButton->setVisible(false);
    cancelButton->setVisible(false);
}

void UserManagementWidget::searchUsers()
{
    QString searchText = searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        loadUsers();
        return;
    }
    
    for (int i = 0; i < userTable->rowCount(); ++i) {
        bool match = false;
        int searchColumn = searchTypeCombo->currentIndex();
        
        if (searchColumn < userTable->columnCount()) {
            QTableWidgetItem *item = userTable->item(i, searchColumn);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
            }
        }
        
        userTable->setRowHidden(i, !match);
    }
}

void UserManagementWidget::clearSearch()
{
    searchEdit->clear();
    loadUsers();
}

void UserManagementWidget::loadUsers()
{
    for (int i = 0; i < userTable->rowCount(); ++i) {
        userTable->setRowHidden(i, false);
    }
}

void UserManagementWidget::onUserSelected(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < userTable->rowCount()) {
        // 可以在这里加载用户详细信息
    }
}

void UserManagementWidget::loadSampleUsers()
{
    // 清空现有数据
    userTable->setRowCount(0);
    
    // 模拟用户数据
    QList<QStringList> sampleUsers = {
        {"1001", "admin", "管理员", "admin@flight.com", "13800138001", "管理员", "活跃", "2024-01-01 10:00:00"},
        {"1002", "zhangsan", "张三", "zhangsan@email.com", "13800138002", "普通用户", "活跃", "2024-01-15 14:30:00"},
        {"1003", "lisi", "李四", "lisi@email.com", "13800138003", "VIP用户", "活跃", "2024-02-01 09:15:00"},
        {"1004", "wangwu", "王五", "wangwu@email.com", "13800138004", "普通用户", "暂停", "2024-02-15 16:45:00"},
        {"1005", "zhaoliu", "赵六", "zhaoliu@email.com", "13800138005", "客服", "活跃", "2024-03-01 11:20:00"},
        {"1006", "qianqi", "钱七", "qianqi@email.com", "13800138006", "VIP用户", "活跃", "2024-03-15 13:10:00"},
        {"1007", "sunba", "孙八", "sunba@email.com", "13800138007", "普通用户", "禁用", "2024-03-20 15:30:00"},
        {"1008", "zhoujiu", "周九", "zhoujiu@email.com", "13800138008", "管理员", "活跃", "2024-04-01 08:45:00"}
    };
    
    for (const QStringList &user : sampleUsers) {
        int row = userTable->rowCount();
        userTable->insertRow(row);
        
        for (int col = 0; col < user.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(user[col]);
            item->setTextAlignment(Qt::AlignCenter);
            
            // 根据状态设置颜色
            if (col == 6) { // 状态列
                if (user[col] == "活跃") {
                    item->setForeground(QColor(0, 128, 0)); // 绿色
                } else if (user[col] == "暂停") {
                    item->setForeground(QColor(255, 165, 0)); // 橙色
                } else if (user[col] == "禁用") {
                    item->setForeground(QColor(255, 0, 0)); // 红色
                }
            }
            
            userTable->setItem(row, col, item);
        }
    }
    
    updateUserTable();
}

void UserManagementWidget::validateUserForm()
{
    if (usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "验证失败", "用户名不能为空");
        return;
    }
    
    if (emailEdit->text().isEmpty()) {
        QMessageBox::warning(this, "验证失败", "邮箱不能为空");
        return;
    }
    
    if (phoneEdit->text().isEmpty()) {
        QMessageBox::warning(this, "验证失败", "电话不能为空");
        return;
    }
}

void UserManagementWidget::updateUserTable()
{
    userCountLabel->setText(QString("用户总数: %1").arg(userTable->rowCount()));
}

void UserManagementWidget::clearUserForm()
{
    userIdEdit->clear();
    usernameEdit->clear();
    passwordEdit->clear();
    emailEdit->clear();
    phoneEdit->clear();
    firstNameEdit->clear();
    lastNameEdit->clear();
    birthDateEdit->setDate(QDate(1990, 1, 1));
    genderCombo->setCurrentIndex(0);
    roleCombo->setCurrentIndex(0);
    statusCombo->setCurrentIndex(0);
    addressEdit->clear();
}

void UserManagementWidget::enableUserForm(bool enabled)
{
    usernameEdit->setEnabled(enabled);
    passwordEdit->setEnabled(enabled);
    emailEdit->setEnabled(enabled);
    phoneEdit->setEnabled(enabled);
    firstNameEdit->setEnabled(enabled);
    lastNameEdit->setEnabled(enabled);
    birthDateEdit->setEnabled(enabled);
    genderCombo->setEnabled(enabled);
    roleCombo->setEnabled(enabled);
    statusCombo->setEnabled(enabled);
    addressEdit->setEnabled(enabled);
}

void UserManagementWidget::applyStyles()
{
    QString styleSheet = R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 8px;
            margin: 10px 0;
            padding-top: 15px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 20px;
            padding: 0 10px 0 10px;
        }
        QLineEdit, QComboBox, QDateEdit {
            padding: 8px;
            border: 1px solid #cccccc;
            border-radius: 4px;
            font-size: 14px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus {
            border-color: #4a90e2;
            outline: none;
        }
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #357abd;
        }
        QPushButton:pressed {
            background-color: #2968a3;
        }
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
        QTableWidget {
            gridline-color: #e0e0e0;
            background-color: white;
            alternate-background-color: #f9f9f9;
            selection-background-color: #4a90e2;
        }
        QTableWidget::item {
            padding: 8px;
        }
        QTableWidget::item:selected {
            color: white;
        }
        QHeaderView::section {
            background-color: #f0f0f0;
            padding: 8px;
            border: 1px solid #e0e0e0;
            font-weight: bold;
        }
        QTextEdit {
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 8px;
        }
    )";
    
    setStyleSheet(styleSheet);
}
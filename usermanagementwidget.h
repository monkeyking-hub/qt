#ifndef USERMANAGEMENTWIDGET_H
#define USERMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QProgressBar>

class UserManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserManagementWidget(QWidget *parent = nullptr);

private slots:
    void addUser();
    void editUser();
    void deleteUser();
    void searchUsers();
    void clearSearch();
    void loadUsers();
    void onUserSelected(int row, int column);
    void saveUser();
    void cancelEdit();

private:
    void setupUI();
    void setupUserForm();
    void setupUserTable();
    void setupSearchBar();
    void connectSignals();
    void loadSampleUsers();
    void validateUserForm();
    void updateUserTable();
    void clearUserForm();
    void enableUserForm(bool enabled);
    
    // 用户表单组件
    QGroupBox *userFormGroup;
    QLineEdit *userIdEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QDateEdit *birthDateEdit;
    QComboBox *genderCombo;
    QComboBox *roleCombo;
    QComboBox *statusCombo;
    QTextEdit *addressEdit;
    
    // 搜索组件
    QLineEdit *searchEdit;
    QComboBox *searchTypeCombo;
    QPushButton *searchButton;
    QPushButton *clearSearchButton;
    
    // 用户表格
    QTableWidget *userTable;
    QLabel *userCountLabel;
    
    // 按钮
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    
    // 布局
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *searchLayout;
    QGridLayout *formLayout;
    
    // 状态
    bool isEditing;
    int currentEditRow;
    
    // 样式设置
    void applyStyles();
};

#endif // USERMANAGEMENTWIDGET_H
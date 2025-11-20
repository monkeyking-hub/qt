#ifndef FLIGHTBOOKINGWIDGET_H
#define FLIGHTBOOKINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QMessageBox>

class FlightBookingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlightBookingWidget(QWidget *parent = nullptr);

private slots:
    void selectFlight();
    void bookFlight();
    void clearForm();
    void calculateTotalPrice();
    void onPaymentMethodChanged();

private:
    void setupUI();
    void setupFlightSelection();
    void setupPassengerInfo();
    void setupPaymentInfo();
    void setupBookingSummary();
    void connectSignals();
    void loadAvailableFlights();
    void validateBooking();
    
    // 航班选择区域
    QGroupBox *flightSelectionGroup;
    QComboBox *flightCombo;
    QLabel *flightDetailsLabel;
    QPushButton *selectFlightButton;
    
    // 乘客信息区域
    QGroupBox *passengerInfoGroup;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QLineEdit *idNumberEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QDateEdit *birthDateEdit;
    QComboBox *genderCombo;
    QSpinBox *adultCountSpin;
    QSpinBox *childCountSpin;
    QSpinBox *infantCountSpin;
    
    // 支付信息区域
    QGroupBox *paymentInfoGroup;
    QComboBox *paymentMethodCombo;
    QLineEdit *cardNumberEdit;
    QLineEdit *cardHolderEdit;
    QLineEdit *expiryDateEdit;
    QLineEdit *cvvEdit;
    QCheckBox *insuranceCheckBox;
    QCheckBox *mealCheckBox;
    QCheckBox *seatCheckBox;
    
    // 预订摘要区域
    QGroupBox *bookingSummaryGroup;
    QTextEdit *summaryTextEdit;
    QLabel *basePriceLabel;
    QLabel *taxesLabel;
    QLabel *insuranceLabel;
    QLabel *totalPriceLabel;
    
    // 按钮
    QPushButton *bookButton;
    QPushButton *clearButton;
    QPushButton *cancelButton;
    
    // 布局
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QGridLayout *flightLayout;
    QGridLayout *passengerLayout;
    QGridLayout *paymentLayout;
    QVBoxLayout *summaryLayout;
    
    // 数据
    QMap<QString, QStringList> flightData;
    double basePrice;
    double taxes;
    double insuranceFee;
    double totalPrice;
    
    // 样式设置
    void applyStyles();
};

#endif // FLIGHTBOOKINGWIDGET_H
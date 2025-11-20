#include "flightbookingwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QSplitter>
#include <QScrollArea>

FlightBookingWidget::FlightBookingWidget(QWidget *parent)
    : QWidget(parent)
    , basePrice(0.0)
    , taxes(0.0)
    , insuranceFee(0.0)
    , totalPrice(0.0)
{
    setupUI();
    connectSignals();
    loadAvailableFlights();
    applyStyles();
}

void FlightBookingWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // 设置标题
    QLabel *titleLabel = new QLabel("航班预订", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0;");
    mainLayout->addWidget(titleLabel);
    
    // 创建滚动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    
    setupFlightSelection();
    setupPassengerInfo();
    setupPaymentInfo();
    setupBookingSummary();
    
    scrollLayout->addWidget(flightSelectionGroup);
    scrollLayout->addWidget(passengerInfoGroup);
    scrollLayout->addWidget(paymentInfoGroup);
    scrollLayout->addWidget(bookingSummaryGroup);
    
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
    
    // 按钮
    buttonLayout = new QHBoxLayout();
    bookButton = new QPushButton("确认预订", this);
    clearButton = new QPushButton("清除表单", this);
    cancelButton = new QPushButton("取消预订", this);
    
    buttonLayout->addWidget(bookButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void FlightBookingWidget::setupFlightSelection()
{
    flightSelectionGroup = new QGroupBox("选择航班", this);
    flightLayout = new QGridLayout(flightSelectionGroup);
    
    QLabel *flightLabel = new QLabel("可用航班:", this);
    flightCombo = new QComboBox(this);
    flightLayout->addWidget(flightLabel, 0, 0);
    flightLayout->addWidget(flightCombo, 0, 1);
    
    flightDetailsLabel = new QLabel("请选择航班", this);
    flightDetailsLabel->setStyleSheet("padding: 10px; background-color: #f0f0f0; border-radius: 4px;");
    flightLayout->addWidget(flightDetailsLabel, 1, 0, 1, 2);
    
    selectFlightButton = new QPushButton("查看详情", this);
    flightLayout->addWidget(selectFlightButton, 2, 0, 1, 2);
}

void FlightBookingWidget::setupPassengerInfo()
{
    passengerInfoGroup = new QGroupBox("乘客信息", this);
    passengerLayout = new QGridLayout(passengerInfoGroup);
    
    // 姓名
    passengerLayout->addWidget(new QLabel("姓名:", this), 0, 0);
    firstNameEdit = new QLineEdit(this);
    firstNameEdit->setPlaceholderText("名");
    passengerLayout->addWidget(firstNameEdit, 0, 1);
    
    passengerLayout->addWidget(new QLabel("姓氏:", this), 0, 2);
    lastNameEdit = new QLineEdit(this);
    lastNameEdit->setPlaceholderText("姓");
    passengerLayout->addWidget(lastNameEdit, 0, 3);
    
    // 身份证号
    passengerLayout->addWidget(new QLabel("身份证号:", this), 1, 0);
    idNumberEdit = new QLineEdit(this);
    idNumberEdit->setPlaceholderText("请输入身份证号码");
    passengerLayout->addWidget(idNumberEdit, 1, 1, 1, 3);
    
    // 联系方式
    passengerLayout->addWidget(new QLabel("手机号码:", this), 2, 0);
    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("请输入手机号码");
    passengerLayout->addWidget(phoneEdit, 2, 1);
    
    passengerLayout->addWidget(new QLabel("邮箱:", this), 2, 2);
    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("请输入邮箱地址");
    passengerLayout->addWidget(emailEdit, 2, 3);
    
    // 出生日期和性别
    passengerLayout->addWidget(new QLabel("出生日期:", this), 3, 0);
    birthDateEdit = new QDateEdit(QDate(1990, 1, 1), this);
    birthDateEdit->setCalendarPopup(true);
    passengerLayout->addWidget(birthDateEdit, 3, 1);
    
    passengerLayout->addWidget(new QLabel("性别:", this), 3, 2);
    genderCombo = new QComboBox(this);
    genderCombo->addItems({"男", "女"});
    passengerLayout->addWidget(genderCombo, 3, 3);
    
    // 乘客人数
    passengerLayout->addWidget(new QLabel("成人数量:", this), 4, 0);
    adultCountSpin = new QSpinBox(this);
    adultCountSpin->setMinimum(1);
    adultCountSpin->setMaximum(9);
    adultCountSpin->setValue(1);
    passengerLayout->addWidget(adultCountSpin, 4, 1);
    
    passengerLayout->addWidget(new QLabel("儿童数量:", this), 4, 2);
    childCountSpin = new QSpinBox(this);
    childCountSpin->setMinimum(0);
    childCountSpin->setMaximum(9);
    passengerLayout->addWidget(childCountSpin, 4, 3);
    
    passengerLayout->addWidget(new QLabel("婴儿数量:", this), 5, 0);
    infantCountSpin = new QSpinBox(this);
    infantCountSpin->setMinimum(0);
    infantCountSpin->setMaximum(9);
    passengerLayout->addWidget(infantCountSpin, 5, 1);
}

void FlightBookingWidget::setupPaymentInfo()
{
    paymentInfoGroup = new QGroupBox("支付信息", this);
    paymentLayout = new QGridLayout(paymentInfoGroup);
    
    // 支付方式
    paymentLayout->addWidget(new QLabel("支付方式:", this), 0, 0);
    paymentMethodCombo = new QComboBox(this);
    paymentMethodCombo->addItems({"信用卡", "借记卡", "支付宝", "微信支付", "银行转账"});
    paymentLayout->addWidget(paymentMethodCombo, 0, 1, 1, 3);
    
    // 卡片信息
    paymentLayout->addWidget(new QLabel("卡号:", this), 1, 0);
    cardNumberEdit = new QLineEdit(this);
    cardNumberEdit->setPlaceholderText("请输入卡号");
    paymentLayout->addWidget(cardNumberEdit, 1, 1, 1, 3);
    
    paymentLayout->addWidget(new QLabel("持卡人姓名:", this), 2, 0);
    cardHolderEdit = new QLineEdit(this);
    cardHolderEdit->setPlaceholderText("请输入持卡人姓名");
    paymentLayout->addWidget(cardHolderEdit, 2, 1, 1, 3);
    
    paymentLayout->addWidget(new QLabel("有效期:", this), 3, 0);
    expiryDateEdit = new QLineEdit(this);
    expiryDateEdit->setPlaceholderText("MM/YY");
    paymentLayout->addWidget(expiryDateEdit, 3, 1);
    
    paymentLayout->addWidget(new QLabel("CVV:", this), 3, 2);
    cvvEdit = new QLineEdit(this);
    cvvEdit->setPlaceholderText("CVV");
    cvvEdit->setEchoMode(QLineEdit::Password);
    paymentLayout->addWidget(cvvEdit, 3, 3);
    
    // 附加服务
    paymentLayout->addWidget(new QLabel("附加服务:", this), 4, 0, 3, 1);
    
    insuranceCheckBox = new QCheckBox("航班保险 (¥200)", this);
    paymentLayout->addWidget(insuranceCheckBox, 4, 1, 1, 3);
    
    mealCheckBox = new QCheckBox("特殊餐食 (¥50)", this);
    paymentLayout->addWidget(mealCheckBox, 5, 1, 1, 3);
    
    seatCheckBox = new QCheckBox("优选座位 (¥100)", this);
    paymentLayout->addWidget(seatCheckBox, 6, 1, 1, 3);
}

void FlightBookingWidget::setupBookingSummary()
{
    bookingSummaryGroup = new QGroupBox("预订摘要", this);
    summaryLayout = new QVBoxLayout(bookingSummaryGroup);
    
    summaryTextEdit = new QTextEdit(this);
    summaryTextEdit->setReadOnly(true);
    summaryTextEdit->setMaximumHeight(200);
    summaryLayout->addWidget(summaryTextEdit);
    
    // 价格明细
    QHBoxLayout *priceLayout = new QHBoxLayout();
    
    priceLayout->addWidget(new QLabel("基础价格:", this));
    basePriceLabel = new QLabel("¥0", this);
    basePriceLabel->setAlignment(Qt::AlignRight);
    priceLayout->addWidget(basePriceLabel);
    
    priceLayout->addWidget(new QLabel("税费:", this));
    taxesLabel = new QLabel("¥0", this);
    taxesLabel->setAlignment(Qt::AlignRight);
    priceLayout->addWidget(taxesLabel);
    
    priceLayout->addWidget(new QLabel("保险费用:", this));
    insuranceLabel = new QLabel("¥0", this);
    insuranceLabel->setAlignment(Qt::AlignRight);
    priceLayout->addWidget(insuranceLabel);
    
    priceLayout->addWidget(new QLabel("总计:", this));
    totalPriceLabel = new QLabel("¥0", this);
    totalPriceLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #e74c3c;");
    totalPriceLabel->setAlignment(Qt::AlignRight);
    priceLayout->addWidget(totalPriceLabel);
    
    summaryLayout->addLayout(priceLayout);
}

void FlightBookingWidget::connectSignals()
{
    connect(selectFlightButton, &QPushButton::clicked, this, &FlightBookingWidget::selectFlight);
    connect(bookButton, &QPushButton::clicked, this, &FlightBookingWidget::bookFlight);
    connect(clearButton, &QPushButton::clicked, this, &FlightBookingWidget::clearForm);
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, "取消", "预订已取消");
    });
    
    connect(paymentMethodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FlightBookingWidget::onPaymentMethodChanged);
    
    connect(adultCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FlightBookingWidget::calculateTotalPrice);
    connect(childCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FlightBookingWidget::calculateTotalPrice);
    connect(infantCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &FlightBookingWidget::calculateTotalPrice);
    
    connect(insuranceCheckBox, &QCheckBox::toggled, this, &FlightBookingWidget::calculateTotalPrice);
    connect(mealCheckBox, &QCheckBox::toggled, this, &FlightBookingWidget::calculateTotalPrice);
    connect(seatCheckBox, &QCheckBox::toggled, this, &FlightBookingWidget::calculateTotalPrice);
}

void FlightBookingWidget::loadAvailableFlights()
{
    // 模拟航班数据
    flightData["CA1234 - 北京到上海 - ¥1,280"] = {"CA1234", "北京", "上海", "08:00", "10:30", "中国国际航空", "1280"};
    flightData["MU5678 - 北京到上海 - ¥1,150"] = {"MU5678", "北京", "上海", "09:15", "11:45", "东方航空", "1150"};
    flightData["CZ9012 - 北京到上海 - ¥1,320"] = {"CZ9012", "北京", "上海", "10:30", "13:00", "南方航空", "1320"};
    flightData["HU3456 - 北京到上海 - ¥1,080"] = {"HU3456", "北京", "上海", "11:45", "14:15", "海南航空", "1080"};
    
    flightCombo->addItems(flightData.keys());
}

void FlightBookingWidget::selectFlight()
{
    QString selectedFlight = flightCombo->currentText();
    if (flightData.contains(selectedFlight)) {
        QStringList details = flightData[selectedFlight];
        QString detailText = QString("航班号: %1\n航线: %2 - %3\n时间: %4 - %5\n航空公司: %6\n价格: ¥%7")
                           .arg(details[0], details[1], details[2], details[3], details[4], details[5], details[6]);
        flightDetailsLabel->setText(detailText);
        
        basePrice = details[6].toDouble();
        calculateTotalPrice();
    }
}

void FlightBookingWidget::bookFlight()
{
    validateBooking();
    
    QString summary = generateBookingSummary();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认预订", 
        summary + "\n\n确认预订此航班吗？",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "预订成功", "您的航班预订已成功！\n预订确认邮件将发送到您的邮箱。");
        clearForm();
    }
}

void FlightBookingWidget::clearForm()
{
    firstNameEdit->clear();
    lastNameEdit->clear();
    idNumberEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
    birthDateEdit->setDate(QDate(1990, 1, 1));
    genderCombo->setCurrentIndex(0);
    adultCountSpin->setValue(1);
    childCountSpin->setValue(0);
    infantCountSpin->setValue(0);
    
    cardNumberEdit->clear();
    cardHolderEdit->clear();
    expiryDateEdit->clear();
    cvvEdit->clear();
    
    insuranceCheckBox->setChecked(false);
    mealCheckBox->setChecked(false);
    seatCheckBox->setChecked(false);
    
    flightCombo->setCurrentIndex(0);
    flightDetailsLabel->setText("请选择航班");
    
    basePrice = 0.0;
    calculateTotalPrice();
}

void FlightBookingWidget::calculateTotalPrice()
{
    if (basePrice > 0) {
        int totalPassengers = adultCountSpin->value() + childCountSpin->value() + infantCountSpin->value();
        double adultPrice = basePrice * adultCountSpin->value();
        double childPrice = basePrice * 0.5 * childCountSpin->value();
        double infantPrice = basePrice * 0.1 * infantCountSpin->value();
        
        basePrice = adultPrice + childPrice + infantPrice;
        taxes = basePrice * 0.1;
        insuranceFee = insuranceCheckBox->isChecked() ? 200.0 : 0.0;
        
        totalPrice = basePrice + taxes + insuranceFee;
        
        if (mealCheckBox->isChecked()) {
            totalPrice += 50.0 * totalPassengers;
        }
        if (seatCheckBox->isChecked()) {
            totalPrice += 100.0 * totalPassengers;
        }
        
        basePriceLabel->setText(QString("¥%1").arg(basePrice, 0, 'f', 2));
        taxesLabel->setText(QString("¥%1").arg(taxes, 0, 'f', 2));
        insuranceLabel->setText(QString("¥%1").arg(insuranceFee, 0, 'f', 2));
        totalPriceLabel->setText(QString("¥%1").arg(totalPrice, 0, 'f', 2));
        
        updateBookingSummary();
    }
}

void FlightBookingWidget::onPaymentMethodChanged()
{
    bool isCardPayment = (paymentMethodCombo->currentIndex() == 0 || paymentMethodCombo->currentIndex() == 1);
    cardNumberEdit->setEnabled(isCardPayment);
    cardHolderEdit->setEnabled(isCardPayment);
    expiryDateEdit->setEnabled(isCardPayment);
    cvvEdit->setEnabled(isCardPayment);
}

void FlightBookingWidget::validateBooking()
{
    if (firstNameEdit->text().isEmpty() || lastNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "信息不完整", "请填写完整的姓名");
        return;
    }
    
    if (idNumberEdit->text().isEmpty()) {
        QMessageBox::warning(this, "信息不完整", "请填写身份证号码");
        return;
    }
    
    if (phoneEdit->text().isEmpty()) {
        QMessageBox::warning(this, "信息不完整", "请填写手机号码");
        return;
    }
    
    if (emailEdit->text().isEmpty()) {
        QMessageBox::warning(this, "信息不完整", "请填写邮箱地址");
        return;
    }
    
    if (flightCombo->currentIndex() == 0) {
        QMessageBox::warning(this, "信息不完整", "请选择航班");
        return;
    }
}

QString FlightBookingWidget::generateBookingSummary()
{
    QString summary = "=== 预订摘要 ===\n";
    summary += QString("航班: %1\n").arg(flightCombo->currentText());
    summary += QString("乘客: %1 %2\n").arg(firstNameEdit->text(), lastNameEdit->text());
    summary += QString("身份证: %1\n").arg(idNumberEdit->text());
    summary += QString("电话: %1\n").arg(phoneEdit->text());
    summary += QString("邮箱: %1\n").arg(emailEdit->text());
    summary += QString("成人: %1, 儿童: %2, 婴儿: %3\n")
               .arg(adultCountSpin->value())
               .arg(childCountSpin->value())
               .arg(infantCountSpin->value());
    summary += QString("支付方式: %1\n").arg(paymentMethodCombo->currentText());
    summary += QString("总计: ¥%1").arg(totalPrice, 0, 'f', 2);
    
    return summary;
}

void FlightBookingWidget::updateBookingSummary()
{
    summaryTextEdit->setText(generateBookingSummary());
}

void FlightBookingWidget::applyStyles()
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
        QLineEdit, QComboBox, QDateEdit, QSpinBox {
            padding: 8px;
            border: 1px solid #cccccc;
            border-radius: 4px;
            font-size: 14px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus {
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
        QTextEdit {
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 8px;
            background-color: #f9f9f9;
        }
        QCheckBox {
            spacing: 10px;
            font-size: 14px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #cccccc;
            border-radius: 3px;
        }
        QCheckBox::indicator:checked {
            background-color: #4a90e2;
            border-color: #4a90e2;
        }
    )";
    
    setStyleSheet(styleSheet);
}
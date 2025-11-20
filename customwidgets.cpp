#include "customwidgets.h"
#include <QPainter>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QJsonDocument>
#include <QStyleOption>
#include <QApplication>

// ModernButton 实现
ModernButton::ModernButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , m_backgroundColor("#4a90e2")
    , m_hoverColor("#357abd")
    , m_currentColor(m_backgroundColor)
    , m_isHovered(false)
{
    setStyleSheet("ModernButton { border: none; border-radius: 6px; color: white; font-weight: bold; }");
}

void ModernButton::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    if (!m_isHovered) {
        m_currentColor = color;
        update();
    }
}

void ModernButton::setHoverColor(const QColor &color)
{
    m_hoverColor = color;
    if (m_isHovered) {
        m_currentColor = color;
        update();
    }
}

QColor ModernButton::backgroundColor() const
{
    return m_backgroundColor;
}

QColor ModernButton::hoverColor() const
{
    return m_hoverColor;
}

void ModernButton::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    m_currentColor = m_hoverColor;
    update();
    QPushButton::enterEvent(event);
}

void ModernButton::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    m_currentColor = m_backgroundColor;
    update();
    QPushButton::leaveEvent(event);
}

void ModernButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.setBrush(m_currentColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 6, 6);
    
    // 绘制文本
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(rect(), Qt::AlignCenter, text());
    
    QPushButton::paintEvent(event);
}

// StatusCard 实现
StatusCard::StatusCard(const QString &title, const QString &value, const QString &icon, QWidget *parent)
    : QFrame(parent), m_icon(icon), m_color("#4a90e2")
{
    setupUI();
    setTitle(title);
    setValue(value);
    setIcon(icon);
}

void StatusCard::setupUI()
{
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    
    iconLabel = new QLabel(this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 32px; margin-bottom: 10px;");
    
    titleLabel = new QLabel(this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 14px; color: #666666; margin-bottom: 5px;");
    
    valueLabel = new QLabel(this);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    
    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    
    setStyleSheet(
        "StatusCard {"
        "background-color: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "padding: 20px;"
        "}"
    );
}

void StatusCard::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void StatusCard::setValue(const QString &value)
{
    valueLabel->setText(value);
}

void StatusCard::setIcon(const QString &icon)
{
    m_icon = icon;
    iconLabel->setText(icon);
}

void StatusCard::setColor(const QColor &color)
{
    m_color = color;
    valueLabel->setStyleSheet(QString("font-size: 24px; font-weight: bold; color: %1;").arg(color.name()));
}

// AnimatedProgressBar 实现
AnimatedProgressBar::AnimatedProgressBar(QWidget *parent)
    : QProgressBar(parent)
    , animationTimer(new QTimer(this))
    , targetValue(0)
    , currentValue(0)
    , animationStep(1)
{
    connect(animationTimer, &QTimer::timeout, this, &AnimatedProgressBar::updateAnimation);
}

void AnimatedProgressBar::startAnimation()
{
    animationTimer->start(20);
}

void AnimatedProgressBar::stopAnimation()
{
    animationTimer->stop();
}

void AnimatedProgressBar::setAnimatedValue(int value)
{
    targetValue = value;
    currentValue = this->value();
    startAnimation();
}

void AnimatedProgressBar::updateAnimation()
{
    if (currentValue < targetValue) {
        currentValue = qMin(currentValue + animationStep, targetValue);
        setValue(currentValue);
    } else if (currentValue > targetValue) {
        currentValue = qMax(currentValue - animationStep, targetValue);
        setValue(currentValue);
    } else {
        stopAnimation();
    }
}

// FlightStatusIndicator 实现
FlightStatusWidget::FlightStatusWidget(QWidget *parent)
    : QWidget(parent)
    , m_status(OnTime)
    , m_statusText("准点")
{
    setFixedSize(100, 30);
}

void FlightStatusWidget::setStatus(Status status)
{
    m_status = status;
    switch (status) {
        case OnTime: m_statusText = "准点"; break;
        case Delayed: m_statusText = "延误"; break;
        case Cancelled: m_statusText = "取消"; break;
        case Boarding: m_statusText = "登机中"; break;
        case Departed: m_statusText = "已起飞"; break;
        case Arrived: m_statusText = "已到达"; break;
    }
    update();
}

void FlightStatusWidget::setStatusText(const QString &text)
{
    m_statusText = text;
    update();
}

FlightStatusWidget::Status FlightStatusWidget::getStatus() const
{
    return m_status;
}

void FlightStatusWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QColor color = getStatusColor(m_status);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
    
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(rect(), Qt::AlignCenter, m_statusText);
    
    QWidget::paintEvent(event);
}

QColor FlightStatusWidget::getStatusColor(Status status)
{
    switch (status) {
        case OnTime: return QColor(76, 175, 80);
        case Delayed: return QColor(255, 152, 0);
        case Cancelled: return QColor(244, 67, 54);
        case Boarding: return QColor(33, 150, 243);
        case Departed: return QColor(156, 39, 176);
        case Arrived: return QColor(0, 188, 212);
        default: return QColor(158, 158, 158);
    }
}

// LoadingWidget 实现
LoadingWidget::LoadingWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
    , m_loadingText(text)
    , animationTimer(new QTimer(this))
    , m_animationAngle(0)
    , m_isLoading(false)
{
    setupUI();
    connect(animationTimer, &QTimer::timeout, this, &LoadingWidget::updateAnimation);
}

void LoadingWidget::setupUI()
{
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    
    textLabel = new QLabel(m_loadingText, this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("font-size: 14px; color: #666666;");
    
    layout->addWidget(textLabel);
    
    setFixedSize(200, 100);
}

void LoadingWidget::setLoadingText(const QString &text)
{
    m_loadingText = text;
    textLabel->setText(text);
}

void LoadingWidget::start()
{
    m_isLoading = true;
    animationTimer->start(50);
    show();
}

void LoadingWidget::stop()
{
    m_isLoading = false;
    animationTimer->stop();
    hide();
}

bool LoadingWidget::isLoading() const
{
    return m_isLoading;
}

void LoadingWidget::paintEvent(QPaintEvent *event)
{
    if (!m_isLoading) {
        QWidget::paintEvent(event);
        return;
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制旋转的圆圈
    int centerX = width() / 2;
    int centerY = height() / 2 - 20;
    int radius = 20;
    
    painter.translate(centerX, centerY);
    painter.rotate(m_animationAngle);
    
    QPen pen(QColor(74, 144, 226), 3);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    // 绘制部分圆弧
    int startAngle = 0;
    int spanAngle = 270 * 16;
    painter.drawArc(-radius, -radius, radius * 2, radius * 2, startAngle, spanAngle);
    
    painter.rotate(-m_animationAngle);
    painter.translate(-centerX, -centerY);
    
    QWidget::paintEvent(event);
}

void LoadingWidget::updateAnimation()
{
    m_animationAngle = (m_animationAngle + 10) % 360;
    update();
}

// ModernSearchBox 实现
ModernSearchBox::ModernSearchBox(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void ModernSearchBox::setupUI()
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("搜索...");
    searchEdit->setStyleSheet(
        "QLineEdit {"
        "border: 1px solid #e0e0e0;"
        "border-radius: 4px;"
        "padding: 8px;"
        "font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "border-color: #4a90e2;"
        "}"
    );
    
    searchButton = new QPushButton("🔍", this);
    searchButton->setFixedSize(30, 30);
    searchButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background-color: #4a90e2;"
        "color: white;"
        "border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "background-color: #357abd;"
        "}"
    );
    
    clearButton = new QPushButton("✕", this);
    clearButton->setFixedSize(30, 30);
    clearButton->setVisible(false);
    clearButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background-color: #f0f0f0;"
        "color: #666666;"
        "border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "background-color: #e0e0e0;"
        "}"
    );
    
    layout->addWidget(searchEdit);
    layout->addWidget(clearButton);
    layout->addWidget(searchButton);
    
    connect(searchEdit, &QLineEdit::textChanged, this, &ModernSearchBox::onTextChanged);
    connect(searchButton, &QPushButton::clicked, this, &ModernSearchBox::onSearchClicked);
    connect(clearButton, &QPushButton::clicked, searchEdit, &QLineEdit::clear);
    connect(searchEdit, &QLineEdit::returnPressed, this, &ModernSearchBox::onSearchClicked);
}

void ModernSearchBox::setPlaceholderText(const QString &text)
{
    searchEdit->setPlaceholderText(text);
}

QString ModernSearchBox::text() const
{
    return searchEdit->text();
}

void ModernSearchBox::clear()
{
    searchEdit->clear();
}

void ModernSearchBox::onTextChanged(const QString &text)
{
    clearButton->setVisible(!text.isEmpty());
    emit textChanged(text);
}

void ModernSearchBox::onSearchClicked()
{
    emit searchRequested(searchEdit->text());
    emit returnPressed();
}

// FlightInfoCard 实现
FlightInfoCard::FlightInfoCard(const QJsonObject &flightData, QWidget *parent)
    : QFrame(parent), m_flightData(flightData), m_isHovered(false)
{
    setupUI();
    updateDisplay();
}

void FlightInfoCard::setupUI()
{
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    
    flightNumberLabel = new QLabel(this);
    flightNumberLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333;");
    
    airlineLabel = new QLabel(this);
    airlineLabel->setStyleSheet("font-size: 14px; color: #666666;");
    
    routeLabel = new QLabel(this);
    routeLabel->setStyleSheet("font-size: 16px; color: #333333; margin: 10px 0;");
    
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("font-size: 14px; color: #666666;");
    
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px 10px; border-radius: 4px;");
    
    layout->addWidget(flightNumberLabel);
    layout->addWidget(airlineLabel);
    layout->addWidget(routeLabel);
    layout->addWidget(timeLabel);
    layout->addWidget(statusLabel);
    
    setStyleSheet(
        "FlightInfoCard {"
        "background-color: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "margin: 5px;"
        "}"
        "FlightInfoCard:hover {"
        "border-color: #4a90e2;"
        "background-color: #f8f9fa;"
        "}"
    );
    
    setCursor(Qt::PointingHandCursor);
}

void FlightInfoCard::setFlightData(const QJsonObject &data)
{
    m_flightData = data;
    updateDisplay();
}

QJsonObject FlightInfoCard::flightData() const
{
    return m_flightData;
}

void FlightInfoCard::updateDisplay()
{
    flightNumberLabel->setText(m_flightData["flight_number"].toString());
    airlineLabel->setText(m_flightData["airline"].toString());
    routeLabel->setText(QString("%1 → %2").arg(m_flightData["departure"].toString(), m_flightData["destination"].toString()));
    timeLabel->setText(QString("%1 - %2").arg(m_flightData["departure_time"].toString(), m_flightData["arrival_time"].toString()));
    
    QString status = m_flightData["status"].toString();
    statusLabel->setText(status);
    
    if (status == "准点") {
        statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px 10px; border-radius: 4px; background-color: #e8f5e8; color: #2e7d32;");
    } else if (status == "延误") {
        statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px 10px; border-radius: 4px; background-color: #fff3e0; color: #f57c00;");
    } else if (status == "取消") {
        statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px 10px; border-radius: 4px; background-color: #ffebee; color: #c62828;");
    }
}

void FlightInfoCard::mousePressEvent(QMouseEvent *event)
{
    emit cardClicked(m_flightData["flight_number"].toString());
    QFrame::mousePressEvent(event);
}

void FlightInfoCard::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    update();
    QFrame::enterEvent(event);
}

void FlightInfoCard::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    update();
    QFrame::leaveEvent(event);
}
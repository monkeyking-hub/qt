#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

// 自定义按钮组件
class ModernButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor)

public:
    explicit ModernButton(const QString &text = "", QWidget *parent = nullptr);
    
    void setBackgroundColor(const QColor &color);
    void setHoverColor(const QColor &color);
    QColor backgroundColor() const;
    QColor hoverColor() const;

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_backgroundColor;
    QColor m_hoverColor;
    QColor m_currentColor;
    bool m_isHovered;
};

// 自定义状态卡片组件
class StatusCard : public QFrame
{
    Q_OBJECT

public:
    explicit StatusCard(const QString &title, const QString &value, const QString &icon = "", QWidget *parent = nullptr);
    
    void setTitle(const QString &title);
    void setValue(const QString &value);
    void setIcon(const QString &icon);
    void setColor(const QColor &color);

private:
    QLabel *titleLabel;
    QLabel *valueLabel;
    QLabel *iconLabel;
    QVBoxLayout *layout;
    QString m_icon;
    QColor m_color;
    
    void setupUI();
};

// 自定义进度条组件
class AnimatedProgressBar : public QProgressBar
{
    Q_OBJECT

public:
    explicit AnimatedProgressBar(QWidget *parent = nullptr);
    
    void startAnimation();
    void stopAnimation();
    void setAnimatedValue(int value);

private slots:
    void updateAnimation();

private:
    QTimer *animationTimer;
    int targetValue;
    int currentValue;
    int animationStep;
};

// 自定义航班状态指示器
class FlightStatusIndicator : public QWidget
{
    Q_OBJECT

public:
    enum Status {
        OnTime,
        Delayed,
        Cancelled,
        Boarding,
        Departed,
        Arrived
    };
    
    explicit FlightStatusWidget(QWidget *parent = nullptr);
    
    void setStatus(Status status);
    void setStatusText(const QString &text);
    Status getStatus() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Status m_status;
    QString m_statusText;
    QColor getStatusColor(Status status);
};

// 自定义加载动画组件
class LoadingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingWidget(const QString &text = "加载中...", QWidget *parent = nullptr);
    
    void setLoadingText(const QString &text);
    void start();
    void stop();
    bool isLoading() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    QString m_loadingText;
    QTimer *animationTimer;
    int m_animationAngle;
    bool m_isLoading;
    QLabel *textLabel;
    QVBoxLayout *layout;
};

// 自定义搜索框组件
class ModernSearchBox : public QWidget
{
    Q_OBJECT

public:
    explicit ModernSearchBox(QWidget *parent = nullptr);
    
    void setPlaceholderText(const QString &text);
    QString text() const;
    void clear();

signals:
    void textChanged(const QString &text);
    void searchRequested(const QString &text);
    void returnPressed();

private slots:
    void onTextChanged(const QString &text);
    void onSearchClicked();

private:
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QHBoxLayout *layout;
    
    void setupUI();
};

// 自定义航班信息卡片
class FlightInfoCard : public QFrame
{
    Q_OBJECT

public:
    explicit FlightInfoCard(const QJsonObject &flightData, QWidget *parent = nullptr);
    
    void setFlightData(const QJsonObject &data);
    QJsonObject flightData() const;

signals:
    void cardClicked(const QString &flightNumber);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QJsonObject m_flightData;
    QLabel *flightNumberLabel;
    QLabel *airlineLabel;
    QLabel *routeLabel;
    QLabel *timeLabel;
    QLabel *statusLabel;
    QVBoxLayout *layout;
    bool m_isHovered;
    
    void setupUI();
    void updateDisplay();
};

#endif // CUSTOMWIDGETS_H
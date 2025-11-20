#ifndef FLIGHTDETAILSWIDGET_H
#define FLIGHTDETAILSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QProgressBar>
#include <QCalendarWidget>
#include <QTabWidget>

class FlightDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlightDetailsWidget(QWidget *parent = nullptr);

private slots:
    void loadFlightDetails();
    void refreshFlights();
    void searchFlightDetails();
    void clearSearch();
    void onFlightSelected(int row, int column);
    void exportFlightData();
    void printFlightDetails();

private:
    void setupUI();
    void setupSearchBar();
    void setupFlightTable();
    void setupDetailTabs();
    void setupStatusBar();
    void connectSignals();
    void loadSampleFlights();
    void updateFlightInfo(const QString &flightNumber);
    void updateFlightStatistics();
    void showFlightOnMap(const QString &flightNumber);
    
    // 搜索组件
    QComboBox *searchTypeCombo;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QPushButton *refreshButton;
    QComboBox *dateFilterCombo;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    
    // 航班表格
    QTableWidget *flightTable;
    QLabel *flightCountLabel;
    
    // 详细信息标签页
    QTabWidget *detailTabs;
    QWidget *basicInfoTab;
    QWidget *routeInfoTab;
    QWidget *passengerInfoTab;
    QWidget *statusInfoTab;
    
    // 基本信息组件
    QLabel *flightNumberLabel;
    QLabel *airlineLabel;
    QLabel *aircraftLabel;
    QLabel *departureTimeLabel;
    QLabel *arrivalTimeLabel;
    QLabel *gateLabel;
    QLabel *terminalLabel;
    QTextEdit *remarksEdit;
    
    // 航线信息组件
    QLabel *departureAirportLabel;
    QLabel *arrivalAirportLabel;
    QLabel *distanceLabel;
    QLabel *durationLabel;
    QLabel *altitudeLabel;
    QLabel *speedLabel;
    
    // 乘客信息组件
    QTableWidget *passengerTable;
    QLabel *passengerCountLabel;
    QLabel *loadFactorLabel;
    QProgressBar *loadProgressBar;
    
    // 状态信息组件
    QLabel *statusLabel;
    QLabel *delayLabel;
    QLabel *cancelReasonLabel;
    QTextEdit *statusHistoryEdit;
    
    // 按钮
    QPushButton *exportButton;
    QPushButton *printButton;
    QPushButton *mapButton;
    
    // 布局
    QVBoxLayout *mainLayout;
    QHBoxLayout *searchLayout;
    QHBoxLayout *buttonLayout;
    QGridLayout *basicInfoLayout;
    QGridLayout *routeInfoLayout;
    QVBoxLayout *passengerLayout;
    QVBoxLayout *statusLayout;
    
    // 数据
    QMap<QString, QStringList> flightDetails;
    
    // 样式设置
    void applyStyles();
};

#endif // FLIGHTDETAILSWIDGET_H
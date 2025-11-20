#ifndef FLIGHTSEARCHWIDGET_H
#define FLIGHTSEARCHWIDGET_H

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
#include <QProgressBar>
#include <QTimer>

class FlightSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlightSearchWidget(QWidget *parent = nullptr);

private slots:
    void searchFlights();
    void clearSearch();
    void onFlightSelected(int row, int column);
    void onSearchComplete();
    void updateSearchProgress();

private:
    void setupUI();
    void setupSearchForm();
    void setupResultsTable();
    void setupStatusBar();
    void connectSignals();
    void loadSampleData();
    
    // 搜索表单组件
    QGroupBox *searchGroupBox;
    QLineEdit *departureEdit;
    QLineEdit *destinationEdit;
    QDateEdit *departureDateEdit;
    QDateEdit *returnDateEdit;
    QComboBox *tripTypeCombo;
    QComboBox *passengerCountCombo;
    QComboBox *classTypeCombo;
    QPushButton *searchButton;
    QPushButton *clearButton;
    
    // 结果表格
    QTableWidget *resultsTable;
    QLabel *resultsLabel;
    QProgressBar *searchProgressBar;
    
    // 布局
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QGridLayout *searchLayout;
    
    // 搜索状态
    bool isSearching;
    QTimer *searchTimer;
    
    // 样式设置
    void applyStyles();
};

#endif // FLIGHTSEARCHWIDGET_H
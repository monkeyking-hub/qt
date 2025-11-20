#include "flightsearchwidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidgetItem>
#include <QDebug>

FlightSearchWidget::FlightSearchWidget(QWidget *parent)
    : QWidget(parent)
    , isSearching(false)
    , searchTimer(nullptr)
{
    setupUI();
    connectSignals();
    loadSampleData();
    applyStyles();
}

void FlightSearchWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // 设置标题
    QLabel *titleLabel = new QLabel("航班查询", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0;");
    mainLayout->addWidget(titleLabel);
    
    setupSearchForm();
    setupResultsTable();
    setupStatusBar();
}

void FlightSearchWidget::setupSearchForm()
{
    searchGroupBox = new QGroupBox("搜索条件", this);
    searchLayout = new QGridLayout(searchGroupBox);
    
    // 出发地
    QLabel *departureLabel = new QLabel("出发地:", this);
    departureEdit = new QLineEdit(this);
    departureEdit->setPlaceholderText("请输入出发城市");
    searchLayout->addWidget(departureLabel, 0, 0);
    searchLayout->addWidget(departureEdit, 0, 1);
    
    // 目的地
    QLabel *destinationLabel = new QLabel("目的地:", this);
    destinationEdit = new QLineEdit(this);
    destinationEdit->setPlaceholderText("请输入目的地城市");
    searchLayout->addWidget(destinationLabel, 0, 2);
    searchLayout->addWidget(destinationEdit, 0, 3);
    
    // 航班类型
    QLabel *tripTypeLabel = new QLabel("航班类型:", this);
    tripTypeCombo = new QComboBox(this);
    tripTypeCombo->addItems({"单程", "往返", "多程"});
    searchLayout->addWidget(tripTypeLabel, 1, 0);
    searchLayout->addWidget(tripTypeCombo, 1, 1);
    
    // 出发日期
    QLabel *departureDateLabel = new QLabel("出发日期:", this);
    departureDateEdit = new QDateEdit(QDate::currentDate(), this);
    departureDateEdit->setCalendarPopup(true);
    searchLayout->addWidget(departureDateLabel, 1, 2);
    searchLayout->addWidget(departureDateEdit, 1, 3);
    
    // 返程日期
    QLabel *returnDateLabel = new QLabel("返程日期:", this);
    returnDateEdit = new QDateEdit(QDate::currentDate().addDays(7), this);
    returnDateEdit->setCalendarPopup(true);
    returnDateEdit->setEnabled(false); // 默认禁用，单程时不需要
    searchLayout->addWidget(returnDateLabel, 2, 0);
    searchLayout->addWidget(returnDateEdit, 2, 1);
    
    // 乘客人数
    QLabel *passengerCountLabel = new QLabel("乘客人数:", this);
    passengerCountCombo = new QComboBox(this);
    passengerCountCombo->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9"});
    searchLayout->addWidget(passengerCountLabel, 2, 2);
    searchLayout->addWidget(passengerCountCombo, 2, 3);
    
    // 舱位等级
    QLabel *classTypeLabel = new QLabel("舱位等级:", this);
    classTypeCombo = new QComboBox(this);
    classTypeCombo->addItems({"经济舱", "商务舱", "头等舱"});
    searchLayout->addWidget(classTypeLabel, 3, 0);
    searchLayout->addWidget(classTypeCombo, 3, 1);
    
    // 按钮
    buttonLayout = new QHBoxLayout();
    searchButton = new QPushButton("搜索航班", this);
    clearButton = new QPushButton("清除条件", this);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    
    searchLayout->addLayout(buttonLayout, 4, 0, 1, 4);
    
    mainLayout->addWidget(searchGroupBox);
    
    // 连接航班类型变化信号
    connect(tripTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                returnDateEdit->setEnabled(index == 1); // 往返时启用返程日期
            });
}

void FlightSearchWidget::setupResultsTable()
{
    resultsLabel = new QLabel("搜索结果: 0 个航班", this);
    mainLayout->addWidget(resultsLabel);
    
    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(8);
    
    QStringList headers;
    headers << "航班号" << "出发地" << "目的地" << "出发时间" << "到达时间" 
            << "航空公司" << "价格" << "状态";
    resultsTable->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    resultsTable->setAlternatingRowColors(true);
    resultsTable->horizontalHeader()->setStretchLastSection(true);
    resultsTable->verticalHeader()->setVisible(false);
    
    // 设置列宽
    resultsTable->setColumnWidth(0, 100); // 航班号
    resultsTable->setColumnWidth(1, 120); // 出发地
    resultsTable->setColumnWidth(2, 120); // 目的地
    resultsTable->setColumnWidth(3, 150); // 出发时间
    resultsTable->setColumnWidth(4, 150); // 到达时间
    resultsTable->setColumnWidth(5, 120); // 航空公司
    resultsTable->setColumnWidth(6, 100); // 价格
    resultsTable->setColumnWidth(7, 80);  // 状态
    
    mainLayout->addWidget(resultsTable);
}

void FlightSearchWidget::setupStatusBar()
{
    searchProgressBar = new QProgressBar(this);
    searchProgressBar->setVisible(false);
    searchProgressBar->setRange(0, 0); // 不确定进度
    mainLayout->addWidget(searchProgressBar);
}

void FlightSearchWidget::connectSignals()
{
    connect(searchButton, &QPushButton::clicked, this, &FlightSearchWidget::searchFlights);
    connect(clearButton, &QPushButton::clicked, this, &FlightSearchWidget::clearSearch);
    connect(resultsTable, &QTableWidget::cellClicked, this, &FlightSearchWidget::onFlightSelected);
}

void FlightSearchWidget::searchFlights()
{
    if (isSearching) {
        return;
    }
    
    // 验证输入
    if (departureEdit->text().isEmpty() || destinationEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请填写出发地和目的地");
        return;
    }
    
    if (departureEdit->text() == destinationEdit->text()) {
        QMessageBox::warning(this, "输入错误", "出发地和目的地不能相同");
        return;
    }
    
    isSearching = true;
    searchButton->setEnabled(false);
    searchProgressBar->setVisible(true);
    resultsLabel->setText("正在搜索...");
    
    // 模拟搜索延迟
    searchTimer = new QTimer(this);
    connect(searchTimer, &QTimer::timeout, this, &FlightSearchWidget::onSearchComplete);
    searchTimer->start(2000); // 2秒后完成搜索
}

void FlightSearchWidget::onSearchComplete()
{
    isSearching = false;
    searchButton->setEnabled(true);
    searchProgressBar->setVisible(false);
    
    // 模拟搜索结果
    loadSampleData();
    
    resultsLabel->setText(QString("搜索结果: %1 个航班").arg(resultsTable->rowCount()));
    
    if (searchTimer) {
        searchTimer->deleteLater();
        searchTimer = nullptr;
    }
}

void FlightSearchWidget::clearSearch()
{
    departureEdit->clear();
    destinationEdit->clear();
    departureDateEdit->setDate(QDate::currentDate());
    returnDateEdit->setDate(QDate::currentDate().addDays(7));
    tripTypeCombo->setCurrentIndex(0);
    passengerCountCombo->setCurrentIndex(0);
    classTypeCombo->setCurrentIndex(0);
    
    resultsTable->setRowCount(0);
    resultsLabel->setText("搜索结果: 0 个航班");
}

void FlightSearchWidget::onFlightSelected(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < resultsTable->rowCount()) {
        QTableWidgetItem *item = resultsTable->item(row, 0);
        if (item) {
            QString flightNumber = item->text();
            QMessageBox::information(this, "航班选择", 
                                   QString("您选择了航班: %1").arg(flightNumber));
        }
    }
}

void FlightSearchWidget::loadSampleData()
{
    // 清空现有数据
    resultsTable->setRowCount(0);
    
    // 模拟航班数据
    QList<QStringList> sampleFlights = {
        {"CA1234", "北京", "上海", "08:00", "10:30", "中国国际航空", "¥1,280", "准点"},
        {"MU5678", "北京", "上海", "09:15", "11:45", "东方航空", "¥1,150", "准点"},
        {"CZ9012", "北京", "上海", "10:30", "13:00", "南方航空", "¥1,320", "延误"},
        {"HU3456", "北京", "上海", "11:45", "14:15", "海南航空", "¥1,080", "准点"},
        {"FM7890", "北京", "上海", "13:00", "15:30", "上海航空", "¥1,200", "准点"},
        {"JD2345", "北京", "上海", "14:15", "16:45", "首都航空", "¥980", "准点"},
        {"3U6789", "北京", "上海", "15:30", "18:00", "四川航空", "¥1,100", "准点"},
        {"ZH1234", "北京", "上海", "16:45", "19:15", "深圳航空", "¥1,250", "准点"}
    };
    
    for (const QStringList &flight : sampleFlights) {
        int row = resultsTable->rowCount();
        resultsTable->insertRow(row);
        
        for (int col = 0; col < flight.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(flight[col]);
            item->setTextAlignment(Qt::AlignCenter);
            
            // 根据状态设置颜色
            if (col == 7) { // 状态列
                if (flight[col] == "延误") {
                    item->setForeground(QColor(255, 100, 100)); // 红色
                } else {
                    item->setForeground(QColor(100, 255, 100)); // 绿色
                }
            }
            
            resultsTable->setItem(row, col, item);
        }
    }
}

void FlightSearchWidget::updateSearchProgress()
{
    // 更新搜索进度（如果需要的话）
}

void FlightSearchWidget::applyStyles()
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
        QProgressBar {
            border: 1px solid #cccccc;
            border-radius: 4px;
            text-align: center;
        }
        QProgressBar::chunk {
            background-color: #4a90e2;
            border-radius: 3px;
        }
    )";
    
    setStyleSheet(styleSheet);
}
#include "flightdetailswidget.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QSplitter>
#include <QScrollArea>

FlightDetailsWidget::FlightDetailsWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    connectSignals();
    loadSampleFlights();
    applyStyles();
}

void FlightDetailsWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // 设置标题
    QLabel *titleLabel = new QLabel("航班详情", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0;");
    mainLayout->addWidget(titleLabel);
    
    setupSearchBar();
    setupFlightTable();
    setupDetailTabs();
    setupStatusBar();
    
    // 按钮
    buttonLayout = new QHBoxLayout();
    exportButton = new QPushButton("导出数据", this);
    printButton = new QPushButton("打印详情", this);
    mapButton = new QPushButton("查看地图", this);
    
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(printButton);
    buttonLayout->addWidget(mapButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void FlightDetailsWidget::setupSearchBar()
{
    searchLayout = new QHBoxLayout();
    
    searchLayout->addWidget(new QLabel("搜索类型:", this));
    searchTypeCombo = new QComboBox(this);
    searchTypeCombo->addItems({"航班号", "航空公司", "出发地", "目的地", "状态"});
    searchLayout->addWidget(searchTypeCombo);
    
    searchLayout->addWidget(new QLabel("关键词:", this));
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("输入搜索关键词");
    searchLayout->addWidget(searchEdit);
    
    searchLayout->addWidget(new QLabel("日期范围:", this));
    startDateEdit = new QDateEdit(QDate::currentDate().addDays(-7), this);
    searchLayout->addWidget(startDateEdit);
    
    searchLayout->addWidget(new QLabel("至", this));
    endDateEdit = new QDateEdit(QDate::currentDate(), this);
    searchLayout->addWidget(endDateEdit);
    
    searchButton = new QPushButton("搜索", this);
    clearButton = new QPushButton("清除", this);
    refreshButton = new QPushButton("刷新", this);
    
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearButton);
    searchLayout->addWidget(refreshButton);
    searchLayout->addStretch();
    
    mainLayout->addLayout(searchLayout);
}

void FlightDetailsWidget::setupFlightTable()
{
    flightCountLabel = new QLabel("航班总数: 0", this);
    mainLayout->addWidget(flightCountLabel);
    
    flightTable = new QTableWidget(this);
    flightTable->setColumnCount(10);
    
    QStringList headers;
    headers << "航班号" << "航空公司" << "出发地" << "目的地" << "计划起飞" 
            << "实际起飞" << "计划到达" << "实际到达" << "状态" << "登机口";
    flightTable->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    flightTable->setSelectionMode(QAbstractItemView::SingleSelection);
    flightTable->setAlternatingRowColors(true);
    flightTable->horizontalHeader()->setStretchLastSection(true);
    flightTable->verticalHeader()->setVisible(false);
    
    // 设置列宽
    flightTable->setColumnWidth(0, 80);  // 航班号
    flightTable->setColumnWidth(1, 120); // 航空公司
    flightTable->setColumnWidth(2, 100); // 出发地
    flightTable->setColumnWidth(3, 100); // 目的地
    flightTable->setColumnWidth(4, 130); // 计划起飞
    flightTable->setColumnWidth(5, 130); // 实际起飞
    flightTable->setColumnWidth(6, 130); // 计划到达
    flightTable->setColumnWidth(7, 130); // 实际到达
    flightTable->setColumnWidth(8, 80);  // 状态
    flightTable->setColumnWidth(9, 80);  // 登机口
    
    mainLayout->addWidget(flightTable);
}

void FlightDetailsWidget::setupDetailTabs()
{
    detailTabs = new QTabWidget(this);
    
    // 基本信息标签页
    basicInfoTab = new QWidget();
    basicInfoLayout = new QGridLayout(basicInfoTab);
    
    int row = 0;
    basicInfoLayout->addWidget(new QLabel("航班号:"), row, 0);
    flightNumberLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(flightNumberLabel, row, 1);
    
    basicInfoLayout->addWidget(new QLabel("航空公司:"), row, 2);
    airlineLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(airlineLabel, row, 3);
    
    row++;
    basicInfoLayout->addWidget(new QLabel("机型:"), row, 0);
    aircraftLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(aircraftLabel, row, 1);
    
    basicInfoLayout->addWidget(new QLabel("登机口:"), row, 2);
    gateLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(gateLabel, row, 3);
    
    row++;
    basicInfoLayout->addWidget(new QLabel("航站楼:"), row, 0);
    terminalLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(terminalLabel, row, 1);
    
    basicInfoLayout->addWidget(new QLabel("状态:"), row, 2);
    statusLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(statusLabel, row, 3);
    
    row++;
    basicInfoLayout->addWidget(new QLabel("计划起飞:"), row, 0);
    departureTimeLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(departureTimeLabel, row, 1);
    
    basicInfoLayout->addWidget(new QLabel("计划到达:"), row, 2);
    arrivalTimeLabel = new QLabel("-", this);
    basicInfoLayout->addWidget(arrivalTimeLabel, row, 3);
    
    row++;
    basicInfoLayout->addWidget(new QLabel("备注:"), row, 0);
    remarksEdit = new QTextEdit(this);
    remarksEdit->setMaximumHeight(100);
    remarksEdit->setReadOnly(true);
    basicInfoLayout->addWidget(remarksEdit, row, 1, 1, 3);
    
    detailTabs->addTab(basicInfoTab, "基本信息");
    
    // 航线信息标签页
    routeInfoTab = new QWidget();
    routeInfoLayout = new QGridLayout(routeInfoTab);
    
    row = 0;
    routeInfoLayout->addWidget(new QLabel("出发机场:"), row, 0);
    departureAirportLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(departureAirportLabel, row, 1);
    
    routeInfoLayout->addWidget(new QLabel("到达机场:"), row, 2);
    arrivalAirportLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(arrivalAirportLabel, row, 3);
    
    row++;
    routeInfoLayout->addWidget(new QLabel("飞行距离:"), row, 0);
    distanceLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(distanceLabel, row, 1);
    
    routeInfoLayout->addWidget(new QLabel("飞行时长:"), row, 2);
    durationLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(durationLabel, row, 3);
    
    row++;
    routeInfoLayout->addWidget(new QLabel("巡航高度:"), row, 0);
    altitudeLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(altitudeLabel, row, 1);
    
    routeInfoLayout->addWidget(new QLabel("巡航速度:"), row, 2);
    speedLabel = new QLabel("-", this);
    routeInfoLayout->addWidget(speedLabel, row, 3);
    
    detailTabs->addTab(routeInfoTab, "航线信息");
    
    // 乘客信息标签页
    passengerInfoTab = new QWidget();
    passengerLayout = new QVBoxLayout(passengerInfoTab);
    
    passengerCountLabel = new QLabel("乘客总数: 0", this);
    loadFactorLabel = new QLabel("载客率: 0%", this);
    loadProgressBar = new QProgressBar(this);
    
    passengerLayout->addWidget(passengerCountLabel);
    passengerLayout->addWidget(loadFactorLabel);
    passengerLayout->addWidget(loadProgressBar);
    
    passengerTable = new QTableWidget(this);
    passengerTable->setColumnCount(5);
    QStringList passengerHeaders;
    passengerHeaders << "座位号" << "姓名" << "票号" << "舱位" << "状态";
    passengerTable->setHorizontalHeaderLabels(passengerHeaders);
    passengerTable->horizontalHeader()->setStretchLastSection(true);
    passengerTable->setMaximumHeight(200);
    
    passengerLayout->addWidget(passengerTable);
    detailTabs->addTab(passengerInfoTab, "乘客信息");
    
    // 状态信息标签页
    statusInfoTab = new QWidget();
    statusLayout = new QVBoxLayout(statusInfoTab);
    
    delayLabel = new QLabel("延误: 0分钟", this);
    cancelReasonLabel = new QLabel("取消原因: -", this);
    
    statusLayout->addWidget(delayLabel);
    statusLayout->addWidget(cancelReasonLabel);
    
    statusHistoryEdit = new QTextEdit(this);
    statusHistoryEdit->setReadOnly(true);
    statusLayout->addWidget(new QLabel("状态历史:"));
    statusLayout->addWidget(statusHistoryEdit);
    
    detailTabs->addTab(statusInfoTab, "状态信息");
    
    mainLayout->addWidget(detailTabs);
}

void FlightDetailsWidget::setupStatusBar()
{
    // 可以添加状态栏组件
}

void FlightDetailsWidget::connectSignals()
{
    connect(searchButton, &QPushButton::clicked, this, &FlightDetailsWidget::searchFlightDetails);
    connect(clearButton, &QPushButton::clicked, this, &FlightDetailsWidget::clearSearch);
    connect(refreshButton, &QPushButton::clicked, this, &FlightDetailsWidget::refreshFlights);
    connect(flightTable, &QTableWidget::cellClicked, this, &FlightDetailsWidget::onFlightSelected);
    connect(exportButton, &QPushButton::clicked, this, &FlightDetailsWidget::exportFlightData);
    connect(printButton, &QPushButton::clicked, this, &FlightDetailsWidget::printFlightDetails);
    connect(mapButton, &QPushButton::clicked, this, [this]() {
        int row = flightTable->currentRow();
        if (row >= 0) {
            QString flightNumber = flightTable->item(row, 0)->text();
            showFlightOnMap(flightNumber);
        }
    });
}

void FlightDetailsWidget::loadSampleFlights()
{
    // 清空现有数据
    flightTable->setRowCount(0);
    
    // 模拟航班数据
    QList<QStringList> sampleFlights = {
        {"CA1234", "中国国际航空", "北京首都", "上海浦东", "08:00", "08:15", "10:30", "10:45", "延误", "A12"},
        {"MU5678", "东方航空", "北京首都", "上海虹桥", "09:15", "09:10", "11:45", "11:40", "准点", "B08"},
        {"CZ9012", "南方航空", "北京首都", "上海浦东", "10:30", "10:30", "13:00", "13:00", "准点", "C15"},
        {"HU3456", "海南航空", "北京首都", "上海虹桥", "11:45", "12:00", "14:15", "14:30", "延误", "D22"},
        {"FM7890", "上海航空", "北京首都", "上海浦东", "13:00", "13:00", "15:30", "15:30", "准点", "E05"},
        {"JD2345", "首都航空", "北京首都", "上海虹桥", "14:15", "14:15", "16:45", "16:45", "准点", "F18"},
        {"3U6789", "四川航空", "北京首都", "上海浦东", "15:30", "15:45", "18:00", "18:15", "延误", "G11"},
        {"ZH1234", "深圳航空", "北京首都", "上海虹桥", "16:45", "16:45", "19:15", "19:15", "准点", "H09"}
    };
    
    for (const QStringList &flight : sampleFlights) {
        int row = flightTable->rowCount();
        flightTable->insertRow(row);
        
        for (int col = 0; col < flight.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(flight[col]);
            item->setTextAlignment(Qt::AlignCenter);
            
            // 根据状态设置颜色
            if (col == 8) { // 状态列
                if (flight[col] == "延误") {
                    item->setForeground(QColor(255, 100, 100)); // 红色
                } else if (flight[col] == "准点") {
                    item->setForeground(QColor(100, 255, 100)); // 绿色
                } else if (flight[col] == "取消") {
                    item->setForeground(QColor(255, 0, 0)); // 深红色
                }
            }
            
            flightTable->setItem(row, col, item);
        }
    }
    
    flightCountLabel->setText(QString("航班总数: %1").arg(flightTable->rowCount()));
}

void FlightDetailsWidget::loadFlightDetails()
{
    int currentRow = flightTable->currentRow();
    if (currentRow < 0) {
        return;
    }
    
    QString flightNumber = flightTable->item(currentRow, 0)->text();
    updateFlightInfo(flightNumber);
}

void FlightDetailsWidget::refreshFlights()
{
    loadSampleFlights();
    QMessageBox::information(this, "刷新完成", "航班数据已刷新");
}

void FlightDetailsWidget::searchFlightDetails()
{
    QString searchText = searchEdit->text().trimmed();
    if (searchText.isEmpty()) {
        loadSampleFlights();
        return;
    }
    
    for (int i = 0; i < flightTable->rowCount(); ++i) {
        bool match = false;
        int searchColumn = searchTypeCombo->currentIndex();
        
        // 映射搜索类型到表格列
        switch (searchColumn) {
            case 0: searchColumn = 0; break; // 航班号
            case 1: searchColumn = 1; break; // 航空公司
            case 2: searchColumn = 2; break; // 出发地
            case 3: searchColumn = 3; break; // 目的地
            case 4: searchColumn = 8; break; // 状态
        }
        
        if (searchColumn < flightTable->columnCount()) {
            QTableWidgetItem *item = flightTable->item(i, searchColumn);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                match = true;
            }
        }
        
        flightTable->setRowHidden(i, !match);
    }
}

void FlightDetailsWidget::clearSearch()
{
    searchEdit->clear();
    startDateEdit->setDate(QDate::currentDate().addDays(-7));
    endDateEdit->setDate(QDate::currentDate());
    loadSampleFlights();
}

void FlightDetailsWidget::onFlightSelected(int row, int column)
{
    Q_UNUSED(column);
    
    if (row >= 0 && row < flightTable->rowCount()) {
        QString flightNumber = flightTable->item(row, 0)->text();
        updateFlightInfo(flightNumber);
    }
}

void FlightDetailsWidget::updateFlightInfo(const QString &flightNumber)
{
    // 模拟航班详细信息
    flightNumberLabel->setText(flightNumber);
    airlineLabel->setText("中国国际航空");
    aircraftLabel->setText("Boeing 737-800");
    gateLabel->setText("A12");
    terminalLabel->setText("T3");
    statusLabel->setText("延误");
    departureTimeLabel->setText("08:00");
    arrivalTimeLabel->setText("10:30");
    remarksEdit->setText("由于天气原因，航班延误15分钟起飞");
    
    departureAirportLabel->setText("北京首都国际机场 (PEK)");
    arrivalAirportLabel->setText("上海浦东国际机场 (PVG)");
    distanceLabel->setText("1,075 km");
    durationLabel->setText("2小时30分钟");
    altitudeLabel->setText("11,000 米");
    speedLabel->setText("850 km/h");
    
    // 模拟乘客数据
    passengerTable->setRowCount(0);
    QList<QStringList> passengers = {
        {"1A", "张三", "CA123401", "经济舱", "已登机"},
        {"1B", "李四", "CA123402", "经济舱", "已登机"},
        {"2A", "王五", "CA123403", "商务舱", "已登机"},
        {"2B", "赵六", "CA123404", "经济舱", "未登机"},
        {"3A", "钱七", "CA123405", "经济舱", "已登机"}
    };
    
    for (const QStringList &passenger : passengers) {
        int row = passengerTable->rowCount();
        passengerTable->insertRow(row);
        for (int col = 0; col < passenger.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(passenger[col]);
            item->setTextAlignment(Qt::AlignCenter);
            passengerTable->setItem(row, col, item);
        }
    }
    
    passengerCountLabel->setText(QString("乘客总数: %1").arg(passengers.size()));
    loadFactorLabel->setText("载客率: 65%");
    loadProgressBar->setValue(65);
    
    delayLabel->setText("延误: 15分钟");
    cancelReasonLabel->setText("取消原因: -");
    
    QString statusHistory = QString("2024-01-01 06:00 - 航班计划\n")
                          + QString("2024-01-01 07:30 - 开始值机\n")
                          + QString("2024-01-01 08:00 - 计划起飞\n")
                          + QString("2024-01-01 08:15 - 实际起飞 (延误15分钟)\n")
                          + QString("2024-01-01 10:45 - 实际到达 (延误15分钟)");
    statusHistoryEdit->setText(statusHistory);
}

void FlightDetailsWidget::updateFlightStatistics()
{
    // 更新航班统计信息
}

void FlightDetailsWidget::showFlightOnMap(const QString &flightNumber)
{
    QMessageBox::information(this, "地图视图", 
                           QString("正在显示航班 %1 的实时地图位置\n\n")
                           .arg(flightNumber) +
                           "此功能需要集成地图API来实现实时航班跟踪");
}

void FlightDetailsWidget::exportFlightData()
{
    QMessageBox::information(this, "导出数据", "航班数据已导出到 flight_data.csv");
}

void FlightDetailsWidget::printFlightDetails()
{
    QMessageBox::information(this, "打印", "航班详情已发送到打印机");
}

void FlightDetailsWidget::applyStyles()
{
    QString styleSheet = R"(
        QTabWidget::pane {
            border: 1px solid #cccccc;
            background-color: white;
        }
        QTabWidget::tab-bar {
            alignment: center;
        }
        QTabBar::tab {
            background-color: #f0f0f0;
            border: 1px solid #cccccc;
            padding: 8px 16px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background-color: #4a90e2;
            color: white;
        }
        QTabBar::tab:hover {
            background-color: #e0e0e0;
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
            background-color: #f9f9f9;
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
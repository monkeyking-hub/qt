#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>

class DatabaseHelper : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseHelper(QObject *parent = nullptr);
    ~DatabaseHelper();
    
    bool connectToDatabase(const QString &hostName, const QString &dbName, 
                          const QString &username, const QString &password);
    
    // 航班相关操作
    bool insertFlight(const QJsonObject &flightData);
    QJsonArray getFlights(const QString &departure = "", const QString &destination = "");
    QJsonObject getFlightDetails(const QString &flightNumber);
    bool updateFlightStatus(const QString &flightNumber, const QString &status);
    
    // 用户相关操作
    bool insertUser(const QJsonObject &userData);
    QJsonObject getUser(const QString &username);
    bool updateUser(const QString &userId, const QJsonObject &userData);
    bool deleteUser(const QString &userId);
    
    // 预订相关操作
    bool insertBooking(const QJsonObject &bookingData);
    QJsonArray getUserBookings(const QString &userId);
    QJsonObject getBookingDetails(const QString &bookingId);
    bool updateBookingStatus(const QString &bookingId, const QString &status);
    
    // 系统统计
    QJsonObject getFlightStatistics();
    QJsonObject getUserStatistics();
    QJsonObject getBookingStatistics();
    
    // 数据库维护
    bool createTables();
    void closeDatabase();

signals:
    void databaseConnected();
    void databaseError(const QString &error);

private:
    QSqlDatabase database;
    bool isConnected;
    
    void initializeDatabase();
    bool createFlightTable();
    bool createUserTable();
    bool createBookingTable();
    bool createPassengerTable();
    
    QVariant executeScalar(const QString &query);
    QSqlQuery executeQuery(const QString &query, const QVariantList &params = QVariantList());
};

#endif // DATABASEHELPER_H
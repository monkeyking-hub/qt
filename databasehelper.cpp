#include "databasehelper.h"
#include <QSqlRecord>
#include <QDebug>

DatabaseHelper::DatabaseHelper(QObject *parent)
    : QObject(parent)
    , isConnected(false)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseHelper::~DatabaseHelper()
{
    closeDatabase();
}

bool DatabaseHelper::connectToDatabase(const QString &hostName, const QString &dbName, 
                                     const QString &username, const QString &password)
{
    Q_UNUSED(hostName);
    Q_UNUSED(username);
    Q_UNUSED(password);
    
    database.setDatabaseName(dbName.isEmpty() ? "flightsystem.db" : dbName);
    
    if (!database.open()) {
        emit databaseError(QString("无法连接数据库: %1").arg(database.lastError().text()));
        return false;
    }
    
    isConnected = true;
    createTables();
    emit databaseConnected();
    return true;
}

bool DatabaseHelper::insertFlight(const QJsonObject &flightData)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("INSERT INTO flights (flight_number, airline, departure, destination, "
                 "departure_time, arrival_time, status, gate, aircraft) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    
    query.addBindValue(flightData["flight_number"].toString());
    query.addBindValue(flightData["airline"].toString());
    query.addBindValue(flightData["departure"].toString());
    query.addBindValue(flightData["destination"].toString());
    query.addBindValue(flightData["departure_time"].toString());
    query.addBindValue(flightData["arrival_time"].toString());
    query.addBindValue(flightData["status"].toString());
    query.addBindValue(flightData["gate"].toString());
    query.addBindValue(flightData["aircraft"].toString());
    
    return query.exec();
}

QJsonArray DatabaseHelper::getFlights(const QString &departure, const QString &destination)
{
    QJsonArray flights;
    
    if (!isConnected) return flights;
    
    QString queryString = "SELECT * FROM flights WHERE 1=1";
    QVariantList params;
    
    if (!departure.isEmpty()) {
        queryString += " AND departure = ?";
        params.append(departure);
    }
    
    if (!destination.isEmpty()) {
        queryString += " AND destination = ?";
        params.append(destination);
    }
    
    QSqlQuery query(database);
    query.prepare(queryString);
    
    for (const QVariant &param : params) {
        query.addBindValue(param);
    }
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject flight;
            QSqlRecord record = query.record();
            
            for (int i = 0; i < record.count(); ++i) {
                flight[record.fieldName(i)] = query.value(i).toString();
            }
            
            flights.append(flight);
        }
    }
    
    return flights;
}

QJsonObject DatabaseHelper::getFlightDetails(const QString &flightNumber)
{
    QJsonObject flight;
    
    if (!isConnected) return flight;
    
    QSqlQuery query(database);
    query.prepare("SELECT * FROM flights WHERE flight_number = ?");
    query.addBindValue(flightNumber);
    
    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            flight[record.fieldName(i)] = query.value(i).toString();
        }
    }
    
    return flight;
}

bool DatabaseHelper::updateFlightStatus(const QString &flightNumber, const QString &status)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("UPDATE flights SET status = ? WHERE flight_number = ?");
    query.addBindValue(status);
    query.addBindValue(flightNumber);
    
    return query.exec();
}

bool DatabaseHelper::insertUser(const QJsonObject &userData)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("INSERT INTO users (username, password, email, phone, first_name, "
                 "last_name, role, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    
    query.addBindValue(userData["username"].toString());
    query.addBindValue(userData["password"].toString());
    query.addBindValue(userData["email"].toString());
    query.addBindValue(userData["phone"].toString());
    query.addBindValue(userData["first_name"].toString());
    query.addBindValue(userData["last_name"].toString());
    query.addBindValue(userData["role"].toString());
    query.addBindValue(userData["status"].toString());
    
    return query.exec();
}

QJsonObject DatabaseHelper::getUser(const QString &username)
{
    QJsonObject user;
    
    if (!isConnected) return user;
    
    QSqlQuery query(database);
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            user[record.fieldName(i)] = query.value(i).toString();
        }
    }
    
    return user;
}

bool DatabaseHelper::updateUser(const QString &userId, const QJsonObject &userData)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("UPDATE users SET email = ?, phone = ?, first_name = ?, last_name = ?, "
                 "role = ?, status = ? WHERE id = ?");
    
    query.addBindValue(userData["email"].toString());
    query.addBindValue(userData["phone"].toString());
    query.addBindValue(userData["first_name"].toString());
    query.addBindValue(userData["last_name"].toString());
    query.addBindValue(userData["role"].toString());
    query.addBindValue(userData["status"].toString());
    query.addBindValue(userId);
    
    return query.exec();
}

bool DatabaseHelper::deleteUser(const QString &userId)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(userId);
    
    return query.exec();
}

bool DatabaseHelper::insertBooking(const QJsonObject &bookingData)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("INSERT INTO bookings (user_id, flight_number, booking_date, status, "
                 "total_price, passenger_count) VALUES (?, ?, ?, ?, ?, ?)");
    
    query.addBindValue(bookingData["user_id"].toString());
    query.addBindValue(bookingData["flight_number"].toString());
    query.addBindValue(bookingData["booking_date"].toString());
    query.addBindValue(bookingData["status"].toString());
    query.addBindValue(bookingData["total_price"].toString());
    query.addBindValue(bookingData["passenger_count"].toString());
    
    return query.exec();
}

QJsonArray DatabaseHelper::getUserBookings(const QString &userId)
{
    QJsonArray bookings;
    
    if (!isConnected) return bookings;
    
    QSqlQuery query(database);
    query.prepare("SELECT * FROM bookings WHERE user_id = ?");
    query.addBindValue(userId);
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject booking;
            QSqlRecord record = query.record();
            
            for (int i = 0; i < record.count(); ++i) {
                booking[record.fieldName(i)] = query.value(i).toString();
            }
            
            bookings.append(booking);
        }
    }
    
    return bookings;
}

QJsonObject DatabaseHelper::getBookingDetails(const QString &bookingId)
{
    QJsonObject booking;
    
    if (!isConnected) return booking;
    
    QSqlQuery query(database);
    query.prepare("SELECT * FROM bookings WHERE id = ?");
    query.addBindValue(bookingId);
    
    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            booking[record.fieldName(i)] = query.value(i).toString();
        }
    }
    
    return booking;
}

bool DatabaseHelper::updateBookingStatus(const QString &bookingId, const QString &status)
{
    if (!isConnected) return false;
    
    QSqlQuery query(database);
    query.prepare("UPDATE bookings SET status = ? WHERE id = ?");
    query.addBindValue(status);
    query.addBindValue(bookingId);
    
    return query.exec();
}

QJsonObject DatabaseHelper::getFlightStatistics()
{
    QJsonObject stats;
    
    if (!isConnected) return stats;
    
    QSqlQuery query(database);
    if (query.exec("SELECT COUNT(*) as total_flights FROM flights")) {
        if (query.next()) {
            stats["total_flights"] = query.value("total_flights").toString();
        }
    }
    
    if (query.exec("SELECT COUNT(*) as active_flights FROM flights WHERE status = '准点'")) {
        if (query.next()) {
            stats["active_flights"] = query.value("active_flights").toString();
        }
    }
    
    return stats;
}

QJsonObject DatabaseHelper::getUserStatistics()
{
    QJsonObject stats;
    
    if (!isConnected) return stats;
    
    QSqlQuery query(database);
    if (query.exec("SELECT COUNT(*) as total_users FROM users")) {
        if (query.next()) {
            stats["total_users"] = query.value("total_users").toString();
        }
    }
    
    return stats;
}

QJsonObject DatabaseHelper::getBookingStatistics()
{
    QJsonObject stats;
    
    if (!isConnected) return stats;
    
    QSqlQuery query(database);
    if (query.exec("SELECT COUNT(*) as total_bookings FROM bookings")) {
        if (query.next()) {
            stats["total_bookings"] = query.value("total_bookings").toString();
        }
    }
    
    return stats;
}

bool DatabaseHelper::createTables()
{
    return createFlightTable() && createUserTable() && createBookingTable() && createPassengerTable();
}

bool DatabaseHelper::createFlightTable()
{
    QSqlQuery query(database);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS flights ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "flight_number TEXT UNIQUE NOT NULL,"
        "airline TEXT NOT NULL,"
        "departure TEXT NOT NULL,"
        "destination TEXT NOT NULL,"
        "departure_time TEXT NOT NULL,"
        "arrival_time TEXT NOT NULL,"
        "status TEXT DEFAULT '准点',"
        "gate TEXT,"
        "aircraft TEXT,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );
}

bool DatabaseHelper::createUserTable()
{
    QSqlQuery query(database);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "phone TEXT,"
        "first_name TEXT,"
        "last_name TEXT,"
        "role TEXT DEFAULT '普通用户',"
        "status TEXT DEFAULT '活跃',"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );
}

bool DatabaseHelper::createBookingTable()
{
    QSqlQuery query(database);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS bookings ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "flight_number TEXT NOT NULL,"
        "booking_date TEXT NOT NULL,"
        "status TEXT DEFAULT '已预订',"
        "total_price REAL NOT NULL,"
        "passenger_count INTEGER DEFAULT 1,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (flight_number) REFERENCES flights(flight_number)"
        ")"
    );
}

bool DatabaseHelper::createPassengerTable()
{
    QSqlQuery query(database);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS passengers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "booking_id INTEGER NOT NULL,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "id_number TEXT NOT NULL,"
        "seat_number TEXT,"
        "class_type TEXT DEFAULT '经济舱',"
        "FOREIGN KEY (booking_id) REFERENCES bookings(id)"
        ")"
    );
}

void DatabaseHelper::closeDatabase()
{
    if (database.isOpen()) {
        database.close();
    }
    isConnected = false;
}

QVariant DatabaseHelper::executeScalar(const QString &query)
{
    QSqlQuery sqlQuery(database);
    if (sqlQuery.exec(query) && sqlQuery.next()) {
        return sqlQuery.value(0);
    }
    return QVariant();
}

QSqlQuery DatabaseHelper::executeQuery(const QString &query, const QVariantList &params)
{
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare(query);
    
    for (const QVariant &param : params) {
        sqlQuery.addBindValue(param);
    }
    
    sqlQuery.exec();
    return sqlQuery;
}
#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class APIManager : public QObject
{
    Q_OBJECT

public:
    explicit APIManager(QObject *parent = nullptr);
    
    // 航班相关API
    void searchFlights(const QString &departure, const QString &destination, const QDate &date);
    void getFlightDetails(const QString &flightNumber);
    void bookFlight(const QJsonObject &bookingData);
    
    // 用户相关API
    void loginUser(const QString &username, const QString &password);
    void registerUser(const QJsonObject &userData);
    void getUserProfile(const QString &userId);
    
    // 系统相关API
    void getSystemStatus();
    void getFlightStatistics();

signals:
    void flightSearchCompleted(const QJsonArray &flights);
    void flightDetailsReceived(const QJsonObject &details);
    void bookingCompleted(const QJsonObject &result);
    void loginCompleted(const QJsonObject &result);
    void registrationCompleted(const QJsonObject &result);
    void userProfileReceived(const QJsonObject &profile);
    void systemStatusReceived(const QJsonObject &status);
    void statisticsReceived(const QJsonObject &statistics);
    void errorOccurred(const QString &error);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString baseApiUrl;
    
    void makeApiCall(const QString &endpoint, const QJsonObject &data = QJsonObject());
    QJsonObject createRequestData(const QStringList &params);
    void parseResponse(const QByteArray &response, const QString &requestType);
};

#endif // APIMANAGER_H
#include "apimanager.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QDebug>

APIManager::APIManager(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , baseApiUrl("https://api.flightsystem.com/v1")
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &APIManager::handleNetworkReply);
}

void APIManager::searchFlights(const QString &departure, const QString &destination, const QDate &date)
{
    QJsonObject requestData;
    requestData["departure"] = departure;
    requestData["destination"] = destination;
    requestData["date"] = date.toString("yyyy-MM-dd");
    
    makeApiCall("/flights/search", requestData);
}

void APIManager::getFlightDetails(const QString &flightNumber)
{
    QString endpoint = QString("/flights/%1").arg(flightNumber);
    makeApiCall(endpoint);
}

void APIManager::bookFlight(const QJsonObject &bookingData)
{
    makeApiCall("/bookings", bookingData);
}

void APIManager::loginUser(const QString &username, const QString &password)
{
    QJsonObject requestData;
    requestData["username"] = username;
    requestData["password"] = password;
    
    makeApiCall("/auth/login", requestData);
}

void APIManager::registerUser(const QJsonObject &userData)
{
    makeApiCall("/auth/register", userData);
}

void APIManager::getUserProfile(const QString &userId)
{
    QString endpoint = QString("/users/%1").arg(userId);
    makeApiCall(endpoint);
}

void APIManager::getSystemStatus()
{
    makeApiCall("/system/status");
}

void APIManager::getFlightStatistics()
{
    makeApiCall("/statistics/flights");
}

void APIManager::makeApiCall(const QString &endpoint, const QJsonObject &data)
{
    QUrl url(baseApiUrl + endpoint);
    QNetworkRequest request(url);
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "FlightSystem/1.0");
    
    QByteArray postData;
    if (!data.isEmpty()) {
        QJsonDocument doc(data);
        postData = doc.toJson();
    }
    
    QNetworkReply *reply = networkManager->post(request, postData);
    reply->setProperty("endpoint", endpoint);
    reply->setProperty("requestData", data);
}

void APIManager::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(QString("API Error: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QString endpoint = reply->property("endpoint").toString();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit errorOccurred(QString("JSON Parse Error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    
    QJsonObject response = doc.object();
    
    // 根据不同的端点发出相应的信号
    if (endpoint.contains("/flights/search")) {
        emit flightSearchCompleted(response["flights"].toArray());
    } else if (endpoint.contains("/flights/") && !endpoint.contains("/search")) {
        emit flightDetailsReceived(response);
    } else if (endpoint.contains("/bookings")) {
        emit bookingCompleted(response);
    } else if (endpoint.contains("/auth/login")) {
        emit loginCompleted(response);
    } else if (endpoint.contains("/auth/register")) {
        emit registrationCompleted(response);
    } else if (endpoint.contains("/users/")) {
        emit userProfileReceived(response);
    } else if (endpoint.contains("/system/status")) {
        emit systemStatusReceived(response);
    } else if (endpoint.contains("/statistics/flights")) {
        emit statisticsReceived(response);
    }
    
    reply->deleteLater();
}

QJsonObject APIManager::createRequestData(const QStringList &params)
{
    QJsonObject data;
    // 根据参数创建请求数据
    return data;
}

void APIManager::parseResponse(const QByteArray &response, const QString &requestType)
{
    // 解析响应数据
    Q_UNUSED(response);
    Q_UNUSED(requestType);
}
#include "apimanager.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QDebug>
#include <QHttpMultiPart>

APIManager::APIManager(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , baseApiUrl("https://api.flightsystem.com/v1")
    , authToken("")
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &APIManager::handleNetworkReply);
}

APIManager::~APIManager()
{
    for (auto timer : requestTimers) {
        timer->deleteLater();
    }
    requestTimers.clear();
}

void APIManager::setBaseUrl(const QString &url)
{
    baseApiUrl = url;
}

void APIManager::setAuthToken(const QString &token)
{
    authToken = token;
}

QString APIManager::getAuthToken() const
{
    return authToken;
}

// 认证API
void APIManager::loginUser(const QString &username, const QString &password)
{
    QJsonObject requestData;
    requestData["username"] = username;
    requestData["password"] = password;
    makeApiCall("/auth/login", "POST", requestData, false);
}

void APIManager::registerUser(const QJsonObject &userData)
{
    makeApiCall("/auth/register", "POST", userData, false);
}

void APIManager::logoutUser()
{
    makeApiCall("/auth/logout", "POST");
    authToken = "";
}

void APIManager::refreshToken()
{
    makeApiCall("/auth/refresh", "POST");
}

// 航班搜索API
void APIManager::searchFlights(const QString &departure, const QString &destination,
                                const QDate &date, const QString &returnDate,
                                int passengers, const QString &seatClass)
{
    QJsonObject requestData;
    requestData["departure"] = departure;
    requestData["destination"] = destination;
    requestData["date"] = date.toString("yyyy-MM-dd");
    if (!returnDate.isEmpty()) {
        requestData["return_date"] = returnDate;
    }
    requestData["passengers"] = passengers;
    requestData["seat_class"] = seatClass;
    
    makeApiCall("/flights/search", "POST", requestData);
}

void APIManager::getFlightDetails(const QString &flightNumber)
{
    QString endpoint = QString("/flights/%1").arg(flightNumber);
    makeApiCall(endpoint, "GET");
}

void APIManager::getFlightSchedule(const QString &departure, const QString &destination)
{
    QString endpoint = QString("/flights/schedule?from=%1&to=%2").arg(departure, destination);
    makeApiCall(endpoint, "GET");
}

void APIManager::getAvailableSeats(const QString &flightNumber)
{
    QString endpoint = QString("/flights/%1/seats").arg(flightNumber);
    makeApiCall(endpoint, "GET");
}

// 预订API
void APIManager::bookFlight(const QJsonObject &bookingData)
{
    makeApiCall("/bookings", "POST", bookingData);
}

void APIManager::getBookingDetails(const QString &bookingId)
{
    QString endpoint = QString("/bookings/%1").arg(bookingId);
    makeApiCall(endpoint, "GET");
}

void APIManager::cancelBooking(const QString &bookingId)
{
    QString endpoint = QString("/bookings/%1/cancel").arg(bookingId);
    makeApiCall(endpoint, "POST");
}

void APIManager::getBookingHistory(const QString &userId)
{
    QString endpoint = QString("/users/%1/bookings").arg(userId);
    makeApiCall(endpoint, "GET");
}

// 用户API
void APIManager::getUserProfile(const QString &userId)
{
    QString endpoint = QString("/users/%1").arg(userId);
    makeApiCall(endpoint, "GET");
}

void APIManager::updateUserProfile(const QString &userId, const QJsonObject &userData)
{
    QString endpoint = QString("/users/%1").arg(userId);
    makeApiCall(endpoint, "PUT", userData);
}

void APIManager::getUserBookings(const QString &userId)
{
    QString endpoint = QString("/users/%1/bookings").arg(userId);
    makeApiCall(endpoint, "GET");
}

void APIManager::getUserFavorites(const QString &userId)
{
    QString endpoint = QString("/users/%1/favorites").arg(userId);
    makeApiCall(endpoint, "GET");
}

void APIManager::addFavorite(const QString &userId, const QString &flightNumber)
{
    QJsonObject data;
    data["flight_number"] = flightNumber;
    QString endpoint = QString("/users/%1/favorites").arg(userId);
    makeApiCall(endpoint, "POST", data);
}

void APIManager::removeFavorite(const QString &userId, const QString &flightNumber)
{
    QString endpoint = QString("/users/%1/favorites/%2").arg(userId, flightNumber);
    makeApiCall(endpoint, "DELETE");
}

// 评价API
void APIManager::getFlightReviews(const QString &flightNumber)
{
    QString endpoint = QString("/flights/%1/reviews").arg(flightNumber);
    makeApiCall(endpoint, "GET");
}

void APIManager::submitReview(const QString &bookingId, const QJsonObject &reviewData)
{
    QString endpoint = QString("/bookings/%1/review").arg(bookingId);
    makeApiCall(endpoint, "POST", reviewData);
}

// 支付API
void APIManager::processPayment(const QJsonObject &paymentData)
{
    makeApiCall("/payments/process", "POST", paymentData);
}

void APIManager::validatePaymentMethod(const QJsonObject &paymentInfo)
{
    makeApiCall("/payments/validate", "POST", paymentInfo);
}

// 系统API
void APIManager::getSystemStatus()
{
    makeApiCall("/system/status", "GET");
}

void APIManager::getFlightStatistics()
{
    makeApiCall("/statistics/flights", "GET");
}

void APIManager::getNotifications(const QString &userId)
{
    QString endpoint = QString("/users/%1/notifications").arg(userId);
    makeApiCall(endpoint, "GET");
}

void APIManager::markNotificationRead(const QString &notificationId)
{
    QString endpoint = QString("/notifications/%1/read").arg(notificationId);
    makeApiCall(endpoint, "POST");
}

void APIManager::makeApiCall(const QString &endpoint, const QString &method,
                             const QJsonObject &data, bool useAuth)
{
    QUrl url(baseApiUrl + endpoint);
    QNetworkRequest request(url);
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "FlightSystem/1.0");
    
    if (useAuth && !authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(authToken).toUtf8());
    }
    
    QNetworkReply *reply = nullptr;
    QByteArray postData;
    
    if (!data.isEmpty()) {
        QJsonDocument doc(data);
        postData = doc.toJson(QJsonDocument::Compact);
    }
    
    if (method == "GET") {
        reply = networkManager->get(request);
    } else if (method == "POST") {
        reply = networkManager->post(request, postData);
    } else if (method == "PUT") {
        reply = networkManager->put(request, postData);
    } else if (method == "DELETE") {
        reply = networkManager->deleteResource(request);
    }
    
    if (reply) {
        reply->setProperty("endpoint", endpoint);
        reply->setProperty("method", method);
        pendingRequests[reply] = endpoint;
        setupRequestTimer(reply);
    }
}

void APIManager::setupRequestTimer(QNetworkReply *reply)
{
    QTimer *timer = new QTimer(this);
    requestTimers[reply] = timer;
    
    connect(timer, &QTimer::timeout, this, &APIManager::handleRequestTimeout);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            timer, &QTimer::stop);
    
    timer->setSingleShot(true);
    timer->start(REQUEST_TIMEOUT_MS);
}

void APIManager::handleRequestTimeout()
{
    QTimer *timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;
    
    for (auto it = requestTimers.begin(); it != requestTimers.end(); ++it) {
        if (it.value() == timer) {
            QNetworkReply *reply = it.key();
            QString endpoint = reply->property("endpoint").toString();
            emit requestTimeout(endpoint);
            reply->abort();
            break;
        }
    }
}

void APIManager::handleNetworkReply(QNetworkReply *reply)
{
    if (requestTimers.contains(reply)) {
        requestTimers[reply]->stop();
        requestTimers[reply]->deleteLater();
        requestTimers.remove(reply);
    }
    
    pendingRequests.remove(reply);
    
    QString endpoint = reply->property("endpoint").toString();
    
    if (reply->error() != QNetworkReply::NoError) {
        if (reply->error() != QNetworkReply::OperationCancelledError) {
            emit errorOccurred(QString("API Error: %1").arg(reply->errorString()));
        }
        reply->deleteLater();
        return;
    }
    
    QByteArray responseData = reply->readAll();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit errorOccurred(QString("JSON Parse Error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    
    QJsonObject response = doc.object();
    
    // 路由响应到相应的信号
    if (endpoint.contains("/auth/login")) {
        if (response.contains("token")) {
            authToken = response["token"].toString();
        }
        emit loginCompleted(response);
    } else if (endpoint.contains("/auth/register")) {
        emit registerCompleted(response);
    } else if (endpoint.contains("/auth/logout")) {
        emit logoutCompleted(response);
    } else if (endpoint.contains("/auth/refresh")) {
        if (response.contains("token")) {
            authToken = response["token"].toString();
        }
        emit tokenRefreshed(authToken);
    } else if (endpoint.contains("/flights/search")) {
        emit flightSearchCompleted(response.contains("flights") ? response["flights"].toArray() : QJsonArray());
    } else if (endpoint.contains("/flights/") && endpoint.contains("/seats")) {
        emit seatsReceived(response.contains("seats") ? response["seats"].toArray() : QJsonArray());
    } else if (endpoint.contains("/flights/") && endpoint.contains("/reviews")) {
        emit reviewsReceived(response.contains("reviews") ? response["reviews"].toArray() : QJsonArray());
    } else if (endpoint.contains("/flights/schedule")) {
        emit flightScheduleReceived(response.contains("schedule") ? response["schedule"].toArray() : QJsonArray());
    } else if (endpoint.contains("/flights/") && !endpoint.contains("/search")) {
        emit flightDetailsReceived(response);
    } else if (endpoint.contains("/bookings/") && endpoint.contains("/review")) {
        emit reviewSubmitted(response);
    } else if (endpoint.contains("/bookings/") && endpoint.contains("/cancel")) {
        emit bookingCancelled(response);
    } else if (endpoint.contains("/bookings/")) {
        emit bookingDetailsReceived(response);
    } else if (endpoint.contains("/bookings")) {
        emit bookingCompleted(response);
    } else if (endpoint.contains("/users/") && endpoint.contains("/bookings")) {
        emit userBookingsReceived(response.contains("bookings") ? response["bookings"].toArray() : QJsonArray());
    } else if (endpoint.contains("/users/") && endpoint.contains("/favorites") && !endpoint.contains("/favorites/")) {
        if (endpoint.endsWith("/favorites")) {
            emit userFavoritesReceived(response.contains("favorites") ? response["favorites"].toArray() : QJsonArray());
        }
    } else if (endpoint.contains("/favorites")) {
        if (endpoint.count("/") == 3) {
            emit favoriteAdded(response);
        } else {
            emit favoriteRemoved(response);
        }
    } else if (endpoint.contains("/users/") && endpoint.contains("/notifications")) {
        emit notificationsReceived(response.contains("notifications") ? response["notifications"].toArray() : QJsonArray());
    } else if (endpoint.contains("/notifications/") && endpoint.contains("/read")) {
        emit notificationMarkedRead(response);
    } else if (endpoint.contains("/users/")) {
        emit userProfileReceived(response);
    } else if (endpoint.contains("/payments/process")) {
        emit paymentProcessed(response);
    } else if (endpoint.contains("/payments/validate")) {
        emit paymentValidated(response);
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
    return data;
}

void APIManager::parseResponse(const QByteArray &response, const QString &endpoint)
{
    Q_UNUSED(response);
    Q_UNUSED(endpoint);
}
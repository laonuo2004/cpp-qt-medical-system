#include "networkmanager_client.h"
#include <QDebug>
#include <QJsonParseError>
#include <QDateTime>
#include <QEventLoop>
#include <QUuid>

NetworkManagerClient::NetworkManagerClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_serverHost("localhost")
    , m_serverPort(12345)
    , m_requestCounter(0)
{
    // 连接套接字信号
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManagerClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManagerClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManagerClient::onDataReceived);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &NetworkManagerClient::onError);
    
    qDebug() << "客户端网络管理器初始化完成";
}

NetworkManagerClient::~NetworkManagerClient()
{
    disconnectFromServer();
    
    // 清理所有定时器
    for (auto timer : m_requestTimers.values()) {
        timer->deleteLater();
    }
    m_requestTimers.clear();
}

bool NetworkManagerClient::connectToServer(const QString& host, quint16 port)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qWarning() << "已经连接到服务器";
        return true;
    }
    
    m_serverHost = host;
    m_serverPort = port;
    
    qInfo() << "正在连接到服务器:" << host << ":" << port;
    m_socket->connectToHost(host, port);
    
    return true;
}

void NetworkManagerClient::disconnectFromServer()
{
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        qInfo() << "断开与服务器的连接";
        m_socket->disconnectFromHost();
    }
}

bool NetworkManagerClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

QVariantMap NetworkManagerClient::sendRequestSync(const QString& action, const QVariantMap& params, int timeout)
{
    if (!isConnected()) {
        qWarning() << "未连接到服务器，无法发送请求";
        QVariantMap errorResponse;
        errorResponse["success"] = false;
        errorResponse["error"] = "未连接到服务器";
        return errorResponse;
    }
    
    QString requestId = generateRequestId();
    QJsonObject request = createRequest(action, params);
    request["requestId"] = requestId;
    
    // 设置同步等待标记
    m_syncRequestCompleted[requestId] = false;
    m_pendingSyncResponses[requestId] = QVariantMap();
    
    // 发送请求
    sendJsonRequest(request);
    
    // 创建事件循环等待响应
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(timeout);
    
    // 连接完成信号
    connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(this, &NetworkManagerClient::responseReceived, [&](const QString& id, const QVariantMap&) {
        if (id == requestId) {
            loop.quit();
        }
    });
    
    timeoutTimer.start();
    loop.exec();
    
    // 检查是否超时
    if (!m_syncRequestCompleted.value(requestId, false)) {
        qWarning() << "请求超时:" << action;
        m_syncRequestCompleted.remove(requestId);
        m_pendingSyncResponses.remove(requestId);
        
        QVariantMap errorResponse;
        errorResponse["success"] = false;
        errorResponse["error"] = "请求超时";
        return errorResponse;
    }
    
    // 获取响应并清理
    QVariantMap response = m_pendingSyncResponses.take(requestId);
    m_syncRequestCompleted.remove(requestId);
    
    return response;
}

void NetworkManagerClient::sendRequestAsync(const QString& action, const QVariantMap& params, 
                                           std::function<void(bool, const QVariantMap&)> callback)
{
    if (!isConnected()) {
        qWarning() << "未连接到服务器，无法发送异步请求";
        if (callback) {
            QVariantMap errorResponse;
            errorResponse["error"] = "未连接到服务器";
            callback(false, errorResponse);
        }
        return;
    }
    
    QString requestId = generateRequestId();
    QJsonObject request = createRequest(action, params);
    request["requestId"] = requestId;
    
    // 存储回调
    if (callback) {
        m_asyncCallbacks[requestId] = callback;
        
        // 设置超时定时器
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(10000); // 10秒超时
        connect(timer, &QTimer::timeout, [this, requestId]() {
            onRequestTimeout();
            // 超时处理
            if (m_asyncCallbacks.contains(requestId)) {
                auto callback = m_asyncCallbacks.take(requestId);
                QVariantMap errorResponse;
                errorResponse["error"] = "请求超时";
                callback(false, errorResponse);
            }
            if (m_requestTimers.contains(requestId)) {
                m_requestTimers.take(requestId)->deleteLater();
            }
        });
        timer->start();
        m_requestTimers[requestId] = timer;
    }
    
    // 发送请求
    sendJsonRequest(request);
}

NetworkManagerClient::DataRow NetworkManagerClient::variantMapToDataRow(const QVariantMap& map)
{
    DataRow row;
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        row[it.key()] = it.value();
    }
    return row;
}

NetworkManagerClient::ResultSet NetworkManagerClient::variantListToResultSet(const QVariantList& list)
{
    ResultSet resultSet;
    for (const QVariant& item : list) {
        if (item.canConvert<QVariantMap>()) {
            resultSet.push_back(variantMapToDataRow(item.toMap()));
        }
    }
    return resultSet;
}

void NetworkManagerClient::onConnected()
{
    qInfo() << "已连接到服务器:" << m_serverHost << ":" << m_serverPort;
    emit connectionStatusChanged(true);
}

void NetworkManagerClient::onDisconnected()
{
    qInfo() << "与服务器断开连接";
    emit connectionStatusChanged(false);
    
    // 清理所有等待的请求
    for (const QString& requestId : m_asyncCallbacks.keys()) {
        auto callback = m_asyncCallbacks.take(requestId);
        QVariantMap errorResponse;
        errorResponse["error"] = "连接断开";
        callback(false, errorResponse);
    }
    
    // 清理定时器
    for (auto timer : m_requestTimers.values()) {
        timer->deleteLater();
    }
    m_requestTimers.clear();
}

void NetworkManagerClient::onDataReceived()
{
    QByteArray data = m_socket->readAll();
    
    // 解析JSON响应
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON解析错误:" << error.errorString();
        emit networkError("JSON解析错误: " + error.errorString());
        return;
    }
    
    QJsonObject response = doc.object();
    handleServerResponse(response);
}

void NetworkManagerClient::onError(QAbstractSocket::SocketError error)
{
    QString errorString = m_socket->errorString();
    qWarning() << "网络错误:" << error << errorString;
    emit networkError(errorString);
}

void NetworkManagerClient::onRequestTimeout()
{
    qWarning() << "请求超时";
}

QString NetworkManagerClient::generateRequestId()
{
    return QString("req_%1_%2").arg(++m_requestCounter).arg(QDateTime::currentMSecsSinceEpoch());
}

void NetworkManagerClient::sendJsonRequest(const QJsonObject& request)
{
    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "发送请求:" << request.value("action").toString() << "ID:" << request.value("requestId").toString();
    
    m_socket->write(data);
    m_socket->flush();
}

void NetworkManagerClient::handleServerResponse(const QJsonObject& response)
{
    QString requestId = response.value("requestId").toString();
    bool success = response.value("success").toBool();
    
    qDebug() << "收到响应:" << requestId << "成功:" << success;
    
    QVariantMap responseData = jsonObjectToVariantMap(response);
    
    // 处理同步请求响应
    if (m_syncRequestCompleted.contains(requestId)) {
        m_pendingSyncResponses[requestId] = responseData;
        m_syncRequestCompleted[requestId] = true;
    }
    
    // 处理异步请求响应
    if (m_asyncCallbacks.contains(requestId)) {
        auto callback = m_asyncCallbacks.take(requestId);
        
        // 清理定时器
        if (m_requestTimers.contains(requestId)) {
            m_requestTimers.take(requestId)->deleteLater();
        }
        
        // 调用回调
        if (callback) {
            callback(success, responseData);
        }
    }
    
    // 发出通用响应信号
    emit responseReceived(requestId, responseData);
}

QJsonObject NetworkManagerClient::createRequest(const QString& action, const QVariantMap& params)
{
    QJsonObject request;
    request["action"] = action;
    request["params"] = variantMapToJsonObject(params);
    request["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return request;
}

QJsonObject NetworkManagerClient::variantMapToJsonObject(const QVariantMap& map)
{
    QJsonObject obj;
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        obj[it.key()] = QJsonValue::fromVariant(it.value());
    }
    return obj;
}

QVariantMap NetworkManagerClient::jsonObjectToVariantMap(const QJsonObject& obj)
{
    QVariantMap map;
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        map[it.key()] = it.value().toVariant();
    }
    return map;
}

QJsonArray NetworkManagerClient::variantListToJsonArray(const QVariantList& list)
{
    QJsonArray arr;
    for (const QVariant& item : list) {
        arr.append(QJsonValue::fromVariant(item));
    }
    return arr;
}

QVariantList NetworkManagerClient::jsonArrayToVariantList(const QJsonArray& arr)
{
    QVariantList list;
    for (const QJsonValue& value : arr) {
        list.append(value.toVariant());
    }
    return list;
}

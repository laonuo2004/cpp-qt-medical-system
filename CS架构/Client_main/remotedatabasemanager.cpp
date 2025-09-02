#include "remotedatabasemanager.h"
#include<QTcpSocket>
#include <QAbstractSocket>

RemoteDatabaseManager::RemoteDatabaseManager(QObject *parent)
    : QObject(parent),
      m_socket(new QTcpSocket(this)),
      m_responseReceived(false),
      m_currentRequestId(0)
{
    connect(m_socket, &QTcpSocket::readyRead, this, &RemoteDatabaseManager::socketReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &RemoteDatabaseManager::socketDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &RemoteDatabaseManager::socketError);
}

RemoteDatabaseManager::~RemoteDatabaseManager()
{
    disconnectFromServer();
}

bool RemoteDatabaseManager::connectToServer(const QString& host, quint16 port, int timeoutMs)
{
    m_socket->connectToHost(host, port);
    if (!m_socket->waitForConnected(timeoutMs)) {
        m_lastError = m_socket->errorString();
        qCritical() << "Failed to connect to server:" << m_lastError;
        emit errorOccurred(m_lastError);
        emit connectionStatusChanged(false);
        return false;
    }
    qInfo() << "Connected to server at" << host << ":" << port;
    emit connectionStatusChanged(true);
    return true;
}

void RemoteDatabaseManager::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected();
        qInfo() << "Disconnected from server.";
        emit connectionStatusChanged(false);
    }
}

bool RemoteDatabaseManager::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void RemoteDatabaseManager::socketReadyRead()
{
    QMutexLocker locker(&m_mutex);

    m_readBuffer.append(m_socket->readAll());

    while (true) {
        int newlineIndex = m_readBuffer.indexOf('\n');
        if (newlineIndex == -1) {
            break; // 没有完整的消息
        }

        QByteArray message = m_readBuffer.left(newlineIndex).trimmed();
        m_readBuffer.remove(0, newlineIndex + 1);

        if (message.isEmpty()) continue;

        QJsonParseError parseError;
        QJsonDocument responseDoc = QJsonDocument::fromJson(message, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Client JSON parse error:" << parseError.errorString() << "for message:" << message;
            m_lastError = "Invalid JSON response from server.";
            emit errorOccurred(m_lastError);
            continue;
        }

        // 假设服务器每次请求只返回一个响应，并且客户端只发出一个请求后等待
        // 在更复杂的异步场景中，需要一个请求ID来匹配请求和响应
        m_lastResponse = responseDoc;
        m_responseReceived = true;
        m_waitCondition.wakeOne(); // 唤醒等待的线程
        break; // 处理完一个响应后退出，因为sendRequest只等待一个响应
    }
}

void RemoteDatabaseManager::socketDisconnected()
{
    qWarning() << "Server disconnected.";
    m_lastError = "Server disconnected.";
    emit errorOccurred(m_lastError);
    emit connectionStatusChanged(false);

    QMutexLocker locker(&m_mutex);
    m_responseReceived = true; // 即使是错误，也认为“收到”了结果（断开连接）
    m_waitCondition.wakeAll(); // 唤醒所有等待的线程，告知连接已断开
}

void RemoteDatabaseManager::socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    m_lastError = m_socket->errorString();
    qCritical() << "Socket error:" << m_lastError;
    emit errorOccurred(m_lastError);

    QMutexLocker locker(&m_mutex);
    m_responseReceived = true; // 即使是错误，也认为“收到”了结果
    m_waitCondition.wakeAll(); // 唤醒所有等待的线程
}

QJsonDocument RemoteDatabaseManager::sendRequest(const QString& methodName, const QJsonArray& args)
{
    if (!isConnected()) {
        m_lastError = "Not connected to database server.";
        emit errorOccurred(m_lastError);
        return QJsonDocument(); // 返回空文档表示失败
    }

    QJsonObject requestObj;
    requestObj["method"] = methodName;
    requestObj["args"] = args;
    // 在更复杂的场景中，可以添加 "requestId" 到请求和响应
    // requestObj["requestId"] = ++m_currentRequestId;

    QJsonDocument requestDoc(requestObj);
    QByteArray requestBytes = requestDoc.toJson(QJsonDocument::Compact) + "\n"; // 以换行符作为消息结束标志

    QMutexLocker locker(&m_mutex);
    m_responseReceived = false; // 在发送新请求前重置标志

    m_socket->write(requestBytes);
    if (!m_socket->waitForBytesWritten(5000)) { // 等待请求被发送
        m_lastError = "Failed to send request: " + m_socket->errorString();
        emit errorOccurred(m_lastError);
        return QJsonDocument();
    }

    // 等待 socketReadyRead 槽函数处理响应并唤醒
    if (!m_waitCondition.wait(&m_mutex, 10000)) { // 最多等待 10 秒
        m_lastError = "Timeout waiting for server response.";
        emit errorOccurred(m_lastError);
        return QJsonDocument();
    }

    if (!m_responseReceived) {
        // 这通常意味着连接在等待过程中断开，或者内部逻辑错误
        return QJsonDocument(); // 错误已通过 socketDisconnected/socketError 报告
    }

    // 成功收到响应
    return m_lastResponse;
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::parseResultSetFromJson(const QJsonArray& jsonArray)
{
    ResultSet resultSet;
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            DataRow row;
            QJsonObject rowObj = value.toObject();
            for (auto it = rowObj.begin(); it != rowObj.end(); ++it) {
                // 对于日期和时间类型，客户端接收后需要根据原始类型进行转换
                // 这里暂时都转为QVariant，后续使用时再根据需要to<QDate>(), to<QDateTime>()
                row[it.key()]=it.value().toVariant();
            }
            resultSet.push_back(row);
        }
    }
    return resultSet;
}
// --- 模仿 DatabaseManager 的公共接口的实现 ---

bool RemoteDatabaseManager::initDatabase() {
    QJsonArray args; // 不需要参数
    QJsonDocument responseDoc = sendRequest("initDatabase", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

bool RemoteDatabaseManager::execute(const QString& sql) {
    QJsonArray args;
    args.append(sql);
    QJsonDocument responseDoc = sendRequest("execute", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::query(const QString& sql) {
    QJsonArray args;
    args.append(sql);
    QJsonDocument responseDoc = sendRequest("query", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for query.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}


bool RemoteDatabaseManager::insert(const QString& tableName, const DataRow& data)
{
    QJsonArray args;
    args.append(tableName);
    QJsonObject dataObj;
    for (auto const& [key, val] : data) {
        dataObj.insert(key, QJsonValue::fromVariant(val));
    }
    args.append(dataObj);

    QJsonDocument responseDoc = sendRequest("insert", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

bool RemoteDatabaseManager::update(const QString& tableName, const DataRow& data, const QString& whereClause) {
    QJsonArray args;
    args.append(tableName);
    QJsonObject dataObj;
    for (auto const& [key, val] : data) {
        dataObj.insert(key, QJsonValue::fromVariant(val));
    }
    args.append(dataObj);
    args.append(whereClause);

    QJsonDocument responseDoc = sendRequest("update", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}
bool RemoteDatabaseManager::remove(const QString& tableName, const QString& whereClause) {
    QJsonArray args;
    args.append(tableName);
    args.append(whereClause);

    QJsonDocument responseDoc = sendRequest("remove", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

bool RemoteDatabaseManager::beginTransaction() {
    QJsonArray args;
    QJsonDocument responseDoc = sendRequest("beginTransaction", args);
    if (responseDoc.isNull()) return false;
    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

bool RemoteDatabaseManager::commitTransaction() {
    QJsonArray args;
    QJsonDocument responseDoc = sendRequest("commitTransaction", args);
    if (responseDoc.isNull()) return false;
    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

bool RemoteDatabaseManager::rollbackTransaction() {
    QJsonArray args;
    QJsonDocument responseDoc = sendRequest("rollbackTransaction", args);
    if (responseDoc.isNull()) return false;
    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return true;
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}


// 高级查询接口
RemoteDatabaseManager::ResultSet RemoteDatabaseManager::getDoctorsByDepartment(const QString& department)
{
    QJsonArray args;
    args.append(department);

    QJsonDocument responseDoc = sendRequest("getDoctorsByDepartment", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for getDoctorsByDepartment.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::getPatientAppointments(int patientId, const QDate& date)
{
    QJsonArray args;
    args.append(patientId);
    if (date.isValid()) {
        args.append(date.toString("yyyy-MM-dd")); // 使用 yyyy-MM-dd 格式
    }

    QJsonDocument responseDoc = sendRequest("getPatientAppointments", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for getPatientAppointments.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::getDoctorSchedule(const QString& doctorId, const QDate& date) {
    QJsonArray args;
    args.append(doctorId);
    args.append(date.toString("yyyy-MM-dd")); // 使用 yyyy-MM-dd 格式

    QJsonDocument responseDoc = sendRequest("getDoctorSchedule", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for getDoctorSchedule.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}

bool RemoteDatabaseManager::isTimeSlotAvailable(const QString& doctorId, const QDateTime& dateTime) {
    QJsonArray args;
    args.append(doctorId);
    args.append(dateTime.toString("yyyy-MM-dd hh:mm:ss")); // 使用服务器期望的格式

    QJsonDocument responseDoc = sendRequest("isTimeSlotAvailable", args);
    if (responseDoc.isNull()) return false;

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        return responseObj["result"].toBool();
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return false;
    }
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::getPatientMedicalRecords(int patientId) {
    QJsonArray args;
    args.append(patientId);

    QJsonDocument responseDoc = sendRequest("getPatientMedicalRecords", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for getPatientMedicalRecords.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}

RemoteDatabaseManager::ResultSet RemoteDatabaseManager::getChatHistory(int user1Id, int user2Id, int limit) {
    QJsonArray args;
    args.append(user1Id);
    args.append(user2Id);
    args.append(limit);

    QJsonDocument responseDoc = sendRequest("getChatHistory", args);
    if (responseDoc.isNull()) return ResultSet();

    QJsonObject responseObj = responseDoc.object();
    if (responseObj["success"].toBool()) {
        if (responseObj.contains("result") && responseObj["result"].isArray()) {
            return parseResultSetFromJson(responseObj["result"].toArray());
        } else {
            m_lastError = "Server response missing result or invalid format for getChatHistory.";
            emit errorOccurred(m_lastError);
            return ResultSet();
        }
    } else {
        m_lastError = responseObj["error"].toString();
        emit errorOccurred(m_lastError);
        return ResultSet();
    }
}

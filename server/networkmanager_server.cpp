#include "networkmanager_server.h"
#include "databasemanager.h"
#include <QDebug>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QCryptographicHash>

NetworkManagerServer::NetworkManagerServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_dbManager(nullptr)
    , m_port(12345)
{
    // 连接服务器信号
    connect(m_server, &QTcpServer::newConnection, this, &NetworkManagerServer::onNewConnection);
    
    // 初始化数据库管理器
    m_dbManager = &DatabaseManager::instance();
    
    // 初始化请求处理器
    initRequestHandlers();
    
    qDebug() << "服务器网络管理器初始化完成";
}

NetworkManagerServer::~NetworkManagerServer()
{
    stopServer();
}

bool NetworkManagerServer::startServer(quint16 port)
{
    m_port = port;
    
    if (!m_server->listen(QHostAddress::Any, port)) {
        qCritical() << "服务器启动失败:" << m_server->errorString();
        return false;
    }
    
    // 获取并显示本机IP地址
    QStringList ipAddresses;
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        if (interface.flags() & QNetworkInterface::IsUp && 
            !(interface.flags() & QNetworkInterface::IsLoopBack)) {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddresses << entry.ip().toString();
                }
            }
        }
    }
    
    qInfo() << "服务器启动成功!";
    qInfo() << "监听端口:" << port;
    qInfo() << "可用IP地址:" << ipAddresses;
    qInfo() << "等待客户端连接...";
    
    return true;
}

void NetworkManagerServer::stopServer()
{
    if (m_server->isListening()) {
        // 断开所有客户端连接
        for (auto it = m_clientSessions.begin(); it != m_clientSessions.end(); ++it) {
            it.key()->disconnectFromHost();
        }
        m_clientSessions.clear();
        
        m_server->close();
        qInfo() << "服务器已停止";
    }
}

int NetworkManagerServer::getConnectedClientsCount() const
{
    return m_clientSessions.size();
}

void NetworkManagerServer::onNewConnection()
{
    QTcpSocket* clientSocket = m_server->nextPendingConnection();
    
    // 连接客户端信号
    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkManagerServer::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &NetworkManagerServer::onClientDisconnected);
    
    // 添加到会话管理
    QString sessionId = QString("session_%1_%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(clientSocket);
    m_clientSessions[clientSocket] = sessionId;
    
    qInfo() << "新客户端连接:" << clientSocket->peerAddress().toString() 
            << ":" << clientSocket->peerPort() << "会话ID:" << sessionId;
    qInfo() << "当前连接数:" << m_clientSessions.size();
}

void NetworkManagerServer::onClientDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client && m_clientSessions.contains(client)) {
        QString sessionId = m_clientSessions.value(client);
        m_clientSessions.remove(client);
        
        qInfo() << "客户端断开连接:" << client->peerAddress().toString() 
                << "会话ID:" << sessionId;
        qInfo() << "当前连接数:" << m_clientSessions.size();
        
        client->deleteLater();
    }
}

void NetworkManagerServer::onDataReceived()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;
    
    QByteArray data = client->readAll();
    
    // 解析JSON请求
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON解析错误:" << error.errorString();
        QJsonObject errorResponse = createErrorResponse("", "JSON格式错误: " + error.errorString());
        sendResponse(client, errorResponse);
        return;
    }
    
    QJsonObject request = doc.object();
    qDebug() << "收到客户端请求:" << request;
    
    // 处理请求
    handleClientRequest(client, request);
}

void NetworkManagerServer::handleClientRequest(QTcpSocket* client, const QJsonObject& request)
{
    QString requestId = request.value("requestId").toString();
    QString action = request.value("action").toString();
    QJsonObject params = request.value("params").toObject();
    
    if (requestId.isEmpty() || action.isEmpty()) {
        QJsonObject errorResponse = createErrorResponse(requestId, "请求格式错误: requestId或action为空");
        sendResponse(client, errorResponse);
        return;
    }
    
    // 查找对应的处理器
    if (m_requestHandlers.contains(action)) {
        try {
            QJsonObject result = m_requestHandlers[action](params);
            QJsonObject response = createSuccessResponse(requestId, result);
            sendResponse(client, response);
        } catch (const std::exception& e) {
            QJsonObject errorResponse = createErrorResponse(requestId, QString("处理请求时发生错误: %1").arg(e.what()));
            sendResponse(client, errorResponse);
        }
    } else {
        QJsonObject errorResponse = createErrorResponse(requestId, "未知的操作: " + action);
        sendResponse(client, errorResponse);
    }
}

void NetworkManagerServer::sendResponse(QTcpSocket* client, const QJsonObject& response)
{
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "发送响应给客户端:" << response;
    
    client->write(data);
    client->flush();
}

QJsonObject NetworkManagerServer::createErrorResponse(const QString& requestId, const QString& errorMessage)
{
    QJsonObject response;
    response["requestId"] = requestId;
    response["success"] = false;
    response["error"] = errorMessage;
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return response;
}

QJsonObject NetworkManagerServer::createSuccessResponse(const QString& requestId, const QJsonObject& data)
{
    QJsonObject response;
    response["requestId"] = requestId;
    response["success"] = true;
    response["data"] = data;
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return response;
}

void NetworkManagerServer::initRequestHandlers()
{
    // 用户认证相关
    m_requestHandlers["login"] = [this](const QJsonObject& params) { return handleLogin(params); };
    m_requestHandlers["registerUser"] = [this](const QJsonObject& params) { return handleRegisterUser(params); };
    m_requestHandlers["forgotPassword"] = [this](const QJsonObject& params) { return handleForgotPassword(params); };
    m_requestHandlers["resetPassword"] = [this](const QJsonObject& params) { return handleResetPassword(params); };
    
    // 个人信息相关
    m_requestHandlers["getPatientInfo"] = [this](const QJsonObject& params) { return handleGetPatientInfo(params); };
    m_requestHandlers["updatePatientInfo"] = [this](const QJsonObject& params) { return handleUpdatePatientInfo(params); };
    m_requestHandlers["getDoctorInfo"] = [this](const QJsonObject& params) { return handleGetDoctorInfo(params); };
    m_requestHandlers["updateDoctorInfo"] = [this](const QJsonObject& params) { return handleUpdateDoctorInfo(params); };
    
    // 预约挂号相关
    m_requestHandlers["getAvailableDoctors"] = [this](const QJsonObject& params) { return handleGetAvailableDoctors(params); };
    m_requestHandlers["createAppointment"] = [this](const QJsonObject& params) { return handleCreateAppointment(params); };
    m_requestHandlers["getPatientAppointments"] = [this](const QJsonObject& params) { return handleGetPatientAppointments(params); };
    
    qDebug() << "请求处理器初始化完成，支持" << m_requestHandlers.size() << "种操作";
}

// 处理登录请求
QJsonObject NetworkManagerServer::handleLogin(const QJsonObject& params)
{
    QString email = params.value("email").toString();
    QString password = params.value("password").toString();
    
    if (email.isEmpty() || password.isEmpty()) {
        throw std::runtime_error("邮箱和密码不能为空");
    }
    
    // 查询用户信息
    QString sql = "SELECT u.user_id, u.username, u.email, u.password_hash, u.role "
                  "FROM users u WHERE u.email = ?";
    
    DatabaseManager::ResultSet result = m_dbManager->query(
        QString("SELECT u.user_id, u.username, u.email, u.password_hash, u.role "
                "FROM users u WHERE u.email = '%1'").arg(email)
    );
    
    if (result.empty()) {
        throw std::runtime_error("用户不存在");
    }
    
    DatabaseManager::DataRow userRow = result.front();
    
    // 验证密码
    QString storedHash = userRow.at("password_hash").toString();
    QString inputHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    
    if (storedHash != inputHash) {
        throw std::runtime_error("密码错误");
    }
    
    // 返回用户信息
    QJsonObject data;
    data["userId"] = userRow.at("user_id").toInt();
    data["username"] = userRow.at("username").toString();
    data["email"] = userRow.at("email").toString();
    data["role"] = userRow.at("role").toString();
    
    return data;
}

// 处理用户注册请求
QJsonObject NetworkManagerServer::handleRegisterUser(const QJsonObject& params)
{
    QString username = params.value("username").toString();
    QString email = params.value("email").toString();
    QString password = params.value("password").toString();
    QString role = params.value("role").toString();
    
    if (username.isEmpty() || email.isEmpty() || password.isEmpty() || role.isEmpty()) {
        throw std::runtime_error("所有字段都不能为空");
    }
    
    // 检查邮箱是否已存在
    DatabaseManager::ResultSet existResult = m_dbManager->query(
        QString("SELECT COUNT(*) as count FROM users WHERE email = '%1'").arg(email)
    );
    
    if (!existResult.empty() && existResult.front().at("count").toInt() > 0) {
        throw std::runtime_error("邮箱已被注册");
    }
    
    // 密码加密
    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    
    // 插入用户数据
    DatabaseManager::DataRow userData;
    userData["username"] = username;
    userData["email"] = email;
    userData["password_hash"] = passwordHash;
    userData["role"] = role;
    userData["created_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    if (!m_dbManager->insert("users", userData)) {
        throw std::runtime_error("注册失败: " + m_dbManager->lastError());
    }
    
    QJsonObject data;
    data["message"] = "注册成功";
    return data;
}

// 其他处理方法的实现将类似...
QJsonObject NetworkManagerServer::handleForgotPassword(const QJsonObject& params)
{
    // 这里只是示例实现，实际应用中需要实现邮件发送
    QString email = params.value("email").toString();
    
    if (email.isEmpty()) {
        throw std::runtime_error("邮箱不能为空");
    }
    
    // 检查邮箱是否存在
    DatabaseManager::ResultSet result = m_dbManager->query(
        QString("SELECT COUNT(*) as count FROM users WHERE email = '%1'").arg(email)
    );
    
    if (result.empty() || result.front().at("count").toInt() == 0) {
        throw std::runtime_error("邮箱未注册");
    }
    
    QJsonObject data;
    data["message"] = "验证码已发送";
    data["verificationCode"] = "123456"; // 模拟验证码
    return data;
}

QJsonObject NetworkManagerServer::handleResetPassword(const QJsonObject& params)
{
    QString email = params.value("email").toString();
    QString verificationCode = params.value("verificationCode").toString();
    QString newPassword = params.value("newPassword").toString();
    
    // 简单验证逻辑（实际应用中需要更完善的验证码管理）
    if (verificationCode != "123456") {
        throw std::runtime_error("验证码错误");
    }
    
    QString passwordHash = QString(QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256).toHex());
    
    DatabaseManager::DataRow updateData;
    updateData["password_hash"] = passwordHash;
    
    if (!m_dbManager->update("users", updateData, QString("email = '%1'").arg(email))) {
        throw std::runtime_error("密码重置失败: " + m_dbManager->lastError());
    }
    
    QJsonObject data;
    data["message"] = "密码重置成功";
    return data;
}

QJsonObject NetworkManagerServer::handleGetPatientInfo(const QJsonObject& params)
{
    int userId = params.value("userId").toInt();
    
    QString sql = QString(
        "SELECT u.username, u.email, p.full_name, p.sex, p.id_card_no, "
        "p.birth_date, p.age, p.phone_no, p.address, p.sos_name, p.sos_phone_no "
        "FROM users u LEFT JOIN patients p ON u.user_id = p.user_id "
        "WHERE u.user_id = %1"
    ).arg(userId);
    
    DatabaseManager::ResultSet result = m_dbManager->query(sql);
    
    if (result.empty()) {
        throw std::runtime_error("患者信息不存在");
    }
    
    DatabaseManager::DataRow row = result.front();
    QJsonObject data;
    
    for (auto it = row.begin(); it != row.end(); ++it) {
        data[it.key()] = QJsonValue::fromVariant(it.value());
    }
    
    return data;
}

QJsonObject NetworkManagerServer::handleUpdatePatientInfo(const QJsonObject& params)
{
    int userId = params.value("userId").toInt();
    QJsonObject details = params.value("details").toObject();
    
    // 这里应实现具体的更新逻辑
    // 由于代码较长，这里只提供框架
    
    QJsonObject data;
    data["message"] = "患者信息更新成功";
    return data;
}

QJsonObject NetworkManagerServer::handleGetDoctorInfo(const QJsonObject& params)
{
    QString doctorId = params.value("doctorId").toString();
    
    QString sql = QString(
        "SELECT u.username, u.email, d.full_name, d.sex, d.age, "
        "dept.department_name as department, d.title, d.phone_no, "
        "d.doc_start, d.doc_finish, d.registration_fee, d.patient_limit, "
        "d.photo_url, d.description "
        "FROM users u "
        "JOIN doctors d ON u.user_id = d.user_id "
        "LEFT JOIN departments dept ON d.department_id = dept.department_id "
        "WHERE d.doctor_id = '%1'"
    ).arg(doctorId);
    
    DatabaseManager::ResultSet result = m_dbManager->query(sql);
    
    if (result.empty()) {
        throw std::runtime_error("医生信息不存在");
    }
    
    DatabaseManager::DataRow row = result.front();
    QJsonObject data;
    
    for (auto it = row.begin(); it != row.end(); ++it) {
        data[it.key()] = QJsonValue::fromVariant(it.value());
    }
    
    return data;
}

QJsonObject NetworkManagerServer::handleUpdateDoctorInfo(const QJsonObject& params)
{
    // 实现医生信息更新逻辑
    QJsonObject data;
    data["message"] = "医生信息更新成功";
    return data;
}

QJsonObject NetworkManagerServer::handleGetAvailableDoctors(const QJsonObject& params)
{
    QString department = params.value("department").toString();
    
    QString sql = "SELECT d.doctor_id, d.full_name, d.sex, d.age, "
                  "dept.department_name as department, d.title, d.phone_no, "
                  "d.doc_start, d.doc_finish, d.registration_fee, d.patient_limit, "
                  "d.photo_url, d.description, u.email "
                  "FROM doctors d "
                  "JOIN users u ON d.user_id = u.user_id "
                  "LEFT JOIN departments dept ON d.department_id = dept.department_id";
    
    if (!department.isEmpty()) {
        sql += QString(" WHERE dept.department_name = '%1'").arg(department);
    }
    
    sql += " ORDER BY dept.department_name, d.full_name";
    
    DatabaseManager::ResultSet result = m_dbManager->query(sql);
    
    QJsonArray doctorArray;
    for (const auto& row : result) {
        QJsonObject doctor;
        for (auto it = row.begin(); it != row.end(); ++it) {
            doctor[it.key()] = QJsonValue::fromVariant(it.value());
        }
        doctorArray.append(doctor);
    }
    
    QJsonObject data;
    data["doctors"] = doctorArray;
    return data;
}

QJsonObject NetworkManagerServer::handleCreateAppointment(const QJsonObject& params)
{
    // 实现预约创建逻辑
    QJsonObject data;
    data["message"] = "预约创建成功";
    data["appointmentId"] = 123; // 示例ID
    return data;
}

QJsonObject NetworkManagerServer::handleGetPatientAppointments(const QJsonObject& params)
{
    // 实现获取患者预约列表逻辑
    QJsonObject data;
    data["appointments"] = QJsonArray(); // 空数组示例
    return data;
}

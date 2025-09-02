#ifndef NETWORKMANAGER_SERVER_H
#define NETWORKMANAGER_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QDateTime>

#include "databasemanager.h"

/**
 * @class NetworkManagerServer
 * @brief 服务器端网络管理器
 * 
 * 负责监听客户端连接，处理网络请求，调用数据库管理器执行操作
 * 使用TCP Socket + JSON协议进行通信
 */
class NetworkManagerServer : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManagerServer(QObject *parent = nullptr);
    ~NetworkManagerServer();

    /**
     * @brief 启动服务器监听
     * @param port 监听端口，默认12345
     * @return bool 启动是否成功
     */
    bool startServer(quint16 port = 12345);

    /**
     * @brief 停止服务器
     */
    void stopServer();

    /**
     * @brief 获取当前连接的客户端数量
     */
    int getConnectedClientsCount() const;

private slots:
    /**
     * @brief 处理新的客户端连接
     */
    void onNewConnection();

    /**
     * @brief 处理客户端断开连接
     */
    void onClientDisconnected();

    /**
     * @brief 处理客户端发送的数据
     */
    void onDataReceived();

private:
    /**
     * @brief 处理客户端请求
     * @param client 客户端socket
     * @param request 请求的JSON对象
     */
    void handleClientRequest(QTcpSocket* client, const QJsonObject& request);

    /**
     * @brief 发送响应给客户端
     * @param client 客户端socket
     * @param response 响应的JSON对象
     */
    void sendResponse(QTcpSocket* client, const QJsonObject& response);

    /**
     * @brief 创建错误响应
     * @param requestId 请求ID
     * @param errorMessage 错误信息
     * @return QJsonObject 错误响应对象
     */
    QJsonObject createErrorResponse(const QString& requestId, const QString& errorMessage);

    /**
     * @brief 创建成功响应
     * @param requestId 请求ID
     * @param data 响应数据
     * @return QJsonObject 成功响应对象
     */
    QJsonObject createSuccessResponse(const QString& requestId, const QJsonObject& data = QJsonObject());

    // 用户认证相关处理方法
    QJsonObject handleLogin(const QJsonObject& params);
    QJsonObject handleRegisterUser(const QJsonObject& params);
    QJsonObject handleForgotPassword(const QJsonObject& params);
    QJsonObject handleResetPassword(const QJsonObject& params);
    
    // 个人信息管理处理方法
    QJsonObject handleGetPatientInfo(const QJsonObject& params);
    QJsonObject handleUpdatePatientInfo(const QJsonObject& params);
    QJsonObject handleGetDoctorInfo(const QJsonObject& params);
    QJsonObject handleUpdateDoctorInfo(const QJsonObject& params);
    
    // 预约挂号相关处理方法
    QJsonObject handleGetAvailableDoctors(const QJsonObject& params);
    QJsonObject handleGetDoctorScheduleForDate(const QJsonObject& params);
    QJsonObject handleCreateAppointment(const QJsonObject& params);
    QJsonObject handleGetPatientAppointments(const QJsonObject& params);
    QJsonObject handleCancelAppointment(const QJsonObject& params);
    QJsonObject handleUpdateAppointmentStatus(const QJsonObject& params);
    
    // 病历医嘱相关处理方法
    QJsonObject handleGetPatientMedicalHistory(const QJsonObject& params);
    QJsonObject handleCreateMedicalRecord(const QJsonObject& params);
    QJsonObject handleAddPrescription(const QJsonObject& params);
    QJsonObject handleGetPatientPrescriptions(const QJsonObject& params);
    
    // 聊天通信相关处理方法
    QJsonObject handleSendMessage(const QJsonObject& params);
    QJsonObject handleGetChatHistory(const QJsonObject& params);
    QJsonObject handleGetRecentContacts(const QJsonObject& params);
    
    // 考勤管理相关处理方法
    QJsonObject handleCheckIn(const QJsonObject& params);
    QJsonObject handleCheckOut(const QJsonObject& params);
    QJsonObject handleGetAttendanceHistory(const QJsonObject& params);
    QJsonObject handleSubmitLeaveRequest(const QJsonObject& params);
    QJsonObject handleGetLeaveRequests(const QJsonObject& params);
    QJsonObject handleCancelLeave(const QJsonObject& params);
    
    // 科室管理相关处理方法
    QJsonObject handleGetAllDepartments(const QJsonObject& params);
    QJsonObject handleGetDepartmentInfo(const QJsonObject& params);
    
    // 药品管理相关处理方法
    QJsonObject handleSearchDrugs(const QJsonObject& params);
    QJsonObject handleGetDrugDetails(const QJsonObject& params);
    
    // 工具方法
    QJsonObject dataRowToJsonObject(const DatabaseManager::DataRow& row);
    QJsonArray resultSetToJsonArray(const DatabaseManager::ResultSet& resultSet);
    QVariantMap jsonObjectToVariantMap(const QJsonObject& obj);
    QVariantList jsonArrayToVariantList(const QJsonArray& arr);

private:
    QTcpServer* m_server;                           // TCP服务器
    QMap<QTcpSocket*, QString> m_clientSessions;    // 客户端会话管理
    DatabaseManager* m_dbManager;                   // 数据库管理器指针
    quint16 m_port;                                // 监听端口

    // 请求处理映射表
    QMap<QString, std::function<QJsonObject(const QJsonObject&)>> m_requestHandlers;

    /**
     * @brief 初始化请求处理器映射
     */
    void initRequestHandlers();
};

#endif // NETWORKMANAGER_SERVER_H

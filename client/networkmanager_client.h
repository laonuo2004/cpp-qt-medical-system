#ifndef NETWORKMANAGER_CLIENT_H
#define NETWORKMANAGER_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMap>
#include <QVariantMap>
#include <QVariantList>
#include <functional>

/**
 * @class NetworkManagerClient
 * @brief 客户端网络管理器
 * 
 * 负责与服务器建立连接，发送请求并处理响应
 * 使用TCP Socket + JSON协议进行通信
 */
class NetworkManagerClient : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManagerClient(QObject *parent = nullptr);
    ~NetworkManagerClient();

    /**
     * @brief 连接到服务器
     * @param host 服务器地址
     * @param port 服务器端口
     * @return bool 连接是否成功发起
     */
    bool connectToServer(const QString& host, quint16 port);

    /**
     * @brief 断开与服务器的连接
     */
    void disconnectFromServer();

    /**
     * @brief 检查是否已连接到服务器
     */
    bool isConnected() const;

    /**
     * @brief 发送同步请求（阻塞模式）
     * @param action 请求动作
     * @param params 请求参数
     * @param timeout 超时时间（毫秒）
     * @return QVariantMap 响应数据
     */
    QVariantMap sendRequestSync(const QString& action, const QVariantMap& params, int timeout = 5000);

    /**
     * @brief 发送异步请求（非阻塞模式）
     * @param action 请求动作
     * @param params 请求参数
     * @param callback 回调函数
     */
    void sendRequestAsync(const QString& action, const QVariantMap& params, 
                         std::function<void(bool success, const QVariantMap& response)> callback);

    /**
     * @brief 模拟DatabaseManager的DataRow类型，用于兼容现有代码
     */
    using DataRow = std::map<QString, QVariant>;
    using ResultSet = std::vector<DataRow>;

    /**
     * @brief 将QVariantMap转换为DataRow格式
     */
    DataRow variantMapToDataRow(const QVariantMap& map);

    /**
     * @brief 将QVariantList转换为ResultSet格式  
     */
    ResultSet variantListToResultSet(const QVariantList& list);

private slots:
    /**
     * @brief 处理连接成功信号
     */
    void onConnected();

    /**
     * @brief 处理连接断开信号
     */
    void onDisconnected();

    /**
     * @brief 处理服务器数据
     */
    void onDataReceived();

    /**
     * @brief 处理连接错误
     */
    void onError(QAbstractSocket::SocketError error);

    /**
     * @brief 处理请求超时
     */
    void onRequestTimeout();

private:
    /**
     * @brief 生成唯一的请求ID
     */
    QString generateRequestId();

    /**
     * @brief 发送JSON请求到服务器
     */
    void sendJsonRequest(const QJsonObject& request);

    /**
     * @brief 处理服务器响应
     */
    void handleServerResponse(const QJsonObject& response);

    /**
     * @brief 创建请求对象
     */
    QJsonObject createRequest(const QString& action, const QVariantMap& params);

    /**
     * @brief QVariantMap与JSON之间的转换
     */
    QJsonObject variantMapToJsonObject(const QVariantMap& map);
    QVariantMap jsonObjectToVariantMap(const QJsonObject& obj);
    QJsonArray variantListToJsonArray(const QVariantList& list);
    QVariantList jsonArrayToVariantList(const QJsonArray& arr);

signals:
    /**
     * @brief 连接状态变化信号
     */
    void connectionStatusChanged(bool connected);

    /**
     * @brief 网络错误信号
     */
    void networkError(const QString& error);

    /**
     * @brief 异步响应接收信号
     */
    void responseReceived(const QString& requestId, const QVariantMap& response);

private:
    QTcpSocket* m_socket;                                                    // TCP套接字
    QString m_serverHost;                                                    // 服务器地址
    quint16 m_serverPort;                                                   // 服务器端口
    
    // 同步请求支持
    QMap<QString, QVariantMap> m_pendingSyncResponses;                      // 等待的同步响应
    QMap<QString, bool> m_syncRequestCompleted;                            // 同步请求完成标记
    
    // 异步请求支持
    QMap<QString, std::function<void(bool, const QVariantMap&)>> m_asyncCallbacks; // 异步回调
    
    // 超时管理
    QMap<QString, QTimer*> m_requestTimers;                                 // 请求超时定时器
    
    int m_requestCounter;                                                   // 请求计数器
};

#endif // NETWORKMANAGER_CLIENT_H

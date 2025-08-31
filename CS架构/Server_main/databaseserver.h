// File 3: databaseserver.h
#ifndef DATABASESERVER_H
#define DATABASESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QNetworkInterface>
#include "databasemanager.h" // 包含 DatabaseManager 定义

// 前向声明，因为 DatabaseServer 会用到
class DatabaseServer;

// 为每个客户端连接创建一个工作线程
class ClientWorker : public QObject
{
    Q_OBJECT
public:
    // 构造函数接收一个套接字和数据库连接名
    explicit ClientWorker(QTcpSocket* socket, const QString& connectionName, QObject* parent = nullptr);
    ~ClientWorker(); // 添加析构函数以释放资源

signals:
    void finished();
    void error(const QString& msg);
    void sendResponse(QTcpSocket* socket, const QJsonDocument& response);

public slots:
    void processSocketReads(); // 改为更通用的名字，由线程启动时连接
    void disconnected();

private:
    void initializeInThread(); // 在工作线程中初始化数据库连接
    void processRequest(const QJsonDocument& requestDoc);
    QJsonDocument createResponse(bool success, const QVariant& result = QVariant(), const QString& errorMessage = QString());

    // JSON (反)序列化辅助函数
    // DatabaseManager::ResultSet parseResultSetFromJson(const QJsonArray& jsonArray); // Removed, not used in client request parsing
    DatabaseManager::DataRow parseDataRowFromJson(const QJsonObject& jsonObject);
    QJsonArray serializeResultSetToJson(const DatabaseManager::ResultSet& resultSet);

    QTcpSocket* m_socket; // Publicly accessible for DatabaseServer's clientWorkerFinished cleanup
    friend class DatabaseServer; // Allow DatabaseServer to access m_socket for cleanup

    QString m_connectionName;      // 用于在线程中创建数据库连接
    DatabaseManager* m_dbManager;  // 必须是指针，在线程中创建
    QByteArray m_readBuffer;       // 缓冲来自此客户端的局部消息
};

class DatabaseServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DatabaseServer(QObject *parent = nullptr);
    bool start(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    // 此槽用于从任何工作线程接收响应并将其写入主线程管理的套接字
    void handleResponse(QTcpSocket* socket, const QJsonDocument& response);
    // Slot to clean up client threads when they finish
    void clientWorkerFinished(); // NEW

private:
    // bool createTablesIfNotExist(QSqlDatabase &db); // REMOVED

    // 维护客户端socket和线程的映射，以便于管理
    QMap<QTcpSocket*, QThread*> m_clientThreads;
};

#endif // DATABASESERVER_H

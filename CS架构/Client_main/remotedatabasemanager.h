#ifndef REMOTEDATABASMANAGER_H
#define REMOTEDATABASMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#include <map>
#include <vector>
#include <QDate>
#include <QDateTime>
#include <QVariant>

class RemoteDatabaseManager : public QObject
{
    Q_OBJECT
public:
    // 重新定义 DataRow 和 ResultSet，或从公共头文件引入
    using DataRow = std::map<QString, QVariant>;
    using ResultSet = std::vector<DataRow>;

    explicit RemoteDatabaseManager(QObject *parent = nullptr);
    ~RemoteDatabaseManager() override;

    bool connectToServer(const QString& host, quint16 port, int timeoutMs = 50000);
    void disconnectFromServer();
    bool isConnected() const;

    // 模仿 DatabaseManager 的公共接口
    bool initDatabase(); // 客户端调用此方法，实际上是通知服务器初始化
    bool execute(const QString& sql);
    ResultSet query(const QString& sql);
    bool insert(const QString& tableName, const DataRow& data);
    bool update(const QString& tableName, const DataRow& data, const QString& whereClause);
    bool remove(const QString& tableName, const QString& whereClause);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    ResultSet getDoctorsByDepartment(const QString& department);
    ResultSet getPatientAppointments(int patientId, const QDate& date = QDate());
    ResultSet getDoctorSchedule(const QString& doctorId, const QDate& date);
    bool isTimeSlotAvailable(const QString& doctorId, const QDateTime& dateTime);
    ResultSet getPatientMedicalRecords(int patientId);
    ResultSet getChatHistory(int user1Id, int user2Id, int limit = 50);

    QString lastError() const { return m_lastError; }

signals:
    void connectionStatusChanged(bool isConnected);
    void errorOccurred(const QString& errorMessage);

private slots:
    void socketReadyRead();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError socketError);

private:
    // 内部帮助函数：发送请求并等待响应
    QJsonDocument sendRequest(const QString& methodName, const QJsonArray& args);

    // 内部帮助函数：解析服务器返回的 ResultSet
    ResultSet parseResultSetFromJson(const QJsonArray& jsonArray);

    QTcpSocket* m_socket;
    QString m_lastError;
    QByteArray m_readBuffer; // 缓冲来自服务器的局部消息

    // 用于同步请求/响应的机制
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QJsonDocument m_lastResponse;
    bool m_responseReceived;
    quint64 m_currentRequestId; // 用于匹配请求和响应
};

#endif // REMOTEDATABASMANAGER_H

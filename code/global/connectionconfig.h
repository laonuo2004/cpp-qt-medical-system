#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H

#include <QObject>
#include <QSettings>
#include <QString>

/**
 * @class ConnectionConfig
 * @brief 连接配置管理类
 *
 * 该类负责管理数据库连接配置，包括本地/远程模式切换、
 * 服务器地址、端口等配置信息的持久化存储和读取。
 */
class ConnectionConfig : public QObject
{
    Q_OBJECT
public:
    // 连接模式枚举
    enum ConnectionMode {
        LocalMode,      // 本地SQLite模式
        RemoteMode      // 远程服务器模式
    };

    // 单例模式获取实例
    static ConnectionConfig& instance();

    // 禁止拷贝构造和赋值操作
    ConnectionConfig(const ConnectionConfig&) = delete;
    ConnectionConfig& operator=(const ConnectionConfig&) = delete;

    // 获取当前连接模式
    ConnectionMode getConnectionMode() const;

    // 设置连接模式
    void setConnectionMode(ConnectionMode mode);

    // 获取远程服务器地址
    QString getRemoteHost() const;

    // 设置远程服务器地址
    void setRemoteHost(const QString& host);

    // 获取远程服务器端口
    quint16 getRemotePort() const;

    // 设置远程服务器端口
    void setRemotePort(quint16 port);

    // 获取连接超时时间（毫秒）
    int getConnectionTimeout() const;

    // 设置连接超时时间（毫秒）
    void setConnectionTimeout(int timeoutMs);

    // 获取本地数据库文件名
    QString getLocalDatabaseName() const;

    // 设置本地数据库文件名
    void setLocalDatabaseName(const QString& dbName);

    // 保存配置到文件
    void saveConfig();

    // 从文件加载配置
    void loadConfig();

    // 重置为默认配置
    void resetToDefaults();

signals:
    // 配置变化信号
    void configChanged();

private:
    // 私有构造函数，实现单例模式
    explicit ConnectionConfig(QObject *parent = nullptr);

    // 析构函数
    ~ConnectionConfig() override;

    // 设置对象
    QSettings* m_settings;

    // 当前配置
    ConnectionMode m_connectionMode;
    QString m_remoteHost;
    quint16 m_remotePort;
    int m_connectionTimeout;
    QString m_localDatabaseName;
};

#endif // CONNECTIONCONFIG_H

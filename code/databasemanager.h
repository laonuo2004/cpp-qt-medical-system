#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <map>
#include <vector>
#include <memory>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    // 单例模式获取实例
    static DatabaseManager& instance();

    // 禁止拷贝构造和赋值操作
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // 数据行表示
    using DataRow = std::map<QString, QVariant>;
    // 结果集表示
    using ResultSet = std::vector<DataRow>;

    // 检查数据库是否连接
    bool isConnected() const;

    // 获取最后一次错误信息
    QString lastError() const;

    // 初始化数据库，创建必要的表
    bool initDatabase();

    // 执行非查询SQL语句
    bool execute(const QString& sql);

    // 执行查询SQL语句
    ResultSet query(const QString& sql);

    // 插入数据
    bool insert(const QString& tableName, const DataRow& data);

    // 更新数据
    bool update(const QString& tableName, const DataRow& data, const QString& whereClause);

    // 删除数据
    bool remove(const QString& tableName, const QString& whereClause);

    // 事务操作
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    // 获取数据库连接对象(谨慎使用)
    QSqlDatabase database() const;

signals:
    // 数据库连接状态变化信号
    void connectionStatusChanged(bool isConnected);

    // 错误发生信号
    void errorOccurred(const QString& errorMessage);

private:
    // 私有构造函数，实现单例模式
    explicit DatabaseManager(QObject *parent = nullptr);

    // 析构函数
    ~DatabaseManager() override;

    // 初始化数据库连接
    bool initConnection(const QString& dbName = "medical_system.db");

    // 创建数据表
    bool createTables();

    // 数据库连接对象
    QSqlDatabase m_db;

    // 最后一次错误信息
    mutable QString m_lastError;
};

#endif // DATABASEMANAGER_H

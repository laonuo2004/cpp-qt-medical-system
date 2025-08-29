#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>


DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
        // 初始化数据库连接
        bool success = initConnection();
        emit connectionStatusChanged(success);

        if (success) {
            // 初始化数据库表结构
            if (!initDatabase()) {
                qWarning() << "数据库表结构初始化失败:" << lastError();
            }
        } else {
            qCritical() << "数据库连接初始化失败:" << lastError();
        }
    }
// 单例实例初始化
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

    DatabaseManager::~DatabaseManager()
    {
        if (m_db.isOpen()) {
            m_db.close();
            emit connectionStatusChanged(false);
            qDebug() << "数据库连接已关闭";
        }
    }

    bool DatabaseManager::isConnected() const
    {
        return m_db.isOpen();
    }

    QString DatabaseManager::lastError() const
    {
        if (m_db.lastError().type() != QSqlError::NoError) {
            return m_db.lastError().text();
        }
        return m_lastError;
    }

    bool DatabaseManager::initDatabase()
    {
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            return false;
        }

        return createTables();
    }

    bool DatabaseManager::execute(const QString &sql)
    {
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            emit errorOccurred(m_lastError);
            return false;
        }

        QSqlQuery query(m_db);
        if (!query.exec(sql)) {
            m_lastError = query.lastError().text();
            emit errorOccurred(m_lastError);
            qWarning() << "SQL执行失败:" << sql << "错误:" << m_lastError;
            return false;
        }

        return true;
    }

    DatabaseManager::ResultSet DatabaseManager::query(const QString &sql)
    {
        ResultSet result;
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            emit errorOccurred(m_lastError);
            return result;
        }

        QSqlQuery query(m_db);
        if (!query.exec(sql)) {
            m_lastError = query.lastError().text();
            emit errorOccurred(m_lastError);
            qWarning() << "查询失败:" << sql << "错误:" << m_lastError;
            return result;
        }

        // 解析查询结果
        QSqlRecord record = query.record();
        int columnCount = record.count();

        while (query.next()) {
            DataRow row;
            for (int i = 0; i < columnCount; ++i) {
                QString columnName = record.fieldName(i);
                QVariant value = query.value(i);
                row[columnName] = value;
            }
            result.push_back(row);
        }

        return result;
    }

    bool DatabaseManager::insert(const QString &tableName, const DataRow &data)
    {
        if (data.empty()) {
            m_lastError = "插入数据不能为空";
            emit errorOccurred(m_lastError);
            return false;
        }

        // 构建INSERT语句
        QString sql = QString("INSERT INTO %1 (").arg(tableName);
        QString valuesClause = "VALUES (";

        auto it = data.begin();
        while (it != data.end()) {
            sql += it->first;
            valuesClause += ":" + it->first;

            ++it;
            if (it != data.end()) {
                sql += ", ";
                valuesClause += ", ";
            }
        }

        sql += ") " + valuesClause + ")";

        // 执行插入
        QSqlQuery query(m_db);
        query.prepare(sql);

        for (const auto& pair : data) {
            query.bindValue(":" + pair.first, pair.second);
        }

        if (!query.exec()) {
            m_lastError = query.lastError().text();
            emit errorOccurred(m_lastError);
            qWarning() << "插入失败:" << sql << "错误:" << m_lastError;
            return false;
        }

        return true;
    }

    bool DatabaseManager::update(const QString &tableName, const DataRow &data, const QString &whereClause)
    {
        if (data.empty()) {
            m_lastError = "更新数据不能为空";
            emit errorOccurred(m_lastError);
            return false;
        }

        if (whereClause.isEmpty()) {
            m_lastError = "更新操作必须指定WHERE条件";
            emit errorOccurred(m_lastError);
            return false;
        }

        // 构建UPDATE语句
        QString sql = QString("UPDATE %1 SET ").arg(tableName);

        auto it = data.begin();
        while (it != data.end()) {
            sql += it->first + " = :" + it->first;

            ++it;
            if (it != data.end()) {
                sql += ", ";
            }
        }

        sql += " WHERE " + whereClause;

        // 执行更新
        QSqlQuery query(m_db);
        query.prepare(sql);

        for (const auto& pair : data) {
            query.bindValue(":" + pair.first, pair.second);
        }

        if (!query.exec()) {
            m_lastError = query.lastError().text();
            emit errorOccurred(m_lastError);
            qWarning() << "更新失败:" << sql << "错误:" << m_lastError;
            return false;
        }

        return true;
    }

    bool DatabaseManager::remove(const QString &tableName, const QString &whereClause)
    {
        if (whereClause.isEmpty()) {
            m_lastError = "删除操作必须指定WHERE条件";
            emit errorOccurred(m_lastError);
            return false;
        }

        QString sql = QString("DELETE FROM %1 WHERE %2").arg(tableName).arg(whereClause);
        return execute(sql);
    }

    bool DatabaseManager::beginTransaction()
    {
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            emit errorOccurred(m_lastError);
            return false;
        }

        return m_db.transaction();
    }

    bool DatabaseManager::commitTransaction()
    {
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            emit errorOccurred(m_lastError);
            return false;
        }

        return m_db.commit();
    }

    bool DatabaseManager::rollbackTransaction()
    {
        if (!isConnected()) {
            m_lastError = "数据库未连接";
            emit errorOccurred(m_lastError);
            return false;
        }

        return m_db.rollback();
    }

    QSqlDatabase DatabaseManager::database() const
    {
        return m_db;
    }

    bool DatabaseManager::initConnection(const QString &dbName)
    {
        // 检查是否已存在相同名称的连接
        if (QSqlDatabase::contains("medical_system_connection")) {
            m_db = QSqlDatabase::database("medical_system_connection");
        } else {
            // 创建新的数据库连接
            m_db = QSqlDatabase::addDatabase("QSQLITE", "medical_system_connection");
        }

        // 设置数据库文件路径（放在应用程序目录下）
        QString dbPath = QDir(QCoreApplication::applicationDirPath()).filePath(dbName);
        m_db.setDatabaseName(dbPath);

        // 打开数据库
        if (!m_db.open()) {
            m_lastError = m_db.lastError().text();
            emit errorOccurred("数据库打开失败: " + m_lastError);
            return false;
        }

        qDebug() << "数据库连接成功，路径:" << dbPath;
        return true;
    }

    bool DatabaseManager::createTables()
    {
        // 开始事务批量创建表
        if (!beginTransaction()) {
            return false;
        }

        // 1. 用户表（存储所有角色用户）
        bool success = execute(R"(
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL,
                email TEXT NOT NULL UNIQUE,
                password TEXT NOT NULL,
                role INTEGER NOT NULL, -- 0:管理员, 1:医生, 2:患者
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )");

        // 2. 医生信息表（扩展医生角色的信息）
        if (success) {
            success = execute(R"(
                CREATE TABLE IF NOT EXISTS doctors (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    user_id INTEGER NOT NULL,
                    department TEXT,
                    title TEXT, -- 职称
                    specialty TEXT, -- 专长
                    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
                )
            )");
        }

        // 3. 患者信息表（扩展患者角色的信息）
        if (success) {
            success = execute(R"(
                CREATE TABLE IF NOT EXISTS patients (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    user_id INTEGER NOT NULL,
                    gender TEXT,
                    age INTEGER,
                    phone TEXT,
                    address TEXT,
                    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
                )
            )");
        }

        // 根据事务结果提交或回滚
        if (success) {
            success = commitTransaction();
            qDebug() << "数据库表结构创建/验证成功";
        } else {
            rollbackTransaction();
            qWarning() << "数据库表结构创建失败";
        }

        return success;
    }

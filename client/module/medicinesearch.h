#ifndef MEDICINESEARCH_H
#define MEDICINESEARCH_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QString>

class MedicineSearch : public QObject
{
    Q_OBJECT

public:
    explicit MedicineSearch(QObject *parent = nullptr);
    ~MedicineSearch();

    // 数据库操作
    bool initializeDatabase();
    bool insertRecord(const QString &sid, const QString &phone);
    bool updateRecord(const QString &sid, const QString &phone);
    bool deleteAllRecords();
    QStandardItemModel* selectAllRecords();
    bool recordExists(const QString &sid);

    // 获取错误信息
    QString lastError() const;

private:
    QSqlDatabase m_database;
    QString m_lastError;
};

#endif // MEDICINESEARCH_H

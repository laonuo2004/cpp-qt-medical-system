
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QDate>
#include <QDateTime>
#include <QSqlQuery> // Add this for the overloaded query method

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager() override;

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    using DataRow = QMap<QString, QVariant>;
    using ResultSet = QList<DataRow>;

    void setDatabase(const QSqlDatabase &db);

    bool isConnected() const;
    QString lastError() const;
    bool initDatabase();
    bool execute(const QString& sql);

    // Original query method (for simple SQL strings)
    ResultSet query(const QString& sql);
    // Overloaded query method (for already prepared and executed QSqlQuery objects)
    ResultSet query(QSqlQuery& sqlQuery); // NEW DECLARATION

    bool insert(const QString& tableName, const DataRow& data);
    bool update(const QString& tableName, const DataRow& data, const QString& whereClause);
    bool remove(const QString& tableName, const QString& whereClause);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    QSqlDatabase database() const;

    // --- High-level query methods ---
    ResultSet getDoctorsByDepartment(const QString& department);
    ResultSet getPatientAppointments(int patientId, const QDate& date = QDate());
    ResultSet getDoctorSchedule(const QString& doctorId, const QDate& date);
    bool isTimeSlotAvailable(const QString& doctorId, const QDateTime& dateTime);
    ResultSet getPatientMedicalRecords(int patientId);
    ResultSet getChatHistory(int user1Id, int user2Id, int limit = 50);

signals:
    void errorOccurred(const QString& errorMessage);

private:
    bool createTables();

    QSqlDatabase m_db;
    mutable QString m_lastError;
};

#endif // DATABASEMANAGER_H

#include "medicinesearch.h"
#include <QMessageBox>
#include <QSqlError>

MedicineSearch::MedicineSearch(QObject *parent)
    : QObject(parent)
{
    // 初始化数据库连接
    m_database = QSqlDatabase::addDatabase("QSQLITE", "medicine_connection");
    m_database.setDatabaseName("my.db");
}

MedicineSearch::~MedicineSearch()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool MedicineSearch::initializeDatabase()
{
    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        return false;
    }

    QSqlQuery query(m_database);
    if (!query.exec("CREATE TABLE IF NOT EXISTS student(sid INTEGER PRIMARY KEY, phone VARCHAR(20))")) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool MedicineSearch::insertRecord(const QString &sid, const QString &phone)
{
    if (sid.isEmpty() || phone.isEmpty()) {
        m_lastError = "ID or phone is empty";
        return false;
    }

    if (recordExists(sid)) {
        m_lastError = "Record already exists";
        return false;
    }

    QSqlQuery query(m_database);
    QString sql = QString("INSERT INTO student(sid, phone) VALUES(%1, '%2')")
                  .arg(sid).arg(phone);

    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool MedicineSearch::updateRecord(const QString &sid, const QString &phone)
{
    if (sid.isEmpty() || phone.isEmpty()) {
        m_lastError = "SID or phone is empty";
        return false;
    }

    if (!recordExists(sid)) {
        m_lastError = "Record does not exist";
        return false;
    }

    QSqlQuery query(m_database);
    QString sql = QString("UPDATE student SET phone = '%1' WHERE sid = %2")
                  .arg(phone).arg(sid);

    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool MedicineSearch::deleteAllRecords()
{
    QSqlQuery query(m_database);
    if (!query.exec("DELETE FROM student")) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QStandardItemModel* MedicineSearch::selectAllRecords()
{
    QSqlQuery query(m_database);
    if (!query.exec("SELECT sid, phone FROM student")) {
        m_lastError = query.lastError().text();
        return nullptr;
    }

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << "SID" << "Phone");

    int row = 0;
    while (query.next()) {
        model->setItem(row, 0, new QStandardItem(query.value(0).toString()));
        model->setItem(row, 1, new QStandardItem(query.value(1).toString()));
        row++;
    }

    return model;
}

bool MedicineSearch::recordExists(const QString &sid)
{
    QSqlQuery query(m_database);
    QString sql = QString("SELECT sid FROM student WHERE sid = %1").arg(sid);

    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        return false;
    }

    return query.next();
}

QString MedicineSearch::lastError() const
{
    return m_lastError;
}

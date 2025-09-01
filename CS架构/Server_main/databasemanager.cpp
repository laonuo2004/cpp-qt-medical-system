// File 2: databasemanager.cpp
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) {
        QString connectionName = m_db.connectionName();
        m_db.close();
        // After closing, we must remove the connection by its unique name
        // to free up resources.
        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << "Database connection" << connectionName << "closed and removed.";
    }
}

void DatabaseManager::setDatabase(const QSqlDatabase &db)
{
    m_db = db;
}

bool DatabaseManager::isConnected() const
{
    return m_db.isOpen();
}

QString DatabaseManager::lastError() const
{
    // Return the last error from the database connection itself if available,
    // otherwise return the last manually set error message.
    if (m_db.lastError().type() != QSqlError::NoError) {
        return m_db.lastError().text();
    }
    return m_lastError;
}

bool DatabaseManager::initDatabase()
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError); // Emit error here as well
        return false;
    }
    qDebug() << "Initializing database tables...";
    return createTables();
}

bool DatabaseManager::execute(const QString &sql)
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec(sql)) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "SQL Execute Failed:" << sql << "Error:" << m_lastError;
        return false;
    }
    return true;
}

// Original query method (for simple SQL strings, uses the new overloaded method)
DatabaseManager::ResultSet DatabaseManager::query(const QString &sql)
{
    QSqlQuery q(m_db);
    if (!q.exec(sql)) {
        m_lastError = q.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "Query Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return query(q); // Call the overloaded method to process results
}

// NEW: Overloaded query method to process an already executed QSqlQuery object
DatabaseManager::ResultSet DatabaseManager::query(QSqlQuery& sqlQuery)
{
    ResultSet result;
    if (!isConnected()) { // Redundant if called from already connected scope, but safe check
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return result;
    }

    // Assume sqlQuery is already executed, just check its status and iterate
    if (sqlQuery.lastError().type() != QSqlError::NoError) {
        m_lastError = sqlQuery.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "Executed Query Failed (from passed QSqlQuery):" << sqlQuery.lastError().text();
        return result;
    }

    QSqlRecord record = sqlQuery.record();
    while (sqlQuery.next()) {
        DataRow row;
        for (int i = 0; i < record.count(); ++i) {
            row.insert(record.fieldName(i), sqlQuery.value(i));
        }
        result.append(row);
    }
    return result;
}

bool DatabaseManager::insert(const QString &tableName, const DataRow &data)
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }
    if (data.isEmpty()) {
        m_lastError = "Cannot insert empty data.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }

    QString columns, placeholders;
    auto it = data.constBegin();
    while (it != data.constEnd()) {
        columns += it.key();
        placeholders += ":" + it.key();
        ++it;
        if (it != data.constEnd()) {
            columns += ", ";
            placeholders += ", ";
        }
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(tableName, columns, placeholders);

    QSqlQuery query(m_db);
    if (!query.prepare(sql)) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError); // Emit error
        qWarning() << "Insert Prepare Failed:" << sql << "Error:" << m_lastError;
        return false;
    }

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError); // Emit error
        qWarning() << "Insert Exec Failed:" << sql << "Error:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::update(const QString &tableName, const DataRow &data, const QString &whereClause)
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }
    if (data.isEmpty()) {
        m_lastError = "Cannot update with empty data.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }
    if (whereClause.isEmpty()) {
        m_lastError = "Update operation must have a WHERE clause.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }

    QString setClause;
    auto it = data.constBegin();
    while (it != data.constEnd()) {
        setClause += QString("%1 = :%1").arg(it.key());
        ++it;
        if (it != data.constEnd()) {
            setClause += ", ";
        }
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE %3").arg(tableName, setClause, whereClause);

    QSqlQuery query(m_db);
    if (!query.prepare(sql)) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError); // Emit error
        qWarning() << "Update Prepare Failed:" << sql << "Error:" << m_lastError;
        return false;
    }

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError); // Emit error
        qWarning() << "Update Exec Failed:" << sql << "Error:" << m_lastError;
        return false;
    }
    return true;
}

bool DatabaseManager::remove(const QString &tableName, const QString &whereClause)
{
    if (!isConnected()) { // Added connection check
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return false;
    }
    if (whereClause.isEmpty()) {
        m_lastError = "Delete operation must have a WHERE clause.";
        emit errorOccurred(m_lastError); // Emit error
        return false;
    }
    QString sql = QString("DELETE FROM %1 WHERE %2").arg(tableName, whereClause);
    return execute(sql);
}

bool DatabaseManager::beginTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return false;
    }
    if (!m_db.transaction()) {
        m_lastError = m_db.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }
    return true;
}

bool DatabaseManager::commitTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return false;
    }
    if (!m_db.commit()) {
        m_lastError = m_db.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }
    return true;
}

bool DatabaseManager::rollbackTransaction()
{
    if (!isConnected()) {
        m_lastError = "Database is not connected.";
        emit errorOccurred(m_lastError);
        return false;
    }
    if (!m_db.rollback()) {
        m_lastError = m_db.lastError().text();
        emit errorOccurred(m_lastError);
        return false;
    }
    return true;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

bool DatabaseManager::createTables()
{
    if (!beginTransaction()) {
        // m_lastError is already set by beginTransaction()
        return false;
    }

    bool success = true;

    // Define all table creation queries
    const QList<QString> createQueries = {
        R"(CREATE TABLE IF NOT EXISTS users (user_id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, email TEXT NOT NULL UNIQUE, password_hash TEXT NOT NULL, role TEXT NOT NULL CHECK(role IN ('admin', 'doctor', 'patient')), created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP))",
        R"(CREATE TABLE IF NOT EXISTS admins (admin_id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL UNIQUE, FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS patients (patient_id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL UNIQUE, full_name TEXT, sex TEXT CHECK(sex IN ('男', '女')), id_card_no TEXT UNIQUE, birth_date DATE, age INTEGER, phone_no TEXT UNIQUE, address TEXT, sos_name TEXT, sos_phone_no TEXT UNIQUE, FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS doctors (doctor_id TEXT PRIMARY KEY, user_id INTEGER NOT NULL UNIQUE, full_name TEXT, sex TEXT CHECK(sex IN ('男', '女')), age INTEGER, department TEXT, title TEXT, phone_no TEXT UNIQUE, doc_start TIME, doc_finish TIME, registration_fee REAL, patient_limit INTEGER, photo_url TEXT, description TEXT, FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS appointments (appointment_id INTEGER PRIMARY KEY AUTOINCREMENT, patient_id INTEGER NOT NULL, doctor_id TEXT NOT NULL, appointment_date DATE NOT NULL, appointment_time DATETIME NOT NULL, status TEXT NOT NULL DEFAULT 'scheduled' CHECK(status IN ('scheduled', 'completed', 'cancelled')), payment_status TEXT NOT NULL DEFAULT 'unpaid' CHECK(payment_status IN ('unpaid', 'paid')), FOREIGN KEY (patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE, FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS medical_records (record_id INTEGER PRIMARY KEY AUTOINCREMENT, appointment_id INTEGER NOT NULL UNIQUE, diagnosis_notes TEXT, diagnosis_date DATE, created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY (appointment_id) REFERENCES appointments(appointment_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS hospitalizations (hospitalization_id INTEGER PRIMARY KEY AUTOINCREMENT, record_id INTEGER NOT NULL UNIQUE, doctor_id TEXT NOT NULL, ward_no TEXT, bed_no TEXT, admission_date DATE, discharge_date DATE, FOREIGN KEY (record_id) REFERENCES medical_records(record_id) ON DELETE CASCADE, FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS prescriptions (prescription_id INTEGER PRIMARY KEY AUTOINCREMENT, record_id INTEGER NOT NULL, details TEXT, issued_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY (record_id) REFERENCES medical_records(record_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS drugs (drug_id INTEGER PRIMARY KEY AUTOINCREMENT, drug_name TEXT NOT NULL UNIQUE, description TEXT, usage TEXT, precautions TEXT, drug_price REAL, image_url TEXT))",
        R"(CREATE TABLE IF NOT EXISTS chat_messages (message_id INTEGER PRIMARY KEY AUTOINCREMENT, sender_id INTEGER NOT NULL, receiver_id INTEGER NOT NULL, content TEXT NOT NULL, sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY (sender_id) REFERENCES users(user_id) ON DELETE CASCADE, FOREIGN KEY (receiver_id) REFERENCES users(user_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS case_templates (template_id INTEGER PRIMARY KEY AUTOINCREMENT, created_by_doctor_id TEXT NOT NULL, template_name TEXT NOT NULL, default_diagnosis TEXT, default_prescription TEXT, FOREIGN KEY (created_by_doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE))",
        R"(CREATE TABLE IF NOT EXISTS attendance (attendance_id INTEGER PRIMARY KEY AUTOINCREMENT, doctor_id TEXT NOT NULL, date DATE NOT NULL, check_in_time TIME, check_out_time TIME, FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE, UNIQUE(doctor_id, date)))",
        R"(CREATE TABLE IF NOT EXISTS leave_requests (request_id INTEGER PRIMARY KEY AUTOINCREMENT, doctor_id TEXT NOT NULL, request_type TEXT NOT NULL CHECK(request_type IN ('因公', '因私')), start_date DATE NOT NULL, end_date DATE NOT NULL, reason TEXT, status TEXT NOT NULL DEFAULT '未销假' CHECK(status IN ('已销假', '未销假')), FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE))"
    };

    // Execute each query
    for (const QString& sql : createQueries) {
        if (!execute(sql)) { // execute already emits errors and sets m_lastError
            success = false;
            // m_lastError is already set by execute
            break; // Stop on first error
        }
    }

    // Commit or rollback based on success
    if (success) {
        if (!commitTransaction()) {
            // m_lastError is already set by commitTransaction()
            success = false;
        }
    } else {
        rollbackTransaction(); // Rollback will also update m_lastError via the execute() call
    }

    if(success) {
        qDebug() << "Database tables created/verified successfully.";
    } else {
        qWarning() << "Database table creation failed:" << m_lastError;
    }

    return success;
}

// --- High-level Query Implementations ---

DatabaseManager::ResultSet DatabaseManager::getDoctorsByDepartment(const QString& department)
{
    QSqlQuery query(m_db);
    QString sql = R"(
        SELECT d.doctor_id, d.full_name, d.sex, d.age, d.department, d.title,
               d.phone_no, d.doc_start, d.doc_finish, d.registration_fee,
               d.patient_limit, d.photo_url, d.description, u.email
        FROM doctors d JOIN users u ON d.user_id = u.user_id
    )";
    if (!department.isEmpty()) {
        sql += " WHERE d.department = :department";
    }
    sql += " ORDER BY d.department, d.full_name";

    query.prepare(sql);
    if (!department.isEmpty()) {
        query.bindValue(":department", department);
    }

    if (!query.exec()) { // Execute the query here
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "getDoctorsByDepartment Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return this->query(query); // Pass the executed query to the overloaded method
}

DatabaseManager::ResultSet DatabaseManager::getPatientAppointments(int patientId, const QDate& date)
{
    QString sql = R"(
        SELECT a.appointment_id, a.appointment_date, a.appointment_time,
               a.status, a.payment_status, d.doctor_id, d.full_name as doctor_name,
               d.department, d.title, d.registration_fee
        FROM appointments a JOIN doctors d ON a.doctor_id = d.doctor_id
        WHERE a.patient_id = :patientId
    )";
    if (date.isValid()) {
        sql += " AND a.appointment_date = :date";
    }
    sql += " ORDER BY a.appointment_time DESC";

    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":patientId", patientId);
    if (date.isValid()) {
        query.bindValue(":date", date.toString(Qt::ISODate));
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "getPatientAppointments Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return this->query(query);
}

DatabaseManager::ResultSet DatabaseManager::getDoctorSchedule(const QString& doctorId, const QDate& date)
{
    QString sql = R"(
        SELECT a.appointment_time, a.status, p.full_name as patient_name, p.phone_no as patient_phone
        FROM appointments a JOIN patients p ON a.patient_id = p.patient_id
        WHERE a.doctor_id = :doctorId AND a.appointment_date = :date
        ORDER BY a.appointment_time
    )";
    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":doctorId", doctorId);
    query.bindValue(":date", date.toString(Qt::ISODate));

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "getDoctorSchedule Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return this->query(query);
}

bool DatabaseManager::isTimeSlotAvailable(const QString& doctorId, const QDateTime& dateTime)
{
    QString sql = R"(
        SELECT COUNT(*) FROM appointments
        WHERE doctor_id = :doctorId AND appointment_time = :dateTime AND status != 'cancelled'
    )";
    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":doctorId", doctorId);
    query.bindValue(":dateTime", dateTime.toString(Qt::ISODate));

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt() == 0;
        }
    }
    m_lastError = query.lastError().text();
    emit errorOccurred(m_lastError);
    qWarning() << "isTimeSlotAvailable Failed:" << sql << "Error:" << m_lastError;
    return false;
}

DatabaseManager::ResultSet DatabaseManager::getPatientMedicalRecords(int patientId)
{
    QString sql = R"(
        SELECT mr.record_id, mr.diagnosis_notes, mr.diagnosis_date, mr.created_at,
               a.appointment_time, d.doctor_id, d.full_name as doctor_name, d.department, d.title,
               GROUP_CONCAT(pr.details, '; ') as prescriptions
        FROM medical_records mr
        JOIN appointments a ON mr.appointment_id = a.appointment_id
        JOIN doctors d ON a.doctor_id = d.doctor_id
        LEFT JOIN prescriptions pr ON mr.record_id = pr.record_id
        WHERE a.patient_id = :patientId
        GROUP BY mr.record_id
        ORDER BY mr.created_at DESC
    )";
    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "getPatientMedicalRecords Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return this->query(query);
}

DatabaseManager::ResultSet DatabaseManager::getChatHistory(int user1Id, int user2Id, int limit)
{
    QString sql = R"(
        SELECT cm.message_id, cm.sender_id, cm.receiver_id, cm.content, cm.sent_at,
               u1.username as sender_name, u2.username as receiver_name
        FROM chat_messages cm
        JOIN users u1 ON cm.sender_id = u1.user_id
        JOIN users u2 ON cm.receiver_id = u2.user_id
        WHERE (cm.sender_id = :user1 AND cm.receiver_id = :user2)
           OR (cm.sender_id = :user2 AND cm.receiver_id = :user1)
        ORDER BY cm.sent_at DESC
        LIMIT :limit
    )";
    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":user1", user1Id);
    query.bindValue(":user2", user2Id);
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        emit errorOccurred(m_lastError);
        qWarning() << "getChatHistory Failed:" << sql << "Error:" << m_lastError;
        return ResultSet();
    }
    return this->query(query);
}

// File 4: databaseserver.cpp
#include "databaseserver.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QUuid> // For generating unique connection names

// Define a global, unique connection name for the database configuration
const QString GLOBAL_DB_CONNECTION_NAME = "SmartMedicalDBConnection";

// =================================================================
// ClientWorker Implementation
// =================================================================

ClientWorker::ClientWorker(QTcpSocket* socket, const QString& connectionName, QObject* parent)
    : QObject(parent),
      m_socket(socket),
      m_connectionName(connectionName),
      m_dbManager(nullptr) // Initialize as null; it will be created within its own thread
{
    // The socket's parent will be set to this worker, ensuring it's managed in the worker's thread
    m_socket->setParent(this);

    // Connect socket signals to worker's slots
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientWorker::processSocketReads);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientWorker::disconnected);
}

ClientWorker::~ClientWorker()
{
    qDebug() << "ClientWorker in thread" << QThread::currentThread() << "is being destroyed.";
    // Clean up the DatabaseManager instance to prevent memory leaks
    if (m_dbManager) {
        delete m_dbManager;
        m_dbManager = nullptr;
    }
}

void ClientWorker::initializeInThread()
{
    // This function is executed within the worker's thread after it starts
    qDebug() << "Initializing DatabaseManager for thread:" << QThread::currentThread();

    // 1. Clone the global database configuration to create a unique, independent connection for this thread
    // Use QUuid to generate a truly unique connection name, more robust than reinterpret_cast
    QString threadConnectionName = GLOBAL_DB_CONNECTION_NAME + "_" + QUuid::createUuid().toString(QUuid::Id128);

    // Fix: Explicitly cast m_connectionName and threadConnectionName to const QString&
    QSqlDatabase db = QSqlDatabase::cloneDatabase(static_cast<const QString&>(m_connectionName), static_cast<const QString&>(threadConnectionName));

    if (!db.isValid()) {
        qWarning() << "Worker thread" << QThread::currentThread() << "failed to clone database connection named:" << m_connectionName;
        emit sendResponse(m_socket, createResponse(false, QVariant(), "Server error: cannot prepare database connection."));
        return;
    }

    if (!db.open()) {
        qWarning() << "Worker thread" << QThread::currentThread() << "failed to open database connection:" << db.lastError().text();
        emit sendResponse(m_socket, createResponse(false, QVariant(), "Server error: cannot connect to database."));
        // Ensure the invalid connection is removed
        QSqlDatabase::removeDatabase(threadConnectionName);
        return;
    }

    // 2. Create a new DatabaseManager instance for this thread and assign the thread-local database connection to it
    m_dbManager = new DatabaseManager();
    m_dbManager->setDatabase(db); // This is where the QSqlDatabase object is passed

    // Also connect the error signal from the DB manager to this worker's error signal
    // This allows database errors to be propagated back to the client, or at least logged.
    connect(m_dbManager, &DatabaseManager::errorOccurred, this, [this](const QString& errorMessage){
        qWarning() << "DatabaseManager error in worker:" << errorMessage;
        // Optionally, send a response with this error immediately, or let processRequest handle it
        // For simplicity, processRequest already checks m_dbManager->lastError()
    });
}

void ClientWorker::processSocketReads()
{
    m_readBuffer.append(m_socket->readAll());

    while (true) {
        int newlineIndex = m_readBuffer.indexOf('\n');
        if (newlineIndex == -1) {
            break; // No complete message found, wait for more data
        }

        QByteArray message = m_readBuffer.left(newlineIndex).trimmed();
        m_readBuffer.remove(0, newlineIndex + 1);

        if (message.isEmpty()) continue;

        QJsonParseError parseError;
        QJsonDocument requestDoc = QJsonDocument::fromJson(message, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error from client:" << parseError.errorString();
            emit sendResponse(m_socket, createResponse(false, QVariant(), "Invalid JSON request."));
            continue;
        }

        processRequest(requestDoc);
    }
}

void ClientWorker::disconnected()
{
    qInfo() << "Client disconnected from" << m_socket->peerAddress().toString();
    m_socket->deleteLater(); // Schedule the socket for deletion
    emit finished();      // Signal that this worker's job is done
}

void ClientWorker::processRequest(const QJsonDocument& requestDoc)
{
    if (!m_dbManager) {
        initializeInThread();
        if (!m_dbManager || !m_dbManager->isConnected()) {
             qCritical() << "Database is not available for worker thread. Aborting request.";
             emit sendResponse(m_socket, createResponse(false, QVariant(), "Server database is not available."));
             return;
        }
    }

    QJsonObject requestObj = requestDoc.object();
    QString methodName = requestObj["method"].toString();
    QJsonArray argsArray = requestObj["args"].toArray();

    QVariant result;
    bool success = true;
    QString errorMessage;

    qDebug() << "Received request:" << methodName << "from" << m_socket->peerAddress().toString();

    try {
        if (methodName == "isConnected") {
            success = m_dbManager->isConnected();
            result = success; // Return true/false directly
            if (!success) errorMessage = m_dbManager->lastError();
        } else if (methodName == "lastError") { // Added lastError retrieval
            result = m_dbManager->lastError();
            success = true; // Retrieving error message is always "successful"
        } else if (methodName == "initDatabase") {
            success = m_dbManager->initDatabase();
            if (!success) errorMessage = m_dbManager->lastError();
        } else if (methodName == "execute") {
            if (argsArray.size() == 1 && argsArray[0].isString()) {
                success = m_dbManager->execute(argsArray[0].toString());
                if (!success) errorMessage = m_dbManager->lastError();
            } else { success = false; errorMessage = "Invalid arguments for execute (expected 1 string)."; }
        } else if (methodName == "query") {
            if (argsArray.size() == 1 && argsArray[0].isString()) {
                DatabaseManager::ResultSet rs = m_dbManager->query(argsArray[0].toString());
                if (!m_dbManager->lastError().isEmpty()) { // Check for errors reported by query method
                    success = false; errorMessage = m_dbManager->lastError();
                } else {
                    result = QVariant::fromValue(rs);
                    success = true;
                }
            } else { success = false; errorMessage = "Invalid arguments for query (expected 1 string)."; }
        } else if (methodName == "insert") {
            if (argsArray.size() == 2 && argsArray[0].isString() && argsArray[1].isObject()) {
                success = m_dbManager->insert(argsArray[0].toString(), parseDataRowFromJson(argsArray[1].toObject()));
                if (!success) errorMessage = m_dbManager->lastError();
            } else { success = false; errorMessage = "Invalid arguments for insert (expected 1 string, 1 object)."; }
        } else if (methodName == "update") {
            if (argsArray.size() == 3 && argsArray[0].isString() && argsArray[1].isObject() && argsArray[2].isString()) {
                success = m_dbManager->update(argsArray[0].toString(), parseDataRowFromJson(argsArray[1].toObject()), argsArray[2].toString());
                if (!success) errorMessage = m_dbManager->lastError();
            } else { success = false; errorMessage = "Invalid arguments for update (expected 1 string, 1 object, 1 string)."; }
        } else if (methodName == "remove") {
            if (argsArray.size() == 2 && argsArray[0].isString() && argsArray[1].isString()) {
                success = m_dbManager->remove(argsArray[0].toString(), argsArray[1].toString());
                if (!success) errorMessage = m_dbManager->lastError();
            } else { success = false; errorMessage = "Invalid arguments for remove (expected 1 string, 1 string)."; }
        } else if (methodName == "beginTransaction") {
            success = m_dbManager->beginTransaction();
            if (!success) errorMessage = m_dbManager->lastError();
        } else if (methodName == "commitTransaction") {
            success = m_dbManager->commitTransaction();
            if (!success) errorMessage = m_dbManager->lastError();
        } else if (methodName == "rollbackTransaction") {
            success = m_dbManager->rollbackTransaction();
            if (!success) errorMessage = m_dbManager->lastError();
        }
        // High-level query methods
        else if (methodName == "getDoctorsByDepartment") {
            if (argsArray.size() == 1 && argsArray[0].isString()) {
                DatabaseManager::ResultSet rs = m_dbManager->getDoctorsByDepartment(argsArray[0].toString());
                if (!m_dbManager->lastError().isEmpty()) {
                    success = false; errorMessage = m_dbManager->lastError();
                } else {
                    result = QVariant::fromValue(rs);
                    success = true;
                }
            } else { success = false; errorMessage = "Invalid arguments for getDoctorsByDepartment (expected 1 string)."; }
        } else if (methodName == "getPatientAppointments") {
            if (argsArray.size() >= 1 && argsArray.size() <= 2 && argsArray[0].isDouble()) {
                int patientId = argsArray[0].toInt();
                QDate date;
                if (argsArray.size() == 2 && argsArray[1].isString()) {
                    date = QDate::fromString(argsArray[1].toString(), Qt::ISODate);
                    if (!date.isValid()) {
                        success = false; errorMessage = "Invalid date format for getPatientAppointments.";
                    }
                }
                if (success) { // Only proceed if date parsing was successful
                    DatabaseManager::ResultSet rs = m_dbManager->getPatientAppointments(patientId, date);
                    if (!m_dbManager->lastError().isEmpty()) {
                        success = false; errorMessage = m_dbManager->lastError();
                    } else {
                        result = QVariant::fromValue(rs);
                        success = true;
                    }
                }
            } else { success = false; errorMessage = "Invalid arguments for getPatientAppointments (expected 1 int, optional 1 date string)."; }
        } else if (methodName == "getDoctorSchedule") {
            if (argsArray.size() == 2 && argsArray[0].isString() && argsArray[1].isString()) {
                QDate date = QDate::fromString(argsArray[1].toString(), Qt::ISODate);
                if (!date.isValid()) { success = false; errorMessage = "Invalid date format for getDoctorSchedule."; }
                else {
                    DatabaseManager::ResultSet rs = m_dbManager->getDoctorSchedule(argsArray[0].toString(), date);
                    if (!m_dbManager->lastError().isEmpty()) {
                        success = false; errorMessage = m_dbManager->lastError();
                    } else {
                        result = QVariant::fromValue(rs);
                        success = true;
                    }
                }
            } else { success = false; errorMessage = "Invalid arguments for getDoctorSchedule (expected 1 string, 1 date string)."; }
        } else if (methodName == "isTimeSlotAvailable") {
            if (argsArray.size() == 2 && argsArray[0].isString() && argsArray[1].isString()) {
                QDateTime dateTime = QDateTime::fromString(argsArray[1].toString(), Qt::ISODate);
                if (!dateTime.isValid()) { success = false; errorMessage = "Invalid datetime format for isTimeSlotAvailable."; }
                else {
                    result = m_dbManager->isTimeSlotAvailable(argsArray[0].toString(), dateTime);
                    if (!m_dbManager->lastError().isEmpty() && !result.toBool()) { // Error only if false AND dbmanager reports error
                        success = false; errorMessage = m_dbManager->lastError();
                    } else {
                        success = true;
                    }
                }
            } else { success = false; errorMessage = "Invalid arguments for isTimeSlotAvailable (expected 1 string, 1 datetime string)."; }
        } else if (methodName == "getPatientMedicalRecords") {
            if (argsArray.size() == 1 && argsArray[0].isDouble()) {
                DatabaseManager::ResultSet rs = m_dbManager->getPatientMedicalRecords(argsArray[0].toInt());
                if (!m_dbManager->lastError().isEmpty()) {
                    success = false; errorMessage = m_dbManager->lastError();
                } else {
                    result = QVariant::fromValue(rs);
                    success = true;
                }
            } else { success = false; errorMessage = "Invalid arguments for getPatientMedicalRecords (expected 1 int)."; }
        } else if (methodName == "getChatHistory") {
            if (argsArray.size() >= 2 && argsArray.size() <= 3 && argsArray[0].isDouble() && argsArray[1].isDouble()) {
                int user1Id = argsArray[0].toInt();
                int user2Id = argsArray[1].toInt();
                int limit = 50;
                if (argsArray.size() == 3 && argsArray[2].isDouble()) {
                    limit = argsArray[2].toInt();
                }
                DatabaseManager::ResultSet rs = m_dbManager->getChatHistory(user1Id, user2Id, limit);
                if (!m_dbManager->lastError().isEmpty()) {
                    success = false; errorMessage = m_dbManager->lastError();
                } else {
                    result = QVariant::fromValue(rs);
                    success = true;
                }
            } else { success = false; errorMessage = "Invalid arguments for getChatHistory (expected 2 int, optional 1 int)."; }
        }
        else {
            success = false;
            errorMessage = "Unknown method: " + methodName;
        }
    } catch (const std::exception& e) {
        success = false;
        errorMessage = QString("Server internal error: %1").arg(e.what());
    } catch (...) {
        success = false;
        errorMessage = "Unknown server internal error.";
    }

    emit sendResponse(m_socket, createResponse(success, result, errorMessage));
}

QJsonDocument ClientWorker::createResponse(bool success, const QVariant& result, const QString& errorMessage) {
    QJsonObject responseObj;
    responseObj["success"] = success;
    if (success) {
        // Handle ResultSet specifically, otherwise use QJsonValue::fromVariant
        if (result.canConvert<DatabaseManager::ResultSet>()) {
            responseObj["result"] = serializeResultSetToJson(result.value<DatabaseManager::ResultSet>());
        } else if (result.isValid()) {
            responseObj["result"] = QJsonValue::fromVariant(result);
        }
        // If result is not valid (e.g., for methods returning just bool like insert/update),
        // and it's not explicitly set, no "result" key is added, which is fine.
    } else {
        responseObj["error"] = errorMessage;
    }
    return QJsonDocument(responseObj);
}

DatabaseManager::DataRow ClientWorker::parseDataRowFromJson(const QJsonObject& jsonObject) {
    DatabaseManager::DataRow row;
    for (auto it = jsonObject.constBegin(); it != jsonObject.constEnd(); ++it) {
        row[it.key()] = it.value().toVariant();
    }
    return row;
}

QJsonArray ClientWorker::serializeResultSetToJson(const DatabaseManager::ResultSet& resultSet) {
    QJsonArray jsonArray;
    for (const auto& row : resultSet) {
        QJsonObject rowObj;
        for (auto it = row.constBegin(); it != row.constEnd(); ++it) {
            rowObj.insert(it.key(), QJsonValue::fromVariant(it.value()));
        }
        jsonArray.append(rowObj);
    }
    return jsonArray;
}

// =================================================================
// DatabaseServer Implementation
// =================================================================

DatabaseServer::DatabaseServer(QObject *parent) : QTcpServer(parent)
{
    // 1. Add the global database configuration. This step only registers the settings.
    // This connection definition is NOT opened on the main thread and will be cloned by workers.
    QSqlDatabase dbDefinition = QSqlDatabase::addDatabase("QSQLITE", GLOBAL_DB_CONNECTION_NAME);
    dbDefinition.setDatabaseName("/home/ada/QtWorks/build-SmartMedicalv05-Desktop_Qt_5_12_8_GCC_64bit-Debug/Server_main/medical_system.db");

    // 2. Temporarily create and open a dedicated connection on the main thread solely for initialization purposes.
    // This ensures table creation uses the centralized logic from DatabaseManager and cleans up afterward.
    QString mainThreadInitConnectionName = GLOBAL_DB_CONNECTION_NAME + "_init";

    // Fix: Explicitly cast mainThreadInitConnectionName to const QString&
    // GLOBAL_DB_CONNECTION_NAME is already a const QString, so no cast is needed for it.
    QSqlDatabase initDb = QSqlDatabase::cloneDatabase(static_cast<const QString&>(GLOBAL_DB_CONNECTION_NAME), (mainThreadInitConnectionName));

    if (!initDb.isValid()) {
        qCritical() << "FATAL: Could not clone database for server initialization.";
        // It's a fatal error if we can't even get a valid DB object definition.
        // The application should likely terminate or indicate a severe issue.
        // For a server, this is usually a setup problem.
        return; // Server cannot start if init DB is invalid
    }

    if (!initDb.open()) {
        qCritical() << "FATAL: Could not open database for server initialization:" << initDb.lastError().text();
        QSqlDatabase::removeDatabase(mainThreadInitConnectionName); // Clean up the failed connection
        return; // Server cannot start if init DB fails to open
    }

    qInfo() << "Database connection successful for initialization. Path: \"" << initDb.databaseName() << "\"";

    // 3. Use a temporary DatabaseManager instance to create/verify database tables.
    DatabaseManager tempDbManager;
    tempDbManager.setDatabase(initDb); // Assign the temporary connection
    if (!tempDbManager.initDatabase()) { // Call its init method
        qCritical() << "FATAL: Failed to initialize database tables:" << tempDbManager.lastError();
        initDb.close();
        QSqlDatabase::removeDatabase(mainThreadInitConnectionName);
        return; // Server cannot start if table creation fails
    }
    qInfo() << "Database tables initialized/verified successfully.";

    // 4. IMPORTANT: Close and remove the temporary connection on the main thread.
    // This ensures the main thread does not hold an open connection, and the global definition
    // remains ready for cloning by worker threads.
    if (initDb.isOpen()) {
        initDb.close();
    }
    QSqlDatabase::removeDatabase(mainThreadInitConnectionName);
}

bool DatabaseServer::start(quint16 port)
{
    // Listen on all available IPv4 network interfaces. This is standard practice for a server.
    if (!this->listen(QHostAddress::AnyIPv4, port)) {
        qCritical() << QString("Server failed to bind to port [%1] on all interfaces: %2").arg(port).arg(this->errorString());
        return false;
    }

    qInfo() << QString("Server is listening on [%1:%2]").arg(this->serverAddress().toString()).arg(this->serverPort());
    return true;
}

void DatabaseServer::incomingConnection(qintptr socketDescriptor)
{
    qInfo() << "New connection pending...";

    QThread* thread = new QThread(this);
    thread->setObjectName(QString("WorkerThread_%1").arg(socketDescriptor));

    QTcpSocket* socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Failed to set socket descriptor for new connection.";
        delete socket;
        delete thread;
        return;
    }

    // Pass the connection NAME (QString) to the worker.
    ClientWorker* worker = new ClientWorker(socket, GLOBAL_DB_CONNECTION_NAME);
    worker->moveToThread(thread);

    // --- Signal-slot connections for lifecycle management ---
    connect(worker, &ClientWorker::finished, thread, &QThread::quit);
    connect(worker, &ClientWorker::finished, worker, &ClientWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    // NEW: Connect worker finished to server's cleanup slot
    connect(worker, &ClientWorker::finished, this, &DatabaseServer::clientWorkerFinished);
    connect(worker, &ClientWorker::sendResponse, this, &DatabaseServer::handleResponse);

    m_clientThreads.insert(socket, thread); // Store the socket and its thread

    thread->start();

    qInfo() << "Client connection passed to thread" << thread->objectName();
}

void DatabaseServer::handleResponse(QTcpSocket* socket, const QJsonDocument& response)
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(response.toJson(QJsonDocument::Compact) + "\n");
    } else {
        qWarning() << "Attempted to send response to an invalid or disconnected socket. Socket descriptor:" << (socket ? socket->socketDescriptor() : -1);
        // The socket and thread cleanup will be handled by clientWorkerFinished or socket disconnected signal.
        // No removal from m_clientThreads here directly.
    }
}

void DatabaseServer::clientWorkerFinished()
{
    // This slot is called when a ClientWorker emits 'finished'.
    // The 'sender()' is the ClientWorker object.
    ClientWorker* worker = qobject_cast<ClientWorker*>(sender());
    if (worker) {
        QTcpSocket* socket = worker->m_socket; // Access the socket held by the worker
        if (socket) {
            // Find the thread associated with this socket in our map
            QThread* thread = m_clientThreads.value(socket, nullptr);

            if (thread) {
                qDebug() << "Cleaning up finished worker thread for socket:" << socket->peerAddress().toString()
                         << ", thread:" << thread->objectName();
                m_clientThreads.remove(socket); // Remove the entry from the map
                // The worker and thread are already scheduled for deleteLater via their connections
            } else {
                qWarning() << "clientWorkerFinished: Could not find thread for socket" << socket->peerAddress().toString()
                           << "in m_clientThreads map.";
            }
        } else {
            qWarning() << "clientWorkerFinished: Worker had a null socket.";
        }
    } else {
        qWarning() << "clientWorkerFinished: Sender was not a ClientWorker.";
    }
}

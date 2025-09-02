#include "widget.h"

#include <QApplication>
#include "remotedatabasemanager.h"
#include<QDebug>
#include<QDate>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RemoteDatabaseManager client;
    if (client.connectToServer("127.0.0.1", 12345)) { // 连接到服务器
        qInfo() << "Client connected to server.";

        // 示例：使用远程数据库管理器
//        if (client.initDatabase()) { // 让服务器初始化数据库
//            qInfo() << "Server database initialized.";
//        } else {
//            qWarning() << "Failed to initialize server database:" << client.lastError();
//        }


        RemoteDatabaseManager::DataRow doctorData;
        doctorData["name"] = "Dr. Alice";
        doctorData["department"] = "Cardiology";
        doctorData["contact"] = "alice@example.com";
        if (client.insert("Doctors", doctorData)) {
            qInfo() << "Doctor inserted successfully.";
        } else {
            qWarning() << "Failed to insert doctor:" << client.lastError();
        }

        RemoteDatabaseManager::ResultSet doctors = client.getDoctorsByDepartment("Cardiology");
        if (!doctors.empty()) {
            qInfo() << "Doctors in Cardiology:";
            for (const auto& row : doctors) {
                qInfo() << "  ID:" << row.at("id").toInt() << ", Name:" << row.at("name").toString() << ", Contact:" << row.at("contact").toString();
            }
        } else {
            qWarning() << "No doctors found in Cardiology or error:" << client.lastError();
        }

        RemoteDatabaseManager::DataRow patientData;
        patientData["name"] = "John Doe";
        patientData["dob"] = QDate(1990, 5, 15).toString(Qt::ISODate);
        patientData["contact"] = "john@example.com";
        if (client.insert("Patients", patientData)) {
            qInfo() << "Patient inserted successfully.";
        } else {
            qWarning() << "Failed to insert patient:" << client.lastError();
        }

        // 假设ID，实际应从查询结果获取
        int patientId = 1; // 假设John Doe的ID是1
        int doctorId = 1; // 假设Dr. Alice的ID是1

        RemoteDatabaseManager::DataRow appointmentData;
        appointmentData["patientId"] = patientId;
        appointmentData["doctorId"] = doctorId;
        appointmentData["appointmentTime"] = QDateTime(QDate::currentDate().addDays(1), QTime(10, 0)).toString(Qt::ISODate);
        appointmentData["reason"] = "Routine checkup";
        if (client.insert("Appointments", appointmentData)) {
            qInfo() << "Appointment booked successfully.";
        } else {
            qWarning() << "Failed to book appointment:" << client.lastError();
        }

        RemoteDatabaseManager::ResultSet patientAppointments = client.getPatientAppointments(patientId);
        if (!patientAppointments.empty()) {
            qInfo() << "Patient John Doe's Appointments:";
            for (const auto& row : patientAppointments) {
                qInfo() << "  Doctor:" << row.at("doctorName").toString() << ", Time:" << row.at("appointmentTime").toDateTime().toString();
            }
        } else {
            qWarning() << "No appointments for John Doe or error:" << client.lastError();
        }


        client.disconnectFromServer();

    }
    else
    {
        qCritical() << "Client failed to connect to server:" << client.lastError();
    return a.exec();
    }
}




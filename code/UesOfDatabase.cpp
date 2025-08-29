#include "widget.h"
#include <QApplication>
#include "databasemanager.h"
#include <QDebug>

// 演示数据库操作的函数
void demoDatabaseOperations() {
    // 获取数据库管理器实例
    DatabaseManager& dbManager = DatabaseManager::instance();

    // 检查数据库连接状态
    if (!dbManager.isConnected()) {
        qCritical() << "数据库连接失败，无法进行操作";
        return;
    }
    qDebug() << "\n===== 数据库连接成功 =====";

    // 1. 插入演示数据
    qDebug() << "\n===== 插入用户数据 =====";
    // 插入管理员
    DatabaseManager::DataRow adminData;
    adminData["username"] = "王管理员";
    adminData["email"] = "admin@hospital.com";
    adminData["password"] = "admin123"; // 实际应用中应使用加密存储
    adminData["role"] = 0; // 0表示管理员
    bool adminInserted = dbManager.insert("users", adminData);
    qDebug() << "管理员插入" << (adminInserted ? "成功" : "失败") <<
                (adminInserted ? "" : dbManager.lastError());

    // 插入医生
    DatabaseManager::DataRow doctorData;
    doctorData["username"] = "张医生";
    doctorData["email"] = "doctor@hospital.com";
    doctorData["password"] = "doctor123";
    doctorData["role"] = 1; // 1表示医生
    bool doctorInserted = dbManager.insert("users", doctorData);
    qDebug() << "医生插入" << (doctorInserted ? "成功" : "失败") <<
                (doctorInserted ? "" : dbManager.lastError());

    // 2. 查询演示
    qDebug() << "\n===== 查询所有用户 =====";
    DatabaseManager::ResultSet allUsers = dbManager.query("SELECT * FROM users");
    for (const auto& row : allUsers) {
        qDebug() << "ID:" << row.at("id").toInt() <<
                    "姓名:" << row.at("username").toString() <<
                    "邮箱:" << row.at("email").toString() <<
                    "角色:" << row.at("role").toInt();
    }

    // 3. 更新演示
    qDebug() << "\n===== 更新用户信息 =====";
    DatabaseManager::DataRow updateData;
    updateData["password"] = "new_password"; // 更新密码
    bool updated = dbManager.update("users", updateData, "username = '张医生'");
    qDebug() << "密码更新" << (updated ? "成功" : "失败") <<
                (updated ? "" : dbManager.lastError());

    // 4. 条件查询演示
    qDebug() << "\n===== 查询医生角色用户 =====";
    DatabaseManager::ResultSet doctors = dbManager.query("SELECT * FROM users WHERE role = 1");
    for (const auto& row : doctors) {
        qDebug() << "医生信息: " << row.at("username").toString() <<
                    "邮箱:" << row.at("email").toString();
    }

    // 5. 删除演示（实际应用中谨慎使用）
    qDebug() << "\n===== 删除演示数据 =====";
    bool deleted = dbManager.remove("users", "email = 'admin@hospital.com'");
    qDebug() << "管理员删除" << (deleted ? "成功" : "失败") <<
                (deleted ? "" : dbManager.lastError());

    // 6. 事务演示
    qDebug() << "\n===== 事务操作演示 =====";
    if (dbManager.beginTransaction()) {
        // 批量插入操作
        DatabaseManager::DataRow patient1;
        patient1["username"] = "李患者";
        patient1["email"] = "patient1@hospital.com";
        patient1["password"] = "patient123";
        patient1["role"] = 2; // 2表示患者

        DatabaseManager::DataRow patient2;
        patient2["username"] = "赵患者";
        patient2["email"] = "patient2@hospital.com";
        patient2["password"] = "patient123";
        patient2["role"] = 2;

        bool p1Success = dbManager.insert("users", patient1);
        bool p2Success = dbManager.insert("users", patient2);

        if (p1Success && p2Success) {
            dbManager.commitTransaction();
            qDebug() << "事务提交成功，患者数据插入完成";
        } else {
            dbManager.rollbackTransaction();
            qDebug() << "事务回滚，患者数据未保存";
        }
    }

    // 最终数据状态
    qDebug() << "\n===== 最终用户列表 =====";
    DatabaseManager::ResultSet finalUsers = dbManager.query("SELECT * FROM users");
    for (const auto& row : finalUsers) {
        qDebug() << "ID:" << row.at("id").toInt() <<
                    "姓名:" << row.at("username").toString() <<
                    "角色:" << row.at("role").toInt();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化并显示主窗口
    Widget w;
    w.show();

    // 演示数据库功能
    demoDatabaseOperations();

    return a.exec();
}


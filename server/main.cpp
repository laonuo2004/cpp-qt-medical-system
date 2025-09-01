#include <QCoreApplication>
#include <QDebug>
#include <csignal>
#include "networkmanager_server.h"
#include "databasemanager.h"

// 全局指针，用于信号处理
NetworkManagerServer* g_server = nullptr;

// 信号处理函数，用于优雅关闭服务器
void signalHandler(int signal)
{
    qInfo() << "收到信号" << signal << "，正在关闭服务器...";
    if (g_server) {
        g_server->stopServer();
    }
    QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("SmartMedical Server");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("BIT Computer Science Team 12");
    
    qInfo() << "=== 智慧医疗管理系统服务器 ===";
    qInfo() << "版本: 1.0";
    qInfo() << "开发团队: 北京理工大学计算机学院第12组";
    qInfo() << "正在启动服务器...";
    
    // 注册信号处理器，用于优雅关闭
    std::signal(SIGINT, signalHandler);   // Ctrl+C
    std::signal(SIGTERM, signalHandler);  // 终止信号
    
    // 初始化数据库
    DatabaseManager& dbManager = DatabaseManager::instance();
    if (!dbManager.isConnected()) {
        qCritical() << "数据库连接失败，服务器无法启动！";
        return -1;
    }
    qInfo() << "数据库连接成功";
    
    // 创建并启动网络服务器
    NetworkManagerServer server;
    g_server = &server;
    
    // 从命令行参数获取端口号，默认12345
    quint16 port = 12345;
    QStringList args = app.arguments();
    if (args.size() > 1) {
        bool ok;
        quint16 argPort = args[1].toUShort(&ok);
        if (ok && argPort > 1024) {
            port = argPort;
        } else {
            qWarning() << "无效的端口号，使用默认端口" << port;
        }
    }
    
    if (!server.startServer(port)) {
        qCritical() << "服务器启动失败！";
        return -1;
    }
    
    qInfo() << "服务器启动成功，按Ctrl+C停止服务器";
    qInfo() << "=====================================";
    
    // 进入事件循环
    int result = app.exec();
    
    qInfo() << "服务器已停止";
    return result;
}

#include <QCoreApplication>
#include "databaseserver.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DatabaseServer server;
    if (!server.start(12345)) { // 启动服务器并监听端口12345
        return 1;
    }
    qDebug()<<"正在监听";
    return a.exec();
}

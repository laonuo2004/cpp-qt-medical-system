#include <QApplication>
#include <QFile>

#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 加载样式表
    QFile styleFile(":/style/picture/medicaltheme.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
    else
    {
        qWarning("无法加载样式表文件");
    }
    
    // 启动应用程序流程
    Engine::get().startApplicationFlow();
    
    return a.exec();
}
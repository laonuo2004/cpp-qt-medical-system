#include <QApplication>
#include <QFile>

#include "engine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    Engine::get().Login();
    return a.exec();
}

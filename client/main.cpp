#include <QApplication>
#include <QFile>

#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Engine::get().setDarkMode(false);
    Engine::get().startEngine();
    return a.exec();
}

#include <QApplication>

#include "engine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Engine::get().Login();
    return a.exec();
}

#include "loginpanel.h"
#include "engine.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Engine *engine = Engine::instance();
    engine->Login();
    return a.exec();
}

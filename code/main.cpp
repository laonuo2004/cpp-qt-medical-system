#include "loginpanel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginPanel loginPanel;
    loginPanel.show();
    return a.exec();
}

#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>

namespace Ui {
class Manager;
}

class Manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private:
    Ui::Manager *ui;
};

#endif // MANAGER_H

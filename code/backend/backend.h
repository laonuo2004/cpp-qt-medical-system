#ifndef BACKEND_H
#define BACKEND_H

#include <QMainWindow>

namespace Ui {
class Backend;
}

class Backend : public QMainWindow
{
    Q_OBJECT

public:
    explicit Backend(QWidget *parent = nullptr);
    ~Backend();

private:
    Ui::Backend *ui;
};

#endif // BACKEND_H

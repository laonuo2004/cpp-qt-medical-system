#ifndef BACKEND_H
#define BACKEND_H

#include <QMainWindow>

namespace Ui {
class Backend;
}

/**
 * @class Backend
 * @brief 后端界面的主窗口类。
 *
 * 该类表示后端应用程序的主窗口。它的主要控件为一个表格控件，
 * 用于展示从数据库中获取的用户信息，并可以对用户类型进行过滤。
 */

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

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

protected:

    /**
     * @brief 向数据库添加新用户
     *
     * 和菜单栏“添加用户”条目绑定，点击后创建adduser窗口。
     * 能添加管理员、医生、患者三种用户，医生和管理员只能通过后台添加，不能注册。
     *
     * @param userRole 用户类型（0=患者，1=医生，2=后台管理员）。
     */
    void addUser(int userRole);

    /**
     * @brief 打开新对话框
     *
     * 该函数处理后端打开新对话框的操作。
     * 当前应用于添加addDrug和addDepartment对话框。
     * addUser由专门的函数addUser实现。
     */
    void openDialog(QDialog* dialog, const QString& msgTitle, const QString& msgContent);

private:
    Ui::Backend *ui;
};

#endif // BACKEND_H

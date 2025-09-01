#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "global/uicontroller.h"

namespace Ui {
class AddUser;
}

/**
 * @class AddUser
 * @brief 添加用户表单控件
 *
 * 点击添加用户按钮后，弹出此表单。
 * 该表单内容根据用户类型动态变化。
 * 点击确认后，若添加成功，则显示弹窗。
 */

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = nullptr);
    ~AddUser();

    /**
     * @brief 根据用户类型修改表单条目
     *
     * 由于后端能添加管理员、医生、患者三种用户，
     * 所以该控件用于动态为三种用户生成对应的表单条目。
     */
    void buildByRole(int userRole);

private slots:
    void on_confirmBtn_clicked();
    void handleregister();
    void handlefail(const QString &reason);

private:
    Ui::AddUser *ui;
    int usersRole;
};

#endif // ADDUSER_H

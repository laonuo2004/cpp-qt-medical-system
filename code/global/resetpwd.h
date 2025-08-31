#ifndef RESETPWD_H
#define RESETPWD_H

#include <QDialog>

namespace Ui {
class ResetPwd;
}

/**
 * @class ResetPwd
 * @brief 重置密码面板
 *
 * 该类负责处理用户重置密码操作。
 */

class ResetPwd : public QDialog
{
    Q_OBJECT

public:
    explicit ResetPwd(QWidget *parent = nullptr);
    ~ResetPwd();

protected:

    /**
     * @brief 判断重置密码是否成功
     *
     * 获取用户输入新密码，并调用 UIController 进行重置密码。
     * 重置成功后，QDialog发出accept信号，返回登录界面。
     */
    void handleReset();

private:
    Ui::ResetPwd *ui;
};

#endif // RESETPWD_H

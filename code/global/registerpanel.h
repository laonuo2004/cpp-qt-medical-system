#ifndef REGISTERPANEL_H
#define REGISTERPANEL_H

#include <QDialog>

namespace Ui {
class RegisterPanel;
}

/**
 * @class RegisterPanel
 * @brief 注册面板
 *
 * 该类负责处理用户注册操作。
 */

class RegisterPanel : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterPanel(QWidget *parent = nullptr);
    ~RegisterPanel();

protected:

    /**
     * @brief 判断注册是否成功
     *
     * 获取用户输入的用户名和密码等，并调用 UIController 进行注册。
     * 注册成功后，QDialog发出accept信号，返回登录界面。
     */
    void handleRegister();

private:
    Ui::RegisterPanel *ui;
};

#endif // REGISTERPANEL_H

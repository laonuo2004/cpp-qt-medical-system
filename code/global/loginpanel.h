#ifndef LOGINPANEL_H
#define LOGINPANEL_H

#include <QDialog>

namespace Ui {
class LoginPanel;
}

/**
 * @class LoginPanel
 * @brief 登录面板
 *
 * 该类负责处理用户登录与注册操作，提供用户界面与相关操作。
 * 用户可以在此面板上选择注册或登录，并在登录成功后获取主窗口类型（患者、医生或后台管理员）。
 * 登录成功后，触发登录成功信号并关闭登录面板。
 */

class LoginPanel : public QDialog
{
    Q_OBJECT

signals:
    void loginSuccessful();

public:
    explicit LoginPanel(QWidget *parent = nullptr);
    int getMainWindowType();
    ~LoginPanel();

protected:

    /* 打开用户注册界面 */
    void userRegister();

    /* 打开忘记密码界面 */
    void forgetPwd();

    /**
     * @brief 判断登录是否成功
     *
     * 获取用户输入的用户名和密码，并调用 UIController 进行登录验证。
     * 登录成功后，QDialog发出accept信号，触发主窗口的创建。
     */
    void handleLogin();



private:
    Ui::LoginPanel *ui;
};

#endif // LOGINPANEL_H

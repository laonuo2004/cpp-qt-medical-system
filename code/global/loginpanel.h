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

    /**
     * @brief 用户注册操作
     *
     * 打开注册面板，用户可以在此输入注册信息并进行注册。
     * 注册成功后，弹出提示框显示注册结果。
     */
    void userRegister();

    /**
     * @brief 用户登录操作
     *
     * 获取用户输入的用户名和密码，并调用 UIController 进行登录验证。
     * 登录成功后，通过发射 loginSuccessful 信号，触发主窗口的创建。
     */
    void userLogin();

private:
    Ui::LoginPanel *ui;
};

#endif // LOGINPANEL_H

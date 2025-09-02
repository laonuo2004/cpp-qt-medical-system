#include "loginpanel.h"
#include "ui_loginpanel.h"
#include "registerpanel.h"
#include "uicontroller.h"
#include "resetpwd.h"
#include "engine.h"
#include <QMessageBox>

LoginPanel::LoginPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);

    /* // TODO:
     * 注意：此处的登录按钮被绑定到了接受对话框，以免除密码验证！仅供测试使用！
     * 后期应将accept替换为handleLogin。
     */

    connect(ui->LoginBtn, &QPushButton::clicked, this, &LoginPanel::accept);
    connect(ui->RegisterBtn, &QPushButton::clicked, this, &LoginPanel::userRegister);
    connect(ui->forgetPwdBtn, &QPushButton::clicked, this, &LoginPanel::forgetPwd);
    connect(ui->chooseRoleBtn, &QPushButton::clicked, this, &LoginPanel::reject);
    connect(this, &LoginPanel::loginSuccessful, this, &LoginPanel::accept);

    UiController& controller = UiController::get();
    connect(&controller, &UiController::loginSuccessAdmin, this, &LoginPanel::handleLogin);
    connect(&controller, &UiController::loginSuccessDoctor, this, &LoginPanel::handleLogin);
    connect(&controller, &UiController::loginSuccessPatient, this, &LoginPanel::handleLogin);
    connect(&controller, &UiController::loginFailed, this, &LoginPanel::onLoginFailed);
}

LoginPanel::~LoginPanel()
{
    delete ui;
}

void LoginPanel::userRegister()
{
    RegisterPanel* registerPanel = new RegisterPanel(this);
    if (registerPanel->exec() == QDialog::Accepted)
    {
        //QMessageBox::information(this, tr("用户注册"), tr("用户注册成功。"));
    }
    delete registerPanel;
}

void LoginPanel::forgetPwd()
{
    ResetPwd* resetPwdPanel = new ResetPwd(this);
    if (resetPwdPanel->exec() == QDialog::Accepted)
    {
        //QMessageBox::information(this, tr("重置密码"), tr("密码修改成功。"));
    }
    delete resetPwdPanel;
}

void LoginPanel::handleLogin(const QString& id)
{

    accept();
}

void LoginPanel::onLoginFailed(const QString &reason)
{
    QMessageBox::warning(this, tr("登录失败"), reason);
}

void LoginPanel::buildByRole(int userRole)
{
    switch (userRole)
    {
    case 0: // 患者身份登录
    {
        ui->HeaderLabel->setText("患者登录");
        ui->userIdentity->setText("邮箱");
        ui->pwdInput->setPlaceholderText("至少包含8位，大小写字母，数字和特殊字符");
        ui->pwdInput->setEchoMode(QLineEdit::Password);
        break;
    }
    case 1: // 医生身份登录
    {
        ui->HeaderLabel->setText("医生登录");
        ui->userIdentity->setText("工号");
        ui->pwdInput->setPlaceholderText("至少包含8位，大小写字母，数字和特殊字符");
        ui->pwdInput->setEchoMode(QLineEdit::Password);
        ui->RegisterBtn->hide();
        break;
    }
    case 2: // 后端管理员身份登录
    {
        ui->HeaderLabel->setText("管理员登录");
        ui->userIdentity->setText("管理员号");
        ui->pwdInput->setPlaceholderText("至少包含8位，大小写字母，数字和特殊字符");
        ui->pwdInput->setEchoMode(QLineEdit::Password);
        ui->RegisterBtn->hide();
        break;
    }
    }
}

void LoginPanel::on_LoginBtn_clicked()
{
    QString phone=ui->userInput->text();
    QString pwd=ui->pwdInput->text();
    UiController::get().login(phone,pwd);

}

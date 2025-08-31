#include "loginpanel.h"
#include "ui_loginpanel.h"
#include "registerpanel.h"
#include "uicontroller.h"
#include "resetpwd.h"
#include <QMessageBox>

LoginPanel::LoginPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);
    connect(ui->LoginBtn, &QPushButton::clicked, this, &LoginPanel::handleLogin);
    connect(ui->RegisterBtn, &QPushButton::clicked, this, &LoginPanel::userRegister);
    connect(ui->forgetPwdBtn, &QPushButton::clicked, this, &LoginPanel::forgetPwd);
    connect(this, &LoginPanel::loginSuccessful, this, &LoginPanel::accept);
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
        QMessageBox::information(this, tr("用户注册"), tr("用户注册成功。"));
    }
    delete registerPanel;
}

void LoginPanel::forgetPwd()
{
    ResetPwd* resetPwdPanel = new ResetPwd(this);
    if (resetPwdPanel->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("重置密码"), tr("密码修改成功。"));
    }
    delete resetPwdPanel;
}

void LoginPanel::handleLogin()
{
    bool isLoginSuccessful = false;

    /*
     * 处理登录逻辑...
     */

    isLoginSuccessful = true; // 待删除，测试时使用

    if (isLoginSuccessful)
    {
        emit accept();
    }
    else
    {
        emit reject();
    }
}

int LoginPanel::getMainWindowType()
{
    return ui->roleInput->currentIndex();
}

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
    
    // 连接UiController的登录信号
    UiController& controller = UiController::get();
    connect(&controller, &UiController::loginSuccessAdmin, this, &LoginPanel::accept);
    connect(&controller, &UiController::loginSuccessDoctor, this, &LoginPanel::accept);
    connect(&controller, &UiController::loginSuccessPatient, this, &LoginPanel::accept);
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
    QString email = ui->EmailEdit->text().trimmed();
    QString password = ui->PasswordEdit->text();
    
    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, tr("登录错误"), tr("请输入邮箱和密码"));
        return;
    }
    
    // 调用UiController进行登录验证
    UiController::get().login(email, password);
    // 不需要在这里处理结果，UiController会发出信号，信号已连接到accept()或onLoginFailed()
}

void LoginPanel::onLoginFailed(const QString &reason)
{
    QMessageBox::warning(this, tr("登录失败"), reason);
}

int LoginPanel::getMainWindowType()
{
    return ui->roleInput->currentIndex();
}

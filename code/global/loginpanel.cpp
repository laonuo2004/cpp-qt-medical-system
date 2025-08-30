#include "loginpanel.h"
#include "ui_loginpanel.h"
#include "registerpanel.h"
#include "uicontroller.h"
#include <QMessageBox>

LoginPanel::LoginPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);
    connect(ui->RegisterBtn, &QPushButton::clicked, this, &LoginPanel::userRegister);
    connect(ui->LoginBtn, &QPushButton::clicked, this, &LoginPanel::userLogin);
    connect(this, &LoginPanel::loginSuccessful, this, &LoginPanel::accept);
}

LoginPanel::~LoginPanel()
{
    delete ui;
}

void LoginPanel::userRegister()
{
    RegisterPanel* registerPanel = new RegisterPanel(this);
    if (registerPanel->exec())
    {
        QMessageBox::information(this, tr("用户注册"), tr("用户注册成功。"));
    }
    delete registerPanel;
}

void LoginPanel::userLogin()
{
    // Login successful logic ...
    UiController::get().login(ui->userInput->text(), ui->pwdInput->text());
    emit loginSuccessful();
}

int LoginPanel::getMainWindowType()
{
    return ui->roleInput->currentIndex();
}

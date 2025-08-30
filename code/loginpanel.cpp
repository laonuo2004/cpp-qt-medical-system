#include "loginpanel.h"
#include "ui_loginpanel.h"
#include "registerpanel.h"
#include "patientclient.h"
#include "doctorclient.h"
#include "manager.h"

#include <QMessageBox>

LoginPanel::LoginPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);
    connect(this, &LoginPanel::loginSuccessful, this, &LoginPanel::accept);
}

LoginPanel::~LoginPanel()
{
    delete ui;
}

void LoginPanel::on_RegisterBtn_clicked()
{
    RegisterPanel* registerPanel = new RegisterPanel(this);
    if (registerPanel->exec())
    {
        QMessageBox::information(this, tr("用户注册"), tr("用户注册成功。"));
    }
    delete registerPanel;
}

void LoginPanel::on_LoginBtn_clicked()
{
    // Login successful logic ...
    emit loginSuccessful();
}

int LoginPanel::getMainWindowType()
{
    return ui->roleInput->currentIndex();
}

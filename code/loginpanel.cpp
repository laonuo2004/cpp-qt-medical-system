#include "loginpanel.h"
#include "ui_loginpanel.h"
#include "registerpanel.h"

#include <QMessageBox>

LoginPanel::LoginPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPanel)
{
    ui->setupUi(this);
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
}

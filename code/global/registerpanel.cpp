#include "registerpanel.h"
#include "ui_registerpanel.h"
#include <QDebug>

RegisterPanel::RegisterPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPanel)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &RegisterPanel::handleRegister);
}

RegisterPanel::~RegisterPanel()
{
    delete ui;
}

void RegisterPanel::handleRegister()
{
    bool isRegisterSuccessful = false;

    /*
     * 处理重置密码逻辑...
     */

    isRegisterSuccessful = true; // 待删除，测试时使用

    if (isRegisterSuccessful)
    {
        emit accept();
    }
    else
    {
        emit reject();
    }
}

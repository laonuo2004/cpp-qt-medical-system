#include "registerinfo.h"
#include "ui_registerinfo.h"

RegisterInfo::RegisterInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterInfo)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &RegisterInfo::handlePatientRegister);
}

RegisterInfo::~RegisterInfo()
{
    delete ui;
}

void RegisterInfo::handlePatientRegister()
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

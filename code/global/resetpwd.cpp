#include "resetpwd.h"
#include "ui_resetpwd.h"

ResetPwd::ResetPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetPwd)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &ResetPwd::handleReset);
}


ResetPwd::~ResetPwd()
{
    delete ui;
}

void ResetPwd::handleReset()
{
    bool isResetSuccessful = false;

    /*
     * 处理重置密码逻辑...
     */

    isResetSuccessful = true; // 待删除，测试时使用

    if (isResetSuccessful)
    {
        emit accept();
    }
    else
    {
        emit reject();
    }
}

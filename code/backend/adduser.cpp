#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::buildByRole(int userRole)
{
    switch (userRole)
    {
    case 0: // 患者身份登录
    {
        setWindowTitle("添加患者用户");
        ui->userIdentity->setText("邮箱");
        break;
    }
    case 1: // 医生身份登录
    {
        setWindowTitle("添加医生用户");
        ui->userIdentity->setText("工号");
        break;
    }
    case 2: // 后端管理员身份登录
    {
        setWindowTitle("添加管理员用户");
        ui->userIdentity->setText("管理员号");
        break;
    }
    }
}

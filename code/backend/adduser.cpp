#include "adduser.h"
#include "ui_adduser.h"
#include<QDebug>

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(&UiController::get(),&UiController::registrationFailed,this,&AddUser::handlefail);
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::buildByRole(int userRole)
{
    usersRole=userRole;
    ui->lineEdit_2->setPlaceholderText("至少包含8位，大小写字母，数字和特殊字符");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    switch (userRole)
    {
    case 0: // 患者身份登录
    {
        setWindowTitle("添加患者用户");
        ui->userIdentity->setText("手机号");
        break;
    }
    case 1: // 医生身份登录
    {
        setWindowTitle("添加医生用户");
        ui->userIdentity->setText("手机号");
        break;
    }
    case 2: // 后端管理员身份登录
    {
        setWindowTitle("添加管理员用户");
        ui->userIdentity->setText("手机号");
        break;
    }
    }
}
void AddUser::handleregister()
{
    switch (usersRole)
    {
    case 0: // 患者身份登录
    {

        QString username = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();
        UiController::get().registerUser(username,password,UserRole::Patient );
        break;
    }
    case 1: // 医生身份登录
    {

        QString username = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();

        UiController::get().registerUser(username,password,UserRole::Doctor);
        break;
    }
    case 2: // 后端管理员身份登录
    {

        QString username = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();
        UiController::get().registerUser(username,password,UserRole::Admin);
        break;
    }
    }
}
void AddUser::on_confirmBtn_clicked()
{
    handleregister();
}
void AddUser::handlefail(const QString &reason)
{
    qDebug()<<reason;

    return;
}

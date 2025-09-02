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
        QString email = ui->lineEdit->text();  // lineEdit应该是邮箱输入框
        QString password = ui->lineEdit_2->text();
        QString username = email;  // 使用邮箱作为用户名，或者可以要求用户输入独立的用户名
        UiController::get().registerUser(username, email, password, UserRole::Patient);
        break;
    }
    case 1: // 医生身份登录
    {
        QString email = ui->lineEdit->text();  // lineEdit应该是邮箱输入框
        QString password = ui->lineEdit_2->text();
        QString username = email;  // 使用邮箱作为用户名，或者可以要求用户输入独立的用户名
        UiController::get().registerUser(username, email, password, UserRole::Doctor);
        break;
    }
    case 2: // 后端管理员身份登录
    {
        QString email = ui->lineEdit->text();  // lineEdit应该是邮箱输入框
        QString password = ui->lineEdit_2->text();
        QString username = email;  // 使用邮箱作为用户名，或者可以要求用户输入独立的用户名
        UiController::get().registerUser(username, email, password, UserRole::Admin);
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

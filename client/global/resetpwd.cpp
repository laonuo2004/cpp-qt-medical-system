#include "resetpwd.h"
#include "ui_resetpwd.h"
#include <QMessageBox>
#include <QDebug>

ResetPwd::ResetPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetPwd),
    m_controller(UiController::get())
{
    ui->setupUi(this);
    ui->NewPasswordEdit->setPlaceholderText("至少包含8位，大小写字母，数字和特殊字符");
    ui->NewPasswordEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &ResetPwd::handleReset);
    
    // 连接UiController的密码重置信号
    connect(&m_controller, &UiController::passwordResetSuccess, this, &ResetPwd::onPasswordResetSuccess);
    connect(&m_controller, &UiController::passwordResetFailed, this, &ResetPwd::onPasswordResetFailed);
}


ResetPwd::~ResetPwd()
{
    delete ui;
}

void ResetPwd::handleReset()
{
    QString email = ui->EmailEdit->text().trimmed();
    QString verificationCode = ui->VerificationCodeEdit->text().trimmed();
    QString newPassword = ui->NewPasswordEdit->text();
    
    if (email.isEmpty() || verificationCode.isEmpty() || newPassword.isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请填写所有字段"));
        return;
    }
    
    // 调用UiController进行密码重置
    m_controller.resetPassword(email, verificationCode, newPassword);
}

void ResetPwd::onPasswordResetSuccess()
{
    qDebug() << "Password reset successful!";
    QMessageBox::information(this, tr("重置成功"), tr("密码重置成功！"));
    accept();
}

void ResetPwd::onPasswordResetFailed(const QString &reason)
{
    qDebug() << "Password reset failed:" << reason;
    QMessageBox::warning(this, tr("重置失败"), tr("密码重置失败：") + reason);
}

void ResetPwd::on_sendBtn_clicked()
{
    QString email=ui->EmailEdit->text();
    UiController::get().forgotPassword(email);

}

void ResetPwd::on_confirmBtn_clicked()
{
    QString email=ui->EmailEdit->text();
    QString veri=ui->VerificationCodeEdit->text();
    QString pass=ui->NewPasswordEdit->text();
    QString confirm=ui->lineEdit_3->text();
    if(pass!=confirm)
    {
        QMessageBox::warning(this, tr("注册错误"), tr("密码设置不一致"));
    }
    UiController::get().resetPassword(email,veri,pass);
}

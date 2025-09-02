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

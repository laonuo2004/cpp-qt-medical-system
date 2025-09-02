#include "registerinfo.h"
#include "ui_registerinfo.h"

RegisterInfo::RegisterInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterInfo)
{
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &RegisterInfo::handlePatientRegister);
}

RegisterInfo::RegisterInfo(const QVariantMap& doctorInfo, const QString& timeslot, QWidget *parent)
    : QDialog(parent), ui(new Ui::RegisterInfo),
      m_doctorInfo(doctorInfo), m_timeslot(timeslot)
{
    ui->setupUi(this);
    applyToUi();
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

void RegisterInfo::applyToUi()
{
    ui->label_2->setText(m_doctorInfo.value("department").toString());
    ui->label_4->setText(m_doctorInfo.value("full_name").toString());
    ui->label_6->setText(m_timeslot);
    ui->label_8->setText(m_doctorInfo.value("registration_fee").toString());
}

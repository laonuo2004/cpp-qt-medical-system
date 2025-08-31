#include "doctorinfo.h"
#include "ui_doctorinfo.h"
#include "registerinfo.h"

#include <QMessageBox>

DoctorInfo::DoctorInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &DoctorInfo::patientRegister);
}

DoctorInfo::~DoctorInfo()
{
    delete ui;
}

void DoctorInfo::patientRegister()
{
    RegisterInfo* registerPanel = new RegisterInfo(this);
    if (registerPanel->exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("挂号"), tr("挂号成功。"));
    }
    else
    {
        // 处理具体失败的原因
    }
    delete registerPanel;
}

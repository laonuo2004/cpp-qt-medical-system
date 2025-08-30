#include "doctorinfo.h"
#include "ui_doctorinfo.h"

DoctorInfo::DoctorInfo(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
}

DoctorInfo::~DoctorInfo()
{
    delete ui;
}

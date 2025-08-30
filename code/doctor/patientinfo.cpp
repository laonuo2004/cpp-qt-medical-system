#include "patientinfo.h"
#include "ui_patientinfo.h"

PatientInfo::PatientInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

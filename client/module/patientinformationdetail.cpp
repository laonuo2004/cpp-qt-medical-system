#include "patientinformationdetail.h"
#include "ui_patientinformationdetail.h"

PatientInformationDetail::PatientInformationDetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInformationDetail)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
}

PatientInformationDetail::~PatientInformationDetail()
{
    delete ui;
}

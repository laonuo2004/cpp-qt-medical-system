#include "doctorinformationdetail.h"
#include "ui_doctorinformationdetail.h"

DoctorInformationDetail::DoctorInformationDetail(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInformationDetail)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
}

DoctorInformationDetail::~DoctorInformationDetail()
{
    delete ui;
}

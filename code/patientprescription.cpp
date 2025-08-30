#include "patientprescription.h"
#include "ui_patientprescription.h"

PatientPrescription::PatientPrescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientPrescription)
{
    ui->setupUi(this);
}

PatientPrescription::~PatientPrescription()
{
    delete ui;
}

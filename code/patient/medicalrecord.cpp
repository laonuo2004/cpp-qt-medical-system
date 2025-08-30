#include "medicalrecord.h"
#include "ui_medicalrecord.h"
#include "prescription.h"

MedicalRecord::MedicalRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecord)
{
    ui->setupUi(this);
}

MedicalRecord::~MedicalRecord()
{
    delete ui;
}

void MedicalRecord::on_prescriptionBtn_clicked()
{
    Prescription* patientPrescription = new Prescription(this);
    patientPrescription->exec();
    delete patientPrescription;
}

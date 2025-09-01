#include "medicalrecord.h"
#include "ui_medicalrecord.h"
#include "prescription.h"

MedicalRecord::MedicalRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecord)
{
    ui->setupUi(this);
    connect(ui->prescriptionBtn, &QPushButton::clicked, this, &MedicalRecord::showPrescription);
}

MedicalRecord::~MedicalRecord()
{
    delete ui;
}

void MedicalRecord::showPrescription()
{
    Prescription* patientPrescription = new Prescription(this);
    patientPrescription->exec();
    delete patientPrescription;
}

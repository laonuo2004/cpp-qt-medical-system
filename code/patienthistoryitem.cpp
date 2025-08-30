#include "patienthistoryitem.h"
#include "ui_patienthistoryitem.h"
#include "patientprescription.h"

PatientHistoryItem::PatientHistoryItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientHistoryItem)
{
    ui->setupUi(this);
    setFixedSize(600, 75);
}

PatientHistoryItem::~PatientHistoryItem()
{
    delete ui;
}

void PatientHistoryItem::on_prescriptionBtn_clicked()
{
    PatientPrescription* patientPrescription = new PatientPrescription(this);
    patientPrescription->show();
}

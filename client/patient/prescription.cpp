#include "prescription.h"
#include "ui_prescription.h"
#include "patientchattool.h"

Prescription::Prescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prescription)
{
    ui->setupUi(this);
    connect(ui->chatBtn, &QPushButton::clicked, this, &Prescription::startChat);
}

Prescription::~Prescription()
{
    delete ui;
}

void Prescription::buildUpPrescription(const QVariantMap& prescription)
{
    // 配置处方显示的具体信息。
    ui->prescriptionIdInput->setText(prescription.value("prescription_id").toString());
    ui->dateInput->setText(prescription.value("diagnosis_date").toString());
    ui->departmentInput->setText(prescription.value("department").toString());
    ui->doctorInput->setText(prescription.value("doctor_name").toString());
    ui->prescriptionDetail->setText(prescription.value("details").toString());
    ui->medicalOrder->setText(prescription.value("diagnosis_notes").toString());
}

void Prescription::startChat()
{
    PatientChatTool* chatDialog = new PatientChatTool(this);
    if (chatDialog->exec() == QDialog::Accepted)
    {

    }
    delete chatDialog;
}



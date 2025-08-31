#include "medicalhistoryfunc.h"
#include "ui_medicalhistoryfunc.h"

MedicalHistoryFunc::MedicalHistoryFunc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalHistoryFunc)
{
    ui->setupUi(this);
    controller = new UiController(this);
    connect(ui->searchMedicalHiatory , &QPushButton::clicked , this , &MedicalHistoryFunc::searchMedicalHistory) ;
}

MedicalHistoryFunc::~MedicalHistoryFunc()
{
    delete ui;
}

void MedicalHistoryFunc::searchMedicalHistory()
{
    int patientId = ui->PatientId->text().toUInt();
    QVariantList medicalHistory = controller->getPatientMedicalHistory(patientId);

    QString historyString;
    if (medicalHistory.isEmpty()) {
        historyString = "No medical records found.";
    } else {
        for (const QVariant &record : medicalHistory) {
            QVariantMap recordMap = record.toMap();
            historyString += "Record ID: " + recordMap["record_id"].toString() + "\n";
            historyString += "Appointment ID: " + recordMap["appointment_id"].toString() + "\n";
            historyString += "Diagnosis: " + recordMap["diagnosis_notes"].toString() + "\n";
            historyString += "Date: " + recordMap["diagnosis_date"].toString() + "\n";
            historyString += "Created: " + recordMap["created_at"].toString() + "\n";
            historyString += "----------------------------\n"; // 分隔线
        }
    }

    ui->MedicalHistoryInfo->setPlainText(historyString);

}

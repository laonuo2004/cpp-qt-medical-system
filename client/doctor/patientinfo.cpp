#include "patientinfo.h"
#include "ui_patientinfo.h"
#include "diagnose.h"
#include "doctorchattool.h"
#include "uicontroller.h"
#include "chatdialog.h"
PatientInfo::PatientInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->diagnoseBtn, &QPushButton::clicked, this, &PatientInfo::diagnose);
    connect(ui->chatBtn, &QPushButton::clicked, this, &PatientInfo::startChat);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

void PatientInfo::diagnose()
{
    Diagnose* diagnosePanel = new Diagnose(this);
    diagnosePanel->buildUpDiagnose(m_doctorId, m_patientInfo);
    if (diagnosePanel->exec() == QDialog::Accepted)
    {

    }
    delete diagnosePanel;
}

void PatientInfo::startChat()
{
    ChatDialog* chatDialog = new ChatDialog(this , 12 , 19);
    if (chatDialog->exec() == QDialog::Accepted)
    {

    }
    delete chatDialog;
}

void PatientInfo::buildUpPatientInfo(int doctorId, const QVariantMap& scheduleInfo)
{
    m_doctorId = doctorId;
    QVariantMap patientInfo = UiController::get().getPatientInfo(scheduleInfo.value("patient_id").toInt());
    m_patientInfo = patientInfo;

    ui->nameInput->setText(patientInfo.value("full_name").toString());
    ui->genderInput->setText(patientInfo.value("sex").toString());
    ui->ageInput->setText(patientInfo.value("age").toString());
    ui->contactInput->setText(patientInfo.value("phone_no").toString());
}

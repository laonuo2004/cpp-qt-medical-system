#include "patientcard.h"
#include "ui_patientcard.h"
#include "patientinfo.h"
#include "uicontroller.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

PatientCard::PatientCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(120, 150);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &PatientCard::openDetailPanel);
}

PatientCard::~PatientCard()
{
    delete ui;
}

void PatientCard::openDetailPanel()
{
    PatientInfo* detailPanel = new PatientInfo(this);
    detailPanel->buildUpPatientInfo(m_doctorId, m_scheduleInfo);
    detailPanel->exec();
    delete detailPanel;
}

void PatientCard::buildUpPatientCard(int doctorId, QVariantMap &scheduleInfo)
{
    m_scheduleInfo = scheduleInfo;
    m_doctorId = doctorId;

    QVariantMap patientInfo = UiController::get().getPatientInfo(scheduleInfo.value("patient_id").toInt());
    ui->nameInput->setText(patientInfo.value("full_name").toString());
    ui->ageInput->setText(patientInfo.value("age").toString());
    ui->dateInput->setText(scheduleInfo.value("appointment_time").toString());
}

#include "reportpage.h"
#include "ui_reportpage.h"
#include "medicalrecord.h"

ReportPage::ReportPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportPage)
{
    ui->setupUi(this);
    loadPatientHistory();
}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::loadPatientHistory()
{
    // Test
    for (int i = 0; i < 3; i++)
    {
        MedicalRecord* NewHistoryItem = new MedicalRecord(this);
        ui->PatientHistoryList->addWidget(NewHistoryItem);
    }
}

#include "patientreportwidget.h"
#include "ui_patientreportwidget.h"
#include "patienthistoryitem.h"

PatientReportWidget::PatientReportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientReportWidget)
{
    ui->setupUi(this);
    loadPatientHistory();
}

PatientReportWidget::~PatientReportWidget()
{
    delete ui;
}

void PatientReportWidget::loadPatientHistory()
{
    // Test
    for (int i = 0; i < 12; i++)
    {
        PatientHistoryItem* NewHistoryItem = new PatientHistoryItem(this);
        ui->PatientHistoryList->addWidget(NewHistoryItem);
    }
}

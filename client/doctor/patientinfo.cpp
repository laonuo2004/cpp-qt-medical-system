#include "patientinfo.h"
#include "ui_patientinfo.h"
#include "diagnose.h"

PatientInfo::PatientInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInfo)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->diagnoseBtn, &QPushButton::clicked, this, &PatientInfo::diagnose);
}

PatientInfo::~PatientInfo()
{
    delete ui;
}

void PatientInfo::diagnose()
{
    Diagnose* diagnosePanel = new Diagnose(this);
    if (diagnosePanel->exec() == QDialog::Accepted)
    {

    }
    delete diagnosePanel;
}

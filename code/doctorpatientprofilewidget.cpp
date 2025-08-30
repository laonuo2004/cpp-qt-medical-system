#include "doctorpatientprofilewidget.h"
#include "ui_doctorpatientprofilewidget.h"
#include "userinfocard.h"

DoctorPatientProfileWidget::DoctorPatientProfileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorPatientProfileWidget)
{
    ui->setupUi(this);
    loadPatientInformation();
}

DoctorPatientProfileWidget::~DoctorPatientProfileWidget()
{
    delete ui;
}

void DoctorPatientProfileWidget::loadPatientInformation()
{
    // Test
    for (int i = 0; i < 3; i++)
    {
        UserInfoCard* NewPatientCard = new UserInfoCard(this, true);
        ui->PatientCards->addWidget(NewPatientCard, i / 2, i % 2);
    }
}

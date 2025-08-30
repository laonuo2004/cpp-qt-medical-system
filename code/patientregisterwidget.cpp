#include "patientregisterwidget.h"
#include "ui_patientregisterwidget.h"
#include "userinfocard.h"

PatientRegisterWidget::PatientRegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientRegisterWidget)
{
    ui->setupUi(this);
    loadDoctorInformation();
}

PatientRegisterWidget::~PatientRegisterWidget()
{
    delete ui;
}


void PatientRegisterWidget::loadDoctorInformation()
{
    // Test
    for (int i = 0; i < 11; i++)
    {
        UserInfoCard* NewDoctorCard = new UserInfoCard(this, false);
        ui->DoctorCardList->addWidget(NewDoctorCard, i / 2, i % 2);
        ui->scrollAreaWidgetContents->setGeometry(0, 0, 600, ui->scrollAreaWidgetContents->height() + 40);
    }
}

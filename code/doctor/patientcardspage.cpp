#include "patientcardspage.h"
#include "ui_patientcardspage.h"
#include "userinfocard.h"

PatientCardsPage::PatientCardsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientCardsPage)
{
    ui->setupUi(this);
    loadPatientInformation();
}

PatientCardsPage::~PatientCardsPage()
{
    delete ui;
}

void PatientCardsPage::loadPatientInformation()
{
    // Test
    for (int i = 0; i < 3; i++)
    {
        UserInfoCard* NewPatientCard = new UserInfoCard(this, true);
        ui->PatientCards->addWidget(NewPatientCard, i / 2, i % 2);
    }
}

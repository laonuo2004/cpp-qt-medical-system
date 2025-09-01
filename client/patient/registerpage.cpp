#include "registerpage.h"
#include "ui_registerpage.h"
#include "doctorcard.h"

RegisterPage::RegisterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
    loadDoctorInformation();
}

RegisterPage::~RegisterPage()
{
    delete ui;
}


void RegisterPage::loadDoctorInformation()
{
    // Test
    for (int i = 0; i < 3; i++)
    {
        DoctorCard* NewDoctorCard = new DoctorCard(this);
        ui->DoctorCardList->addWidget(NewDoctorCard, i / 2, i % 2);
    }
}

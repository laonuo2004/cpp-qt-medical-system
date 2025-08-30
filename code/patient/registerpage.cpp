#include "registerpage.h"
#include "ui_registerpage.h"
#include "userinfocard.h"

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
        UserInfoCard* NewDoctorCard = new UserInfoCard(this, false);
        ui->DoctorCardList->addWidget(NewDoctorCard, i / 2, i % 2);
    }
}

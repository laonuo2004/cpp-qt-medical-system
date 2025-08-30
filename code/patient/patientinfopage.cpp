#include "patientinfopage.h"
#include "ui_patientinfopage.h"

PatientInfoPage::PatientInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoPage)
{
    ui->setupUi(this);
    ui->PatientPhoto->setFixedSize(150, 175);
}

PatientInfoPage::~PatientInfoPage()
{
    delete ui;
}

#include "doctorinfopage.h"
#include "ui_doctorinfopage.h"

DoctorInfoPage::DoctorInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorInfoPage)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(150, 175);
}

DoctorInfoPage::~DoctorInfoPage()
{
    delete ui;
}

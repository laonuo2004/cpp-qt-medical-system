#include "doctorinfopage.h"
#include "ui_doctorinfopage.h"
#include "editdoctorinfo.h"

DoctorInfoPage::DoctorInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorInfoPage)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &DoctorInfoPage::editDoctorInfo);
}

DoctorInfoPage::~DoctorInfoPage()
{
    delete ui;
}

void DoctorInfoPage::editDoctorInfo()
{
    EditDoctorInfo* editPanel = new EditDoctorInfo(this);
    if (editPanel->exec() == QDialog::Accepted)
    {
        QList<QString> InfoSet;
        editPanel->getInfoSet(InfoSet);
        ui->nameLabel->setText(InfoSet[0]);
        ui->idLabel->setText(InfoSet[1]);
        ui->departmentLabel->setText(InfoSet[2]);
        ui->birthdayLabel->setText(InfoSet[3]);
        ui->contactLabel->setText(InfoSet[4]);
        ui->addressLabel->setText(InfoSet[5]);
        ui->DoctorPhoto->setPixmap(editPanel->getImage());
    }
    delete editPanel;
}

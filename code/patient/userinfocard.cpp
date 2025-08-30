#include "userinfocard.h"
#include "ui_userinfocard.h"
#include "patientinfo.h"
#include "doctorinfo.h"

UserInfoCard::UserInfoCard(QWidget *parent, bool isPatient) :
    QWidget(parent), ui(new Ui::UserInfoCard), m_isPatient(isPatient)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(120, 150);

    if (isPatient)
    {
        ui->ShowText_1->setText("姓名:");
        ui->ShowText_2->setText("年龄:");
        ui->ShowText_3->setText("序号:");
    }
    else
    {
        ui->ShowText_1->setText("姓名:");
        ui->ShowText_2->setText("科室:");
        ui->ShowText_3->setText("职务:");
    }

    connect(ui->DetailBtn, &QPushButton::clicked, this, &UserInfoCard::openDetailPanel);
}

UserInfoCard::~UserInfoCard()
{
    delete ui;
}

void UserInfoCard::openDetailPanel()
{
    QDialog* detailPanel;
    if (m_isPatient)
    {
        detailPanel = new PatientInfo;
    }
    else
    {
        detailPanel = new DoctorInfo;
    }
    detailPanel->exec();
    delete detailPanel;
}

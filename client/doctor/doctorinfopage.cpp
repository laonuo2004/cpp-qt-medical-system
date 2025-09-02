#include "doctorinfopage.h"
#include "ui_doctorinfopage.h"
#include "editdoctorinfo.h"
#include "uicontroller.h"

DoctorInfoPage::DoctorInfoPage(QWidget *parent, int doctorId) :
    QWidget(parent),
    ui(new Ui::DoctorInfoPage),
    m_doctorId(doctorId)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &DoctorInfoPage::editDoctorInfo);

    QVariantMap doctorInfo = UiController::get().getDoctorInfo(QString::number(m_doctorId));
    ui->nameLabel->setText(doctorInfo.value("full_name").toString());
    ui->idLabel->setText(doctorInfo.value("doctor_id").toString());
    ui->departmentLabel->setText(doctorInfo.value("department_name").toString());
    ui->birthdayLabel->setText(doctorInfo.value("age").toString());
    ui->contactLabel->setText(doctorInfo.value("phone_no").toString());
    ui->introInput->setText(doctorInfo.value("description").toString());
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
        ui->introInput->setText(InfoSet[5]);
        ui->DoctorPhoto->setPixmap(editPanel->getImage());

        QVariantMap doctorInfo;
        doctorInfo["full_name"] = InfoSet[0];
        doctorInfo["doctor_id"] = InfoSet[1];
        doctorInfo["department_name"] = InfoSet[2];
        doctorInfo["age"] = InfoSet[3];
        doctorInfo["phone_no"] = InfoSet[4];
        doctorInfo["description"] = InfoSet[5];
        UiController::get().updateDoctorInfo(QString::number(m_doctorId), doctorInfo);
    }
    delete editPanel;
}

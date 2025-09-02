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

    QVariantMap patientInfo = UiController::get().getDoctorInfo(QString::number(m_doctorId));
    ui->nameLabel->setText(patientInfo.value("full_name").toString());
    ui->idLabel->setText(patientInfo.value("doctor_id").toString());
    ui->departmentLabel->setText(patientInfo.value("department_name").toString());
    ui->birthdayLabel->setText(patientInfo.value("age").toString());
    ui->contactLabel->setText(patientInfo.value("phone_no").toString());
    // ui->addressLabel->setText(patientInfo.value("address").toString()); 地址， 在数据库中没有对应条目
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

        QVariantMap patientInfo;
        patientInfo["full_name"] = InfoSet[0];
        patientInfo["doctor_id"] = InfoSet[1];
        patientInfo["department_name"] = InfoSet[2];
        patientInfo["age"] = InfoSet[3];
        patientInfo["phone_no"] = InfoSet[4];
        // patientInfo["address"] = InfoSet[5]; 地址， 在数据库中没有对应条目
        UiController::get().updateDoctorInfo(QString::number(m_doctorId), patientInfo);
    }
    delete editPanel;
}

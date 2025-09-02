#include "patientinfopage.h"
#include "ui_patientinfopage.h"
#include "editpatientinfo.h"

#include <QTextCharFormat>

PatientInfoPage::PatientInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoPage)
{
    ui->setupUi(this);
    ui->PatientPhoto->setFixedSize(200, 250);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &PatientInfoPage::editPatientInfo);

    QTextCharFormat weekendFormat;
    weekendFormat.setForeground(QColor("#357ABD"));

    // 周六
    ui->calendarWidget->setWeekdayTextFormat(Qt::Saturday, weekendFormat);
    // 周日
    ui->calendarWidget->setWeekdayTextFormat(Qt::Sunday, weekendFormat);

}

PatientInfoPage::~PatientInfoPage()
{
    delete ui;
}

void PatientInfoPage::editPatientInfo()
{
    EditPatientInfo* editPanel = new EditPatientInfo(this);
    if (editPanel->exec() == QDialog::Accepted)
    {
        QList<QString> InfoSet;
        editPanel->getInfoSet(InfoSet);
        ui->nameLabel->setText(InfoSet[0]);
        ui->idLabel->setText(InfoSet[1]);
        ui->genderLabel->setText(InfoSet[2]);
        ui->birthdayLabel->setText(InfoSet[3]);
        ui->ageLabel->setText(InfoSet[4]);
        ui->contactLabel->setText(InfoSet[5]);
        ui->emailLabel->setText(InfoSet[6]);
        ui->emerNameLabel->setText(InfoSet[7]);
        ui->emerContactLabel->setText(InfoSet[8]);
        ui->PatientPhoto->setPixmap(editPanel->getImage());
    }
    delete editPanel;
}

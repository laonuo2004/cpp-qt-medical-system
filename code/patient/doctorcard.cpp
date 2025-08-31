#include "doctorcard.h"
#include "ui_doctorcard.h"
#include "patientinfo.h"
#include "doctorinfo.h"

DoctorCard::DoctorCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(120, 150);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &DoctorCard::openDetailPanel);
}

DoctorCard::~DoctorCard()
{
    delete ui;
}

void DoctorCard::openDetailPanel()
{
    QDialog* detailPanel = new DoctorInfo;
    detailPanel->exec();
    delete detailPanel;
}

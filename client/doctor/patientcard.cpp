#include "patientcard.h"
#include "ui_patientcard.h"
#include "patientinfo.h"

PatientCard::PatientCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientCard)
{
    ui->setupUi(this);
    ui->Photo->setFixedSize(120, 150);
    connect(ui->DetailBtn, &QPushButton::clicked, this, &PatientCard::openDetailPanel);
}

PatientCard::~PatientCard()
{
    delete ui;
}

void PatientCard::openDetailPanel()
{
    QDialog* detailPanel = new PatientInfo(this);
    detailPanel->exec();
    delete detailPanel;
}

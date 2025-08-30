#include "patientinformationwidget.h"
#include "ui_patientinformationwidget.h"

PatientInformationWidget::PatientInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInformationWidget)
{
    ui->setupUi(this);
    ui->PatientPhoto->setFixedSize(150, 175);
}

PatientInformationWidget::~PatientInformationWidget()
{
    delete ui;
}

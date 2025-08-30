#include "doctorinformationwidget.h"
#include "ui_doctorinformationwidget.h"

DoctorInformationWidget::DoctorInformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoctorInformationWidget)
{
    ui->setupUi(this);
    ui->DoctorPhoto->setFixedSize(150, 175);
}

DoctorInformationWidget::~DoctorInformationWidget()
{
    delete ui;
}

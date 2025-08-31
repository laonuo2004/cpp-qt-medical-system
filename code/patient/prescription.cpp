#include "prescription.h"
#include "ui_prescription.h"

Prescription::Prescription(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prescription)
{
    ui->setupUi(this);
}

Prescription::~Prescription()
{
    delete ui;
}

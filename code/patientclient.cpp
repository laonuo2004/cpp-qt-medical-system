#include "patientclient.h"
#include "ui_patientclient.h"

PatientClient::PatientClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatientClient)
{
    ui->setupUi(this);
}

PatientClient::~PatientClient()
{
    delete ui;
}

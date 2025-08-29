#include "doctorclient.h"
#include "ui_doctorclient.h"

DoctorClient::DoctorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DoctorClient)
{
    ui->setupUi(this);
}

DoctorClient::~DoctorClient()
{
    delete ui;
}

#include "doctorinformationdetail.h"
#include "ui_doctorinformationdetail.h"

DoctorInformationDetail::DoctorInformationDetail(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInformationDetail)
{
    ui->setupUi(this);
    chattool = new PatientChatTool();
    chattool->hide();
    ui->DoctorPhoto->setFixedSize(200, 250);
    connect(ui->ChatBtn , &QPushButton::clicked , [this](){chattool->show();} );
}

DoctorInformationDetail::~DoctorInformationDetail()
{
    delete ui;
}

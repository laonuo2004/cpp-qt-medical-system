#include "patienthistoryitem.h"
#include "ui_patienthistoryitem.h"
#include "prescription.h"

PatientHistoryItem::PatientHistoryItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientHistoryItem)
{
    ui->setupUi(this);
    payInterface = new PayInterface();
    payInterface->hide();

    setFixedSize(600, 75);
    connect(ui->payBtn , &QPushButton::clicked , [this](){payInterface->show();});
}

PatientHistoryItem::~PatientHistoryItem()
{
    delete ui;
}

void PatientHistoryItem::on_prescriptionBtn_clicked()
{
    Prescription* patientPrescription = new Prescription(this);
    patientPrescription->exec();
    delete patientPrescription;
}

#include "patientclient.h"
#include "ui_patientclient.h"
#include "patientinformationwidget.h"
#include "patientregisterwidget.h"
#include "patientreportwidget.h"

PatientClient::PatientClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatientClient)
{
    ui->setupUi(this);
    // Build up stack widget.
    m_patientInformationWidget = new PatientInformationWidget(this);
    m_patientRegisterWidget = new PatientRegisterWidget(this);
    m_patientReportWidget = new PatientReportWidget(this);
    ui->stackedWidget->addWidget(m_patientInformationWidget);
    ui->stackedWidget->addWidget(m_patientRegisterWidget);
    ui->stackedWidget->addWidget(m_patientReportWidget);
    connect(ui->InfoBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->RegisterBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->ReportBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(2);});

    // Setup side bar button size.
    ui->InfoBtn->setFixedSize(150, 50);
    ui->RegisterBtn->setFixedSize(150, 50);
    ui->ReportBtn->setFixedSize(150, 50);
}

PatientClient::~PatientClient()
{
    delete ui;
}

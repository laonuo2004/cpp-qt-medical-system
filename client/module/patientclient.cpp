#include "patientclient.h"
#include "ui_patientclient.h"
#include "patientinformationwidget.h"
#include "patientregisterwidget.h"
#include "patientreportwidget.h"
#include "engine.h"
#include "patientchattool.h"
#include "basicfunc.h"
#include "medicinesearchfunc.h"
#include "medicalhistoryfunc.h"

PatientClient::PatientClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PatientClient)
{
    ui->setupUi(this);
    // Build up stack widget.
    m_patientInformationWidget = new PatientInformationWidget(this);
    m_patientRegisterWidget = new PatientRegisterWidget(this);
    m_patientReportWidget = new PatientReportWidget(this);
    m_patientPayWidget = new PayInterface(this) ;
    m_patientChatWIdget = new ChatInterface(this) ;
    m_patientDataWidget = new APIGet(this) ;
    m_patientDrugWidget = new MedicineSearchFunc(this);
    m_patientMedicalHistoryWidget = new MedicalHistoryFunc(this);


    ui->stackedWidget->addWidget(m_patientInformationWidget);
    ui->stackedWidget->addWidget(m_patientRegisterWidget);
    ui->stackedWidget->addWidget(m_patientReportWidget);
    ui->stackedWidget->addWidget(m_patientPayWidget);
    ui->stackedWidget->addWidget(m_patientChatWIdget);
    ui->stackedWidget->addWidget(m_patientDataWidget);
    ui->stackedWidget->addWidget(m_patientDrugWidget);
    ui->stackedWidget->addWidget(m_patientMedicalHistoryWidget);


    // Setup side bar button size.
    ui->InfoBtn->setFixedSize(150, 50);
    ui->RegisterBtn->setFixedSize(150, 50);
    ui->ReportBtn->setFixedSize(150, 50);
    ui->LogoutBtn->setFixedSize(150, 50);
    ui->PayBtn->setFixedSize(150,50);
    ui->ChatBtn->setFixedSize(150,50) ;
    ui->DataBtn->setFixedSize(150,50);
    ui->DrugBtn->setFixedSize(150,50);
    ui->MedicalHistooryBtn->setFixedSize(150,50);


    connect(ui->InfoBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(0);});
    connect(ui->RegisterBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(1);});
    connect(ui->ReportBtn, &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(2);});
    connect(ui->PayBtn , &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(3);}) ;
    connect(ui->ChatBtn , &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(4);}) ;
    connect(ui->DataBtn , &QPushButton::clicked, [this](){ui->stackedWidget->setCurrentIndex(5);}) ;
    connect(ui->DrugBtn , &QPushButton::clicked , [this](){ui->stackedWidget->setCurrentIndex(6);});
    connect(ui->MedicalHistooryBtn , &QPushButton::clicked , [this](){ui->stackedWidget->setCurrentIndex(7);});
    connect(ui->LogoutBtn, &QPushButton::clicked, Engine::instance(), &Engine::Login);
}

PatientClient::~PatientClient()
{
    delete ui;
}
